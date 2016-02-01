#include "neuron.h"



int NetNeuron::connectTo(NeuronSP neuron, Weight weight){
    NW[neuron] = weight;
    
    if(neuron->layer >= layer)
        layer = neuron->layer +1;
    
    return layer;
}

double NetNeuron::computeNet(){
    net = 0;
    for(auto const& it : NW){
        net += (it.first->output) * (it.second.weight);
    }
    net += bias.weight;
    return net;
}

double NetNeuron::computeOut(){
  output = activationFunction(net);
  return output;
}