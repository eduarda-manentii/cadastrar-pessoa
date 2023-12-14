#ifndef _PERSISTENT_EXCEPTION_H_
#define _PERSISTENT_EXCEPTION_H_

#include <exception>
#include <string>

namespace persistent{

    class Exception : public std::exception{

    public:

        Exception(const std::string& msg);
        ~Exception() throw();

        const char* what()const throw();
        std::string message()const;

    private:

        const std::string msg;

    };

}

#endif //_PERSISTENT_EXCEPTION_H_
