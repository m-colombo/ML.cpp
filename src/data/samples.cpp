//
//  samples.cpp
//  XC
//
//  Created by Michele Colombo on 04/02/2016.
//  Copyright © 2016 Michele Colombo. All rights reserved.
//

#include "samples.h"
#include <ctime>
#include <algorithm>

//static
std::minstd_rand0 SamplesPermutation::rand = std::minstd_rand0((int) time(NULL));

//static
std::deque<size_t> SamplesPermutation::getPermutation(size_t size){
    std::deque<size_t> permutation;
    for(size_t i = 0; i < size;)
        permutation.push_back(i++);
    std::shuffle(permutation.begin(), permutation.end(), rand);
    return permutation;
}

SamplesPermutationIterator SamplesPermutation::begin(){
    return SamplesPermutationIterator(samples, permutation);
}

SamplesPermutationIterator SamplesPermutation::end(){
    SamplesPermutationIterator ret (samples, permutation);
    ret.goToEnd();
    return ret;
}

