//
//  data_normalization.h
//  XAA1
//
//  Created by Michele Colombo on 16/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__data_normalization__
#define __XAA1__data_normalization__

#include "samples.h"
#include "../selection/model_info.h"

//TODO extend for 1-of-k
class DataNormalizer : public ModelInfo{
public:
    virtual double normalize(double value) = 0;
    virtual double denormalize(double value) = 0;
    virtual void setDataSource(Doubles const& values)=0;
    
    virtual ~DataNormalizer()=default;
};
typedef std::shared_ptr<DataNormalizer> DataNormalizerSP;


class MinMaxNormalizer : public DataNormalizer{
public:
    MinMaxNormalizer(double min_target, double max_target);
    
    void setDataSource(Doubles const& values) override;
    double normalize(double value) override;
    double denormalize(double value) override;
    
    std::string getInfo() override {return "MinMax["+std::to_string(min_target)+","+std::to_string(max_target)+"]";}
    
    
protected:
    double min_target, max_target;
    double min_value, max_value;
};

class IdentityNormalizer : public DataNormalizer{
public:
    
    IdentityNormalizer(){};
    
    void setDataSource(Doubles const& values) override {}
    double normalize(double value) override {return value;}
    double denormalize(double value) override {return value;}

    std::string getInfo() override {return "Identity";}
};

class ZNormalizer : public DataNormalizer{
public:
    ZNormalizer()=default;
    void setDataSource(Doubles const& values) override;
    double normalize(double value) override { return (value - mean) / sd;}
    double denormalize(double value) override { return (value * sd) + mean;}
    std::string getInfo() override {return "Z";}
protected:
    double mean, sd;
};

#endif /* defined(__XAA1__data_normalization__) */
