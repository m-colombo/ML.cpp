//
//  logistic_neuron.h
//  XAA1
//
//  Created by Michele Colombo on 11/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "logistic_neuron.h"
#include <cmath>

double LogisticNeuron::activationFunction(double x){
    return 1 / (1 + std::exp(-slope * x));
}

double LogisticNeuron::activationDerivative(double x){
    double o = activationFunction(x);
    return slope * o * (1 - o);
}

double TanhNeuron::activationFunction(double x){
    return std::tanh(slope * x / 2);
}

double TanhNeuron::activationDerivative(double x){
    double o = activationFunction(x);
    return slope * (1 - o*o) / 2;
}

