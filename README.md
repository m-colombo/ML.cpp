# ML.cpp
Best way to lean how something works is implement it from scratch, so here I wanted to get a closer look to C++11 and some machine learning algorithm.

## Features
Many of the following component are quite modular, thus can be easily extended or replaced.

- Data normalization: MinMax with custom range and z-score normalization; automatic dataset normalization.
- Neuron activation: Linear, sigmoid and tanh with parameterized slope.
- Network: Arbitrary acyclic topologies can be built.
- Learning algorithm: Backpropagation is the only algorithm implemented. It provides: momentum , regularization through weight penalization, variable batch size, variable weights initialization, separated loss function for gradient and performance, pluggable custom stop criterias, pluggable learning observer to perform custom action through the iterations (e.g. to measure selection error).
- Loss: Mean squared error, mean euclidean error, misclassification ratio.
- Stop criteria: Maximum iteration threshold, training error threshold (lower and upper),
simple early stop.
- Model selection: Multiprocess grid search with hold-out cross-validation.

## Limitations and improvements
- Persistence hasn’t been handled yet. The lack of strong reflection mechanism in C++ make
serialization and deserialization quite a pain in the ass.
- Neurons representation is explicit, however a more compact representation may have led to further optimization (e.g. vectorization, no virtual methods invoke, no object allocation).
- Multiprocess grid search have been implemented quite naively with the only purpose to speed up experiments, no IPC mechanism have been implemented. So, for instance, worker results are stored in files and their collection is not handled by the system, is left to post processing
- Exp performance is the bottleneck of the whole system, some investigation on fastest sigmoid may led to great performance improvements

## Examples
- Monk's problem

## Portability
- Easy to use on all run/build enviroment. Xcode and Visual studio solution are provided

## License
MIT
