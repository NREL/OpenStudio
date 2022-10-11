#include "./ScriptEngineGlobals.hpp"
#include "../scriptengine/ScriptEngine.hpp"

namespace openstudio {

std::unique_ptr<openstudio::ScriptEngine> rubyEngine;
std::unique_ptr<openstudio::ScriptEngine> pythonEngine;

}  // namespace openstudio
