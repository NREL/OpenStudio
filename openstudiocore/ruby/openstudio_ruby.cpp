#include "init_openstudio.hpp"
#include <ruby.h>

extern "C" {
  void Init_Encoding(void);
  void rb_call_inits(void);
  void ruby_prog_init(void);
}

extern "C" {
void Init_openstudio(void) {

  init_openstudio_internal();
}
}

// This is because we link against a hacked, static ruby
// that also accommodates static linked embedded ruby / openstudio cli 
extern "C" {
  int rb_hasFile(const char *t_filename) {
    return 0;
  }

  int rb_require_embedded(const char *t_filename) {
    return 0;
  }
}

