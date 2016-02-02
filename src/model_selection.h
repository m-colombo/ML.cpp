//
//  model_selection.h
//  XAA1
//
//  Created by Michele Colombo on 22/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__model_selection__
#define __XAA1__model_selection__

#include "samples.h"
#include "backprop_learner.h"
#include "sample_normalization.h"

#include<functional>

class GridSearchHoldOutCV{
public:
    std::string results_dir;
    
    //Search parameters
    std::shared_ptr<Loss> targetLoss;
    int trials = 1;
    
    //When stop criteria depends on the normalization methods they can be generated
    std::function<std::deque<std::shared_ptr<Stop::Criteria>>(SamplesSP normalized_selection, std::shared_ptr<DenormalizedLoss> dloss)> stopCriteriaGen;
    std::deque<std::shared_ptr<Stop::Criteria>> stopCriteria;
    
    //TODO allow observers
    
    //Hyper parameters to select
    std::deque<SampleNormalizer> normalizers;
    std::deque<std::shared_ptr<FeedforwardNetwork>> topologies;
    std::deque<std::pair<double,double>> initialWeights;
    std::deque<std::shared_ptr<DifLoss>> gradientLoss;
    std::deque<double> batchRatios, learningRates, momentums, weightDecays;
    
    //Data to use
    SamplesSP train, selection;
    
    //Split data in training and selection
    GridSearchHoldOutCV(Samples const& original_data, double train_ratio, std::string const& results_dir);
    
    GridSearchHoldOutCV(std::shared_ptr<Samples> train, std::shared_ptr<Samples> selection, std::string const& results_dir);
    
    template<class T_LOSS>
    void setTargetLoss(T_LOSS const & loss){
        targetLoss = std::make_shared<T_LOSS>(loss);
    }
    
    template<class T_IN, class T_OUT>
    void addNormalizer(T_IN const& input_protoype, T_OUT const& output_prototype){
        SampleNormalizer sn;
        sn.initialize(*train, input_protoype, output_prototype);
        normalizers.push_back(sn);
    }
    
    template<class T_HIDDEN, class T_OUT>
    void addTopologies  (std::deque<std::deque<int>> const& hidden_layers){
        for(auto& h : hidden_layers)
            topologies.push_back(FeedforwardNetwork::build<T_HIDDEN, T_OUT>(train->at(0).input.size(), h, (int)train->at(0).output.size()));
    }
    
    template<class T_LOSS>
    void addGradientLoss(){
        gradientLoss.push_back(std::make_shared<T_LOSS>());
    }
    
    size_t getTotalModels();
    
    void search(int process_number=0,int process_total = 1);

};

#endif /* defined(__XAA1__model_selection__) */
