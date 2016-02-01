//
//  observer.cpp
//  XAA1
//
//  Created by Michele Colombo on 08/01/2016.
//  Copyright (c) 2016 Michele Colombo. All rights reserved.
//

#include "observer.h"
using namespace std;
    
//#include <iostream>
void Observer::IterationError::postIteration(BackPropagation const& learner){
    outf << learner.current_iteration << "," << learner.N.testDataSet(*data, *loss) << endl;

	//cout << learner.current_iteration << "," << learner.N.testDataSet(*data, *loss) << endl;
}