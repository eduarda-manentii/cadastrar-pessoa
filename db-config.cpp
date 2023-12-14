#include <persistent/driver/Pool.h>
#include <persistent/persistent.h>
// #include <persistent/postgres/Database.h>
#include <persistent/sqlite/Database.h>

namespace persistent {

Database& get_database() {
    // static postgres::Database database("host=localhost user=postgres
    // password=postgres dbname=persistent-test");
    static sqlite::Database database("db/development.sqlite3");
    static PoolDatabase pool(database);
    return pool;
}

}  // namespace persistent
