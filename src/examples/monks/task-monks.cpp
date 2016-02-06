
#include "task-monks.h"
#include "monks-interface.h"
#include "../../learners/stop/stop_criteria.h"
#include "../../learners/backprop_learner.h"
#include "../../neurons/logistic_neuron.h"
#include "../../learners/observer/observer.h"

#include <iostream>

using namespace std;

void Learn_Monks1(std::string const& data_dir, std::string const& out_dir){
    auto dir = createAndGetRunFolder(out_dir);
    
    Monks train;
    train.LoadFile(data_dir+"/monks-1.train");
    
    auto N = FeedforwardNetwork::build<TanhNeuron, LogisticNeuron>(train.samples->at(0).input.size(), deque<int>{4}, 2);
    
    MeanSquaredError loss;
    auto BP = BackPropagation(*N, loss,loss);
    
    BP.learning_rate =  4;
    BP.momentum = 0.5;
    BP.weight_decay = 0.001;

    BP.StopCriteria = {
        make_shared<Stop::MaxIteration>(1000),
//        make_shared<Stop::TrainingConvergence>(0.01),
    };

    Monks test;
    test.LoadFile(data_dir+"/monks-1.test");
    
    BP.observers = {
        make_shared<Observer::IterationError>(dir+"/performance.csv", test.samples, make_shared<MisclassificationRatio1ofK>()),
        make_shared<Observer::IterationError>(dir+"/test_error.csv", test.samples, make_shared<MeanSquaredError>()),
        make_shared<Observer::IterationError>(dir+"/train_error.csv", train.samples, make_shared<MeanSquaredError>())
    };
    
    BP.learn(train.samples, {-0.5, 0.5});
    
    MisclassificationRatio1ofK acc;
    cout << BP.N.testDataSet(*test.samples, acc) * 100.0  << "% misclassification" << endl;
    
}


void Learn_Monks2(std::string const& data_dir, std::string const& out_dir){
    auto dir = createAndGetRunFolder(out_dir);
    
    Monks train;
    train.LoadFile(data_dir+"/monks-2.train");
    
    auto N = FeedforwardNetwork::build<TanhNeuron, LogisticNeuron>(train.samples->at(0).input.size(), deque<int>{2}, 2);


    MeanSquaredError loss;
    auto BP = BackPropagation(*N, loss,loss);
    
    BP.learning_rate =  4;
    BP.momentum = 0.5;
    BP.weight_decay = 0.001;
    
    BP.StopCriteria = {
        make_shared<Stop::MaxIteration>(1000),
//        make_shared<Stop::TrainingConvergence>(0.02),
    };

    
    Monks test;
    test.LoadFile(data_dir+"/monks-2.test");
    
    BP.observers = {
        make_shared<Observer::IterationError>(dir+"/performance.csv", test.samples, make_shared<MisclassificationRatio1ofK>()),
        make_shared<Observer::IterationError>(dir+"/test_error.csv", test.samples, make_shared<MeanSquaredError>()),
        make_shared<Observer::IterationError>(dir+"/train_error.csv", train.samples, make_shared<MeanSquaredError>())
    };
    
    BP.learn(train.samples, {-0.5, 0.5});
    
    MisclassificationRatio1ofK acc;
    cout << BP.N.testDataSet(*test.samples, acc) * 100.0  << "% misclassification" << endl;
    
}

void Learn_Monks3(std::string const& data_dir, std::string const& out_dir){
    auto dir = createAndGetRunFolder(out_dir);
    
    Monks train;
    train.LoadFile(data_dir+"/monks-3.train");
    
    auto N = FeedforwardNetwork::build<TanhNeuron, LogisticNeuron>(train.samples->at(0).input.size(), deque<int>{2}, 2);
    
    MeanSquaredError loss;
    auto BP = BackPropagation(*N, loss,loss);
    
    BP.learning_rate =  4;
    BP.momentum = 0.5;
    BP.weight_decay = 0.001;
    
    BP.StopCriteria = {
        make_shared<Stop::MaxIteration>(1000),
//        make_shared<Stop::TrainingConvergence>(0.04),
    };
    
    Monks test;
    test.LoadFile(data_dir+"/monks-3.test");
    
    BP.observers = {
        make_shared<Observer::IterationError>(dir+"/performance.csv", test.samples, make_shared<MisclassificationRatio1ofK>()),
        make_shared<Observer::IterationError>(dir+"/test_error.csv", test.samples, make_shared<MeanSquaredError>()),
        make_shared<Observer::IterationError>(dir+"/train_error.csv", train.samples, make_shared<MeanSquaredError>())
    };
    
    BP.learn(train.samples, {-0.5, 0.5});
    
    MisclassificationRatio1ofK acc;
    cout << BP.N.testDataSet(*test.samples, acc) * 100.0 << "% misclassification" << endl;
    
}
