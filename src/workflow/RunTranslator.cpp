#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../energyplus/ForwardTranslator.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include "../utilities/filetypes/ForwardTranslatorOptions.hpp"

#include "../utilities/core/Filesystem.hpp"

#include <chrono>

namespace openstudio {

void OSWorkflow::runTranslator() {

  state = State::Translator;

  // skip if halted
  // TODO: I don't understand this
  if (runner.halted()) {
    LOG(Info, "Workflow halted, skipping OSM to IDF translation");
    // This allows model arguments to still be calculated
    workspace_ = Workspace{};
  }

  auto runDir = workflowJSON.absoluteRunDir();
  OS_ASSERT(openstudio::filesystem::is_directory(runDir));

  // TODO: Copy in the weather file defined in the registry, or alternately in the options

  // Translate the OSM to an IDF
  LOG(Info, "Beginning the translation to IDF")
  auto start = std::chrono::system_clock::now();
  // TimeLogger.start('Translating to EnergyPlus')

  // ensure objects exist for reporting purposes
  model.getUniqueModelObject<openstudio::model::Facility>();
  model.getUniqueModelObject<openstudio::model::Building>();

  openstudio::energyplus::ForwardTranslator ft;
  ft.setForwardTranslatorOptions(workflowJSON.runOptions()->forwardTranslatorOptions());
  workspace_ = ft.translateModel(model);
  auto end = std::chrono::system_clock::now();

  fmt::print("Translating OSM to EnergyPlus IDF took {}\n", std::chrono::duration_cast<std::chrono::seconds>(end - start).count());

  if (!workflowJSON.runOptions() && !workflowJSON.runOptions()->debug()) {
    return;
  }

  LOG(Info, "Saving IDF")
  auto savePath = workflowJSON.absoluteRootDir() / "in.idf";
  // TODO: workflow gem was actually serializating via model_idf.to_s for speed...
  workspace_->save(savePath, true);
  LOG(Info, "Saved IDF as " << savePath);
}

}  // namespace openstudio
