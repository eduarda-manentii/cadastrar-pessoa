#ifndef _PERSISTENT_SQLITE_STATEMENT_H_
#define _PERSISTENT_SQLITE_STATEMENT_H_

#include <persistent/driver/Statement.h>
#include <sqlite3x/sqlite3x.hpp>

namespace persistent {
namespace sqlite {

class Extractor;

class Statement : public persistent::Statement {
public:
    Statement(sqlite3x::sqlite3_connection& conn, const std::string& sql);

    void bind_null();
    void bind_int(int val);
    void bind_double(double val);
    void bind_string(std::string val);

    std::unique_ptr<persistent::Extractor> execute_query();
    void execute();
    uint64_t execute_insert(const std::string&);

private:
    sqlite3x::sqlite3_command cmd;
    int index;
};


}
}

#endif  //_PERSISTENT_SQLITE_STATEMENT_H_
