#include "Statement.h"

#include "Extractor.h"


namespace persistent {
namespace sqlite {


Statement::Statement(sqlite3x::sqlite3_connection& conn, const std::string& sql) : cmd(conn, sql), index(1) {
}

void Statement::bind_string(std::string val) {
    cmd.bind(index++, val);
}

void Statement::bind_double(double val) {
    cmd.bind(index++, val);
}

void Statement::bind_int(int val) {
    cmd.bind(index++, val);
}

void Statement::bind_null() {
    cmd.bind(index++);
}

void Statement::execute() {
    cmd.executenonquery();
}

std::unique_ptr<persistent::Extractor> Statement::execute_query() {
    return std::make_unique<Extractor>(cmd.executereader());
}

uint64_t Statement::execute_insert(const std::string&) {
    cmd.executenonquery();
    return cmd.insertid();
}

}
}
