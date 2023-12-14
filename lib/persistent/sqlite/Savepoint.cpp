#include "Savepoint.h"

#include <sqlite3x/sqlite3x.hpp>
#include <sstream>

namespace persistent{ namespace sqlite{

    Savepoint::Savepoint(sqlite3x::sqlite3_connection& con, const string& name):
        con(con),
        committed(false),
        name(name){
            con.executenonquery("SAVEPOINT " + name);
        }


    Savepoint::~Savepoint(){
        if(!committed){
            con.executenonquery("ROLLBACK TO SAVEPOINT " + name);
        }
    }


    void Savepoint::commit(){
        con.executenonquery("RELEASE " + name);
        committed = true;
    }

    string Savepoint::generate_name(){
        size_t n;
        {
            std::lock_guard<std::mutex> l(seq_mutex);
            n = seq++;
        }
        std::stringstream ss;        
        ss << "savepoint_" << n; ///sync(seq)
        return ss.str();
    }

    size_t Savepoint::seq = 0;
    std::mutex Savepoint::seq_mutex;


}}
