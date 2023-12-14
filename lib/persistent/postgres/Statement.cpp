#include "Statement.h"

#include "Extractor.h"
#include <persistent/Exception.h>
#include <libpq-fe.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <algorithm>


namespace persistent{
namespace postgres{


std::string convert_params(const std::string& sql){
    size_t n = 1;
    size_t i = 0;
    int pos = 0;
    std::stringstream s;
    while((pos = sql.find('?', i)) != -1){
        s << sql.substr(i, pos-i)
          << '$' << n;
        i = pos+1;
        ++n;
    }
    s << sql.substr(i);
    return s.str();
}




Statement::Statement(PGconn* con, const std::string& sql):
    con(con),
    sql(convert_params(sql)){

}

Statement::~Statement(){
    for(const char* st : params){
        if(st){
            delete[] st;
        }
    }
}


void add(std::vector<const char*>& params, const std::string& val){
    char* st = new char[val.size() + 1];
    *std::copy(val.begin(), val.end(), st) = 0;
    params.push_back(st);
}

void Statement::bind_null(){
    params.push_back(nullptr);
}

void Statement::bind_int(int val){
    add(params, boost::lexical_cast<std::string>(val));
}

void Statement::bind_double(double val){
    add(params, boost::lexical_cast<std::string>(val));
}

void Statement::bind_string(std::string val){
    add(params, val);
}


PGresult* exec(PGconn* con, const std::string& sql, std::vector<const char*>& params){
    return PQexecParams(
                con,
                sql.c_str(),
                params.size(),
                nullptr,
                params.data(),
                nullptr,
                nullptr,
                0
                );
}

void check_error(PGresult* res, ExecStatusType code){
    auto status = PQresultStatus(res);
    if(status != code){
        throw persistent::Exception(std::string("postgres result: ") + PQresultErrorMessage(res));
    }
}

void Statement::execute(){
    PGresult* res = exec(con, sql, params);
    std::unique_ptr<persistent::Extractor> ext(new Extractor(res));
    check_error(res, PGRES_COMMAND_OK);
}

std::unique_ptr<persistent::Extractor> Statement::execute_query(){
    PGresult* res = exec(con, sql, params);
    std::unique_ptr<persistent::Extractor> ext(new Extractor(res));
    check_error(res, PGRES_TUPLES_OK);
    return std::move(ext);
}

uint64_t Statement::execute_insert(const std::string& primary_key){
    std::string s = sql + " returning " + primary_key;
    PGresult* res = exec(con, s, params);
    std::unique_ptr<persistent::Extractor> ext(new Extractor(res));
    check_error(res, PGRES_TUPLES_OK);
    return boost::lexical_cast<uint64_t>(PQgetvalue(res, 0, 0));
}


}
}
