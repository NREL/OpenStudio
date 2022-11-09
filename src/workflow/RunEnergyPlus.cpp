#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/FileOperations.hpp"

#include "../epjson/epJSONTranslator.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"

#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/ASCIIStrings.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <fmt/format.h>
#include <json/json.h>
#include <boost/process.hpp>
#include <boost/regex.hpp>

#include <cstdlib>
#include <stdexcept>

namespace openstudio {

struct PrepareRunDirResults
{
  openstudio::filesystem::path energyPlusExe;
  openstudio::filesystem::path expandObjectsExe;
  std::vector<openstudio::filesystem::path> copiedEnergyPlusFiles;
};

// TODO: is this really necessary?! the part that copies the idd ini epjson in particular I question
auto prepareEnergyPlusDir(const openstudio::filesystem::path& runDirPath, openstudio::filesystem::path energyPlusDirectory = {})
  -> PrepareRunDirResults {
  namespace fs = openstudio::filesystem;

  static constexpr std::array<std::string_view, 3> copyFileExtensions{".idd", ".ini", ".epjson"};

  static const boost::regex energyplusRegex(R"(^energyplus\d{0,4}$)");
  static const boost::regex expandObjectsRegex(R"(^energyplus\d{0,4}$)");
  boost::smatch matches;

  if (energyPlusDirectory.empty()) {
    energyPlusDirectory = openstudio::getEnergyPlusDirectory();
  }

  PrepareRunDirResults result;

  for (const auto& dirEnt : openstudio::filesystem::directory_iterator{energyPlusDirectory}) {
    const auto& dirEntryPath = dirEnt.path();
    if (!fs::is_regular_file(dirEntryPath)) {
      continue;
    }
    auto lower_ext = openstudio::ascii_to_lower_copy(dirEntryPath.extension().string());
    if (std::find(copyFileExtensions.cbegin(), copyFileExtensions.cend(), dirEntryPath.extension().string()) != copyFileExtensions.cend()) {
      auto targetPath = runDirPath / dirEntryPath.filename();
      openstudio::filesystem::copy_file(dirEntryPath, targetPath, openstudio::filesystem::copy_options::overwrite_existing);
      result.copiedEnergyPlusFiles.emplace_back(std::move(targetPath));
    } else {
      auto lower_filename = openstudio::ascii_to_lower_copy(dirEntryPath.filename().string());
      if (boost::regex_match(lower_filename, matches, energyplusRegex)) {
        result.energyPlusExe = dirEntryPath;
      } else if (boost::regex_match(lower_filename, matches, expandObjectsRegex)) {
        result.expandObjectsExe = dirEntryPath;
      }
    }
  }

  if (result.energyPlusExe.empty()) {
    throw std::runtime_error(fmt::format("Could not find EnergyPlus executable in {}\n", energyPlusDirectory.string()));
  }

  if (result.energyPlusExe.empty()) {
    throw std::runtime_error(fmt::format("Could not find EnergyPlus executable in {}\n", energyPlusDirectory.string()));
  }

  LOG_FREE(Info, "openstudio.OSWorkflow.prepareEnergyPlusDir", "EnergyPlus executable path is " << result.energyPlusExe);
  LOG_FREE(Info, "openstudio.OSWorkflow.prepareEnergyPlusDir", "ExpandObjects executable path is " << result.expandObjectsExe);

  return result;
}

void OSWorkflow::runEnergyPlus() {

  if (runner.halted()) {
    LOG(Info, "Workflow halted, skipping the EnergyPlus simulation");
    return;
  }

  auto runDirPath = workflowJSON.absoluteRunDir();
  auto [energyPlusExe, expandObjectsExe, copiedEnergyPlusFiles] = prepareEnergyPlusDir(runDirPath);

  boost::filesystem::current_path(runDirPath);
  LOG(Info, "Starting simulation in run directory: " << runDirPath);

  auto inIDF = runDirPath / "in.idf";
  // TODO: is this the right place /Do we want to do that if we chose epJSON?
  detailedTimeBlock("Saving IDF", [this, &inIDF] { workspace_->save(inIDF, true); });

  // TODO: workflow-gem was manually running expandObjects prior to the potential serialization to json
  // I think we should rather pass -x to the E+ cmd line
  if (!workflowJSON.runOptions()->skipExpandObjects()) {
    detailedTimeBlock("Running ExpandObjects", [&runDirPath, &inIDF, &expandObjectsExe] {
      const std::string cmd = openstudio::toString(expandObjectsExe.native());
      LOG(Info, "Running command '" << cmd << "'");

      [[maybe_unused]] auto result = std::system(cmd.c_str());
      auto expanded = runDirPath / "expanded.idf";

      if (openstudio::filesystem::exists(expanded)) {
        if (openstudio::filesystem::is_regular_file(inIDF)) {
          boost::filesystem::rename(inIDF, runDirPath / "pre-expand.idf");
          boost::filesystem::rename(expanded, inIDF);
        }
      }
    });
  }

  if (workflowJSON.runOptions()->epjson()) {
    LOG(Info, "Beginning the translation to epJSON using OpenStudio");
    Json::Value jsonVal;

    detailedTimeBlock("Translating to EnergyPlus epJSON", [this, &jsonVal]() { jsonVal = openstudio::epJSON::toJSON(workspace_.get()); });

    inIDF = runDirPath / openstudio::toPath("in.epJSON");
    if (openstudio::filesystem::is_regular_file(inIDF)) {
      openstudio::filesystem::remove(inIDF);
    }
    detailedTimeBlock("Saving epJSON", [&jsonVal, &inIDF]() {
      std::ofstream ofs(openstudio::toString(inIDF), std::ofstream::trunc);
      ofs << jsonVal.toStyledString() << '\n';
    });
  }

  // TODO: eventually we should change this system call to be an API call to libenergyplusapi (but we need E+ to add cmake exports)
  // cf https://github.com/NREL/EnergyPlus/pull/9712 and my proof of concept at https://github.com/jmarrec/EnergyPlus-Cpp-Demo
  const std::string cmd = fmt::format("{} {} 2>&1", openstudio::toString(energyPlusExe.native()), openstudio::toString(inIDF.native()));
  LOG(Info, "Running command '" << cmd << "'");

  detailedTimeBlock("Running EnergyPlus", [&cmd] {
    auto result = std::system(cmd.c_str());
    LOG(Info, "EnergyPlus returned " << result << "'");
  });
}
}  // namespace openstudio
