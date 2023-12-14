#include "Connection.h"

#include "Statement.h"
#include "Savepoint.h"

#include <sqlite3x/sqlite3x.hpp>
#include <unordered_set>


#include <iostream>

namespace persistent{ namespace sqlite{


    Connection::Connection(const std::string& database): conn(database.c_str()){
        conn.setbusytimeout(5000);
    }
    Connection::~Connection(){
    }

    std::unique_ptr<persistent::Statement> Connection::prepare(const std::string& sql){
        return std::make_unique<Statement>(conn, sql);
    }

    void Connection::transaction(std::function<void()> callback){
        sqlite3x::sqlite3_transaction trans(conn);
        callback();
        trans.commit();
    }



    std::string create_sql(const std::string& table_name, const persistent::TableDefinition& definition){        
        if(definition.columns.size() == 0){
            return "";
        }
        string sql = "CREATE TABLE " + table_name + "(";
        auto pks = definition.pks;
        auto cols = definition.columns;
        if(pks.size() == 1 && *pks.begin() == "integer"){
            sql += *pks.begin() + " integer primary key autoincrement";
            for(auto col : cols){
                if(col.name != *pks.begin()){
                    sql += ',' + col.name + ' ' + col.type;
                }
            }
        }
        else{
            for(auto col : cols){
                sql += col.name + ' ' + col.type + ',';
            }
            sql += " primary key(";
            for(auto col : pks){
                sql += col + ',';
            }
            sql[sql.size()-1] = ')';

        }
        return sql += ')';
    }

    std::string create_sql(const persistent::TableDefinition& definition){
        return create_sql(definition.name, definition);
    }


    void Connection::update_schema(const persistent::TableDefinition& definition){        
        std::cout << "updating " << definition.name << " table..." << std::endl;
        using namespace sqlite3x;


        string sql = create_sql(definition);
        string sql_anterior;
        {
            sqlite3_command cmd(conn, "select sql from sqlite_master where name=?");
            cmd.bind(1, definition.name);

            sqlite3_reader rd = cmd.executereader();

            if(!rd.read()){
                conn.executenonquery(sql);
                return;
            }
            sql_anterior = rd.getstring(0);

        }
        if(sql.substr(sql.find('(')) != sql_anterior.substr(sql_anterior.find('('))){
            int count = conn.executeint("select count(*) from " + definition.name);
            if(count == 0){
                conn.executenonquery("drop table " + definition.name);
                conn.executenonquery(sql);
            }
            else{

                std::unordered_set<string> previous;
                std::unordered_set<string> current;
                {
                    sqlite3_command cmd(conn, "select * from " + definition.name + " limit 1");
                    sqlite3_reader rd = cmd.executereader();
                    rd.read();
                    int n = rd.columncount();
                    for(int i = 0; i < n; ++i){
                        string col = rd.getcolname(i);
                        previous.insert(col);
                    }
                    for(auto col : definition.columns){
                        current.insert(col.name);
                    }
                }

                vector<string> intersection;

                for(const string& s : previous){
                    if(current.count(s)){
                        intersection.push_back(s);
                    }
                }
                string columns;
                for(const string& s : intersection){
                    columns += s;
                    columns += ',';
                }
                columns = columns.substr(0, columns.size()-1);



                string temp_table = "__" + definition.name + "_temp_";

                Savepoint sp(conn);

                sql = create_sql(temp_table, definition) + ";";
                conn.executenonquery(sql);

                sql = "insert into " + temp_table + "(" + columns + ") select " + columns + " from " + definition.name + ";";
                conn.executenonquery(sql);

                sql = "drop table " + definition.name + ";";
                conn.executenonquery(sql);

                sql = "alter table " + temp_table + " rename to " + definition.name + ";";
                conn.executenonquery(sql);

                sp.commit();

            }
        }
    }




}}
