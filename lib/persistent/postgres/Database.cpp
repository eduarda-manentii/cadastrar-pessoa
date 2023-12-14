#include "Database.h"

#include "Connection.h"

namespace persistent{
namespace postgres{


Database::Database(const std::string& connInfo):
    connInfo(connInfo){
}

Database::~Database(){

}

std::unique_ptr<persistent::Connection> Database::connect(){
    return std::unique_ptr<persistent::Connection>(new persistent::postgres::Connection(connInfo));
}



}
}

