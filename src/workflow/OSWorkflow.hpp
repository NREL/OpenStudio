#ifndef WORKFLOW_OSWORKFLOW_HPP
#define WORKFLOW_OSWORKFLOW_HPP

#include "../measure/OSRunner.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"
#include <memory>

#define USE_RUBY_ENGINE 1
#define USE_PYTHON_ENGINE 1

namespace openstudio {

struct WorkflowRunOptions;
class Variant;

namespace measure {
  class OSArgument;
  using OSArgumentMap = std::map<std::string, OSArgument>;
}  // namespace measure

class OSWorkflow
{
 public:
  OSWorkflow(const filesystem::path& oswPath, ScriptEngineInstance& ruby, ScriptEngineInstance& python);
  OSWorkflow(const WorkflowRunOptions& t_workflowRunOptions, ScriptEngineInstance& ruby, ScriptEngineInstance& python);

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

  bool m_no_simulation = false;
  bool m_post_process_only = false;

  // stdout stuff
  bool m_show_stdout = false;
  bool m_add_timings = false;
  bool m_style_stdout = false;

  static void applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue);
};

}  // namespace openstudio

#endif  // WORKFLOW_OSWORKFLOW_HPP
