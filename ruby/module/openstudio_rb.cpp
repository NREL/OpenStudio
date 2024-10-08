/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../bindings/InitRubyBindings.hpp"
#include <RubyAPI.hpp>
#include <iostream>
#include <ruby.h>

extern "C"
{

  RUBY_API void Init_openstudio(void) {

    openstudio::ruby::initRubyBindings();
  }
}
