#ifndef _PERSISTENT_SQL_H_
#define _PERSISTENT_SQL_H_

#include <string>


#include <vector>


namespace persistent {

namespace Sql {

using std::vector;
using std::string;

string select(const string& table, const vector<string>& columns, const vector<string>& conditions);
string select(const string& table, const string& columns, const vector<string>& conditions);
string insert(const string& table, const vector<string>& columns);
string update(const string& table, const vector<string>& columns, const vector<string>& conditions);
string delete_(const string& table, const vector<string>& conditions);
}
}

#endif  //_PERSISTENT_SQL_H_
