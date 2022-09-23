#ifndef WORKFLOW_OSWORKFLOW_HPP
#define WORKFLOW_OSWORKFLOW_HPP

#include "../measure/OSRunner.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

namespace openstudio {

class OSWorkflow
{
 public:
  OSWorkflow(const filesystem::path& oswPath, const ScriptEngineInstance& ruby, const ScriptEngineInstance& python);

  void run();

 private:
  ScriptEngineInstance rubyEngine;
  ScriptEngineInstance pythonEngine;
  WorkflowJSON workflowJSON;
  measure::OSRunner runner{workflowJSON};
};

}  // namespace openstudio

#endif  // WORKFLOW_OSWORKFLOW_HPP
