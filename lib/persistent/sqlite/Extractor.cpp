#include "Extractor.h"


namespace persistent { namespace sqlite {


Extractor::Extractor(const sqlite3x::sqlite3_reader& reader): reader(reader), index(0){

}


bool Extractor::next_row() {
    index = 0;
    return reader.read();
}

void Extractor::next_column(){
    ++index;
}



void Extractor::extract_string(std::string& val) {
    reader.get(index, val);
}

void Extractor::extract_int(int& val) {
    reader.get(index, val);
}

void Extractor::extract_double(double& val) {
    reader.get(index, val);
}

bool Extractor::is_null() {
    return reader.isnull(index);
}






}}
