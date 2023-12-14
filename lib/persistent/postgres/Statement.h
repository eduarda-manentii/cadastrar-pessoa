#ifndef _PERSISTENT_POSTGRES_STATEMENT_H_
#define _PERSISTENT_POSTGRES_STATEMENT_H_


#include <persistent/driver/Statement.h>
#include <vector>

typedef struct pg_conn PGconn;

namespace persistent{
namespace postgres{


class Statement : public persistent::Statement{

public:
    Statement(PGconn* con, const std::string& sql);
    ~Statement();

    void bind_null();
    void bind_int(int val);
    void bind_double(double val);
    void bind_string(std::string val);

    void execute();
    std::unique_ptr<Extractor> execute_query();
    uint64_t execute_insert(const std::string& primary_key);

private:

    PGconn* con;
    std::string sql;
    std::vector<const char*> params;
};

}
}

#endif // _PERSISTENT_POSTGRES_STATEMENT_H_
