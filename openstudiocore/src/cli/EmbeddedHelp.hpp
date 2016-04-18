
#include <iostream>

extern "C" {
  void Init_generator(void);
  void Init_parser(void);
}

namespace embedded_help { 
  inline void initJSONParser() {
    Init_parser();
  }

  inline void initJSONGenerator() {
    Init_generator();
  }
}
