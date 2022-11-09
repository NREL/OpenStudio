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

  // Copy in the weather file
  if (!epwPath.empty()) {
    openstudio::filesystem::copy_file(epwPath, runDir / "in.epw", openstudio::filesystem::copy_options::overwrite_existing);
  } else {
    LOG(Warn, "EPW file not found");
  }

  // Translate the OSM to an IDF
  LOG(Info, "Beginning the translation to IDF")
  detailedTimeBlock("Translating to EnergyPlus IDF", [this]() {
    openstudio::energyplus::ForwardTranslator ft;
    ft.setForwardTranslatorOptions(workflowJSON.runOptions()->forwardTranslatorOptions());
    workspace_ = ft.translateModel(model);
  });

  LOG(Info, "Successfully translated to IDF");

  if (!workflowJSON.runOptions() && !workflowJSON.runOptions()->debug()) {
    return;
  }

  LOG(Info, "Saving IDF to Root Directory");
  auto savePath = workflowJSON.absoluteRootDir() / "in.idf";
  detailedTimeBlock("Saving IDF", [this, &savePath]() {
    // TODO: workflow gem was actually serializating via model_idf.to_s for speed...
    workspace_->save(savePath, true);
  });
  LOG(Info, "Saved IDF as " << savePath);
}

}  // namespace openstudio
