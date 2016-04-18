
#include <iostream>

extern "C" {
  void Init_generator(void);
  void Init_parser(void);
}

namespace embedded_help { 
  inline void initJSONParser() {
    std::cout << "initJSONParser" << std::endl;
    Init_parser();
    std::cout << "initJSONParser Done" << std::endl;
  }

  inline void initJSONGenerator() {
    std::cout << "initJSONGenerator" << std::endl;
    Init_generator();
  }
}
