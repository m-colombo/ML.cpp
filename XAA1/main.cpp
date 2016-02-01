//
//  main.cpp
//  XAA1
//
//  Created by Michele Colombo on 10/12/2015.
//  Copyright (c) 2015 Michele Colombo. All rights reserved.
//

#include "task-monks.h"

#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

    
    if(argc < 4){
        cout << "Invalid number or argument, required: task data_path out_path [pnum] [ptot]" << endl;
        return 1;
    }
    
    string
        task = argv[1],
		data_path	= argv[2],	
		out_path	= argv[3];
    
    int process_number = argc >= 5 ? atoi(argv[4]) : 0;
    int process_total = argc >= 5 ? atoi(argv[5]) : 1;

	srand((unsigned int)time(NULL));

    if(task == "monks1")
        Learn_Monks1(data_path, out_path);
    else if(task == "monks2")
        Learn_Monks2(data_path, out_path);
    else if(task == "monks3")
        Learn_Monks3(data_path, out_path);

    else{
        cout << "Invalid task, supported: monks1, monks2, monks3" << endl;
		return 1;
    }

    return 0;
}