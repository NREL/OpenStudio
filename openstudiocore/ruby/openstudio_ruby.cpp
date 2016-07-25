#include "init_openstudio.hpp"
#include <ruby.h>

extern "C" {
void Init_openstudio(void) {

  init_openstudio_internal();
}
}

