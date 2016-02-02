//
//  observer.h
//  XAA1
//
//  Created by Michele Colombo on 08/01/2016.
//  Copyright (c) 2016 Michele Colombo. All rights reserved.
//

#ifndef __XAA1__observer__
#define __XAA1__observer__

#include "backprop_learner.h"
#include <fstream>


namespace Observer{
    
    class IterationError : public IObserver{
        std::ofstream outf;
        std::string outfn;
        SamplesSP data;
        std::shared_ptr<Loss> loss;
        
    public:
        IterationError(std::string const& out_file, SamplesSP data, std::shared_ptr<Loss> loss) : outfn(out_file), data(data), loss(loss){}
        
        void preLearn(BackPropagation const& learner) override {
            outf.open(outfn);
        };
        
        void postLearn(BackPropagation const& learner) override {
            outf.close();
        };
        
        void postIteration(BackPropagation const& learner) override;
    };

}

#endif /* defined(__XAA1__observer__) */
