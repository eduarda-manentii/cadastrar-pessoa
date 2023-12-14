#include "Column.h"

namespace persistent{

    template <>
    std::string like_operator<std::string>(){
        return " like ";
    }

    template <>
    void set_integer(int& var, int val){
        var = val;
    }

    template <>
    bool is_integer<int>(){
        return true;
    }



    template <>
     std::string column_typename<int>(){
        return "integer";
    }

    template <>
    std::string column_typename<std::string>(){
        return "text";
    }

    template <>
    std::string column_typename<double>(){
        return "double";
    }



}
