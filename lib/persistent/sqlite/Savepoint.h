#ifndef _PERSISTENT_SQLITE_SAVEPOINT_H_
#define _PERSISTENT_SQLITE_SAVEPOINT_H_

#include <string>
#include <mutex>

namespace sqlite3x{
    class sqlite3_connection;
}

namespace persistent{ namespace sqlite{

    using std::string;

    class Savepoint{

        sqlite3x::sqlite3_connection& con;
        bool committed;
        string name;

        public:

        Savepoint(sqlite3x::sqlite3_connection& con, const string& name = generate_name());
        ~Savepoint();
        void commit();

        private:

        static string generate_name();
        static size_t seq;
        static std::mutex seq_mutex;


    };
}}

#endif //_PERSISTENT_SQLITE_SAVEPOINT_H_
