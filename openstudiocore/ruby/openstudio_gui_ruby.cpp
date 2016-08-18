#include "RubyGuiAPI.hpp"
#include "ruby.h"

extern "C" {

void Init_openstudiomodeleditor();

RUBY_GUI_API void Init_openstudio_gui(void) {

  rb_require("");
  Init_openstudiomodeleditor();

}

}

