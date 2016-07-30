#include "init_openstudio.hpp"
#include "RubyAPI.hpp"

extern "C" {
RUBY_API void Init_openstudio(void) {

  init_openstudio_internal();
}

}

