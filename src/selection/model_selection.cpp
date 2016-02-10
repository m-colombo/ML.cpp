//
//  model_selection.cpp
//  XAA1
//
//  Created by Michele Colombo on 22/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "model_selection.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
#include "../learners/stop/stop_criteria.h"
#include <limits>

using namespace std;

GridSearchHoldOutCV::GridSearchHoldOutCV(Samples const & original_data, double train_ratio, string const& results_dir): results_dir(results_dir){
    
    //Randomly shuffle data and split into two set
    deque<Sample> temp(original_data.begin(), original_data.end());
    minstd_rand0 rnd((int)time(NULL));
    shuffle(temp.begin(), temp.end(), rnd);

    auto pivot = temp.begin() + (int) (temp.size() * train_ratio);
    train = make_shared<Samples>(temp.begin(), pivot);
    selection = make_shared<Samples>(pivot, temp.end());
    
}

GridSearchHoldOutCV::GridSearchHoldOutCV(std::shared_ptr<Samples> train, std::shared_ptr<Samples> selection, string const& results_dir) :
    results_dir(results_dir),
    train(train),
    selection(selection)
{}


size_t GridSearchHoldOutCV::getTotalModels(){
    return 
    normalizers.size() *
    topologies.size() *
    gradientLoss.size() *
    initialWeights.size() *
    learningRates.size() *
    momentums.size() *
    weightDecays.size() *
    batchRatios.size();
}

namespace Observer {

	class Score : public IObserver {

		SamplesSP data;
		std::shared_ptr<Loss> loss;
		int iterations = 1;

	public:
		pair<int, double> best;

		Score(SamplesSP data, std::shared_ptr<Loss> loss, int iterations) : data(data), loss(loss), iterations(iterations) {}

		void postIteration(BackPropagation const& learner) override {
			if (learner.current_iteration % iterations == 0) {
				double er = learner.N.testDataSet(*data, *loss);
				if (learner.current_iteration == iterations || best.second > er)
					best = {learner.current_iteration, er};
			}
		}
	};

}


void GridSearchHoldOutCV::search(int process_number,int process_total){
	int model = 0;
	size_t total = getTotalModels();

    std::multimap<double, std::pair<int, double>> process_results; //Selection -> model_id x validation
    
    //Search info
    if(process_number == 0){
        ofstream info(results_dir+"/search_info.txt");
        info << "#Total models: " << total << endl;
        info << "#Target loss: " << targetLoss->getInfo() << endl;
        info << "#Stop criteria: " << endl;
        for(auto& s : stopCriteria)
            info << s->getInfo() << endl;

        info.close();
    }

    for(auto& norm : normalizers){
        auto train_n = make_shared<Samples>(norm.normalizeDataset(*train));
        auto selection_n = make_shared<Samples>(norm.normalizeDataset(*selection));
        auto nLoss = make_shared<DenormalizedLoss>(norm, targetLoss);
        
        modelScoreMethod->setSampleNormalizer(norm);
        
        for(auto& network : topologies){
            for(auto& gLoss : gradientLoss){
                for(auto& wi : initialWeights){
                    for(auto eta : learningRates){
                    for(auto alpha : momentums){
                    for(auto lamda : weightDecays){
                    for(auto batchR : batchRatios){
                        
                        //Easiest way to parallelize
                        if(++model % process_total != process_number)
                            continue;
                        
                        auto model_dir = createSubFolder(results_dir, "model_"+ to_string(model));
                        
                        ofstream info(model_dir+"/info.txt");
                        info << "#Normalization\n" << norm.getInfo() << endl;
                        info << "#Network\n" << network->getInfo() << endl;
                        info << "#Learning: backpropagation" << endl; //TODO hardcoded since is the only available so far
                        info << "Gradient loss: " << gLoss->getInfo() << endl;
                        info << "Initial weight range: [" << to_string(wi.first) << "," << to_string(wi.second) << "]\n";
                        info << "Learning rate: " << to_string(eta) << endl;
                        info << "Momentum: " << to_string(alpha) << endl;
                        info << "Weight decay: " << to_string(lamda) << endl;
                        info << "Batch size: " << to_string(round(train->size() * batchR)) << endl;
                        info << "Training size: " << to_string(train->size()) << endl;
                        info << "Selection size: " << to_string(selection->size()) << endl;
                        info.close();
                
                        modelScoreMethod->newModel();
                        for(int trial = 1; trial <= trials; trial++){
                            auto trial_dir = createSubFolder(model_dir, "trial_"+to_string(trial));
                            auto learner = BackPropagation(*network, *gLoss, *nLoss, batchR);
                            
                            learner.learning_rate = eta;
                            learner.momentum = alpha;
                            learner.weight_decay = lamda;
                            
                            if(stopCriteriaGen)
                                learner.StopCriteria = stopCriteriaGen(selection_n,nLoss);
                            else
                                learner.StopCriteria = stopCriteria;
                            
							auto score = make_shared<Observer::Score>(selection_n, nLoss, 10);

							learner.observers = { modelScoreMethod };

                            learner.learn(train_n, wi);
                            
                            ofstream stopInfo(trial_dir+"/stop.txt");
                            stopInfo << learner.Stopper->stopMessage() << endl;
							stopInfo << "Iteration " << learner.current_iteration << endl;
							stopInfo << "Best: " << score->best.second << " at " << score->best.first << endl;
                            stopInfo.close();
                        }
                        process_results.insert({modelScoreMethod->getScore(),{model, modelScoreMethod->getValidation()}});

						cout << "Testing model " << model << "\t of " << total << "\tBest: " << process_results.begin()->first << "\t" << process_results.begin()->second.first << endl;
                    }}}}
                }
            }
        }
    }
    
    //Write into a file the best results every process has found
    ofstream resultInfo(results_dir + "/results_" + to_string(process_number) + ".csv");
    for(auto& p : process_results)
        resultInfo << p.first << "," << p.second.first <<  "," << p.second.second << endl;
    resultInfo.close();
    
}