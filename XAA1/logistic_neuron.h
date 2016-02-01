//
//  logistic_neuron.h
//  XAA1
//
//  Created by Michele Colombo on 11/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__logistic_neuron__
#define __XAA1__logistic_neuron__

#include "neuron.h"

class LogisticNeuron : public NetNeuron{
public:
    double slope = 1;
    
    LogisticNeuron()=default;
    
    double activationFunction(double x) override;
    double activationDerivative(double x) override;
    
    std::string getInfo() override {return "Logistic slope " + std::to_string(slope);}
};

class TanhNeuron : public NetNeuron{
public:
    double slope = 1;
    
    TanhNeuron()=default;
    
    double activationFunction(double x) override;
    double activationDerivative(double x) override;
    std::string getInfo() override {return "Tanh slope " + std::to_string(slope);}
};

#endif /* defined(__XAA1__logistic_neuron__) */
