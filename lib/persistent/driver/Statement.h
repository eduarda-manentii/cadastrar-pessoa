#ifndef _PERSISTENT_DRIVER_STATEMENT_H_
#define _PERSISTENT_DRIVER_STATEMENT_H_


#include "Extractor.h"

#include <string>
#include <memory>


namespace persistent{


    class Statement{

    public:

        virtual ~Statement();

        virtual void bind_null() = 0;
        virtual void bind_int(int val) = 0;
        virtual void bind_double(double val) = 0;
        virtual void bind_string(std::string val) = 0;

        virtual void execute();
        virtual std::unique_ptr<Extractor> execute_query() = 0;
        virtual uint64_t execute_insert(const std::string& primary_key) = 0;

        template <typename T>
        void bind(T val);

    };

}

#endif //_PERSISTENT_DRIVER_STATEMENT_H_
