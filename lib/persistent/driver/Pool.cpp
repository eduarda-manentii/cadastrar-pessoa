#include "Pool.h"

#include "Database.h"
#include "Connection.h"
#include "Statement.h"
#include "persistent/Exception.h"

namespace persistent {

using lock_guard = std::lock_guard<std::mutex>;


PoolConnection::PoolConnection(PoolDatabase& database)
    : database(database){
    connection = database.pop();
}

PoolConnection::~PoolConnection() {
    database.push(std::move(connection));
}

std::unique_ptr<Statement> PoolConnection::prepare(const std::string& sql) {
    return connection->prepare(sql);
}

void PoolConnection::update_schema(const TableDefinition& definition) {
    connection->update_schema(definition);
}

void PoolConnection::transaction(std::function<void()> callback) {
    connection->transaction(callback);
}

PoolDatabase::PoolDatabase(Database& database)
    : database(database) {
}

std::unique_ptr<Connection> PoolDatabase::connect() {    
    return std::make_unique<PoolConnection>(*this);
}


std::unique_ptr<Connection> PoolDatabase::pop() {
    lock_guard l(mutex);
    if (connections.size() == 0) {
        ++_total_conenctions;
        return database.connect();
    }
    auto con = std::move(connections.back());
    connections.pop_back();
    return con;
}

void PoolDatabase::push(std::unique_ptr<Connection>&& connection) {
    lock_guard l(mutex);
    connections.push_front(std::forward<std::unique_ptr<Connection>>(connection));
}



}
