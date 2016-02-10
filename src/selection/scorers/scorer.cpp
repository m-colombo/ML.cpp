//
//  scorer.cpp
//  XC
//
//  Created by Michele Colombo on 06/02/2016.
//  Copyright © 2016 Michele Colombo. All rights reserved.
//

#include "scorer.hpp"
#include <algorithm>

namespace Scorer{

    
    void AverageBestLoss::postIteration(const BackPropagation &learner){
        int it = learner.current_iteration;
        if (it > iterations_to_skip && it % iterations_step == 0) {
            double er = learner.N.testDataSet(selection_n, *dLoss);
            
            if(previous_error != std::numeric_limits<double>::infinity()){
                double avg = (er + previous_error) / 2.0;
                if (std::get<1>(current_best) > avg)
                    current_best = std::make_tuple(it, avg, learner.N.testDataSet(validation_n, *dLoss));
            }
            previous_error = er;
        }
    }

    void AverageBestLoss::postLearn(const BackPropagation &learner){
        results[current_trial++] = current_best;
        current_best = std::make_tuple(-1, std::numeric_limits<double>::max(),std::numeric_limits<double>::max());
    }
    
    double AverageBestLoss::getScore(){
        double avg = 0;
        for(auto& r : results)
            avg += std::get<1>(r.second);
        return avg/results.size();
    }
    
    double AverageBestLoss::getValidation(){
        double avg = 0;
        for(auto& r : results)
            avg += std::get<2>(r.second);
        return avg/results.size();
    }
    
    void AverageBestLoss::newModel(){
        current_trial = 0;
        results = decltype(results)();
        current_best = std::make_tuple(-1, std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    }
    
    void AverageBestLoss::setSampleNormalizer(const SampleNormalizer &normalizer){
        selection_n = normalizer.normalizeDataset(*selection);
        validation_n = normalizer.normalizeDataset(*validation);
        dLoss = std::make_shared<DenormalizedLoss>(normalizer, loss);
    }
}