#ifndef _PERSISTENT_SQLITE_DATABASE_H_
#define _PERSISTENT_SQLITE_DATABASE_H_


#include "persistent/driver/Database.h"

#include <string>

namespace persistent{ namespace sqlite{


    class Database : public persistent::Database{

        public:

        Database(const std::string& path);


        std::unique_ptr<persistent::Connection> connect();


        private:

        std::string path;

    };

}}

#endif //_PERSISTENT_SQLITE_DATABASE_H_
