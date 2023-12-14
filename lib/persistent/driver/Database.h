#ifndef _PERSISTENT_DRIVER_DATABASE_H_
#define _PERSISTENT_DRIVER_DATABASE_H_


#include <memory>

namespace persistent{

    class Connection;

    class Database{

        public:

        virtual ~Database();
        virtual std::unique_ptr<Connection> connect() = 0;

    };

}


#endif //_PERSISTENT_DRIVER_DATABASE_H_
