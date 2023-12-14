#include "Extractor.h"

#include <persistent/Exception.h>
#include <libpq-fe.h>
#include <boost/lexical_cast.hpp>

namespace persistent{
namespace postgres{


Extractor::Extractor(PGresult* res):
    res(res),
    row(-1),
    col(0),
    rows(PQntuples(res)){
}

Extractor::~Extractor(){
    PQclear(res);
}

bool Extractor::is_null(){
    return PQgetisnull(res, row, col);
}

void Extractor::extract_int(int& val){
    val = boost::lexical_cast<int>(PQgetvalue(res, row, col));
}

void Extractor::extract_double(double& val){
    val = boost::lexical_cast<double>(PQgetvalue(res, row, col));
}

void Extractor::extract_string(std::string& val){
    val = PQgetvalue(res, row, col);
}

bool Extractor::next_row(){
    ++row;
    if(row < rows){
        col = 0;
        return true;
    }
    return false;
}

void Extractor::next_column(){
    ++col;
}



}
}
