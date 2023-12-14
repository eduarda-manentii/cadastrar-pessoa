#include "Database.h"
#include "Connection.h"

namespace persistent{ namespace sqlite{


    Database::Database(const std::string& path): path(path){
    }

    std::unique_ptr<persistent::Connection> Database::connect(){
        return std::unique_ptr<persistent::Connection>{new Connection(path)};
    }

}}
