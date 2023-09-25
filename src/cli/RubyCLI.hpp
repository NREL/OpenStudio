/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RUBY_CLI_included
#define RUBY_CLI_included

#include <memory>

namespace openstudio {

class ScriptEngineInstance;

int rubyCLI(ScriptEngineInstance& rubyEngine);

}  // namespace openstudio

#endif  // RUBY_CLI_included
