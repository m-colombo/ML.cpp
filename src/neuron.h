#ifndef NEURON_H
#define NEURON_H

#include <unordered_map>
#include <memory>
#include "model_info.h"

class Neuron {
public:
    double net      = 0;
    double output	= 0;
    int 	 layer	= 0;
    
    //Error correction term: [d\d(out) Error(out)] * [d/dx activation(x)]
    double delta	= 0;
    
    bool isOut = false;
    double targetOut = 0;
    
    Neuron()=default;
    virtual ~Neuron()=default;
    
    virtual double computeNet(){return 0;}
    virtual double computeOut(){return 0;}
};

struct Weight{
    double weight;      //Current value
    double lastDelta;   //Last update needed for momentum
    double nextDelta;   //Accumulator for batch updates
    
    Weight(double w = 0.0) : weight(w), lastDelta(0.0), nextDelta(0.0) { }
};

typedef std::shared_ptr<Neuron> NeuronSP;
typedef std::unordered_map<NeuronSP, Weight> NeuronSPxWeight;

class DataNeuron : public Neuron{
public:
    DataNeuron()=default;
    void setData(double data){output=data;}
    double computeOut() override {return output;}
};

typedef std::shared_ptr<DataNeuron> DataNeuronSP;

class NetNeuron : public Neuron, public ModelInfo{
public:
    Weight bias;
    NeuronSPxWeight NW;     //input connections
    
    //Return the new this.layer
    virtual int connectTo(NeuronSP neuron, Weight weight);
    
    double computeNet() override;
    double computeOut() override;
    
    virtual double activationFunction(double x) = 0;
    virtual double activationDerivative(double x) = 0;
};

typedef std::shared_ptr<NetNeuron> NetNeuronSP;

//Used for regression output
class LinearNeuron : public NetNeuron{
public:
    
    double activationFunction(double x) override{return x;}
    double activationDerivative(double x) override {return 1;};
    std::string getInfo() override {return "Identity";}
};

#endif