#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <persistent/driver/Extractor.h>


typedef struct pg_result PGresult;


namespace persistent{
namespace postgres{


class Extractor : public persistent::Extractor{
public:
    Extractor(PGresult* res);
    ~Extractor();

    bool is_null();
    void extract_int(int& val);
    void extract_double(double& val);
    void extract_string(std::string& val);
    bool next_row();
    void next_column();

private:
    PGresult* res;
    int row;
    int col;
    int rows;
};


}
}

#endif // EXTRACTOR_H
