//
//  monks-interface.h
//  XAA1
//
//  Created by Michele Colombo on 11/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__monks_interface__
#define __XAA1__monks_interface__

#include "samples.h"

class Monks{
public:
    Monks()=default;
    std::shared_ptr<Samples> samples;
    void LoadFile(std::string const& fileName);
private:
    const int attribute_max_value[6] = {3,3,2,3,4,2};
};

#endif /* defined(__XAA1__monks_interface__) */
