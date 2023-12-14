#ifndef _PERSISTENT_DRIVER_CONNECTION_H_
#define _PERSISTENT_DRIVER_CONNECTION_H_


#include <memory>
#include <functional>

#include "../TableDefinition.h"

namespace persistent{

    class Statement;
    class Extractor;

    class Connection{

        public:

        virtual ~Connection();

        virtual std::unique_ptr<Statement> prepare(const std::string& sql) = 0;
        virtual void update_schema(const TableDefinition& definition) = 0;
        virtual void transaction(std::function<void()> callback) = 0;

    };

}

#endif //_PERSISTENT_DRIVER_CONNECTION_H_
