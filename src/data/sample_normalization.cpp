//
//  sample_normalization.cpp
//  XAA1
//
//  Created by Michele Colombo on 29/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "sample_normalization.h"

//Util function which maps a sample to normalizers list
Doubles applyNormalization(Doubles const & data, std::deque<DataNormalizerSP> const & normalizers){
    assert(data.size() == normalizers.size());
    
    Doubles ret;
    
    auto d = data.begin(); auto n = normalizers.begin();
    for(; d != data.end(); d++, n++)
        ret.push_back((*n)->normalize(*d));
    
    return ret;
}

//Util function which maps a sample to denormalizers list
Doubles applyDenormalization(Doubles const & data, std::deque<DataNormalizerSP> const & normalizers){
    assert(data.size() == normalizers.size());
    
    Doubles ret;
    
    auto d = data.begin(); auto n = normalizers.begin();
    for(; d != data.end(); d++, n++)
        ret.push_back((*n)->denormalize(*d));
    
    return ret;
}

Doubles SampleNormalizer::normalizeInput(const Doubles &input) const{
    return applyNormalization(input, input_normalizers);
}

Doubles SampleNormalizer::denormalizeInput(const Doubles &input) const{
    return applyDenormalization(input, input_normalizers);
}

Doubles SampleNormalizer::normalizeOutput(const Doubles &output) const{
    return applyNormalization(output, output_normalizers);
}

Doubles SampleNormalizer::denormalizeOutput(const Doubles &output) const{
    return applyDenormalization(output, output_normalizers);
}

Samples SampleNormalizer::normalizeDataset(Samples const& data) const{
    Samples ret;
    for(auto& s : data){
        ret.push_back(Sample{
            normalizeInput(s.input),
            normalizeOutput(s.output),
            s.sample_id
        });
    }
    return ret;
}

Samples SampleNormalizer::denormalizeDataset(Samples const& data) const{
    Samples ret;
    for(auto& s : data){
        ret.push_back(Sample{
            denormalizeInput(s.input),
            denormalizeOutput(s.output),
            s.sample_id
        });
    }
    return ret;
}

std::string SampleNormalizer::getInfo(){
    //Assuming homogenous input and homogenous output normalization
    return "Input: " + input_normalizers[0]->getInfo() + "\nOutput: " + output_normalizers[0]->getInfo() + "\n";
}