#ifndef WORKFLOW_OSWORKFLOW_HPP
#define WORKFLOW_OSWORKFLOW_HPP

#include "../measure/OSRunner.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

#define USE_RUBY_ENGINE 1
#define USE_PYTHON_ENGINE 1

namespace openstudio {

class Variant;

namespace measure {
  class OSArgument;
  using OSArgumentMap = std::map<std::string, OSArgument>;
}  // namespace measure

class OSWorkflow
{
 public:
  OSWorkflow(const filesystem::path& oswPath, ScriptEngineInstance& ruby, ScriptEngineInstance& python);

  void run();

 private:
  // TODO: add a Logger
#if USE_RUBY_ENGINE
  ScriptEngineInstance& rubyEngine;
#endif
#if USE_PYTHON_ENGINE
  ScriptEngineInstance& pythonEngine;
#endif
  WorkflowJSON workflowJSON;
  measure::OSRunner runner{workflowJSON};

  static void applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue);
};

}  // namespace openstudio

#endif  // WORKFLOW_OSWORKFLOW_HPP
