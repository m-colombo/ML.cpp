
#include "feedforward_network.h"
#include <random>
#include <ctime>

using namespace std;

FeedforwardNetwork::FeedforwardNetwork(size_t inputsNumber)
  {
      for(size_t i = 0; i < inputsNumber; i++)
          inputs.push_back(make_shared<DataNeuron>());
  }

//TODO more efficient?
void FeedforwardNetwork::setSample(Sample const& sample){
    //input
    auto i1 = sample.input.begin();
    auto i2 = inputs.begin();
    for (; i1 != sample.input.end() && i2 != inputs.end(); i1++, i2++)
        (*i2)->setData(*i1);
    
    //output
    i1 = sample.output.begin();
    auto i3 = outs.begin();
    for (; i1 != sample.output.end() && i3 != outs.end(); i1++, i3++)
        (*i3)->targetOut = (*i1);
    
}

void FeedforwardNetwork::propagateInput(){
    for(auto& i : layers){
        i.second->computeNet();
        i.second->computeOut();
    }
    
    for(auto& o : outs){
        o->computeNet();
        o->computeOut();
    }
}

//TODO more efficient?
Doubles FeedforwardNetwork::getOutputs(){
    Doubles ret;
    
    for(auto& o : outs)
        ret.push_back(o->output);
    
    return ret;
}

//TODO more efficient?
Doubles FeedforwardNetwork::getTargetOuts(){
    Doubles ret;
    
    for(auto& o : outs)
        ret.push_back(o->targetOut);
    
    return ret;
}

void FeedforwardNetwork::randomizeWeights(double min, double max){
    for(auto& n : layers){
        n.second->bias.weight = rand_in_range(min, max);
        for(auto& c : n.second->NW)
            c.second.weight = rand_in_range(min, max);
    }
    for(auto& n : outs){
        n->bias.weight = rand_in_range(min, max);
        for(auto& c : n->NW)
            c.second.weight = rand_in_range(min, max);
    }
}


double FeedforwardNetwork::testDataSet(const Samples &dataset, Loss &loss){
    double accumulator = 0;
    for(auto& s : dataset){
        setSample(s);
        propagateInput();
        accumulator += loss.sampleError(getOutputs(), getTargetOuts());
    }
    return accumulator / dataset.size();
}

Results FeedforwardNetwork::evaluateDataSet(Samples const & data, SampleNormalizer const & denormalizer){
    Results rs;
    for(auto& s : data){
        setSample(s);
        propagateInput();
        
        Result r {
            denormalizer.denormalizeOutput(getOutputs()),
            s.sample_id
        };
        rs.push_back(r);
    }
    
    return rs;
}

string FeedforwardNetwork::getInfo(){
    string ret = "Feedforward ";
    ret += to_string(inputs.size());
    
    int lastKey = layers.begin()->first, counter =0;
    for(auto& p : layers){
        if(p.first != lastKey){
            ret += "x" + to_string(counter);
            counter = 0;
            lastKey = p.first;
        }
        counter++;
    }
    ret += "x" + to_string(counter) + "x" + to_string(outs.size()) + "\n";
    
    //TODO assuming homogenous hidden and homogenous output activation. Simplification which works for the current task but need to be fixed
    ret += "Hidden type: " + layers.begin()->second->getInfo() + "\n";
    ret += "Output type: " + outs[0]->getInfo() + "\n";
    
    return ret;
}


