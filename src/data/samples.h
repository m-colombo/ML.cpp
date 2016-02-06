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

#include <iterator>
#include <random>

typedef std::deque<double> Doubles;

struct Sample{
    Doubles input;
    Doubles output;
    std::string sample_id;
};


typedef std::deque<Sample> Samples;
typedef std::shared_ptr<Samples> SamplesSP;


class SamplesPermutationIterator;
class SamplesPermutation {
    static std::minstd_rand0 rand;
    SamplesSP samples;
    std::deque<size_t> permutation;
    
public:
    SamplesPermutation(SamplesSP const & samples) : samples(samples), permutation(getPermutation(samples->size())){}
                                                                                  
    static std::deque<size_t> getPermutation(size_t size);
    SamplesPermutationIterator begin();
    SamplesPermutationIterator end();
    
};

class SamplesPermutationIterator : public std::iterator<std::forward_iterator_tag, const Sample>{
    size_t idx = 0;
    std::deque<size_t>& permutation;
    SamplesSP const & samples;

public:
    SamplesPermutationIterator(SamplesSP const & samples, std::deque<size_t> & permutation) : samples(samples), permutation(permutation){};
    SamplesPermutationIterator(const SamplesPermutationIterator& it) = default;
    
    SamplesPermutationIterator& operator++() {++idx;return *this;}
    SamplesPermutationIterator operator++(int) {SamplesPermutationIterator tmp(*this); ++*this; return tmp;}
    
    bool isEnd() const {return idx >= permutation.size();}
    void goToEnd(){ idx = permutation.size();}
    void goToBegin(){ idx = 0; }
    
    bool operator==(const SamplesPermutationIterator& it) {return (idx == it.idx || (isEnd() && it.isEnd())) && &permutation == &(it.permutation);}
    bool operator!=(const SamplesPermutationIterator& it) {return !operator==(it);}
    Sample const& operator*() {return (*samples)[permutation[idx]];}
    
    
};


struct Result{
    Doubles outputs;
    std::string sample_id;
};
typedef std::deque<Result> Results;
#endif
