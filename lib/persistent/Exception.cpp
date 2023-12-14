#include "Exception.h"


namespace persistent{


    Exception::Exception(const std::string& msg): msg(msg){
    }

    Exception::~Exception() throw(){
    }

    const char* Exception::what()const throw(){
        return msg.c_str();
    }

    std::string Exception::message()const{
        return msg;
    }


}
