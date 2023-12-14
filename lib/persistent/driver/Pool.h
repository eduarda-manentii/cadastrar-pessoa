#ifndef _PERSISTENT_DRIVER_POOL_H_
#define _PERSISTENT_DRIVER_POOL_H_

#include "Connection.h"
#include "Database.h"

#include <memory>
#include <mutex>
#include <deque>

namespace persistent {


class PoolDatabase;

class PoolConnection : public Connection {
    std::unique_ptr<Connection> connection;
    PoolDatabase& database;

public:
    PoolConnection(PoolDatabase& database);
    virtual ~PoolConnection();

    virtual std::unique_ptr<Statement> prepare(const std::string& sql);
    virtual void update_schema(const TableDefinition& definition);
    virtual void transaction(std::function<void()> callback);
};

class PoolDatabase : public Database {
    Database& database;
    std::deque<std::unique_ptr<Connection>> connections;

    std::mutex mutex;

public:
    PoolDatabase(Database& database);

    std::unique_ptr<Connection> connect();
    inline std::size_t available_connections() const{
        return connections.size();
    }

    inline std::size_t total_connections()const{
        return _total_conenctions;
    }



private:
    friend class PoolConnection;

    std::unique_ptr<Connection> pop();
    void push(std::unique_ptr<Connection>&& connection);

    std::size_t _total_conenctions = 0;

};

}

#endif  //_PERSISTENT_DRIVER_POOL_H_
