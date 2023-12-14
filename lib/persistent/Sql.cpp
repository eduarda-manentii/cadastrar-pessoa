#include "Sql.h"

namespace persistent {
namespace Sql {

template <typename Transformation>
string join(const vector<string>& itens, const string& separator, Transformation transformation) {
    string s;

    auto it = itens.begin();

    if (it != itens.end()) {
        s += transformation(*it++);

        while (it != itens.end()) {
            s += separator;
            s += transformation(*it++);
        }
    }

    return s;
}

string join(const vector<string>& itens, const string& separator) {
    return join(itens, separator, [](const string& s) { return s; });
}

string placeholders(const string& placeholder, size_t num, const string& separator) {
    string s;
    if (num > 0) {
        s += placeholder;
        for (size_t i = 1; i < num; ++i) {
            s += separator;
            s += placeholder;
        }
    }
    return s;
}

string where(const vector<string>& conditions) {
    string sql;
    if (!conditions.empty()) {
        sql = " where " + join(conditions, " and ");
    }
    return sql;
}

string set_clause(const vector<string>& columns, const string& placeholder) {
    return " set " + join(columns, ", ", [&placeholder](const string& s) { return s + "=" + placeholder; });
}

string select(const string& table, const vector<string>& columns, const vector<string>& conditions) {
    return select(table, join(columns, ","), conditions);
}

string select(const string& table, const string& columns, const vector<string>& conditions) {
    return "select " + columns + " from " + table + where(conditions);
}

string insert(const string& table, const vector<string>& columns) {
    return "insert into " + table + "(" + join(columns, ",") + ") values(" + placeholders("?", columns.size(), ",") + ")";
}

string update(const string& table, const vector<string>& columns, const vector<string>& conditions) {
    return "update " + table + set_clause(columns, "?") + where(conditions);
}

string delete_(const string& table, const vector<string>& conditions) {
    return "delete from " + table + where(conditions);
}

}
}
