#include "Util.hpp"

#include "../model/Model.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/bcl/BCLXML.hpp"
#include "../utilities/idf/Workspace.hpp"

#include <boost/regex.hpp>
#include <fmt/format.h>
#include <fmt/std.h>     // Formatting for std::filesystem::path
#include <fmt/chrono.h>  // Formatting for std::chrono

#include <array>
#include <string_view>

namespace openstudio::workflow::util {

model::Model loadOSM(const openstudio::filesystem::path& osmPath) {

  LOG_FREE(Info, "openstudio.worklow.Util", "Loading OSM model");
  LOG_FREE(Info, "openstudio.worklow.Util", "Reading in OSM model " << osmPath);

  openstudio::osversion::VersionTranslator vt;
  auto m_ = vt.loadModel(osmPath);
  if (!m_) {
    throw std::runtime_error(fmt::format("Failed to load OSM file {}\n", openstudio::toString(osmPath)));
  }
  return m_.get();
}

Workspace loadIDF(const openstudio::filesystem::path& idfPath) {
  LOG_FREE(Info, "openstudio.worklow.Util", "Loading IDF workspace");
  LOG_FREE(Info, "openstudio.worklow.Util", "Reading in IDF workspace " << idfPath);

  auto idf_ = Workspace::load(idfPath);
  if (!idf_) {
    throw std::runtime_error(fmt::format("Failed to load IDF file {}\n", openstudio::toString(idfPath)));
  }
  return idf_.get();
};

void gatherReports(const openstudio::filesystem::path& runDirPath, const openstudio::filesystem::path& rootDirPath) {
  namespace fs = openstudio::filesystem;

  LOG_FREE(Info, "openstudio.workflow.Util", "Run directory: " << runDirPath);

  // It's been wiped out during runInitialization
  auto reportsDirPath = rootDirPath / "reports";
  fs::create_directories(reportsDirPath);

  LOG_FREE(Info, "openstudio.workflow.Util", "Reports directory" << rootDirPath);

  // Try to find E+ result file
  auto epHTMPath = runDirPath / "eplustbl.htm";
  if (fs::is_regular_file(epHTMPath)) {
    // TODO: try to force UTF-8? html = File.read(eplus_html); html = html.force_encoding('ISO-8859-1').encode('utf-8', replace: nil)
    LOG_FREE(Info, "openstudio.workflow.Util", "Saving EnergyPlus HTML report to " << reportsDirPath << "/eplustbl.html");

    fs::copy_file(epHTMPath, reportsDirPath / "eplustbl.html", fs::copy_options::overwrite_existing);
  }

  // Find any  `/report.*\..*/` files
  static const boost::regex runDirNumbers(R"(^[0-9]+[0-9]+[0-9]+_(.*)$)");
  for (auto& fPath : fs::recursive_directory_files(runDirPath)) {
    auto fnamePath = fPath.filename();
    auto fnamePathStr = fnamePath.string();
    if (fnamePathStr.substr(0, 6) == "report" && fnamePath.has_extension()) {
      // TODO: lots of workarounds happening in https://github.com/NREL/OpenStudio-workflow-gem/blob/cf99e6096f33b717df79a71c245a4d7aa4ccb7a6/lib/openstudio/workflow/util/post_process.rb#L188-L216
      // TODO: I don't like it one bit. Instead we should store a stepRunDir on the workflowJSON Steps and iterate on that or something
      boost::smatch matches;
      if (boost::regex_search(fPath.parent_path().filename().string(), matches, runDirNumbers)) {
        const std::string measure_dir_name(matches[1].first, matches[1].second);
        const fs::path measureXmlPath = rootDirPath / "measures" / measure_dir_name / "measure.xml";
        std::string measure_class_name;
        if (fs::is_regular_file(measureXmlPath)) {
          measure_class_name = BCLXML(measureXmlPath).className();
        } else {
          measure_class_name = measure_dir_name;
        }
        measure_class_name = openstudio::toUnderscoreCase(measure_class_name);
        const fs::path outPath = reportsDirPath / fmt::format("{}_{}", measure_class_name, fnamePathStr);

        LOG_FREE(Info, "openstudio.workflow.Util", "Saving report " << fnamePath << " to " << outPath);
        fs::copy_file(fnamePath, outPath, fs::copy_options::overwrite_existing);
      }
    }
  }
}

void cleanup(const openstudio::filesystem::path& runDirPath) {
  namespace fs = openstudio::filesystem;

  static constexpr std::array<std::string_view, 2> removeExtensions{"*.mtr", "*.epw"};

  for (const auto& dirEnt : openstudio::filesystem::directory_iterator{runDirPath}) {

    const auto& dirEntryPath = dirEnt.path();
    if (fs::is_regular_file(dirEntryPath)) {
      if (std::find(removeExtensions.cbegin(), removeExtensions.cend(), dirEntryPath.extension().string()) != removeExtensions.cend()) {
        fs::remove(dirEntryPath);
      }
    }
  }

  // Remove empty directories
  for (const auto& dirPath : openstudio::filesystem::directory_directories(runDirPath)) {
    if (boost::filesystem::is_empty(dirPath)) {
      fs::remove_all(dirPath);
    }
  }
}

}  // namespace openstudio::workflow::util
