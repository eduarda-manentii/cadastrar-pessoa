#ifndef _PERSISTENT_ABSTRACTCOLUMN_H_
#define _PERSISTENT_ABSTRACTCOLUMN_H_



#include <string>



namespace persistent{

    class Conditions;
    class Binder;
    class Statement;
    class Extractor;

    template <class T>
    class AbstractColumn{

    public:

        const std::string name;

        AbstractColumn(const std::string& name): name(name){
        }

        virtual ~AbstractColumn(){
        }

        virtual std::string to_string(const T& obj) = 0;
        virtual void construct_null(T& obj) = 0;
        virtual void set_null(T& obj) = 0;
        virtual bool is_null(const T& obj) = 0;

        virtual std::string condition(const T& obj) = 0;
        virtual std::string like_condition(const T& obj) = 0;
        virtual std::string column_type() = 0;

        virtual void bind(Statement& st, const T& obj) = 0;
        virtual void read(Extractor& ex, T& obj) = 0;

        virtual void set(T& obj, int val) = 0;
        virtual bool is_integer() = 0;

        virtual void add_condition(T& obj, Conditions& conditions) = 0;
        virtual void add_like_condition(T& obj, Conditions& conditions) = 0;

    };

}


#endif //_PERSISTENT_ABSTRACTCOLUMN_H_
