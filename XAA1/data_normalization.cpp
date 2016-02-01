//
//  data_normalization.cpp
//  XAA1
//
//  Created by Michele Colombo on 16/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "data_normalization.h"
#include <cmath>
#include <numeric>
#include <algorithm>

MinMaxNormalizer::MinMaxNormalizer(double min_target, double max_target){
    this->min_target = min_target;
    this->max_target = max_target;
}

void MinMaxNormalizer::setDataSource(const Doubles &values){
    auto mm = std::minmax_element(values.begin(), values.end());
    this->min_value = *mm.first;
    this->max_value = *mm.second;
}

double MinMaxNormalizer::normalize(double value){
    return min_target + (max_target - min_target) * (value - min_value) / (max_value - min_value);
}

double MinMaxNormalizer::denormalize(double value){
    return (value - min_target) * (max_value - min_value) / (max_target - min_target) + min_value;
}

void ZNormalizer::setDataSource(const Doubles &values){
    double sum = std::accumulate(values.begin(), values.end(), 0);
    mean = sum / (double)values.size();
    
    double accum = 0.0;
    std::for_each (std::begin(values), std::end(values), [&](const double d) {
        accum += (d - mean) * (d - mean);
    });
    sd = std::sqrt(accum/(double)values.size());
}
