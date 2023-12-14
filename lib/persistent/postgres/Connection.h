#ifndef _PERSISTENT_POSTGRES_CONNECTION_H_
#define _PERSISTENT_POSTGRES_CONNECTION_H_


#include <persistent/driver/Connection.h>

typedef struct pg_conn PGconn;


namespace persistent{
namespace postgres{


class Connection : public persistent::Connection{

public:
    Connection(const std::string& connInfo);
    ~Connection();


    std::unique_ptr<Statement> prepare(const std::string& sql);
    void update_schema(const TableDefinition& definition);
    void transaction(std::function<void()> callback);

private:
    PGconn* con;

};


}
}

#endif // _PERSISTENT_POSTGRES_CONNECTION_H_
