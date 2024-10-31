/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/FileOperations.hpp"

#include "../energyplus/ErrorFile.hpp"
#include "../epjson/epJSONTranslator.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"

#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/core/ASCIIStrings.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <fmt/format.h>
#include <fmt/color.h>

#include <json/json.h>
#include <boost/process.hpp>
#include <boost/regex.hpp>

#include <cstdlib>
#include <stdexcept>

namespace openstudio {

/** PrepareRunDirResults is an RAII helper
  * This will locate E+ exes, copy idd/epsjon to run Directory, and Chdir to the runDirectory.
  * It uses RAII to cleanup after itself (remove copied files, chdir back to original directory) */
struct PrepareRunDirResults
{
  openstudio::filesystem::path energyPlusExe;                       // NOLINT(misc-non-private-member-variables-in-classes)
  openstudio::filesystem::path expandObjectsExe;                    // NOLINT(misc-non-private-member-variables-in-classes)
  std::vector<openstudio::filesystem::path> copiedEnergyPlusFiles;  // NOLINT(misc-non-private-member-variables-in-classes)

  // Doing this with a destructor to ensure that the directory gets cleaned up even if I throw an exception, and I can't forget to do it
  explicit PrepareRunDirResults(openstudio::filesystem::path runDirPath, openstudio::filesystem::path energyPlusDirectory = {})
    : m_runDirPath(std::move(runDirPath)) {
    LOG(Debug, "Original Directory: " << m_curDirPath);
    LOG(Debug, "Changing To run directory: " << m_runDirPath);
    boost::filesystem::current_path(m_runDirPath);

    // TODO: is this really necessary?! the part that copies the idd ini epjson in particular I question
    static constexpr std::array<std::string_view, 3> copyFileExtensions{".idd", ".ini", ".epjson"};
#if defined _WIN32
    static const boost::regex energyplusRegex(R"(^energyplus.exe$)");
    static const boost::regex expandObjectsRegex(R"(^expandobjects.exe$)");
#else
    static const boost::regex energyplusRegex(R"(^energyplus\d{0,4}$)");
    static const boost::regex expandObjectsRegex(R"(^expandobjects\d{0,4}$)");
#endif
    boost::smatch matches;

    if (energyPlusDirectory.empty()) {
      energyPlusDirectory = openstudio::getEnergyPlusDirectory();
    }

    for (const auto& dirEnt : openstudio::filesystem::directory_iterator{energyPlusDirectory}) {
      const auto& dirEntryPath = dirEnt.path();
      if (!openstudio::filesystem::is_regular_file(dirEntryPath)) {
        continue;
      }
      auto lower_ext = openstudio::ascii_to_lower_copy(dirEntryPath.extension().string());
      if (std::find(copyFileExtensions.cbegin(), copyFileExtensions.cend(), lower_ext) != copyFileExtensions.cend()) {
        auto targetPath = m_runDirPath / dirEntryPath.filename();
        openstudio::filesystem::copy_file(dirEntryPath, targetPath, openstudio::filesystem::copy_options::overwrite_existing);
        copiedEnergyPlusFiles.emplace_back(std::move(targetPath));
      } else {
        auto lower_filename = openstudio::ascii_to_lower_copy(dirEntryPath.filename().string());
        if (boost::regex_match(lower_filename, matches, energyplusRegex)) {
          energyPlusExe = dirEntryPath;
        } else if (boost::regex_match(lower_filename, matches, expandObjectsRegex)) {
          expandObjectsExe = dirEntryPath;
        }
      }
    }

    if (energyPlusExe.empty()) {
      throw std::runtime_error(fmt::format("Could not find EnergyPlus executable in {}\n", energyPlusDirectory.string()));
    }

    if (expandObjectsExe.empty()) {
      throw std::runtime_error(fmt::format("Could not find ExpandObjects executable in {}\n", energyPlusDirectory.string()));
    }

    // for (const auto& filePath : copiedEnergyPlusFiles) {
    //   fmt::print("{}\n", filePath.string());
    // }

    LOG(Info, "EnergyPlus executable path is " << energyPlusExe);
    LOG(Info, "ExpandObjects executable path is " << expandObjectsExe);
  }

  PrepareRunDirResults(const PrepareRunDirResults&) noexcept = default;
  PrepareRunDirResults(PrepareRunDirResults&&) noexcept = default;
  PrepareRunDirResults& operator=(const PrepareRunDirResults&) noexcept = default;
  PrepareRunDirResults& operator=(PrepareRunDirResults&&) noexcept = default;

  ~PrepareRunDirResults() {
    LOG(Info, "Removing any copied EnergyPlus files");
    for (const auto& filePath : copiedEnergyPlusFiles) {
      openstudio::filesystem::remove(filePath);
    }

    for (const auto& p : {m_runDirPath / "packaged_measures", m_runDirPath / "Energy+.ini"}) {
      openstudio::filesystem::remove_all(p);
    }

    LOG(Debug, "Changing Current Directory back to: " << m_curDirPath);
    boost::filesystem::current_path(m_curDirPath);
  }

 private:
  REGISTER_LOGGER("openstudio.OSWorkflow.prepareEnergyPlusDir");
  openstudio::filesystem::path m_runDirPath;
  openstudio::filesystem::path m_curDirPath = boost::filesystem::current_path();
};

void OSWorkflow::runEnergyPlus() {

  state = State::EnergyPlus;

  constexpr bool useBoostProcess = true;

  if (runner.halted()) {
    LOG(Info, "Workflow halted, skipping the EnergyPlus simulation");
    return;
  }

  // TODO: there is no point chdir'ing here. Just pass --output-directory to E+?
  auto currentDir = boost::filesystem::current_path();

  // TODO: we need to think about exception handling... workflow gem is full of try catch, instead we could just use enum return types to indicate
  // whether it failed or not or something
  // Eg here I'm supposed to wrap all of the above in a try/catch, so I can ensure that clean_directory is called, then reraise the exception...
  try {
    auto runDirPath = workflowJSON.absoluteRunDir();
    PrepareRunDirResults runDirResults(runDirPath);
    LOG(Info, "Starting simulation in run directory: " << runDirPath);

    auto inIDF = runDirPath / "in.idf";
    // TODO: is this the right place /Do we want to do that if we chose epJSON?
    detailedTimeBlock("Saving IDF", [this, &inIDF] { workspace_->save(inIDF, true); });

    std::ofstream stdout_ofs(openstudio::toString(runDirPath / "stdout-energyplus"), std::ofstream::trunc);

    // TODO: workflow-gem was manually running expandObjects prior to the potential serialization to json
    // Should we rather pass -x to the E+ cmd line?
    if (!workflowJSON.runOptions()->skipExpandObjects()) {
      const std::string cmd = fmt::format("\"{}\"", openstudio::toString(runDirResults.expandObjectsExe.native()));
      LOG(Info, "Running command '" << cmd << "'");

      int result = 0;
      detailedTimeBlock("Running ExpandObjects", [this, /*&cmd,*/ &result, &runDirResults, &stdout_ofs] {
        // result = std::system(cmd.c_str());
        namespace bp = boost::process;
        bp::ipstream is;
        std::string line;
        bp::child c(runDirResults.expandObjectsExe, bp::std_out > is);
        while (c.running() && std::getline(is, line)) {
          stdout_ofs << openstudio::ascii_trim_right(line) << '\n';  // Fix for windows...
          if (m_show_stdout) {
            fmt::print("{}\n", line);
          }
        }
        c.wait();
        result = c.exit_code();
      });
      if (result != 0) {
        LOG(Warn, "ExpandObjects returned a non-zero exit code (" << result << ").");
      }
      auto expanded = runDirPath / "expanded.idf";
      if (openstudio::filesystem::exists(expanded)) {
        if (openstudio::filesystem::is_regular_file(inIDF)) {
          boost::filesystem::rename(inIDF, runDirPath / "pre-expand.idf");
          boost::filesystem::rename(expanded, inIDF);
        }
      }
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
    const std::string cmd = fmt::format("\"{}\" {}", openstudio::toString(runDirResults.energyPlusExe.native()), inIDF.filename().string());
    LOG(Info, "Running command '" << cmd << "'");

    // boost::process allows redirecting stdout / stderr easily, but I can no longer debug in LLDB, which is annoying
    // Edit: actually std::system has the same issue... it captures a SIGVTALRM
    // Disable with: `pro hand -p true -s false SIGVTALRM`
    int result = 0;

    if constexpr (useBoostProcess) {
      detailedTimeBlock("Running EnergyPlus", [this, /*&cmd,*/ &result, &runDirResults, &inIDF, &stdout_ofs] {
        // result = std::system(cmd.c_str());
        namespace bp = boost::process;
        bp::ipstream is;
        std::string line;
        bp::child c(runDirResults.energyPlusExe, inIDF.filename(), (bp::std_out & bp::std_err) > is);
        while (c.running() && std::getline(is, line)) {
          stdout_ofs << openstudio::ascii_trim_right(line) << '\n';  // Fix for windows...
          if (m_show_stdout) {
            fmt::print("{}\n", line);
          }
        }
        c.wait();
        result = c.exit_code();
      });
    } else {
      detailedTimeBlock("Running EnergyPlus", [&cmd, &result] { result = std::system(cmd.c_str()); });
    }

    LOG(Info, "EnergyPlus returned '" << result << "'");
    if (result != 0) {
      LOG(Warn, "EnergyPlus returned a non-zero exit code (" << result << "). Check the stdout-energyplus log");
    }

    {
      auto errPath = runDirPath / "eplusout.err";
      if (openstudio::filesystem::is_regular_file(errPath)) {

        const auto errContent = openstudio::filesystem::read_as_string(errPath);
        workflowJSON.setEplusoutErr(errContent);

        // TODO: just do a search in the errContent?
        if (errContent.find("EnergyPlus Terminated--Fatal Error Detected") != std::string::npos) {
          throw std::runtime_error("EnergyPlus Terminated with a Fatal Error. Check eplusout.err log.");
        }

        // TODO: or we use ErrorFile... In which case what's the point of parsing the number of warnings/severe from eplusout.end?
        // Actually, ErrorFile doesn't understand recurring warnings
        // TODO: add a channel filter on the logger to avoid catching all the debug statements in the ErrorFile class
        const openstudio::energyplus::ErrorFile errFile(errPath);
        std::string status = errFile.completedSuccessfully() ? "Completed Successfully" : "Failed";
        if (m_show_stdout) {
          if (m_style_stdout) {
            fmt::print("RunEnergyPlus: {} with ", status);
            fmt::print(fmt::fg(fmt::color::red), "{} Fatal Errors, ", errFile.fatalErrors().size());
            fmt::print(fmt::fg(fmt::color::orange), "{} Severe Errors, ", errFile.severeErrors().size());
            fmt::print(fmt::fg(fmt::color::yellow), "{} Warnings.", errFile.warnings().size());
            fmt::print("\n");
          } else {
            fmt::print("RunEnergyPlus: {} with {} Fatal Errors, {} Severe Errors, {} Warnings.\n", status, errFile.fatalErrors().size(),
                       errFile.severeErrors().size(), errFile.warnings().size());
          }
        }
        if (!errFile.completedSuccessfully()) {
          throw std::runtime_error("EnergyPlus Terminated with a Fatal Error. Check eplusout.err log.");
        }
      } else {
        LOG(Warn, "EnergyPlus did not create an eplusout.err");
      }
    }

    {
      auto endPath = runDirPath / "eplusout.end";
      if (openstudio::filesystem::is_regular_file(endPath)) {
        const auto endContent = openstudio::filesystem::read_as_string(endPath);
        static const boost::regex warningRegex(R"((\d*).Warning)");
        static const boost::regex severeRegex(R"((\d*).Severe.Errors)");
        boost::smatch matches;
        std::string nWarningStr;
        if (boost::regex_search(endContent, matches, warningRegex)) {
          nWarningStr = std::string(matches[1].first, matches[1].second);
        } else {
          OS_ASSERT(false);
        }
        std::string nSevereStr;
        if (boost::regex_search(endContent, matches, severeRegex)) {
          nSevereStr = std::string(matches[1].first, matches[1].second);
        } else {
          OS_ASSERT(false);
        }
        LOG(Info, "EnergyPlus finished with " << nWarningStr << " warnings and " << nSevereStr << " severe errors");
        if (endContent.find("EnergyPlus Terminated--Fatal Error Detected") != std::string::npos) {
          throw std::runtime_error("EnergyPlus Terminated with a Fatal Error. Check eplusout.err log.");
        }

      } else {
        throw std::runtime_error("EnergyPlus failed and did not create an eplusout.end file. Check the stdout-energyplus log.");
      }
    }

  } catch (const std::exception& e) {
    LOG_AND_THROW(e.what());
  }

  // Set the SQL path
  auto expectedSqlPath = workflowJSON.absoluteRunDir() / "eplusout.sql";
  if (openstudio::filesystem::is_regular_file(expectedSqlPath)) {
    sqlPath = expectedSqlPath;
  } else {
    LOG(Warn, "Unable to find sql file at " << expectedSqlPath);
  }
}

}  // namespace openstudio
