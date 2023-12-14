#ifndef _PERSISTENT_TRANSACTION_H_
#define _PERSISTENT_TRANSACTION_H_

#include <memory>

namespace persistent {

class Connection;

struct Transaction {
    Transaction(std::shared_ptr<Connection> connection);
    ~Transaction();

    static std::shared_ptr<Connection> get_current_connection();
};

}  // namespace persistent

#endif  // _PERSISTENT_TRANSACTION_H_
