//
//  sample_normalization.h
//  XAA1
//
//  Created by Michele Colombo on 29/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__sample_normalization__
#define __XAA1__sample_normalization__

#include "samples.h"
#include "data_normalization.h"
#include <cassert>
#include "model_info.h"

class SampleNormalizer : public ModelInfo{
    std::deque<DataNormalizerSP> input_normalizers, output_normalizers;
    
public:
    SampleNormalizer()=default;
    
    //TODO a normalizer type per features
    template<class T_IN, class T_OUT>
    void initialize(Samples const & data, T_IN const& input_prototype, T_OUT const& output_prototype){
        
        std::deque<Doubles> input_features(data[0].input.size());
        std::deque<Doubles> output_features(data[0].output.size());
        
        //Collect feature values from samples list. (Rows list to cols list)
        for(auto& s : data){
            
            assert(s.input.size() == input_features.size() && s.output.size() == output_features.size());
            
            auto s_inp_it = s.input.begin();
            auto feature_inp_it = input_features.begin();
            for(;s_inp_it != s.input.end(); s_inp_it++, feature_inp_it++)
                feature_inp_it->push_back(*s_inp_it);
            
            auto s_out_it = s.output.begin();
            auto feature_out_it = output_features.begin();
            for(;s_out_it != s.output.end(); s_out_it++, feature_out_it++)
                feature_out_it->push_back(*s_out_it);
        }
        
        input_normalizers.clear(); output_normalizers.clear();
        for(auto& i : input_features){
            auto n = std::make_shared<T_IN>(input_prototype);
            n->setDataSource(i);
            input_normalizers.push_back(n);
        }
        
        for(auto& o : output_features){
            auto n = std::make_shared<T_OUT>(output_prototype);
            n->setDataSource(o);
            output_normalizers.push_back(n);
        }
    }
    
    //TODO extend for 1-of-k
    Doubles normalizeInput(Doubles const & input) const;
    Doubles normalizeOutput(Doubles const & output) const;
    Doubles denormalizeInput(Doubles const & input) const;
    Doubles denormalizeOutput(Doubles const & output) const;
    
    Samples normalizeDataset(Samples const & data) const;
    Samples denormalizeDataset(Samples const & data) const;
    
    std::string getInfo() override;
};
#endif /* defined(__XAA1__sample_normalization__) */
