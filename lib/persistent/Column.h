#ifndef _PERSISTENT_COLUMN_H_
#define _PERSISTENT_COLUMN_H_

#include "AbstractColumn.h"
#include "Condition.h"
#include "null.h"
#include "persistent/driver/Extractor.h"
#include "persistent/driver/Statement.h"

#include <boost/lexical_cast.hpp>


namespace persistent{


    template <typename T>
    std::string like_operator(){
        return "=";
    }

    template <typename T>
    void set_integer(T&, int){
    }

    template <>
    void set_integer(int& var, int val);

    template <typename T>
    bool is_integer(){
        return false;
    }

    template <>
    bool is_integer<int>();


    template <typename T>
    std::string column_typename();


    template <class T, typename C>
    class Column : public AbstractColumn<T>{

    public:

        typedef C T::*Member;

        Column(const std::string& name, Member member) : AbstractColumn<T>(name), member(member){
        }

        C& ref(T& obj) {
            return obj.*member;
        }

        const C& ref(const T& obj) {
            return obj.*member;
        }

        std::string to_string(const T& obj){
            if(ref(obj) == null::value<C>()){
                return std::string();
            }
            return boost::lexical_cast<std::string>(ref(obj));
        }



        void construct_null(T& obj){
            null::construct(ref(obj));
        }

        void set_null(T& obj){
            ref(obj) = null::value<C>();
        }

        bool is_null(const T& obj){
            return ref(obj) == nil;
        }



        std::string operator_condition(const T& obj, const std::string& op){
            if(is_null(obj)){
                return this->name + " is null";
            }
            return this->name + op + "?";
        }

        std::string condition(const T& obj){
            return operator_condition(obj, "=");
        }

        std::string like_condition(const T& obj){
            return operator_condition(obj, like_operator<C>());
        }

        std::string column_type(){
            return column_typename<C>();
        }



        void bind(Statement& st, const T& obj){
            if(is_null(obj)){
                st.bind_null();
            }
            else{
                st.bind(ref(obj));
            }
        }

        void read(Extractor& ex, T& obj){
            if(ex.is_null()){
                ref(obj) = null::value<C>();
            }
            else{
                ex.extract(ref(obj));
            }
            ex.next_column();
        }


        void set(T& obj, int val){
            set_integer(ref(obj), val);
        }

        bool is_integer(){
            return persistent::is_integer<C>();
        }

        void add_condition(T& obj, Conditions& conditions){
            conditions.set_where(condition(obj), ref(obj));
        }

        void add_like_condition(T& obj, Conditions& conditions){
            conditions.set_where(like_condition(obj), ref(obj));
        }


    private:
        Member member;

    };

}

#endif //_PERSISTENT_COLUMN_H_
