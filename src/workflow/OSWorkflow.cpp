#include "./OSWorkflow.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include <iostream>

namespace openstudio {

OSWorkflow::OSWorkflow(const filesystem::path& oswPath, const ScriptEngineInstance& ruby, const ScriptEngineInstance& python)
  : rubyEngine(ruby), pythonEngine(python), workflowJSON(oswPath) {}

void OSWorkflow::run() {
  rubyEngine->exec("puts 'Hello from Ruby'");
  pythonEngine->exec("print('Hello from Python')");

  // 1. Instantiate seed model

  const auto modelSteps = workflowJSON.getMeasureSteps(openstudio::MeasureType::ModelMeasure);

  for (const auto& step : modelSteps) {
    const auto measureName = step.measureDirName();
    const auto measurePath = workflowJSON.findMeasure(measureName);
    if (measurePath) {
      BCLMeasure bclMeasure(measurePath.get());
      std::cout << "Class Name: " << bclMeasure.className() << std::endl;
      const auto rubyScript = bclMeasure.primaryRubyScriptPath();
      if (rubyScript) {
        std::cout << "Script Path: " << rubyScript.get() << std::endl;
      }
      // 2. Determine Ruby or Python
      // 3. Import measure.(py|rb)
      // 4. Instantiate Measure
      // 5. Run Measure
    }
  }
}

}  // namespace openstudio
