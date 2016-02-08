//
//  scorer.hpp
//  XC
//
//  Created by Michele Colombo on 06/02/2016.
//  Copyright © 2016 Michele Colombo. All rights reserved.
//

#ifndef scorer_hpp
#define scorer_hpp

#include "../../learners/observer/observer.h"
#include <limits>

namespace Scorer{
    class IScorer : public Observer::IObserver{
    public:
        virtual double getScore()=0;
        virtual void reset(){};
        virtual ~IScorer()=default;
    };
    
    class AverageBestLoss : public IScorer {
        
        SamplesSP data;
        std::shared_ptr<Loss> loss;
        int current_trial = 0;
        int iterations_to_skip, iterations_step;

        //int moving_average_window; TODO actually implement
        double previous_error = std::numeric_limits<double>::infinity(); //Implementing a moving average of size two to smooth fluctuation
        
        
        std::map<int, std::pair<int, double>> results; // Trial -> Iteration x Error
        std::pair<int, double> current_best = {-1, std::numeric_limits<int>::max() };    //Iteration, error
        
    public:
        
        AverageBestLoss(SamplesSP data, std::shared_ptr<Loss> loss, int initial_iterations_to_skip, int iterations_step) :
            data(data),
            loss(loss),
            iterations_step(iterations_step),
            iterations_to_skip(initial_iterations_to_skip)
            {}
        
        void postIteration(BackPropagation const& learner) override;
        void postLearn(BackPropagation const& learner) override;
        
        double getScore() override;
        void reset() override;
    };
}

#endif /* scorer_hpp */
