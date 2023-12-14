#ifndef _PERSISTENT_DRIVER_EXTRACTOR_H_
#define _PERSISTENT_DRIVER_EXTRACTOR_H_

#include <string>

namespace persistent {

class Extractor {

public:
  virtual ~Extractor();

  virtual bool is_null() = 0;
  virtual void extract_int(int &val) = 0;
  virtual void extract_double(double &val) = 0;
  virtual void extract_string(std::string &val) = 0;
  virtual bool next_row() = 0;
  virtual void next_column() = 0;

  template <typename T> void extract(T &var);
};


}

#endif //_PERSISTENT_DRIVER_EXTRACTOR_H_
