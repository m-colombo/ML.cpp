//
//  loss.h
//  XAA1
//
//  Created by Michele Colombo on 17/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__loss__
#define __XAA1__loss__

#include "../../selection/model_info.h"
#include "../../data/sample_normalization.h"

class Loss : public ModelInfo {
public:
    virtual double sampleError(Doubles const & outs, Doubles const & target) = 0;
};
typedef std::shared_ptr<Loss> LossSP;

class DifLoss : public Loss{
public:
    virtual double derivativeOnOut(Doubles const & outs, Doubles const & targets, int index) = 0;
};

class DenormalizedLoss : public Loss {
    SampleNormalizer denormalizer;
    std::shared_ptr<Loss> loss;
public:
    DenormalizedLoss(SampleNormalizer const & denormalizer, std::shared_ptr<Loss> loss) :
        loss(loss),
		denormalizer(denormalizer)
    {}
    
    double sampleError(Doubles const & outs, Doubles const & targets) override {
        return loss->sampleError(denormalizer.denormalizeOutput(outs), denormalizer.denormalizeOutput(targets));
    };
    
    std::string getInfo() override {return "Denormalized " + loss->getInfo();}
};

class MeanSquaredError : public DifLoss{
public:
    MeanSquaredError()=default;
    double sampleError(Doubles const & outs, Doubles const & targets) override;
    double derivativeOnOut(Doubles const & outs, Doubles const & targets, int index) override;
    std::string getInfo() override {return "Mean squared error";}
};

class MeanEuclideanError : public DifLoss{
public:
    MeanEuclideanError()=default;
    double sampleError(Doubles const & outs, Doubles const & targets) override;
    double derivativeOnOut(Doubles const & outs, Doubles const & target, int index) override;
    std::string getInfo() override {return "Mean euclidean error";}
};

class MisclassificationRatio1ofK : public Loss{
public:
    MisclassificationRatio1ofK()=default;
    double sampleError(Doubles const & outs, Doubles const & targets) override;
    std::string getInfo() override {return "Classification precision";}
};

#endif /* defined(__XAA1__loss__) */
