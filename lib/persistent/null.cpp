#include "null.h"

#include <limits>

namespace null{

    template <>
    int value(){
        return std::numeric_limits<int>::min();
    }

    template <>
    double value(){
        return -std::numeric_limits<double>::max();
    }

    template <>
    void construct(int& val){
        val = value<int>();
    }

    template <>
    void construct(double& val){
        val = value<double>();
    }

}

null::Nil nil;
