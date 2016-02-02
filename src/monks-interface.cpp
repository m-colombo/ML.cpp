//
//  monks1-interface.cpp
//  XAA1
//
//  Created by Michele Colombo on 11/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "monks-interface.h"
#include <fstream>
#include <iostream>

using namespace std;

void Monks::LoadFile(string const& fileName){
    samples = make_shared<Samples>();
    
    ifstream data;
    data.open(fileName, ios::in);
    if(data.fail()){
        cout << "Failed to open " + fileName << endl;
        exit(1);
    }
    
    while(!data.eof()){
        auto sample = Sample();
    
        int label;
        int attribute;
    
        //One-of-K
        //Preventing to force asymptotical convergence using 0.1,0.9 as target
        data >> label;
        if(label == 0){
            sample.output.push_back(0.1);
            sample.output.push_back(0.9);
        }
        else{
            sample.output.push_back(0.9);
            sample.output.push_back(0.1);
        }
    
        //One-of-K
        for(int i = 0; i < 6; i++){
            data >> attribute;
            
            for(int j = 0; j < attribute_max_value[i]; j++){
                sample.input.push_back(j+1 == attribute ? 1 : 0);
            }
            
        }
        
        data >> sample.sample_id;
        samples->push_back(sample);
    }
    
    data.close();
    
}