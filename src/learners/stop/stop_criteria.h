//
//  stop_criteria.h
//  XAA1
//
//  Created by Michele Colombo on 14/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef XAA1_stop_criteria_h
#define XAA1_stop_criteria_h

#include "../backprop_learner.h"
#include <string>

namespace Stop {
    
    class MaxIteration : public Criteria{
    public:
        int iteration = 0;
        MaxIteration(int iteration) : iteration(iteration){};
        bool hasToStop(BackPropagation const& learner) override;
        std::string stopMessage() override;
        std::string getInfo() override;
    };
    
    class TrainingConvergence : public Criteria{
    public:
        double abs_error_threshold = 0;
        TrainingConvergence(double abs_error_threshold) : abs_error_threshold(abs_error_threshold) {};
        bool hasToStop(BackPropagation const& learner) override;
        std::string stopMessage() override;
        std::string getInfo() override;
    };
    
    class TrainingDivergence : public Criteria{
    public:
        double abs_error_threshold = 0;
		int initial_iteration_to_skip, check_every;
        TrainingDivergence(double abs_error_threshold, int initial_iteration_to_skip = 50, int check_every = 10) : abs_error_threshold(abs_error_threshold), initial_iteration_to_skip(initial_iteration_to_skip),check_every(check_every) {};
        bool hasToStop(BackPropagation const& learner) override;
        std::string stopMessage() override;
        std::string getInfo() override;
    };
    
    // Simple because doesn't store the best model, it just compares error
    class SimpleEarlyStop : public Criteria{
    public:
        double last_test_error = INFINITY, minimum_error_improvement;
        int iteration_to_wait, initial_iteration_to_skip = 200;
        SamplesSP test;
        std::shared_ptr<Loss> targetLoss;
        
        SimpleEarlyStop(double minimum_error_improvement, int initial_iteration_to_skip, int iteration_to_wait, SamplesSP test, std::shared_ptr<Loss> targetLoss) :
            minimum_error_improvement(minimum_error_improvement),
            iteration_to_wait(iteration_to_wait),
            initial_iteration_to_skip(initial_iteration_to_skip),
            test(test),
            targetLoss(targetLoss){}

        bool hasToStop(BackPropagation const& learner) override;
        std::string stopMessage() override;
        std::string getInfo() override;
    };
}

#endif
