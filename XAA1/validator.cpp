//#include <iostream>
//#include <fstream>
//#include <thread>
//#include <list>
//#include "learning_backprop.h"
//#include "dataInterface.h"
//#include "validator.h"
//
//
//
//using namespace std;
//
//BPValidator::BPValidator(list<BackPropagation * > models, DataInterface * test, string out){
//    this->models = models;
//    this->test = test;
//    this->outName = out;
//}
//
//void BPValidator::worker(BackPropagation * model , int model_id){
//    double avg_qLoss = 0, avg_rate = 0, max_rate = 0;
//    for(int i = 0; i < n_trials; i++){
//        cout << "Model " << model_id << " Trial " << i << endl;
//        model->start();
//        NetworkTest t(model->network, test);
//        //Race condition ?
//        out << "Model " << model_id << " Trial " << i << ": " << t.rate << "% in " << model->n_iteration << " iteration " << endl;
//        avg_qLoss += t.avg_qLoss;
//        avg_rate += t.rate;
//        max_rate = (t.rate > max_rate ? t.rate : max_rate);
//        
//        /*
//        //LAMBDA is WORKING ? AVG weight
//        double uW = 0, oW = 0;
//        int count = 0;
//        for(NeuronsIt oIt = model->network->outs.begin(); oIt != model->network->outs.end(); oIt++){
//            for(NeuronxWeightIt wIt = ((NetNeuron*)(*oIt))->NW.begin(); wIt != ((NetNeuron *)(*oIt))->NW.end(); wIt++){
//                count++;
//                uW += wIt->second.weight;
//            }
//        }
//        for(NeuronsIt oIt = model->network->layers[1].begin(); oIt != model->network->layers[1].end(); oIt++){
//            for(NeuronxWeightIt wIt = ((NetNeuron*)(*oIt))->NW.begin(); wIt != ((NetNeuron *)(*oIt))->NW.end(); wIt++){
//                count++;
//                uW += wIt->second.weight;
//            }
//        }
//        uW = uW / count;
//        for(NeuronsIt oIt = model->network->outs.begin(); oIt != model->network->outs.end(); oIt++){
//            for(NeuronxWeightIt wIt = ((NetNeuron*)(*oIt))->NW.begin(); wIt != ((NetNeuron *)(*oIt))->NW.end(); wIt++){
//                oW += (uW - wIt->second.weight)*(uW - wIt->second.weight);
//            }
//        }
//        for(NeuronsIt oIt = model->network->layers[1].begin(); oIt != model->network->layers[1].end(); oIt++){
//            for(NeuronxWeightIt wIt = ((NetNeuron*)(*oIt))->NW.begin(); wIt != ((NetNeuron *)(*oIt))->NW.end(); wIt++){
//                oW += (uW - wIt->second.weight)*(uW - wIt->second.weight);
//            }
//        }
//        
//        out << "Model " << model_id << " Avg_weight: " << uW << " Variance: " << oW / count << endl;
//        */
//    }
//    
//    out << "@Model " << model_id << " avg_qLoss " << avg_qLoss << " max_rate " << max_rate << endl;
//    return;
//}
//
//void BPValidator::validate(){
//    list<thread> workers;
//    
//    out.open(this->outName);
//    //out << "VALIDATION ON: " << this->test->fileName << endl << endl;
//    
//    list<BackPropagation *>::iterator itM = models.begin();
//    int i = 0;
//    while(itM != models.end()){
//        out << "MODEL#" << ++i << endl << "Unit: " << (*itM)->network->layers[1].size() << "\tEta: " << (*itM)->eta << "\tAlpha: " << (*itM)->alpha << "\tLambda: " << (*itM)->lambda << "\tMax Iteration: " << (*itM)->max_iteration << endl;
//        workers.push_back(thread(&BPValidator::worker, this, (*itM), i));  
//        itM++;
//    }
//    
//    list<thread>::iterator itT = workers.begin();
//    while(itT != workers.end()){
//        (*itT).join();
//        itT++;
//    }
//    out.close();
//}
