#ifndef _PERSISTENT_BOOLEAN_H_
#define _PERSISTENT_BOOLEAN_H_


#include <boost/logic/tribool.hpp>
#include <string>

namespace persistent{

    using namespace boost::logic;

    class boolean : public tribool{

    public:

        boolean();
        boolean(const boolean& b);
        boolean(const tribool& b);
        boolean(bool b);

    };

    bool operator==(const boolean& b1, const boolean& b2);

    std::string to_string(const boolean& b);
    void from_string(boolean& b, const std::string& s);


}


#endif //_PERSISTENT_BOOLEAN_H_
