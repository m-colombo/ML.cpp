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
            double er = learner.N.testDataSet(*data, *loss);
            if(previous_error != std::numeric_limits<double>::infinity()){
                double avg = (er + previous_error) / 2.0;
                if (current_best.second > avg)
                    current_best = {it, avg};
            }
            previous_error = er;
        }
    }

    void AverageBestLoss::postLearn(const BackPropagation &learner){
        results[current_trial++] = current_best;
        current_best = {-1, std::numeric_limits<int>::max() };
    }
    
    double AverageBestLoss::getScore(){
        double avg = 0;
        for(auto& r : results)
            avg += r.second.second;
        return avg/results.size();
    }
    
    void AverageBestLoss::reset(){
        current_trial = 0;
        results = decltype(results)();
        current_best = {-1, std::numeric_limits<int>::max()};
    }
}