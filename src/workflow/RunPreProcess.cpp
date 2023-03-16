#include "OSWorkflow.hpp"

#include "RunPreProcessMonthlyReports.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include "../utilities/core/Filesystem.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Output_SQLite_FieldEnums.hxx>

#include <array>
#include <string_view>
#include <string>

namespace openstudio {

void OSWorkflow::runPreProcess() {
  state = State::PreProcess;

  auto runDirPath = workflowJSON.absoluteRunDir();
  const bool skipEnergyPlusPreprocess = workflowJSON.runOptions()->skipEnergyPlusPreprocess();

  // Save the preprocess file
  if (!skipEnergyPlusPreprocess) {
    // Save to run dir
    auto savePath = runDirPath / "pre-preprocess.idf";
    detailedTimeBlock("Saving Preprocess IDF to Run Dir", [this, &savePath]() {
      // TODO: workflow gem was actually serializating via model.to_s for speed...
      workspace_->save(savePath, true);
    });
  }

  // Add any EnergyPlus Output Requests from Reporting Measures
  LOG(Info, "Beginning to collect output requests from Reporting measures.");
  const bool energyplus_output_requests = true;
  applyMeasures(MeasureType::ReportingMeasure, energyplus_output_requests);
  LOG(Info, "Finished collecting output requests from Reporting measures.");

  // Skip the pre-processor if halted
  if (runner.halted()) {
    LOG(Info, "Workflow halted, skipping the EnergyPlus pre-processor");
    return;
  }

  // TODO: I am extremely confused as to why we force add this stuff...
  if (!skipEnergyPlusPreprocess) {
    LOG(Info, "Running EnergyPlus Preprocess");
    detailedTimeBlock("Running EnergyPlus Preprocess", [this] {
      if (workspace_->getObjectsByType(IddObjectType::Output_SQLite).empty()) {
        LOG(Info, "Adding SQL Output to IDF");
        auto sqliteOutput_ = workspace_->addObject(IdfObject(IddObjectType::Output_SQLite));
        sqliteOutput_->setString(Output_SQLiteFields::OptionType, "SimpleAndTabular");
      }

      for (auto meter : c_monthlyReports) {
        auto idfObject_ = openstudio::IdfObject::load(std::string{meter});
        OS_ASSERT(idfObject_);
        workspace_->addObject(idfObject_.get());
      }
      for (auto meter : c_metersForced) {
        auto idfObject_ = openstudio::IdfObject::load(std::string{meter});
        OS_ASSERT(idfObject_);
        workspace_->addObject(idfObject_.get());
      }
    });
    LOG(Info, "Finished preprocess job for EnergyPlus simulation");
  }

  // Save final IDF
  auto savePath = runDirPath / "in.idf";
  detailedTimeBlock("Saving Final IDF to Run Dir", [this, &savePath]() {
    // TODO: workflow gem was actually serializating via model.to_s for speed...
    workspace_->save(savePath, true);
  });
  LOG(Debug, "Saved final IDF as " << savePath);

  // Perform pre-processing on in.idf to capture logic in RunManager

  saveIDFToRootDirIfDebug();
}
}  // namespace openstudio
