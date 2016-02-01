//
//  samples.h
//  XAA1
//
//  Created by Michele Colombo on 18/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef XAA1_samples_h
#define XAA1_samples_h

#include <deque>
#include <string>
#include <memory>

typedef std::deque<double> Doubles;

struct Sample{
    Doubles input;
    Doubles output;
    std::string sample_id;
};

typedef std::deque<Sample> Samples;
typedef std::shared_ptr<Samples> SamplesSP;
typedef std::deque<Sample>::const_iterator SamplesIt;


struct Result{
    Doubles outputs;
    std::string sample_id;
};
typedef std::deque<Result> Results;
#endif
