//
//  loss.cpp
//  XAA1
//
//  Created by Michele Colombo on 17/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "loss.h"
#include <cmath>
#include <algorithm>

double MeanSquaredError::sampleError(Doubles const & outs, Doubles const & targets){
    double sum = 0;
    
    auto o = outs.begin(), t = targets.begin();
    for(; t != targets.end(); t++, o++)
        sum += (*t - *o)*(*t - *o);
    return sum / 2;
}

double MeanSquaredError::derivativeOnOut(Doubles const & outs, Doubles const & targets, int index){
    return -(targets[index] - outs[index]);
}

double MeanEuclideanError::sampleError(Doubles const & outs, Doubles const & targets){
    long double sum = 0;
    
    auto o = outs.begin(), t = targets.begin();
    for(; t != targets.end(); t++, o++)
        sum += (*t-*o)*(*t-*o);
    
    return static_cast<double>(std::sqrt(sum));
}

//TODO avoid recomputing sample error for each out
double MeanEuclideanError::derivativeOnOut(Doubles const & outs, Doubles const & target, int index){
    
    return - (target[index] - outs[index]) * sampleError(outs,target);
}

double MisclassificationRatio1ofK::sampleError(const Doubles &outs, const Doubles &targets){
    auto t_idx_max = std::distance(targets.begin(), max_element(targets.begin(), targets.end()));
    auto o_idx_max = std::distance(outs.begin(), max_element(outs.begin(), outs.end()));
    
    return t_idx_max == o_idx_max ? 0.0 : 1.0;
}