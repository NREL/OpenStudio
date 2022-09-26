#ifndef WORKFLOW_OSWORKFLOW_HPP
#define WORKFLOW_OSWORKFLOW_HPP

#include "../measure/OSRunner.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

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
  ScriptEngineInstance& rubyEngine;
  ScriptEngineInstance& pythonEngine;
  WorkflowJSON workflowJSON;
  measure::OSRunner runner{workflowJSON};

  static void applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue);
};

}  // namespace openstudio

#endif  // WORKFLOW_OSWORKFLOW_HPP
