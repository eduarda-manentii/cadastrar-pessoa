#ifndef _PERSISTENT_SQLITE_CONNECTION_H_
#define _PERSISTENT_SQLITE_CONNECTION_H_

#include "persistent/driver/Connection.h"
#include "sqlite3x/sqlite3x.hpp"

namespace persistent{ namespace sqlite{

    class Connection : public persistent::Connection{

        public:

        Connection(const std::string& database);
        ~Connection();

        std::unique_ptr<persistent::Statement> prepare(const std::string& sql);
        void update_schema(const persistent::TableDefinition& definition);
        void transaction(std::function<void()> callback);


        private:

        sqlite3x::sqlite3_connection conn;


    };


}}

#endif //_PERSISTENT_SQLITE_CONNECTION_H_
