#ifndef _PERSISTENT_CONDITION_H_
#define _PERSISTENT_CONDITION_H_


#include "driver/Statement.h"

#include <string>
#include <sstream>


#include <vector>
#include <tuple>
#include <memory>

#include <boost/lexical_cast.hpp>


#include <iostream>


namespace persistent{


    template <typename TupleType, size_t size>
    struct for_each_tuple_helper{


        template <typename Function>
        static void for_each(TupleType& tupl, Function func){
            for_each_tuple_helper<TupleType, size-1>::for_each(tupl, func);
            func(std::get<size-1>(tupl));
        }
    };

    template <typename TupleType>
    struct for_each_tuple_helper<TupleType, 0>{


        template <typename Function>
        static void for_each(TupleType&, Function){
        }
    };

    template <typename TupleType, typename Function>
    void tuple_for_each(TupleType& tupl, Function func){
        for_each_tuple_helper<TupleType, std::tuple_size<TupleType>::value>::for_each(tupl, func);
    }



    class AbstractCondition{

    public:

        const std::string sql;

        AbstractCondition(const std::string& sql): sql(sql){
        }

        virtual void bind(Statement& st) = 0;

    };


    template <typename... Values>
    class Condition : public AbstractCondition{

    public:

        Condition(const std::string& sql, Values... values): AbstractCondition(sql), values(values...){
        }

    private:

        std::tuple<Values...> values;

        struct ConditionBinder{

            Statement& st;

            ConditionBinder(Statement& st): st(st){
            }

            template <typename T>
            void operator()(const T& val){
                st.bind(val);
            }

        };

        void bind(Statement& st){
            tuple_for_each(values, [&st](auto val){
                st.bind(val);
            });
        }

    };


    typedef std::unique_ptr<AbstractCondition> PCondition;



    class Conditions{

        struct Data{
            std::vector<PCondition> conditions;
            std::string order_clause;
            std::string limit_clause;
            std::string joins;
        };

        std::unique_ptr<Data> data;


    public:

        Conditions(): data(std::make_unique<Data>()){
        }

        Conditions(Conditions&& other): data(std::move(other.data)){
        }

        void bind(Statement& st){

            for(PCondition& cond : data->conditions){
                cond->bind(st);
            }
        }

        std::vector<std::string> get(){
            std::vector<std::string> conds;
            for(PCondition& cond : data->conditions){
                conds.push_back(cond->sql);
            }
            return conds;
        }

        Conditions&& operator&&(Conditions&& other){
            for(auto&& condition : other.data->conditions){
                data->conditions.emplace_back(std::move(condition));
            }
            return move();
        }

        template <typename... Args>
        Conditions&& where(const std::string& sql, Args... args){
            set_where(sql, args...);
            return move();
        }

        template <typename... Args>
        Conditions&& operator()(const std::string& sql, Args... args){
            set_where(sql, args...);
            return move();
        }

        inline Conditions&& order(const std::string& order_clause){
            set_order(order_clause);
            return move();
        }

        const std::string& order()const{
            return data->order_clause;
        }

        Conditions&& limit(size_t _limit){
            set_limit(_limit);
            return move();
        }

        Conditions&& limit(size_t _limit, int offset){
            set_limit(_limit, offset);
            return move();
        }

        const std::string& limit(){
            return data->limit_clause;
        }

        Conditions&& joins(const std::string& _joins){
            set_joins(_joins);
            return move();
        }

        Conditions&& left_join(const std::string& table, const std::string& on){
            set_left_join(table, on);
            return move();
        }

        const std::string& joins()const{
            return data->joins;
        }



    private:

        Conditions(const Conditions&) = delete;
        Conditions& operator=(const Conditions&) = delete;


        inline Conditions&& move(){
            return std::move(*this);
        }

        template <typename T, typename C>
        friend class Column;

    protected:

        template <typename... Args>
        void set_where(const std::string& sql, Args... args){
            data->conditions.push_back(PCondition(new Condition<Args...>(sql, args...)));
        }

        void set_order(const std::string& order_clause){
            data->order_clause = " order by " + order_clause;
        }

        void set_limit(size_t _limit){
            std::stringstream s;
            s << " limit " << _limit;
            data->limit_clause = s.str();
        }

        void set_limit(size_t _limit, size_t offset){
            std::stringstream s;
            s << " limit " << _limit << " offset " << offset;
            data->limit_clause = s.str();
        }

        void set_joins(const std::string& _joins){
            data->joins += " " + _joins;
        }

        void set_left_join(const std::string& table, const std::string& on){
            std::ostringstream s;
            s << data->joins;
            s << " left join " << table << " on (" << on << ")";
            data->joins = s.str();
        }


    };

    template <typename... T>
    Conditions conditions(const std::string& sql, T... args){
        Conditions cond;
        return cond(sql, args...);
    }

    template <typename... T>
    Conditions where(const std::string& sql, T... args){
        return conditions(sql, args...);
    }



    Conditions order_by(const std::string& sql);
    Conditions all();


}

#endif //_PERSISTENT_CONDITION_H_
