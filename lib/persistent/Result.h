#ifndef _PERSISTENT_RESULT_H_
#define _PERSISTENT_RESULT_H_


#include "Condition.h"
#include <vector>


namespace persistent{


    template <class T>
    class Result : private Conditions{

    public:
        //Conditions conditions;

        Result(Conditions&& conditions): Conditions(std::move(conditions)){
        }


        template <typename ...Args>
        Result<T>&& where(const std::string& sql, Args ...args){
            Conditions::set_where(sql, args...);
            return std::move(*this);
        }


        Result<T>&& limit(size_t _limit){
            Conditions::set_limit(_limit);
            return std::move(*this);
        }

        Result<T>&& limit(size_t _limit, size_t offset){
            Conditions::set_limit(_limit, offset);
            return std::move(*this);
        }

        Result<T>&& order(std::string _order){
            Conditions::set_order(_order);
            return std::move(*this);
        }

        Result<T>&& joins(const std::string& _joins){
            Conditions::set_joins(_joins);
            return std::move(*this);
        }

        Result<T>&& left_join(const std::string& _table, const std::string& on){
            Conditions::set_left_join(_table, on);
            return std::move(*this);
        }


        template <typename Callback>
        void each(Callback callback){
            T::find(std::forward<Conditions>(self()), callback);
        }

        int count(){
            return T::count(std::move(*this));
        }

        double calculate(const std::string& calc){
            return T::calculate(calc, std::forward<Conditions>(self()));
        }

        operator std::vector<T>(){
            std::vector<T> v;
            each([&v](T& obj){
                v.push_back(obj);
            });
            return v;
        }

        operator T(){
            T obj;
            T::find(self().limit(1), [&obj](T& p){
                obj = p;
            });
            return obj;
        }

    private:

        inline Conditions& self(){
            return *this;
        }
    };

}




#endif //_PERSISTENT_RESULT_H_
