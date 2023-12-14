#ifndef _PERSISTENT_TABLE_DEFINITION_H_
#define _PERSISTENT_TABLE_DEFINITION_H_


#include <string>


#include <vector>



namespace persistent{

using std::string;
using std::vector;

    struct ColumnDefinition{
        string type;
        string name;

        ColumnDefinition(const string& type, const string& name);
    };

    struct TableDefinition{

        string name;
        vector<ColumnDefinition> columns;
        vector<string> pks;

        TableDefinition();
        TableDefinition(const string& name);
        void add_column(const string& type, const string& name);
        void add_pk(const string& pk);

    };


}

#endif //_PERSISTENT_TABLE_DEFINITION_H_
