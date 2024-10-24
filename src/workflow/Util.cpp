/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/core/ZipFile.hpp"
#include "../utilities/bcl/BCLXML.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/filetypes/WorkflowStepResult.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/time/DateTime.hpp"

#include <boost/filesystem/operations.hpp>
#include <utilities/idd/IddEnums.hxx>

#include <boost/regex.hpp>
#include <fmt/format.h>
#include <fmt/std.h>     // Formatting for std::filesystem::path
#include <fmt/chrono.h>  // Formatting for std::chrono

#include <algorithm>
#include <array>
#include <iterator>
#include <map>
#include <string_view>
#include <regex>
#include <string>

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
    std::string fnamePathStr = fnamePath.string();
    if (fnamePathStr.substr(0, 6) == "report" && fnamePath.has_extension()) {
      // TODO: lots of workarounds happening in https://github.com/NREL/OpenStudio-workflow-gem/blob/cf99e6096f33b717df79a71c245a4d7aa4ccb7a6/lib/openstudio/workflow/util/post_process.rb#L188-L216
      // TODO: I don't like it one bit. Instead we should store a stepRunDir on the workflowJSON Steps and iterate on that or something
      boost::smatch matches;
      const std::string dirNameStr = fPath.parent_path().filename().string();
      if (boost::regex_search(dirNameStr, matches, runDirNumbers)) {
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

        const fs::path oriPath = runDirPath / fPath;
        LOG_FREE(Info, "openstudio.workflow.Util", "Saving report " << oriPath << " to " << outPath);
        fs::copy_file(oriPath, outPath, fs::copy_options::overwrite_existing);
      }
    }
  }

  // Remove empty directories
  for (const auto& dirEnt : fs::directory_iterator{runDirPath}) {
    const auto& dirEntryPath = dirEnt.path();
    if (fs::is_directory(dirEntryPath) && fs::is_empty(dirEntryPath)) {
      LOG_FREE(Info, "openstudio.workflow.Util", "Removing empty directory " << dirEntryPath);
      fs::remove(dirEntryPath);
    }
  }
}

void cleanup(const openstudio::filesystem::path& runDirPath) {
  namespace fs = openstudio::filesystem;

  static constexpr std::array<std::string_view, 2> removeExtensions{".mtr", ".epw"};

  for (const auto& dirEnt : fs::directory_iterator{runDirPath}) {
    const auto& dirEntryPath = dirEnt.path();
    if (fs::is_regular_file(dirEntryPath)) {
      if (std::find(removeExtensions.cbegin(), removeExtensions.cend(), dirEntryPath.extension().string()) != removeExtensions.cend()) {
        fs::remove(dirEntryPath);
      }
    }
  }
}

/*****************************************************************************************************************************************************
*                                                E N E R G Y P L U S    O U T P U T R E Q U E S T S                                                 *
*****************************************************************************************************************************************************/

bool mergeOutputTableSummaryReports(IdfObject& existingObject, const IdfObject& newObject) {

  bool added = false;

  // Merge
  std::vector<std::string> reports;
  {
    auto existinEgs = existingObject.extensibleGroups();
    reports.reserve(existinEgs.size());
    std::transform(existinEgs.cbegin(), existinEgs.cend(), std::back_inserter(reports), [](const auto& eg) { return eg.getString(0).get(); });
  }

  std::vector<std::string> reportsToAdd;
  {
    auto newEgs = newObject.extensibleGroups();
    std::transform(newEgs.cbegin(), newEgs.cend(), std::back_inserter(reportsToAdd), [](const auto& eg) { return eg.getString(0).get(); });
  }

  for (const auto& newReport : reportsToAdd) {
    if (std::find(reports.cbegin(), reports.cend(), newReport) != reports.cend()) {
      existingObject.pushExtensibleGroup({newReport});
      added = true;
    }
  }

  return added;
}

bool addEnergyPlusOutputRequest(Workspace& workspace, IdfObject& idfObject) {

  static const std::vector<IddObjectType> allowedObjects{
    IddObjectType::Output_Surfaces_List,
    IddObjectType::Output_Surfaces_Drawing,
    IddObjectType::Output_Schedules,
    IddObjectType::Output_Constructions,
    IddObjectType::Output_Table_TimeBins,
    IddObjectType::Output_Table_Monthly,
    IddObjectType::Output_Variable,
    IddObjectType::Output_Meter,
    IddObjectType::Output_Meter_MeterFileOnly,
    IddObjectType::Output_Meter_Cumulative,
    IddObjectType::Output_Meter_Cumulative_MeterFileOnly,
    IddObjectType::Meter_Custom,
    IddObjectType::Meter_CustomDecrement,
    IddObjectType::EnergyManagementSystem_OutputVariable,
  };

  auto iddObjectType = idfObject.iddObject().type();

  if (std::find(allowedObjects.cbegin(), allowedObjects.end(), iddObjectType) != allowedObjects.end()) {

    // If already present, don't do it
    for (const auto& wo : workspace.getObjectsByType(iddObjectType)) {
      if (idfObject.dataFieldsEqual(wo)) {
        return false;
      }
    }

    workspace.addObject(idfObject);

    return true;
  }

  //  static const std::vector<IddObjectType> allowedUniqueObjects{
  //    // IddObjectType::Output_EnergyManagementSystem, // TODO: have to merge
  //    // IddObjectType::OutputControl_SurfaceColorScheme, // TODO: have to merge
  //    IddObjectType::Output_Table_SummaryReports,  // TODO: have to merge
  //
  //    // Not allowed
  //    // IddObjectType::OutputControl_Table_Style,
  //    // IddObjectType::OutputControl_ReportingTolerances,
  //    // IddObjectType::Output_SQLite,
  //  };

  if (iddObjectType == IddObjectType::Output_Table_SummaryReports) {
    auto summaryReports = workspace.getObjectsByType(iddObjectType);
    if (summaryReports.empty()) {
      workspace.addObject(idfObject);
      return true;
    } else {
      mergeOutputTableSummaryReports(summaryReports.front(), idfObject);
    }
  }

  return false;
}

/*****************************************************************************************************************************************************
*                                                           O U T P U T    A D A P T E R                                                            *
*****************************************************************************************************************************************************/

void zipDirectory(const openstudio::path& dirPath, const openstudio::path& zipFilePath) {

  namespace fs = openstudio::filesystem;

  if (fs::exists(zipFilePath)) {
    fs::remove(zipFilePath);
  }

  {
    openstudio::ZipFile zf(zipFilePath, false);  // Passing add = false means you create the zip, not append

    static constexpr std::array<std::string_view, 3> filterOutDirNames{"seed", "measures", "weather"};

    auto directorySize = [](const openstudio::path& dirPath) {
      uintmax_t dirSize = 0;
      for (const auto& dirEnt : fs::recursive_directory_iterator{dirPath}) {
        const auto& dirEntryPath = dirEnt.path();
        if (fs::is_regular_file(dirEntryPath)) {
          dirSize += fs::file_size(dirEntryPath);
        }
      }
      return dirSize;
    };

    for (const auto& dirEnt : fs::directory_iterator{dirPath}) {
      const auto& dirEntryPath = dirEnt.path();
      if (fs::is_directory(dirEntryPath)) {

        // Skip a few directory that should not be zipped as they are inputs
        if (std::find(filterOutDirNames.cbegin(), filterOutDirNames.cend(), dirEntryPath.extension().string()) != filterOutDirNames.cend()) {
          continue;
        }

        // TODO: do I need a helper like the workflow-gem was doing with add_directory_to_zip?
        zf.addDirectory(dirEntryPath, fs::relative(dirEntryPath, dirPath));
      } else {
        auto ext = dirEntryPath.extension().string();
        if ((ext.find(".zip") != std::string::npos) || (ext.find(".rb") != std::string::npos)) {
          continue;
        }
        zf.addFile(dirEntryPath, fs::relative(dirEntryPath, dirPath));
      }
    }
  }

  // chmod 644. TODO: is this necessary? 644 should be default already
  boost::filesystem::permissions(zipFilePath, boost::filesystem::perms::owner_read | boost::filesystem::perms::owner_write
                                                | boost::filesystem::perms::group_read | boost::filesystem::perms::others_read);
}

void zipResults(const openstudio::path& dirPath) {

  namespace fs = openstudio::filesystem;

  if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
    zipDirectory(dirPath, dirPath / "data_point.zip");
  }

  // TODO: the reports folder is not inside the run/ directory though?
  // zip up only the reports folder
  auto reportDirPath = dirPath / "reports";
  if (fs::exists(reportDirPath) && fs::is_directory(reportDirPath)) {
    zipDirectory(reportDirPath, dirPath / "data_point_reports.zip");
  }
}

// TODO: investigate removing this. `OSRunner::registerValue` already calls `OSRunner::cleanValueName`, perhaps we just reuse that
std::string sanitizeKey(std::string key) {
  static const std::regex invalidCharsRegex(R"([|!@#$%^&*(){}\\[\];:'",<.>/?+=]+)");
  static const std::regex squeezeUnderscoresRegex(R"(_{2,})");
  static const std::regex trailingUnderscoreOrWhiteSpaceRegex(R"((_|\s)+$)");

  if (std::regex_search(key, invalidCharsRegex)) {
    LOG_FREE(Warn, "openstudio.worklow.Util", fmt::format("Renaming result key '{}' to remove invalid characters", key));
    // Replace invalid characters with underscores
    key = std::regex_replace(key, invalidCharsRegex, "_");
  }

  // Squeeze consecutive underscores
  key = std::regex_replace(key, squeezeUnderscoresRegex, "_");
  // Strip trailing underscores or whitespace
  key = std::regex_replace(key, trailingUnderscoreOrWhiteSpaceRegex, "");

  return key;
}

bool addResultMeasureInfo(WorkflowStepResult& result, BCLMeasure& measure) {
  try {
    result.setMeasureType(measure.measureType());
    result.setMeasureName(measure.name());
    result.setMeasureId(measure.uid());
    result.setMeasureVersionId(measure.versionId());
    auto version_modified_ = measure.versionModified();
    if (version_modified_) {
      result.setMeasureVersionModified(*version_modified_);
    }
    result.setMeasureXmlChecksum(measure.xmlChecksum());
    result.setMeasureClassName(measure.className());
    result.setMeasureDisplayName(measure.displayName());
    result.setMeasureTaxonomy(measure.taxonomyTag());
    return true;
  } catch (...) {
    return false;
  }
}

}  // namespace openstudio::workflow::util
