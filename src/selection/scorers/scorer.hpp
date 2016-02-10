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
#include "../../data/sample_normalization.h"
#include <limits>

namespace Scorer{
    class IScorer : public Observer::IObserver{
    public:
        virtual double getScore()=0;
        virtual void setSampleNormalizer(SampleNormalizer const& normalizer){};
        virtual void newModel(){};
        virtual double getValidation()=0;
        virtual ~IScorer()=default;
    };
    
    class AverageBestLoss : public IScorer {
        
        SamplesSP selection, validation;
        Samples selection_n, validation_n;
        
        std::shared_ptr<Loss> loss;
        std::shared_ptr<DenormalizedLoss> dLoss;
        
        int current_trial = 0;
        int iterations_to_skip, iterations_step;

        //int moving_average_window; TODO actually implement
        double previous_error = std::numeric_limits<double>::infinity(); //Implementing a moving average of size two to smooth fluctuation
        
        
        std::map<int, std::tuple<int, double, double>> results; // Trial -> Iteration x selection x validation
        std::tuple<int, double, double> current_best = {-1, std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};    // iteration x selection x validation
        
    public:
        
        AverageBestLoss(LossSP loss, int initial_iterations_to_skip, int iterations_step, SamplesSP selection, SamplesSP validation) :
            loss(loss),
            iterations_step(iterations_step),
            iterations_to_skip(initial_iterations_to_skip),
            selection(selection),
            validation(validation)
            {}
        
        void postIteration(BackPropagation const& learner) override;
        void postLearn(BackPropagation const& learner) override;
        
        double getScore() override;
        double getValidation() override;
        void setSampleNormalizer(SampleNormalizer const& normalizer) override;
        void newModel() override;
    };
}

#endif /* scorer_hpp */
