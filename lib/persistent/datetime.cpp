#include "datetime.h"
#include "driver/Extractor.h"
#include "driver/Statement.h"


namespace persistent{

    std::string to_string(const date& d){
        return boost::gregorian::to_sql_string(d);
    }

    std::string to_string(const datetime& d){
        return to_string(d.date()) + " " + boost::posix_time::to_simple_string(d.time_of_day());
    }

    template <typename T>
    std::string column_typename();

    template <>
    std::string column_typename<date>(){
        return "text";
    }

    template <>
    std::string column_typename<datetime>(){
        return "text";
    }

    template <>
    void Extractor::extract(date& var){
        std::string s;
        extract(s);
        from_string(var, s);
    }

    template <>
    void Extractor::extract(datetime& var){
        std::string s;
        extract(s);
        from_string(var, s);
    }

    template <>
    void Statement::bind(date val){
        bind(to_string(val));
    }

    template <>
    void Statement::bind(datetime val){
        bind(to_string(val));
    }

    void from_string(date& var, const std::string& s){
        var = boost::gregorian::from_string(s);
    }

    void from_string(datetime& var, const std::string& s){
        var = boost::posix_time::time_from_string(s);
    }

    date today(){
        return now().date();
    }

    datetime now(){
        auto n = boost::posix_time::second_clock::local_time();
        return n - gmt_offset();
    }

    seconds gmt_offset(){
        std::time_t t;
        time(&t);
        std::tm tm;
        ::localtime_r(&t, &tm);
        return seconds(tm.tm_gmtoff);
    }



}
