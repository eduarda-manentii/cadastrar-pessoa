#include "TableDefinition.h"


namespace persistent{


    ColumnDefinition::ColumnDefinition(const string& type, const string& name): type(type), name(name){
    }

    TableDefinition::TableDefinition(){
    }

    TableDefinition::TableDefinition(const string& name): name(name){
    }

    void TableDefinition::add_column(const string& type, const string& name){
        columns.emplace_back(type, name);
    }

    void TableDefinition::add_pk(const string& pk){
        pks.push_back(pk);
    }


}
