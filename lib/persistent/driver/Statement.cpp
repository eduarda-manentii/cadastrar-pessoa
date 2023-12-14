#include "Statement.h"


namespace persistent{


    Statement::~Statement(){
    }

    void Statement::execute(){
        execute_query();
    }

    template <>
    void Statement::bind(int val){
        bind_int(val);
    }

    template <>
    void Statement::bind(double val){
        bind_double(val);
    }

    template <>
    void Statement::bind(std::string val){
        bind_string(val);
    }

    template <>
    void Statement::bind(const char* val){
        bind_string(val);
    }

}
