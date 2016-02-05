
#include "backprop_learner.h"
#include <iostream>
#include <algorithm>

using namespace std;

BackPropagation::BackPropagation(FeedforwardNetwork& network, DifLoss& gradientLoss, Loss& taskLoss, double batchRatio) : N(network),gradientLoss(gradientLoss), taskLoss(taskLoss),batchRatio(batchRatio) {
}

void BackPropagation::learn(std::shared_ptr<Samples>& train, std::pair<double,double> initial_weight_range){

    TR = train;
    setBatchRatio(batchRatio);
    current_iteration = 0;
    N.randomizeWeights(initial_weight_range.first, initial_weight_range.second);

    SamplesPermutation permutation(TR);
    auto TR_it = permutation.begin();
    
    for(auto& s : StopCriteria)
        s->reset();
    
    for(auto& o : observers)
        o->preLearn(*this);
    
    do{
        iteration(TR_it);

        //For debug purposes
        //cout << "Iteration " << current_iteration << " Error " << iteration_error << endl;
        
        for(auto& o : observers)
            o->postIteration(*this);
        
    }while (!hasToStop());
    
    for(auto& o : observers)
        o->postLearn(*this);
}

void BackPropagation::iteration(SamplesPermutationIterator & TR_it){

    current_iteration++;
    iteration_error = 0;
    
    //Samples batch evaluation
    for(int i = 0; i < batchSize; i++){
        
        N.setSample(*TR_it);
        N.propagateInput();
        
        //Reset hidden errors
        for(auto &n : N.layers)
            n.second->delta = 0;
        
        //Compute out errors
        auto outs = N.getOutputs();
        auto targets = N.getTargetOuts();
        iteration_error += taskLoss.sampleError(outs, targets);
        
        //Compute outs error correction and backpropagate to the layer before
        int o_idx=0;
        for(auto &o : N.outs){
            o->delta = gradientLoss.derivativeOnOut(outs, targets, o_idx++) * o->activationDerivative(o->net);
            backPropagateError(o);
        }
        
        //Hidden layers back propagation
        for(auto h = N.layers.rbegin(); h != N.layers.rend(); h++){
            auto &n = h->second;
            
            //The sum of propagated error terms is already here thanks to visit order
            n->delta *= n->activationDerivative(n->net);
            backPropagateError(n);
        }
        
        //Next sample
        TR_it++;
        if (TR_it.isEnd())
            TR_it.goToBegin();
    }
    iteration_error /= batchSize;
    
    //Apply weights correction
    for(auto const &o : N.outs)
        applyWeightsCorrection(o);
    for(auto const &h : N.layers)  //No matter which order, there are no dependencies
        applyWeightsCorrection(h.second);

}

void BackPropagation::backPropagateError(NetNeuronSP const & n){
    //Backpropagate error on every incoming connection and neuron
    for(auto& c : n->NW){
        //Back propagate delta correction term
        c.first->delta += (c.second.weight * n->delta);
        //Add current sample contribute for next weight update. (Here delta refers to delta weight)
        c.second.nextDelta += learning_rate * n->delta * c.first->output;
    }
    //Propagate bias error
    n->bias.nextDelta += learning_rate * n->delta * 1;
}

void BackPropagation::applyWeightsCorrection(NetNeuronSP const & n){
    //Update every incoming connection
    for(auto& c : n->NW){
        
        c.second.lastDelta =
            c.second.nextDelta  / batchSize     //Normalize summed correction on batch size
            + momentum * c.second.lastDelta     //Add momentum using previous update
            + 2* weight_decay * c.second.weight;//Weight penalization term
        
        c.second.weight -= c.second.lastDelta;  //Actually update weight
        c.second.nextDelta = 0;                 //Reset accumulator
    }
    //Bias update
    n->bias.lastDelta = n->bias.nextDelta  /batchSize + momentum * n->bias.lastDelta;
    n->bias.weight -= n->bias.lastDelta;
    n->bias.nextDelta = 0;
}

bool BackPropagation::hasToStop(){

    for(auto& stop : StopCriteria){
        if(stop->hasToStop(*this)){
            Stopper = stop;
            
            //For debug purposes
            //cout << "Stop criteria met: " << stop->stopMessage() << endl;
            return true;
        }
    }
    
    return false;
}

