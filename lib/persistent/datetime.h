#ifndef _PERSISTENT_DATETIME_H_H
#define _PERSISTENT_DATETIME_H_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <string>



namespace persistent{

    //using namespace boost::gregorian;
    //using namespace boost::posix_time;
    typedef boost::gregorian::date date;
    typedef boost::posix_time::ptime datetime;
    typedef boost::posix_time::seconds seconds;

    date today();
    datetime now();

    seconds gmt_offset();


    std::string to_string(const date& d);
    std::string to_string(const datetime& d);
    void from_string(date& var, const std::string& s);
    void from_string(datetime& var, const std::string& s);

}


#endif //_PERSISTENT_DATETIME_H_H
