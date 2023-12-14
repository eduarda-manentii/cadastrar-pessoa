#ifndef _PERSISTENT_TABLE_H_
#define _PERSISTENT_TABLE_H_


#include "Column.h"
#include "TableDefinition.h"

#include <vector>
#include <unordered_map>
using std::vector;


namespace persistent{

    template <class T>
    class Table{

    public:


        Table(){
            schema();
            build_definition();
        }

        ~Table(){
            for(AbstractColumn<T>* col : _columns){
                delete col;
            }
        }

        const string& name()const{
            return _name;
        }

        const vector<AbstractColumn<T>*>& columns()const{
            return _columns;
        }

        const vector<AbstractColumn<T>*>& pks()const{
            return _pks;
        }

        const vector<AbstractColumn<T>*>& non_pks()const{
            return _non_pks;
        }

        const TableDefinition& definition()const{
            return _definition;
        }

        AbstractColumn<T>* get_column(const string& column_name)const{
            auto it = _column_names.find(column_name);
            if(it == _column_names.end()){
                throw Exception("Table " + name() + " doesn't have a column named " + column_name + ".");
            }
            return it->second;
        }

        void table(const string& _name){
            this->_name = _name;
        }

        template <typename C>
        void column(const string& name, C T::*member){
            AbstractColumn<T>* col = new Column<T, C>(name, member);
            _non_pks.push_back(col);
            _columns.push_back(col);
            _column_names[name] = col;
        }

        template <typename P, typename C>
        void column(const std::string& name, C P::*member){
            column<C>(name, member);
        }

        template <typename C>
        void pk(const string& name, C T::*member){
            AbstractColumn<T>* col = new Column<T, C>(name, member);
            _pks.push_back(col);
            _columns.push_back(col);
            _column_names[name] = col;
        }

    protected:
    private:
        string _name;
        vector<AbstractColumn<T>*> _columns;
        vector<AbstractColumn<T>*> _pks;
        vector<AbstractColumn<T>*> _non_pks;
        std::unordered_map<string, AbstractColumn<T>*> _column_names;
        TableDefinition _definition;


        void schema();

        void build_definition(){
            _definition.name = name();
            for(auto col : columns()){
                _definition.add_column(col->column_type(), col->name);
            }
            for(auto pk : pks()){
                _definition.add_pk(pk->name);
            }
        }

    };

}

#endif //_PERSISTENT_TABLE_H_
