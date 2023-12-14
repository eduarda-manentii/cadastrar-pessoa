#include "Connection.h"
#include <libpq-fe.h>
#include <persistent/Exception.h>
#include "Statement.h"

#include <string>
#include <sstream>

namespace persistent{
namespace postgres{

Connection::Connection(const std::string& connInfo){
    con = PQconnectdb(connInfo.c_str());
    if(PQstatus(con) != CONNECTION_OK){
        throw persistent::Exception(std::string("postgres connect error: ") + PQerrorMessage(con));
    }
}

Connection::~Connection(){
    PQfinish(con);
}



std::unique_ptr<persistent::Statement> persistent::postgres::Connection::prepare(const std::string& sql){
    return std::unique_ptr<persistent::Statement>(new Statement(con, sql));
}

void Connection::update_schema(const TableDefinition&){
}

class PGResult{

public:

    PGResult(PGresult* res): res(res){

    }

    ~PGResult(){
        PQclear(res);
    }

    void check(){
        if(PQresultStatus(res) != PGRES_COMMAND_OK){
            throw persistent::Exception(std::string("postgres command error: ") + PQresultErrorMessage(res));
        }
    }

private:
    PGresult* res;
};


class Transaction{

public:
    Transaction(PGconn* con):
        con(con){
        PGResult res(PQexec(con, "BEGIN"));
    }

    ~Transaction(){
        if(!success){
            PGResult res(PQexec(con, "ROLLBACK"));
        }
    }

    void commit(){
        PGResult res(PQexec(con, "COMMIT"));
        success = true;
    }

private:
    PGconn* con;
    bool success = false;

};

void Connection::transaction(std::function<void ()> callback){    
    Transaction t(con);
    callback();
    t.commit();
}



}
}
