
#ifndef NETWORK_H
#define NETWORK_H

#include "../neurons/neuron.h"
#include "../data/samples.h"
#include "../util.h"
#include "../learners/loss/loss.h"
#include "../selection/model_info.h"

#include <deque>
#include <map>
#include <algorithm>

typedef std::deque<DataNeuronSP> DataLayer;
typedef std::multimap<int,NetNeuronSP> Layers;
typedef std::deque<NetNeuronSP> OutLayer;


class FeedforwardNetwork : public ModelInfo{
 public:
    DataLayer	inputs;
    Layers	    layers;
    OutLayer	outs;
   
    FeedforwardNetwork(size_t inputsNumber);
   
    void setSample(Sample const& sample);
    void propagateInput();

    Doubles getOutputs();
    Doubles getTargetOuts();
    
    void randomizeWeights(double min, double max);
    
    // T is constrained to be an iterable of NeuronSP compatible type
    template<class T>
    void addNeuron(NetNeuronSP n, const T& inputs, Doubles const& Ws){
        
        int layer = 0;
        auto i1 = inputs.begin();
        auto i2 = Ws.begin();
        
        for(; i1 != inputs.end() && i2 != Ws.end(); i1++, i2++){
            layer = std::max(n->connectTo(*i1, Weight{*i2}), layer);
        }
        
        if(n->isOut)
            outs.push_back(n);
        else
            layers.insert(make_pair(layer,n));
    }
    
    // T is constrained to be an iterable of NeuronSP compatible type
    template<class T>
    void addNeuron(NetNeuronSP n, const T& inputs){
        Doubles ws;
        for(int i = 0; i < inputs.size(); i++)
            ws.push_back(0);
        
        addNeuron(n, inputs, ws);
    }
    
    //Util to build a multi layer network
    template<class T_HIDDEN, class T_OUT>
    static std::shared_ptr<FeedforwardNetwork> build(size_t input, std::deque<int> hiddens, int output){
        
        auto N = std::make_shared<FeedforwardNetwork>(input);
        std::deque<NetNeuronSP> previousLayer;
        
        bool isFirst = true;
        for(int hn : hiddens){
            std::deque<NetNeuronSP> layer;
        
            for(int i=0; i < hn; i++){
                auto n = std::make_shared<T_HIDDEN>();
                layer.push_back(n);
                if(isFirst)
                    N->addNeuron(n, N->inputs);
                else
                    N->addNeuron(n, previousLayer);
            }
            previousLayer = layer;
            isFirst = false;
        }
        
        for(int i = 0; i < output; i++){
            auto n = std::make_shared<T_OUT>();
            n->isOut = true;
            N->addNeuron(n, previousLayer);
        }

        return N;
    }
    
    double testDataSet(Samples const & dataset, Loss & loss);
    
    Results evaluateDataSet(Samples const & data, SampleNormalizer const & denormalizer);
    
    std::string getInfo() override;
private:
    //TODO copy constructor
    //Network represented with shared_ptr can't be copied straight forward
    FeedforwardNetwork(FeedforwardNetwork & n) = delete;
};


#endif