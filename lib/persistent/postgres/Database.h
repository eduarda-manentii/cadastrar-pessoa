#ifndef DATABASE_H
#define DATABASE_H

#include <persistent/driver/Database.h>



namespace persistent{
namespace postgres{


class Database : public persistent::Database{
public:
    Database(const std::string& connInfo);
    ~Database();

    std::unique_ptr<persistent::Connection> connect();

private:
    std::string connInfo;
};


}
}

#endif // DATABASE_H
