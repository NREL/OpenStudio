/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef WORKFLOW_OSWORKFLOW_HPP
#define WORKFLOW_OSWORKFLOW_HPP

#include "Timer.hpp"

#include "../measure/OSRunner.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

#include <functional>
#include <map>
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

  bool run();

 private:
  REGISTER_LOGGER("openstudio.workflow.OSWorkflow");
#if USE_RUBY_ENGINE
  ScriptEngineInstance& rubyEngine;
#endif
#if USE_PYTHON_ENGINE
  ScriptEngineInstance& pythonEngine;
#endif
  WorkflowJSON workflowJSON;
  measure::OSRunner runner{workflowJSON};
  model::Model model;
  boost::optional<Workspace> workspace_;
  openstudio::filesystem::path epwPath;
  openstudio::filesystem::path sqlPath;

  // TODO: use a unique_ptr or an Instance?
  std::unique_ptr<workflow::util::TimerCollection> m_timers = nullptr;

  // TODO: should problably store as json directly...
  // { measureName : { arg_name: arg_value }}
  std::map<std::string, std::map<std::string, openstudio::Variant>> output_attributes;

  bool m_no_simulation = false;
  bool m_post_process_only = false;

  // stdout stuff
  bool m_show_stdout = false;
  bool m_add_timings = false;
  bool m_detailed_timings = true;
  bool m_style_stdout = false;

  /** @name Jobs */
  //@{
  // Jobs

  enum class State
  {
    Queued = -10,
    Errored = -1,
    Finished = 0,

    Initialization = 1,
    OpenStudioMeasures,
    Translator,
    EnergyPlusMeasures,
    PreProcess,
    EnergyPlus,
    ReportingMeasures,
    PostProcess,
    Cleanup,
  };
  State state = State::Queued;

  using memJobFunPtr = void (OSWorkflow::*)();

  // void timeJob(memJobFunPtr, std::string message);

  template <class F, class... Args>
  auto detailedTimeBlock(std::string message, F&& func, Args&&... args) {
    if (m_add_timings && m_detailed_timings) {
      m_timers->newTimer(std::move(message), 1);
    }
    std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
    if (m_add_timings && m_detailed_timings) {
      m_timers->tockCurrentTimer();
    }
  }

  // Wipes and creates directory, loads the seed/idf file
  void runInitialization();
  void runOpenStudioMeasures();
  void runTranslator();
  void runEnergyPlusMeasures();
  void runPreProcess();
  void runEnergyPlus();
  void runReportingMeasures();
  void runPostProcess();

  void runCleanup();

  //@}

  void initializeWeatherFileFromOSW();
  void updateLastWeatherFileFromModel();
  void applyMeasures(MeasureType measureType, bool energyplus_output_requests = false);
  static void applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue);
  void saveOSMToRootDirIfDebug();
  void saveIDFToRootDirIfDebug();

  // write output_attributes to the measure_attributes.json
  void communicateMeasureAttributes() const;
};

}  // namespace openstudio

#endif  // WORKFLOW_OSWORKFLOW_HPP
