/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "VersionTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/ScheduleTypeRegistry.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"
#include "../model/ModelExtensibleGroup.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/idf/IdfObject_Impl.hpp"
#include "../utilities/core/ASCIIStrings.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>

#include <OpenStudio.hxx>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace openstudio {
namespace osversion {

  // Helper class
  // A passed lvalue binds to name, then is copied into m_name.
  RefactoredObjectData::RefactoredObjectData(const IdfObject& oldObject, const IdfObject& newObject)
    : m_oldObject(oldObject), m_newObject(newObject) {}

  // A passed rvalue binds to rname, then is moved into m_name.
  RefactoredObjectData::RefactoredObjectData(IdfObject&& oldObject, IdfObject&& newObject)
    : m_oldObject(std::move(oldObject)), m_newObject(std::move(newObject)) {}

  IdfObject RefactoredObjectData::oldObject() const {
    return m_oldObject;
  }

  IdfObject RefactoredObjectData::newObject() const {
    return m_newObject;
  }

  VersionTranslator::VersionTranslator() : m_originalVersion("0.0.0"), m_allowNewerVersions(true) {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.osversion\\.VersionTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());

    // Register required update methods, indexed on the file version returned by the method (the
    // target version).
    //   - For all but latest version, these are non-trivial transformations between two subsequent
    //     versions.
    //   - For the latest version, start with the defaultUpdate method, and then switch it out for a
    //     non-trivial transformation if necessary. If the previous update was also trivial, just
    //     replace that version number with the new one. (At most there should only be one occurrence
    //     of defaultUpdate in this list.)
    m_updateMethods[VersionString("0.7.2")] = &VersionTranslator::update_0_7_1_to_0_7_2;
    m_updateMethods[VersionString("0.7.3")] = &VersionTranslator::update_0_7_2_to_0_7_3;
    m_updateMethods[VersionString("0.7.4")] = &VersionTranslator::update_0_7_3_to_0_7_4;
    m_updateMethods[VersionString("0.9.2")] = &VersionTranslator::update_0_9_1_to_0_9_2;
    m_updateMethods[VersionString("0.9.6")] = &VersionTranslator::update_0_9_5_to_0_9_6;
    m_updateMethods[VersionString("0.10.0")] = &VersionTranslator::update_0_9_6_to_0_10_0;
    m_updateMethods[VersionString("0.11.1")] = &VersionTranslator::update_0_11_0_to_0_11_1;
    m_updateMethods[VersionString("0.11.2")] = &VersionTranslator::update_0_11_1_to_0_11_2;
    m_updateMethods[VersionString("0.11.5")] = &VersionTranslator::update_0_11_4_to_0_11_5;
    m_updateMethods[VersionString("0.11.6")] = &VersionTranslator::update_0_11_5_to_0_11_6;
    m_updateMethods[VersionString("1.0.2")] = &VersionTranslator::update_1_0_1_to_1_0_2;
    m_updateMethods[VersionString("1.0.3")] = &VersionTranslator::update_1_0_2_to_1_0_3;
    m_updateMethods[VersionString("1.2.3")] = &VersionTranslator::update_1_2_2_to_1_2_3;
    m_updateMethods[VersionString("1.3.5")] = &VersionTranslator::update_1_3_4_to_1_3_5;
    m_updateMethods[VersionString("1.5.4")] = &VersionTranslator::update_1_5_3_to_1_5_4;
    m_updateMethods[VersionString("1.7.2")] = &VersionTranslator::update_1_7_1_to_1_7_2;
    m_updateMethods[VersionString("1.7.5")] = &VersionTranslator::update_1_7_4_to_1_7_5;
    m_updateMethods[VersionString("1.8.4")] = &VersionTranslator::update_1_8_3_to_1_8_4;
    m_updateMethods[VersionString("1.8.5")] = &VersionTranslator::update_1_8_4_to_1_8_5;
    m_updateMethods[VersionString("1.9.0")] = &VersionTranslator::update_1_8_5_to_1_9_0;
    m_updateMethods[VersionString("1.9.3")] = &VersionTranslator::update_1_9_2_to_1_9_3;
    m_updateMethods[VersionString("1.9.5")] = &VersionTranslator::update_1_9_4_to_1_9_5;
    m_updateMethods[VersionString("1.10.0")] = &VersionTranslator::update_1_9_5_to_1_10_0;
    m_updateMethods[VersionString("1.10.2")] = &VersionTranslator::update_1_10_1_to_1_10_2;
    m_updateMethods[VersionString("1.10.6")] = &VersionTranslator::update_1_10_5_to_1_10_6;
    m_updateMethods[VersionString("1.11.4")] = &VersionTranslator::update_1_11_3_to_1_11_4;
    m_updateMethods[VersionString("1.11.5")] = &VersionTranslator::update_1_11_4_to_1_11_5;
    m_updateMethods[VersionString("1.12.1")] = &VersionTranslator::update_1_12_0_to_1_12_1;
    m_updateMethods[VersionString("1.12.4")] = &VersionTranslator::update_1_12_3_to_1_12_4;
    m_updateMethods[VersionString("2.1.1")] = &VersionTranslator::update_2_1_0_to_2_1_1;
    m_updateMethods[VersionString("2.1.2")] = &VersionTranslator::update_2_1_1_to_2_1_2;
    m_updateMethods[VersionString("2.3.1")] = &VersionTranslator::update_2_3_0_to_2_3_1;
    m_updateMethods[VersionString("2.4.2")] = &VersionTranslator::update_2_4_1_to_2_4_2;
    m_updateMethods[VersionString("2.5.0")] = &VersionTranslator::update_2_4_3_to_2_5_0;
    m_updateMethods[VersionString("2.6.1")] = &VersionTranslator::update_2_6_0_to_2_6_1;
    m_updateMethods[VersionString("2.6.2")] = &VersionTranslator::update_2_6_1_to_2_6_2;
    m_updateMethods[VersionString("2.7.0")] = &VersionTranslator::update_2_6_2_to_2_7_0;
    m_updateMethods[VersionString("2.7.1")] = &VersionTranslator::update_2_7_0_to_2_7_1;
    m_updateMethods[VersionString("2.7.2")] = &VersionTranslator::update_2_7_1_to_2_7_2;
    m_updateMethods[VersionString("2.9.0")] = &VersionTranslator::update_2_8_1_to_2_9_0;
    m_updateMethods[VersionString("2.9.1")] = &VersionTranslator::update_2_9_0_to_2_9_1;
    m_updateMethods[VersionString("3.0.0")] = &VersionTranslator::update_2_9_1_to_3_0_0;
    m_updateMethods[VersionString("3.0.1")] = &VersionTranslator::update_3_0_0_to_3_0_1;
    m_updateMethods[VersionString("3.1.0")] = &VersionTranslator::update_3_0_1_to_3_1_0;
    m_updateMethods[VersionString("3.2.0")] = &VersionTranslator::update_3_1_0_to_3_2_0;
    m_updateMethods[VersionString("3.2.1")] = &VersionTranslator::update_3_2_0_to_3_2_1;
    m_updateMethods[VersionString("3.3.0")] = &VersionTranslator::update_3_2_1_to_3_3_0;
    m_updateMethods[VersionString("3.4.0")] = &VersionTranslator::update_3_3_0_to_3_4_0;
    m_updateMethods[VersionString("3.5.0")] = &VersionTranslator::update_3_4_0_to_3_5_0;
    m_updateMethods[VersionString("3.5.1")] = &VersionTranslator::update_3_5_0_to_3_5_1;
    m_updateMethods[VersionString("3.6.0")] = &VersionTranslator::update_3_5_1_to_3_6_0;
    m_updateMethods[VersionString("3.7.0")] = &VersionTranslator::update_3_6_1_to_3_7_0;
    m_updateMethods[VersionString("3.8.0")] = &VersionTranslator::update_3_7_0_to_3_8_0;
    m_updateMethods[VersionString("3.9.0")] = &VersionTranslator::update_3_8_0_to_3_9_0;
    m_updateMethods[VersionString("3.9.1")] = &VersionTranslator::update_3_9_0_to_3_9_1;
    // m_updateMethods[VersionString("3.10.0")] = &VersionTranslator::defaultUpdate;

    // List of previous versions that may be updated to this one.
    //   - To increment the translator, add an entry for the version just released (branched for
    //     release).
    m_startVersions = {
      VersionString("0.7.0"),  VersionString("0.7.1"),  VersionString("0.7.2"),  VersionString("0.7.3"),  VersionString("0.7.4"),
      VersionString("0.7.5"),  VersionString("0.7.6"),  VersionString("0.8.0"),  VersionString("0.8.1"),  VersionString("0.8.2"),
      VersionString("0.8.3"),  VersionString("0.8.4"),  VersionString("0.8.5"),  VersionString("0.9.0"),  VersionString("0.9.1"),
      VersionString("0.9.2"),  VersionString("0.9.3"),  VersionString("0.9.4"),  VersionString("0.9.5"),  VersionString("0.9.6"),
      VersionString("0.10.0"), VersionString("0.10.1"), VersionString("0.10.2"), VersionString("0.10.3"), VersionString("0.10.4"),
      VersionString("0.10.5"), VersionString("0.11.0"), VersionString("0.11.1"), VersionString("0.11.2"), VersionString("0.11.3"),
      VersionString("0.11.4"), VersionString("0.11.5"), VersionString("0.11.6"), VersionString("1.0.0"),  VersionString("1.0.1"),
      VersionString("1.0.2"),  VersionString("1.0.3"),  VersionString("1.0.4"),  VersionString("1.0.5"),  VersionString("1.0.6"),
      VersionString("1.0.7"),  VersionString("1.1.0"),  VersionString("1.1.1"),  VersionString("1.1.2"),  VersionString("1.1.3"),
      VersionString("1.2.0"),  VersionString("1.2.1"),  VersionString("1.2.2"),  VersionString("1.2.3"),  VersionString("1.2.4"),
      VersionString("1.2.5"),  VersionString("1.3.0"),  VersionString("1.3.1"),  VersionString("1.3.2"),  VersionString("1.3.3"),
      VersionString("1.3.4"),  VersionString("1.3.5"),  VersionString("1.4.0"),  VersionString("1.4.1"),  VersionString("1.4.2"),
      VersionString("1.4.3"),  VersionString("1.5.0"),  VersionString("1.5.1"),  VersionString("1.5.2"),  VersionString("1.5.3"),
      VersionString("1.5.4"),  VersionString("1.5.5"),  VersionString("1.6.0"),  VersionString("1.6.1"),  VersionString("1.6.2"),
      VersionString("1.6.3"),  VersionString("1.7.0"),  VersionString("1.7.1"),  VersionString("1.7.2"),  VersionString("1.7.3"),
      VersionString("1.7.4"),  VersionString("1.7.5"),  VersionString("1.8.0"),  VersionString("1.8.1"),  VersionString("1.8.2"),
      VersionString("1.8.3"),  VersionString("1.8.4"),  VersionString("1.8.5"),  VersionString("1.9.0"),  VersionString("1.9.1"),
      VersionString("1.9.2"),  VersionString("1.9.3"),  VersionString("1.9.4"),  VersionString("1.9.5"),  VersionString("1.10.0"),
      VersionString("1.10.1"), VersionString("1.10.2"), VersionString("1.10.3"), VersionString("1.10.4"), VersionString("1.10.5"),
      VersionString("1.10.6"), VersionString("1.11.0"), VersionString("1.11.1"), VersionString("1.11.2"), VersionString("1.11.3"),
      VersionString("1.11.4"), VersionString("1.11.5"), VersionString("1.11.6"), VersionString("1.12.0"), VersionString("1.12.1"),
      VersionString("1.12.2"), VersionString("1.12.3"), VersionString("1.12.4"), VersionString("1.12.5"), VersionString("1.12.6"),
      VersionString("1.13.0"), VersionString("1.13.1"), VersionString("1.13.2"), VersionString("1.13.3"), VersionString("1.13.4"),
      VersionString("1.14.0"), VersionString("2.0.0"),  VersionString("2.0.1"),  VersionString("2.0.2"),  VersionString("2.0.3"),
      VersionString("2.0.5"),  VersionString("2.1.0"),  VersionString("2.1.1"),  VersionString("2.1.2"),  VersionString("2.2.0"),
      VersionString("2.2.1"),  VersionString("2.2.2"),  VersionString("2.3.0"),  VersionString("2.3.1"),  VersionString("2.4.0"),
      VersionString("2.4.1"),  VersionString("2.4.2"),  VersionString("2.4.3"),  VersionString("2.5.0"),  VersionString("2.5.1"),
      VersionString("2.5.2"),  VersionString("2.6.0"),  VersionString("2.6.1"),  VersionString("2.6.2"),  VersionString("2.7.0"),
      VersionString("2.7.1"),  VersionString("2.7.2"),  VersionString("2.8.0"),  VersionString("2.8.1"),  VersionString("2.9.0"),
      VersionString("2.9.1"),  VersionString("3.0.0"),  VersionString("3.0.1"),  VersionString("3.1.0"),  VersionString("3.2.0"),
      VersionString("3.2.1"),  VersionString("3.3.0"),  VersionString("3.4.0"),  VersionString("3.5.0"),  VersionString("3.5.1"),
      VersionString("3.6.0"),  VersionString("3.6.1"),  VersionString("3.7.0"),  VersionString("3.8.0"),  VersionString("3.9.0"),
      // Note: do **not** include the **current** version in m_startVersions, stop at the previous release
      //VersionString("3.9.1"),
    };
  }

  boost::optional<model::Model> VersionTranslator::loadModel(const openstudio::path& pathToOldOsm, ProgressBar* progressBar) {
    LOG(Trace, "Loading model from " << toString(pathToOldOsm) << ".");
    if (getFileExtension(pathToOldOsm) != modelFileExtension()) {
      LOG(Error, "Cannot loadModel for path'" << toString(pathToOldOsm) << "'. Extension must be '" << modelFileExtension() << "'. For '"
                                              << componentFileExtension() << "'s use loadComponent.");
      return boost::none;
    }

    path wp = completePathToFile(pathToOldOsm, path(), modelFileExtension(), false);
    openstudio::filesystem::ifstream inFile(wp);
    if (inFile) {
      return loadModel(inFile, progressBar);
    }
    return boost::none;
  }

  boost::optional<model::Model> VersionTranslator::loadModel(std::istream& is, ProgressBar* progressBar) {
    return updateVersion(is, false, progressBar);
  }

  boost::optional<model::Model> VersionTranslator::loadModelFromString(const std::string& str, ProgressBar* progressBar) {
    std::stringstream ss(str);
    return updateVersion(ss, false, progressBar);
  }

  boost::optional<model::Component> VersionTranslator::loadComponent(const openstudio::path& pathToOldOsc, ProgressBar* progressBar) {
    LOG(Trace, "Loading component from " << toString(pathToOldOsc) << ".");
    if (getFileExtension(pathToOldOsc) != componentFileExtension()) {
      LOG(Error, "Cannot loadComponent for path'" << toString(pathToOldOsc) << "'. Extension must be '" << componentFileExtension() << "'. For '"
                                                  << modelFileExtension() << "'s use loadModel.");
      return boost::none;
    }
    path wp = completePathToFile(pathToOldOsc, path(), componentFileExtension(), false);
    openstudio::filesystem::ifstream inFile(wp);
    if (inFile) {
      return loadComponent(inFile, progressBar);
    }

    return boost::none;
  }

  boost::optional<model::Component> VersionTranslator::loadComponent(std::istream& is, ProgressBar* progressBar) {
    model::OptionalModel result = updateVersion(is, true, progressBar);
    if (result) {
      return result->optionalCast<model::Component>();
    }
    return boost::none;
  }

  VersionString VersionTranslator::originalVersion() const {
    return m_originalVersion;
  }

  std::vector<LogMessage> VersionTranslator::warnings() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() != Warn; }), result.end());
    return result;
  }

  std::vector<LogMessage> VersionTranslator::errors() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() <= Warn; }), result.end());
    return result;
  }

  std::vector<IdfObject> VersionTranslator::deprecatedObjects() const {
    return m_deprecated;
  }

  std::vector<IdfObject> VersionTranslator::untranslatedObjects() const {
    return m_untranslated;
  }

  std::vector<IdfObject> VersionTranslator::newObjects() const {
    return m_new;
  }

  std::vector<RefactoredObjectData> VersionTranslator::refactoredObjects() const {
    return m_refactored;
  }

  bool VersionTranslator::allowNewerVersions() const {
    return m_allowNewerVersions;
  }

  void VersionTranslator::setAllowNewerVersions(bool allowNewerVersions) {
    m_allowNewerVersions = allowNewerVersions;
  }

  boost::optional<model::Model> VersionTranslator::updateVersion(std::istream& is, bool isComponent, ProgressBar* progressBar) {
    m_originalVersion = VersionString("0.0.0");
    m_map.clear();
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();
    m_deprecated.clear();
    m_untranslated.clear();
    m_new.clear();
    m_refactored.clear();
    m_nObjectsStart = 0;
    m_nObjectsFinalIdf = 0;
    m_nObjectsFinalModel = 0;
    m_isComponent = isComponent;

    initializeMap(is);
    OS_ASSERT(m_map.size() < 2u);
    if (m_map.empty()) {
      return boost::none;
    }

    if (progressBar) {
      progressBar->setMinimum(0);
      progressBar->setMaximum(m_startVersions.size());
    }
    for (const VersionString& startVersion : m_startVersions) {
      if (progressBar) {
        progressBar->setWindowTitle("Upgrading from " + startVersion.str());
        progressBar->setValue(progressBar->value() + 1);
      }
      update(startVersion);  // does nothing if no map entry
    }
    if (progressBar) {
      progressBar->setValue(m_startVersions.size());
    }

    model::OptionalModel result;
    IdfFile finalModel = m_map[VersionString(openStudioVersion())];
    LOG(Debug, "Final model has " << finalModel.numObjects() << " objects in IDF form.");
    m_nObjectsFinalIdf = finalModel.numObjects();
    int numExpectedObjects = m_nObjectsStart + newObjects().size() - deprecatedObjects().size() - untranslatedObjects().size();
    if (m_nObjectsFinalIdf != numExpectedObjects) {
      LOG(Warn, "Expected final translated model to have " << numExpectedObjects << ", but it actually has " << m_nObjectsFinalIdf << " objects.");
      if ((m_nObjectsStart > 0) && (m_nObjectsFinalIdf == 0)) {
        LOG(Error, "Original model contained " << m_nObjectsStart << ", but final translated model is empty.");
        return boost::none;
      }
      numExpectedObjects = m_nObjectsFinalIdf;
    }

    // validity checking
    Workspace finalWorkspace(finalModel);
    model::Model tempModel(finalWorkspace);  // None-level strictness!
    OS_ASSERT(tempModel.strictnessLevel() == StrictnessLevel::Minimal);
    std::vector<std::shared_ptr<InterobjectIssueInformation>> issueInfo = fixInterobjectIssuesStage1(tempModel, m_originalVersion);
    if (!tempModel.isValid(StrictnessLevel::Draft)) {
      LOG(Error, "Model with Version " << openStudioVersion() << " IDD is not valid to draft " << "strictness level.");
      LOG(Error, tempModel.validityReport(StrictnessLevel::Draft));
      return boost::none;
    }
    bool test = tempModel.setStrictnessLevel(StrictnessLevel::Draft);
    OS_ASSERT(test);
    fixInterobjectIssuesStage2(tempModel, issueInfo);

    if (isComponent) {
      try {
        result = model::Component(tempModel.toIdfFile());  // includes name conflict fixes
      } catch (std::exception& e) {
        LOG(Error, "Could not translate component, because " << e.what());
      }
    } else {
      result = model::Model(tempModel.toIdfFile());  // includes name conflict fixes
    }

    if (result) {
      m_nObjectsFinalModel = result->numObjects();
      LOG(Debug, "Final model has " << m_nObjectsFinalModel << " objects in Model form.");
      if ((m_nObjectsStart > 0) && (m_nObjectsFinalModel == 0)) {
        LOG(Error, "Original model contained " << m_nObjectsStart << ", but final translated model is empty.");
        return boost::none;
      }
    }
    return result;
  }

  void VersionTranslator::initializeMap(std::istream& is) {
    // default version is 0.7.0
    VersionString currentVersion("0.7.0");

    // load with current IDD and extract version
    if (boost::optional<VersionString> candidate = IdfFile::loadVersionOnly(is)) {
      currentVersion = *candidate;
    }

    // we didn't bump Version Identifier in 3.3.0's idd OS:Version object, so the following is necessary
    if (currentVersion == VersionString("3.2.2")) {
      currentVersion = VersionString("3.3.0");
    }

    m_originalVersion = currentVersion;  // save for user
    is.seekg(std::ios_base::beg);        // prep to re-read file

    // bracket allowable versions
    LOG(Debug, "Starting translation from Version " << currentVersion.str() << ".");
    if (currentVersion < VersionString("0.7.0")) {
      LOG(Error, "Version translation is not provided for OpenStudio models created prior to " << "Version 0.7.0.");
      return;
    }
    if (currentVersion > VersionString(openStudioVersion())) {
      if (m_allowNewerVersions) {
        // if currentVersion is just one ahead, may be a developer using the cloud.
        // let it pass as if currentVersion == openStudioVersion(), with a warning
        if (VersionString(openStudioVersion()).isNextVersion(currentVersion)) {
          LOG(Warn, "Version extracted from file '" << currentVersion.str() << "' is one " << "increment ahead of OpenStudio Version "
                                                    << openStudioVersion() << ". "
                                                    << "Proceeding as if these versions are the same. Use with caution.");
          currentVersion = VersionString(openStudioVersion());
        } else {
          // if currentVersion is farther ahead, log error and return nothing
          LOG(Error, "Version extracted from file '" << currentVersion.str() << "' is not supported by OpenStudio Version " << openStudioVersion()
                                                     << ". Please check https://www.openstudio.net for updates.");
          return;
        }
      } else {
        // log error and return nothing
        LOG(Error, "Version extracted from file '" << currentVersion.str() << "' is newer than current OpenStudio Version " << openStudioVersion()
                                                   << ". Please check https://www.openstudio.net for updates.");
        return;
      }
    }

    // load IdfFile with correct IddFile and save
    OptionalIdfFile oIdfFile;
    IddFileAndFactoryWrapper iddFile = getIddFile(currentVersion);
    if (iddFile.iddFileType() == IddFileType::UserCustom) {
      oIdfFile = IdfFile::load(is, iddFile.iddFile());
      if (currentVersion == VersionString(1, 9, 0)) {
        if (oIdfFile) {
          auto sizingObjects = oIdfFile->getObjectsByType(iddFile.getObject("OS:Sizing:Zone").get());

          // Figure out if the OS:Sizing:Zone object looks like it is from CBECC
          // it will have extra fields that would were not in the "real" 1.9.0 IDD
          bool fromCBECC = false;
          for (auto const& object : sizingObjects) {
            if (auto value = object.getString(2)) {
              if (istringEqual("SupplyAirTemperature", value.get()) || istringEqual("TemperatureDifference", value.get())) {
                fromCBECC = true;
                break;
              }
            }
          }

          if (fromCBECC) {
            // Remove the sizing objects so that the translation will proceed smoothly
            oIdfFile->removeObjects(sizingObjects);

            // Get a special CBECC idd file,
            // load the idf file again against that idd,
            // get the sizing objects and save them for later,
            // we will reintrodce the sizing objects in the version 1.10.2 phase of the translation
            // when they were officially part of OS
            auto cbeccIddFile = get_1_9_0_CBECC_IddFile();

            is.seekg(0, std::ios::beg);
            auto cbeccIdfFile = IdfFile::load(is, cbeccIddFile);
            OS_ASSERT(cbeccIdfFile);
            m_cbeccSizingObjects = cbeccIdfFile->getObjectsByType(cbeccIddFile.getObject("OS:Sizing:Zone").get());
          }
        }
      }
    } else {
      oIdfFile = IdfFile::load(is, iddFile.iddFileType());
    }
    if (!oIdfFile) {
      LOG(Error, "Unable to load Model with Version " << currentVersion.str() << " IDD.");
      return;
    }

    IdfFile idfFile = *oIdfFile;
    m_nObjectsStart = idfFile.numObjects();

    // DLM: would like to check validity here, can't due to bug with validity checking using custom idds

    m_map[currentVersion] = idfFile;
    LOG(Debug, "Initial model has " << idfFile.numObjects() << " objects.");
  }

  IddFileAndFactoryWrapper VersionTranslator::getIddFile(const VersionString& version) {
    IddFileAndFactoryWrapper result(IddFileType::OpenStudio);
    if (version < VersionString(openStudioVersion())) {
      OptionalIddFile iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio, version);
      if (!iddFile) {
        LOG_AND_THROW("Unable to retrieve OpenStudio Version " << version.str() << " IDD from the IddFactory.");
      }
      result = IddFileAndFactoryWrapper(*iddFile);
    }
    return result;
  }

  void VersionTranslator::update(const VersionString& startVersion) {
    auto start = m_map.find(startVersion);
    if (start != m_map.end()) {

      std::string translatedIdf;
      VersionString lastVersion("0.0.0");
      boost::optional<IddFileAndFactoryWrapper> oIddFile;
      for (auto it = m_updateMethods.begin(), itEnd = m_updateMethods.end(); it != itEnd; ++it) {
        // make sure map iteration is behaving as expected
        OS_ASSERT(lastVersion < it->first);
        lastVersion = it->first;
        if (startVersion < it->first) {
          oIddFile = getIddFile(it->first);
          translatedIdf = it->second(this, start->second, *oIddFile);
          break;
        }
      }

      if (translatedIdf.empty()) {
        LOG(Error, "Unable to complete translation from " << startVersion.str() << " to " << lastVersion.str()
                                                          << ". Unable to find and execute the appropriate update method.");
        return;
      }
      std::stringstream ss(translatedIdf);
      OptionalIdfFile oIdfFile;
      if (oIddFile->iddFileType() == IddFileType::UserCustom) {
        oIdfFile = IdfFile::load(ss, oIddFile->iddFile());
      } else {
        oIdfFile = IdfFile::load(ss, oIddFile->iddFileType());
      }
      if (!oIdfFile) {
        LOG(Error, "Unable to complete translation from " << startVersion.str() << " to " << lastVersion.str()
                                                          << ". Could not load translated IDF using the "
                                                          << "latter version's IddFile. Translated text: " << '\n'
                                                          << translatedIdf);
        return;
      }
      IdfFile idfFile = *oIdfFile;
      if (m_isComponent) {
        updateComponentData(idfFile);
      }
      m_map[oIdfFile->version()] = idfFile;
      LOG(Debug, "Translation to " << lastVersion.str() << " model has " << oIdfFile->numObjects() << " objects.");
    }
  }

  void VersionTranslator::updateComponentData(IdfFile& idfFile) {
    if (OptionalIddObject oIddObject = idfFile.iddFile().getObject("OS:ComponentData")) {
      auto compDatas = idfFile.getObjectsByType(*oIddObject);
      if (compDatas.empty()) {
        return;
      }

      std::set<std::string> newHandles;
      std::transform(m_new.cbegin(), m_new.cend(), std::inserter(newHandles, newHandles.begin()), [](const auto& n) { return n.getString(0).get(); });

      std::set<std::string> deletedHandles;
      std::transform(m_untranslated.cbegin(), m_untranslated.cend(), std::inserter(deletedHandles, deletedHandles.begin()),
                     [](const auto& n) { return n.getString(0).get(); });
      std::transform(m_deprecated.cbegin(), m_deprecated.cend(), std::inserter(deletedHandles, deletedHandles.begin()),
                     [](const auto& n) { return n.getString(0).get(); });

      std::erase_if(newHandles, [&deletedHandles](auto& s) { return deletedHandles.contains(s); });

      // There should really be only one ComponentData object anyways, it'll throw in the Component ctor later if not...
      for (auto& compData : compDatas) {
        // We want to preserve the original order, so primary Object stays the same... so we don't use a set
        std::vector<std::string> currentHandles;
        auto egs = compData.extensibleGroups();
        std::transform(egs.cbegin(), egs.cend(), std::back_inserter(currentHandles), [](const auto& eg) { return eg.getString(0).get(); });

        std::erase_if(currentHandles, [&deletedHandles](auto& s) { return deletedHandles.contains(s); });

        currentHandles.reserve(currentHandles.size() + newHandles.size());
        // m_new isn 't cleared between distinct version updates, so we can't bindly copy everything.
        std::copy_if(
          std::make_move_iterator(newHandles.begin()), std::make_move_iterator(newHandles.end()), std::back_inserter(currentHandles),
          [&currentHandles](const auto& elem) { return std::find(currentHandles.begin(), currentHandles.end(), elem) == currentHandles.end(); });

        compData.clearExtensibleGroups();
        for (auto& handle : currentHandles) {
          compData.pushExtensibleGroup({handle});
        }
      }
    }
  }

  std::string VersionTranslator::defaultUpdate(const IdfFile& idf, const IddFileAndFactoryWrapper& targetIdd) {
    // use for version increments with no IDD changes
    std::stringstream ss;

    ss << idf.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(targetIdd.iddFile());
    ss << targetIdf.versionObject().get();

    // all other objects
    for (const IdfObject& object : idf.objects()) {
      ss << object;
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_7_1_to_0_7_2(const IdfFile& idf_0_7_1, const IddFileAndFactoryWrapper& idd_0_7_2) {
    // Url field refinements
    std::stringstream ss;

    ss << idf_0_7_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_7_2.iddFile());
    ss << targetIdf.versionObject().get();

    // all other objects
    for (const IdfObject& object : idf_0_7_1.objects()) {
      IdfObject toPrint = object;
      if (object.iddObject().name() == "OS:WeatherFile") {
        toPrint = updateUrlField_0_7_1_to_0_7_2(object, 9);
      } else if (object.iddObject().name() == "OS:Construction:WindowDataFile") {
        toPrint = updateUrlField_0_7_1_to_0_7_2(object, 1);
      } else if (object.iddObject().name() == "OS:Luminaire:Definition") {
        toPrint = updateUrlField_0_7_1_to_0_7_2(object, 1);
      }

      ss << toPrint;
    }

    return ss.str();
  }

  IdfObject VersionTranslator::updateUrlField_0_7_1_to_0_7_2(const IdfObject& object, unsigned index) {
    IdfObject result = object;
    if (OptionalString os = object.getString(index)) {
      std::string original = *os;
      if (!original.empty()) {
        std::string transformed = toString(toPath(original));
        if (!transformed.empty()) {
          IdfObject objCopy = object.clone();
          bool ok = objCopy.setString(index, transformed);
          OS_ASSERT(ok);
          result = objCopy;
        } else {
          LOG(Warn, "Tried to update the file path '" << original << "' to the new format, " << "but was unsuccessful.");
        }
      }
    }
    return result;
  }

  std::string VersionTranslator::update_0_7_2_to_0_7_3(const IdfFile& idf_0_7_2, const IddFileAndFactoryWrapper& idd_0_7_3) {
    // use for version increments with no IDD changes
    std::stringstream ss;

    ss << idf_0_7_2.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_7_3.iddFile());
    ss << targetIdf.versionObject().get();

    // all other objects
    for (const IdfObject& object : idf_0_7_2.objects()) {
      if (istringEqual(object.iddObject().name(), "OS:PlantLoop")) {
        // ETH@20120514 Kyle - Please refine/completely rework this as appropriate.
        LOG(Warn, "This model contains an out-of-date " << object.iddObject().name() << " object. "
                                                        << "In particular, it needs a bypass branch added in order to run properly in EnergyPlus.");
      }
      ss << object;
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_7_3_to_0_7_4(const IdfFile& idf_0_7_3, const IddFileAndFactoryWrapper& idd_0_7_4) {
    std::stringstream ss;
    IddObject componentDataIdd = idd_0_7_4.getObject("OS:ComponentData").get();
    IdfObject componentDataIdf(componentDataIdd);
    int fs = IdfObject::printedFieldSpace();

    ss << idf_0_7_3.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_7_4.iddFile());
    ss << targetIdf.versionObject().get();

    // all other objects
    for (IdfObject& object : idf_0_7_3.objects()) {
      if (istringEqual(object.iddObject().name(), "OS:ComponentData:Tags")) {
        m_deprecated.push_back(object);
        continue;
      }
      if (istringEqual(object.iddObject().name(), "OS:ComponentData:Attributes")) {
        m_deprecated.push_back(object);
        continue;
      }

      std::stringstream objectSS;
      object.printName(objectSS);
      std::string handleStr = toString(object.handle());
      objectSS << "  " << handleStr;
      if (object.numFields() == 0) {
        objectSS << ";";
      } else {
        objectSS << ",";
      }
      for (int cnt = handleStr.size(); cnt < fs; ++cnt) {
        objectSS << " ";
      }
      objectSS << "  " << componentDataIdf.fieldComment(0, true) << '\n';

      if (istringEqual(object.iddObject().name(), "OS:ComponentData")) {
        // create new, refactored OS:ComponentData object from original data
        componentDataIdf = IdfObject(componentDataIdd);
        componentDataIdf.setName(object.name().get());
        // uuid
        OptionalString os = object.getString(1);
        if (os) {
          componentDataIdf.setString(2, *os);
        } else {
          LOG(Warn,
              "No value available for required " << object.iddObject().name() << " field " << object.iddObject().getField(1).get().name() << ".");
        }
        // version uuid
        os = object.getString(2);
        if (os) {
          componentDataIdf.setString(3, *os);
        } else {
          LOG(Warn,
              "No value available for required " << object.iddObject().name() << " field " << object.iddObject().getField(2).get().name() << ".");
        }
        // creation timestamp - leave blank because unknown
        // version timestamp - leave blank because unknown
        // object list
        StringVector handleForContents(1u);
        for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
          std::string typeStr = eg.getString(0).get();
          typeStr = boost::regex_replace(typeStr, boost::regex("_"), ":");
          std::string nameStr = eg.getString(1).get();
          OptionalIddObjectType objTyp;
          try {
            objTyp = IddObjectType(typeStr);
          } catch (...) {
          }

          // find candidates and match
          IdfObjectVector candidates;
          OptionalIdfObject match;
          if (!nameStr.empty()) {
            candidates = idf_0_7_3.getObjectsByName(nameStr);
            // narrow by object type
            auto it(candidates.begin());
            while (it != candidates.end()) {
              bool keep = true;
              if (objTyp && (it->iddObject().type() != IddObjectType::UserCustom)) {
                if (it->iddObject().type() != *objTyp) {
                  keep = false;
                }
              } else if (!istringEqual(it->iddObject().name(), typeStr)) {
                keep = false;
              }
              if (!keep) {
                it = candidates.erase(it);
                continue;
              }
              ++it;
            }
            if (!candidates.empty()) {
              match = candidates[0];
            }
          }
          if (!match) {
            // object without a name--get by type, then check first field if non-empty nameStr.
            if (OptionalIddObject oIddObject = idf_0_7_3.iddFile().getObject(typeStr)) {
              candidates = idf_0_7_3.getObjectsByType(*oIddObject);
            }
            auto it(candidates.begin());
            while (it != candidates.end()) {
              bool keep = true;
              if (!nameStr.empty()) {
                if (!it->getString(0, true) || (!istringEqual(nameStr, it->getString(0, true).get()))) {
                  keep = false;
                }
              }
              if (!keep) {
                it = candidates.erase(it);
                continue;
              }
              ++it;
            }
            if (!candidates.empty()) {
              match = candidates[0];
            }
            if (match && match->name()) {
              LOG(Warn, "Found match for object in OS:ComponentData contents list by type only, even " << "though this type of object (" << typeStr
                                                                                                       << ") has a name field.");
            }
          }

          // raise warnings and errors
          if (!match) {
            if (eg.groupIndex() == 0u) {
              LOG(Error, "Unable to locate primary object in contents list of " << '\n'
                                                                                << object
                                                                                << " Will throw this OS:ComponentData object out of the model.");
              break;
            } else {
              LOG(Warn, "Unable to locate object in OS:ComponentData contents list called out "
                          << "as object type '" << typeStr << "', and with name '" << nameStr
                          << "'. Skipping this object (that is, removing it from the Component " << "definition).");
              continue;
            }
          }
          if (candidates.size() > 1u) {
            LOG(Warn, "Ambiguous match for object in OS:ComponentData contents list called out "
                        << "as object type '" << typeStr << "', and with name '" << nameStr
                        << "'. Connecting translated OS:ComponentData object to first match.");
          }

          // add match to translated contents list
          handleForContents[0] = toString(match->handle());
          IdfExtensibleGroup reg = componentDataIdf.pushExtensibleGroup(handleForContents);
          OS_ASSERT(!reg.empty());
        }

        if (componentDataIdf.numExtensibleGroups() == 0u) {
          // totally invalid OS:ComponentData object. just don't include in translated file.
          m_untranslated.push_back(object);
          continue;
        }

        for (unsigned i = 1, n = componentDataIdf.numFields(); i < n; ++i) {
          if (i == n - 1) {
            componentDataIdf.printField(objectSS, i, true);
          } else {
            componentDataIdf.printField(objectSS, i);
          }
        }
        m_refactored.emplace_back(object, componentDataIdf);
      } else {
        // modify the object if necessary
        if (istringEqual(object.iddObject().name(), "OS:Connection")) {

          // increment inlet port as other object's fields have shifted
          boost::optional<int> portId = object.getInt(2);
          if (portId) {
            object.setInt(2, portId.get() + 1);
          } else {
            LOG(Warn, "OS:Connection object does not specify Inlet Port");

            // remove the object?
            //m_untranslated.push_back(object);
            //continue;
          }

          // increment outlet port as other object's fields have shifted
          portId = object.getInt(4);
          if (portId) {
            object.setInt(4, portId.get() + 1);
          } else {
            LOG(Warn, "OS:Connection object does not specify Outlet Port");

            // remove the object?
            //m_untranslated.push_back(object);
            //continue;
          }
        }

        // loop over all the fields
        for (unsigned i = 0, n = object.numFields(); i < n; ++i) {
          if (i == n - 1) {
            object.printField(objectSS, i, true);
          } else {
            object.printField(objectSS, i);
          }
        }
      }

      ss << objectSS.str();
    }

    return ss.str();
  }

  std::vector<std::shared_ptr<VersionTranslator::InterobjectIssueInformation>>
    VersionTranslator::fixInterobjectIssuesStage1(model::Model& model, const VersionString& startVersion) {
    OS_ASSERT(model.strictnessLevel() == StrictnessLevel::Minimal);
    std::vector<std::shared_ptr<InterobjectIssueInformation>> result;

    if (startVersion < VersionString("0.8.4")) {
      std::shared_ptr<InterobjectIssueInformation> info = fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model);
      result.push_back(info);
    }

    return result;
  }

  void VersionTranslator::fixInterobjectIssuesStage2(model::Model& model,
                                                     std::vector<std::shared_ptr<InterobjectIssueInformation>>& stage1Information) {
    OS_ASSERT(model.strictnessLevel() == StrictnessLevel::Draft);

    for (std::shared_ptr<InterobjectIssueInformation>& info : stage1Information) {
      if (info->endVersion == VersionString("0.8.4")) {
        fixInterobjectIssuesStage2_0_8_3_to_0_8_4(model, info);
      }
    }
  }

  std::shared_ptr<VersionTranslator::InterobjectIssueInformation> VersionTranslator::fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model::Model& model) {

    auto result = std::make_shared<InterobjectIssueInformation_0_8_3_to_0_8_4>();

    // deal with component data
    model::ComponentDataVector allComponentData = model.getConcreteModelObjects<model::ComponentData>();
    std::vector<IdfObject> allIdfComponentData;  // make sure idf versions share data
    std::vector<model::ModelObjectVector> allComponentDataObjects;
    for (const model::ComponentData& cd : allComponentData) {
      allComponentDataObjects.push_back(cd.componentObjects());
      allIdfComponentData.push_back(cd.idfObject());
    }
    std::set<int> found;
    // loop through all schedules
    model::ScheduleVector schedules = model.getModelObjects<model::Schedule>();
    for (model::Schedule& schedule : schedules) {
      // get users
      model::ModelObjectVector users = schedule.getModelObjectSources<model::ModelObject>();
      if (!users.empty()) {
        if (!schedule.scheduleTypeLimits() || !schedule.isValid(StrictnessLevel::Draft)) {
          LOG(Debug, "Adding Schedule '" << schedule.name().get() << "' to the list of schedules that need fix-up.");
          result->schedules.push_back(schedule);
          auto& thisUsers = result->users.emplace_back(std::move(users));
          // keep separate track of component data objects
          auto& thisComponentDataObjects = result->componentDataObjects.emplace_back();
          IdfObjectVector thisSchedulesComponentData;
          for (int i = 0, n = allComponentData.size(); i < n; ++i) {
            model::ModelObjectVector objs = allComponentDataObjects[i];
            if (std::find(objs.begin(), objs.end(), schedule) != objs.end()) {
              thisSchedulesComponentData.push_back(allIdfComponentData[i]);
              found.insert(i);
            }
          }
          for (IdfObject& cd : thisSchedulesComponentData) {
            auto it = std::find_if(thisUsers.begin(), thisUsers.end(),
                                   [&cd](const IdfObject& idfObject) { return handleEquals<IdfObject, Handle>(idfObject, cd.handle()); });
            if (it != thisUsers.end()) {
              thisUsers.erase(it);
            }
            auto jit = std::find(allIdfComponentData.begin(), allIdfComponentData.end(), cd);
            OS_ASSERT(jit != allIdfComponentData.end());
            thisComponentDataObjects.push_back(*jit);
          }

          std::vector<std::vector<unsigned>> thisSchedulesIndices;
          std::vector<std::vector<model::ScheduleTypeKey>> thisSchedulesKeys;
          auto it = thisUsers.begin();
          while (it != thisUsers.end()) {
            UnsignedVector thisUsersIndices = it->getSourceIndices(schedule.handle());
            std::vector<model::ScheduleTypeKey> thisUsersKeys = it->getScheduleTypeKeys(schedule);

            if (thisUsersKeys.empty()) {
              it = thisUsers.erase(it);
              continue;
            }

            OS_ASSERT(thisUsersIndices.size() == thisUsersKeys.size());
            for (unsigned index : thisUsersIndices) {
              it->setString(index, "");  // clear for now--only change to be made at none strictness
            }
            thisSchedulesIndices.push_back(thisUsersIndices);
            thisSchedulesKeys.push_back(thisUsersKeys);
            if (std::find_if(result->originalUsers.begin(), result->originalUsers.end(),
                             [&it](const IdfObject& idfObject) { return handleEquals<IdfObject, Handle>(idfObject, it->handle()); })
                == result->originalUsers.end()) {
              result->originalUsers.push_back(it->idfObject());
            }
            ++it;
          }
          result->indices.push_back(thisSchedulesIndices);
          result->keys.push_back(thisSchedulesKeys);
        }
      }
    }

    // remove component data objects from model--replace in Stage 2
    for (int index : found) {
      m_untranslated.push_back(allIdfComponentData[index]);
      if (allComponentData[index].initialized()) {
        allComponentData[index].remove();
      }
    }

    return result;
  }

  void VersionTranslator::fixInterobjectIssuesStage2_0_8_3_to_0_8_4(model::Model& model, std::shared_ptr<InterobjectIssueInformation>& info) {
    std::shared_ptr<InterobjectIssueInformation_0_8_3_to_0_8_4> schedulesToFixup =
      std::dynamic_pointer_cast<InterobjectIssueInformation_0_8_3_to_0_8_4>(info);

    // make sure ScheduleDays are owned by their ScheduleRules and ScheduleRulesets
    model::ScheduleDayVector daySchedules = model.getConcreteModelObjects<model::ScheduleDay>();
    for (model::ScheduleDay& daySchedule : daySchedules) {
      model::ScheduleRulesetVector rulesetUsers = daySchedule.getModelObjectSources<model::ScheduleRuleset>();
      model::ScheduleRuleVector ruleUsers = daySchedule.getModelObjectSources<model::ScheduleRule>();
      if (rulesetUsers.size() + ruleUsers.size() > 1u) {
        model::OptionalModelObject primaryUser;
        if (!rulesetUsers.empty()) {
          primaryUser = rulesetUsers[0];
          for (unsigned i = 0, ni = rulesetUsers.size(); i < ni; ++i) {
            UnsignedVector indices = rulesetUsers[i].getSourceIndices(daySchedule.handle());
            unsigned j = 0;
            if (i == 0) {
              j = 1;
            }
            for (unsigned nj = indices.size(); j < nj; ++j) {
              model::ModelObject dayScheduleClone = daySchedule.clone();
              rulesetUsers[i].setPointer(indices[j], dayScheduleClone.handle());
            }
          }
        }
        for (auto& ruleUser : ruleUsers) {
          UnsignedVector indices = ruleUser.getSourceIndices(daySchedule.handle());
          unsigned j = 0;
          if (!primaryUser) {
            primaryUser = ruleUser;
            j = 1;
          }
          for (unsigned nj = indices.size(); j < nj; ++j) {
            model::ModelObject dayScheduleClone = daySchedule.clone();
            ruleUser.setPointer(indices[j], dayScheduleClone.handle());
          }
        }
      }
    }

    unsigned nSched = schedulesToFixup->schedules.size();
    OS_ASSERT(schedulesToFixup->users.size() == nSched);
    OS_ASSERT(schedulesToFixup->indices.size() == nSched);
    OS_ASSERT(schedulesToFixup->keys.size() == nSched);
    OS_ASSERT(schedulesToFixup->componentDataObjects.size() == nSched);

    for (unsigned i = 0; i < nSched; ++i) {

      // resolve schedule type limits
      model::Schedule schedule = schedulesToFixup->schedules[i];
      LOG(Debug, "Fixing up Schedule '" << schedule.name().get() << "'.");
      model::OptionalScheduleTypeLimits originalScheduleLimits = schedule.scheduleTypeLimits();
      model::OptionalScheduleTypeLimits scheduleLimits(originalScheduleLimits);
      IdfObject originalSchedule = schedule.idfObject();
      model::ScheduleVector candidates;  // vector of clones
      bool okToModifySchedule(true);

      unsigned nUsers = schedulesToFixup->users[i].size();
      OS_ASSERT(schedulesToFixup->indices[i].size() == nUsers);
      OS_ASSERT(schedulesToFixup->keys[i].size() == nUsers);

      for (unsigned j = 0; j < nUsers; ++j) {
        model::ModelObject user = schedulesToFixup->users[i][j];
        UnsignedVector indices = schedulesToFixup->indices[i][j];
        std::vector<model::ScheduleTypeKey> keys = schedulesToFixup->keys[i][j];

        OS_ASSERT(!keys.empty());

        unsigned n = keys.size();
        OS_ASSERT(indices.size() == n);
        for (unsigned k = 0, n = keys.size(); k < n; ++k) {
          bool ok(false);
          if (scheduleLimits) {
            ok = isCompatible(keys[k].className(), keys[k].scheduleDisplayName(), *scheduleLimits);
          }
          if (ok) {
            ok = user.setPointer(indices[k], schedule.handle());
            OS_ASSERT(ok);
            okToModifySchedule = false;
          } else {
            for (model::Schedule& candidate : candidates) {
              if (model::OptionalScheduleTypeLimits limits = candidate.scheduleTypeLimits()) {
                if (isCompatible(keys[k].className(), keys[k].scheduleDisplayName(), *limits)) {
                  user.setPointer(indices[k], candidate.handle());
                  schedulesToFixup->refactoredUsers.insert(user);
                  ok = true;
                  break;
                }
              }
            }
          }

          if (!ok) {
            unsigned modelN = model.numObjects();
            if (okToModifySchedule) {
              if (scheduleLimits) {
                ok = schedule.resetScheduleTypeLimits();
                OS_ASSERT(ok);  // unhooked schedule in Stage 1
              }
              ok = checkOrAssignScheduleTypeLimits(keys[k].className(), keys[k].scheduleDisplayName(), schedule);
              OS_ASSERT(ok);
              scheduleLimits = schedule.scheduleTypeLimits();
              if (model.numObjects() > modelN) {
                m_new.push_back(schedule.scheduleTypeLimits().get().idfObject());
              }
              okToModifySchedule = false;
              ok = user.setPointer(indices[k], schedule.handle());
              OS_ASSERT(ok);
            } else {
              auto clonedSchedule = schedule.clone().cast<model::Schedule>();
              ok = clonedSchedule.resetScheduleTypeLimits();
              OS_ASSERT(ok);
              modelN = model.numObjects();
              ok = checkOrAssignScheduleTypeLimits(keys[k].className(), keys[k].scheduleDisplayName(), clonedSchedule);
              OS_ASSERT(ok);
              if (model.numObjects() > modelN) {
                m_new.push_back(clonedSchedule.scheduleTypeLimits().get().idfObject());
              }
              ok = user.setPointer(indices[k], clonedSchedule.handle());
              candidates.push_back(clonedSchedule);
            }
          }
        }  // for keys
      }  // for users
      m_refactored.emplace_back(originalSchedule, schedule.idfObject());
      for (const auto& candidate : candidates) {
        model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidate);
        m_new.push_back(candidate.idfObject());
        m_new.push_back(candidate.scheduleTypeLimits().get().idfObject());
        for (unsigned k = 1, nc = wholeCandidate.size(); k < nc; ++k) {
          m_new.push_back(wholeCandidate[k].idfObject());
        }
      }
      // start to fix-up component data--add objects
      std::set<model::ModelObject> usersSet(schedulesToFixup->users[i].begin(), schedulesToFixup->users[i].end());
      for (IdfObject& cd : schedulesToFixup->componentDataObjects[i]) {
        LOG(Debug, "Adding ScheduleTypeLimits '" << scheduleLimits->name().get() << "' to component data list.");
        cd.pushExtensibleGroup(StringVector(1u, toString(scheduleLimits->handle())));
        for (const auto& candidate : candidates) {
          model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidate);
          LOG(Debug, "Adding Schedule '" << candidate.name().get() << "', its ScheduleTypeLimits, and all children to component data list.");
          cd.pushExtensibleGroup(StringVector(1u, toString(candidate.handle())));
          cd.pushExtensibleGroup(StringVector(1u, toString(candidate.scheduleTypeLimits()->handle())));
          for (unsigned k = 1, nc = wholeCandidate.size(); k < nc; ++k) {
            cd.pushExtensibleGroup(StringVector(1u, toString(wholeCandidate[k].handle())));
          }
        }
      }
      // remove original schedule type limits if now unused
      if (originalScheduleLimits) {
        OS_ASSERT(*originalScheduleLimits != *scheduleLimits);
        if (originalScheduleLimits->sources().empty()) {
          m_untranslated.push_back(originalScheduleLimits->idfObject());
          originalScheduleLimits->remove();
        }
      }
    }  // for schedules

    // clean up component data
    std::set<IdfObject, IdfObjectImplLess> uniqueComponentData;
    for (const std::vector<IdfObject>& scheduleCD : schedulesToFixup->componentDataObjects) {
      for (const IdfObject& cd : scheduleCD) {
        uniqueComponentData.insert(cd);
      }
    }
    IdfObjectVector uniqueComponentDataVector(uniqueComponentData.begin(), uniqueComponentData.end());
    for (IdfObject& cd : uniqueComponentDataVector) {
      // remove duplicates and objects not in model
      model::ModelObjectVector componentObjects;
      unsigned i = 0;
      while (i < cd.numExtensibleGroups()) {
        Handle h = toUUID(*cd.getExtensibleGroup(i).getString(0));
        if (std::find_if(componentObjects.begin(), componentObjects.end(),
                         [&h](const model::ModelObject& mo) { return handleEquals<model::ModelObject, Handle>(mo, h); })
            != componentObjects.end()) {
          cd.eraseExtensibleGroup(i);
          continue;
        }
        if (OptionalWorkspaceObject object = model.getObject(h)) {
          componentObjects.push_back(object->cast<model::ModelObject>());
        } else {
          cd.eraseExtensibleGroup(i);
          continue;
        }
        ++i;
      }

      // remove schedules that have no users in the component
      model::ModelObjectVector toRemove;
      std::sort(componentObjects.begin(), componentObjects.end(), IdfObjectImplLess());
      for (unsigned i = 0, n = componentObjects.size(); i < n; ++i) {
        if (model::OptionalSchedule schedule = componentObjects[i].optionalCast<model::Schedule>()) {
          if (i == 0) {
            continue;
          }
          model::ModelObjectVector users = castVector<model::ModelObject>(schedule->sources());
          std::sort(users.begin(), users.end(), IdfObjectImplLess());
          model::ModelObjectVector children = model::getRecursiveChildren(*schedule);
          std::sort(children.begin(), children.end(), IdfObjectImplLess());
          model::ModelObjectVector intersection(users.size(), componentObjects[0]);
          model::ModelObjectVector result(users.size(), componentObjects[0]);
          // intersect users and componentObjects
          auto intersectionEnd = std::set_intersection(users.begin(), users.end(), componentObjects.begin(), componentObjects.end(),
                                                       intersection.begin(), IdfObjectImplLess());
          // subtract children from intersection
          auto resultEnd =
            std::set_difference(intersection.begin(), intersectionEnd, children.begin(), children.end(), result.begin(), IdfObjectImplLess());
          if (int(resultEnd - result.begin()) == 0) {
            toRemove.insert(toRemove.end(), children.begin(), children.end());
          }
        }
      }

      auto it = componentObjects.begin();
      i = 0;
      while (it != componentObjects.end()) {
        if (std::find(toRemove.begin(), toRemove.end(), *it) != toRemove.end()) {
          cd.eraseExtensibleGroup(i);
          it = componentObjects.erase(it);
          continue;
        }
        ++it;
        ++i;
      }

      // remove schedule type limits that have no users in the component
      it = componentObjects.begin();
      i = 0;
      while (it != componentObjects.end()) {
        if (model::OptionalScheduleTypeLimits limits = it->optionalCast<model::ScheduleTypeLimits>()) {
          model::ModelObjectVector users = castVector<model::ModelObject>(limits->sources());
          std::sort(users.begin(), users.end(), IdfObjectImplLess());
          model::ModelObjectVector intersection(users.size(), componentObjects[0]);
          auto intersectionEnd = std::set_intersection(componentObjects.begin(), componentObjects.end(), users.begin(), users.end(),
                                                       intersection.begin(), IdfObjectImplLess());
          if (int(intersectionEnd - intersection.begin()) == 0) {
            cd.eraseExtensibleGroup(i);
            it = componentObjects.erase(it);
            continue;
          }
        }
        ++it;
        ++i;
      }

      // add component data back to model and mark as refactored
      OptionalWorkspaceObject restoredComponentData = model.addObject(cd);
      OS_ASSERT(restoredComponentData);
      restoredComponentData->cast<model::ComponentData>().createVersionUUID();
      m_new.push_back(restoredComponentData->idfObject());
    }

    // populate m_refactored
    model::ModelObjectVector refactoredUsersVector(schedulesToFixup->refactoredUsers.begin(), schedulesToFixup->refactoredUsers.end());
    for (model::ModelObject& user : refactoredUsersVector) {
      auto it = std::find_if(schedulesToFixup->originalUsers.begin(), schedulesToFixup->originalUsers.end(),
                             [&user](const IdfObject& idfObject) { return handleEquals<IdfObject, Handle>(idfObject, user.handle()); });
      OS_ASSERT(it != schedulesToFixup->originalUsers.end());
      m_refactored.emplace_back(*it, user.idfObject());
    }
  }

  std::string VersionTranslator::update_0_9_1_to_0_9_2(const IdfFile& idf_0_9_1, const IddFileAndFactoryWrapper& idd_0_9_2) {
    // use for version increments with no IDD changes
    std::stringstream ss;

    ss << idf_0_9_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_9_2.iddFile());
    ss << targetIdf.versionObject().get();

    // Fixup all thermal zone objects
    for (const IdfObject& object : idf_0_9_1.objects()) {
      if (object.iddObject().name() == "OS:ThermalZone") {
        boost::optional<std::string> s;

        IdfObject newInletPortList(idd_0_9_2.getObject("OS:PortList").get());
        newInletPortList.setString(0, toString(createUUID()));

        IdfObject newExhaustPortList(idd_0_9_2.getObject("OS:PortList").get());
        newExhaustPortList.setString(0, toString(createUUID()));

        IdfObject newZoneHVACEquipmentList(idd_0_9_2.getObject("OS:ZoneHVAC:EquipmentList").get());

        IdfObject newThermalZone(idd_0_9_2.getObject("OS:ThermalZone").get());

        boost::optional<IdfObject> fanPowerTerminal;
        boost::optional<IdfObject> newFPTSecondaryInletConn;

        for (unsigned i = 0; i < object.numFields(); i++) {
          s = object.getString(i);
          if (s) {
            switch (i) {
              case 0:  // Handle
              {
                newZoneHVACEquipmentList.setString(2, s.get());

                newThermalZone.setString(i, s.get());

                break;
              }
              case 9:  // Inlet Port List
              {
                newThermalZone.setString(i, newInletPortList.getString(0).get());

                newInletPortList.setString(2, s.get());

                // Find the connection object associated with the inlet port
                std::vector<IdfObject> connections = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Connection").get());
                for (auto& connection : connections) {
                  if (connection.getString(0).get() == s.get()) {
                    // Fixup the connection's target object and port
                    connection.setString(4, newInletPortList.getString(0).get());
                    connection.setUnsigned(5, 2);

                    // Find the inlet model object to the connection - assume it is a node
                    if (boost::optional<std::string> sourceObject1Handle = connection.getString(2)) {
                      std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                      for (const auto& node : nodes) {
                        if (sourceObject1Handle.get() == node.getString(0).get()) {
                          // Find the connection object associated with the inlet node
                          if (boost::optional<std::string> nodeInletConnectionHandle = node.getString(2)) {
                            for (const auto& connection2 : connections) {
                              if (nodeInletConnectionHandle.get() == connection2.getString(0).get()) {
                                // Find the inlet model object to connection2
                                if (boost::optional<std::string> sourceObject2Handle = connection2.getString(2)) {
                                  std::vector<IdfObject> objects = idf_0_9_1.objects();
                                  for (const auto& object2 : objects) {
                                    if (object2.getString(0).get() == sourceObject2Handle.get()) {
                                      if (object2.iddObject() != idf_0_9_1.iddFile().getObject("OS:AirLoopHVAC:ZoneSplitter").get()) {
                                        // If there is zone equipment add it to the zone equipment list
                                        newZoneHVACEquipmentList.setString(3, sourceObject2Handle.get());
                                        newZoneHVACEquipmentList.setUnsigned(4, 1);
                                        newZoneHVACEquipmentList.setUnsigned(5, 1);
                                      }

                                      if (object2.iddObject()
                                          == idf_0_9_1.iddFile().getObject("OS:AirTerminal:SingleDuct:ParallelPIU:Reheat").get()) {
                                        fanPowerTerminal = object2;
                                      }

                                      break;
                                    }
                                  }
                                }
                                break;
                              }
                            }
                          }
                          break;
                        }
                      }
                    }
                    break;
                  }
                }

                break;
              }
              case 10:  // Exhaust Port List
              {
                newThermalZone.setString(i, newExhaustPortList.getString(0).get());

                // Find the connection object associated with the exhaust port
                std::vector<IdfObject> connections = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Connection").get());
                for (auto& connection : connections) {
                  if (connection.getString(0).get() == s.get()) {
                    // Find the target model object of the connection - assume it is a node
                    if (boost::optional<std::string> target1Handle = connection.getString(4)) {
                      std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                      for (auto& node : nodes) {
                        if (target1Handle.get() == node.getString(0).get()) {
                          // Does the exhaust node have a outlet connection?
                          if (boost::optional<std::string> connection2Handle = node.getString(3)) {
                            if (connection2Handle && !connection2Handle.get().empty()) {
                              // We fix up the connection to the thermal zone.
                              // Now actually it is a connection to the port list.
                              newExhaustPortList.setString(2, s.get());
                              connection.setString(2, newExhaustPortList.getString(0).get());
                              connection.setUnsigned(3, 2);
                            }
                          } else if (fanPowerTerminal) {
                            newExhaustPortList.setString(2, s.get());
                            connection.setString(2, newExhaustPortList.getString(0).get());
                            connection.setUnsigned(3, 2);

                            newFPTSecondaryInletConn = IdfObject(idd_0_9_2.getObject("OS:Connection").get());
                            newFPTSecondaryInletConn->setString(0, toString(createUUID()));

                            newFPTSecondaryInletConn->setString(2, node.getString(0).get());
                            newFPTSecondaryInletConn->setUnsigned(3, 3);
                            newFPTSecondaryInletConn->setString(4, fanPowerTerminal->getString(0).get());
                            newFPTSecondaryInletConn->setUnsigned(5, 8);

                            node.setString(3, newFPTSecondaryInletConn->getString(0).get());
                            fanPowerTerminal->setString(8, newFPTSecondaryInletConn->getString(0).get());
                          } else {
                            // Exhaust nodes don't exist by default anymore.
                            // They are only needed if there is zone equipment hooked up to them.
                            // We break the connection to the thermal zone.
                            connection.setString(2, "");
                            connection.setString(3, "");
                            connection.setString(4, "");
                            connection.setString(5, "");

                            node.setString(2, "");
                            node.setString(3, "");
                          }
                          break;
                        }
                      }
                    }
                    break;
                  }
                }
                break;
              }
              default: {
                newThermalZone.setString(i, s.get());
                break;
              }
            }
          }
        }

        ss << newThermalZone;
        ss << newInletPortList;
        ss << newExhaustPortList;
        ss << newZoneHVACEquipmentList;

        m_new.push_back(newInletPortList);
        m_new.push_back(newExhaustPortList);
        m_new.push_back(newZoneHVACEquipmentList);

        if (newFPTSecondaryInletConn) {
          ss << newFPTSecondaryInletConn.get();
        }
      }
    }

    for (const IdfObject& object : idf_0_9_1.objects()) {
      if (object.iddObject().name() != "OS:ThermalZone") {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_9_5_to_0_9_6(const IdfFile& idf_0_9_5, const IddFileAndFactoryWrapper& idd_0_9_6) {
    // if multiple OS:RunPeriod objects remove them all
    bool skipRunPeriods = false;
    unsigned numRunPeriods = 0;
    for (const IdfObject& object : idf_0_9_5.objects()) {
      if (object.iddObject().name() == "OS:RunPeriod") {
        ++numRunPeriods;

        if (numRunPeriods > 1) {
          LOG(Warn, "Multiple OS:RunPeriod objects are no longer supported, these have been removed");
          skipRunPeriods = true;
          break;
        }
      }
    }

    // use for version increments with no IDD changes
    std::stringstream ss;

    ss << idf_0_9_5.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_9_6.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_0_9_5.objects()) {
      if (object.iddObject().name() == "OS:PlantLoop") {
        IdfObject newSizingPlant(idd_0_9_6.getObject("OS:Sizing:Plant").get());

        newSizingPlant.setString(0, toString(createUUID()));

        newSizingPlant.setString(1, object.getString(0).get());

        newSizingPlant.setString(2, "Heating");

        newSizingPlant.setDouble(3, 0.001);

        newSizingPlant.setDouble(4, 0.001);

        ss << newSizingPlant;

        m_new.push_back(newSizingPlant);

        ss << object;
      } else if (object.iddObject().name() == "OS:Sizing:Parameters") {
        IdfObject newSizingParameters = object.clone(true);

        if (!object.getDouble(1)) {
          newSizingParameters.setDouble(1, 1.25);
        }

        if (!object.getDouble(2)) {
          newSizingParameters.setDouble(2, 1.15);
        }

        ss << newSizingParameters;
      } else if (object.iddObject().name() == "OS:RunPeriod") {
        if (skipRunPeriods) {
          // put the object in the untranslated list
          m_untranslated.push_back(object);
        } else {
          ss << object;
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_9_6_to_0_10_0(const IdfFile& idf_0_9_6, const IddFileAndFactoryWrapper& idd_0_10_0) {
    std::stringstream ss;

    ss << idf_0_9_6.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_10_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_0_9_6.objects()) {

      if (object.iddObject().name() == "OS:RadianceParameters") {
        boost::optional<std::string> value = object.getString(14);

        if (!value || *value == "146" || *value == "581" || *value == "2321") {
          ss << object;
        } else {
          IdfObject newParameters = object.clone(true);
          newParameters.setString(14, "");

          ss << newParameters;
          m_refactored.emplace_back(std::move(object), std::move(newParameters));
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_11_0_to_0_11_1(const IdfFile& idf_0_11_0, const IddFileAndFactoryWrapper& idd_0_11_1) {
    // use for version increments with no IDD changes
    std::stringstream ss;

    ss << idf_0_11_0.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_11_1.iddFile());
    ss << targetIdf.versionObject().get();

    // hold OS:ComponentData objects for later
    std::vector<IdfObject> componentDataObjects;
    std::vector<std::string> removedComponentCostLineItemHandles;

    for (const IdfObject& object : idf_0_11_0.objects()) {
      if (object.iddObject().name() == "OS:ComponentCost:LineItem") {
        removedComponentCostLineItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:ComponentData") {
        componentDataObjects.push_back(object);
      } else {
        ss << object;
      }
    }

    if (!removedComponentCostLineItemHandles.empty()) {
      LOG(Warn, "OS:ComponentCost:LineItem objects created before 0.11.1 are no longer supported, " << removedComponentCostLineItemHandles.size()
                                                                                                    << " objects have been removed.");
    }

    // remove these handles from any component data objects
    // DLM: this is probably a standard thing we want to do in every version translation function
    for (const IdfObject& componentDataObject : componentDataObjects) {

      // if object was primary component remove the component data completely
      boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
      if (primaryObjectHandle) {
        if (std::find(removedComponentCostLineItemHandles.begin(), removedComponentCostLineItemHandles.end(), *primaryObjectHandle)
            != removedComponentCostLineItemHandles.end()) {
          LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
          m_untranslated.push_back(componentDataObject);
          continue;
        }
      }

      // translate base fields
      componentDataObject.printName(ss, true);
      componentDataObject.printField(ss, 0, false);  // Handle
      componentDataObject.printField(ss, 1, false);  // Name
      componentDataObject.printField(ss, 2, false);  // UUID
      componentDataObject.printField(ss, 3, false);  // Version UUID
      componentDataObject.printField(ss, 4, false);  // Creation Timestamp
      componentDataObject.printField(ss, 5, false);  // Version Timestamp

      // make list of fields to keep
      std::vector<unsigned> extensibleIndicesToKeep;
      for (unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i) {
        boost::optional<std::string> objectHandle = componentDataObject.getString(i);
        if (objectHandle) {
          if (std::find(removedComponentCostLineItemHandles.begin(), removedComponentCostLineItemHandles.end(), *objectHandle)
              == removedComponentCostLineItemHandles.end()) {
            extensibleIndicesToKeep.push_back(i);
          }
        }
      }

      // write out remaining fields
      for (auto it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it) {
        if (it == itend - 1) {
          componentDataObject.printField(ss, *it, true);
        } else {
          componentDataObject.printField(ss, *it, false);
        }
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_11_1_to_0_11_2(const IdfFile& idf_0_11_1, const IddFileAndFactoryWrapper& idd_0_11_2) {
    // This version update has two things to do.
    // Make updates for new control related objects.
    // Make updates for component costs.

    std::stringstream ss;

    ss << idf_0_11_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_11_2.iddFile());
    ss << targetIdf.versionObject().get();

    // hold OS:ComponentData objects for later
    std::vector<IdfObject> componentDataObjects;
    std::vector<std::string> removedItemHandles;

    unsigned numComponentCostAdjustment = 0;
    unsigned numComponentCostAdjustmentRemoved = 0;
    unsigned numComponentCostReferenceRemoved = 0;
    unsigned numLifeCycleCostUseAdjustmentRemoved = 0;

    boost::optional<IdfObject> alwaysOnSchedule;

    if (!m_isComponent) {
      // Add an always on discrete schedule if one does not already exist

      for (const IdfObject& object : idf_0_11_1.objects()) {
        if (object.iddObject().name() == "OS:Schedule:Constant") {
          if (boost::optional<std::string> name = object.getString(1)) {
            if (istringEqual(name.get(), "Always On Discrete")) {
              if (boost::optional<double> value = object.getDouble(3)) {
                if (equal<double>(value.get(), 1.0)) {
                  alwaysOnSchedule = object;
                }
              }
            }
          }
        }
      }

      if (!alwaysOnSchedule) {
        alwaysOnSchedule = IdfObject(idd_0_11_2.getObject("OS:Schedule:Constant").get());

        alwaysOnSchedule->setString(0, toString(createUUID()));

        alwaysOnSchedule->setString(1, "Always On Discrete");

        alwaysOnSchedule->setDouble(3, 1.0);

        IdfObject typeLimits(idd_0_11_2.getObject("OS:ScheduleTypeLimits").get());

        typeLimits.setString(0, toString(createUUID()));

        typeLimits.setString(1, "Always On Discrete Limits");

        typeLimits.setDouble(2, 0.0);

        typeLimits.setDouble(3, 1.0);

        typeLimits.setString(4, "Discrete");

        typeLimits.setString(5, "Availability");

        alwaysOnSchedule->setString(2, typeLimits.getString(0).get());

        ss << alwaysOnSchedule.get();

        ss << typeLimits;

        m_new.push_back(alwaysOnSchedule.get());

        m_new.push_back(typeLimits);
      }
    }

    for (const IdfObject& object : idf_0_11_1.objects()) {
      // There are no official components with HVAC at this version.
      // We don't do this translation because it breaks tests related to OS:ComponentData when new objects are added.
      // Future version will do something better.
      if (object.iddObject().name() == "OS:Controller:OutdoorAir" && !m_isComponent) {
        IdfObject newOAController = object.clone(true);

        IdfObject newMechVentController(idd_0_11_2.getObject("OS:Controller:MechanicalVentilation").get());

        newMechVentController.setString(0, toString(createUUID()));

        newMechVentController.setString(4, "ZoneSum");

        newMechVentController.setString(2, alwaysOnSchedule->getString(0).get());

        newOAController.setString(20, newMechVentController.getString(0).get());

        ss << newOAController;

        ss << newMechVentController;

        m_new.push_back(newMechVentController);
      } else if (object.iddObject().name() == "OS:AirLoopHVAC" && !m_isComponent) {
        IdfObject newAirLoopHVAC = object.clone(true);

        IdfObject newAvailList(idd_0_11_2.getObject("OS:AvailabilityManagerAssignmentList").get());

        newAvailList.setString(0, toString(createUUID()));

        newAirLoopHVAC.setString(3, newAvailList.getString(0).get());

        IdfObject newAvailabilityManagerScheduled(idd_0_11_2.getObject("OS:AvailabilityManager:Scheduled").get());

        newAvailabilityManagerScheduled.setString(0, toString(createUUID()));

        newAvailabilityManagerScheduled.setString(2, alwaysOnSchedule->getString(0).get());

        IdfExtensibleGroup eg = newAvailList.insertExtensibleGroup(0);

        eg.setString(0, newAvailabilityManagerScheduled.getString(0).get());

        IdfObject newAvailabilityManagerNightCycle(idd_0_11_2.getObject("OS:AvailabilityManager:NightCycle").get());

        newAvailabilityManagerNightCycle.setString(0, toString(createUUID()));

        eg = newAvailList.insertExtensibleGroup(1);

        eg.setString(0, newAvailabilityManagerNightCycle.getString(0).get());

        ss << newAirLoopHVAC;

        ss << newAvailList;

        ss << newAvailabilityManagerScheduled;

        ss << newAvailabilityManagerNightCycle;

        m_new.push_back(newAvailList);

        m_new.push_back(newAvailabilityManagerScheduled);

        m_new.push_back(newAvailabilityManagerNightCycle);
      } else if (object.iddObject().name() == "OS:ComponentCost:Adjustment") {
        numComponentCostAdjustment += 1;

        // this was made unique, remove if more than 1
        if (numComponentCostAdjustment == 1) {
          ss << object;
        } else {
          numComponentCostAdjustmentRemoved += 1;
          removedItemHandles.push_back(toString(object.handle()));
          m_untranslated.push_back(object);
        }
      } else if (object.iddObject().name() == "OS:ComponentCost:Reference") {
        numComponentCostReferenceRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:LifeCycleCost:UseAdjustment") {
        numLifeCycleCostUseAdjustmentRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:LifeCycleCost:Parameters") {
        object.printName(ss, true);
        object.printField(ss, 0, false);          // Handle
        ss << "Custom, !- AnalysisType" << '\n';  // Name -> AnalysisType

        for (unsigned i = 2, imax = 12; i < imax; ++i) {
          if (i == imax - 1) {
            object.printField(ss, i, true);
          } else {
            object.printField(ss, i, false);
          }
        }
      } else if (object.iddObject().name() == "OS:ComponentData") {
        componentDataObjects.push_back(object);
      } else {
        ss << object;
      }
    }

    if (numComponentCostAdjustmentRemoved > 0) {
      LOG(Warn, "Multiple OS:ComponentCost:Adjustment objects created in a single model before 0.11.1 are no longer supported, "
                  << numComponentCostAdjustmentRemoved << " objects have been removed.");
    }
    if (numComponentCostReferenceRemoved > 0) {
      LOG(Warn, "OS:ComponentCost:Reference objects created before 0.11.1 are no longer supported, " << numComponentCostReferenceRemoved
                                                                                                     << " objects have been removed.");
    }
    if (numLifeCycleCostUseAdjustmentRemoved > 0) {
      LOG(Warn, "OS:LifeCycleCos:UseAdjustment objects created before 0.11.1 are no longer supported, " << numLifeCycleCostUseAdjustmentRemoved
                                                                                                        << " objects have been removed.");
    }

    // remove these handles from any component data objects
    // DLM: this is probably a standard thing we want to do in every version translation function
    for (const IdfObject& componentDataObject : componentDataObjects) {

      // if object was primary component remove the component data completely
      boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
      if (primaryObjectHandle) {
        if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *primaryObjectHandle) != removedItemHandles.end()) {
          LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
          m_untranslated.push_back(componentDataObject);
          continue;
        }
      }

      // translate base fields
      componentDataObject.printName(ss, true);
      componentDataObject.printField(ss, 0, false);  // Handle
      componentDataObject.printField(ss, 1, false);  // Name
      componentDataObject.printField(ss, 2, false);  // UUID
      componentDataObject.printField(ss, 3, false);  // Version UUID
      componentDataObject.printField(ss, 4, false);  // Creation Timestamp
      componentDataObject.printField(ss, 5, false);  // Version Timestamp

      // make list of fields to keep
      std::vector<unsigned> extensibleIndicesToKeep;
      for (unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i) {
        boost::optional<std::string> objectHandle = componentDataObject.getString(i);
        if (objectHandle) {
          if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *objectHandle) == removedItemHandles.end()) {
            extensibleIndicesToKeep.push_back(i);
          }
        }
      }

      // write out remaining fields
      for (auto it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it) {
        if (it == itend - 1) {
          componentDataObject.printField(ss, *it, true);
        } else {
          componentDataObject.printField(ss, *it, false);
        }
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_11_4_to_0_11_5(const IdfFile& idf_0_11_4, const IddFileAndFactoryWrapper& idd_0_11_5) {
    // Make updates for component costs.

    std::stringstream ss;

    ss << idf_0_11_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_11_5.iddFile());
    ss << targetIdf.versionObject().get();

    // hold OS:ComponentData objects for later
    std::vector<IdfObject> componentDataObjects;
    std::vector<std::string> removedItemHandles;

    unsigned numComponentCostLineItemRemoved = 0;
    unsigned numLifeCycleNonRecurringCostRemoved = 0;
    unsigned numLifeCycleRecurringCostRemoved = 0;

    for (const IdfObject& object : idf_0_11_4.objects()) {

      if (object.iddObject().name() == "OS:ComponentCost:LineItem") {
        numComponentCostLineItemRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:LifeCycleCost:NonrecurringCost") {
        numLifeCycleNonRecurringCostRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:LifeCycleCost:RecurringCosts") {
        numLifeCycleRecurringCostRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      } else if (object.iddObject().name() == "OS:ComponentData") {
        componentDataObjects.push_back(object);
      } else {
        ss << object;
      }
    }

    if (numComponentCostLineItemRemoved > 0) {
      LOG(Warn, "OS:ComponentCost:LineItem objects created before 0.11.5 are no longer supported, " << numComponentCostLineItemRemoved
                                                                                                    << " objects have been removed.");
    }
    if (numLifeCycleNonRecurringCostRemoved > 0) {
      LOG(Warn, "OS:LifeCycleCost:NonrecurringCost objects created before 0.11.5 are no longer supported, " << numLifeCycleNonRecurringCostRemoved
                                                                                                            << " objects have been removed.");
    }
    if (numLifeCycleRecurringCostRemoved > 0) {
      LOG(Warn, "OS:LifeCycleCost:RecurringCosts objects created before 0.11.5 are no longer supported, " << numLifeCycleRecurringCostRemoved
                                                                                                          << " objects have been removed.");
    }

    // remove these handles from any component data objects
    // DLM: this is probably a standard thing we want to do in every version translation function
    for (const IdfObject& componentDataObject : componentDataObjects) {

      // if object was primary component remove the component data completely
      boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
      if (primaryObjectHandle) {
        if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *primaryObjectHandle) != removedItemHandles.end()) {
          LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
          m_untranslated.push_back(componentDataObject);
          continue;
        }
      }

      // translate base fields
      componentDataObject.printName(ss, true);
      componentDataObject.printField(ss, 0, false);  // Handle
      componentDataObject.printField(ss, 1, false);  // Name
      componentDataObject.printField(ss, 2, false);  // UUID
      componentDataObject.printField(ss, 3, false);  // Version UUID
      componentDataObject.printField(ss, 4, false);  // Creation Timestamp
      componentDataObject.printField(ss, 5, false);  // Version Timestamp

      // make list of fields to keep
      std::vector<unsigned> extensibleIndicesToKeep;
      for (unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i) {
        boost::optional<std::string> objectHandle = componentDataObject.getString(i);
        if (objectHandle) {
          if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *objectHandle) == removedItemHandles.end()) {
            extensibleIndicesToKeep.push_back(i);
          }
        }
      }

      // write out remaining fields
      for (auto it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it) {
        if (it == itend - 1) {
          componentDataObject.printField(ss, *it, true);
        } else {
          componentDataObject.printField(ss, *it, false);
        }
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_0_11_5_to_0_11_6(const IdfFile& idf_0_11_5, const IddFileAndFactoryWrapper& idd_0_11_6) {
    // Update the OS:PortList object to point back to the OS:ThermalZone

    std::stringstream ss;

    ss << idf_0_11_5.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_0_11_6.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_0_11_5.objects()) {

      if (object.iddObject().name() == "OS:ThermalZone") {

        boost::optional<std::string> inletPortListString = object.getString(9);
        boost::optional<std::string> exhaustPortListString = object.getString(10);

        for (IdfObject& object2 : idf_0_11_5.objects()) {

          if (object2.iddObject().name() == "OS:PortList") {

            if (boost::optional<std::string> object2Handle = object2.getString(0)) {

              if ((inletPortListString && object2Handle.get() == inletPortListString.get())
                  || (exhaustPortListString && object2Handle.get() == exhaustPortListString.get())) {

                // Make the required change to OS:PortList

                IdfObject newPortList(idd_0_11_6.getObject("OS:PortList").get());

                boost::optional<std::string> s;

                if ((s = object2.getString(0))) {
                  newPortList.setString(0, s.get());
                }

                if ((s = object2.getString(1))) {
                  newPortList.setString(1, s.get());
                }

                if ((s = object.getString(0))) {
                  newPortList.setString(2, s.get());
                }

                for (unsigned i = 2; i < object2.numFields(); i++) {

                  if ((s = object2.getString(i))) {
                    newPortList.setString(i + 1, s.get());
                  }
                }

                ss << newPortList;
                m_refactored.emplace_back(std::move(object2), std::move(newPortList));
              }
            }
          }
        }

        ss << object;

      } else if (object.iddObject().name() == "OS:PortList") {

        continue;

      } else {

        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_0_1_to_1_0_2(const IdfFile& idf_1_0_1, const IddFileAndFactoryWrapper& idd_1_0_2) {
    std::stringstream ss;

    ss << idf_1_0_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_0_2.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_0_1.objects()) {

      if (object.iddObject().name() == "OS:Boiler:HotWater") {

        if (object.getString(15) && istringEqual(object.getString(15).get(), "VariableFlow")) {
          // Update Boiler Flow Mode

          IdfObject newBoiler = object.clone(true);

          newBoiler.setString(15, "LeavingSetpointModulated");

          ss << newBoiler;
          m_refactored.emplace_back(std::move(object), std::move(newBoiler));

        } else {

          ss << object;
        }
      } else if (object.iddObject().name() == "OS:Boiler:HotWater") {

        if (object.getString(15) && istringEqual(object.getString(15).get(), "VariableFlow")) {
          // Update Chiller Flow Mode

          IdfObject newChiller = object.clone(true);

          newChiller.setString(15, "LeavingSetpointModulated");

          ss << newChiller;
          m_refactored.emplace_back(std::move(object), std::move(newChiller));

        } else {

          ss << object;
        }

      } else {

        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_0_2_to_1_0_3(const IdfFile& idf_1_0_2, const IddFileAndFactoryWrapper& idd_1_0_3) {
    std::stringstream ss;

    ss << idf_1_0_2.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_0_3.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_0_2.objects()) {

      if (object.iddObject().name() == "OS:RadianceParameters") {
        boost::optional<std::string> value = object.getString(14);

        if (value && (*value == "581" || *value == "2321")) {
          IdfObject newParameters = object.clone(true);

          if (*value == "581") {
            newParameters.setString(14, "578");
          } else {
            newParameters.setString(14, "2306");
          }

          ss << newParameters;
          m_refactored.emplace_back(std::move(object), std::move(newParameters));
        } else {
          ss << object;
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_2_2_to_1_2_3(const IdfFile& idf_1_2_2, const IddFileAndFactoryWrapper& idd_1_2_3) {
    std::stringstream ss;

    ss << idf_1_2_2.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_2_3.iddFile());
    ss << targetIdf.versionObject().get();

    boost::optional<int> numberOfStories;
    boost::optional<int> numberOfAboveGroundStories;
    boost::optional<std::string> buildingTypeValue;
    boost::optional<IdfObject> buildingObject;

    for (IdfObject& object : idf_1_2_2.objects()) {

      if (object.iddObject().name() == "OS:StandardsInformation:Construction") {
        boost::optional<std::string> value = object.getString(2);  // Intended Surface Type

        if (value && (istringEqual(*value, "ExteriorFloor") || istringEqual(*value, "ExposedExteriorFloor"))) {
          IdfObject newObject = object.clone(true);
          if (istringEqual(*value, "ExteriorFloor")) {
            newObject.setString(2, "GroundContactFloor");
          } else {
            newObject.setString(2, "ExteriorFloor");
          }
          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          ss << object;
        }

      } else if (object.iddObject().name() == "OS:Building") {

        buildingObject = object;

      } else if (object.iddObject().name() == "OS:StandardsInformation:Building") {

        //Number of Stories
        numberOfStories = object.getInt(1);

        // Number of Above Ground Stories
        numberOfAboveGroundStories = object.getInt(2);

        // Building Type Value
        buildingTypeValue = object.getString(6);

        m_deprecated.push_back(object);

      } else {
        ss << object;
      }
    }

    if (buildingObject) {
      boost::optional<IddObject> buildingIdd = idd_1_2_3.getObject("OS:Building");
      OS_ASSERT(buildingIdd);
      IdfObject newBuildingObject(*buildingIdd);

      // Handle
      boost::optional<std::string> s = buildingObject->getString(0);
      if (s) {
        bool test = newBuildingObject.setString(0, *s);
        OS_ASSERT(test);
      }

      // Name
      s = buildingObject->getString(1);
      if (s) {
        bool test = newBuildingObject.setString(1, *s);
        OS_ASSERT(test);
      }

      // Building Sector Type
      s = buildingObject->getString(2);
      if (s) {
        bool test = newBuildingObject.setString(2, *s);
        OS_ASSERT(test);
      }

      // North Axis
      s = buildingObject->getString(3);
      if (s) {
        bool test = newBuildingObject.setString(3, *s);
        OS_ASSERT(test);
      }

      // Nominal Floor to Floor Height
      s = buildingObject->getString(4);
      if (s) {
        bool test = newBuildingObject.setString(4, *s);
        OS_ASSERT(test);
      }

      // Space Type Name
      s = buildingObject->getString(5);
      if (s) {
        bool test = newBuildingObject.setString(5, *s);
        OS_ASSERT(test);
      }

      // Default Construction Set Name
      s = buildingObject->getString(6);
      if (s) {
        bool test = newBuildingObject.setString(6, *s);
        OS_ASSERT(test);
      }

      // Default Schedule Set Name
      s = buildingObject->getString(7);
      if (s) {
        bool test = newBuildingObject.setString(7, *s);
        OS_ASSERT(test);
      }

      // Standards Number of Stories
      if (numberOfStories) {
        bool test = newBuildingObject.setInt(8, *numberOfStories);
        OS_ASSERT(test);
      }

      /// Standards Number of Above Ground Stories
      if (numberOfAboveGroundStories) {
        bool test = newBuildingObject.setInt(9, *numberOfAboveGroundStories);
        OS_ASSERT(test);
      }

      // Standards Building Type
      if (buildingTypeValue) {
        bool test = newBuildingObject.setString(10, *buildingTypeValue);
        OS_ASSERT(test);
      }

      ss << newBuildingObject;
      m_refactored.emplace_back(std::move(*buildingObject), std::move(newBuildingObject));
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_3_4_to_1_3_5(const IdfFile& idf_1_3_4, const IddFileAndFactoryWrapper& idd_1_3_5) {
    std::stringstream ss;

    ss << idf_1_3_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_3_5.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_3_4.objects()) {

      if (object.iddObject().name() == "OS:Refrigeration:WalkIn") {

        IdfObject newWalkin = object.clone(true);

        boost::optional<std::string> s = object.getString(22);
        if (s) {
          bool test = newWalkin.setString(22, "");
          OS_ASSERT(test);
          test = newWalkin.setString(23, *s);
          OS_ASSERT(test);
        }

        ss << newWalkin;
        m_refactored.emplace_back(std::move(object), std::move(newWalkin));

      } else {

        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_5_3_to_1_5_4(const IdfFile& idf_1_5_3, const IddFileAndFactoryWrapper& idd_1_5_4) {
    std::stringstream ss;

    ss << idf_1_5_3.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_5_4.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_5_3.objects()) {
      if (object.iddObject().name() == "OS:TimeDependentValuation") {
        // put the object in the untranslated list
        m_untranslated.emplace_back(std::move(object));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_7_1_to_1_7_2(const IdfFile& idf_1_7_1, const IddFileAndFactoryWrapper& idd_1_7_2) {
    std::stringstream ss;

    ss << idf_1_7_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_7_2.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_7_1.objects()) {
      if (object.iddObject().name() == "OS:EvaporativeCooler:Direct:ResearchSpecial") {
        auto iddObject = idd_1_7_2.getObject("OS:EvaporativeCooler:Direct:ResearchSpecial");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 10; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        auto d = object.getDouble(4);
        if (!d) {
          newObject.setString(4, "Autosize");
        }
        newObject.setDouble(11, 0.1);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (object.iddObject().name() == "OS:EvaporativeCooler:Indirect:ResearchSpecial") {
        auto iddObject = idd_1_7_2.getObject("OS:EvaporativeCooler:Indirect:ResearchSpecial");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 19; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        auto d = object.getDouble(5);
        if (!d) {
          newObject.setString(5, "Autosize");
        }
        newObject.setDouble(22, 0.1);
        newObject.setDouble(24, 1.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_7_4_to_1_7_5(const IdfFile& idf_1_7_4, const IddFileAndFactoryWrapper& idd_1_7_5) {
    std::stringstream ss;

    ss << idf_1_7_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_7_5.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_7_4.objects()) {
      if (object.iddObject().name() == "OS:Sizing:System") {
        auto iddObject = idd_1_7_5.getObject("OS:Sizing:System");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 21; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        newObject.setDouble(21, 1.0);
        newObject.setDouble(22, 9.9676501E-3);
        newObject.setDouble(23, 1.0);
        newObject.setDouble(24, 3.9475456E-5);
        newObject.setDouble(25, 9.9676501E-3);
        newObject.setDouble(26, 1.0);
        newObject.setDouble(27, 1.0);
        newObject.setDouble(28, 3.1588213E-5);
        newObject.setString(29, "CoolingDesignCapacity");
        newObject.setString(30, "Autosize");
        newObject.setDouble(31, 234.7);
        newObject.setDouble(32, 1.0);
        newObject.setString(33, "HeatingDesignCapacity");
        newObject.setString(34, "Autosize");
        newObject.setDouble(35, 157.0);
        newObject.setDouble(36, 1.0);
        newObject.setString(37, "OnOff");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (object.iddObject().name() == "OS:Sizing:Plant") {
        auto iddObject = idd_1_7_5.getObject("OS:Sizing:Plant");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 5; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        newObject.setString(5, "NonCoincident");
        newObject.setInt(6, 1);
        newObject.setString(7, "None");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (object.iddObject().name() == "OS:DistrictCooling") {
        IdfObject newObject = object.clone(true);

        if (!object.getDouble(4)) {
          newObject.setString(4, "Autosize");
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (object.iddObject().name() == "OS:DistrictHeating") {
        IdfObject newObject = object.clone(true);

        if (!object.getDouble(4)) {
          newObject.setString(4, "Autosize");
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (object.iddObject().name() == "OS:Humidifier:Steam:Electric") {
        IdfObject newObject = object.clone(true);

        if (!object.getDouble(3)) {
          newObject.setString(4, "Autosize");
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_8_3_to_1_8_4(const IdfFile& idf_1_8_3, const IddFileAndFactoryWrapper& idd_1_8_4) {
    std::stringstream ss;

    ss << idf_1_8_3.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_8_4.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_8_3.objects()) {
      auto iddname = object.iddObject().name();
      if (iddname == "OS:PlantLoop") {
        auto iddObject = idd_1_8_4.getObject("OS:PlantLoop");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 4; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        for (size_t i = 5; i < 23; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i + 2, s.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AirLoopHVAC") {
        auto iddObject = idd_1_8_4.getObject("OS:AirLoopHVAC");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        // Handle
        if (auto value = object.getString(0)) {
          newObject.setString(0, value.get());
        }

        // Name
        if (auto value = object.getString(1)) {
          newObject.setString(1, value.get());
        }

        // Controller List Name
        // Not used

        // AvailabilityManagerAssignmentList
        auto availabilityManagerListHandle = object.getString(3);
        OS_ASSERT(availabilityManagerListHandle);
        auto availabilityManagerList = idf_1_8_3.getObject(toUUID(availabilityManagerListHandle.get()));
        OS_ASSERT(availabilityManagerList);

        auto availabilityManagerScheduledHandle = availabilityManagerList->getString(2);
        OS_ASSERT(availabilityManagerScheduledHandle);
        auto availabilityManagerScheduled = idf_1_8_3.getObject(toUUID(availabilityManagerScheduledHandle.get()));
        OS_ASSERT(availabilityManagerScheduled);

        auto availabilityScheduleHandle = availabilityManagerScheduled->getString(2);
        OS_ASSERT(availabilityScheduleHandle);
        auto availabilitySchedule = idf_1_8_3.getObject(toUUID(availabilityScheduleHandle.get()));
        OS_ASSERT(availabilitySchedule);

        auto availabilityManagerNightCycleHandle = availabilityManagerList->getString(3);
        OS_ASSERT(availabilityManagerNightCycleHandle);
        auto availabilityManagerNightCycle = idf_1_8_3.getObject(toUUID(availabilityManagerNightCycleHandle.get()));
        OS_ASSERT(availabilityManagerNightCycle);

        auto controlType = availabilityManagerNightCycle->getString(4);

        // Availability Schedule
        newObject.setString(3, toString(availabilitySchedule->handle()));

        // Availability Manager
        if (controlType
            && (istringEqual("CycleOnAny", controlType.get()) || istringEqual("CycleOnControlZone", controlType.get())
                || istringEqual("CycleOnAnyZoneFansOnly", controlType.get()))) {
          newObject.setString(4, toString(availabilityManagerNightCycle->handle()));
        }

        // All the remaining fields are unchanged but shifted down one to account for new field
        for (size_t i = 4; i < 15; ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(i + 1, value.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AvailabilityManager:Scheduled") {
        m_deprecated.push_back(object);
      } else if (iddname == "OS:AvailabilityManagerAssignmentList") {
        m_deprecated.push_back(object);
      } else if (iddname == "OS:AvailabilityManager:NightCycle") {
        auto controlType = object.getString(4);
        if (controlType
            && (istringEqual("CycleOnAny", controlType.get()) || istringEqual("CycleOnControlZone", controlType.get())
                || istringEqual("CycleOnAnyZoneFansOnly", controlType.get()))) {
          ss << object;
        } else {
          m_deprecated.push_back(object);
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_8_4_to_1_8_5(const IdfFile& idf_1_8_4, const IddFileAndFactoryWrapper& idd_1_8_5) {
    std::stringstream ss;

    ss << idf_1_8_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_8_5.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_1_8_4.objects()) {
      auto iddname = object.iddObject().name();
      if (iddname == "OS:SetpointManager:Scheduled") {
        if ((!object.getString(2)) || object.getString(2).get().empty()) {
          auto iddObject = idd_1_8_5.getObject("OS:SetpointManager::Scheduled");
          OS_ASSERT(iddObject);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < 5; ++i) {
            if (i == 2) {
              newObject.setString(i, "Temperature");
            } else if (auto s = object.getString(i)) {
              newObject.setString(i, s.get());
            }
          }
          ss << newObject;
        } else {
          ss << object;
        }
      } else if (iddname == "OS:PlantLoop") {
        if ((!object.getString(20)) || object.getString(20).get().empty()) {
          auto iddObject = idd_1_8_5.getObject("OS:PlantLoop");
          OS_ASSERT(iddObject);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < 25; ++i) {
            if (i == 20) {
              newObject.setString(i, "Optimal");
            } else if (auto s = object.getString(i)) {
              newObject.setString(i, s.get());
            }
          }
          ss << newObject;
        } else {
          ss << object;
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_8_5_to_1_9_0(const IdfFile& idf_1_8_5, const IddFileAndFactoryWrapper& idd_1_9_0) {
    std::stringstream ss;

    ss << idf_1_8_5.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_9_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_8_5.objects()) {
      auto iddname = object.iddObject().name();
      if (iddname == "OS:SpaceInfiltration:EffectiveLeakageArea") {
        auto iddObject = idd_1_9_0.getObject("OS:SpaceInfiltration:EffectiveLeakageArea");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (i == 4) {
            if (auto value = object.getDouble(i)) {
              newObject.setDouble(i, convert(value.get(), "m^2", "cm^2").get());
            }
          } else if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_9_2_to_1_9_3(const IdfFile& idf_1_9_2, const IddFileAndFactoryWrapper& idd_1_9_3) {
    std::stringstream ss;

    ss << idf_1_9_2.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_9_3.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_9_2.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:EvaporativeCooler:Direct:ResearchSpecial") {
        auto iddObject = idd_1_9_3.getObject("OS:EvaporativeCooler:Direct:ResearchSpecial");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (i < 5) {
            if (auto value = object.getString(i)) {
              newObject.setString(i, value.get());
            }
          } else if (i == 5) {
            if (auto value = object.getString(i)) {
              newObject.setString(i + 1, value.get());
            }
            newObject.setString(i, "Autosize");
          } else {
            if (auto value = object.getString(i)) {
              newObject.setString(i + 1, value.get());
            }
          }
        }
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneAirMassFlowConservation") {
        auto iddObject = idd_1_9_3.getObject("OS:ZoneAirMassFlowConservation");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        if (auto value = object.getString(0)) {  // Handle -> Handle
          newObject.setString(0, value.get());
        }
        if (auto value =
              object.getString(1)) {  // Adjust Zone Mixing For Zone Air Mass Flow Balance -> Adjust Zone Mixing For Zone Air Mass Flow Balance
          newObject.setString(1, value.get());
        } else {
          // old default was Yes, new default is No
        }
        if (auto value = object.getString(2)) {  // Source Zone Infiltration Treatment -> Infiltration Balancing Method
          // old default was AddInfiltrationFlow, new default is AddInfiltrationFlow
          // old keys AddInfiltrationFlow, AdjustInfiltrationFlow
          // new keys AddInfiltrationFlow, AdjustInfiltrationFlow, None
          newObject.setString(2, value.get());
        }
        // new field Infiltration Balancing Zones is defaulted to MixingSourceZonesOnly
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
        auto iddObject = idd_1_9_3.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(18, "No");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AirTerminal:SingleDuct:VAV:NoReheat") {
        auto iddObject = idd_1_9_3.getObject("OS:AirTerminal:SingleDuct:VAV:NoReheat");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(10, "No");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_9_4_to_1_9_5(const IdfFile& idf_1_9_4, const IddFileAndFactoryWrapper& idd_1_9_5) {
    std::stringstream ss;

    ss << idf_1_9_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_9_5.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_9_4.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Controller:WaterCoil") {
        auto iddObject = idd_1_9_5.getObject("OS:Controller:WaterCoil");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0, j = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(j, value.get());
          }
          if (i == 1) {
            // new field after i = 1
            ++j;
          }
          ++j;
        }

        // Figure out value of new field j = 2
        // This is the handle of the associated water coil
        // Past versions made this connection by matching the controller actuator node
        // to the coil water inlet node.
        // To do this version translation we have to track down that node
        if (auto actuatorNodeHandle = object.getString(6)) {
          auto coilHandle = [&](const IddObject& type, unsigned waterInletIndex) {
            boost::optional<std::string> result;
            auto coils = idf_1_9_4.getObjectsByType(type);
            for (const auto& coil : coils) {
              // waterInletConnection will be a handle to a connection object
              if (auto waterInletConnectionHandle = coil.getString(waterInletIndex)) {
                if (auto waterInletConnection = idf_1_9_4.getObject(toUUID(waterInletConnectionHandle.get()))) {
                  if (auto sourceHandle = waterInletConnection->getString(2)) {
                    if (sourceHandle.get() == actuatorNodeHandle.get()) {
                      result = toString(coil.handle());
                      break;
                    }
                  }
                }
              }
            }
            return result;
          };
          if (auto handle = coilHandle(idf_1_9_4.iddFile().getObject("OS:Coil:Cooling:Water").get(), 10)) {
            newObject.setString(2, handle.get());
          } else if (auto handle = coilHandle(idf_1_9_4.iddFile().getObject("OS:Coil:Heating:Water").get(), 5)) {
            newObject.setString(2, handle.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_9_5_to_1_10_0(const IdfFile& idf_1_9_5, const IddFileAndFactoryWrapper& idd_1_10_0) {
    std::stringstream ss;

    ss << idf_1_9_5.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_10_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_9_5.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
        auto iddObject = idd_1_10_0.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(i, value.get());
          }
        }

        // This is a redo because some models didn't get updated by the logic in
        // 1.9.2 -> 1.9.3
        {
          auto controlForOA = object.getString(18);
          if ((!controlForOA) || controlForOA->empty()) {
            newObject.setString(18, "No");
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AirTerminal:SingleDuct:VAV:NoReheat") {
        auto iddObject = idd_1_10_0.getObject("OS:AirTerminal:SingleDuct:VAV:NoReheat");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto value = object.getString(i)) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(10, "No");
        // This is a redo because some models didn't get updated by the logic in
        // 1.9.2 -> 1.9.3
        {
          auto controlForOA = object.getString(10);
          if ((!controlForOA) || controlForOA->empty()) {
            newObject.setString(10, "No");
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_10_1_to_1_10_2(const IdfFile& idf_1_10_1, const IddFileAndFactoryWrapper& idd_1_10_2) {

    std::stringstream ss;

    ss << idf_1_10_1.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_10_2.iddFile());
    ss << targetIdf.versionObject().get();

    auto zones = idf_1_10_1.getObjectsByType(idf_1_10_1.iddFile().getObject("OS:ThermalZone").get());

    for (IdfObject& object : idf_1_10_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:ThermostatSetpoint:DualSetpoint") {
        // Get all of the zones that point to this thermostat
        std::vector<IdfObject> referencingZones;
        for (const auto& zone : zones) {
          if (auto thermostatHandle = zone.getString(19)) {
            if (toUUID(thermostatHandle.get()) == object.handle()) {
              referencingZones.push_back(zone);
            }
          }
        }

        // Clone the thermostat for every zone after the first
        if (referencingZones.size() > 1u) {
          for (auto& referencingZone : referencingZones) {
            // This will leave the original thermostate hanging out alone in most circumstances
            // but since we are messing with the name it is probably best
            auto newThermostat = object.clone();
            newThermostat.setName(referencingZone.nameString() + " Thermostat");
            ss << newThermostat;
            m_new.push_back(newThermostat);
            auto newHandle = newThermostat.getString(0).get();
            referencingZone.setString(19, newHandle);
          }
        }
        ss << object;
      } else if (iddname == "OS:Sizing:Zone") {
        auto iddObject = idd_1_10_2.getObject("OS:Sizing:Zone");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (i == 2) {
            newObject.setString(newi, "SupplyAirTemperature");
            ++newi;
            if (auto value = object.getString(i)) {
              newObject.setString(newi, value.get());
            }
            ++newi;
            newObject.setDouble(newi, 11.11);
          } else if (i == 3) {
            newObject.setString(newi, "SupplyAirTemperature");
            ++newi;
            if (auto value = object.getString(i)) {
              newObject.setString(newi, value.get());
            }
            ++newi;
            newObject.setDouble(newi, 11.11);
          } else if (auto value = object.getString(i)) {
            newObject.setString(newi, value.get());
          }
          ++newi;
        }

        newObject.setString(24, "No");
        newObject.setString(25, "NeutralSupplyAir");
        newObject.setString(26, "Autosize");
        newObject.setString(27, "Autosize");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    // Reintroduce m_cbeccSizingObjects
    for (auto const& sizingObject : m_cbeccSizingObjects) {
      auto iddObject = idd_1_10_2.getObject("OS:Sizing:Zone");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for (size_t i = 0; i < sizingObject.numNonextensibleFields(); ++i) {
        if (auto value = sizingObject.getString(i)) {
          newObject.setString(i, value.get());
        }
      }
      newObject.setString(24, "No");
      newObject.setString(25, "NeutralSupplyAir");
      newObject.setString(26, "Autosize");
      newObject.setString(27, "Autosize");

      m_new.push_back(newObject);
      ss << newObject;
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_10_5_to_1_10_6(const IdfFile& idf_1_10_5, const IddFileAndFactoryWrapper& idd_1_10_6) {
    std::stringstream ss;

    ss << idf_1_10_5.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_10_6.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_10_5.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:PlantLoop") {
        auto iddObject = idd_1_10_6.getObject("OS:PlantLoop");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            if (i == 20) {
              if (istringEqual("Sequential", s.get())) {
                newObject.setString(i, "SequentialLoad");
              } else if (istringEqual("Uniform", s.get())) {
                newObject.setString(i, "UniformLoad");
              } else {
                newObject.setString(i, s.get());
              }
            } else {
              newObject.setString(i, s.get());
            }
          } else {
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_11_3_to_1_11_4(const IdfFile& idf_1_11_3, const IddFileAndFactoryWrapper& idd_1_11_4) {
    std::stringstream ss;

    ss << idf_1_11_3.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_11_4.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_11_3.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Coil:Heating:Water:Baseboard") {
        auto iddObject = idd_1_11_4.getObject("OS:Coil:Heating:Water:Baseboard");
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(newi, s.get());
            if (i == 1) {
              newi = newi + 4;
            }
          }
          ++newi;
        }

        newObject.setString(2, "HeatingDesignCapacity");
        newObject.setString(3, "Autosize");
        newObject.setDouble(4, 0.0);
        newObject.setDouble(5, 0.8);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_11_4_to_1_11_5(const IdfFile& idf_1_11_4, const IddFileAndFactoryWrapper& idd_1_11_5) {
    std::stringstream ss;

    ss << idf_1_11_4.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_11_5.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_11_4.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Coil:Heating:DX:SingleSpeed") {
        auto iddObject = idd_1_11_5.getObject("OS:Coil:Heating:DX:SingleSpeed");
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {

          if (i == 6) {
            newObject.setDouble(i, 773.3);
            newi = i + 1;
          } else if (i > 6) {
            newi = i + 1;
          } else {
            newi = i;
          }

          if (auto s = object.getString(i)) {
            newObject.setString(newi, s.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_12_0_to_1_12_1(const IdfFile& idf_1_12_0, const IddFileAndFactoryWrapper& idd_1_12_1) {
    std::stringstream ss;

    ss << idf_1_12_0.header() << '\n' << '\n';

    // new version object
    IdfFile targetIdf(idd_1_12_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_12_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Meter") {
        auto iddObject = idd_1_12_1.getObject("OS:Output:Meter");
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(newi, s.get());
          }
          ++newi;
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_1_12_3_to_1_12_4(const IdfFile& idf_1_12_3, const IddFileAndFactoryWrapper& idd_1_12_4) {
    std::stringstream ss;

    ss << idf_1_12_3.header() << '\n' << '\n';
    IdfFile targetIdf(idd_1_12_4.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_1_12_3.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
        auto iddObject = idd_1_12_4.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            if (i == 14u) {
              if (istringEqual("Reverse", s.get())) {
                newObject.setString(i, "ReverseWithLimits");
              } else {
                newObject.setString(i, s.get());
              }
            } else {
              newObject.setString(i, s.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_1_0_to_2_1_1(const IdfFile& idf_2_1_0, const IddFileAndFactoryWrapper& idd_2_1_1) {
    std::stringstream ss;

    ss << idf_2_1_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_1_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_1_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:WaterHeater:Stratified") {
        auto iddObject = idd_2_1_1.getObject("OS:WaterHeater:Stratified");
        IdfObject newObject(iddObject.get());

        size_t oldi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((i == 64u) || (i == 65u)) {
            newObject.setDouble(i, 0.0);
          } else {
            if (auto s = object.getString(oldi)) {
              newObject.setString(i, s.get());
            }
            oldi++;
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Heating") {
        auto iddObject = idd_2_1_1.getObject("OS:HeatPump:WaterToWater:EquationFit:Heating");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }
        newObject.setDouble(20, 7.5);
        newObject.setDouble(21, 1.0);
        newObject.setString(22, "");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Cooling") {
        auto iddObject = idd_2_1_1.getObject("OS:HeatPump:WaterToWater:EquationFit:Cooling");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }
        newObject.setDouble(20, 8.0);
        newObject.setDouble(21, 1.0);
        newObject.setString(22, "");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_1_1_to_2_1_2(const IdfFile& idf_2_1_1, const IddFileAndFactoryWrapper& idd_2_1_2) {
    std::stringstream ss;

    ss << idf_2_1_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_1_2.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_1_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:PlantLoop") {
        auto iddObject = idd_2_1_2.getObject("OS:PlantLoop");
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (i == 3u) {
            newObject.setInt(newi, 0);
            ++newi;
          } else {
            if (auto s = object.getString(i)) {
              newObject.setString(newi, s.get());
            }
          }
          ++newi;
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:ZoneHVAC:FourPipeFanCoil") {
        auto iddObject = idd_2_1_2.getObject("OS:ZoneHVAC:FourPipeFanCoil");
        IdfObject newObject(iddObject.get());

        size_t newi = 0;
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(newi, s.get());
          }
          ++newi;
        }
        newObject.setString(23, "Autosize");
        newObject.setString(24, "Autosize");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_3_0_to_2_3_1(const IdfFile& idf_2_3_0, const IddFileAndFactoryWrapper& idd_2_3_1) {
    std::stringstream ss;

    ss << idf_2_3_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_3_1.iddFile());
    ss << targetIdf.versionObject().get();

    boost::optional<std::string> value;

    for (IdfObject& object : idf_2_3_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Pump:ConstantSpeed") {
        auto iddObject = idd_2_3_1.getObject("OS:Pump:ConstantSpeed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(16, "PowerPerFlowPerPressure");
        newObject.setString(17, "348701.1");
        newObject.setString(18, "1.282051282");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:Pump:VariableSpeed") {
        auto iddObject = idd_2_3_1.getObject("OS:Pump:VariableSpeed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(25, "0.5");
        newObject.setString(26, "PowerPerFlowPerPressure");
        newObject.setString(27, "348701.1");
        newObject.setString(28, "1.282051282");
        newObject.setString(29, "0.0");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:CoolingTower:SingleSpeed") {
        auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:SingleSpeed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(29, "0.1");
        newObject.setString(30, "0.1");
        newObject.setString(31, "1.25");
        newObject.setString(32, "0.1");
        newObject.setString(33, "35.0");
        newObject.setString(34, "25.6");
        newObject.setString(35, "Autosize");
        newObject.setString(36, "Autosize");
        newObject.setString(37, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:CoolingTower:TwoSpeed") {
        auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:TwoSpeed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(41, "35.0");
        newObject.setString(42, "25.6");
        newObject.setString(43, "Autosize");
        newObject.setString(44, "Autosize");
        newObject.setString(45, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:CoolingTower:VariableSpeed") {
        auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:VariableSpeed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(31, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Chiller:Electric:EIR") {
        auto iddObject = idd_2_3_1.getObject("OS:Chiller:Electric:EIR");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        if (object.getString(17) && (!object.getString(17).get().empty())) {
          newObject.setString(19, "WaterCooled");
        } else {
          newObject.setString(19, "AirCooled");
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:AirLoopHVAC") {
        auto iddObject = idd_2_3_1.getObject("OS:AirLoopHVAC");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        /* We need handle the change from the old behavior (one AvailabilityManager:xxx) to the new AVMList
           * First, we must create an AVMList for each loop, then if an AVM was assigned, we put it on the AVM list
           */

        // Create an AVMList with a handle and a name
        std::string avmName;
        if (auto loopName = object.getString(1)) {
          avmName = loopName.get();
        }
        avmName += " AvailabilityManagerAssignmentList";

        IdfObject avmList(idd_2_3_1.getObject("OS:AvailabilityManagerAssignmentList").get());
        std::string avmHandle = toString(createUUID());
        avmList.setString(0, avmHandle);
        avmList.setString(1, avmName);

        // Check if there was an AVM assigned to the Loop, it so, put it on the newly created AVMList
        if (auto existingAVMUUid = object.getString(4)) {
          avmList.pushExtensibleGroup(StringVector(1u, existingAVMUUid.get()));
        }

        // Assign AVM list to loop
        newObject.setString(4, avmHandle);

        m_refactored.emplace_back(object, newObject);
        m_new.push_back(avmList);

        ss << newObject;
        ss << avmList;

      } else if (iddname == "OS:PlantLoop") {
        auto iddObject = idd_2_3_1.getObject("OS:PlantLoop");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Create an AVMList with a handle and a name
        std::string avmName;
        if (auto loopName = object.getString(1)) {
          avmName = loopName.get();
        }
        avmName += " AvailabilityManagerAssignmentList";

        IdfObject avmList(idd_2_3_1.getObject("OS:AvailabilityManagerAssignmentList").get());
        std::string avmHandle = toString(createUUID());
        avmList.setString(0, avmHandle);
        avmList.setString(1, avmName);

        // Check if there was an AVM assigned to the Loop, it so, put it on the newly created AVMList
        if (auto existingAVMUUid = object.getString(22)) {
          avmList.pushExtensibleGroup(StringVector(1u, existingAVMUUid.get()));
        }

        // Assign AVM list to loop
        newObject.setString(22, avmHandle);

        m_refactored.emplace_back(object, newObject);
        m_new.push_back(avmList);

        ss << newObject;
        ss << avmList;

      } else if (iddname == "OS:AvailabilityManager:NightCycle") {
        auto iddObject = idd_2_3_1.getObject("OS:AvailabilityManager:NightCycle");
        IdfObject newObject(iddObject.get());

        // Cycling Run Time Control Type was placed before Cycling Run Time
        for (size_t i = 0; i < object.numNonextensibleFields() - 1; ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // No need to set the default Cycling Run Time Control Type, it has a defaulted option
        // But we need to carry the cycling Run Time (seconds) in the right field
        if (auto cyclingRunTime = newObject.getDouble(6)) {
          newObject.setDouble(7, cyclingRunTime.get());
        }

        // We need to create the four ModelObjectLists for the control zones
        // 8 = Control Zone or Zone List Name
        std::string controlThermalZoneListName;
        if (auto avmName = object.getString(1)) {
          controlThermalZoneListName = avmName.get();
        }
        controlThermalZoneListName += " Control Zone List";

        IdfObject controlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
        std::string controlThermalZoneListHandle = toString(createUUID());
        controlThermalZoneList.setString(0, controlThermalZoneListHandle);
        controlThermalZoneList.setString(1, controlThermalZoneListName);
        newObject.setString(8, controlThermalZoneListHandle);

        // 9 = Cooling Control Zone or Zone List Name
        std::string coolingControlThermalZoneListName;
        if (auto avmName = object.getString(1)) {
          coolingControlThermalZoneListName = avmName.get();
        }
        coolingControlThermalZoneListName += " Cooling Control Zone List";

        IdfObject coolingControlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
        std::string coolingControlThermalZoneListHandle = toString(createUUID());
        coolingControlThermalZoneList.setString(0, coolingControlThermalZoneListHandle);
        coolingControlThermalZoneList.setString(1, coolingControlThermalZoneListName);
        newObject.setString(9, coolingControlThermalZoneListHandle);

        // 10 = Heating Control Zone or Zone List Name
        std::string heatingControlThermalZoneListName;
        if (auto avmName = object.getString(1)) {
          heatingControlThermalZoneListName = avmName.get();
        }
        heatingControlThermalZoneListName += " Heating Control Zone List";

        IdfObject heatingControlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
        std::string heatingControlThermalZoneListHandle = toString(createUUID());
        heatingControlThermalZoneList.setString(0, heatingControlThermalZoneListHandle);
        heatingControlThermalZoneList.setString(1, heatingControlThermalZoneListName);
        newObject.setString(10, heatingControlThermalZoneListHandle);

        // 11 = Heating Control Zone or Zone List Name
        std::string heatingZoneFansOnlyThermalZoneListName;
        if (auto avmName = object.getString(1)) {
          heatingZoneFansOnlyThermalZoneListName = avmName.get();
        }
        heatingZoneFansOnlyThermalZoneListName += " Heating Zone Fans Only Zone List";

        IdfObject heatingZoneFansOnlyThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
        std::string heatingZoneFansOnlyThermalZoneListHandle = toString(createUUID());
        heatingZoneFansOnlyThermalZoneList.setString(0, heatingZoneFansOnlyThermalZoneListHandle);
        heatingZoneFansOnlyThermalZoneList.setString(1, heatingZoneFansOnlyThermalZoneListName);
        newObject.setString(11, heatingZoneFansOnlyThermalZoneListHandle);

        m_refactored.emplace_back(object, newObject);
        m_new.push_back(controlThermalZoneList);
        m_new.push_back(coolingControlThermalZoneList);
        m_new.push_back(heatingControlThermalZoneList);
        m_new.push_back(heatingZoneFansOnlyThermalZoneList);

        ss << newObject;
        ss << controlThermalZoneList;
        ss << coolingControlThermalZoneList;
        ss << heatingControlThermalZoneList;
        ss << heatingZoneFansOnlyThermalZoneList;

      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_4_1_to_2_4_2(const IdfFile& idf_2_4_1, const IddFileAndFactoryWrapper& idd_2_4_2) {
    std::stringstream ss;

    ss << idf_2_4_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_4_2.iddFile());
    ss << targetIdf.versionObject().get();

    boost::optional<std::string> value;

    for (IdfObject& object : idf_2_4_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:BuildingUnit") {
        auto iddObject = idd_2_4_2.getObject("OS:BuildingUnit");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        iddObject = idd_2_4_2.getObject("OS:AdditionalProperties");
        IdfObject additionalProperties(iddObject.get());
        additionalProperties.setString(0, toString(createUUID()));
        additionalProperties.setString(1, newObject.getString(0).get());  // point additional properties to new object

        size_t newIdx = 2;
        for (size_t oldIdx = object.numNonextensibleFields(); oldIdx < object.numFields(); ++oldIdx, ++newIdx) {
          if ((value = object.getString(oldIdx))) {
            additionalProperties.setString(newIdx, value.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
        m_new.push_back(additionalProperties);
        ss << additionalProperties;

      } else if (iddname == "OS:Boiler:HotWater") {
        auto iddObject = idd_2_4_2.getObject("OS:Boiler:HotWater");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }
        newObject.setString(18, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Boiler:Steam") {
        auto iddObject = idd_2_4_2.getObject("OS:Boiler:Steam");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }
        newObject.setString(16, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:WaterHeater:Mixed") {
        auto iddObject = idd_2_4_2.getObject("OS:WaterHeater:Mixed");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Source Side Flow Control Mode
        newObject.setString(40, "IndirectHeatPrimarySetpoint");

        // Indirect Alternate Setpoint Temperature Schedule Name: nothing to do, leave empty

        // End Use Subcategory
        newObject.setString(42, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Chiller:Electric:EIR") {
        auto iddObject = idd_2_4_2.getObject("OS:Chiller:Electric:EIR");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Condenser Heat Recovery Relative Capacity Fraction: this is an optional
        // but in E+ code (ChillerElectricEIR.cc > GetElectricEIRChillerInput() around line 655)
        // if omitted (and heat recovery is used), it defaults to 1.0
        // newObject.setDouble(32, 1.0);

        // Heat Recovery Inlet High Temperature Limit Schedule Name: leave empty

        // Heat Recovery Leaving Temperature Setpoint Node Name: leave empty

        // endUseSubcategory
        newObject.setString(34, "General");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Default case
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_4_3_to_2_5_0(const IdfFile& idf_2_4_3, const IddFileAndFactoryWrapper& idd_2_5_0) {
    std::stringstream ss;

    ss << idf_2_4_3.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_5_0.iddFile());
    ss << targetIdf.versionObject().get();

    boost::optional<std::string> value;

    for (IdfObject& object : idf_2_4_3.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AirflowNetworkZone") {
        // In 2.5.0, a field "Name" was inserted right after the handle
        auto iddObject = idd_2_5_0.getObject("OS:AirflowNetworkZone");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i == 0) {
              // Handle
              newObject.setString(i, value.get());
            } else if (i == 1) {
              // Thermal Zone Name field

              // We place the Thermal Zone's handle into the right field
              newObject.setString(i + 1, value.get());

              // We want to rename the AFN Zone object with its thermal Zone name
              // So we need to locate the right thermal zone from its handle so we can get the name
              boost::optional<IdfObject> _zone = idf_2_4_3.getObject(toUUID(value.get()));
              if (_zone) {
                newObject.setString(i, "Airflow Network Zone " + _zone->nameString());
              } else {
                // Safety fall back, name it with the thermal zone handle instead...
                newObject.setString(i, "Airflow Network Zone " + value.get());
              }

            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Default case
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_6_0_to_2_6_1(const IdfFile& idf_2_6_0, const IddFileAndFactoryWrapper& idd_2_6_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_6_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_6_1.iddFile());
    ss << targetIdf.versionObject().get();

    struct ConnectionInfo
    {
      std::string zoneHandle;
      std::string connectionHandle;
      std::string newPortListHandle;
    };
    // map of a connection object handle to a ConnectionInfo instance
    std::map<std::string, ConnectionInfo> connectionsToFix;

    // Find the connection object associated with the return air port
    auto zones = idf_2_6_0.getObjectsByType(idf_2_6_0.iddFile().getObject("OS:ThermalZone").get());
    for (auto& zone : zones) {
      // index 12 is the handle of a connection that will need fixing
      value = zone.getString(12);
      if (value) {
        ConnectionInfo info;
        info.zoneHandle = zone.getString(0).get();
        info.connectionHandle = value.get();
        connectionsToFix[value.get()] = info;
      }
    }

    for (IdfObject& object : idf_2_6_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:ThermalZone") {
        auto iddObject = idd_2_6_1.getObject("OS:ThermalZone");
        IdfObject newObject(iddObject.get());

        IdfObject newReturnPortList(idd_2_6_1.getObject("OS:PortList").get());

        auto h = toString(createUUID());
        newReturnPortList.setString(0, h);
        newReturnPortList.setString(2, object.getString(0).get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          value = object.getString(i);
          if (value) {
            if (i == 12) {
              auto eg = newReturnPortList.pushExtensibleGroup();
              eg.setString(0, value.get());
              connectionsToFix[value.get()].newPortListHandle = h;
              newObject.setString(i, h);
            } else {
              newObject.setString(i, value.get());
            }
          }
        }

        m_refactored.emplace_back(object, newObject);
        m_new.push_back(newReturnPortList);
        ss << newObject;
        ss << newReturnPortList;
      } else if (iddname == "OS:Connection") {
        value = object.getString(0);
        OS_ASSERT(value);
        auto c = connectionsToFix.find(value.get());
        if (c != connectionsToFix.end()) {
          IdfObject newConnection(idd_2_6_1.getObject("OS:Connection").get());
          for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
            value = object.getString(i);
            if (value) {
              newConnection.setString(i, value.get());
            }
          }
          // index 3 is the source object port,
          // it needs to specify a port on the PortList instead of the ThermalZone now
          newConnection.setString(2, c->second.newPortListHandle);
          newConnection.setUnsigned(3, 3);
          ss << newConnection;
          m_refactored.emplace_back(std::move(object), std::move(newConnection));
        } else {
          ss << object;
        }
        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_6_1_to_2_6_2(const IdfFile& idf_2_6_1, const IddFileAndFactoryWrapper& idd_2_6_2) {
    std::stringstream ss;

    ss << idf_2_6_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_6_2.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_6_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:EvaporativeCooler:Direct:ResearchSpecial") {

        auto iddObject = idd_2_6_2.getObject("OS:EvaporativeCooler:Direct:ResearchSpecial");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          auto value = object.getString(i);
          if (value) {
            newObject.setString(i, value.get());
          }
        }
        // The last three fields were added in #3118 as NON optional doubles, so default to extreme values
        // to make it behave like when blank = no control

        // Evaporative Operation Minimum Drybulb Temperature
        if (!newObject.getDouble(14)) {
          newObject.setDouble(14, -99);
        }
        // Evaporative Operation Maximum Limit Wetbulb Temperature
        if (!newObject.getDouble(15)) {
          newObject.setDouble(15, 99);
        }
        // Evaporative Operation Maximum Limit Drybulb Temperature
        if (!newObject.getDouble(16)) {
          newObject.setDouble(16, 99);
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:EquipmentList") {
        // In 2.6.2, a field "Load Distribution Scheme" was inserted right after the thermal zone
        auto iddObject = idd_2_6_2.getObject("OS:ZoneHVAC:EquipmentList");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          auto value = object.getString(i);
          if (value) {
            if (i < 3) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_6_2_to_2_7_0(const IdfFile& idf_2_6_2, const IddFileAndFactoryWrapper& idd_2_7_0) {
    std::stringstream ss;

    ss << idf_2_6_2.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_7_0.iddFile());
    ss << targetIdf.versionObject().get();

    struct ConnectionInfo
    {
      std::string atuHandle;
      std::string connectionHandle;
      std::string newNodeHandle;
    };
    // map of a connection object handle to a ConnectionInfo instance
    std::map<std::string, ConnectionInfo> connectionsToFix;

    // Find the connections object associated with the ATU Uncontrolled
    auto atus = idf_2_6_2.getObjectsByType(idf_2_6_2.iddFile().getObject("OS:AirTerminal:SingleDuct:Uncontrolled").get());
    for (auto& atu : atus) {
      // index 3 = Air Inlet Node Name
      // It's the handle of a connection that will need fixing
      // Because ATU Uncontrolled was directly connected to ZoneSplitter; now we want a node in between
      auto value = atu.getString(3);
      if (value) {
        ConnectionInfo info;
        info.atuHandle = atu.getString(0).get();
        info.connectionHandle = value.get();
        connectionsToFix[value.get()] = info;
      }
    }

    for (IdfObject& object : idf_2_6_2.objects()) {
      auto iddname = object.iddObject().name();

      // ATU:SingleDuct:Uncontrolled got made obsolete by ATU:SingleDuct:ConstantVolume:NoReheat in E+ 9.0.0
      // in order to be more consistent with the naming of other ATUs, but it also isn't exactly laid out the same
      if (iddname == "OS:AirTerminal:SingleDuct:Uncontrolled") {
        // We just create a new object, and copy every field but one.
        auto iddObject = idd_2_7_0.getObject("OS:AirTerminal:SingleDuct:ConstantVolume:NoReheat");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if (auto s = object.getString(i)) {
            if (i == 3) {
              // ATU Uncontrolled references the AirLoopHVAC:ZoneSplitter directly, here we want a node in between, so we need a new node and a new
              // connection
              // Before: ZoneSplitter ----connection1---->ATU Uncontrolled
              // New:    ZoneSplitter-----connection1---->newNode----------newConnection----ATU Single Duct CV No Reheat
              auto nodeIdd = idd_2_7_0.getObject("OS:Node");
              auto nodeHandle = toString(createUUID());
              IdfObject newNode(nodeIdd.get());
              newNode.setString(0, nodeHandle);

              auto connectionIdd = idd_2_7_0.getObject("OS:Connection");
              // We pass fastname = true, to mimic the normal behavior (a connection gets a handle for name)
              IdfObject newConnection(connectionIdd.get(), true);
              auto newConnectionHandle = toString(createUUID());
              newConnection.setString(0, newConnectionHandle);
              // Name
              // Source Object: Node
              newConnection.setString(2, nodeHandle);
              // Outlet Port: 3
              newConnection.setInt(3, 3);

              // Target Object: ATU
              newConnection.setString(4, newObject.getString(0).get());
              // Inlet Port: i=3
              newConnection.setInt(5, 3);

              // ATU now refers to the newConnection
              newObject.setString(i, newConnectionHandle);

              // The existing connection is going to have to point to the newNode
              connectionsToFix[s.get()].newNodeHandle = nodeHandle;

              newNode.setName(object.nameString() + " Inlet Node");
              // Node Inlet Port = old connection
              newNode.setString(2, s.get());
              // Outlet Port = New connection
              newNode.setString(3, newConnectionHandle);

              // Register new objects
              m_new.push_back(newNode);
              m_new.push_back(newConnection);
              ss << newNode;
              ss << newConnection;

            } else {
              // Otherwise, keep the same
              newObject.setString(i, s.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Connection") {
        // No-Op for now
        auto value = object.getString(0);
        OS_ASSERT(value);
        if (connectionsToFix.find(value.get()) == connectionsToFix.end()) {
          // No need to fix it, we just push it
          ss << object;
        }

      } else if (iddname == "OS:Building") {
        // Inserted a field "Standards Template" at position 10
        auto iddObject = idd_2_7_0.getObject("OS:Building");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 10; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        for (size_t i = 10; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i + 1, s.get());
          }
        }

        // Field is optional string, so leave it empty

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:SpaceType") {
        // Added a field "Standards Template" at position 6
        auto iddObject = idd_2_7_0.getObject("OS:SpaceType");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 6; ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i, s.get());
          }
        }

        for (size_t i = 6; i < object.numNonextensibleFields(); ++i) {
          if (auto s = object.getString(i)) {
            newObject.setString(i + 1, s.get());
          }
        }

        // Field is optional string, so leave it empty

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else {
        ss << object;
      }
    }

    // Do a second pass, though you'd expect connections to be after the objects it connects, technically it's possible that it wouldn't
    // if the user moved objects manually inside the file
    // So I do want connectionInfos to be properly populated with newNodeHandle
    for (IdfObject& object : idf_2_6_2.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Connection") {
        auto value = object.getString(0);
        OS_ASSERT(value);
        auto c = connectionsToFix.find(value.get());
        if (c != connectionsToFix.end()) {
          IdfObject newConnection(idd_2_7_0.getObject("OS:Connection").get());
          for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
            auto value = object.getString(i);
            if (value) {
              newConnection.setString(i, value.get());
            }
          }

          // The target object (field 4) becomes the Node handle
          // And it connects to the "Inlet Port" of the name (field 2 of the node)
          newConnection.setString(4, c->second.newNodeHandle);
          newConnection.setUnsigned(5, 2);
          ss << newConnection;
          m_refactored.emplace_back(std::move(object), std::move(newConnection));
        }
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_7_0_to_2_7_1(const IdfFile& idf_2_7_0, const IddFileAndFactoryWrapper& idd_2_7_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_7_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_7_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_7_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Sizing:System") {
        // Note JM 2018-11-05:
        // We deprecated the 'Latent' option which is no longer used by E+ (and hasn't been for a long time)
        // It should have produced a crash anyways before, but if we find 'Latent', we replace by 'Total'
        // Why did I choose 'Total' and not 'Sensible' (or VentilationRequirement)?
        // 'Sensible' is the default choice, but if the user had picked 'Latent', they probably didn't want 'Sensible', so I went with 'Total' instead.
        value = object.getString(2);
        if (value && istringEqual(value.get(), "Latent")) {
          IdfObject newObject = object.clone(true);
          newObject.setString(2, "Total");
          LOG(Warn, "OS:Sizing:System does not support 'Latent' as 'Type of Load To Size On'. "
                      << "It was replaced by 'Total' instead for object with handle '" << newObject.getString(0).get()
                      << "'. Please review carefully.");

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          // Nothing to do here
          ss << object;
        }
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_7_1_to_2_7_2(const IdfFile& idf_2_7_1, const IddFileAndFactoryWrapper& idd_2_7_2) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_7_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_7_2.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_7_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:WeatherFile") {
        // Note JM 2019-01-08: We remove the file:// prefix from QUrl
        value = object.getString(10);
        // If there's a filepath, and it starts with file://
        if (value && (value.get().rfind("file://", 0) == 0)) {
          IdfObject newObject = object.clone(true);
          newObject.setString(10, value.get().substr(7));
          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          // Nothing to do here
          ss << object;
        }

        // Both of these happen to have the url field at pos 2 (note: neither of these are actually implemented in the SDK, but let's be safe)
      } else if ((iddname == "OS:Construction:WindowDataFile") || (iddname == "OS:Luminaire:Definition")) {
        // Note JM 2019-01-08: We remove the file:// prefix from QUrl
        value = object.getString(2);
        // If there's a filepath, and it starts with file://
        if (value && (value.get().rfind("file://", 0) == 0)) {
          IdfObject newObject = object.clone(true);
          newObject.setString(2, value.get().substr(7));
          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          // Nothing to do here
          ss << object;
        }

      } else if (iddname == "OS:ZoneHVAC:EquipmentList") {

        auto iddObject = idd_2_7_2.getObject("OS:ZoneHVAC:EquipmentList");
        IdfObject newObject(iddObject.get());

        // Copy non extensible fields in place
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Copy the existing eg values (the new fields were added at the end of the extensible groups, and have defaults)
        for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
          IdfExtensibleGroup new_eg = newObject.pushExtensibleGroup();
          for (size_t i = 0; i < 3; ++i) {
            new_eg.setString(i, eg.getString(i).get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_8_1_to_2_9_0(const IdfFile& idf_2_8_1, const IddFileAndFactoryWrapper& idd_2_9_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_8_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_9_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_2_8_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Foundation:Kiva") {
        auto iddObject = idd_2_9_0.getObject("OS:Foundation:Kiva");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 2) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Schedule:FixedInterval") {
        auto iddObject = idd_2_9_0.getObject("OS:Schedule:FixedInterval");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 3) {
              // Schedule Type Limits Name
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:EquipmentList") {
        auto iddObject = idd_2_9_0.getObject("OS:ZoneHVAC:EquipmentList");
        IdfObject newObject(iddObject.get());

        // Copy non extensible fields in place
        for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Copy the eg values 0 through 2 and create ScheduleConstant out of 3 and 4
        for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
          IdfExtensibleGroup new_eg = newObject.pushExtensibleGroup();
          for (size_t i = 0; i < 5; ++i) {
            if (i > 2) {
              boost::optional<double> fraction;
              if ((fraction = eg.getDouble(i))) {
                auto iddObject = idd_2_9_0.getObject("OS:Schedule:Constant");
                IdfObject scheduleConstant(iddObject.get());
                std::string uuid = toString(createUUID());
                scheduleConstant.setString(0, uuid);
                scheduleConstant.setString(1, "Name");
                // eg.getString(0) is the equipment handle
                //
                boost::optional<IdfObject> _eq = idf_2_8_1.getObject(toUUID(eg.getString(0).get()));
                if (_eq) {
                  // name of zone equipment plus group index
                  scheduleConstant.setString(1, _eq->nameString() + " " + std::to_string(static_cast<int>(i)));
                }

                scheduleConstant.setString(2, "");
                scheduleConstant.setDouble(3, fraction.get());

                m_new.push_back(scheduleConstant);
                ss << scheduleConstant;

                new_eg.setString(i, uuid);
              }
            } else {
              new_eg.setString(i, eg.getString(i).get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ThermalStorage:Ice:Detailed") {
        auto iddObject = idd_2_9_0.getObject("OS:ThermalStorage:Ice:Detailed");
        IdfObject newObject(iddObject.get());

        // Inserting two fields: after position 5 and after position 6
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              // 0-5 Unchanged
              newObject.setString(i, value.get());
            } else if (i == 6) {
              // 6 (Discharge Curve) is Shifted by one field
              newObject.setString(i + 1, value.get());
            } else {
              // 7 (Charge Curve)-End Shifted by two fields
              newObject.setString(i + 2, value.get());
            }
          }
        }

        // Now deal with new fields.
        // From https://github.com/NREL/EnergyPlus/pull/7339/files#diff-6bcecd46a03668bc5e9998616e6e8066R476, E+ transition rules
        // if QuadraticLinear => FractionDischargedLMTD/FractionChargedLMTD for discharge/charge respectively
        // if CubicLinear => LMTDMassFlow
        // else, does something wrong.
        //
        // OpenStudio doesn't wrap CubicLinear. I doubt many people were using TableMultiVariableLookup and probably wouldn't more right to set that as
        // LMTDMassFlow, so let's just do ahead and set everything to Fraction(Dis)ChargedLMTD

        // DischargingCurve was in field 6. New object 6 is the Discharging Specifications, 7 is the Discharging Curve
        newObject.setString(6, "FractionDischargedLMTD");
        /*
           *boost::optional<std::string> dischargingCurveHandle = object.getString(6);
           *OS_ASSERT(dischargingCurveHandle);
           *boost::optional<IdfObject>  dischargingCurve = idf_2_8_1.getObject(toUUID(dischargingCurveHandle.get()));
           *OS_ASSERT(dischargingCurve);
           *IddObject dischargingCurveIddObject = dischargingCurve->iddObject();
           *std::string dischargingCurveIddObjectName = dischargingCurveIddObject.name();
           *if (openstudio::istringEqual(dischargingCurveIddObjectName, "OS:Curve:QuadraticLinear")) {
           *  newObject.setString(6, "FractionDischargedLMTD");
           *} else {
           *  newObject.setString(6, "LMTDMassFlow");
           *}
           */

        // ChargingCurve was in field 7. New Object 8 is the Charging Specifications, 9 is the Charging Curve
        newObject.setString(8, "FractionChargedLMTD");

        /*
           *boost::optional<std::string> chargingCurveHandle = object.getString(7);
           *OS_ASSERT(chargingCurveHandle);
           *boost::optional<IdfObject>  chargingCurve = idf_2_8_1.getObject(toUUID(chargingCurveHandle.get()));
           *OS_ASSERT(chargingCurve);
           *IddObject chargingCurveIddObject = chargingCurve->iddObject();
           *std::string chargingCurveIddObjectName = chargingCurveIddObject.name();
           *if (openstudio::istringEqual(chargingCurveIddObjectName, "OS:Curve:QuadraticLinear")) {
           *  newObject.setString(8, "FractionChargedLMTD");
           *} else {
           *  newObject.setString(8, "LMTDMassFlow");
           *}
           */

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass") {
        auto iddObject = idd_2_9_0.getObject("OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass");
        IdfObject newObject(iddObject.get());

        // We only add two fields at the end, so copy all existing
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // At 24, we add a Connection and a Node for the Plenum or Mixer Inlet Node
        auto nodeIdd = idd_2_9_0.getObject("OS:Node");
        auto nodeHandle = toString(createUUID());
        IdfObject newNode(nodeIdd.get());
        newNode.setString(0, nodeHandle);

        auto connectionIdd = idd_2_9_0.getObject("OS:Connection");
        // We pass fastname = true, to mimic the normal behavior (a connection gets a handle for name)
        IdfObject newConnection(connectionIdd.get(), true);
        auto newConnectionHandle = toString(createUUID());
        newConnection.setString(0, newConnectionHandle);
        // Name
        // Source Object: Unitary
        newConnection.setString(2, newObject.getString(0).get());
        // Outlet Port: 24
        newConnection.setInt(3, 24);

        // Target Object: Node
        newConnection.setString(4, nodeHandle);
        // Inlet Port: i=2
        newConnection.setInt(5, 2);

        // Unitary now refers to the newConnection
        newObject.setString(24, newConnectionHandle);

        newNode.setName(object.nameString() + " Plenum or Mixer Inlet Node");
        // Node Inlet Port = new connection
        newNode.setString(2, newConnectionHandle);
        // Outlet Port stays blank

        // At 25, we add a numeric field Minimum Runtime Before Operating Change, which we want to set to zero to match historical behavior
        // and what the Ctor now does
        newObject.setDouble(25, 0.0);

        // Register new objects
        m_new.push_back(newNode);
        m_new.push_back(newConnection);
        ss << newNode;
        ss << newConnection;

        // Register refactored
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Four fields were added but only the last (End Use Subcat) was implemented, but withotu transition rules either
      } else if ((iddname == "OS:HeaderedPumps:ConstantSpeed") || (iddname == "OS:HeaderedPumps:VariableSpeed")) {
        auto iddObject = idd_2_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        unsigned currentIndex = 20;
        if (iddname == "OS:HeaderedPumps:ConstantSpeed") {
          currentIndex = 15;
        }

        // DesignPowerSizingMethod
        newObject.setString(currentIndex++, "PowerPerFlowPerPressure");
        // DesignElectricPowerPerUnitFlowRate
        newObject.setDouble(currentIndex++, 348701.1);
        // DesignElectricPowerPerUnitFlowRate
        newObject.setDouble(currentIndex++, 1.282051282);

        // EndUseSubcategory
        if ((value = object.getString(currentIndex))) {
          newObject.setString(currentIndex, value.get());
        } else {
          newObject.setString(currentIndex, "General");
        }

        // Register refactored
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_9_0_to_2_9_1(const IdfFile& idf_2_9_0, const IddFileAndFactoryWrapper& idd_2_9_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_9_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_2_9_1.iddFile());
    ss << targetIdf.versionObject().get();

    boost::optional<IdfObject> alwaysOnDiscreteSchedule;

    // Add an alwaysOnDiscreteSchedule if one does not already exist
    if (!m_isComponent) {
      for (const IdfObject& object : idf_2_9_0.objects()) {
        if (object.iddObject().name() == "OS:Schedule:Constant") {
          if (boost::optional<std::string> name = object.getString(1)) {
            if (istringEqual(name.get(), "Always On Discrete")) {
              if (boost::optional<double> value = object.getDouble(3)) {
                if (equal<double>(value.get(), 1.0)) {
                  alwaysOnDiscreteSchedule = object;
                }
              }
            }
          }
        }
      }

      if (!alwaysOnDiscreteSchedule) {
        alwaysOnDiscreteSchedule = IdfObject(idd_2_9_1.getObject("OS:Schedule:Constant").get());

        alwaysOnDiscreteSchedule->setString(0, toString(createUUID()));
        alwaysOnDiscreteSchedule->setString(1, "Always On Discrete");
        alwaysOnDiscreteSchedule->setDouble(3, 1.0);

        IdfObject typeLimits(idd_2_9_1.getObject("OS:ScheduleTypeLimits").get());
        typeLimits.setString(0, toString(createUUID()));
        typeLimits.setString(1, "Always On Discrete Limits");
        typeLimits.setDouble(2, 0.0);
        typeLimits.setDouble(3, 1.0);
        typeLimits.setString(4, "Discrete");
        typeLimits.setString(5, "Availability");

        alwaysOnDiscreteSchedule->setString(2, typeLimits.getString(0).get());

        ss << alwaysOnDiscreteSchedule.get();
        ss << typeLimits;

        // Register new objects
        m_new.push_back(alwaysOnDiscreteSchedule.get());
        m_new.push_back(typeLimits);
      }
    }  // End locating or creating alwaysOnDiscreteSchedule

    for (IdfObject& object : idf_2_9_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AvailabilityManager:NightCycle") {
        auto iddObject = idd_2_9_1.getObject("OS:AvailabilityManager:NightCycle");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Applicability Schedule
        newObject.setString(2, alwaysOnDiscreteSchedule->getString(0).get());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_2_9_1_to_3_0_0(const IdfFile& idf_2_9_1, const IddFileAndFactoryWrapper& idd_3_0_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_2_9_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_0_0.iddFile());
    ss << targetIdf.versionObject().get();

    // Making the map case-insensitive by providing a Comparator `IstringCompare`
    const std::map<std::string, std::string, openstudio::IstringCompare> replaceFuelTypesMap({
      {"FuelOil#1", "FuelOilNo1"},
      {"FuelOil#2", "FuelOilNo2"},
      {"Gas", "NaturalGas"},
      {"PropaneGas", "Propane"},
    });

    const std::multimap<std::string, int> fuelTypeRenamesMap({
      {"OS:OtherEquipment", 6},                                // Fuel Type
      {"OS:Exterior:FuelEquipment", 4},                        // Fuel Use Type
      {"OS:AirConditioner:VariableRefrigerantFlow", 67},       // Fuel Type
      {"OS:Boiler:Steam", 2},                                  // Fuel Type
      {"OS:Coil:Cooling:DX:MultiSpeed", 16},                   // Fuel Type
      {"OS:Coil:Heating:Gas", 11},                             // Fuel Type
      {"OS:Coil:Heating:DX:MultiSpeed", 16},                   // Fuel Type
      {"OS:WaterHeater:Mixed", 11},                            // Heater Fuel Type
      {"OS:WaterHeater:Mixed", 15},                            // Off Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Mixed", 18},                            // On Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Stratified", 17},                       // Heater Fuel Type
      {"OS:WaterHeater:Stratified", 20},                       // Off Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Stratified", 24},                       // On Cycle Parasitic Fuel Type
      {"OS:Generator:MicroTurbine", 13},                       // Fuel Type
      {"OS:LifeCycleCost:UsePriceEscalation", 2},              // Resource
      {"OS:Meter:Custom", 2},                                  // Fuel Type
      {"OS:Meter:CustomDecrement", 2},                         // Fuel Type
      {"OS:EnergyManagementSystem:MeteredOutputVariable", 5},  // Resource Type

      // Note JM 2020-03-05: OS:BoilerHotWater is handled in its own block below
      // {"OS:Boiler:HotWater", 2},  // Fuel Type
    });

    auto checkIfReplaceNeeded = [replaceFuelTypesMap](const IdfObject& object, int fieldIndex) -> bool {
      // std::map::contains() only in C++20
      if (boost::optional<std::string> _fuelType = object.getString(fieldIndex)) {
        return replaceFuelTypesMap.find(_fuelType.get()) != replaceFuelTypesMap.end();
      }
      return false;
    };

    auto replaceForField = [&replaceFuelTypesMap](const IdfObject& object, IdfObject& newObject, int fieldIndex) -> void {
      if (boost::optional<std::string> _fuelType = object.getString(fieldIndex)) {
        auto it = replaceFuelTypesMap.find(_fuelType.get());
        if (it != replaceFuelTypesMap.end()) {
          LOG(Trace, "Replacing " << _fuelType.get() << " with " << it->second << " at fieldIndex " << fieldIndex << " for " << object.nameString());
          newObject.setString(fieldIndex, it->second);
        }
      }
    };

    for (IdfObject& object : idf_2_9_1.objects()) {
      auto iddname = object.iddObject().name();

      if (fuelTypeRenamesMap.find(iddname) != fuelTypeRenamesMap.end()) {
        LOG(Trace, "Checking for a fuel type rename in Object of type '" << iddname << "' and named '" << object.nameString() << "'");
        auto rangeFields = fuelTypeRenamesMap.equal_range(iddname);
        // First pass, find if a replacement is needed
        bool isReplaceNeeded = false;
        for (auto it = rangeFields.first; it != rangeFields.second; ++it) {
          if (checkIfReplaceNeeded(object, it->second)) {
            isReplaceNeeded = true;
            break;
          }
        }
        if (isReplaceNeeded) {
          LOG(Trace, "Replace needed!");

          // Make a new object, and copy evertything in place
          auto iddObject = idd_3_0_0.getObject(iddname);
          IdfObject newObject(iddObject.get());
          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }

          // Then handle the renames
          for (auto it = rangeFields.first; it != rangeFields.second; ++it) {
            replaceForField(object, newObject, it->second);
          }

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          // No-op
          ss << object;
        }

      } else if (iddname == "OS:Material") {
        auto iddObject = idd_3_0_0.getObject("OS:Material");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          // Specific Heat
          if (i == 6) {
            // This field is required so it should always be initialized
            if (boost::optional<double> _value = object.getDouble(6)) {
              if (_value.get() == 0.1) {
                LOG(Warn, "Updated Specific Heat for OS:Material named '" << object.nameString() << "' from " << value.get()
                                                                          << " to the new default of 1400.0.");
                newObject.setDouble(6, 1400.0);
              } else if (_value.get() < 100) {
                LOG(Warn, "Updated Specific Heat for OS:Material named '" << object.nameString() << "' from " << value.get()
                                                                          << " to the new minimum of 100.0.");
                newObject.setDouble(6, 100.0);
              } else {
                newObject.setDouble(6, _value.get());
              }
            } else {
              // Let's be safe
              newObject.setDouble(6, 1400.0);
            }
          } else if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Schedule:Rule") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Removed field 12: Apply Holiday
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 12) {
              newObject.setString(i, value.get());
            } else if (i > 12) {
              newObject.setString(i - 1, value.get());
            }  // i == 12: skip
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Note: OS:ScheduleRuleset got a new optional field at the end, so no-op
        // } else if (iddname == "OS:Schedule:Ruleset") {

      } else if (iddname == "OS:ZoneHVAC:UnitHeater") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          // Maximum Hot Water Flow Rate: this wasn't explicitly set as autosize in Ctor as it should have been
          if (i == 9) {
            if (boost::optional<double> _value = object.getDouble(i)) {
              newObject.setDouble(i, _value.get());
            } else {
              // If not a double, either blank or actual autosize => set to autosize
              newObject.setString(i, "autosize");
            }
          } else if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ClimateZones") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Deleted Field 1 & 2 (0-indexed): 'Active Institution' and 'Active Year'
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 1) {
              // 0 Unchanged
              newObject.setString(i, value.get());
            } else if (i > 2) {
              // 3-End shifted -2
              newObject.setString(i - 2, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Boiler:HotWater") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Deleted Field 7: Design Water Outlet Temperature
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              // 0-6 Unchanged
              newObject.setString(i, value.get());
            } else if (i > 7) {
              // 8-End shifted -1
              newObject.setString(i - 1, value.get());
            }
          }
        }

        // Fuel Type: renames
        replaceForField(object, newObject, 2);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Chiller:Electric:EIR") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          // 6: Reference Chilled Water Flow Rate: used to default to autosize in IDD, now required
          if (i == 6) {
            if (boost::optional<double> _value = object.getDouble(i)) {
              newObject.setDouble(i, _value.get());
            } else {
              // If not a double, it's Autosize (either hard-set, or by default from 2.9.1 IDD)
              newObject.setString(i, "Autosize");
            }

            // 24: Design Heat Recovery Water Flow Rate: was already required, and used to default to 0.0 in CTOR:
            //     * Technically we could get away doing nothing here.
            //     * Instead we'll check if the value is 0.0 (old ctor default) and that it's not connected to a HR Loop
            //       by checking field 25 ('Heat Recovery Inlet Node Name'), in which case we switch it to Autosize
          } else if (i == 24) {
            if (boost::optional<double> _value = object.getDouble(i)) {
              newObject.setDouble(i, _value.get());

              // Unless it was 0.0 (default ctor) and not connected to a HR loop, then switch it to Autosize
              if (_value.get() == 0.0) {
                if (!object.getString(25).has_value()) {
                  newObject.setString(i, "Autosize");
                }
              }
            } else {
              // Should never get here, but just in case...
              newObject.setString(i, "Autosize");
            }

            // 31: Condenser Heat Recovery Relative Capacity Fraction => now required, defaults to 1.0
          } else if (i == 31) {
            if (boost::optional<double> _value = object.getDouble(i)) {
              newObject.setDouble(i, _value.get());
            } else {
              newObject.setDouble(i, 1.0);
            }

            // All other fields: unchanged
          } else if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ShadowCalculation") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Handle
        for (size_t i = 0; i < object.numFields(); ++i) {
          value = object.getString(i);
          if (value && !value->empty()) {
            switch (i) {
              case 0:  // Handle
                newObject.setString(0, value.get());
                break;
              case 1:  // Calculation Frequency => Shading Calculation Update Frequency
                newObject.setString(3, value.get());
                break;
              case 2:  // Maximum Figures in Shadow Overlap Calculations
                newObject.setString(4, value.get());
                break;
              case 3:  // Polygon Clipping Algorithm
                newObject.setString(5, value.get());
                break;
              case 4:  // Polygon Clipping Algorithm
                newObject.setString(7, value.get());
                break;
              case 5:  // Calculation Method => Shading Calculation Update Frequency Method + key rename
                if (openstudio::istringEqual("TimestepFrequency", value.get())) {
                  newObject.setString(2, "Timestep");
                } else {  // AverageOverDaysInFrequency
                  newObject.setString(2, "Periodic");
                }
                break;
              default:
                LOG(Error, "ShadowCalculation appears to have had more than 6 fields which is impossible");
                OS_ASSERT(false);
                break;
            }
          }
        }

        // NEW REQUIRED FIELDS

        // Shading Calculation Method
        newObject.setString(1, "PolygonClipping");
        // Pixel Counting Resolution
        newObject.setInt(6, 512);
        // Output External Shading Calculation Results
        newObject.setString(8, "No");
        // Disable Self-Shading Within Shading Zone Groups
        newObject.setString(9, "No");
        // Disable Self-Shading From Shading Zone Groups to Other Zones
        newObject.setString(10, "No");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Sizing:Zone") {
        auto iddObject = idd_3_0_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // I moved fields 22 & 23 to the end (Design Zone Air Distribution Effectiveness in Cooling|Heating Mode)
        // to group all fields that belong onto DesignSpecification:ZoneAirDistribution in E+ together
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 22) {
              newObject.setString(i, value.get());
            } else if (i < 24) {
              // No need to initialize these fields by default especially now that they're at the end
              if (!value->empty()) {
                newObject.setString(i + 4, value.get());
              }
            } else {
              newObject.setString(i - 2, value.get());
            }
          }
        }

        // Two fields were plain added to the end: Design Zone Secondary Recirculation Fraction,
        // and  Design Minimum Zone Ventilation Efficiency, but both are optional (has default) so no-op there

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow") {
        // Note #3687 was originally planned for 2.9.0 inclusion, so VT was there. But it was only merged to develop3 and hence relased in 3.0.0
        // Moving it in the right location, as needed per #4016
        auto iddObject = idd_3_0_0.getObject("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow");
        IdfObject newObject(iddObject.get());

        // Added fields at end, so copy everything existing in place
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // 21 = AVM List
        // 22 = Design Spec ZoneHVAC Sizing
        // 23 = Supplemental Heating Coil (optional)
        // Maximum SAT for Supplemental Heater
        newObject.setString(24, "Autosize");
        // Maximum OATdb for Supplemental Heater
        newObject.setDouble(25, 21.0);

        // Register refactored
        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();
  }

  std::string VersionTranslator::update_3_0_0_to_3_0_1(const IdfFile& idf_3_0_0, const IddFileAndFactoryWrapper& idd_3_0_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_0_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_0_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_3_0_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Coil:Cooling:DX:SingleSpeed") {
        // Inserted field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' at position 15 (0-indexed)
        auto iddObject = idd_3_0_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 15) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(15, -25.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode") {
        // Inserted field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' at position 15 (0-indexed)
        auto iddObject = idd_3_0_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 15) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(15, -25.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Coil:Cooling:DX:MultiSpeed") {
        // Inserted field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' at position 7 (0-indexed)
        auto iddObject = idd_3_0_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(7, -25.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Coil:Cooling:DX:VariableSpeed") {
        // Inserted field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' at position 15 (0-indexed)
        auto iddObject = idd_3_0_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 15) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(15, -25.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Coil:Cooling:DX:TwoSpeed") {
        // Inserted 'Unit Internal Static Air Pressure' at field 7
        // Inserted field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' at position 23 (0-indexed)
        auto iddObject = idd_3_0_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              // Handle
              newObject.setString(i, value.get());
            } else if (i < 22) {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            } else {
              // Every other is shifted by two fields
              newObject.setString(i + 2, value.get());
            }
          }
        }

        // Set new field per I/O ref /source code default, same as Model Ctor, since it was made required-field
        newObject.setDouble(7, 773.3);

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(23, -25.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_0_0_to_3_0_1

  std::string VersionTranslator::update_3_0_1_to_3_1_0(const IdfFile& idf_3_0_1, const IddFileAndFactoryWrapper& idd_3_1_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_0_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_1_0.iddFile());
    ss << targetIdf.versionObject().get();

    /*****************************************************************************************************************************************************
       *                                                               Output:Variable fuel                                                                *
       *****************************************************************************************************************************************************/

    const static boost::regex re_strip_multiple_spaces("[' ']{2,}");

    // Making the map case-insensitive by providing a Comparator `IstringCompare`
    // https://github.com/NREL/EnergyPlus/blob/v9.4.0-IOFreeze/src/Transition/SupportFiles/Report%20Variables%209-3-0%20to%209-4-0.csv
    const static std::map<std::string, std::string, openstudio::IstringCompare> replaceOutputVariablesMap({
      {"Other Equipment FuelOil#1 Rate", "Other Equipment FuelOilNo1 Rate"},
      {"Other Equipment FuelOil#2 Rate", "Other Equipment FuelOilNo2 Rate"},
      {"Exterior Equipment FuelOil#1 Energy", "Exterior Equipment FuelOilNo1 Energy"},
      {"Exterior Equipment FuelOil#2 Energy", "Exterior Equipment FuelOilNo2 Energy"},
      {"FuelOil#1:Facility", "FuelOilNo1:Facility"},
      {"FuelOil#2:Facility", "FuelOilNo2:Facility"},
      {"FuelOil#1:HVAC", "FuelOilNo1:HVAC"},
      {"FuelOil#2:HVAC", "FuelOilNo2:HVAC"},
      {"ExteriorEquipment:FuelOil#1", "ExteriorEquipment:FuelOilNo1"},
      {"Cooling:FuelOil#1", "Cooling:FuelOilNo1"},
      {"Heating:FuelOil#1", "Heating:FuelOilNo1"},
      {"WaterSystems:FuelOil#1", "WaterSystems:FuelOilNo1"},
      {"Cogeneration:FuelOil#1", "Cogeneration:FuelOilNo1"},
      {"ExteriorEquipment:FuelOil#2", "ExteriorEquipment:FuelOilNo2"},
      {"Cooling:FuelOil#2", "Cooling:FuelOilNo2"},
      {"Heating:FuelOil#2", "Heating:FuelOilNo2"},
      {"WaterSystems:FuelOil#2", "WaterSystems:FuelOilNo2"},
      {"Cogeneration:FuelOil#2", "Cogeneration:FuelOilNo2"},
      {"Chiller FuelOil#1 Rate", "Chiller FuelOilNo1 Rate"},
      {"Chiller FuelOil#1 Energy", "Chiller FuelOilNo1 Energy"},
      {"Chiller FuelOil#1 Mass Flow Rate", "Chiller FuelOilNo1 Mass Flow Rate"},
      {"Chiller FuelOil#2 Rate", "Chiller FuelOilNo2 Rate"},
      {"Chiller FuelOil#2 Energy", "Chiller FuelOilNo2 Energy"},
      {"Chiller FuelOil#2 Mass Flow Rate", "Chiller FuelOilNo2 Mass Flow Rate"},
      {"Boiler FuelOil#1 Rate", "Boiler FuelOilNo1 Rate"},
      {"Boiler FuelOil#1 Energy", "Boiler FuelOilNo1 Energy"},
      {"Boiler FuelOil#2 Rate", "Boiler FuelOilNo2 Rate"},
      {"Boiler FuelOil#2 Energy", "Boiler FuelOilNo2 Energy"},
      {"Cooling Coil FuelOil#1 Power", "Cooling Coil FuelOilNo1 Power"},
      {"Cooling Coil FuelOil#1 Energy", "Cooling Coil FuelOilNo1 Energy"},
      {"Cooling Coil FuelOil#2 Power", "Cooling Coil FuelOilNo2 Power"},
      {"Cooling Coil FuelOil#2 Energy", "Cooling Coil FuelOilNo2 Energy"},
      {"Generator FuelOil#1 Rate", "Generator FuelOilNo1 Rate"},
      {"Generator FuelOil#1 Energy", "Generator FuelOilNo1 Energy"},
      {"Generator FuelOil#1 Mass Flow Rate", "Generator FuelOilNo1 Mass Flow Rate"},
      {"Generator FuelOil#2 Rate", "Generator FuelOilNo2 Rate"},
      {"Generator FuelOil#2 Energy", "Generator FuelOilNo2 Energy"},
      {"Generator FuelOil#2 Mass Flow Rate", "Generator FuelOilNo2 Mass Flow Rate"},
      {"Generator FuelOil#1 HHV Basis Rate", "Generator FuelOilNo1 HHV Basis Rate"},
      {"Generator FuelOil#1 HHV Basis Energy", "Generator FuelOilNo1 HHV Basis Energy"},
      {"Generator FuelOil#2 HHV Basis Rate", "Generator FuelOilNo2 HHV Basis Rate"},
      {"Generator FuelOil#2 HHV Basis Energy", "Generator FuelOilNo2 HHV Basis Energy"},
      {"FuelOil#2Emissions:Source", "FuelOilNo2Emissions:Source"},
      {"FuelOil#2Emissions:CO2", "FuelOilNo2Emissions:CO2"},
      {"FuelOil#2Emissions:CO", "FuelOilNo2Emissions:CO"},
      {"FuelOil#2Emissions:CH4", "FuelOilNo2Emissions:CH4"},
      {"FuelOil#2Emissions:NOx", "FuelOilNo2Emissions:NOx"},
      {"FuelOil#2Emissions:N2O", "FuelOilNo2Emissions:N2O"},
      {"FuelOil#2Emissions:SO2", "FuelOilNo2Emissions:SO2"},
      {"FuelOil#2Emissions:PM", "FuelOilNo2Emissions:PM"},
      {"FuelOil#2Emissions:PM10", "FuelOilNo2Emissions:PM10"},
      {"FuelOil#2Emissions:PM2.5", "FuelOilNo2Emissions:PM2.5"},
      {"FuelOil#2Emissions:NH3", "FuelOilNo2Emissions:NH3"},
      {"FuelOil#2Emissions:NMVOC", "FuelOilNo2Emissions:NMVOC"},
      {"FuelOil#2Emissions:Hg", "FuelOilNo2Emissions:Hg"},
      {"FuelOil#2Emissions:Pb", "FuelOilNo2Emissions:Pb"},
      {"FuelOil#2Emissions:WaterEnvironmentalFactors", "FuelOilNo2Emissions:WaterEnvironmentalFactors"},
      {"FuelOil#2Emissions:Nuclear High", "FuelOilNo2Emissions:Nuclear High"},
      {"FuelOil#2Emissions:Nuclear Low", "FuelOilNo2Emissions:Nuclear Low"},
      {"FuelOil#1Emissions:Source", "FuelOilNo1Emissions:Source"},
      {"FuelOil#1Emissions:CO2", "FuelOilNo1Emissions:CO2"},
      {"FuelOil#1Emissions:CO", "FuelOilNo1Emissions:CO"},
      {"FuelOil#1Emissions:CH4", "FuelOilNo1Emissions:CH4"},
      {"FuelOil#1Emissions:NOx", "FuelOilNo1Emissions:NOx"},
      {"FuelOil#1Emissions:N2O", "FuelOilNo1Emissions:N2O"},
      {"FuelOil#1Emissions:SO2", "FuelOilNo1Emissions:SO2"},
      {"FuelOil#1Emissions:PM", "FuelOilNo1Emissions:PM"},
      {"FuelOil#1Emissions:PM10", "FuelOilNo1Emissions:PM10"},
      {"FuelOil#1Emissions:PM2.5", "FuelOilNo1Emissions:PM2.5"},
      {"FuelOil#1Emissions:NH3", "FuelOilNo1Emissions:NH3"},
      {"FuelOil#1Emissions:NMVOC", "FuelOilNo1Emissions:NMVOC"},
      {"FuelOil#1Emissions:Hg", "FuelOilNo1Emissions:Hg"},
      {"FuelOil#1Emissions:Pb", "FuelOilNo1Emissions:Pb"},
      {"FuelOil#1Emissions:WaterEnvironmentalFactors", "FuelOilNo1Emissions:WaterEnvironmentalFactors"},
      {"FuelOil#1Emissions:Nuclear High", "FuelOilNo1Emissions:Nuclear High"},
      {"FuelOil#1Emissions:Nuclear Low", "FuelOilNo1Emissions:Nuclear Low"},
      {"Environmental Impact Fuel Oil #2 CO2 Emissions Mass", "Environmental Impact FuelOilNo2 CO2 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 Source Energy", "Environmental Impact FuelOilNo2 Source Energy"},
      {"Environmental Impact Fuel Oil #2 CO2 Emissions Mass", "Environmental Impact FuelOilNo2 CO2 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 CO Emissions Mass", "Environmental Impact FuelOilNo2 CO Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 CH4 Emissions Mass", "Environmental Impact FuelOilNo2 CH4 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 NOx Emissions Mass", "Environmental Impact FuelOilNo2 NOx Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 N2O Emissions Mass", "Environmental Impact FuelOilNo2 N2O Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 SO2 Emissions Mass", "Environmental Impact FuelOilNo2 SO2 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 PM Emissions Mass", "Environmental Impact FuelOilNo2 PM Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 PM10 Emissions Mass", "Environmental Impact FuelOilNo2 PM10 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 PM2.5 Emissions Mass", "Environmental Impact FuelOilNo2 PM2.5 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 NH3 Emissions Mass", "Environmental Impact FuelOilNo2 NH3 Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 NMVOC Emissions Mass", "Environmental Impact FuelOilNo2 NMVOC Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 Hg Emissions Mass", "Environmental Impact FuelOilNo2 Hg Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 Pb Emissions Mass", "Environmental Impact FuelOilNo2 Pb Emissions Mass"},
      {"Environmental Impact Fuel Oil #2 Water Consumption Volume", "Environmental Impact FuelOilNo2 Water Consumption Volume"},
      {"Environmental Impact Fuel Oil #2 Nuclear High Level Waste Mass", "Environmental Impact FuelOilNo2 Nuclear High Level Waste Mass"},
      {"Environmental Impact Fuel Oil #2 Nuclear Low Level Waste Volume", "Environmental Impact FuelOilNo2 Nuclear Low Level Waste Volume"},
      {"Environmental Impact Fuel Oil #1 Source Energy", "Environmental Impact FuelOilNo1 Source Energy"},
      {"Environmental Impact Fuel Oil #1 CO2 Emissions Mass", "Environmental Impact FuelOilNo1 CO2 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 CO Emissions Mass", "Environmental Impact FuelOilNo1 CO Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 CH4 Emissions Mass", "Environmental Impact FuelOilNo1 CH4 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 NOx Emissions Mass", "Environmental Impact FuelOilNo1 NOx Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 N2O Emissions Mass", "Environmental Impact FuelOilNo1 N2O Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 SO2 Emissions Mass", "Environmental Impact FuelOilNo1 SO2 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 PM Emissions Mass", "Environmental Impact FuelOilNo1 PM Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 PM10 Emissions Mass", "Environmental Impact FuelOilNo1 PM10 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 PM2.5 Emissions Mass", "Environmental Impact FuelOilNo1 PM2.5 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 NH3 Emissions Mass", "Environmental Impact FuelOilNo1 NH3 Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 NMVOC Emissions Mass", "Environmental Impact FuelOilNo1 NMVOC Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 Hg Emissions Mass", "Environmental Impact FuelOilNo1 Hg Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 Pb Emissions Mass", "Environmental Impact FuelOilNo1 Pb Emissions Mass"},
      {"Environmental Impact Fuel Oil #1 Water Consumption Volume", "Environmental Impact FuelOilNo1 Water Consumption Volume"},
      {"Environmental Impact Fuel Oil #1 Nuclear High Level Waste Mass", "Environmental Impact FuelOilNo1 Nuclear High Level Waste Mass"},
      {"Environmental Impact Fuel Oil #1 Nuclear Low Level Waste Volume", "Environmental Impact FuelOilNo1 Nuclear Low Level Waste Volume"},
      {"Baseboard Electric Energy", "Baseboard Electricity Energy"},
      {"Baseboard Electric Power", "Baseboard Electricity Rate"},
      {"Boiler Electric Power", "Boiler Electricity Rate"},
      {"Boiler Electric Energy", "Boiler Electricity Energy"},
      {"Boiler Ancillary Electric Power", "Boiler Ancillary Electricity Rate"},
      {"Boiler Ancillary Electric Energy", "Boiler Ancillary Electricity Energy"},
      {"Generator Produced Electric Power", "Generator Produced AC Electricity Rate"},
      {"Generator Produced Electric Energy", "Generator Produced AC Electricity Energy"},
      {"Chiller Electric Power", "Chiller Electricity Rate"},
      {"Chiller Electric Energy", "Chiller Electricity Energy"},
      {"Chiller Condenser Fan Electric Power", "Chiller Condenser Fan Electricity Rate"},
      {"Chiller Condenser Fan Electric Energy", "Chiller Condenser Fan Electricity Energy"},
      {"Chiller Basin Heater Electric Power", "Chiller Basin Heater Electricity Rate"},
      {"Chiller Basin Heater Electric Energy", "Chiller Basin Heater Electricity Energy"},
      {"Chiller Heater Electric Power", "Chiller Heater Electricity Rate"},
      {"Chiller Heater Electric Energy", "Chiller Heater Electricity Energy"},
      {"Chiller Heater Cooling Electric Power", "Chiller Heater Cooling Electricity Rate"},
      {"Chiller Heater Cooling Electric Energy", "Chiller Heater Cooling Electricity Energy"},
      {"Chiller Heater Heating Electric Power", "Chiller Heater Heating Electricity Rate"},
      {"Chiller Heater Heating Electric Energy", "Chiller Heater Heating Electricity Energy"},
      {"Cooling Coil Electric Power", "Cooling Coil Electricity Rate"},
      {"Cooling Coil Electric Energy", "Cooling Coil Electricity Energy"},
      {"Cooling Coil Crankcase Heater Electric Power", "Cooling Coil Crankcase Heater Electricity Rate"},
      {"Cooling Coil Crankcase Heater Electric Energy", "Cooling Coil Crankcase Heater Electricity Energy"},
      {"Cooling Coil Basin Heater Electric Power", "Cooling Coil Basin Heater Electricity Rate"},
      {"Cooling Coil Basin Heater Electric Energy", "Cooling Coil Basin Heater Electricity Energy"},
      {"Cooling Coil Evaporative Condenser Pump Electric Power", "Cooling Coil Evaporative Condenser Pump Electricity Rate"},
      {"Cooling Coil Evaporative Condenser Pump Electric Energy", "Cooling Coil Evaporative Condenser Pump Electricity Energy"},
      {"Cooling Tower Fan Electric Power", "Cooling Tower Fan Electricity Rate"},
      {"Cooling Tower Fan Electric Energy", "Cooling Tower Fan Electricity Energy"},
      {"Cooling Tower Basin Heater Electric Power", "Cooling Tower Basin Heater Electricity Rate"},
      {"Zone Cooltower Pump Electric Power", "Zone Cooltower Pump Electricity Rate"},
      {"Zone Cooltower Pump Electric Energy", "Zone Cooltower Pump Electricity Energy"},
      {"Heating Coil Electric Power", "Heating Coil Electricity Rate"},
      {"Heating Coil Electric Energy", "Heating Coil Electricity Energy"},
      {"Heating Coil Defrost Electric Power", "Heating Coil Defrost Electricity Rate"},
      {"Heating Coil Crankcase Heater Electric Power", "Heating Coil Crankcase Heater Electricity Rate"},
      {"Heating Coil Crankcase Heater Electric Energy", "Heating Coil Crankcase Heater Electricity Energy"},
      {"Cooling Coil Water Heating Electric Power", "Cooling Coil Water Heating Electricity Rate"},
      {"Cooling Coil Water Heating Electric Energy", "Cooling Coil Water Heating Electricity Energy"},
      {"Dehumidifier Electric Power", "Dehumidifier Electricity Rate"},
      {"Dehumidifier Electric Energy", "Dehumidifier Electricity Energy"},
      {"Dehumidifier Exhaust Fan Electric Power", "Dehumidifier Exhaust Fan Electricity Rate"},
      {"Dehumidifier Exhaust Fan Electric Energy", "Dehumidifier Exhaust Fan Electricity Energy"},
      {"Earth Tube Fan Electric Energy", "Earth Tube Fan Electricity Energy"},
      {"Earth Tube Fan Electric Power", "Earth Tube Fan Electricity Rate"},
      {"Gas:Facility", "NaturalGas:Facility"},
      {"Gas:Building", "NaturalGas:Building"},
      {"Gas:Plant", "NaturalGas:Plant"},
      {"Gas:HVAC", "NaturalGas:HVAC"},
      {"Facility Total Purchased Electric Power", "Facility Total Purchased Electricity Rate"},
      {"Facility Total Purchased Electric Energy", "Facility Total Purchased Electricity Energy"},
      {"Facility Total Surplus Electric Power", "Facility Total Surplus Electricity Rate"},
      {"Facility Total Surplus Electric Energy", "Facility Total Surplus Electricity Energy"},
      {"Facility Net Purchased Electric Power", "Facility Net Purchased Electricity Rate"},
      {"Facility Net Purchased Electric Energy", "Facility Net Purchased Electricity Energy"},
      {"Facility Total Building Electric Demand Power", "Facility Total Building Electricity Demand Rate"},
      {"Facility Total HVAC Electric Demand Power", "Facility Total HVAC Electricity Demand Rate"},
      {"Facility Total Electric Demand Power", "Facility Total Electricity Demand Rate"},
      {"Facility Total Produced Electric Power", "Facility Total Produced Electricity Rate"},
      {"Facility Total Produced Electric Energy", "Facility Total Produced Electricity Energy"},
      {"Electric Load Center Produced Electric Power", "Electric Load Center Produced Electricity Rate"},
      {"Electric Load Center Produced Electric Energy", "Electric Load Center Produced Electricity Energy"},
      {"Electric Load Center Supplied Electric Power", "Electric Load Center Supplied Electricity Rate"},
      {"Electric Load Center Drawn Electric Power", "Electric Load Center Drawn Electricity Rate"},
      {"Electric Load Center Requested Electric Power", "Electric Load Center Requested Electricity Rate"},
      {"Generator Requested Electric Power", "Generator Requested Electricity Rate"},
      {"Inverter DC Input Electric Power", "Inverter DC Input Electricity Rate"},
      {"Inverter DC Input Electric Energy", "Inverter DC Input Electricity Energy"},
      {"Inverter AC Output Electric Power", "Inverter AC Output Electricity Rate"},
      {"Inverter AC Output Electric Energy", "Inverter AC Output Electricity Energy"},
      {"Inverter Ancillary AC Electric Power", "Inverter Ancillary AC Electricity Rate"},
      {"Inverter Ancillary AC Electric Energy", "Inverter Ancillary AC Electricity Energy"},
      {"Converter AC Input Electric Power", "Converter AC Input Electricity Rate"},
      {"Converter AC Input Electric Energy", "Converter AC Input Electricity Energy"},
      {"Converter DC Output Electric Power", "Converter DC Output Electricity Rate"},
      {"Converter DC Output Electric Energy", "Converter DC Output Electricity Energy"},
      {"Converter Electric Loss Power", "Converter Electricity Loss Rate"},
      {"Converter Electric Loss Energy", "Converter Electricity Loss Energy"},
      {"Converter Electric Loss Decrement Energy", "Converter Electricity Loss Decrement Energy"},
      {"Converter Ancillary AC Electric Power", "Converter Ancillary AC Electricity Rate"},
      {"Converter Ancillary AC Electric Energy", "Converter Ancillary AC Electricity Energy"},
      {"Transformer Input Electric Power", "Transformer Input Electricity Rate"},
      {"Transformer Input Electric Energy", "Transformer Input Electricity Energy"},
      {"Transformer Output Electric Power", "Transformer Output Electricity Rate"},
      {"Transformer Output Electric Energy", "Transformer Output Electricity Energy"},
      {"Transformer Distribution Electric Loss Energy", "Transformer Distribution Electricity Loss Energy"},
      {"Transformer Cogeneration Electric Loss Energy", "Transformer Cogeneration Electricity Loss Energy"},
      {"Transformer Conversion Electric Loss Energy", "Transformer Conversion Electricity Loss Energy"},
      {"Evaporative Cooler Electric Energy", "Evaporative Cooler Electricity Energy"},
      {"Evaporative Cooler Electric Power", "Evaporative Cooler Electricity Rate"},
      {"Exterior Lights Electric Power", "Exterior Lights Electricity Rate"},
      {"Exterior Lights Electric Energy", "Exterior Lights Electricity Energy"},
      {"Fan Coil Fan Electric Power", "Fan Coil Fan Electricity Rate"},
      {"Fan Coil Fan Electric Energy", "Fan Coil Fan Electricity Energy"},
      {"Fan Electric Power", "Fan Electricity Rate"},
      {"Fan Electric Energy", "Fan Electricity Energy"},
      {"Generator Blower Electric Power", "Generator Blower Electricity Rate"},
      {"Generator Blower Electric Energy", "Generator Blower Electricity Energy"},
      {"Generator Fuel Compressor Electric Power", "Generator Fuel Compressor Electricity Rate"},
      {"Generator Fuel Compressor Electric Energy", "Generator Fuel Compressor Electricity Energy"},
      {"Generator Fuel Reformer Water Pump Electric Power", "Generator Fuel Reformer Water Pump Electricity Rate"},
      {"Generator Fuel Reformer Water Pump Electric Energy", "Generator Fuel Reformer Water Pump Electricity Energy"},
      {"Generator Produced DC Electric Power", "Generator Produced DC Electricity Rate"},
      {"Generator Ancillary AC Electric Power", "Generator Ancillary AC Electricity Rate"},
      {"Generator Ancillary AC Electric Energy", "Generator Ancillary AC Electricity Energy"},
      {"Unitary System Ancillary Electric Power", "Unitary System Ancillary Electricity Rate"},
      {"Unitary System Cooling Ancillary Electric Energy", "Unitary System Cooling Ancillary Electricity Energy"},
      {"Unitary System Heating Ancillary Electric Energy", "Unitary System Heating Ancillary Electricity Energy"},
      {"Unitary System Electric Power", "Unitary System Electricity Rate"},
      {"Unitary System Electric Energy", "Unitary System Electricity Energy"},
      {"Zone Ventilator Electric Power", "Zone Ventilator Electricity Rate"},
      {"Zone Ventilator Electric Energy", "Zone Ventilator Electricity Energy"},
      {"Zone VRF Air Terminal Cooling Electric Power", "Zone VRF Air Terminal Cooling Electricity Rate"},
      {"Zone VRF Air Terminal Cooling Electric Energy", "Zone VRF Air Terminal Cooling Electricity Energy"},
      {"Zone VRF Air Terminal Heating Electric Power", "Zone VRF Air Terminal Heating Electricity Rate"},
      {"Zone VRF Air Terminal Heating Electric Energy", "Zone VRF Air Terminal Heating Electricity Energy"},
      {"VRF Heat Pump Cooling Electric Power", "VRF Heat Pump Cooling Electricity Rate"},
      {"VRF Heat Pump Cooling Electric Energy", "VRF Heat Pump Cooling Electricity Energy"},
      {"VRF Heat Pump Heating Electric Power", "VRF Heat Pump Heating Electricity Rate"},
      {"VRF Heat Pump Heating Electric Energy", "VRF Heat Pump Heating Electricity Energy"},
      {"VRF Heat Pump Compressor Electric Power", "VRF Heat Pump Compressor Electricity Rate"},
      {"VRF Heat Pump Defrost Electric Power", "VRF Heat Pump Defrost Electricity Rate"},
      {"VRF Heat Pump Defrost Electric Energy", "VRF Heat Pump Defrost Electricity Energy"},
      {"VRF Heat Pump Crankcase Heater Electric Power", "VRF Heat Pump Crankcase Heater Electricity Rate"},
      {"VRF Heat Pump Crankcase Heater Electric Energy", "VRF Heat Pump Crankcase Heater Electricity Energy"},
      {"VRF Heat Pump Evaporative Condenser Pump Electric Power", "VRF Heat Pump Evaporative Condenser Pump Electricity Rate"},
      {"VRF Heat Pump Evaporative Condenser Pump Electric Energy", "VRF Heat Pump Evaporative Condenser Pump Electricity Energy"},
      {"VRF Heat Pump Basin Heater Electric Power", "VRF Heat Pump Basin Heater Electricity Rate"},
      {"VRF Heat Pump Basin Heater Electric Energy", "VRF Heat Pump Basin Heater Electricity Energy"},
      {"Zone Combined Outdoor Air Fan Electric Energy", "Zone Combined Outdoor Air Fan Electricity Energy"},
      {"Zone Ventilation Fan Electric Energy", "Zone Ventilation Fan Electricity Energy"},
      {"Heat Pump Electric Power", "Heat Pump Electricity Rate"},
      {"Heat Pump Electric Energy", "Heat Pump Electricity Energy"},
      {"Heat Exchanger Electric Power", "Heat Exchanger Electricity Rate"},
      {"Heat Exchanger Electric Energy", "Heat Exchanger Electricity Energy"},
      {"Heating Coil Gas Energy", "Heating Coil NaturalGas Energy"},
      {"Heating Coil Gas Rate", "Heating Coil NaturalGas Rate"},
      {"Heating Coil Ancillary Gas Rate", "Heating Coil Ancillary NaturalGas Rate"},
      {"Heating Coil Ancillary Gas Energy", "Heating Coil Ancillary NaturalGas Energy"},
      {"Zone Radiant HVAC Gas Rate", "Zone Radiant HVAC NaturalGas Rate"},
      {"Zone Radiant HVAC Gas Energy", "Zone Radiant HVAC NaturalGas Energy"},
      {"Zone Radiant HVAC Electric Power", "Zone Radiant HVAC Electricity Rate"},
      {"Zone Radiant HVAC Electric Energy", "Zone Radiant HVAC Electricity Energy"},
      {"Humidifier Electric Power", "Humidifier Electricity Rate"},
      {"Humidifier Electric Energy", "Humidifier Electricity Energy"},
      {"Humidifier Gas Use Thermal Efficiency", "Humidifier NaturalGas Use Thermal Efficiency"},
      {"Humidifier Gas Use Rate", "Humidifier NaturalGas Rate"},
      {"Humidifier Gas Use Energy", "Humidifier NaturalGas Energy"},
      {"Humidifier Auxiliary Electric Power", "Humidifier Auxiliary Electricity Rate"},
      {"Humidifier Auxiliary Electric Energy", "Humidifier Auxiliary Electricity Energy"},
      {"Zone Hybrid Unitary HVAC Electric Power", "Zone Hybrid Unitary HVAC Electricity Rate"},
      {"Zone Hybrid Unitary HVAC Electric Energy", "Zone Hybrid Unitary HVAC Electricity Energy"},
      {"Zone Hybrid Unitary HVAC Supply Fan Electric Power", "Zone Hybrid Unitary HVAC Supply Fan Electricity Rate"},
      {"Zone Hybrid Unitary HVAC Supply Fan Electric Energy", "Zone Hybrid Unitary HVAC Supply Fan Electricity Energy"},
      {"Ice Thermal Storage Ancillary Electric Power", "Ice Thermal Storage Ancillary Electricity Rate"},
      {"Ice Thermal Storage Ancillary Electric Energy", "Ice Thermal Storage Ancillary Electricity Energy"},
      {"Zone Lights Electric Energy", "Zone Lights Electricity Energy"},
      {"Zone Electric Equipment Electric Energy", "Zone Electric Equipment Electricity Energy"},
      {"Boiler Gas Consumption", "Boiler NaturalGas Consumption"},
      {"Boiler Gas Consumption Rate", "Boiler NaturalGas Consumption Rate"},
      {"Zone Window Air Conditioner Electric Energy", "Zone Window Air Conditioner Electricity Energy"},
      {"Zone Window Air Conditioner Electric Power", "Zone Window Air Conditioner Electricity Rate"},
      {"Water Heater Gas Consumption", "Water Heater NaturalGas Consumption"},
      {"Generator Gas Consumption", "Generator NaturalGas Consumption"},
      {"Pump Electric Energy", "Pump Electricity Energy"},
      {"Pump Electric Power", "Pump Electricity Rate"},
      {"Air System Electric Energy", "Air System Electricity Energy"},
      {"Air System Gas Energy", "Air System Naturalgas Energy"},
      {"Air System Fan Electric Energy", "Air System Fan Electricity Energy"},
      {"Air System DX Heating Coil Electric Energy", "Air System DX Heating Coil Electricity Energy"},
      {"Air System DX Cooling Coil Electric Energy", "Air System DX Cooling Coil Electricity Energy"},
      {"Air System Heating Coil Electric Energy", "Air System Heating Coil Electricity Energy"},
      {"Air System Heating Coil Gas Energy", "Air System Heating Coil NaturalGas Energy"},
      {"Air System Humidifier Electric Energy", "Air System Humidifier Electricity Energy"},
      {"Air System Evaporative Cooler Electric Energy", "Air System Evaporative Cooler Electricity Energy"},
      {"Air System Desiccant Dehumidifier Electric Energy", "Air System Desiccant Dehumidifier Electricity Energy"},
      {"Integrated Heat Pump Electric Power", "Integrated Heat Pump Electricity Rate"},
      {"Integrated Heat Pump Electric Energy", "Integrated Heat Pump Electricity Energy"},
      {"Lights Electric Power", "Lights Electricity Rate"},
      {"Lights Electric Energy", "Lights Electricity Energy"},
      {"Zone Lights Electric Power", "Zone Lights Electricity Rate"},
      {"Zone Lights Electric Energy", "Zone Lights Electricity Energy"},
      {"Electric Equipment Electric Power", "Electric Equipment Electricity Rate"},
      {"Electric Equipment Electric Energy", "Electric Equipment Electricity Energy"},
      {"Zone Electric Equipment Electric Power", "Zone Electric Equipment Electricity Rate"},
      {"Zone Electric Equipment Electric Energy", "Zone Electric Equipment Electricity Energy"},
      {"Gas Equipment Gas Rate", "Gas Equipment NaturalGas Rate"},
      {"Gas Equipment Gas Energy", "Gas Equipment NaturalGas Energy"},
      {"Zone Gas Equipment Gas Rate", "Zone Gas Equipment NaturalGas Rate"},
      {"Zone Gas Equipment Gas Energy", "Zone Gas Equipment NaturalGas Energy"},
      {"ITE CPU Electric Power", "ITE CPU Electricity Rate"},
      {"ITE Fan Electric Power", "ITE Fan Electricity Rate"},
      {"ITE UPS Electric Power", "ITE UPS Electricity Rate"},
      {"ITE CPU Electric Power at Design Inlet Conditions", "ITE CPU Electricity Rate at Design Inlet Conditions"},
      {"ITE Fan Electric Power at Design Inlet Conditions", "ITE Fan Electricity Rate at Design Inlet Conditions"},
      {"ITE CPU Electric Energy", "ITE CPU Electricity Energy"},
      {"ITE Fan Electric Energy", "ITE Fan Electricity Energy"},
      {"ITE UPS Electric Energy", "ITE UPS Electricity Energy"},
      {"ITE CPU Electric Energy at Design Inlet Conditions", "ITE CPU Electricity Energy at Design Inlet Conditions"},
      {"ITE Fan Electric Energy at Design Inlet Conditions", "ITE Fan Electricity Energy at Design Inlet Conditions"},
      {"Zone ITE CPU Electric Power", "Zone ITE CPU Electricity Rate"},
      {"Zone ITE Fan Electric Power", "Zone ITE Fan Electricity Rate"},
      {"Zone ITE UPS Electric Power", "Zone ITE UPS Electricity Rate"},
      {"Zone ITE CPU Electric Power at Design Inlet Conditions", "Zone ITE CPU Electricity Rate at Design Inlet Conditions"},
      {"Zone ITE Fan Electric Power at Design Inlet Conditions", "Zone ITE Fan Electricity Rate at Design Inlet Conditions"},
      {"Zone ITE CPU Electric Energy", "Zone ITE CPU Electricity Energy"},
      {"Zone ITE Fan Electric Energy", "Zone ITE Fan Electricity Energy"},
      {"Zone ITE UPS Electric Energy", "Zone ITE UPS Electricity Energy"},
      {"Zone ITE CPU Electric Energy at Design Inlet Conditions", "Zone ITE CPU Electricity Energy at Design Inlet Conditions"},
      {"Zone ITE Fan Electric Energy at Design Inlet Conditions", "Zone ITE Fan Electricity Energy at Design Inlet Conditions"},
      {"Zone Baseboard Electric Power", "Zone Baseboard Electricity Rate"},
      {"Zone Baseboard Electric Energy", "Zone Baseboard Electricity Energy"},
      {"Zone Radiant HVAC Pump Electric Power", "Zone Radiant HVAC Pump Electricity Rate"},
      {"Zone Radiant HVAC Pump Electric Energy", "Zone Radiant HVAC Pump Electricity Energy"},
      {"Generator Standby Electric Power", "Generator Standby Electricity Rate"},
      {"Generator Ancillary Electric Power", "Generator Ancillary Electricity Rate"},
      {"Generator Ancillary Electric Energy", "Generator Ancillary Electricity Energy"},
      {"Zone Outdoor Air Unit Fan Electric Power", "Zone Outdoor Air Unit Fan Electricity Rate"},
      {"Zone Outdoor Air Unit Fan Electric Energy", "Zone Outdoor Air Unit Fan Electricity Energy"},
      {"InteriorEquipment:Gas", "InteriorEquipment:NaturalGas"},
      {"ExteriorEquipment:Gas", "ExteriorEquipment:NaturalGas"},
      {"Heating:Gas", "Heating:NaturalGas"},
      {"Cooling:Gas", "Cooling:NaturalGas"},
      {"WaterSystems:Gas", "WaterSystems:NaturalGas"},
      {"Cogeneration:Gas", "Cogeneration:NaturalGas"},
      {"Humidifier:Gas", "Humidifier:NaturalGas"},
      {"Air System Humidifier Gas Energy", "Air System Humidifier Natural Gas Energy"},
      {"Generator Produced DC Electric Energy", "Generator Produced DC Electricity Energy"},
      {"Zone Packaged Terminal Heat Pump Electric Power", "Zone Packaged Terminal Heat Pump Electricity Rate"},
      {"Zone Packaged Terminal Heat Pump Electric Energy", "Zone Packaged Terminal Heat Pump Electricity Energy"},
      {"Zone Packaged Terminal Air Conditioner Electric Power", "Zone Packaged Terminal Air Conditioner Electricity Rate"},
      {"Zone Packaged Terminal Air Conditioner Electric Energy", "Zone Packaged Terminal Air Conditioner Electricity Energy"},
      {"Zone Water to Air Heat Pump Electric Power", "Zone Water to Air Heat Pump Electricity Rate"},
      {"Zone Water to Air Heat Pump Electric Energy", "Zone Water to Air Heat Pump Electricity Energy"},
      {"Cooling Coil Cold Weather Protection Electric Energy", "Cooling Coil Cold Weather Protection Electricity Energy"},
      {"Cooling Coil Cold Weather Protection Electric Power", "Cooling Coil Cold Weather Protection Electricity Rate"},
      {"Chiller Heater System Cooling Electric Energy", "Chiller Heater System Cooling Electricity Energy"},
      {"Chiller Heater System Heating Electric Energy", "Chiller Heater System Heating Electricity Energy"},
      {"Chiller Heater System Cooling Electric Power", "Chiller Heater System Cooling Electricity Rate"},
      {"Chiller Heater System Heating Electric Power", "Chiller Heater System Heating Electricity Rate"},
      {"Chiller Heater Cooling Electric Power Unit", "Chiller Heater Cooling Electricity Rate Unit"},
      {"Chiller Heater Heating Electric Power Unit", "Chiller Heater Heating Electricity Rate Unit"},
      {"Chiller Heater Cooling Electric Energy Unit", "Chiller Heater Cooling Electricity Energy Unit"},
      {"Chiller Heater Heating Electric Energy Unit", "Chiller Heater Heating Electricity Energy Unit"},
      {"Environmental Impact Natural Gas Source Energy", "Environmental Impact NaturalGas Source Energy"},
      {"Environmental Impact Natural Gas CO2 Emissions Mass", "Environmental Impact NaturalGas CO2 Emissions Mass"},
      {"Environmental Impact Natural Gas CO Emissions Mass", "Environmental Impact NaturalGas CO Emissions Mass"},
      {"Environmental Impact Natural Gas CH4 Emissions Mass", "Environmental Impact NaturalGas CH4 Emissions Mass"},
      {"Environmental Impact Natural Gas NOx Emissions Mass", "Environmental Impact NaturalGas NOx Emissions Mass"},
      {"Environmental Impact Natural Gas N2O Emissions Mass", "Environmental Impact NaturalGas N2O Emissions Mass"},
      {"Environmental Impact Natural Gas SO2 Emissions Mass", "Environmental Impact NaturalGas SO2 Emissions Mass"},
      {"Environmental Impact Natural Gas PM Emissions Mass", "Environmental Impact NaturalGas PM Emissions Mass"},
      {"Environmental Impact Natural Gas PM10 Emissions Mass", "Environmental Impact NaturalGas PM10 Emissions Mass"},
      {"Environmental Impact Natural Gas PM2.5 Emissions Mass", "Environmental Impact NaturalGas PM2.5 Emissions Mass"},
      {"Environmental Impact Natural Gas NH3 Emissions Mass", "Environmental Impact NaturalGas NH3 Emissions Mass"},
      {"Environmental Impact Natural Gas NMVOC Emissions Mass", "Environmental Impact NaturalGas NMVOC Emissions Mass"},
      {"Environmental Impact Natural Gas Hg Emissions Mass", "Environmental Impact NaturalGas Hg Emissions Mass"},
      {"Environmental Impact Natural Gas Pb Emissions Mass", "Environmental Impact NaturalGas Pb Emissions Mass"},
      {"Environmental Impact Natural Gas Water Consumption Volume", "Environmental Impact NaturalGas Water Consumption Volume"},
      {"Environmental Impact Natural Gas Nuclear High Level Waste Mass", "Environmental Impact NaturalGas Nuclear High Level Waste Mass"},
      {"Environmental Impact Natural Gas Nuclear Low Level Waste Volume", "Environmental Impact NaturalGas Nuclear Low Level Waste Volume"},
      {"Refrigeration Case Evaporator Fan Electric Power", "Refrigeration Case Evaporator Fan Electricity Rate"},
      {"Refrigeration Case Evaporator Fan Electric Energy", "Refrigeration Case Evaporator Fan Electricity Energy"},
      {"Refrigeration Case Lighting Electric Power", "Refrigeration Case Lighting Electricity Rate"},
      {"Refrigeration Case Lighting Electric Energy", "Refrigeration Case Lighting Electricity Energy"},
      {"Refrigeration Case Anti Sweat Electric Power", "Refrigeration Case Anti Sweat Electricity Rate"},
      {"Refrigeration Case Anti Sweat Electric Energy", "Refrigeration Case Anti Sweat Electricity Energy"},
      {"Refrigeration Case Defrost Electric Power", "Refrigeration Case Defrost Electricity Rate"},
      {"Refrigeration Case Defrost Electric Energy", "Refrigeration Case Defrost Electricity Energy"},
      {"Refrigeration Walk In Ancillary Electric Power", "Refrigeration Walk In Ancillary Electricity Rate"},
      {"Refrigeration Walk In Ancillary Electric Energy", "Refrigeration Walk In Ancillary Electricity Energy"},
      {"Refrigeration Walk In Fan Electric Power", "Refrigeration Walk In Fan Electricity Rate"},
      {"Refrigeration Walk In Fan Electric Energy", "Refrigeration Walk In Fan Electricity Energy"},
      {"Refrigeration Walk In Lighting Electric Power", "Refrigeration Walk In Lighting Electricity Rate"},
      {"Refrigeration Walk In Lighting Electric Energy", "Refrigeration Walk In Lighting Electricity Energy"},
      {"Refrigeration Walk In Heater Electric Power", "Refrigeration Walk In Heater Electricity Rate"},
      {"Refrigeration Walk In Heater Electric Energy", "Refrigeration Walk In Heater Electricity Energy"},
      {"Refrigeration Walk In Defrost Electric Power", "Refrigeration Walk In Defrost Electricity Rate"},
      {"Refrigeration Walk In Defrost Electric Energy", "Refrigeration Walk In Defrost Electricity Energy"},
      {"Refrigeration Zone Air Chiller Total Electric Power", "Refrigeration Zone Air Chiller Total Electricity Rate"},
      {"Refrigeration Zone Air Chiller Total Electric Energy", "Refrigeration Zone Air Chiller Total Electricity Energy"},
      {"Refrigeration Zone Air Chiller Fan Electric Power", "Refrigeration Zone Air Chiller Fan Electricity Rate"},
      {"Refrigeration Zone Air Chiller Heater Electric Power", "Refrigeration Zone Air Chiller Heater Electricity Rate"},
      {"Refrigeration Zone Air Chiller Heater Electric Energy", "Refrigeration Zone Air Chiller Heater Electricity Energy"},
      {"Refrigeration Zone Air Chiller Defrost Electric Power", "Refrigeration Zone Air Chiller Defrost Electricity Rate"},
      {"Refrigeration Zone Air Chiller Defrost Electric Energy", "Refrigeration Zone Air Chiller Defrost Electricity Energy"},
      {"Refrigeration Air Chiller Secondary Loop Pump Electric Power", "Refrigeration Air Chiller Secondary Loop Pump Electricity Rate"},
      {"Refrigeration Air Chiller Secondary Loop Pump Electric Energy", "Refrigeration Air Chiller Secondary Loop Pump Electricity Energy"},
      {"Refrigeration Secondary Loop Pump Electric Power", "Refrigeration Secondary Loop Pump Electricity Rate"},
      {"Refrigeration Secondary Loop Pump Electric Energy", "Refrigeration Secondary Loop Pump Electricity Energy"},
      {"Refrigeration Air Chiller Compressor Rack Electric Power", "Refrigeration Air Chiller Compressor Rack Electricity Rate"},
      {"Refrigeration Air Chiller Compressor Rack Electric Energy", "efrigeration Air Chiller Compressor Rack Electricity Energy"},
      {"Refrigeration Air Chiller Compressor Rack Condenser Fan Electric Power",
       "Refrigeration Air Chiller Compressor Rack Condenser Fan Electricity Rate"},
      {"Refrigeration Air Chiller Compressor Rack Condenser Fan Electric Energy",
       "Refrigeration Air Chiller Compressor Rack Condenser Fan Electricity Energy"},
      {"Refrigeration Air Chiller Compressor Rack Evaporative Condenser Pump Electric Power",
       "Refrigeration Air Chiller Compressor Rack Evaporative Condenser Pump Electricity Rate"},
      {"Refrigeration Air Chiller Compressor Rack Evaporative Condenser Pump Electric Energy",
       "Refrigeration Air Chiller Compressor Rack Evaporative Condenser Pump Electricity Energy"},
      {"Refrigeration Air Chiller Compressor Rack Evaporative Condenser Basin Heater Electric Power",
       "Refrigeration Air Chiller Compressor Rack Evaporative Condenser Basin Heater Electricity Rate"},
      {"Refrigeration Air Chiller Compressor Rack Evaporative Condenser Basin Heater Electric Energy",
       "Refrigeration Air Chiller Compressor Rack Evaporative Condenser Basin Heater Electricity Energy"},
      {"Refrigeration Compressor Rack Electric Power", "Refrigeration Compressor Rack Electricity Rate"},
      {"Refrigeration Compressor Rack Electric Energy", "Refrigeration Compressor Rack Electricity Energy"},
      {"Refrigeration Compressor Rack Condenser Fan Electric Power", "Refrigeration Compressor Rack Condenser Fan Electricity Rate"},
      {"Refrigeration Compressor Rack Condenser Fan Electric Energy", "Refrigeration Compressor Rack Condenser Fan Electricity Energy"},
      {"Refrigeration Compressor Rack Evaporative Condenser Pump Electric Power",
       "Refrigeration Compressor Rack Evaporative Condenser Pump Electricity Rate"},
      {"Refrigeration Compressor Rack Evaporative Condenser Pump Electric Energy",
       "Refrigeration Compressor Rack Evaporative Condenser Pump Electricity Energy"},
      {"Refrigeration Compressor Rack Evaporative Condenser Basin Heater Electric Power",
       "Refrigeration Compressor Rack Evaporative Condenser Basin Heater Electricity Rate"},
      {"Refrigeration Compressor Rack Evaporative Condenser Basin Heater Electric Energy",
       "Refrigeration Compressor Rack Evaporative Condenser Basin Heater Electricity Energy"},
      {"Refrigeration Air Chiller System Total Compressor Electric Power", "Refrigeration Air Chiller System Total Compressor Electricity Rate"},
      {"Refrigeration Air Chiller System Total Compressor Electric Energy", "Refrigeration Air Chiller System Total Compressor Electricity Energy"},
      {"Refrigeration Air Chiller System Total Low Stage Compressor Electric Power",
       "Refrigeration Air Chiller System Total Low Stage Compressor Electricity Rate"},
      {"Refrigeration Air Chiller System Total Low Stage Compressor Electric Energy",
       "Refrigeration Air Chiller System Total Low Stage Compressor Electricity Energy"},
      {"Refrigeration Air Chiller System Total High Stage Compressor Electric Power",
       "Refrigeration Air Chiller System Total High Stage Compressor Electricity Rate"},
      {"Refrigeration Air Chiller System Total High Stage Compressor Electric Energy",
       "Refrigeration Air Chiller System Total High Stage Compressor Electricity Energy"},
      {"Refrigeration Air Chiller System Total Low and High Stage Compressor Electric Energy",
       "Refrigeration Air Chiller System Total Low and High Stage Compressor Electricity Energy"},
      {"Refrigeration System Total Compressor Electric Power", "Refrigeration System Total Compressor Electricity Rate"},
      {"Refrigeration System Total Compressor Electric Energy", "Refrigeration System Total Compressor Electricity Energy"},
      {"Refrigeration System Total Low Stage Compressor Electric Power", "Refrigeration System Total Low Stage Compressor Electricity Rate"},
      {"Refrigeration System Total Low Stage Compressor Electric Energy", "Refrigeration System Total Low Stage Compressor Electricity Energy"},
      {"Refrigeration System Total High Stage Compressor Electric Power", "Refrigeration System Total High Stage Compressor Electricity Rate"},
      {"Refrigeration System Total High Stage Compressor Electric Energy", "Refrigeration System Total High Stage Compressor Electricity Energy"},
      {"Refrigeration System Total Low and High Stage Compressor Electric Energy",
       "Refrigeration System Total Low and High Stage Compressor Electricity Energy"},
      {"Refrigeration Air Chiller System Compressor Electric Power", "Refrigeration Air Chiller System Compressor Electricity Rate"},
      {"Refrigeration Air Chiller System Compressor Electric Energy", "Refrigeration Air Chiller System Compressor Electricity Energy"},
      {"Refrigeration Compressor Electric Power", "Refrigeration Compressor Electricity Rate"},
      {"Refrigeration Compressor Electric Energy", "Refrigeration Compressor Electricity Energy"},
      {"Refrigeration Air Chiller System Condenser Fan Electric Power", "Refrigeration Air Chiller System Condenser Fan Electricity Rate"},
      {"Refrigeration Air Chiller System Condenser Fan Electric Energy", "Refrigeration Air Chiller System Condenser Fan Electricity Energy"},
      {"Refrigeration Air Chiller System Condenser Fan Electric Power", "Refrigeration Air Chiller System Condenser Fan Electricity Rate"},
      {"Refrigeration Air Chiller System Condenser Fan Electric Energy", "Refrigeration Air Chiller System Condenser Fan Electricity Energy"},
      {"Refrigeration Air Chiller System Condenser Pump Electric Power", "Refrigeration Air Chiller System Condenser Pump Electricity Rate"},
      {"Refrigeration Air Chiller System Condenser Pump Electric Energy", "Refrigeration Air Chiller System Condenser Pump Electricity Energy"},
      {"Refrigeration Air Chiller System Condenser Basin Heater Electric Power",
       "Refrigeration Air Chiller System Condenser Basin Heater Electricity Rate"},
      {"Refrigeration Air Chiller System Condenser Basin Heater Electric Energy",
       "Refrigeration Air Chiller System Condenser Basin Heater Electricity Energy"},
      {"Refrigeration System Condenser Fan Electric Power", "Refrigeration System Condenser Fan Electricity Rate"},
      {"Refrigeration System Condenser Fan Electric Energy", "Refrigeration System Condenser Fan Electricity Energy"},
      {"Refrigeration System Condenser Fan Electric Power", "Refrigeration System Condenser Fan Electricity Rat"},
      {"Refrigeration System Condenser Fan Electric Energy", "Refrigeration System Condenser Fan Electricity Energy"},
      {"Refrigeration System Condenser Pump Electric Power", "Refrigeration System Condenser Pump Electricity Rate"},
      {"Refrigeration System Condenser Pump Electric Energy", "Refrigeration System Condenser Pump Electricity Energy"},
      {"Refrigeration System Condenser Basin Heater Electric Power", "Refrigeration System Condenser Basin Heater Electricity Rate"},
      {"Refrigeration System Condenser Basin Heater Electric Energy", "Refrigeration System Condenser Basin Heater Electricity Energy"},
      {"Refrigeration Transcritical System Total High Pressure Compressor Electric Power",
       "Refrigeration Transcritical System Total High Pressure Compressor Electricity Rate"},
      {"Refrigeration Transcritical System Total High Pressure Compressor Electric Energy",
       "Refrigeration Transcritical System Total High Pressure Compressor Electricity Energy"},
      {"Refrigeration Transcritical System Total Compressor Electric Energy",
       "Refrigeration Transcritical System Total Compressor Electricity Energy"},
      {"Refrigeration Transcritical System Low Pressure Compressor Electric Power",
       "Refrigeration Transcritical System Low Pressure Compressor Electricity Rate"},
      {"Refrigeration Transcritical System Low Pressure Compressor Electric Energy",
       "Refrigeration Transcritical System Low Pressure Compressor Electricity Energy"},
      {"Refrigeration Compressor Electric Power", "Refrigeration Compressor Electricity Rate"},
      {"Refrigeration Compressor Electric Energy", "Refrigeration Compressor Electricity Energy"},
      {"Refrigeration Compressor Electric Power", "Refrigeration Compressor Electricity Rate"},
      {"Refrigeration Compressor Electric Energy", "Refrigeration Compressor Electricity Energy"},
      {"Refrigeration Transcritical System Gas Cooler Fan Electric Power", "Refrigeration Transcritical System Gas Cooler Fan Electricity Rate"},
      {"Refrigeration Transcritical System Gas Cooler Fan Electric Energy", "Refrigeration Transcritical System Gas Cooler Fan Electricity Energy"},
      {"Air System Humidifier Electric Energy", "Air System Humidifier Electricity Energy"},
      {"Zone Unit Heater Fan Electric Power", "Zone Unit Heater Fan Electricity Rate"},
      {"Zone Unit Heater Fan Electric Energy", "Zone Unit Heater Fan Electricity Energy"},
      {"Zone Unit Ventilator Fan Electric Power", "Zone Unit Ventilator Fan Electricity Rate"},
      {"Zone Unit Ventilator Fan Electric Energy", "Zone Unit Ventilator Fan Electricity Energy"},
      {"Heating Coil Defrost Electric Energy", "Heating Coil Defrost Electricity Energy"},
      {"Cooling Coil Water Heating Pump Electric Power", "Cooling Coil Water Heating Pump Electricity Rate"},
      {"Cooling Coil Water Heating Pump Electric Energy", "Cooling Coil Water Heating Pump Electricity Energy"},
      {"Zone Ventilated Slab Fan Electric Power", "Zone Ventilated Slab Fan Electricity Rate"},
      {"Zone Ventilated Slab Fan Electric Energy", "one Ventilated Slab Fan Electricity Energy"},
      {"Water System Groundwater Well Pump Electric Power", "Water System Groundwater Well Pump Electricity Rate"},
      {"Water System Groundwater Well Pump Electric Energy", "Water System Groundwater Well Pump Electricity Energy"},
      {"Water Heater Electric Power", "Water Heater Electricity Rate"},
      {"Water Heater Electric Energy", "Water Heater Electricity Energy"},
      {"Water Heater Off Cycle Parasitic Electric Power", "Water Heater Off Cycle Parasitic Electricity Rate"},
      {"Water Heater Off Cycle Parasitic Electric Energy", "Water Heater Off Cycle Parasitic Electricity Energy"},
      {"Water Heater On Cycle Parasitic Electric Power", "Water Heater On Cycle Parasitic Electricity Rate"},
      {"Water Heater On Cycle Parasitic Electric Energy", "Water Heater On Cycle Parasitic Electricity Energy"},
      {"Water Heater Off Cycle Ancillary Electric Power", "Water Heater Off Cycle Ancillary Electricity Rate"},
      {"Water Heater Off Cycle Ancillary Electric Energy", "Water Heater Off Cycle Ancillary Electricity Energy"},
      {"Water Heater On Cycle Ancillary Electric Power", "Water Heater On Cycle Ancillary Electricity Rate"},
      {"Water Heater On Cycle Ancillary Electric Energy", "Water Heater On Cycle Ancillary Electricity Energy"},
      {"Water Heater Gas Rate", "Water Heater NaturalGas Rate"},
      {"Water Heater Gas Energy", "Water Heater NaturalGas Energy"},
      {"Water Heater Off Cycle Parasitic Gas Rate", "Water Heater Off Cycle Parasitic NaturalGas Rate"},
      {"Water Heater Off Cycle Parasitic Gas Energy", "Water Heater Off Cycle Parasitic NaturalGas Energy"},
      {"Water Heater On Cycle Parasitic Gas Rate", "Water Heater On Cycle Parasitic NaturalGas Rate"},
      {"Water Heater On Cycle Parasitic Gas Energy", "Water Heater On Cycle Parasitic NaturalGas Energy"},
      {"Water Heater Off Cycle Ancillary Gas Rate", "Water Heater Off Cycle Ancillary NaturalGas Rate"},
      {"Water Heater Off Cycle Ancillary Gas Energy", "Water Heater Off Cycle Ancillary NaturalGas Energy"},
      {"Water Heater On Cycle Ancillary Gas Rate", "Water Heater On Cycle Ancillary NaturalGas Rate"},
      {"Water Heater On Cycle Ancillary Gas Energy", "Water Heater On Cycle Ancillary NaturalGas Energy"},
      {"Water Heater FuelOil#1 Rate", "Water Heater FuelOilNo1 Rate"},
      {"Water Heater FuelOil#1 Energy", "Water Heater FuelOilNo1 Energy"},
      {"Water Heater Off Cycle Parasitic FuelOil#1 Rate", "Water Heater Off Cycle Parasitic FuelOilNo1 Rate"},
      {"Water Heater Off Cycle Parasitic FuelOil#1 Energy", "Water Heater Off Cycle Parasitic FuelOilNo1 Energy"},
      {"Water Heater On Cycle Parasitic FuelOil#1 Rate", "Water Heater On Cycle Parasitic FuelOilNo1 Rate"},
      {"Water Heater On Cycle Parasitic FuelOil#1 Energy", "Water Heater On Cycle Parasitic FuelOilNo1 Energy"},
      {"Water Heater Off Cycle Ancillary FuelOil#1 Rate", "Water Heater Off Cycle Ancillary FuelOilNo1 Rate"},
      {"Water Heater Off Cycle Ancillary FuelOil#1 Energy", "Water Heater Off Cycle Ancillary FuelOilNo1 Energy"},
      {"Water Heater On Cycle Ancillary FuelOil#1 Rate", "Water Heater On Cycle Ancillary FuelOilNo1 Rate"},
      {"Water Heater On Cycle Ancillary FuelOil#1 Energy", "Water Heater On Cycle Ancillary FuelOilNo1 Energy"},
      {"Water Heater FuelOil#2 Rate", "Water Heater FuelOilNo2 Rate"},
      {"Water Heater FuelOil#2 Energy", "Water Heater FuelOilNo2 Energy"},
      {"Water Heater Off Cycle Parasitic FuelOil#2 Rate", "Water Heater Off Cycle Parasitic FuelOilNo2 Rate"},
      {"Water Heater Off Cycle Parasitic FuelOil#2 Energy", "Water Heater Off Cycle Parasitic FuelOilNo2 Energy"},
      {"Water Heater On Cycle Parasitic FuelOil#2 Rate", "Water Heater On Cycle Parasitic FuelOilNo2 Rate"},
      {"Water Heater On Cycle Parasitic FuelOil#2 Energy", "Water Heater On Cycle Parasitic FuelOilNo2 Energy"},
      {"Water Heater Off Cycle Ancillary FuelOil#2 Rate", "Water Heater Off Cycle Ancillary FuelOilNo2 Rate"},
      {"Water Heater Off Cycle Ancillary FuelOil#2 Energy", "Water Heater Off Cycle Ancillary FuelOilNo2 Energy"},
      {"Water Heater On Cycle Ancillary FuelOil#2 Rate", "Water Heater On Cycle Ancillary FuelOilNo2 Rate"},
      {"Water Heater On Cycle Ancillary FuelOil#2 Energy", "Water Heater On Cycle Ancillary FuelOilNo2 Energy"},
      {"Water Heater Pump Electric Power", "Water Heater Pump NaturalGas Rate"},
      {"Water Heater Pump Electric Energy", "Water Heater Pump NaturalGas Energy"},
      {"Zone Dehumidifier Electric Power", "Zone Dehumidifier Electricity Rate"},
      {"Zone Dehumidifier Electric Energy", "Zone Dehumidifier Electricity Energy"},
      {"Zone Dehumidifier Off Cycle Parasitic Electric Power", "Zone Dehumidifier Off Cycle Parasitic Electricity Rate"},
      {"Zone Dehumidifier Off Cycle Parasitic Electric Energy", "Zone Dehumidifier Off Cycle Parasitic Electricity Energy"},
      {"Chiller Gas Rate", "Chiller NaturalGas Rate"},
      {"Chiller Gas Energy", "Chiller NaturalGas Energy"},
      {"Chiller Gas Mass Flow Rate", "Chiller NaturalGas Mass Flow Rate"},
      {"Boiler Gas Rate", "Boiler NaturalGas Rate"},
      {"Boiler Gas Energy", "Boiler NaturalGas Energy"},
      {"Cooling Coil Gas Rate", "Cooling Coil NaturalGas Rate"},
      {"Cooling Coil Gas Energy", "Cooling Coil NaturalGas Energy"},
      {"Generator Gas Rate", "Generator NaturalGas Rate"},
      {"Generator Gas Energy", "Generator NaturalGas Energy"},
      {"Generator Gas Mass Flow Rate", "Generator NaturalGas Mass Flow Rate"},
      {"Generator Gas HHV Basis Rate", "Generator NaturalGas HHV Basis Rate"},
      {"Generator Gas HHV Basis Energy", "Generator NaturalGas HHV Basis Energy"},
      {"Chiller Heater Gas Rate", "Chiller Heater NaturalGas Rate"},
      {"Chiller Heater Gas Energy", "Chiller Heater NaturalGas Energy"},
      {"Chiller Heater FuelOil#1 Rate", "Chiller Heater FuelOilNo1 Rate"},
      {"Chiller Heater FuelOil#1 Energy", "Chiller Heater FuelOilNo1 Energy"},
      {"Chiller Heater FuelOil#2 Rate", "Chiller Heater FuelOilNo2 Rate"},
      {"Chiller Heater FuelOil#2 Energy", "Chiller Heater FuelOilNo2 Energy"},
      {"Heating Coil FuelOil#1 Energy", "Heating Coil FuelOilNo1 Energy"},
      {"Heating Coil FuelOil#2 Rate", "Heating Coil FuelOilNo2 Rate"},
      {"Heating Coil FuelOil#1 Energy", "Heating Coil FuelOilNo1 Energy"},
      {"Heating Coil FuelOil#2 Rate", "Heating Coil FuelOilNo2 Rate"},

      // EMS ACTUATORS
      {"Electric Power Level", "Electricity Rate"},
      {"Gas Power Level", "NaturalGas Rate"},
    });

    /*****************************************************************************************************************************************************
     *                                                          Output:Meter fuel types renames                                                          *
     *****************************************************************************************************************************************************/

    const static std::map<std::string, std::string> meterFuelTypesMap({
      {"FuelOil_1", "FuelOilNo1"},
      {"FuelOil_2", "FuelOilNo2"},
      {"Gas", "NaturalGas"},
    });

    /*****************************************************************************************************************************************************
       *                                                        Shading Control Refactor: pre-scan                                                         *
       *****************************************************************************************************************************************************/

    std::map<std::string, std::string> shadingControlToSurfaceMap;
    std::vector<IdfObject> subSurfaces = idf_3_0_1.getObjectsByType(idf_3_0_1.iddFile().getObject("OS:SubSurface").get());
    for (auto& subSurface : subSurfaces) {
      value = subSurface.getString(7, false, true);  // Shading Control Name
      if (value) {
        shadingControlToSurfaceMap[value.get()] = subSurface.getString(0).get();
      }
    }

    for (IdfObject& object : idf_3_0_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AvailabilityManager:HybridVentilation") {
        // Added 2 new fields at end only, and made them required-field by setting their default in the Ctor, so VT needed

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Set new fields per IDD default, same as Model Ctor, since it was made required-field
        // Minimum HVAC Operation Time
        newObject.setDouble(17, 0.0);
        // Minimum Ventilation Time
        newObject.setDouble(18, 0.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:AirLoopHVAC") {

        // Design Return Air Flow Fraction of Supply Air Flow, inserted at position 6 (0-indexed)

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Set new field per IDD default, same as Model Ctor, since it was made required-field
        newObject.setDouble(6, 1.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Construction:InternalSource") {

        // Two-Dimensional Temperature Calculation Position, inserted at position 6 (0-indexed)
        // Object has extensible groups too

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // If we made it required-field, set new field per IDD default, same as Model Ctor
        // newObject.setDouble(6, 0.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:LowTemperatureRadiant:Electric") {

        // Inserted a field 'Sepoint Control Type' with a default at position 6 (0-indexed)
        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:WaterHeater:HeatPump") {

        // Inserted a field 'Maximum Inlet Air Temperature for Compressor Operation' at position 16 (0-indexed)
        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 16) {
              newObject.setString(i, value.get());
            } else {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Made it a required-field with the E+ IDD default value set in Ctor, so set it here too
        newObject.setDouble(16, 48.89);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:LowTemperatureRadiant:ConstantFlow") {

        // Insertion points (0-indexed)
        // * Fluid to Radiant Surface Heat Transfer Model: 4
        // * Hydronic Tubing Outside Diameter: 6
        // * Hydronic Tubing Conductivity: 8
        // * Running Mean Outdoor Dry-Bulb Temperature Weighting Factor: 10
        // * Changeover Delay Time Period Schedule: 21 : last field, optional

        // Mapping:
        // * Hydronic Tubing Inside Diameter - 4 => 5
        // * Hydronic Tubing Length - 5 => 7
        // * Temperature Control Type - 6 => 9
        // * Low Temp Radiant Constant Flow Heating Coil Name - 7 => 11
        // * Low Temp Radiant Constant Flow Cooling Coil Name - 8 => 12
        // * Rated Flow Rate - 9 => 13
        // * Pump Flow Rate Schedule Name - 10 => 14
        // * Rated Pump Head - 11 => 15
        // * Rated Power Consumption - 12 => 16
        // * Motor Efficiency - 13 => 17
        // * Fraction of Motor Inefficiencies to Fluid Stream - 14 => 18
        // * Number of Circuits - 15 => 19
        // * Circuit Length - 16 => 20

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 4) {
              newObject.setString(i, value.get());
            } else if (i < 5) {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            } else if (i < 6) {
              newObject.setString(i + 2, value.get());
            } else if (i < 7) {
              newObject.setString(i + 3, value.get());
            } else {
              newObject.setString(i + 4, value.get());
            }
          }
        }

        // If we wanted to make them required-field, these are the defaults
        // newObject.setString(4, "ConvectionOnly");
        // newObject.setDouble(6, 0.016);
        // newObject.setDouble(8, 0.35);
        // newObject.setDouble(10, 0.8);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow") {

        // Insertion points (0-indexed)
        // * Fluid to Radiant Surface Heat Transfer Model * 6
        // * Hydronic Tubing Outside Diameter * 8
        // * Hydronic Tubing Conductivity * 10
        // * Setpoint Control Type * 12
        // * Changeover Delay Time Period Schedule * 15

        // Mapping:
        // * Hydronic Tubing Inside Diameter - 6 => 7
        // * Hydronic Tubing Length - 7 => 9
        // * Temperature Control Type - 8 => 11
        // * Number of Circuits - 9 => 13
        // * Circuit Length - 10 => 14

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else if (i < 7) {
              // Shifted by one field
              newObject.setString(i + 1, value.get());
            } else if (i < 8) {
              newObject.setString(i + 2, value.get());
            } else if (i < 9) {
              newObject.setString(i + 3, value.get());
            } else {
              newObject.setString(i + 4, value.get());
            }
          }
        }

        // If we wanted to make them required-field, these are the defaults
        // newObject.setString(6, "ConvectionOnly");
        // newObject.setDouble(8, 0.016);
        // newObject.setDouble(10, 0.35);
        // newObject.setString(23, "HalfFlowPower");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Output:Meter") {

        std::string name = object.nameString();

        // Structured bindings
        for (const auto& [k, v] : meterFuelTypesMap) {
          name = boost::regex_replace(name, boost::regex(k, boost::regex::icase), v);
        }
        if (name == object.nameString()) {
          // No-op
          ss << object;
        } else {

          // Copy everything but 'Variable Name' field
          auto iddObject = idd_3_1_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < object.numFields(); ++i) {
            // Skip name field
            if (i == 1) {
              continue;
            }
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }

          newObject.setName(name);

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        }

      } else if ((iddname == "OS:Output:Variable") || (iddname == "OS:EnergyManagementSystem:Sensor")
                 || (iddname == "OS:EnergyManagementSystem:Actuator")) {

        unsigned variableNameIndex = 3;
        // Note: I forgot to add the EMSActuator case in 3.1.0, it was added after the release
        if (iddname == "OS:EnergyManagementSystem:Actuator") {
          variableNameIndex = 4;  // Actuated Component Control Type
        }

        if ((value = object.getString(variableNameIndex))) {

          std::string variableName = value.get();
          // Strip consecutive spaces and all
          variableName = boost::regex_replace(variableName, re_strip_multiple_spaces, " ");

          auto it = replaceOutputVariablesMap.find(variableName);
          if (it == replaceOutputVariablesMap.end()) {
            // No-op
            ss << object;
          } else {

            // Copy everything but 'Variable Name' field
            auto iddObject = idd_3_1_0.getObject(iddname);
            IdfObject newObject(iddObject.get());

            for (size_t i = 0; i < object.numFields(); ++i) {
              if (i == variableNameIndex) {
                continue;
              } else if ((value = object.getString(i))) {
                newObject.setString(i, value.get());
              }
            }

            LOG(Trace, "Replacing " << variableName << " with " << it->second << " for " << object.nameString());
            newObject.setString(variableNameIndex, it->second);

            ss << newObject;
            m_refactored.emplace_back(std::move(object), std::move(newObject));
          }
        } else {
          // No-op
          ss << object;
        }

      } else if ((iddname == "OS:Meter:Custom") || (iddname == "OS:Meter:CustomDecrement")) {

        bool isReplaceNeeded = false;

        // First pass scan to see if any of the extensible "Output Variable or Meter Name" need replacing
        for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
          if ((value = eg.getString(1))) {

            std::string variableName = value.get();
            // Strip consecutive spaces and all
            variableName = boost::regex_replace(variableName, re_strip_multiple_spaces, " ");

            auto it = replaceOutputVariablesMap.find(variableName);
            if (it != replaceOutputVariablesMap.end()) {
              isReplaceNeeded = true;
              break;
            }
          }
        }
        if (!isReplaceNeeded) {
          // No-op
          ss << object;
        } else {

          // Copy everything but 'Variable Name' field
          auto iddObject = idd_3_1_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          // Copy non extensible fields in place
          for (size_t i = 0; i < object.numNonextensibleFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }

          // Now deal with the extensibles
          for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
            IdfExtensibleGroup new_eg = newObject.pushExtensibleGroup();
            // Copy Key Name as-is
            if (value == eg.getString(0)) {
              new_eg.setString(0, value.get());
            }
            if ((value = eg.getString(1))) {

              std::string variableName = value.get();
              // Strip consecutive spaces and all
              variableName = boost::regex_replace(variableName, re_strip_multiple_spaces, " ");
              auto it = replaceOutputVariablesMap.find(variableName);
              if (it != replaceOutputVariablesMap.end()) {
                new_eg.setString(1, it->second);
              } else {
                new_eg.setString(1, value.get());
              }
            }
          }

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        }

        // Note: Would have needed to do UtilityCost:Tariff for Fuel Type renames too, but not wrapped

      } else if (iddname == "OS:ShadingControl") {
        // get all subsurfaces
        // loop thru subsurfaces
        // check if shading control name equals this handle
        // add extensible field with subsurface handle
        // remove shading control name field from subsurface
        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // New field at end, before extensible, and I moved to E+ default to the Ctor and made it required, so need to set it here
        newObject.setString(13, "Sequential");

        // Add the SubSurface to the list if any
        auto subSurfaceHandleIt = shadingControlToSurfaceMap.find(object.getString(0).get());
        if (subSurfaceHandleIt != shadingControlToSurfaceMap.end()) {
          newObject.pushExtensibleGroup(StringVector(1u, subSurfaceHandleIt->second));
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:SubSurface") {

        auto iddObject = idd_3_1_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else if (i == 7) {  // Shading Control Name
              // no-op
            } else {
              newObject.setString(i - 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_0_1_to_3_1_0

  std::string VersionTranslator::update_3_1_0_to_3_2_0(const IdfFile& idf_3_1_0, const IddFileAndFactoryWrapper& idd_3_2_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_1_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_2_0.iddFile());
    ss << targetIdf.versionObject().get();

    auto makeCurveQuadLinear = [&idd_3_2_0]() -> IdfObject {
      auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();

      IdfObject curveQuadLinear(quadLinearIddObject);

      int i = 7;
      curveQuadLinear.setDouble(i++, -100.0);
      curveQuadLinear.setDouble(i++, 100.0);
      curveQuadLinear.setDouble(i++, -100.0);
      curveQuadLinear.setDouble(i++, 100.0);
      curveQuadLinear.setDouble(i++, 0.0);
      curveQuadLinear.setDouble(i++, 100.0);
      curveQuadLinear.setDouble(i++, 0.0);
      curveQuadLinear.setDouble(i++, 100.0);

      return curveQuadLinear;
    };

    auto makeCurveQuintLinear = [&idd_3_2_0]() -> IdfObject {
      auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuintLinear").get();

      IdfObject curveQuintLinear(quadLinearIddObject);

      int i = 8;
      curveQuintLinear.setDouble(i++, -100.0);
      curveQuintLinear.setDouble(i++, 100.0);
      curveQuintLinear.setDouble(i++, -100.0);
      curveQuintLinear.setDouble(i++, 100.0);
      curveQuintLinear.setDouble(i++, -100.0);
      curveQuintLinear.setDouble(i++, 100.0);
      curveQuintLinear.setDouble(i++, 0.0);
      curveQuintLinear.setDouble(i++, 100.0);
      curveQuintLinear.setDouble(i++, 0.0);
      curveQuintLinear.setDouble(i++, 100.0);

      return curveQuintLinear;
    };

    for (IdfObject& object : idf_3_1_0.objects()) {
      auto iddname = object.iddObject().name();

      if ((iddname == "OS:Coil:Heating:LowTemperatureRadiant:VariableFlow") || (iddname == "OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow")) {

        // Inserted 4 fields at position 2 (0-indexed)
        // * Heating Design Capacity Method = 2
        // * Heating Design Capacity = 3
        // * Heating Design Capacity Per Floor Area = 4
        // * Fraction of Autosized Heating Design Capacity = 5

        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 2) {
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by four fields
              newObject.setString(i + 4, value.get());
            }
          }
        }

        // Set new fields per IDD default, same as Model Ctor
        if (iddname == "OS:Coil:Heating:LowTemperatureRadiant:VariableFlow") {
          newObject.setString(2, "HeatingDesignCapacity");
        } else {
          newObject.setString(2, "CoolingDesignCapacity");
        }

        newObject.setString(3, "Autosize");
        newObject.setDouble(4, 0.0);
        newObject.setDouble(5, 1.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if ((iddname == "OS:Connection") || (iddname == "OS:PortList")) {
        // Deleted the 'Name' field
        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());
        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 1) {
              newObject.setString(i, value.get());
            } else if (i == 1) {
              // Deleted the name: no-op
            } else {
              newObject.setString(i - 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Construction:AirBoundary") {

        // Removed 2 fields at positions 2 and 3 (0-indexed)
        // * Solar and Daylighting Method = 2
        // * Radiant Exchange Method = 3

        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 2) {
              newObject.setString(i, value.get());
            } else if (i == 2 || i == 3) {
              // No-op
            } else {
              newObject.setString(i - 2, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneAirMassFlowConservation") {

        // Field 1 (0-index) 'Yes' becomes 'AdjustMixingOnly' and 'No' becomes 'None'

        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i == 1) {
              std::string cur_choice = value.get();
              if (openstudio::istringEqual("Yes", cur_choice)) {
                newObject.setString(i, "AdjustMixingOnly");
              } else if (openstudio::istringEqual("No", cur_choice)) {
                newObject.setString(i, "None");
              }
            } else {
              newObject.setString(i, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow") {

        // Field 13 (0-index) 'Supply Air Fan Placement' is now defaulted inside the Ctor

        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(13, "DrawThrough");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Coil:Cooling:WaterToAirHeatPump:EquationFit") {
        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Fields that have been removed from 3.1.0 to 3.2.0:
        // --------------------------------------------------
        // * Total Cooling Capacity Coefficient 1 * 11
        // * Total Cooling Capacity Coefficient 2 * 12
        // * Total Cooling Capacity Coefficient 3 * 13
        // * Total Cooling Capacity Coefficient 4 * 14
        // * Total Cooling Capacity Coefficient 5 * 15
        // * Sensible Cooling Capacity Coefficient 1 * 16
        // * Sensible Cooling Capacity Coefficient 2 * 17
        // * Sensible Cooling Capacity Coefficient 3 * 18
        // * Sensible Cooling Capacity Coefficient 4 * 19
        // * Sensible Cooling Capacity Coefficient 5 * 20
        // * Sensible Cooling Capacity Coefficient 6 * 21
        // * Cooling Power Consumption Coefficient 1 * 22
        // * Cooling Power Consumption Coefficient 2 * 23
        // * Cooling Power Consumption Coefficient 3 * 24
        // * Cooling Power Consumption Coefficient 4 * 25
        // * Cooling Power Consumption Coefficient 5 * 26
        //
        // Fields that have been added from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Total Cooling Capacity Curve Name * 11
        // * Sensible Cooling Capacity Curve Name * 12
        // * Cooling Power Consumption Curve Name * 13
        //
        // Fields with changed indices from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Nominal Time for Condensate Removal to Begin - 27 => 14
        // * Ratio of Initial Moisture Evaporation Rate and Steady State Latent Capacity - 28 => 15

        auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();
        auto quintLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();

        IdfObject totalCoolingCapacityCurve = makeCurveQuadLinear();
        totalCoolingCapacityCurve.setName(object.nameString() + " TotCoolCapCurve");

        IdfObject sensibleCoolingCapacityCurve = makeCurveQuintLinear();
        sensibleCoolingCapacityCurve.setName(object.nameString() + " SensCoolCapCurve");

        IdfObject coolingPowerConsumptionCurve = makeCurveQuadLinear();
        coolingPowerConsumptionCurve.setName(object.nameString() + " CoolPowCurve");

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 11) {
              newObject.setString(i, value.get());
            } else if (i < 16) {
              totalCoolingCapacityCurve.setString(i - 11 + 2, value.get());
            } else if (i < 22) {
              sensibleCoolingCapacityCurve.setString(i - 16 + 2, value.get());
            } else if (i < 27) {
              coolingPowerConsumptionCurve.setString(i - 22 + 2, value.get());
            } else {
              newObject.setString(i - 13, value.get());
            }
          }
        }

        newObject.setString(11, totalCoolingCapacityCurve.nameString());
        newObject.setString(12, sensibleCoolingCapacityCurve.nameString());
        newObject.setString(13, coolingPowerConsumptionCurve.nameString());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Register new Curve objects
        ss << totalCoolingCapacityCurve;
        ss << sensibleCoolingCapacityCurve;
        ss << coolingPowerConsumptionCurve;
        m_new.emplace_back(std::move(totalCoolingCapacityCurve));
        m_new.emplace_back(std::move(sensibleCoolingCapacityCurve));
        m_new.emplace_back(std::move(coolingPowerConsumptionCurve));

      } else if (iddname == "OS:Coil:Heating:WaterToAirHeatPump:EquationFit") {
        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Fields that have been removed from 3.1.0 to 3.2.0:
        // --------------------------------------------------
        // * Heating Capacity Coefficient 1 * 10
        // * Heating Capacity Coefficient 2 * 11
        // * Heating Capacity Coefficient 3 * 12
        // * Heating Capacity Coefficient 4 * 13
        // * Heating Capacity Coefficient 5 * 14
        // * Heating Power Consumption Coefficient 1 * 15
        // * Heating Power Consumption Coefficient 2 * 16
        // * Heating Power Consumption Coefficient 3 * 17
        // * Heating Power Consumption Coefficient 4 * 18
        // * Heating Power Consumption Coefficient 5 * 19
        //
        // Fields that have been added from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Heating Capacity Curve Name * 10
        // * Heating Power Consumption Curve Name * 11

        auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();

        IdfObject heatingCapacityCurve = makeCurveQuadLinear();
        heatingCapacityCurve.setName(object.nameString() + " HeatCapCurve");

        IdfObject heatingPowerConsumptionCurve = makeCurveQuadLinear();
        heatingPowerConsumptionCurve.setName(object.nameString() + " HeatPowCurve");

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 10) {
              newObject.setString(i, value.get());
            } else if (i < 15) {
              heatingCapacityCurve.setString(i - 10 + 2, value.get());
            } else {
              heatingPowerConsumptionCurve.setString(i - 15 + 2, value.get());
            }
          }
        }

        newObject.setString(10, heatingCapacityCurve.nameString());
        newObject.setString(11, heatingPowerConsumptionCurve.nameString());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Register new Curve objects
        ss << heatingCapacityCurve;
        ss << heatingPowerConsumptionCurve;
        m_new.emplace_back(std::move(heatingCapacityCurve));
        m_new.emplace_back(std::move(heatingPowerConsumptionCurve));

      } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Cooling") {
        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Fields that have been removed from 3.1.0 to 3.2.0:
        // --------------------------------------------------
        // * Cooling Capacity Coefficient 1 * 10
        // * Cooling Capacity Coefficient 2 * 11
        // * Cooling Capacity Coefficient 3 * 12
        // * Cooling Capacity Coefficient 4 * 13
        // * Cooling Capacity Coefficient 5 * 14
        // * Cooling Compressor Power Coefficient 1 * 15
        // * Cooling Compressor Power Coefficient 2 * 16
        // * Cooling Compressor Power Coefficient 3 * 17
        // * Cooling Compressor Power Coefficient 4 * 18
        // * Cooling Compressor Power Coefficient 5 * 19
        //
        // Fields that have been added from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Cooling Capacity Curve Name * 10
        // * Cooling Compressor Power Curve Name * 11
        //
        // Fields with changed indices from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Reference Coefficient of Performance - 20 => 12
        // * Sizing Factor - 21 => 13
        // * Companion Heating Heat Pump Name - 22 => 14

        auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();

        IdfObject coolingCapacityCurve = makeCurveQuadLinear();
        coolingCapacityCurve.setName(object.nameString() + " CoolCapCurve");

        IdfObject coolingCompressorPowerCurve = makeCurveQuadLinear();
        coolingCompressorPowerCurve.setName(object.nameString() + " CoolCompPowerCurve");

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 10) {
              newObject.setString(i, value.get());
            } else if (i < 15) {
              coolingCapacityCurve.setString(i - 10 + 2, value.get());
            } else if (i < 20) {
              coolingCompressorPowerCurve.setString(i - 15 + 2, value.get());
            } else {
              newObject.setString(i - 8, value.get());
            }
          }
        }

        newObject.setString(10, coolingCapacityCurve.nameString());
        newObject.setString(11, coolingCompressorPowerCurve.nameString());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Register new Curve objects
        ss << coolingCapacityCurve;
        ss << coolingCompressorPowerCurve;
        m_new.emplace_back(std::move(coolingCapacityCurve));
        m_new.emplace_back(std::move(coolingCompressorPowerCurve));

      } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Heating") {
        auto iddObject = idd_3_2_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Fields that have been removed from 3.1.0 to 3.2.0:
        // --------------------------------------------------
        // * Heating Capacity Coefficient 1 * 10
        // * Heating Capacity Coefficient 2 * 11
        // * Heating Capacity Coefficient 3 * 12
        // * Heating Capacity Coefficient 4 * 13
        // * Heating Capacity Coefficient 5 * 14
        // * Heating Compressor Power Coefficient 1 * 15
        // * Heating Compressor Power Coefficient 2 * 16
        // * Heating Compressor Power Coefficient 3 * 17
        // * Heating Compressor Power Coefficient 4 * 18
        // * Heating Compressor Power Coefficient 5 * 19
        //
        // Fields that have been added from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Heating Capacity Curve Name * 10
        // * Heating Compressor Power Curve Name * 11
        //
        // Fields with changed indices from 3.1.0 to 3.2.0:
        // ------------------------------------------------
        // * Reference Coefficient of Performance - 20 => 12
        // * Sizing Factor - 21 => 13
        // * Companion Cooling Heat Pump Name - 22 => 14

        auto quadLinearIddObject = idd_3_2_0.getObject("OS:Curve:QuadLinear").get();

        IdfObject heatingCapacityCurve = makeCurveQuadLinear();
        heatingCapacityCurve.setName(object.nameString() + " HeatCapCurve");

        IdfObject heatingCompressorPowerCurve = makeCurveQuadLinear();
        heatingCompressorPowerCurve.setName(object.nameString() + " HeatCompPowerCurve");

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 10) {
              newObject.setString(i, value.get());
            } else if (i < 15) {
              heatingCapacityCurve.setString(i - 10 + 2, value.get());
            } else if (i < 20) {
              heatingCompressorPowerCurve.setString(i - 15 + 2, value.get());
            } else {
              newObject.setString(i - 8, value.get());
            }
          }
        }

        newObject.setString(10, heatingCapacityCurve.nameString());
        newObject.setString(11, heatingCompressorPowerCurve.nameString());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // Register new Curve objects
        ss << heatingCapacityCurve;
        ss << heatingCompressorPowerCurve;
        m_new.emplace_back(std::move(heatingCapacityCurve));
        m_new.emplace_back(std::move(heatingCompressorPowerCurve));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_1_0_to_3_2_0

  std::string VersionTranslator::update_3_2_0_to_3_2_1(const IdfFile& idf_3_2_0, const IddFileAndFactoryWrapper& idd_3_2_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_2_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_2_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_3_2_0.objects()) {
      auto iddname = object.iddObject().name();

      if ((iddname == "OS:WaterHeater:Mixed") || (iddname == "OS:WaterHeater:Stratified")) {

        // Object is unchanged
        ss << object;

        // But we also add a WaterHeater:Sizing object
        auto iddObject = idd_3_2_1.getObject("OS:WaterHeater:Sizing");
        IdfObject newObject(iddObject.get());

        // WaterHeaterName
        newObject.setString(1, toString(object.handle()));
        // Design Mode
        newObject.setString(2, "PeakDraw");
        // Time Storage Can Meet Peak Draw {hr}
        newObject.setDouble(3, 0.538503);
        // Time for Tank Recovery {hr}
        newObject.setDouble(4, 0.0);
        // Nominal Tank Volume for Autosizing Plant Connections {m3}
        newObject.setDouble(5, 1.0);

        // Register new WaterHeater:Sizing objects
        ss << newObject;
        m_new.emplace_back(std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_2_0_to_3_2_1

  std::string VersionTranslator::update_3_2_1_to_3_3_0(const IdfFile& idf_3_2_1, const IddFileAndFactoryWrapper& idd_3_3_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_2_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_3_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_3_2_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AirTerminal:SingleDuct:InletSideMixer") {

        // Fields that have been added from 3.2.1 to 3.3.0:
        // ------------------------------------------------
        // * Control For Outdoor Air * 5
        // * Per Person Ventilation Rate Mode * 6

        auto iddObject = idd_3_3_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Set new fields per IDD default, same as Model Ctor, since it was made required-field
        newObject.setString(5, "Yes");
        newObject.setString(6, "CurrentOccupancy");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:GroundHeatExchanger:Vertical") {

        // Removed 1 fields at position 11 (0-indexed)
        // * Design Flow Rate  = 11
        // I moved it to the field 4, previously maximumFlowRate. I'm discarding the previous Design Flow Rate version as that's what the comments in
        // GroundHeatExchangerVertical.hpp were saying (maximumFlowRate was used instead of designFlowRate which was unused...)

        auto iddObject = idd_3_3_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 11) {
              newObject.setString(i, value.get());
            } else if (i == 11) {
              // No-op
            } else {
              newObject.setString(i - 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if ((iddname == "OS:Controller:MechanicalVentilation") || (iddname == "OS:Sizing:System")) {

        // OS:Controller:MechanicalVentilation, Field 4: VentilationRateProcedure -> Standard62.1VentilationRateProcedure
        // OS:Sizing:System, Field 20: VentilationRateProcedure -> Standard62.1VentilationRateProcedure
        unsigned int changedIndex = 4;
        if (iddname == "OS:Sizing:System") {
          changedIndex = 20;
        }
        value = object.getString(changedIndex, false, true);
        if (value && openstudio::istringEqual(value.get(), "VentilationRateProcedure")) {

          auto iddObject = idd_3_3_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              if (i == changedIndex) {
                // System Outdoor Air Method
                newObject.setString(i, "Standard62.1VentilationRateProcedure");
              } else {
                newObject.setString(i, value.get());
              }
            }
          }

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));

        } else {
          // Nothing to do since there's no rename to perform
          ss << object;
        }

      } else if (iddname == "OS:SizingPeriod:DesignDay") {

        auto iddObject = idd_3_3_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        std::string humidityIndicatingType;

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 4) {
              newObject.setString(i, value.get());
            } else if (i == 4) {
              humidityIndicatingType = object.getString(15).get();  // Humidity Indicating Type
              if (istringEqual(humidityIndicatingType, "WetBulb") || istringEqual(humidityIndicatingType, "DewPoint")
                  || istringEqual(humidityIndicatingType, "WetBulbProfileMultiplierSchedule")
                  || istringEqual(humidityIndicatingType, "WetBulbProfileDifferenceSchedule")
                  || istringEqual(humidityIndicatingType, "WetBulbProfileDefaultMultipliers")) {
                newObject.setString(16, value.get());  // Wetbulb or DewPoint at Maximum Dry-Bulb
              } else if (istringEqual(humidityIndicatingType, "HumidityRatio")) {
                newObject.setString(17, value.get());  // Humidity Ratio at Maximum Dry-Bulb
              } else if (istringEqual(humidityIndicatingType, "Enthalpy")) {
                newObject.setString(18, value.get());  // Enthalpy at Maximum Dry-Bulb
              }
            } else if (i == 9 || i == 10 || i == 14) {
              if (value.get() == "0") {
                newObject.setString(i - 1, "No");
              } else if (value.get() == "1") {
                newObject.setString(i - 1, "Yes");
              }
            } else if (i < 15) {
              newObject.setString(i - 1, value.get());
            } else if (i == 15) {
              newObject.setString(i - 1, humidityIndicatingType);
            } else if (i == 16) {
              newObject.setString(i - 1, value.get());
            } else if (i < 25) {
              newObject.setString(i + 2, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_2_1_to_3_3_0

  std::string VersionTranslator::update_3_3_0_to_3_4_0(const IdfFile& idf_3_3_0, const IddFileAndFactoryWrapper& idd_3_4_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_3_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_4_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (IdfObject& object : idf_3_3_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Coil:Heating:DX:MultiSpeed") {

        // Stage Data List becomes extensible list (Stage 1, Stage 2, etc.)
        // ModelObjectList gets removed

        auto iddObject = idd_3_4_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < 18; ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Before: There was a single modelObjectList, which extensible groups were the StageDatas
        // Now: We move these directly onto extensible groups
        std::string stageDataList = object.getString(18).get();  // Stage Data List: handle of the ModelObjectList
        if (boost::optional<IdfObject> modelObjectList = idf_3_3_0.getObject(openstudio::toUUID(stageDataList))) {
          m_untranslated.push_back(modelObjectList.get());  // original OS:ModelObjectList
          for (const IdfExtensibleGroup& eg : modelObjectList->extensibleGroups()) {
            std::string stageDataListHandleStr = eg.getString(0).get();
            if (!stageDataListHandleStr.empty()) {
              IdfExtensibleGroup new_eg = newObject.pushExtensibleGroup({stageDataListHandleStr});  // new OS:Coil:Heating:DX:MultiSpeed:StageData
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));
      } else if (iddname == "OS:Coil:Heating:DX:MultiSpeed:StageData") {
        auto iddObject = idd_3_4_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Inserted name at pos 1
        newObject.setString(1, "Coil Heating DX Multi Speed Stage Data");

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 1) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ModelObjectList") {

        bool isOnCoil = false;
        std::vector<IdfObject> coils = idf_3_3_0.getObjectsByType(idf_3_3_0.iddFile().getObject("OS:Coil:Heating:DX:MultiSpeed").get());
        for (auto& coil : coils) {
          if (object.getString(0).get() == coil.getString(18).get()) {  // Handle == Stage Data List
            isOnCoil = true;
          }
        }

        if (!isOnCoil) {
          ss << object;
        }

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_3_0_to_3_4_0

  std::string VersionTranslator::update_3_4_0_to_3_5_0(const IdfFile& idf_3_4_0, const IddFileAndFactoryWrapper& idd_3_5_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_4_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_5_0.iddFile());
    ss << targetIdf.versionObject().get();

    // ZoneHVAC:Packaged AirConditionner / HeatPump: prescan
    // In E+ 22.1.0, you could have a Packaged system with a Fan:ConstantVolume and a blank schedule for Supply Air Fan Operating Mode Schedule Name
    // and it would behave like a cycling fan (which it shouldn't have done). This is now disallowed in E+ 22.2.0, and you also cannot set a Schedule
    // with 0 values with a Fan:ConstantVolume. So we must replace these fans with a Fan:SystemModel (that mimics a Fan:OnOff really, just planning
    // for the future + avoid having to create 2 curves for FanOnOff)
    std::vector<std::string> packagedFanCVHandleStrs;
    {
      std::vector<IdfObject> fanCVs = idf_3_4_0.getObjectsByType(idf_3_4_0.iddFile().getObject("OS:Fan:ConstantVolume").get());
      std::vector<std::string> fanCVHandleStrs;
      fanCVHandleStrs.reserve(fanCVs.size());
      packagedFanCVHandleStrs.reserve(fanCVs.size());
      std::transform(fanCVs.cbegin(), fanCVs.cend(), std::back_inserter(fanCVHandleStrs),
                     [](const auto& idfObject) { return idfObject.getString(0).get(); });
      for (const auto& ptac : idf_3_4_0.getObjectsByType(idf_3_4_0.iddFile().getObject("OS:ZoneHVAC:PackagedTerminalAirConditioner").get())) {
        // If the Supply Air Fan Operating Mode Schedule is empty
        if (ptac.isEmpty(17)) {
          if (auto fanHandleStr_ = ptac.getString(13, false, true)) {
            if (std::find(fanCVHandleStrs.cbegin(), fanCVHandleStrs.cend(), fanHandleStr_.get()) != fanCVHandleStrs.cend()) {
              packagedFanCVHandleStrs.push_back(fanHandleStr_.get());
            }
          }
        }
      }
      for (const auto& pthp : idf_3_4_0.getObjectsByType(idf_3_4_0.iddFile().getObject("OS:ZoneHVAC:PackagedTerminalHeatPump").get())) {
        // If the Supply Air Fan Operating Mode Schedule is empty
        if (pthp.isEmpty(23)) {
          if (auto fanName_ = pthp.getString(13, false, true)) {
            if (std::find(fanCVHandleStrs.cbegin(), fanCVHandleStrs.cend(), fanName_.get()) != fanCVHandleStrs.cend()) {
              packagedFanCVHandleStrs.push_back(fanName_.get());
            }
          }
        }
      }
    }

    std::string alwaysOnDiscreteScheduleHandleStr;
    std::string alwaysOffDiscreteScheduleHandleStr;

    auto getOrCreateAlwaysDiscreteScheduleHandleStr = [this, &ss, &idf_3_4_0, &idd_3_5_0, &alwaysOnDiscreteScheduleHandleStr,
                                                       &alwaysOffDiscreteScheduleHandleStr](bool isAlwaysOn) -> std::string {
      auto& discreteSchHandleStr = isAlwaysOn ? alwaysOnDiscreteScheduleHandleStr : alwaysOffDiscreteScheduleHandleStr;
      if (!discreteSchHandleStr.empty()) {
        return discreteSchHandleStr;
      }
      std::string name = isAlwaysOn ? "Always On Discrete" : "Always Off Discrete";
      double val = isAlwaysOn ? 1.0 : 0.0;
      // Add an alwaysOnDiscreteSchedule if one does not already exist
      for (const IdfObject& object : idf_3_4_0.getObjectsByType(idf_3_4_0.iddFile().getObject("OS:Schedule:Constant").get())) {
        if (boost::optional<std::string> name_ = object.getString(1)) {
          if (istringEqual(name_.get(), name)) {
            if (boost::optional<double> value = object.getDouble(3)) {
              if (equal<double>(value.get(), val)) {
                discreteSchHandleStr = object.getString(0).get();
                break;
              }
            }
          }
        }
      }

      if (discreteSchHandleStr.empty()) {
        auto discreteSch = IdfObject(idd_3_5_0.getObject("OS:Schedule:Constant").get());

        discreteSchHandleStr = toString(createUUID());
        discreteSch.setString(0, discreteSchHandleStr);
        discreteSch.setString(1, name);
        discreteSch.setDouble(3, val);

        IdfObject typeLimits(idd_3_5_0.getObject("OS:ScheduleTypeLimits").get());
        typeLimits.setString(0, toString(createUUID()));
        typeLimits.setString(1, name + " Limits");
        typeLimits.setDouble(2, 0.0);
        typeLimits.setDouble(3, 1.0);
        typeLimits.setString(4, "Discrete");
        typeLimits.setString(5, "Availability");

        discreteSch.setString(2, typeLimits.getString(0).get());

        ss << discreteSch;
        ss << typeLimits;

        // Register new objects
        m_new.push_back(discreteSch);
        m_new.push_back(typeLimits);
      }

      return discreteSchHandleStr;
    };

    if (!packagedFanCVHandleStrs.empty()) {
      alwaysOffDiscreteScheduleHandleStr = getOrCreateAlwaysDiscreteScheduleHandleStr(false);
    }  // End locating or creating alwaysOnDiscreteSchedule

    for (const IdfObject& object : idf_3_4_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Construction") {

        // Remove Construction with Material:AirWall layer
        // Replace with Construction:AirBoundary

        auto iddObject = idd_3_5_0.getObject(iddname);

        bool isAirWall = false;
        if (object.numExtensibleGroups() == 1u) {
          const IdfExtensibleGroup eg = object.extensibleGroups()[0];
          if (boost::optional<IdfObject> layer = idf_3_4_0.getObject(toUUID(eg.getString(0).get()))) {
            auto layeriddname = layer->iddObject().name();
            if (layeriddname == "OS:Material:AirWall") {

              auto iddObject = idd_3_5_0.getObject("OS:Construction:AirBoundary");
              IdfObject newObject(iddObject.get());

              // Handle
              if (auto value = object.getString(0)) {
                newObject.setString(0, value.get());
              }

              // Name
              if (auto value = object.getString(1)) {
                newObject.setString(1, value.get());
              }

              // Surface Rendering Name (Rendering Color)
              if (auto value = object.getString(2)) {
                newObject.setString(5, value.get());
              }

              // Simple Mixing Air Changes per Hour
              // Set ACH to 0.0, to match the old style Material:AirWall (same as the ConstructionAirBoundary Ctor)
              newObject.setDouble(3, 0.0);

              m_refactored.push_back(RefactoredObjectData(object, newObject));
              ss << newObject;

              isAirWall = true;
            }
          }
        }

        if (!isAirWall) {
          ss << object;
        }

      } else if (iddname == "OS:Material:AirWall") {
        m_untranslated.push_back(object);

      } else if (iddname == "OS:Table:MultiVariableLookup") {

        // Stage Data List becomes extensible list (Stage 1, Stage 2, etc.)
        // ModelObjectList gets removed

        IdfObject tableObject(idd_3_5_0.getObject("OS:Table:LookUp").get());

        // Handle and name are copied
        for (size_t i = 0; i < 2; ++i) {
          if ((value = object.getString(i))) {
            tableObject.setString(i, value.get());
          }
        }

        std::string oriInterpMethod = object.getString(2, true).get();
        std::string newInterpMethod = "Cubic";
        std::string newExtrapMethod = "Constant";
        if (openstudio::istringEqual(oriInterpMethod, "LagrangeInterpolationLinearExtrapolation")) {
          newInterpMethod = "Cubic";
          newExtrapMethod = "Linear";
        } else if (istringEqual(oriInterpMethod, "LinearInterpolationOfTable")) {
          newInterpMethod = "Linear";
          newExtrapMethod = "Constant";
        }

        unsigned numIndVars = object.getInt(28).get();
        unsigned numPoints = object.numExtensibleGroups() / (numIndVars + 1);

        IdfObject varList(idd_3_5_0.getObject("OS:ModelObjectList").get());
        std::string varListHandle = toString(createUUID());
        varList.setString(0, varListHandle);
        varList.setString(1, object.nameString() + "_IndependentVariableList");
        tableObject.setString(2, varListHandle);

        // If Normalization Reference is supplied
        if (boost::optional<double> normref_ = object.getDouble(9)) {
          tableObject.setString(3, "DivisorOnly");
          tableObject.setDouble(4, normref_.get());
        } else {
          // Same as Ctor for these required-fields
          tableObject.setString(3, "None");
          tableObject.setDouble(4, 1.0);
        }

        // Min/Maximum OutputValue
        if (boost::optional<double> minVal_ = object.getDouble(20)) {
          tableObject.setDouble(5, minVal_.get());
        }
        if (boost::optional<double> minVal_ = object.getDouble(21)) {
          tableObject.setDouble(6, minVal_.get());
        }

        // Output Unit Type (will pull "Dimensionless" as default if not set, which matches the Ctor)
        tableObject.setString(7, object.getString(27, true).get());

        // 8, 9, 10 are External File stuff, left blank

        // Now extensible Output Values
        for (unsigned k = 0; k < numPoints; ++k) {
          tableObject.pushExtensibleGroup({object.getExtensibleGroup(numIndVars + (k * (numIndVars + 1))).getString(0).get()});
        }

        std::vector<IdfObject> varsAdded;

        for (unsigned i = 0; i < numIndVars; ++i) {
          IdfObject& var = varsAdded.emplace_back(idd_3_5_0.getObject("OS:Table:IndependentVariable").get());
          std::string varHandle = toString(createUUID());
          var.setString(0, varHandle);
          varList.pushExtensibleGroup({varHandle});
          var.setName(object.nameString() + "_IndependentVariable_" + std::to_string(i));

          var.setString(2, newInterpMethod);
          var.setString(3, newExtrapMethod);

          // Min Max
          if (auto minVal_ = object.getDouble(10 + 2 * i)) {
            var.setDouble(4, minVal_.get());
          }
          if (auto maxVal_ = object.getDouble(11 + 2 * i)) {
            var.setDouble(5, maxVal_.get());
          }

          // 6 - Normalization Reference Value is left blank

          // 7 - Unit Type
          var.setString(7, object.getString(22 + i, true).get());

          // 8, 9, 10 are External File stuff, left blank

          // Now extensible values: keep only unique, and sorted
          std::vector<double> xValues;
          for (unsigned k = 0; k < numPoints; ++k) {
            xValues.push_back(object.getExtensibleGroup(i + (k * (numIndVars + 1))).getDouble(0).get());
          }

          std::sort(xValues.begin(), xValues.end());
          xValues.erase(std::unique(xValues.begin(), xValues.end()), xValues.end());

          for (auto& val : xValues) {
            var.pushExtensibleGroup().setDouble(0, val);
          }
        }

        m_refactored.push_back(RefactoredObjectData(object, tableObject));
        m_new.push_back(varList);

        ss << tableObject;
        ss << varList;

        for (auto&& varAdded : varsAdded) {
          ss << varAdded;
          m_new.emplace_back(std::move(varAdded));
        }

      } else if (iddname == "OS:Coil:Heating:DX:SingleSpeed") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Supply Fan Power Per Volume Flow Rate 2023 * 7

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Rated Supply Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(7, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:DX:SingleSpeed") {

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Evaporator Fan Power Per Volume Flow Rate 2023 * 8

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 8) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Rated Supply Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(8, 934.4);

        // New defaults (now required-field, before = optional)
        // ------------------------------------------------
        // From blank to zero:
        // * Nominal Time for Condensate Removal to Begin * 17,
        // * Ratio of Initial Moisture Evaporation Rate and Steady State Latent Capacity * 18
        // * Maximum Cycling Rate * 19
        // * Latent Capacity Time Constant * 20
        for (size_t i = 17; i <= 20; ++i) {
          if (newObject.isEmpty(i)) {
            newObject.setDouble(i, 0.0);
          }
        }

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Heating:DX:MultiSpeed:StageData") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Supply Air Fan Power Per Volume Flow Rate 2023 * 6

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Rated Supply Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(6, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:DX:MultiSpeed:StageData") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Evaporator Fan Power Per Volume Flow Rate 2023 * 7

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Rated Supply Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(7, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:DX:CurveFit:Speed") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Evaporator Fan Power Per Volume Flow Rate 2023 * 9

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 9) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Rated Supply Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(9, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Heating:WaterToAirHeatPump:EquationFit") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Entering Water Temperature * 10
        // * Rated Entering Air Dry-Bulb Temperature * 11
        // * Ratio of Rated Heating Capacity to Rated Cooling Capacity * 12

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 10) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 3, value.get());
            }
          }
        }

        // Rated Entering Water Temperature
        newObject.setDouble(10, 20);

        // Rated Entering Air Dry-Bulb Temperature
        newObject.setDouble(11, 20);

        // Ratio of Rated Heating Capacity to Rated Cooling Capacity
        newObject.setDouble(12, 1.0);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:WaterToAirHeatPump:EquationFit") {

        // Fields that have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Rated Entering Water Temperature * 11
        // * Rated Entering Air Dry-Bulb Temperature * 12
        // * Rated Entering Air Wet-Bulb Temperature * 13

        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 11) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 3, value.get());
            }
          }
        }

        // Rated Entering Water Temperature
        newObject.setDouble(11, 30);

        // Rated Entering Air Dry-Bulb Temperature
        newObject.setDouble(12, 27);

        // Rated Entering Air Wet-Bulb Temperature
        newObject.setDouble(13, 19.0);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Sizing:Zone") {

        // 9 Fields have been added from 3.4.0 to 3.5.0:
        // ------------------------------------------------
        // * Zone Load Sizing Method * 26
        // * Zone Latent Cooling Design Supply Air Humidity Ratio Input Method * 27
        // * Zone Dehumidification Design Supply Air Humidity Ratio * 28
        // * Zone Cooling Design Supply Air Humidity Ratio Difference * 29
        // * Zone Latent Heating Design Supply Air Humidity Ratio Input Method * 30
        // * Zone Humidification Design Supply Air Humidity Ratio * 31
        // * Zone Humidification Design Supply Air Humidity Ratio Difference * 32
        // * Zone Humidistat Dehumidification Set Point Schedule Name * 33
        // * Zone Humidistat Humidification Set Point Schedule Name * 34
        auto iddObject = idd_3_5_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          auto value = object.getString(i);
          if (value) {
            if (i < 26) {
              // Handle
              newObject.setString(i, value.get());
            } else {
              // Every other is shifted by 9 fields
              newObject.setString(i + 9, value.get());
            }
          }
        }

        // New E+ 22.2.0 fields, IDD defaults for required fields set in Ctor
        newObject.setString(26, "Sensible Load Only No Latent Load");
        newObject.setString(27, "HumidityRatioDifference");
        // newObject.setString(28, "");
        newObject.setDouble(29, 0.005);
        newObject.setString(30, "HumidityRatioDifference");
        // newObject.setString(31, "");
        newObject.setDouble(32, 0.005);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Heating:Gas:MultiStage") {

        // 1 Field was made required:
        // ------------------------------------------------
        // * Availability Schedule * 2

        if (!object.isEmpty(2)) {
          ss << object;
        } else {

          auto iddObject = idd_3_5_0.getObject(iddname);
          IdfObject newObject(iddObject.get());
          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }
          newObject.setString(2, getOrCreateAlwaysDiscreteScheduleHandleStr(true));

          m_refactored.push_back(RefactoredObjectData(object, newObject));
          ss << newObject;
        }

      } else if (iddname == "OS:ZoneHVAC:PackagedTerminalHeatPump") {

        if (!object.isEmpty(23)) {
          ss << object;

        } else {
          auto iddObject = idd_3_5_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }
          // alwaysOffDiscreteScheduleHandleStr has already been initialized above
          newObject.setString(23, alwaysOffDiscreteScheduleHandleStr);

          m_refactored.push_back(RefactoredObjectData(object, newObject));
          ss << newObject;
        }
      } else if (iddname == "OS:ZoneHVAC:PackagedTerminalAirConditioner") {

        if (!object.isEmpty(17)) {
          ss << object;

        } else {
          auto iddObject = idd_3_5_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }
          // alwaysOffDiscreteScheduleHandleStr has already been initialized above
          newObject.setString(17, alwaysOffDiscreteScheduleHandleStr);

          m_refactored.push_back(RefactoredObjectData(object, newObject));
          ss << newObject;
        }

      } else if (iddname == "OS:Fan:ConstantVolume") {
        std::string fanHandleStr = object.getString(0).get();
        if (std::find(packagedFanCVHandleStrs.cbegin(), packagedFanCVHandleStrs.cend(), fanHandleStr) == packagedFanCVHandleStrs.cend()) {
          ss << object;
        } else {
          LOG(Warn, "Fan:ConstantVolume "
                      << object.nameString()
                      << " is used in a Packaged System (PTAC or PTHP) that does not have a Supply Air Fan Operating Mode Schedule. "
                         "In 22.1.0 this would effectively, and mistakenly, function as a cycling fan, but this is now disallowed in E+ 22.2.0. "
                         "In order to retain a similar functionality and energy usage, this will be replaced by a Fan:SystemModel "
                         "with an always Off Schedule (cycling fan, similar to a Fan:OnOff)");
          IdfObject newObject(idd_3_5_0.getObject("OS:Fan:SystemModel").get());
          // Handle
          if ((value = object.getString(0))) {
            newObject.setString(0, value.get());
          }

          // Name
          if ((value = object.getString(1))) {
            newObject.setString(1, value.get());
          }

          // Availability Schedule Name
          if ((value = object.getString(2))) {
            newObject.setString(2, value.get());
          }

          // Note the use of "true" to return default. Fan:ConstantVolume has way more defaulted fields, while Fan:SystemModel adheres to the newer
          // "make \required-field and hardcode in ctor" doctrine, AND has different defaults than Fan:ConstantVolume. So we do this to fulfil two
          // goals: Put a value in all required-fields AND set the same values
          // Fan Total Efficiency
          if ((value = object.getString(3, true))) {
            newObject.setString(15, value.get());
          }

          // Pressure Rise
          if ((value = object.getString(4, true))) {
            newObject.setString(8, value.get());
          }

          // Maximum Flow Rate
          if ((value = object.getString(5, true))) {
            newObject.setString(5, value.get());
          }

          // Motor Efficiency
          if ((value = object.getString(6, true))) {
            newObject.setString(9, value.get());
          }

          // Motor In Airstream Fraction
          if ((value = object.getString(7, true))) {
            newObject.setString(10, value.get());
          }

          // Nodes should be blank since it's inside a containing ZoneHVAC
          // Air Inlet Node Name
          if ((value = object.getString(8))) {
            newObject.setString(3, value.get());
          }

          // Air Outlet Node Name
          if ((value = object.getString(9))) {
            newObject.setString(4, value.get());
          }

          // End-Use Subcategory
          if ((value = object.getString(10, true))) {
            newObject.setString(21, value.get());
          }

          // Speed Control Method
          newObject.setString(6, "Discrete");

          // Electric Power Minimum Flow Rate Fraction
          newObject.setDouble(7, 0.0);

          // Design Electric Power Consumption
          newObject.setString(11, "Autosize");

          // Design Power Sizing Method
          newObject.setString(12, "TotalEfficiencyAndPressure");

          // Electric Power Per Unit Flow Rate (not used given Power Sizing Method, but required-field & set in Ctor)
          newObject.setDouble(13, 840.0);

          // Electric Power Per Unit Flow Rate Per Unit Pressure (not used given Power Sizing Method, but required-field & set in Ctor)
          newObject.setDouble(14, 1.66667);

          // Electric Power Function of Flow Fraction Curve Name
          newObject.setString(16, "");

          // Night Ventilation Mode Pressure Rise
          newObject.setString(17, "");

          // Night Ventilation Mode Flow Fraction
          newObject.setString(18, "");

          // Motor Loss Zone Name
          newObject.setString(19, "");

          // Motor Loss Radiative Fraction
          newObject.setDouble(20, 0.0);

          m_refactored.push_back(RefactoredObjectData(object, newObject));
          ss << newObject;
        }

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_4_0_to_3_5_0

  std::string VersionTranslator::update_3_5_0_to_3_5_1(const IdfFile& idf_3_5_0, const IddFileAndFactoryWrapper& idd_3_5_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_5_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_5_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_3_5_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:UnitarySystemPerformance:Multispeed") {

        // 1 Field has been added from 3.5.0 to 3.5.1:
        // ----------------------------------------------
        // * No Load Supply Air Flow Rate Ratio * 3
        auto iddObject = idd_3_5_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 3) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setDouble(3, 1.0);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_5_0_to_3_5_1

  std::string VersionTranslator::update_3_5_1_to_3_6_0(const IdfFile& idf_3_5_1, const IddFileAndFactoryWrapper& idd_3_6_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_5_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_6_0.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_3_5_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:GroundHeatExchanger:HorizontalTrench") {

        // Kusuda fields get moved over to OS:Site:GroundTemperature:Undisturbed:KusudaAchenbach

        auto iddObject = idd_3_6_0.getObject(iddname);
        IdfObject ghxObject(iddObject.get());
        IdfObject kusudaObject(idd_3_6_0.getObject("OS:Site:GroundTemperature:Undisturbed:KusudaAchenbach").get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if (i < 19) {
            if ((value = object.getString(i))) {
              ghxObject.setString(i, value.get());
              if (((i == 11) || (i == 12) || (i == 13))) {  // Soil xxx
                kusudaObject.setString(i - 9, value.get());
              }
            }
          } else if (i == 19) {  // Undisturbed Ground Temperature Model
            ghxObject.setString(i, kusudaObject.getString(0).get());
          } else if (i < 23) {  // Kusuda-Achenbach xxx
            if ((value = object.getString(i))) {
              kusudaObject.setString(i - 15, value.get());
            }
          } else if (i == 23) {  // Evapotranspiration Ground Cover Parameter
            if ((value = object.getString(i))) {
              ghxObject.setString(20, value.get());
            }
          }
        }

        m_refactored.push_back(RefactoredObjectData(object, ghxObject));
        m_new.push_back(kusudaObject);

        ss << ghxObject;
        ss << kusudaObject;

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_5_1_to_3_6_0

  struct CoilLatentTransitionInfo
  {
    enum class ParentType
    {
      Unknown = -1,
      AirLoopHVACUnitary,
      ZoneWAHP
    };
    static ParentType objectParentType(const IdfObject& parentObject) {
      const std::string iddname = parentObject.iddObject().name();
      if (iddname == "OS:AirLoopHVAC:UnitarySystem") {
        return ParentType::AirLoopHVACUnitary;
      } else if (iddname == "OS:ZoneHVAC:WaterToAirHeatPump") {
        return ParentType::ZoneWAHP;
      }
      return ParentType::Unknown;
    };

   public:
    ParentType parentType;
    std::string parentName;
    std::string heatingCoilType;
    std::string coolingCoilType;
    std::string heatingCoilName;
    std::string coolingCoilName;
    double maxCyclingRate = 2.5;      // Nmax, in /hr
    double heatPumpTimeConst = 60.0;  // tau, in s
    double fractionOnCycle = 0.01;
    double hpDelayTime = 60.0;

    // Helpers
    static std::vector<CoilLatentTransitionInfo>::iterator findFromParent(std::vector<CoilLatentTransitionInfo>& infos,
                                                                          const IdfObject& parentObject);
    static std::vector<CoilLatentTransitionInfo>::iterator findFromCoolingCoil(std::vector<CoilLatentTransitionInfo>& infos,
                                                                               const IdfObject& coilObject);
    static std::vector<CoilLatentTransitionInfo>::iterator findFromHeatingCoil(std::vector<CoilLatentTransitionInfo>& infos,
                                                                               const IdfObject& coilObject);

    static IdfObject defaultHeatPumpCoilPLFCorrelationCurve(const IddFileAndFactoryWrapper& idd_3_7_0, const std::string& name,
                                                            double maximumCyclingRatePerHour = 2.5, double heatPumpTimeConstantSeconds = 60.0);

    bool isCurveCreationNeeded() const {
      return (heatingCoilType == "OS:Coil:Heating:WaterToAirHeatPump:EquationFit" ||  //
              coolingCoilType == "OS:Coil:Cooling:WaterToAirHeatPump:EquationFit");
      // OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit, and OS:Coil:Cooling:DX:VariableSpeed already have a PLF curve
      // OS:Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit does not have any change
    }

    IdfObject createCurveLinear(const IddFileAndFactoryWrapper& idd_3_7_0) const;

    std::string curveName() const {
      return fmt::format("{}-AutogeneratedPLFCurve", parentName);
    }

   private:
    mutable boost::optional<IdfObject> cachedCurveLinear_;
  };

  std::vector<CoilLatentTransitionInfo>::iterator CoilLatentTransitionInfo::findFromParent(std::vector<CoilLatentTransitionInfo>& infos,
                                                                                           const IdfObject& parentObject) {

    ParentType pType = objectParentType(parentObject);
    const auto name = parentObject.nameString();
    auto it =
      std::find_if(infos.begin(), infos.end(), [&pType, &name](auto& info) { return (info.parentType == pType) && (info.parentName == name); });
    return it;
  }

  std::vector<CoilLatentTransitionInfo>::iterator CoilLatentTransitionInfo::findFromCoolingCoil(std::vector<CoilLatentTransitionInfo>& infos,
                                                                                                const IdfObject& coilObject) {

    auto it = std::find_if(infos.begin(), infos.end(), [&coilObject](auto& info) {
      return (info.coolingCoilType == coilObject.iddObject().name()) && (info.coolingCoilName == coilObject.nameString());
    });
    return it;
  }

  std::vector<CoilLatentTransitionInfo>::iterator CoilLatentTransitionInfo::findFromHeatingCoil(std::vector<CoilLatentTransitionInfo>& infos,
                                                                                                const IdfObject& coilObject) {

    auto it = std::find_if(infos.begin(), infos.end(), [&coilObject](auto& info) {
      return (info.heatingCoilType == coilObject.iddObject().name()) && (info.heatingCoilName == coilObject.nameString());
    });
    return it;
  }

  IdfObject CoilLatentTransitionInfo::defaultHeatPumpCoilPLFCorrelationCurve(const IddFileAndFactoryWrapper& idd_3_7_0, const std::string& name,
                                                                             double maximumCyclingRatePerHour, double heatPumpTimeConstantSeconds) {

    const double A = 4 * (heatPumpTimeConstantSeconds / 3600.0) * maximumCyclingRatePerHour;
    const double Cd = A * (1 - std::exp(-1 / A));

    auto iddObject = idd_3_7_0.getObject("OS:Curve:Linear");
    IdfObject curveObject(iddObject.get());
    const std::string uuid = toString(createUUID());
    curveObject.setString(0, uuid);
    curveObject.setString(1, name);
    curveObject.setDouble(2, (1 - Cd));
    curveObject.setDouble(3, Cd);
    // Min/Max Value of x
    curveObject.setDouble(4, 0.0);
    curveObject.setDouble(5, 1.0);
    // Min/max curve output
    curveObject.setDouble(6, 0.0);
    curveObject.setDouble(7, 1.0);
    // Input Unit type for X
    curveObject.setString(8, "Dimensionless");
    // Output Unit type
    curveObject.setString(9, "Dimensionless");
    return curveObject;
  }

  IdfObject CoilLatentTransitionInfo::createCurveLinear(const IddFileAndFactoryWrapper& idd_3_7_0) const {
    if (!cachedCurveLinear_) {
      cachedCurveLinear_ =
        CoilLatentTransitionInfo::defaultHeatPumpCoilPLFCorrelationCurve(idd_3_7_0, curveName(), maxCyclingRate, heatPumpTimeConst);
    }
    return *cachedCurveLinear_;
  }

  std::vector<CoilLatentTransitionInfo> preScanCoilLatentChanges(const IdfFile& idf_3_6_1) {

    static constexpr std::array<std::string_view, 2> heatingCoilTypesChanged{"OS:Coil:Heating:WaterToAirHeatPump:EquationFit",
                                                                             "OS:Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit"};
    static constexpr std::array<std::string_view, 3> coolingCoilTypesChanged{"OS:Coil:Cooling:WaterToAirHeatPump:EquationFit",
                                                                             "OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit",
                                                                             "OS:Coil:Cooling:DX:VariableSpeed"};

    std::vector<CoilLatentTransitionInfo> result;

    for (const IdfObject& object : idf_3_6_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:AirLoopHVAC:UnitarySystem") {

        std::string heatingCoilType;
        std::string heatingCoilName;
        std::string coolingCoilType;
        std::string coolingCoilName;

        // 11 - Heating Coil Name
        if (auto handleStr_ = object.getString(11)) {
          if (auto coilObject_ = idf_3_6_1.getObject(toUUID(*handleStr_))) {
            heatingCoilName = coilObject_->nameString();
            heatingCoilType = coilObject_->iddObject().name();
          }
        }

        // 13 - Cooling Coil Name
        if (auto handleStr_ = object.getString(13)) {
          if (auto coilObject_ = idf_3_6_1.getObject(toUUID(*handleStr_))) {
            coolingCoilName = coilObject_->nameString();
            coolingCoilType = coilObject_->iddObject().name();
          }
        }

        const bool hasHeatingChanges =
          (std::find(heatingCoilTypesChanged.cbegin(), heatingCoilTypesChanged.cend(), heatingCoilType) != heatingCoilTypesChanged.end());
        const bool hasCoolingChanges =
          (std::find(coolingCoilTypesChanged.cbegin(), coolingCoilTypesChanged.cend(), coolingCoilType) != coolingCoilTypesChanged.end());
        const bool infoNeeded = hasHeatingChanges || hasCoolingChanges;

        if (!infoNeeded) {
          continue;
        }

        CoilLatentTransitionInfo& info = result.emplace_back();
        info.parentType = CoilLatentTransitionInfo::ParentType::AirLoopHVACUnitary;
        info.parentName = object.nameString();
        info.heatingCoilName = std::move(heatingCoilName);
        info.heatingCoilType = std::move(heatingCoilType);
        info.coolingCoilName = std::move(coolingCoilName);
        info.coolingCoilType = std::move(coolingCoilType);

        // 38 - Maximum Cycling Rate
        if (auto val_ = object.getDouble(38)) {
          info.maxCyclingRate = *val_;
        }

        // 39 - Heat Pump Time Constant
        if (auto val_ = object.getDouble(39)) {
          info.heatPumpTimeConst = *val_;
        }

        // 40 - Fraction of On-Cycle Power Use
        if (auto val_ = object.getDouble(40)) {
          info.fractionOnCycle = *val_;
        }

        // 41 - Heat Pump Fan Delay Time
        if (auto val_ = object.getDouble(41)) {
          info.hpDelayTime = *val_;
        }

      } else if (iddname == "OS:ZoneHVAC:WaterToAirHeatPump") {

        std::string heatingCoilType;
        std::string heatingCoilName;
        std::string coolingCoilType;
        std::string coolingCoilName;

        // 13 - Heating Coil Name
        if (auto handleStr_ = object.getString(13)) {
          if (auto coilObject_ = idf_3_6_1.getObject(toUUID(*handleStr_))) {
            heatingCoilName = coilObject_->nameString();
            heatingCoilType = coilObject_->iddObject().name();
          }
        }

        // 14 - Cooling Coil Name
        if (auto handleStr_ = object.getString(14)) {
          if (auto coilObject_ = idf_3_6_1.getObject(toUUID(*handleStr_))) {
            coolingCoilName = coilObject_->nameString();
            coolingCoilType = coilObject_->iddObject().name();
          }
        }

        const bool hasHeatingChanges =
          (std::find(heatingCoilTypesChanged.cbegin(), heatingCoilTypesChanged.cend(), heatingCoilType) != heatingCoilTypesChanged.end());
        const bool hasCoolingChanges =
          (std::find(coolingCoilTypesChanged.cbegin(), coolingCoilTypesChanged.cend(), coolingCoilType) != coolingCoilTypesChanged.end());
        const bool infoNeeded = hasHeatingChanges || hasCoolingChanges;

        if (!infoNeeded) {
          continue;
        }

        CoilLatentTransitionInfo& info = result.emplace_back();
        info.parentType = CoilLatentTransitionInfo::ParentType::ZoneWAHP;
        info.parentName = object.nameString();
        info.heatingCoilName = std::move(heatingCoilName);
        info.heatingCoilType = std::move(heatingCoilType);
        info.coolingCoilName = std::move(coolingCoilName);
        info.coolingCoilType = std::move(coolingCoilType);

        // 15 - Maximum Cycling Rate
        if (auto val_ = object.getDouble(15)) {
          info.maxCyclingRate = *val_;
        }

        // 16 - Heat Pump Time Constant
        if (auto val_ = object.getDouble(16)) {
          info.heatPumpTimeConst = *val_;
        }

        // 17 - Fraction of On-Cycle Power Use
        if (auto val_ = object.getDouble(17)) {
          info.fractionOnCycle = *val_;
        }

        // 18 - Heat Pump Fan Delay Time
        if (auto val_ = object.getDouble(18)) {
          info.hpDelayTime = *val_;
        }
      }

      // AIRLOOPHVAC:UNITARYHEATPUMP:WATERTOAIR is not wrapped
    }
    return result;
  }

  std::string VersionTranslator::update_3_6_1_to_3_7_0(const IdfFile& idf_3_6_1, const IddFileAndFactoryWrapper& idd_3_7_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_6_1.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_7_0.iddFile());
    ss << targetIdf.versionObject().get();

    std::vector<CoilLatentTransitionInfo> coilTransitionInfos = preScanCoilLatentChanges(idf_3_6_1);

    constexpr std::array<std::pair<std::string_view, size_t>, 10> crankcaseCoilWithIndex{{
      {"OS:Coil:Cooling:DX:CurveFit:Performance", 3},
      {"OS:Coil:Cooling:DX:SingleSpeed", 27},
      {"OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode", 6},
      {"OS:Coil:Cooling:DX:MultiSpeed", 13},
      {"OS:Coil:Heating:DX:SingleSpeed", 19},
      {"OS:Coil:Heating:DX:MultiSpeed", 8},
      {"OS:Coil:Heating:DX:VariableSpeed", 13},
      {"OS:Coil:WaterHeating:AirToWaterHeatPump", 20},
      {"OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed", 18},
      {"OS:Coil:WaterHeating:AirToWaterHeatPump:Wrapped", 13},
    }};

    // Making the map case-insensitive by providing a Comparator `IstringCompare`
    const std::map<std::string, std::string, openstudio::IstringCompare> replaceFuelTypesMap{{
      {"Steam", "DistrictHeatingSteam"},
      {"DistrictHeating", "DistrictHeatingWater"},
      // Additionally, for UtilityBill, align the IDD choices to E+. This will also be covered by this
      {"Gas", "NaturalGas"},
      {"FuelOil_1", "FuelOilNo1"},
      {"FuelOil_2", "FuelOilNo2"},
      {"OtherFuel_1", "OtherFuel1"},
      {"OtherFuel_2", "OtherFuel2"},
    }};

    const std::multimap<std::string, int> fuelTypeRenamesMap{{
      {"OS:OtherEquipment", 6},                                // Fuel Type
      {"OS:Exterior:FuelEquipment", 4},                        // Fuel Use Type
      {"OS:WaterHeater:Mixed", 11},                            // Heater Fuel Type
      {"OS:WaterHeater:Mixed", 15},                            // Off Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Mixed", 18},                            // On Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Stratified", 17},                       // Heater Fuel Type
      {"OS:WaterHeater:Stratified", 20},                       // Off Cycle Parasitic Fuel Type
      {"OS:WaterHeater:Stratified", 24},                       // On Cycle Parasitic Fuel Type
      {"OS:UtilityBill", 2},                                   // Fuel Type
      {"OS:Meter:Custom", 2},                                  // Fuel Type
      {"OS:Meter:CustomDecrement", 2},                         // Fuel Type
      {"OS:EnergyManagementSystem:MeteredOutputVariable", 5},  // Resource Type
      {"OS:PythonPlugin:OutputVariable", 6},                   // Resource Type
    }};

    auto checkIfReplaceNeeded = [replaceFuelTypesMap](const IdfObject& object, int fieldIndex) -> bool {
      if (boost::optional<std::string> fuelType_ = object.getString(fieldIndex)) {
        return replaceFuelTypesMap.contains(*fuelType_);
      }
      return false;
    };

    auto replaceForField = [&replaceFuelTypesMap](const IdfObject& object, IdfObject& newObject, int fieldIndex) -> void {
      if (boost::optional<std::string> fuelType_ = object.getString(fieldIndex)) {
        auto it = replaceFuelTypesMap.find(*fuelType_);
        if (it != replaceFuelTypesMap.end()) {
          LOG(Trace, "Replacing " << *fuelType_ << " with " << it->second << " at fieldIndex " << fieldIndex << " for " << object.nameString());
          newObject.setString(fieldIndex, it->second);
        }
      }
    };

    /*************************************************************************************************************************************************
     *                                                               Output:Variable fuel                                                            *
     ************************************************************************************************************************************************/

    const static boost::regex re_strip_multiple_spaces("[' ']{2,}");

    // Making the map case-insensitive by providing a Comparator `IstringCompare`
    // https://github.com/NREL/EnergyPlus/blob/v9.4.0-IOFreeze/src/Transition/SupportFiles/Report%20Variables%209-3-0%20to%209-4-0.csv
    const static std::map<std::string, std::string, openstudio::IstringCompare> replaceOutputVariablesMap({
      {"District Cooling Chilled Water Energy", "District Cooling Water Energy"},
      {"District Cooling Chilled Water Rate", "District Cooling Water Rate"},
      {"District Cooling Rate", "District Cooling Water Rate"},
      {"District Cooling Inlet Temperature", "District Cooling Water Inlet Temperature"},
      {"District Cooling Outlet Temperature", "District Cooling Water Outlet Temperature"},
      {"District Cooling Mass Flow Rate", "District Cooling Water Mass Flow Rate"},
      {"District Heating Hot Water Energy", "District Heating Water Energy"},
      {"District Heating Hot Water Rate", "District Heating Water Rate"},
      {"District Heating Rate", "District Heating Water Rate"},
      {"District Heating Inlet Temperature", "District Heating Water Inlet Temperature"},
      {"District Heating Outlet Temperature", "District Heating Water Outlet Temperature"},
      {"District Heating Mass Flow Rate", "District Heating Water Mass Flow Rate"},
    });

    /*************************************************************************************************************************************************
     *                                                          Output:Meter fuel types renames                                                      *
     ************************************************************************************************************************************************/

    const static std::array<std::pair<std::string, std::string>, 2> meterFuelTypesMap{{
      {"DistrictHeating", "DistrictHeatingWater"},
      {"Steam", "DistrictHeatingSteam"},
    }};

    // Could make it a static inside the lambda, except that it won't be reset so if you try to translate twice it fails
    std::string discreteSchHandleStr;

    auto getOrCreateAlwaysOnContinuousSheduleHandleStr = [this, &ss, &idf_3_6_1, &idd_3_7_0, &discreteSchHandleStr]() -> std::string {
      if (!discreteSchHandleStr.empty()) {
        LOG(Trace, "Already found 'Always On Continuous' Schedule in model with handle " << discreteSchHandleStr);
        return discreteSchHandleStr;
      }

      const std::string name = "Always On Continuous";
      const double val = 1.0;
      // Add an alwaysOnDiscreteSchedule if one does not already exist
      for (const IdfObject& object : idf_3_6_1.getObjectsByType(idf_3_6_1.iddFile().getObject("OS:Schedule:Constant").get())) {
        if (boost::optional<std::string> name_ = object.getString(1)) {
          if (istringEqual(name_.get(), name)) {
            if (boost::optional<double> value = object.getDouble(3)) {
              if (equal<double>(value.get(), val)) {
                discreteSchHandleStr = object.getString(0).get();  // Store in state variable
                LOG(Trace, "Found existing 'Always On Continuous' Schedule in model with handle " << discreteSchHandleStr);
                return discreteSchHandleStr;
              }
            }
          }
        }
      }

      auto discreteSch = IdfObject(idd_3_7_0.getObject("OS:Schedule:Constant").get());

      discreteSchHandleStr = toString(createUUID());  // Store in state variable
      discreteSch.setString(0, discreteSchHandleStr);
      discreteSch.setString(1, name);
      discreteSch.setDouble(3, val);

      IdfObject typeLimits(idd_3_7_0.getObject("OS:ScheduleTypeLimits").get());
      typeLimits.setString(0, toString(createUUID()));
      typeLimits.setString(1, name + " Limits");
      typeLimits.setDouble(2, 0.0);
      typeLimits.setDouble(3, 1.0);
      typeLimits.setString(4, "Continuous");
      typeLimits.setString(5, "");

      discreteSch.setString(2, typeLimits.getString(0).get());

      ss << discreteSch;
      ss << typeLimits;

      // Register new objects
      m_new.emplace_back(std::move(discreteSch));
      m_new.emplace_back(std::move(typeLimits));
      LOG(Trace, "Created 'Always On Continuous' Schedule with handle " << discreteSchHandleStr);

      return discreteSchHandleStr;
    };

    for (const IdfObject& object : idf_3_6_1.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:GroundHeatExchanger:Vertical") {

        // 1 Field has been added from 3.6.1 to 3.7.0:
        // -------------------------------------------
        // * Undisturbed Ground Temperature Model * 17
        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject ghxObject(iddObject.get());
        IdfObject kusudaObject(idd_3_7_0.getObject("OS:Site:GroundTemperature:Undisturbed:KusudaAchenbach").get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if (i < 17) {
            if ((value = object.getString(i))) {
              ghxObject.setString(i, value.get());
              if (i == 8) {  // Soil Thermal Conductivity
                kusudaObject.setString(i - 6, value.get());
              }
              if (i == 10) {  // Average Soil Surface Temperature
                kusudaObject.setString(i - 5, value.get());
              }
            }
            if (boost::optional<double> _value = object.getDouble(i)) {
              if (i == 9) {  // Soil Specific Heat
                kusudaObject.setDouble(i - 5, _value.get() / 920.0);
              }
            }
          } else {
            if (i == 17) {  // Undisturbed Ground Temperature Model
              ghxObject.setString(i, kusudaObject.getString(0).get());
            }
            if ((value = object.getString(i))) {
              ghxObject.setString(i + 1, value.get());
            }
          }
        }

        kusudaObject.setDouble(3, 920.0);  // Soil Density
        kusudaObject.setDouble(6, 3.2);    // Average Amplitude of Surface Temperature
        kusudaObject.setDouble(7, 8.0);    // Phase Shift of Minimum Surface Temperature

        m_refactored.push_back(RefactoredObjectData(object, ghxObject));
        m_new.push_back(kusudaObject);

        ss << ghxObject;
        ss << kusudaObject;

      } else if ((iddname == "OS:Coil:Cooling:DX:VariableSpeed:SpeedData") || (iddname == "OS:Coil:Heating:DX:VariableSpeed:SpeedData")) {

        // The two coils have the new fields in the same location, with the same defaults

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // Heating:
        // * Rated Supply Fan Power Per Volume Flow Rate 2017 * 5
        // * Rated Supply Fan Power Per Volume Flow Rate 2023 * 6
        // Cooling:
        // * Rated Evaporator Fan Power Per Volume Flow Rate 2017 - 6
        // * Rated Evaporator Fan Power Per Volume Flow Rate 2023 - 7

        const bool is_cooling = (iddname == "OS:Coil:Cooling:DX:VariableSpeed:SpeedData");
        const size_t insertionIndex = is_cooling ? 6 : 5;

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < insertionIndex) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 2, value.get());
            }
          }
        }

        // Rated Supply/Evaporator Fan Power Per Volume Flow Rate 2017
        newObject.setDouble(insertionIndex, 773.3);

        // Rated Supply/Evaporator Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(insertionIndex + 1, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:DX:TwoSpeed") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Rated High Speed Evaporator Fan Power Per Volume Flow Rate 2017 * 7
        // * Rated High Speed Evaporator Fan Power Per Volume Flow Rate 2023 * 8
        // * Rated Low Speed Evaporator Fan Power Per Volume Flow Rate 2017 * 21
        // * Rated Low Speed Evaporator Fan Power Per Volume Flow Rate 2023 * 22

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else if (i < 19) {
              newObject.setString(i + 2, value.get());
            } else {
              newObject.setString(i + 4, value.get());
            }
          }
        }

        // Rated High Speed Evaporator Fan Power Per Volume Flow Rate 2017
        newObject.setDouble(7, 773.3);
        // Rated High Speed Evaporator Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(8, 934.4);

        // Rated Low Speed Evaporator Fan Power Per Volume Flow Rate 2017
        newObject.setDouble(21, 773.3);
        // Rated Low Speed Evaporator Fan Power Per Volume Flow Rate 2023
        newObject.setDouble(22, 934.4);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:AirLoopHVAC:UnitarySystem") {

        // Removed fields
        // 38 - Maximum Cycling Rate
        // 39 - Heat Pump Time Constant
        // 40 - Fraction of On-Cycle Power Use
        // 41 - Heat Pump Fan Delay Time

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 38) {
              newObject.setString(i, value.get());
            } else if (i > 41) {
              newObject.setString(i - 4, value.get());
            }
          }
        }

        // * 11 - Heating Coil Name
        constexpr unsigned heatingCoilNameIndex = 11;
        const bool hasHeatingCoil = !object.isEmpty(heatingCoilNameIndex);

        // * 13 - Cooling Coil Name
        constexpr unsigned coolingCoilNameIndex = 13;
        const bool hasCoolingCoil = !object.isEmpty(coolingCoilNameIndex);

        // We use getString(idx, false, true) to not get default, returned unitialized empty

        // * 18 - Supply Air Flow Rate Method During Cooling Operation
        constexpr unsigned coolingSAFMethodIndex = 18;
        boost::optional<std::string> coolingSAFMethod = object.getString(coolingSAFMethodIndex, false, true);

        // * 23 - Supply Air Flow Rate Method During Heating Operation
        constexpr unsigned heatingSAFMethodIndex = 23;
        boost::optional<std::string> heatingSAFMethod = object.getString(heatingSAFMethodIndex, false, true);

        // * 28 - Supply Air Flow Rate Method When No Cooling or Heating is Required
        constexpr unsigned noCoolHeatSAFMethodIndex = 28;
        boost::optional<std::string> noCoolHeatSAFMethod = object.getString(noCoolHeatSAFMethodIndex, false, true);

        constexpr std::array<std::string_view, 4> coolingSAFMethodChoices{"SupplyAirFlowRate", "FlowPerFloorArea", "FractionOfAutosizedCoolingValue",
                                                                          "FlowPerCoolingCapacity"};

        constexpr std::array<std::string_view, 4> coolingSAFMethodChoicesUC{"SUPPLYAIRFLOWRATE", "FLOWPERFLOORAREA",
                                                                            "FRACTIONOFAUTOSIZEDCOOLINGVALUE", "FLOWPERCOOLINGCAPACITY"};

        constexpr std::array<std::string_view, 4> heatingSAFMethodChoices{"SupplyAirFlowRate", "FlowPerFloorArea", "FractionOfAutosizedHeatingValue",
                                                                          "FlowPerHeatingCapacity"};
        constexpr std::array<std::string_view, 4> heatingSAFMethodChoicesUC{"SUPPLYAIRFLOWRATE", "FLOWPERFLOORAREA",
                                                                            "FRACTIONOFAUTOSIZEDHEATINGVALUE", "FLOWPERHEATINGCAPACITY"};

        constexpr std::array<std::string_view, 6> noCoolHeatSAFMethodChoices{
          "SupplyAirFlowRate",      "FlowPerFloorArea",      "FractionOfAutosizedCoolingValue", "FractionOfAutosizedHeatingValue",
          "FlowPerCoolingCapacity", "FlowPerHeatingCapacity"};

        constexpr std::array<std::string_view, 6> noCoolHeatSAFMethodChoicesUC{
          "SUPPLYAIRFLOWRATE",      "FLOWPERFLOORAREA",      "FRACTIONOFAUTOSIZEDCOOLINGVALUE", "FRACTIONOFAUTOSIZEDHEATINGVALUE",
          "FLOWPERCOOLINGCAPACITY", "FLOWPERHEATINGCAPACITY"};

        // Reset all flow fields, we want only one at best to be set
        for (size_t i = 0; i < coolingSAFMethodChoices.size(); ++i) {
          const size_t fieldIndex = coolingSAFMethodIndex + 1 + i;
          newObject.setString(fieldIndex, "");
        }
        for (size_t i = 0; i < heatingSAFMethodChoices.size(); ++i) {
          const size_t fieldIndex = heatingSAFMethodIndex + 1 + i;
          newObject.setString(fieldIndex, "");
        }
        for (size_t i = 0; i < noCoolHeatSAFMethodChoices.size(); ++i) {
          const size_t fieldIndex = noCoolHeatSAFMethodIndex + 1 + i;
          newObject.setString(fieldIndex, "");
        }

        if (!hasCoolingCoil) {
          newObject.setString(coolingSAFMethodIndex, "None");
        } else if (!coolingSAFMethod) {
          // Technically here E+ checks if the cooling coil rated air flow rate is autosized or not
          // Also could get overriden by the DesignSpecification:ZoneHVAC:Sizing
          // Finally it lets it slide as long as one of the other SupplyAirFlowRateXXX is not empty
          // Here I guess we'll scan the fields in order, and pick the first we find
          // * 19 - Supply Air Flow Rate During Cooling Operation
          // * 20 - Supply Air Flow Rate Per Floor Area During Cooling Operation
          // * 21 - Fraction of Autosized Design Cooling Supply Air Flow Rate
          // * 22 - Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation
          bool found = false;
          for (size_t i = 0; i < coolingSAFMethodChoices.size(); ++i) {
            const size_t fieldIndex = coolingSAFMethodIndex + 1 + i;
            if ((value = object.getString(fieldIndex, false, true))) {
              newObject.setString(coolingSAFMethodIndex, std::string{coolingSAFMethodChoices[i]});
              newObject.setString(fieldIndex, *value);
              found = true;
              break;
            }
          }
          if (!found) {
            LOG(Warn, "For AirLoopHVACUnitarySystem '"
                        << object.nameString()
                        << "', there is a cooling coil, but no Supply Air Flow Rate Method During Cooling Operation and no flow rate fields set. "
                           "Defaulting to SupplyAirFlowRate with Autosize");
            newObject.setString(coolingSAFMethodIndex, "SupplyAirFlowRate");
            newObject.setString(coolingSAFMethodIndex + 1, "Autosize");
          }
        } else {
          std::string coolingSAFMethodUC = ascii_to_upper_copy(*coolingSAFMethod);
          if (coolingSAFMethodUC == "NONE") {
            LOG(Warn, "For AirLoopHVACUnitarySystem '"
                        << object.nameString()
                        << "', Supply Air Flow Method Rate During Cooling Operation is 'None' but you have a Cooling Coil, consider changing it");
          } else {
            auto it = std::find_if(coolingSAFMethodChoicesUC.cbegin(), coolingSAFMethodChoicesUC.cend(),
                                   [&coolingSAFMethodUC](auto& s) { return s == coolingSAFMethodUC; });
            if (it == coolingSAFMethodChoicesUC.cend()) {
              LOG(Error, "For AirLoopHVACUnitarySystem '"
                           << object.nameString() << "', Unrecognized Supply Air Flow Method Rate During Cooling Operation=" << *coolingSAFMethod);
            } else {
              const auto dist = std::distance(coolingSAFMethodChoicesUC.cbegin(), it);
              const size_t index = coolingSAFMethodIndex + 1 + dist;
              if ((value = object.getString(index, false, true))) {
                newObject.setString(index, *value);
              } else {
                LOG(Error, "For AirLoopHVACUnitarySystem '" << object.nameString() << "', Supply Air Flow Method Rate During Cooling Operation is '"
                                                            << *coolingSAFMethod << "' but associated field is empty. Setting it to zero.");
                newObject.setDouble(index, 0.0);
              }
            }
          }
        }

        if (!hasHeatingCoil) {
          newObject.setString(heatingSAFMethodIndex, "None");
        } else if (!heatingSAFMethod) {

          // * 23 - Supply Air Flow Rate Method During Heating Operation
          // * 24 - Supply Air Flow Rate During Heating Operation
          // * 25 - Supply Air Flow Rate Per Floor Area during Heating Operation
          // * 26 - Fraction of Autosized Design Heating Supply Air Flow Rate
          // * 27 - Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation
          bool found = false;
          for (size_t i = 0; i < heatingSAFMethodChoices.size(); ++i) {
            const size_t fieldIndex = heatingSAFMethodIndex + 1 + i;
            if ((value = object.getString(fieldIndex, false, true))) {
              newObject.setString(heatingSAFMethodIndex, std::string{heatingSAFMethodChoices[i]});
              newObject.setString(fieldIndex, *value);
              found = true;
              break;
            }
          }
          if (!found) {
            LOG(Warn, "For AirLoopHVACUnitarySystem '"
                        << object.nameString()
                        << "', there is a heating coil, but no Supply Air Flow Rate Method During Heating Operation and no flow rate fields set. "
                           "Defaulting to SupplyAirFlowRate with Autosize");
            newObject.setString(heatingSAFMethodIndex, "SupplyAirFlowRate");
            newObject.setString(heatingSAFMethodIndex + 1, "Autosize");
          }
        } else {
          std::string heatingSAFMethodUC = ascii_to_upper_copy(*heatingSAFMethod);
          if (heatingSAFMethodUC == "NONE") {
            LOG(Warn, "For AirLoopHVACUnitarySystem '"
                        << object.nameString()
                        << "', Supply Air Flow Method Rate During Heating Operation is 'None' but you have a Heating coil, consider changing it");
          } else {
            auto it = std::find_if(heatingSAFMethodChoicesUC.cbegin(), heatingSAFMethodChoicesUC.cend(),
                                   [&heatingSAFMethodUC](auto& s) { return s == heatingSAFMethodUC; });
            if (it == heatingSAFMethodChoicesUC.cend()) {
              LOG(Error, "For AirLoopHVACUnitarySystem '"
                           << object.nameString() << "', Unrecognized Supply Air Flow Method Rate During Heating Operation=" << *heatingSAFMethod);
            } else {
              auto dist = std::distance(heatingSAFMethodChoicesUC.cbegin(), it);
              const size_t index = heatingSAFMethodIndex + 1 + dist;
              if ((value = object.getString(index, false, true))) {
                newObject.setString(index, *value);
              } else {
                LOG(Error, "For AirLoopHVACUnitarySystem '" << object.nameString() << "', Supply Air Flow Method Rate During Heating Operation is '"
                                                            << *heatingSAFMethod << "' but associated field is empty. Setting it to zero.");
                newObject.setDouble(index, 0.0);
              }
            }
          }
        }

        if (!hasHeatingCoil && !hasCoolingCoil) {
          // TODO: not sure here
          newObject.setString(noCoolHeatSAFMethodIndex, "None");
        } else if (!noCoolHeatSAFMethod) {

          // Blank is equivalent to None here, no question
          newObject.setString(noCoolHeatSAFMethodIndex, "None");
#if 0
          // * 29 - Supply Air Flow Rate When No Cooling or Heating is Required
          // * 30 - Supply Air Flow Rate Per Floor Area When No Cooling or Heating is Required
          // * 31 - Fraction of Autosized Design Cooling Supply Air Flow Rate When No Cooling or Heating is Required
          // * 32 - Fraction of Autosized Design Heating Supply Air Flow Rate When No Cooling or Heating is Required
          // * 33 - Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation When No Cooling or Heating is Required
          // * 34 - Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation When No Cooling or Heating is Required
          bool found = false;
          for (size_t i = 0; i < noCoolHeatSAFMethodChoices.size(); ++i) {
            const size_t fieldIndex = noCoolHeatSAFMethodIndex + 1 + i;
            if ((value = object.getString(fieldIndex, false, true))) {
              newObject.setString(noCoolHeatSAFMethodIndex, std::string{noCoolHeatSAFMethodChoices[i]});
              newObject.setString(fieldIndex, *value);
              found = true;
              break;
            }
          }
          if (!found) {
            LOG(Warn, "For AirLoopHVACUnitarySystem '" << object.nameString()
                             << ", there is no Supply Air Flow Rate Method When No Cooling or Heating is Required and no flow rate fields set. "
                                "Defaulting to None");
            newObject.setString(noCoolHeatSAFMethodIndex, "None");
          }
#endif
        } else {
          std::string noCoolHeatSAFMethodUC = ascii_to_upper_copy(*noCoolHeatSAFMethod);
          if (noCoolHeatSAFMethodUC != "NONE") {
            auto it = std::find_if(noCoolHeatSAFMethodChoicesUC.cbegin(), noCoolHeatSAFMethodChoicesUC.cend(),
                                   [&noCoolHeatSAFMethodUC](auto& s) { return s == noCoolHeatSAFMethodUC; });
            if (it == noCoolHeatSAFMethodChoicesUC.cend()) {
              LOG(Error, "For AirLoopHVACUnitarySystem '"
                           << object.nameString()
                           << "', Unrecognized Supply Air Flow Rate Method When No Cooling or Heating is Required=" << *noCoolHeatSAFMethod);
            } else {
              auto dist = std::distance(noCoolHeatSAFMethodChoicesUC.cbegin(), it);
              const size_t index = noCoolHeatSAFMethodIndex + 1 + dist;
              if ((value = object.getString(index, false, true))) {
                newObject.setString(index, *value);
              } else {
                LOG(Error, "For AirLoopHVACUnitarySystem '" << object.nameString()
                                                            << "', Supply Air Flow Rate Method When No Cooling or Heating is Required is '"
                                                            << *noCoolHeatSAFMethod << "' but associated field is empty");
                newObject.setDouble(index, 0.0);
              }
            }
          }
        }

        ss << newObject;
        m_refactored.push_back(RefactoredObjectData(object, std::move(newObject)));

        auto it = CoilLatentTransitionInfo::findFromParent(coilTransitionInfos, object);
        if (it != coilTransitionInfos.end()) {
          if (it->isCurveCreationNeeded()) {
            IdfObject plfCurve = it->createCurveLinear(idd_3_7_0);
            ss << plfCurve;
            m_new.emplace_back(std::move(plfCurve));
          }
        }

      } else if (iddname == "OS:ZoneHVAC:WaterToAirHeatPump") {

        // Removed fields
        // 15 - Maximum Cycling Rate
        // 16 - Heat Pump Time Constant
        // 17 - Fraction of On-Cycle Power Use
        // 18 - Heat Pump Fan Delay Time

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 15) {
              newObject.setString(i, value.get());
            } else if (i > 18) {
              newObject.setString(i - 4, value.get());
            }
          }
        }

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

        auto it = CoilLatentTransitionInfo::findFromParent(coilTransitionInfos, object);
        if (it != coilTransitionInfos.end()) {
          if (it->isCurveCreationNeeded()) {
            IdfObject plfCurve = it->createCurveLinear(idd_3_7_0);
            ss << plfCurve;
            m_new.emplace_back(std::move(plfCurve));
          }
        }

      } else if (iddname == "OS:Coil:Cooling:DX:VariableSpeed") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Maximum Cycling Rate * 9
        // * Latent Capacity Time Constant * 10
        // * Fan Delay Time * 11
        // * Crankcase Heater Capacity Function of Temperature Curve Name - 17
        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 9) {
              newObject.setString(i, value.get());
            } else if (i < 14) {
              // * Energy Part Load Fraction Curve Name - 9 => 12
              newObject.setString(i + 3, value.get());
            } else {
              // * Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater Operation - 14 => 18
              newObject.setString(i + 4, value.get());
            }
          }
        }

        auto it = CoilLatentTransitionInfo::findFromCoolingCoil(coilTransitionInfos, object);
        const bool hasCoilInfo = (it != coilTransitionInfos.end());
        // No PLF Curve needed here

        double maxCyclingRate = 2.5;
        double heatPumpTimeConst = 60.0;
        double hpDelayTime = 60.0;
        if (hasCoilInfo) {
          maxCyclingRate = it->maxCyclingRate;
          heatPumpTimeConst = it->heatPumpTimeConst;
          hpDelayTime = it->hpDelayTime;
        }

        newObject.setDouble(9, maxCyclingRate);
        newObject.setDouble(10, heatPumpTimeConst);
        newObject.setDouble(11, hpDelayTime);

        // Cranckcase curve is optional
        // newObject.setString(17; "");

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Cooling:WaterToAirHeatPump:EquationFit") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Part Load Fraction Correlation Curve Name* 17
        // New at end
        // * Maximum Cycling Rate * 20
        // * Latent Capacity Time Constant * 21
        // * Fan Delay Time * 22

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 17) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        auto it = CoilLatentTransitionInfo::findFromCoolingCoil(coilTransitionInfos, object);
        const bool hasCoilInfo = (it != coilTransitionInfos.end());
        const std::string curveName = hasCoilInfo ? it->curveName() : fmt::format("{}-PLFCorrelationCurve", object.nameString());
        newObject.setString(17, curveName);

        // E+ 23.2.0 defaults Maximum Cycling Rate and Latent Capacity Time Constant to 0.0, we don't, cf #4999
        double maxCyclingRate = 2.5;
        double heatPumpTimeConst = 60.0;
        double hpDelayTime = 60.0;
        if (hasCoilInfo) {
          maxCyclingRate = it->maxCyclingRate;
          heatPumpTimeConst = it->heatPumpTimeConst;
          hpDelayTime = it->hpDelayTime;
        }

        newObject.setDouble(20, maxCyclingRate);
        newObject.setDouble(21, heatPumpTimeConst);
        newObject.setDouble(22, hpDelayTime);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

        if (!hasCoilInfo) {
          IdfObject plfCurve = CoilLatentTransitionInfo::defaultHeatPumpCoilPLFCorrelationCurve(idd_3_7_0, curveName);
          ss << plfCurve;
          m_new.emplace_back(std::move(plfCurve));
        }

      } else if (iddname == "OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Maximum Cycling Rate * 12
        // * Latent Capacity Time Constant * 13
        // * Fan Delay Time * 14

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 12) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 3, value.get());
            }
          }
        }

        // No Curve needed here
        auto it = CoilLatentTransitionInfo::findFromCoolingCoil(coilTransitionInfos, object);
        const bool hasCoilInfo = (it != coilTransitionInfos.end());

        // E+ 23.2.0 defaults Maximum Cycling Rate and Latent Capacity Time Constant to 0.0, we don't, cf #4999
        double maxCyclingRate = 2.5;
        double heatPumpTimeConst = 60.0;
        double hpDelayTime = 60.0;
        if (hasCoilInfo) {
          maxCyclingRate = it->maxCyclingRate;
          heatPumpTimeConst = it->heatPumpTimeConst;
          hpDelayTime = it->hpDelayTime;
        }

        newObject.setDouble(12, maxCyclingRate);
        newObject.setDouble(13, heatPumpTimeConst);
        newObject.setDouble(14, hpDelayTime);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:Coil:Heating:WaterToAirHeatPump:EquationFit") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Part Load Fraction Correlation Curve Name * 15

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        auto it = CoilLatentTransitionInfo::findFromHeatingCoil(coilTransitionInfos, object);
        const bool hasCoilInfo = (it != coilTransitionInfos.end());
        const std::string curveName = hasCoilInfo ? it->curveName() : fmt::format("{}-PLFCorrelationCurve", object.nameString());
        newObject.setString(15, curveName);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

        if (!hasCoilInfo) {
          IdfObject plfCurve = CoilLatentTransitionInfo::defaultHeatPumpCoilPLFCorrelationCurve(idd_3_7_0, curveName);
          ss << plfCurve;
          m_new.emplace_back(std::move(plfCurve));
        }

      } else if (iddname == "OS:Boiler:HotWater") {

        // 1 Field has been added from 3.6.1 to 3.7.0:
        // -------------------------------------------
        // * Off Cycle Parasitic Fuel Load * 16
        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 16) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // IDD 3.6.1 had a default of 0.0, so it'll pick that up if empty, and it matches the new Ctor value
        newObject.setDouble(15, object.getDouble(15, true).get());
        newObject.setDouble(16, 0.0);

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:DistrictHeating") {

        // Object was renamed from OS:DistrictHeating to OS:DistrictHeating:Water (since OS:DistrictHeating:Steam was added)

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Capacity Fraction Schedule * 5

        // We start by creating a new object, and copy every field.
        auto iddObject = idd_3_7_0.getObject("OS:DistrictHeating:Water");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        // Add the new "Capacity Fraction Schedule"
        newObject.setString(5, getOrCreateAlwaysOnContinuousSheduleHandleStr());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:DistrictCooling") {
        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Capacity Fraction Schedule * 5

        auto iddObject = idd_3_7_0.getObject("OS:DistrictCooling");
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }
        // Add the new "Capacity Fraction Schedule"
        newObject.setString(5, getOrCreateAlwaysOnContinuousSheduleHandleStr());

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Output:Meter") {

        std::string name = object.nameString();

        // Structured bindings
        for (const auto& [k, v] : meterFuelTypesMap) {
          name = boost::regex_replace(name, boost::regex(k, boost::regex::icase), v);
        }
        if (name == object.nameString()) {
          // No-op
          ss << object;
        } else {

          // Copy everything but 'Variable Name' field
          auto iddObject = idd_3_7_0.getObject(iddname);
          IdfObject newObject(iddObject.get());

          for (size_t i = 0; i < object.numFields(); ++i) {
            // Skip name field
            if (i == 1) {
              continue;
            }
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }

          newObject.setName(name);

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        }

      } else if ((iddname == "OS:Output:Variable") || (iddname == "OS:EnergyManagementSystem:Sensor")
                 || (iddname == "OS:EnergyManagementSystem:Actuator")) {

        unsigned variableNameIndex = 3;
        if (iddname == "OS:EnergyManagementSystem:Actuator") {
          variableNameIndex = 4;  // Actuated Component Control Type
        }

        if ((value = object.getString(variableNameIndex))) {

          std::string variableName = value.get();
          // Strip consecutive spaces and all
          variableName = boost::regex_replace(variableName, re_strip_multiple_spaces, " ");

          auto it = replaceOutputVariablesMap.find(variableName);
          if (it == replaceOutputVariablesMap.end()) {
            // No-op
            ss << object;
          } else {

            // Copy everything but 'Variable Name' field
            auto iddObject = idd_3_7_0.getObject(iddname);
            IdfObject newObject(iddObject.get());

            for (size_t i = 0; i < object.numFields(); ++i) {
              if (i == variableNameIndex) {
                continue;
              } else if ((value = object.getString(i))) {
                newObject.setString(i, value.get());
              }
            }

            LOG(Trace, "Replacing " << variableName << " with " << it->second << " for " << object.nameString());
            newObject.setString(variableNameIndex, it->second);

            ss << newObject;
            m_refactored.emplace_back(std::move(object), std::move(newObject));
          }
        } else {
          // No-op
          ss << object;
        }

      } else if (auto it = std::find_if(crankcaseCoilWithIndex.cbegin(), crankcaseCoilWithIndex.cend(),
                                        [&iddname](const auto& p) { return iddname == p.first; });
                 it != crankcaseCoilWithIndex.cend()) {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Crankcase Heater Capacity Function of Temperature Curve Name * (varies)

        const size_t insertionIndex = it->second;

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < insertionIndex) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        // Cranckcase curve is optional
        // newObject.setString(insertionIndex; "");

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (fuelTypeRenamesMap.find(iddname) != fuelTypeRenamesMap.end()) {
        LOG(Trace, "Checking for a fuel type rename in Object of type '" << iddname << "' and named '" << object.nameString() << "'");
        auto rangeFields = fuelTypeRenamesMap.equal_range(iddname);
        // First pass, find if a replacement is needed
        bool isReplaceNeeded = false;
        for (auto it = rangeFields.first; it != rangeFields.second; ++it) {
          if (checkIfReplaceNeeded(object, it->second)) {
            isReplaceNeeded = true;
            break;
          }
        }
        if (isReplaceNeeded) {
          LOG(Trace, "Replace needed!");

          // Make a new object, and copy evertything in place
          auto iddObject = idd_3_7_0.getObject(iddname);
          IdfObject newObject(iddObject.get());
          for (size_t i = 0; i < object.numFields(); ++i) {
            if ((value = object.getString(i))) {
              newObject.setString(i, value.get());
            }
          }

          // Then handle the renames
          for (auto it = rangeFields.first; it != rangeFields.second; ++it) {
            replaceForField(object, newObject, it->second);
          }

          ss << newObject;
          m_refactored.emplace_back(std::move(object), std::move(newObject));
        } else {
          // No-op
          ss << object;
        }

        //    } else if ((iddname == "OS:Coil:Heating:Gas") || (iddname == "OS:Coil:Heating:Gas:MultiStage")
        //               || (iddname == "OS:Coil:Heating:Gas:MultiStage:StageData") || (iddname == "OS:Coil:Heating:Desuperheater")) {
        //
        //      // No-op: only field name changes:
        //      // * Change Parasitic Electric Load => On Cycle Parasitic Electric Load
        //      // * Change Parasitic Gas Load => Off Cycle Parasitic Gas Load
        //      ss << object;

      } else if (iddname == "OS:Controller:OutdoorAir") {

        // 1 Field has been added from 3.6.1 to 3.7.0:
        // -------------------------------------------
        // * Economizer Operation Staging * 27
        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(27, "InterlockedWithMechanicalCooling");

        m_refactored.push_back(RefactoredObjectData(object, newObject));
        ss << newObject;

      } else if (iddname == "OS:LoadProfile:Plant") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Plant Loop Fluid Type * 7
        // * Degree of SubCooling * 8
        // * Degree of Loop SubCooling * 9
        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(7, "Water");
        newObject.setDouble(8, 5.0);
        newObject.setDouble(9, 20.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:HeatPump:PlantLoop:EIR:Cooling") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Control Type * 16
        // * Flow Mode * 17
        // * Minimum Part Load Ratio * 18
        // * Minimum Source Inlet Temperature * 19
        // * Maximum Source Inlet Temperature * 20
        // * Minimum Supply Water Temperature Curve Name * 21 --- Optional
        // * Maximum Supply Water Temperature Curve Name * 22 --- Optional

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(16, "Load");
        newObject.setString(17, "ConstantFlow");
        newObject.setDouble(18, 0.0);
        newObject.setDouble(19, -100.0);
        newObject.setDouble(20, 100.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:HeatPump:PlantLoop:EIR:Heating") {

        // Fields that have been added from 3.6.1 to 3.7.0:
        // ------------------------------------------------
        // * Heating To Cooling Capacity Sizing Ratio * 16
        // * Heat Pump Sizing Method * 17
        // * Control Type * 18
        // * Flow Mode * 19
        // * Minimum Part Load Ratio * 20
        // * Minimum Source Inlet Temperature * 21
        // * Maximum Source Inlet Temperature * 22
        // * Minimum Supply Water Temperature Curve Name * 23 --- Optional
        // * Maximum Supply Water Temperature Curve Name * 24 --- Optional
        // * Dry Outdoor Correction Factor Curve Name * 25
        // * Maximum Outdoor Dry Bulb Temperature For Defrost Operation * 26
        // * Heat Pump Defrost Control * 27
        // * Heat Pump Defrost Time Period Fraction * 28
        // * Defrost Energy Input Ratio Function of Temperature Curve Name * 29 --- Optional
        // * Timed Empirical Defrost Frequency Curve Name * 30 --- Optional
        // * Timed Empirical Defrost Heat Load Penalty Curve Name * 31 --- Optional
        // * Timed Empirical Defrost Heat Input Energy Fraction Curve Name * 32 --- Optional

        auto iddObject = idd_3_7_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }
        newObject.setDouble(16, 1.0);
        newObject.setString(17, "CoolingCapacity");
        newObject.setString(18, "Load");
        newObject.setString(19, "ConstantFlow");

        newObject.setDouble(20, 0.0);
        newObject.setDouble(21, -100.0);
        newObject.setDouble(22, 100.0);

        newObject.setDouble(26, 10.0);
        newObject.setString(27, "None");
        newObject.setDouble(28, 0.058333);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_6_1_to_3_7_0

  std::string VersionTranslator::update_3_7_0_to_3_8_0(const IdfFile& idf_3_7_0, const IddFileAndFactoryWrapper& idd_3_8_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_7_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_8_0.iddFile());
    ss << targetIdf.versionObject().get();

    constexpr std::array<int, 4> hx_old_100effectiveness_idxs{4, 5, 8, 9};
    constexpr std::array<int, 4> hx_new_effectiveness_curves_idxs{20, 21, 22, 23};
    constexpr std::array<std::string_view, 4> hx_new_table_names{"SensHeat", "LatHeat", "SensCool", "LatCool"};

    for (const IdfObject& object : idf_3_7_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:HeatExchanger:AirToAir:SensibleAndLatent") {

        // 4 Fields have been removed from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * Sensible Effectiveness at 75% Heating Air Flow {dimensionless} * 6
        // * Latent Effectiveness at 75% Heating Air Flow {dimensionless} * 7
        // * Sensible Effectiveness at 75% Cooling Air Flow {dimensionless} * 10
        // * Latent Effectiveness at 75% Cooling Air Flow {dimensionless} * 11

        // 4 Fields have been added from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * Sensible Effectiveness of Heating Air Flow Curve Name * 20
        // * Latent Effectiveness of Heating Air Flow Curve Name * 21
        // * Sensible Effectiveness of Cooling Air Flow Curve Name * 22
        // * Latent Effectiveness of Cooling Air Flow Curve Name * 23

        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else if (i < 8) {
              // no op
            } else if (i < 10) {
              newObject.setString(i - 2, value.get());
            } else if (i < 12) {
              // no op
            } else {
              newObject.setString(i - 4, value.get());
            }
          }
        }

        const std::string varListHandle = toString(createUUID());
        bool tableAdded = false;
        for (size_t i = 0; i < hx_old_100effectiveness_idxs.size(); ++i) {
          // Sensible/Latent Effectiveness at 100% Heating/Cooling Air Flow {dimensionless}
          if (auto e100 = object.getDouble(hx_old_100effectiveness_idxs[i])) {
            // Sensible/Latent Effectiveness at 75% Heating/Cooling Air Flow {dimensionless}

            if (auto e75 = object.getDouble(hx_old_100effectiveness_idxs[i] + 2)) {
              if (e100.get() != e75.get()) {
                tableAdded = true;

                IdfObject tableLookup(idd_3_8_0.getObject("OS:Table:Lookup").get());
                std::string uuid = toString(createUUID());
                tableLookup.setString(0, uuid);                                                                 // Handle
                tableLookup.setString(1, fmt::format("{}_{}Eff", object.nameString(), hx_new_table_names[i]));  // Name
                tableLookup.setString(2, varListHandle);                                                        // Independent Variable List Name
                tableLookup.setString(3, "DivisorOnly");                                                        // Normalization Method
                tableLookup.setDouble(4, e100.get());                                                           // Normalization Divisor
                tableLookup.setDouble(5, 0.0);                                                                  // Minimum Output
                tableLookup.setDouble(6, 10.0);                                                                 // Maximum Output
                tableLookup.setString(7, "Dimensionless");                                                      // Output Unit Type
                tableLookup.pushExtensibleGroup().setDouble(0, e75.get());                                      // Output Value 1
                tableLookup.pushExtensibleGroup().setDouble(0, e100.get());                                     // Output Value 2

                // Sensible/Latent Effectiveness of Heating/Cooling Air Flow Curve Name
                newObject.setString(hx_new_effectiveness_curves_idxs[i], uuid);

                ss << tableLookup;
                m_new.push_back(tableLookup);
              }
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(object, std::move(newObject));

        if (tableAdded) {
          IdfObject varList(idd_3_8_0.getObject("OS:ModelObjectList").get());
          varList.setString(0, varListHandle);
          varList.setString(1, object.nameString() + "_IndependentVariableList");  // Name

          IdfObject var(idd_3_8_0.getObject("OS:Table:IndependentVariable").get());
          std::string varHandle = toString(createUUID());
          var.setString(0, varHandle);
          varList.pushExtensibleGroup({varHandle});                        // Model Object 1
          var.setString(1, object.nameString() + "_IndependentVariable");  // Name
          var.setString(2, "Linear");                                      // Interpolation Method
          var.setString(3, "Linear");                                      // Extrapolation Method
          var.setDouble(4, 0.0);                                           // Minimum Value
          var.setDouble(5, 10.0);                                          // Maximum Value
          var.setString(7, "Dimensionless");                               // Unit Type
          var.pushExtensibleGroup().setDouble(0, 0.75);                    // Value 1
          var.pushExtensibleGroup().setDouble(0, 1.0);                     // Value 2

          ss << varList;
          m_new.emplace_back(std::move(varList));

          ss << var;
          m_new.emplace_back(std::move(var));
        }

      } else if ((iddname == "OS:ZoneHVAC:PackagedTerminalAirConditioner") || (iddname == "OS:ZoneHVAC:PackagedTerminalHeatPump")) {

        // 1 Field has been added from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * No Load Supply Air Flow Rate Control Set To Low Speed * 10
        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 10) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setString(10, "Yes");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:ZoneHVAC:WaterToAirHeatPump") {

        // 1 Field has been added from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * No Load Supply Air Flow Rate Control Set To Low Speed * 9
        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 9) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setString(9, "Yes");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:AirLoopHVAC:UnitarySystem") {

        // 1 Field has been added from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * No Load Supply Air Flow Rate Control Set To Low Speed * 35
        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 35) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setString(35, "Yes");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:People:Definition") {

        // 1 Key has been changed from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * Mean Radiant Temperature Calculation Type * 10
        //   * ZoneAveraged -> EnclosureAveraged

        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i == 10) {
              if (istringEqual(value.get(), "ZoneAveraged")) {
                newObject.setString(10, "EnclosureAveraged");
              } else {
                newObject.setString(10, value.get());
              }
            } else {
              newObject.setString(i, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Schedule:Day") {

        // 1 Field has been modified from 3.7.0 to 3.8.0:
        // ----------------------------------------------
        // * Interpolate to Timestep * 3 - Changed from bool to string choice

        auto iddObject = idd_3_8_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i == 3) {
              if (istringEqual(value.get(), "Yes")) {
                newObject.setString(3, "Average");
              } else {
                newObject.setString(3, "No");
              }
            } else {
              newObject.setString(i, value.get());
            }
          }
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_7_0_to_3_8_0

  std::string VersionTranslator::update_3_8_0_to_3_9_0(const IdfFile& idf_3_8_0, const IddFileAndFactoryWrapper& idd_3_9_0) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_8_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_9_0.iddFile());
    ss << targetIdf.versionObject().get();

    // ZoneHVAC:TerminalUnit:VariableRefrigerantFlow prescan
    // In E+ v24.2.0, we must change Fan:VariableVolume to Fan:SystemModel for Supply Air Fan Object Type / Name.
    std::vector<std::string> vrfFanVVHandleStrs;
    {
      std::vector<IdfObject> fanVVs = idf_3_8_0.getObjectsByType(idf_3_8_0.iddFile().getObject("OS:Fan:VariableVolume").get());
      std::vector<std::string> fanVVHandleStrs;
      fanVVHandleStrs.reserve(fanVVs.size());
      vrfFanVVHandleStrs.reserve(fanVVs.size());
      std::transform(fanVVs.cbegin(), fanVVs.cend(), std::back_inserter(fanVVHandleStrs),
                     [](const auto& idfObject) { return idfObject.getString(0).get(); });
      for (const auto& vrf : idf_3_8_0.getObjectsByType(idf_3_8_0.iddFile().getObject("OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow").get())) {
        if (auto fanHandleStr_ = vrf.getString(14, false, true)) {
          if (std::find(fanVVHandleStrs.cbegin(), fanVVHandleStrs.cend(), fanHandleStr_.get()) != fanVVHandleStrs.cend()) {
            vrfFanVVHandleStrs.emplace_back(std::move(*fanHandleStr_));
          }
        }
      }
    }

    for (const IdfObject& object : idf_3_8_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:Controller:OutdoorAir") {

        // 2 Fields have been made required from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * High Humidity Outdoor Air Flow Ratio * 24
        // * Control High Indoor Humidity Based on Outdoor Humidity Ratio * 25

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        if (newObject.isEmpty(24)) {
          newObject.setDouble(24, 1.0);
        }
        if (newObject.isEmpty(25)) {
          newObject.setString(25, "Yes");
        }

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:OutputControl:Files") {
        // 1 Field has been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Output Space Sizing * 9
        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 9) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setString(9, "Yes");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:HeatPump:PlantLoop:EIR:Heating") {

        // 3 Fields have been inserted from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Heat Recovery Inlet Node Name * 7
        // * Heat Recovery Outlet Node Name * 8
        // * Heat Recovery Reference Flow Rate * 12

        // 1 required Field has been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Minimum Heat Recovery Outlet Temperature * 36

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else if (i < 10) {
              newObject.setString(i + 2, value.get());
            } else {
              newObject.setString(i + 3, value.get());
            }
          }
        }

        newObject.setString(7, "");
        newObject.setString(8, "");
        newObject.setString(12, "Autosize");
        newObject.setDouble(36, 4.5);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:HeatPump:PlantLoop:EIR:Cooling") {

        // 3 Fields have been inserted from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Heat Recovery Inlet Node Name * 7
        // * Heat Recovery Outlet Node Name * 8
        // * Heat Recovery Reference Flow Rate * 12

        // 2 required Fields have been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Maximum Heat Recovery Outlet Temperature * 26
        // * Minimum Thermosiphon Minimum Temperature Difference * 30

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 7) {
              newObject.setString(i, value.get());
            } else if (i < 10) {
              newObject.setString(i + 2, value.get());
            } else {
              newObject.setString(i + 3, value.get());
            }
          }
        }

        newObject.setString(7, "");
        newObject.setString(8, "");
        newObject.setString(12, "Autosize");
        newObject.setDouble(26, 60.0);
        newObject.setDouble(30, 0.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:AirTerminal:SingleDuct:SeriesPIU:Reheat") {

        // 5 Fields have been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Fan Control Type * 16
        // * Minimum Fan Turn Down Ratio * 17
        // * Heating Control Type * 18
        // * Design Heating Discharge Air Temperature * 19
        // * High Limit Heating Discharge Air Temperature * 20

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 16) {
              newObject.setString(i, value.get());
            }
          }
        }

        newObject.setString(16, "ConstantSpeed");
        newObject.setDouble(17, 0.3);
        newObject.setDouble(19, 32.1);
        newObject.setDouble(20, 37.7);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:AirTerminal:SingleDuct:ParallelPIU:Reheat") {

        // 5 Fields have been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Fan Control Type * 17
        // * Minimum Fan Turn Down Ratio * 18
        // * Heating Control Type * 19
        // * Design Heating Discharge Air Temperature * 20
        // * High Limit Heating Discharge Air Temperature * 21

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 17) {
              newObject.setString(i, value.get());
            }
          }
        }

        newObject.setString(17, "ConstantSpeed");
        newObject.setDouble(18, 0.3);
        newObject.setDouble(20, 32.1);
        newObject.setDouble(21, 37.7);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Chiller:Electric:EIR") {

        // 3 required Fields has been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Condenser Flow Control * 35
        // * Condenser Minimum Flow Fraction * 38
        // * Thermosiphon Minimum Temperature Difference * 40

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(35, "ConstantFlow");
        newObject.setDouble(38, 0.2);
        newObject.setDouble(40, 0.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Chiller:Electric:ReformulatedEIR") {

        // 3 required Fields has been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Condenser Flow Control * 31
        // * Condenser Minimum Flow Fraction * 34
        // * Thermosiphon Minimum Temperature Difference * 36

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(31, "ConstantFlow");
        newObject.setDouble(34, 0.2);
        newObject.setDouble(36, 0.0);

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:Fan:VariableVolume") {
        std::string fanHandleStr = object.getString(0).get();
        if (std::find(vrfFanVVHandleStrs.cbegin(), vrfFanVVHandleStrs.cend(), fanHandleStr) == vrfFanVVHandleStrs.cend()) {
          ss << object;
        } else {
          IdfObject newObject(idd_3_9_0.getObject("OS:Fan:SystemModel").get());

          // Handle
          if ((value = object.getString(0))) {
            newObject.setString(0, value.get());
          }

          // Name
          if ((value = object.getString(1))) {
            newObject.setString(1, value.get());
          }

          // Availability Schedule Name
          if ((value = object.getString(2))) {
            newObject.setString(2, value.get());
          }

          // Fan Total Efficiency
          if ((value = object.getString(3, true))) {
            newObject.setString(15, value.get());
          }

          // Pressure Rise
          if ((value = object.getString(4, true))) {
            newObject.setString(8, value.get());
          }

          // Maximum Flow Rate
          if ((value = object.getString(5, true))) {
            newObject.setString(5, value.get());
          }

          // Motor Efficiency
          if ((value = object.getString(9, true))) {
            newObject.setString(9, value.get());
          }

          // Motor In Airstream Fraction
          if ((value = object.getString(10, true))) {
            newObject.setString(10, value.get());
          }

          // Nodes should be blank since it's inside a containing ZoneHVAC
          // Air Inlet Node Name
          if ((value = object.getString(16))) {
            newObject.setString(3, value.get());
          }

          // Air Outlet Node Name
          if ((value = object.getString(17))) {
            newObject.setString(4, value.get());
          }

          // End-Use Subcategory
          if ((value = object.getString(18, true))) {
            newObject.setString(21, value.get());
          }

          // Speed Control Method
          newObject.setString(6, "Continuous");

          // Electric Power Minimum Flow Rate Fraction
          newObject.setDouble(7, 0.0);

          // Design Electric Power Consumption
          newObject.setString(11, "Autosize");

          // Design Power Sizing Method
          newObject.setString(12, "TotalEfficiencyAndPressure");

          // Electric Power Per Unit Flow Rate (not used given Power Sizing Method, but required-field & set in Ctor)
          newObject.setDouble(13, 840.0);

          // Electric Power Per Unit Flow Rate Per Unit Pressure (not used given Power Sizing Method, but required-field & set in Ctor)
          newObject.setDouble(14, 1.66667);

          // OS:Curve:Quartic
          // Create the new Curve:Quartic object and populate with Fan Power Coefficient i
          IdfObject curveQuartic(idd_3_9_0.getObject("OS:Curve:Quartic").get());

          // Handle
          curveQuartic.setString(0, toString(createUUID()));

          // Name
          curveQuartic.setString(1, newObject.getString(1).get() + " Curve");

          // Coefficient1 Constant
          if ((value = object.getString(11, true))) {
            curveQuartic.setString(2, value.get());
          }

          // Coefficient2 x
          if ((value = object.getString(12, true))) {
            curveQuartic.setString(3, value.get());
          }

          // Coefficient3 x**2
          if ((value = object.getString(13, true))) {
            curveQuartic.setString(4, value.get());
          }

          // Coefficient4 x**3
          if ((value = object.getString(14, true))) {
            curveQuartic.setString(5, value.get());
          }

          // Coefficient5 x**4
          if ((value = object.getString(15, true))) {
            curveQuartic.setString(6, value.get());
          }

          // Minimum Value of x
          curveQuartic.setDouble(7, 0.0);

          // Maximum Value of x
          curveQuartic.setDouble(8, 1.0);

          // Minimum Curve Output
          curveQuartic.setDouble(9, 0.0);

          // Maximum Curve Output
          curveQuartic.setDouble(10, 5.0);

          // Input Unit Type for X
          curveQuartic.setString(11, "Dimensionless");

          // Output Unit Type
          curveQuartic.setString(12, "Dimensionless");

          ss << curveQuartic;
          m_new.push_back(curveQuartic);

          // Electric Power Function of Flow Fraction Curve Name
          newObject.setString(16, curveQuartic.getString(0).get());

          // Night Ventilation Mode Pressure Rise
          newObject.setString(17, "");

          // Night Ventilation Mode Flow Fraction
          newObject.setString(18, "");

          // Motor Loss Zone Name
          newObject.setString(19, "");

          // Motor Loss Radiative Fraction
          newObject.setDouble(20, 0.0);

          m_refactored.emplace_back(std::move(object), std::move(newObject));
          ss << newObject;
        }

      } else if (iddname == "OS:Sizing:Zone") {

        // 1 required Field has been added from 3.8.0 to 3.9.0:
        // ----------------------------------------------
        // * Sizing Option * 39

        auto iddObject = idd_3_9_0.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            newObject.setString(i, value.get());
          }
        }

        newObject.setString(39, "Coincident");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_8_0_to_3_9_0

  std::string VersionTranslator::update_3_9_0_to_3_9_1(const IdfFile& idf_3_9_0, const IddFileAndFactoryWrapper& idd_3_9_1) {
    std::stringstream ss;
    boost::optional<std::string> value;

    ss << idf_3_9_0.header() << '\n' << '\n';
    IdfFile targetIdf(idd_3_9_1.iddFile());
    ss << targetIdf.versionObject().get();

    for (const IdfObject& object : idf_3_9_0.objects()) {
      auto iddname = object.iddObject().name();

      if (iddname == "OS:WaterHeater:HeatPump") {

        // 1 Field has been inserted from 3.9.0 to 3.9.1:
        // ----------------------------------------------
        // * Tank Element Control Logic * 25
        auto iddObject = idd_3_9_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 25) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setString(25, "Simultaneous");

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

      } else if (iddname == "OS:GroundHeatExchanger:Vertical") {

        // 1 Field has been inserted from 3.9.0 to 3.9.1:
        // ----------------------------------------------
        // * Bore Hole Top Depth * 6

        auto iddObject = idd_3_9_1.getObject(iddname);
        IdfObject newObject(iddObject.get());

        for (size_t i = 0; i < object.numFields(); ++i) {
          if ((value = object.getString(i))) {
            if (i < 6) {
              newObject.setString(i, value.get());
            } else {
              newObject.setString(i + 1, value.get());
            }
          }
        }

        newObject.setDouble(6, 1.0);  // this value of 1 is what we previously had hardcoded in FT

        ss << newObject;
        m_refactored.emplace_back(std::move(object), std::move(newObject));

        // No-op
      } else {
        ss << object;
      }
    }

    return ss.str();

  }  // end update_3_9_0_to_3_9_1

}  // namespace osversion
}  // namespace openstudio
