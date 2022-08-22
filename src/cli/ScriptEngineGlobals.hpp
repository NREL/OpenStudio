#ifndef ScriptEngineGlobals_Included
#define ScriptEngineGlobals_Included
#include <memory>

namespace openstudio {

class ScriptEngine;

extern std::unique_ptr<openstudio::ScriptEngine> rubyEngine;
extern std::unique_ptr<openstudio::ScriptEngine> pythonEngine;

}  // namespace openstudio

#endif
