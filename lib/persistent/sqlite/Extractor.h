#ifndef _PERSISTENT_SQLITE_EXTRACTOR_H_
#define _PERSISTENT_SQLITE_EXTRACTOR_H_

#include <persistent/driver/Extractor.h>
#include <sqlite3x/sqlite3x.hpp>


namespace persistent{ namespace sqlite{

    class Extractor : public persistent::Extractor{

        public:

        Extractor(const sqlite3x::sqlite3_reader& reader);

        bool is_null();
        void extract_int(int& val);
        void extract_double(double& val);
        void extract_string(std::string& val);
        bool next_row();
        void next_column();


        private:

        sqlite3x::sqlite3_reader reader;
        int index;

    };

}}

#endif //_PERSISTENT_SQLITE_EXTRACTOR_H_
