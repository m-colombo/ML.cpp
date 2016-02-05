#ifndef _BACKPROP_H
#define _BACKPROP_H

#include "feedforward_network.h"
#include "loss.h"

#include <limits>


class BackPropagation;

namespace Stop {
    class Criteria : public ModelInfo{
    public:
        virtual bool hasToStop(BackPropagation const& learner) = 0;
        virtual std::string stopMessage() = 0;
        
        //Allows to reuse criteria objects across different runs
        virtual void reset() {};
    };
}

namespace Observer {
    class IObserver{
    public:
        virtual void preLearn(BackPropagation const& learner){};
        virtual void postLearn(BackPropagation const& learner){};
        virtual void postIteration(BackPropagation const& learner){};
    };
}

class BackPropagation{
public:
    int current_iteration = 0;
    double iteration_error = std::numeric_limits<double>::infinity() ;
    
    Loss &taskLoss;
    DifLoss &gradientLoss;
    
    FeedforwardNetwork &N;
    double learning_rate = 0.01, momentum = 0, weight_decay = 0;
    
    BackPropagation(FeedforwardNetwork & network, DifLoss& gradientLoss, Loss& taskLoss, double batchRatio = 1);

    void learn(std::shared_ptr<Samples>& train, std::pair<double,double> initial_weight_range = std::pair<double,double>(-0.25, 0.25));

    
    bool hasToStop();
    std::deque<std::shared_ptr<Stop::Criteria>> StopCriteria;
    std::shared_ptr<Stop::Criteria> Stopper;    //Stop criteria which made learning stops
    std::deque<std::shared_ptr<Observer::IObserver>> observers;
    
    void setBatchRatio(double r){ batchSize = (size_t)( TR->size() * std::min(1.0,r)); }
protected:
    double batchRatio = 1;
    
    size_t batchSize;
    std::shared_ptr<Samples> TR;    //TODO const constraints
    
    void iteration(SamplesPermutationIterator & TR_it);
    
    void backPropagateError(NetNeuronSP const &n);
    void applyWeightsCorrection(NetNeuronSP const &n);
};

#endif