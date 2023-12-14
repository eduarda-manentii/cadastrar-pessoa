#include "Transaction.h"

#include "Exception.h"
#include "driver/Connection.h"
#include "driver/Database.h"
#include "persistent.h"

namespace persistent {

thread_local std::shared_ptr<Connection> current_connection;

void set_current_connection(std::shared_ptr<Connection> connection) {
    if (current_connection) {
        throw Exception("transaction active");
    }
    current_connection = connection;
}

void unset_current_connection() {
    current_connection = nullptr;
}

Transaction::Transaction(std::shared_ptr<Connection> connection) {
    set_current_connection(connection);
}

Transaction::~Transaction() {
    unset_current_connection();
}

std::shared_ptr<Connection> Transaction::get_current_connection() {
    if (current_connection) {
        return current_connection;
    }
    return get_database().connect();
}

}  // namespace persistent
