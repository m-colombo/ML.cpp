//
//  stop_criteria.cpp
//  XAA1
//
//  Created by Michele Colombo on 14/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "stop_criteria.h"
#include <sstream>
#include <iostream>

using namespace Stop;

bool MaxIteration::hasToStop(BackPropagation const& learner){
    return learner.current_iteration > iteration;
}
std::string MaxIteration::stopMessage(){
    return "Max iteration reached: " + std::to_string(iteration);
}

std::string MaxIteration::getInfo(){
    return "Max iteration " + std::to_string(iteration);
}

bool TrainingConvergence::hasToStop(BackPropagation const& learner){
    return std::abs(learner.iteration_error) < abs_error_threshold;
}

std::string TrainingConvergence::stopMessage(){
    return "Absolute training error under the threshold: " + std::to_string(abs_error_threshold);
}

std::string TrainingConvergence::getInfo(){
    return "Training convergence threshold " + std::to_string(abs_error_threshold);
}


bool TrainingDivergence::hasToStop(BackPropagation const& learner){
    return std::isnan(learner.iteration_error) || 
		(learner.current_iteration > initial_iteration_to_skip && 
			learner.current_iteration % check_every == 0 &&
			std::abs(learner.iteration_error) > abs_error_threshold 
			);
}

std::string TrainingDivergence::stopMessage(){
    return "Error above the divergence threshold : " + std::to_string(abs_error_threshold);
}

std::string TrainingDivergence::getInfo(){
    return "Training divergence threshold " + std::to_string(abs_error_threshold) + " after " + std::to_string(initial_iteration_to_skip) + "th iteration";
}

bool SimpleEarlyStop::hasToStop(const BackPropagation &learner){
    if(learner.current_iteration > initial_iteration_to_skip && learner.current_iteration % iteration_to_wait == 0){
        double e = learner.N.testDataSet(*test, *targetLoss);
        if(last_test_error - e < minimum_error_improvement)
            return true;
        last_test_error = e;
    }
    return false;
}

std::string SimpleEarlyStop::getInfo(){
    return "Simple early stop after " + std::to_string(initial_iteration_to_skip) + "th iteration, every "+ std::to_string(iteration_to_wait) + " improve at least " + std::to_string(minimum_error_improvement) + " using " + targetLoss->getInfo();
}

std::string SimpleEarlyStop::stopMessage(){
        return "SimpleEarlyStop test error improved less than " + std::to_string(minimum_error_improvement) + " in " +  std::to_string(iteration_to_wait) + " iteration using " + targetLoss->getInfo();
}
