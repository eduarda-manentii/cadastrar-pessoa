#include "Extractor.h"


namespace persistent{


Extractor::~Extractor(){

}

template <typename T>
void extract(T& var);

template <>
void Extractor::extract(int& var){
    extract_int(var);
}

template <>
void Extractor::extract(double& var){
    extract_double(var);
}

template <>
void Extractor::extract(std::string& var){
    extract_string(var);
}


}
