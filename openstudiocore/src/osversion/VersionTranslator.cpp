/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/URLHelpers.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include "../utilities/math/FloatCompare.hpp"

#include <OpenStudio.hxx>

#include <QThread>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


namespace openstudio {
namespace osversion {

// Helper class
RefactoredObjectData::RefactoredObjectData(const IdfObject& oldObject, const IdfObject& newObject)
  : m_oldObject(oldObject), m_newObject(newObject) {}

IdfObject RefactoredObjectData::oldObject() const {
  return m_oldObject;
}

IdfObject RefactoredObjectData::newObject() const {
  return m_newObject;
}

VersionTranslator::VersionTranslator()
  : m_originalVersion("0.0.0"),
    m_allowNewerVersions(true)
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.osversion\\.VersionTranslator"));
  m_logSink.setThreadId(QThread::currentThread());

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
  m_updateMethods[VersionString("2.8.1")] = &VersionTranslator::defaultUpdate;

  // List of previous versions that may be updated to this one.
  //   - To increment the translator, add an entry for the version just released (branched for
  //     release).
  m_startVersions.push_back(VersionString("0.7.0"));
  m_startVersions.push_back(VersionString("0.7.1"));
  m_startVersions.push_back(VersionString("0.7.2"));
  m_startVersions.push_back(VersionString("0.7.3"));
  m_startVersions.push_back(VersionString("0.7.4"));
  m_startVersions.push_back(VersionString("0.7.5"));
  m_startVersions.push_back(VersionString("0.7.6"));
  m_startVersions.push_back(VersionString("0.8.0"));
  m_startVersions.push_back(VersionString("0.8.1"));
  m_startVersions.push_back(VersionString("0.8.2"));
  m_startVersions.push_back(VersionString("0.8.3"));
  m_startVersions.push_back(VersionString("0.8.4"));
  m_startVersions.push_back(VersionString("0.8.5"));
  m_startVersions.push_back(VersionString("0.9.0"));
  m_startVersions.push_back(VersionString("0.9.1"));
  m_startVersions.push_back(VersionString("0.9.2"));
  m_startVersions.push_back(VersionString("0.9.3"));
  m_startVersions.push_back(VersionString("0.9.4"));
  m_startVersions.push_back(VersionString("0.9.5"));
  m_startVersions.push_back(VersionString("0.9.6"));
  m_startVersions.push_back(VersionString("0.10.0"));
  m_startVersions.push_back(VersionString("0.10.1"));
  m_startVersions.push_back(VersionString("0.10.2"));
  m_startVersions.push_back(VersionString("0.10.3"));
  m_startVersions.push_back(VersionString("0.10.4"));
  m_startVersions.push_back(VersionString("0.10.5"));
  m_startVersions.push_back(VersionString("0.11.0"));
  m_startVersions.push_back(VersionString("0.11.1"));
  m_startVersions.push_back(VersionString("0.11.2"));
  m_startVersions.push_back(VersionString("0.11.3"));
  m_startVersions.push_back(VersionString("0.11.4"));
  m_startVersions.push_back(VersionString("0.11.5"));
  m_startVersions.push_back(VersionString("0.11.6"));
  m_startVersions.push_back(VersionString("1.0.0"));
  m_startVersions.push_back(VersionString("1.0.1"));
  m_startVersions.push_back(VersionString("1.0.2"));
  m_startVersions.push_back(VersionString("1.0.3"));
  m_startVersions.push_back(VersionString("1.0.4"));
  m_startVersions.push_back(VersionString("1.0.5"));
  m_startVersions.push_back(VersionString("1.0.6"));
  m_startVersions.push_back(VersionString("1.0.7"));
  m_startVersions.push_back(VersionString("1.1.0"));
  m_startVersions.push_back(VersionString("1.1.1"));
  m_startVersions.push_back(VersionString("1.1.2"));
  m_startVersions.push_back(VersionString("1.1.3"));
  m_startVersions.push_back(VersionString("1.2.0"));
  m_startVersions.push_back(VersionString("1.2.1"));
  m_startVersions.push_back(VersionString("1.2.2"));
  m_startVersions.push_back(VersionString("1.2.3"));
  m_startVersions.push_back(VersionString("1.2.4"));
  m_startVersions.push_back(VersionString("1.2.5"));
  m_startVersions.push_back(VersionString("1.3.0"));
  m_startVersions.push_back(VersionString("1.3.1"));
  m_startVersions.push_back(VersionString("1.3.2"));
  m_startVersions.push_back(VersionString("1.3.3"));
  m_startVersions.push_back(VersionString("1.3.4"));
  m_startVersions.push_back(VersionString("1.3.5"));
  m_startVersions.push_back(VersionString("1.4.0"));
  m_startVersions.push_back(VersionString("1.4.1"));
  m_startVersions.push_back(VersionString("1.4.2"));
  m_startVersions.push_back(VersionString("1.4.3"));
  m_startVersions.push_back(VersionString("1.5.0"));
  m_startVersions.push_back(VersionString("1.5.1"));
  m_startVersions.push_back(VersionString("1.5.2"));
  m_startVersions.push_back(VersionString("1.5.3"));
  m_startVersions.push_back(VersionString("1.5.4"));
  m_startVersions.push_back(VersionString("1.5.5"));
  m_startVersions.push_back(VersionString("1.6.0"));
  m_startVersions.push_back(VersionString("1.6.1"));
  m_startVersions.push_back(VersionString("1.6.2"));
  m_startVersions.push_back(VersionString("1.6.3"));
  m_startVersions.push_back(VersionString("1.7.0"));
  m_startVersions.push_back(VersionString("1.7.1"));
  m_startVersions.push_back(VersionString("1.7.2"));
  m_startVersions.push_back(VersionString("1.7.3"));
  m_startVersions.push_back(VersionString("1.7.4"));
  m_startVersions.push_back(VersionString("1.7.5"));
  m_startVersions.push_back(VersionString("1.8.0"));
  m_startVersions.push_back(VersionString("1.8.1"));
  m_startVersions.push_back(VersionString("1.8.2"));
  m_startVersions.push_back(VersionString("1.8.3"));
  m_startVersions.push_back(VersionString("1.8.4"));
  m_startVersions.push_back(VersionString("1.8.5"));
  m_startVersions.push_back(VersionString("1.9.0"));
  m_startVersions.push_back(VersionString("1.9.1"));
  m_startVersions.push_back(VersionString("1.9.2"));
  m_startVersions.push_back(VersionString("1.9.3"));
  m_startVersions.push_back(VersionString("1.9.4"));
  m_startVersions.push_back(VersionString("1.9.5"));
  m_startVersions.push_back(VersionString("1.10.0"));
  m_startVersions.push_back(VersionString("1.10.1"));
  m_startVersions.push_back(VersionString("1.10.2"));
  m_startVersions.push_back(VersionString("1.10.3"));
  m_startVersions.push_back(VersionString("1.10.4"));
  m_startVersions.push_back(VersionString("1.10.5"));
  m_startVersions.push_back(VersionString("1.10.6"));
  m_startVersions.push_back(VersionString("1.11.0"));
  m_startVersions.push_back(VersionString("1.11.1"));
  m_startVersions.push_back(VersionString("1.11.2"));
  m_startVersions.push_back(VersionString("1.11.3"));
  m_startVersions.push_back(VersionString("1.11.4"));
  m_startVersions.push_back(VersionString("1.11.5"));
  m_startVersions.push_back(VersionString("1.11.6"));
  m_startVersions.push_back(VersionString("1.12.0"));
  m_startVersions.push_back(VersionString("1.12.1"));
  m_startVersions.push_back(VersionString("1.12.2"));
  m_startVersions.push_back(VersionString("1.12.3"));
  m_startVersions.push_back(VersionString("1.12.4"));
  m_startVersions.push_back(VersionString("1.12.5"));
  m_startVersions.push_back(VersionString("1.12.6"));
  m_startVersions.push_back(VersionString("1.13.0"));
  m_startVersions.push_back(VersionString("1.13.1"));
  m_startVersions.push_back(VersionString("1.13.2"));
  m_startVersions.push_back(VersionString("1.13.3"));
  m_startVersions.push_back(VersionString("1.13.4"));
  m_startVersions.push_back(VersionString("1.14.0"));
  m_startVersions.push_back(VersionString("2.0.0"));
  m_startVersions.push_back(VersionString("2.0.1"));
  m_startVersions.push_back(VersionString("2.0.2"));
  m_startVersions.push_back(VersionString("2.0.3"));
  m_startVersions.push_back(VersionString("2.0.5"));
  m_startVersions.push_back(VersionString("2.1.0"));
  m_startVersions.push_back(VersionString("2.1.1"));
  m_startVersions.push_back(VersionString("2.1.2"));
  m_startVersions.push_back(VersionString("2.2.0"));
  m_startVersions.push_back(VersionString("2.2.1"));
  m_startVersions.push_back(VersionString("2.2.2"));
  m_startVersions.push_back(VersionString("2.3.0"));
  m_startVersions.push_back(VersionString("2.3.1"));
  m_startVersions.push_back(VersionString("2.4.0"));
  m_startVersions.push_back(VersionString("2.4.1"));
  m_startVersions.push_back(VersionString("2.4.2"));
  m_startVersions.push_back(VersionString("2.4.3"));
  m_startVersions.push_back(VersionString("2.5.0"));
  m_startVersions.push_back(VersionString("2.5.1"));
  m_startVersions.push_back(VersionString("2.5.2"));
  m_startVersions.push_back(VersionString("2.6.0"));
  m_startVersions.push_back(VersionString("2.6.1"));
  m_startVersions.push_back(VersionString("2.6.2"));
  m_startVersions.push_back(VersionString("2.7.0"));
  m_startVersions.push_back(VersionString("2.7.1"));
  m_startVersions.push_back(VersionString("2.7.2"));
  m_startVersions.push_back(VersionString("2.8.0"));
}

boost::optional<model::Model> VersionTranslator::loadModel(const openstudio::path& pathToOldOsm,
                                                           ProgressBar* progressBar)
{
  LOG(Trace,"Loading model from " << toString(pathToOldOsm) << ".");
  if (getFileExtension(pathToOldOsm) != modelFileExtension()) {
    LOG(Error,"Cannot loadModel for path'" << toString(pathToOldOsm)
        << "'. Extension must be '" << modelFileExtension() << "'. For '" <<
        componentFileExtension() << "'s use loadComponent.");
    return boost::none;
  }

  path wp = completePathToFile(pathToOldOsm,path(),modelFileExtension(),false);
  openstudio::filesystem::ifstream inFile(wp);
  if (inFile) {
    return loadModel(inFile,progressBar);
  }
  return boost::none;
}

boost::optional<model::Model> VersionTranslator::loadModel(std::istream& is,
                                                           ProgressBar* progressBar)
{
  return updateVersion(is, false, progressBar);
}

boost::optional<model::Model> VersionTranslator::loadModelFromString(const std::string& str,
                                                                     ProgressBar* progressBar)
{
  std::stringstream ss(str);
  return updateVersion(ss, false, progressBar);
}

boost::optional<model::Component> VersionTranslator::loadComponent(const openstudio::path& pathToOldOsc,
                                                                   ProgressBar* progressBar)
{
  LOG(Trace,"Loading component from " << toString(pathToOldOsc) << ".");
  if (getFileExtension(pathToOldOsc) != componentFileExtension()) {
    LOG(Error,"Cannot loadComponent for path'" << toString(pathToOldOsc)
        << "'. Extension must be '" << componentFileExtension()
        << "'. For '" << modelFileExtension() << "'s use loadModel.");
    return boost::none;
  }
  path wp = completePathToFile(pathToOldOsc,path(),componentFileExtension(),false);
  openstudio::filesystem::ifstream inFile(wp);
  if (inFile) {
    return loadComponent(inFile,progressBar);
  }

  return boost::none;
}

boost::optional<model::Component> VersionTranslator::loadComponent(std::istream& is,
                                                                   ProgressBar* progressBar)
{
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
  std::vector<LogMessage> result;
  for (LogMessage logMessage : m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }
  return result;
}

std::vector<LogMessage> VersionTranslator::errors() const {
  std::vector<LogMessage> result;
  for (LogMessage logMessage : m_logSink.logMessages()){
    if (logMessage.logLevel() > Warn){
      result.push_back(logMessage);
    }
  }
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

bool VersionTranslator::allowNewerVersions() const
{
  return m_allowNewerVersions;
}

void VersionTranslator::setAllowNewerVersions(bool allowNewerVersions)
{
  m_allowNewerVersions = allowNewerVersions;
}

boost::optional<model::Model> VersionTranslator::updateVersion(std::istream& is,
                                                               bool isComponent,
                                                               ProgressBar* progressBar) {
  m_originalVersion = VersionString("0.0.0");
  m_map.clear();
  m_logSink.setThreadId(QThread::currentThread());
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
  if (m_map.size() == 0u) {
    return boost::none;
  }

  if (progressBar){
    progressBar->setMinimum(0);
    progressBar->setMaximum(m_startVersions.size());
  }
  for (const VersionString& startVersion : m_startVersions) {
    if (progressBar){
      progressBar->setWindowTitle("Upgrading from " + startVersion.str());
      progressBar->setValue(progressBar->value()+1);
    }
    update(startVersion); // does nothing if no map entry
  }
  if (progressBar){
    progressBar->setValue(m_startVersions.size());
  }

  model::OptionalModel result;
  IdfFile finalModel = m_map[VersionString(openStudioVersion())];
  LOG(Debug,"Final model has " << finalModel.numObjects() << " objects in IDF form.");
  m_nObjectsFinalIdf = finalModel.numObjects();
  int numExpectedObjects = m_nObjectsStart + newObjects().size() - deprecatedObjects().size() - untranslatedObjects().size();
  if (m_nObjectsFinalIdf != numExpectedObjects) {
    LOG(Warn,"Expected final translated model to have " << numExpectedObjects
        << ", but it actually has " << m_nObjectsFinalIdf << " objects.");
    if ((m_nObjectsStart > 0) && (m_nObjectsFinalIdf == 0)) {
      LOG(Error,"Original model contained " << m_nObjectsStart
          << ", but final translated model is empty.");
      return boost::none;
    }
    numExpectedObjects = m_nObjectsFinalIdf;
  }

  // validity checking
  Workspace finalWorkspace(finalModel);
  model::Model tempModel(finalWorkspace); // None-level strictness!
  OS_ASSERT(tempModel.strictnessLevel() == StrictnessLevel::None);
  std::vector<std::shared_ptr<InterobjectIssueInformation> > issueInfo = fixInterobjectIssuesStage1(
      tempModel,
      m_originalVersion);
  if (!tempModel.isValid(StrictnessLevel::Draft)) {
    LOG(Error,"Model with Version " << openStudioVersion() << " IDD is not valid to draft "
        << "strictness level.");
    LOG(Error, tempModel.validityReport(StrictnessLevel::Draft));
    return boost::none;
  }
  bool test = tempModel.setStrictnessLevel(StrictnessLevel::Draft);
  OS_ASSERT(test);
  fixInterobjectIssuesStage2(tempModel,issueInfo);

  if (isComponent) {
    try {
      result = model::Component(tempModel.toIdfFile()); // includes name conflict fixes
    }
    catch (std::exception& e) {
      LOG(Error,"Could not translate component, because " << e.what());
    }
  } else {
    result = model::Model(tempModel.toIdfFile()); // includes name conflict fixes
  }

  if (result) {
    m_nObjectsFinalModel = result->numObjects();
    LOG(Debug,"Final model has " << m_nObjectsFinalModel << " objects in Model form.");
    if ((m_nObjectsStart > 0) && (m_nObjectsFinalModel == 0)) {
      LOG(Error,"Original model contained " << m_nObjectsStart
          << ", but final translated model is empty.");
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
  m_originalVersion = currentVersion; // save for user
  is.seekg(std::ios_base::beg); // prep to re-read file

  // bracket allowable versions
  LOG(Debug,"Starting translation from Version " << currentVersion.str() << ".");
  if (currentVersion < VersionString("0.7.0")) {
    LOG(Error,"Version translation is not provided for OpenStudio models created prior to "
        << "Version 0.7.0.");
    return;
  }
  if (currentVersion > VersionString(openStudioVersion())) {
    if (m_allowNewerVersions){
      // if currentVersion is just one ahead, may be a developer using the cloud.
      // let it pass as if currentVersion == openStudioVersion(), with a warning
      if (VersionString(openStudioVersion()).isNextVersion(currentVersion)) {
        LOG(Warn,"Version extracted from file '" << currentVersion.str() << "' is one "
            << "increment ahead of OpenStudio Version " << openStudioVersion() << ". "
            << "Proceeding as if these versions are the same. Use with caution.");
        currentVersion = VersionString(openStudioVersion());
      }
      else {
        // if currentVersion is farther ahead, log error and return nothing
        LOG(Error,"Version extracted from file '" << currentVersion.str()
            << "' is not supported by OpenStudio Version " << openStudioVersion()
            << ". Please check https://www.openstudio.net for updates.");
        return;
      }
    }else{
      // log error and return nothing
      LOG(Error,"Version extracted from file '" << currentVersion.str()
          << "' is newer than current OpenStudio Version " << openStudioVersion()
          << ". Please check https://www.openstudio.net for updates.");
      return;
    }
  }

  // load IdfFile with correct IddFile and save
  OptionalIdfFile oIdfFile;
  IddFileAndFactoryWrapper iddFile = getIddFile(currentVersion);
  if (iddFile.iddFileType() == IddFileType::UserCustom) {
    oIdfFile = IdfFile::load(is,iddFile.iddFile());
    if( currentVersion == VersionString(1,9,0) ) {
      if( oIdfFile ) {
        auto sizingObjects = oIdfFile->getObjectsByType(iddFile.getObject("OS:Sizing:Zone").get());

        // Figure out if the OS:Sizing:Zone object looks like it is from CBECC
        // it will have extra fields that would were not in the "real" 1.9.0 IDD
        bool fromCBECC = false;
        for( auto const & object : sizingObjects ) {
          if( auto value = object.getString(2) ) {
            if( istringEqual("SupplyAirTemperature", value.get()) ||
                istringEqual("TemperatureDifference", value.get()) ) {
              fromCBECC = true;
              break;
            }
          }
        }

        if( fromCBECC ) {
          // Remove the sizing objects so that the translation will proceed smoothly
          oIdfFile->removeObjects(sizingObjects);

          // Get a special CBECC idd file,
          // load the idf file again against that idd,
          // get the sizing objects and save them for later,
          // we will reintrodce the sizing objects in the version 1.10.2 phase of the translation
          // when they were officially part of OS
          auto cbeccIddFile = get_1_9_0_CBECC_IddFile();

          is.seekg(0, std::ios::beg);
          auto cbeccIdfFile = IdfFile::load(is,cbeccIddFile);
          OS_ASSERT(cbeccIdfFile);
          m_cbeccSizingObjects = cbeccIdfFile->getObjectsByType(cbeccIddFile.getObject("OS:Sizing:Zone").get());
        }

      }
    }
  }
  else {
    oIdfFile = IdfFile::load(is,iddFile.iddFileType());
  }
  if (!oIdfFile) {
    LOG(Error,"Unable to load Model with Version " << currentVersion.str() << " IDD.");
    return;
  }

  IdfFile idfFile = *oIdfFile;
  m_nObjectsStart = idfFile.numObjects();

  // DLM: would like to check validity here, can't due to bug with validity checking using custom idds

  m_map[currentVersion] = idfFile;
  LOG(Debug,"Initial model has " << idfFile.numObjects() << " objects.");
}

IddFileAndFactoryWrapper VersionTranslator::getIddFile(const VersionString& version) {
  IddFileAndFactoryWrapper result(IddFileType::OpenStudio);
  if (version < VersionString(openStudioVersion())) {
    OptionalIddFile iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio,version);
    if (!iddFile) {
      LOG_AND_THROW("Unable to retrieve OpenStudio Version " << version.str()
          << " IDD from the IddFactory.");
    }
    result = IddFileAndFactoryWrapper(*iddFile);
  }
  return result;
}

void VersionTranslator::update(const VersionString& startVersion) {
  std::map<VersionString, IdfFile>::const_iterator start = m_map.find(startVersion);
  if (start != m_map.end()) {

    std::string translatedIdf;
    VersionString lastVersion("0.0.0");
    boost::optional<IddFileAndFactoryWrapper> oIddFile;
    for (std::map<VersionString, OSVersionUpdater>::const_iterator it = m_updateMethods.begin(),
         itEnd = m_updateMethods.end(); it != itEnd; ++it)
    {
      // make sure map iteration is behaving as expected
      OS_ASSERT(lastVersion < it->first);
      lastVersion = it->first;
      if (startVersion < it->first) {
        oIddFile = getIddFile(it->first);
        translatedIdf = it->second(this,start->second,*oIddFile);
        break;
      }
    }

    if (translatedIdf.empty()) {
      LOG(Error,"Unable to complete translation from " << startVersion.str() << " to "
          << lastVersion.str() << ". Unable to find and execute the appropriate update method.");
      return;
    }
    std::stringstream ss(translatedIdf);
    OptionalIdfFile oIdfFile;
    if (oIddFile->iddFileType() == IddFileType::UserCustom) {
      oIdfFile = IdfFile::load(ss,oIddFile->iddFile());
    }
    else {
      oIdfFile = IdfFile::load(ss,oIddFile->iddFileType());
    }
    if (!oIdfFile) {
      LOG(Error,"Unable to complete translation from " << startVersion.str()
          << " to " << lastVersion.str() << ". Could not load translated IDF using the "
          << "latter version's IddFile. Translated text: " << std::endl << translatedIdf);
      return;
    }
    IdfFile idfFile = *oIdfFile;
    m_map[oIdfFile->version()] = idfFile;
    LOG(Debug,"Translation to " << lastVersion.str() << " model has " << oIdfFile->numObjects()
        << " objects.");
  }
}

std::string VersionTranslator::defaultUpdate(const IdfFile& idf,
                                             const IddFileAndFactoryWrapper& targetIdd)
{
  // use for version increments with no IDD changes
  std::stringstream ss;

  ss << idf.header() << std::endl << std::endl;

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

  ss << idf_0_7_1.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_7_2.iddFile());
  ss << targetIdf.versionObject().get();

  // all other objects
  for (const IdfObject& object : idf_0_7_1.objects()) {
    IdfObject toPrint = object;
    if (object.iddObject().name() == "OS:WeatherFile") {
      toPrint = updateUrlField_0_7_1_to_0_7_2(object,9);
    }
    else if (object.iddObject().name() == "OS:Construction:WindowDataFile") {
      toPrint = updateUrlField_0_7_1_to_0_7_2(object,1);
    }
    else if (object.iddObject().name() == "OS:Luminaire:Definition") {
      toPrint = updateUrlField_0_7_1_to_0_7_2(object,1);
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
      std::string transformed = toString(toURL(toPath(original)));
      if (!transformed.empty()) {
        IdfObject objCopy = object.clone();
        bool ok = objCopy.setString(index,transformed);
        OS_ASSERT(ok);
        result = objCopy;
      }
      else {
        LOG(Warn,"Tried to update the file path '" << original << "' to the new format, "
            << "but was unsuccessful.");
      }
    }
  }
  return result;
}

std::string VersionTranslator::update_0_7_2_to_0_7_3(const IdfFile& idf_0_7_2, const IddFileAndFactoryWrapper& idd_0_7_3) {
  // use for version increments with no IDD changes
  std::stringstream ss;

  ss << idf_0_7_2.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_7_3.iddFile());
  ss << targetIdf.versionObject().get();

  // all other objects
  for (const IdfObject& object : idf_0_7_2.objects()) {
    if (istringEqual(object.iddObject().name(),"OS:PlantLoop")) {
      // ETH@20120514 Kyle - Please refine/completely rework this as appropriate.
      LOG(Warn,"This model contains an out-of-date " << object.iddObject().name() << " object. "
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

  ss << idf_0_7_3.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_7_4.iddFile());
  ss << targetIdf.versionObject().get();

  // all other objects
  for (IdfObject object : idf_0_7_3.objects()) {
    if (istringEqual(object.iddObject().name(),"OS:ComponentData:Tags")) {
      m_deprecated.push_back(object);
      continue;
    }
    if (istringEqual(object.iddObject().name(),"OS:ComponentData:Attributes")) {
      m_deprecated.push_back(object);
      continue;
    }

    std::stringstream objectSS;
    object.printName(objectSS);
    std::string handleStr = toString(object.handle());
    objectSS << "  " << handleStr;
    if (object.numFields() == 0) {
      objectSS << ";";
    }
    else {
      objectSS << ",";
    }
    for (int cnt = handleStr.size(); cnt < fs; ++cnt) {
      objectSS << " ";
    }
    objectSS << "  " << componentDataIdf.fieldComment(0,true) << std::endl;

    if (istringEqual(object.iddObject().name(),"OS:ComponentData")) {
      // create new, refactored OS:ComponentData object from original data
      componentDataIdf = IdfObject(componentDataIdd);
      componentDataIdf.setName(object.name().get());
      // uuid
      OptionalString os = object.getString(1);
      if (os) {
        componentDataIdf.setString(2,*os);
      }
      else {
        LOG(Warn,"No value available for required " << object.iddObject().name()
            << " field " << object.iddObject().getField(1).get().name() << ".");
      }
      // version uuid
      os = object.getString(2);
      if (os) {
        componentDataIdf.setString(3,*os);
      }
      else {
        LOG(Warn,"No value available for required " << object.iddObject().name()
            << " field " << object.iddObject().getField(2).get().name() << ".");
      }
      // creation timestamp - leave blank because unknown
      // version timestamp - leave blank because unknown
      // object list
      StringVector handleForContents(1u);
      for (const IdfExtensibleGroup& eg : object.extensibleGroups()) {
        std::string typeStr = eg.getString(0).get();
        typeStr = boost::regex_replace(typeStr,boost::regex("_"),":");
        std::string nameStr = eg.getString(1).get();
        OptionalIddObjectType objTyp;
        try {
          objTyp = IddObjectType(typeStr);
        }
        catch (...) {}

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
            }
            else if (!istringEqual(it->iddObject().name(),typeStr)) {
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
              if (!it->getString(0,true) || (!istringEqual(nameStr,it->getString(0,true).get()))) {
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
            LOG(Warn,"Found match for object in OS:ComponentData contents list by type only, even "
                << "though this type of object (" << typeStr << ") has a name field.");
          }
        }

        // raise warnings and errors
        if (!match) {
          if (eg.groupIndex() == 0u) {
            LOG(Error,"Unable to locate primary object in contents list of " << std::endl
                << object << " Will throw this OS:ComponentData object out of the model.");
            break;
          }
          else {
            LOG(Warn,"Unable to locate object in OS:ComponentData contents list called out "
                << "as object type '" << typeStr << "', and with name '" << nameStr
                << "'. Skipping this object (that is, removing it from the Component "
                << "definition).");
            continue;
          }
        }
        if (candidates.size() > 1u) {
          LOG(Warn,"Ambiguous match for object in OS:ComponentData contents list called out "
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
          componentDataIdf.printField(objectSS,i,true);
        }
        else {
          componentDataIdf.printField(objectSS,i);
        }
      }
      m_refactored.push_back( RefactoredObjectData(object, componentDataIdf) );
    }
    else
    {
      // modify the object if necessary
      if (istringEqual(object.iddObject().name(),"OS:Connection")) {

        // increment inlet port as other object's fields have shifted
        boost::optional<int> portId = object.getInt(2);
        if( portId ){
          object.setInt(2, portId.get() + 1);
        }else{
          LOG(Warn, "OS:Connection object does not specify Inlet Port");

          // remove the object?
          //m_untranslated.push_back(object);
          //continue;
        }

        // increment outlet port as other object's fields have shifted
        portId = object.getInt(4);
        if( portId ){
          object.setInt(4, portId.get() + 1);
        }else{
          LOG(Warn, "OS:Connection object does not specify Outlet Port");

          // remove the object?
          //m_untranslated.push_back(object);
          //continue;
        }
      }

      // loop over all the fields
      for (unsigned i = 0, n = object.numFields(); i < n; ++i) {
        if (i == n - 1) {
          object.printField(objectSS,i,true);
        }
        else {
          object.printField(objectSS,i);
        }
      }
    }

    ss << objectSS.str();
  }

  return ss.str();
}

std::vector< std::shared_ptr<VersionTranslator::InterobjectIssueInformation> >
VersionTranslator::fixInterobjectIssuesStage1(model::Model& model,
                                              const VersionString& startVersion)
{
  OS_ASSERT(model.strictnessLevel() == StrictnessLevel::None);
  std::vector<std::shared_ptr<InterobjectIssueInformation> > result;

  if (startVersion < VersionString("0.8.4")) {
    std::shared_ptr<InterobjectIssueInformation> info = fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model);
    result.push_back(info);
  }

  return result;
}

void VersionTranslator::fixInterobjectIssuesStage2(
    model::Model& model,
    std::vector<std::shared_ptr<InterobjectIssueInformation> >& stage1Information)
{
  OS_ASSERT(model.strictnessLevel() == StrictnessLevel::Draft);

  for (std::shared_ptr<InterobjectIssueInformation>& info : stage1Information) {
    if (info->endVersion == VersionString("0.8.4")) {
      fixInterobjectIssuesStage2_0_8_3_to_0_8_4(model,info);
    }
  }
}

std::shared_ptr<VersionTranslator::InterobjectIssueInformation>
VersionTranslator::fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model::Model& model) {
  std::shared_ptr<InterobjectIssueInformation_0_8_3_to_0_8_4> result(
      new InterobjectIssueInformation_0_8_3_to_0_8_4());


  // deal with component data
  model::ComponentDataVector allComponentData = model.getConcreteModelObjects<model::ComponentData>();
  std::vector<IdfObject> allIdfComponentData; // make sure idf versions share data
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
        LOG(Debug,"Adding Schedule '" << schedule.name().get() << "' to the list of schedules that need fix-up.");
        result->schedules.push_back(schedule);
        result->users.push_back(users);
        // keep separate track of component data objects
        result->componentDataObjects.push_back(IdfObjectVector());
        IdfObjectVector thisSchedulesComponentData;
        for (int i = 0, n = allComponentData.size(); i < n; ++i) {
          model::ModelObjectVector objs = allComponentDataObjects[i];
          if (std::find(objs.begin(),objs.end(),schedule) != objs.end()) {
            thisSchedulesComponentData.push_back(allIdfComponentData[i]);
            found.insert(i);
          }
        }
        for (IdfObject& cd : thisSchedulesComponentData) {
          auto it = std::find_if(result->users.back().begin(),
                                 result->users.back().end(),
                                 std::bind(handleEquals<IdfObject,Handle>,std::placeholders::_1,cd.handle()));
          if (it != result->users.back().end()) {
            result->users.back().erase(it);
          }
          IdfObjectVector::const_iterator jit = std::find(allIdfComponentData.begin(),
                                                          allIdfComponentData.end(),
                                                          cd);
          OS_ASSERT(jit != allIdfComponentData.end());
          result->componentDataObjects.back().push_back(*jit);
        }

        std::vector< std::vector<unsigned> > thisSchedulesIndices;
        std::vector< std::vector<model::ScheduleTypeKey> > thisSchedulesKeys;
        auto it = result->users.back().begin();
        while (it != result->users.back().end()) {
          UnsignedVector thisUsersIndices = it->getSourceIndices(schedule.handle());
          std::vector<model::ScheduleTypeKey> thisUsersKeys = it->getScheduleTypeKeys(schedule);

          if (thisUsersKeys.empty()) {
            it = result->users.back().erase(it);
            continue;
          }

          OS_ASSERT(thisUsersIndices.size() == thisUsersKeys.size());
          for (unsigned index : thisUsersIndices) {
            it->setString(index,""); // clear for now--only change to be made at none strictness
          }
          thisSchedulesIndices.push_back(thisUsersIndices);
          thisSchedulesKeys.push_back(thisUsersKeys);
          if (std::find_if(result->originalUsers.begin(),
                           result->originalUsers.end(),
                           std::bind(handleEquals<IdfObject,Handle>,std::placeholders::_1,it->handle())) == result->originalUsers.end())
          {
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

void VersionTranslator::fixInterobjectIssuesStage2_0_8_3_to_0_8_4(
    model::Model& model,
    std::shared_ptr<InterobjectIssueInformation>& info)
{
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
            rulesetUsers[i].setPointer(indices[j],dayScheduleClone.handle());
          }
        }
      }
      for (auto & ruleUser : ruleUsers) {
        UnsignedVector indices = ruleUser.getSourceIndices(daySchedule.handle());
        unsigned j = 0;
        if (!primaryUser) {
          primaryUser = ruleUser;
          j = 1;
        }
        for (unsigned nj = indices.size(); j < nj; ++j) {
          model::ModelObject dayScheduleClone = daySchedule.clone();
          ruleUser.setPointer(indices[j],dayScheduleClone.handle());
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
    LOG(Debug,"Fixing up Schedule '" << schedule.name().get() << "'.");
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
          ok = isCompatible(keys[k].first,keys[k].second,*scheduleLimits);
        }
        if (ok) {
          ok = user.setPointer(indices[k],schedule.handle());
          OS_ASSERT(ok);
          okToModifySchedule = false;
        }
        else {
          for (model::Schedule& candidate : candidates) {
            if (model::OptionalScheduleTypeLimits limits = candidate.scheduleTypeLimits()) {
              if (isCompatible(keys[k].first,keys[k].second,*limits)) {
                user.setPointer(indices[k],candidate.handle());
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
              OS_ASSERT(ok); // unhooked schedule in Stage 1
            }
            ok = checkOrAssignScheduleTypeLimits(keys[k].first,keys[k].second,schedule);
            OS_ASSERT(ok);
            scheduleLimits = schedule.scheduleTypeLimits();
            if (model.numObjects() > modelN) {
              m_new.push_back(schedule.scheduleTypeLimits().get().idfObject());
            }
            okToModifySchedule = false;
            ok = user.setPointer(indices[k],schedule.handle());
            OS_ASSERT(ok);
          }
          else {
            model::Schedule clonedSchedule = schedule.clone().cast<model::Schedule>();
            ok = clonedSchedule.resetScheduleTypeLimits();
            OS_ASSERT(ok);
            modelN = model.numObjects();
            ok = checkOrAssignScheduleTypeLimits(keys[k].first,keys[k].second,clonedSchedule);
            OS_ASSERT(ok);
            if (model.numObjects() > modelN) {
              m_new.push_back(clonedSchedule.scheduleTypeLimits().get().idfObject());
            }
            ok = user.setPointer(indices[k],clonedSchedule.handle());
            candidates.push_back(clonedSchedule);
          }
        }
      } // for keys
    } // for users
    m_refactored.push_back(RefactoredObjectData(originalSchedule, schedule.idfObject()));
    for (const auto & candidate : candidates) {
      model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidate);
      m_new.push_back(candidate.idfObject());
      m_new.push_back(candidate.scheduleTypeLimits().get().idfObject());
      for (unsigned k = 1, nc = wholeCandidate.size(); k < nc; ++k) {
        m_new.push_back(wholeCandidate[k].idfObject());
      }
    }
    // start to fix-up component data--add objects
    std::set<model::ModelObject> usersSet(schedulesToFixup->users[i].begin(),schedulesToFixup->users[i].end());
    for (IdfObject& cd : schedulesToFixup->componentDataObjects[i]) {
      LOG(Debug,"Adding ScheduleTypeLimits '" << scheduleLimits->name().get() << "' to component data list.");
      cd.pushExtensibleGroup(StringVector(1u,toString(scheduleLimits->handle())));
      for (const auto & candidate : candidates) {
        model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidate);
        LOG(Debug,"Adding Schedule '" << candidate.name().get()
            << "', its ScheduleTypeLimits, and all children to component data list.");
        cd.pushExtensibleGroup(StringVector(1u,toString(candidate.handle())));
        cd.pushExtensibleGroup(StringVector(1u,toString(candidate.scheduleTypeLimits()->handle())));
        for (unsigned k = 1, nc = wholeCandidate.size(); k < nc; ++k) {
          cd.pushExtensibleGroup(StringVector(1u,toString(wholeCandidate[k].handle())));
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
  } // for schedules

  // clean up component data
  std::set<IdfObject,IdfObjectImplLess> uniqueComponentData;
  for (const std::vector<IdfObject>& scheduleCD : schedulesToFixup->componentDataObjects) {
    for (const IdfObject& cd : scheduleCD) {
      uniqueComponentData.insert(cd);
    }
  }
  IdfObjectVector uniqueComponentDataVector(uniqueComponentData.begin(),uniqueComponentData.end());
  for (IdfObject& cd : uniqueComponentDataVector) {
    // remove duplicates and objects not in model
    model::ModelObjectVector componentObjects;
    unsigned i = 0;
    while (i < cd.numExtensibleGroups()) {
      Handle h = toUUID(*cd.getExtensibleGroup(i).getString(0));
      if (std::find_if(componentObjects.begin(),
                       componentObjects.end(),
                       std::bind(handleEquals<model::ModelObject,Handle>,std::placeholders::_1,h)) != componentObjects.end())
      {
        cd.eraseExtensibleGroup(i);
        continue;
      }
      if (OptionalWorkspaceObject object = model.getObject(h)) {
        componentObjects.push_back(object->cast<model::ModelObject>());
      }
      else {
        cd.eraseExtensibleGroup(i);
        continue;
      }
      ++i;
    }

    // remove schedules that have no users in the component
    model::ModelObjectVector toRemove;
    std::sort(componentObjects.begin(),componentObjects.end(),IdfObjectImplLess());
    for (unsigned i = 0, n = componentObjects.size(); i < n; ++i) {
      if (model::OptionalSchedule schedule = componentObjects[i].optionalCast<model::Schedule>()) {
        if (i == 0) {
          continue;
        }
        model::ModelObjectVector users = castVector<model::ModelObject>(schedule->sources());
        std::sort(users.begin(),users.end(),IdfObjectImplLess());
        model::ModelObjectVector children = model::getRecursiveChildren(*schedule);
        std::sort(children.begin(),children.end(),IdfObjectImplLess());
        model::ModelObjectVector intersection(users.size(),componentObjects[0]), result(users.size(),componentObjects[0]);
        // intersect users and componentObjects
        auto intersectionEnd = std::set_intersection(
            users.begin(),users.end(),
            componentObjects.begin(),componentObjects.end(),
            intersection.begin(),IdfObjectImplLess());
        // subtract children from intersection
        auto resultEnd = std::set_difference(
            intersection.begin(),intersectionEnd,
            children.begin(),children.end(),
            result.begin(),IdfObjectImplLess());
        if (int(resultEnd - result.begin()) == 0) {
          toRemove.insert(toRemove.end(),children.begin(),children.end());
        }
      }
    }

    auto it = componentObjects.begin();
    i = 0;
    while (it != componentObjects.end()) {
      if (std::find(toRemove.begin(),toRemove.end(),*it) != toRemove.end()) {
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
        std::sort(users.begin(),users.end(),IdfObjectImplLess());
        model::ModelObjectVector intersection(users.size(),componentObjects[0]);
        auto intersectionEnd = std::set_intersection(
            componentObjects.begin(),componentObjects.end(),
            users.begin(),users.end(),
            intersection.begin(),IdfObjectImplLess());
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
  model::ModelObjectVector refactoredUsersVector(schedulesToFixup->refactoredUsers.begin(),schedulesToFixup->refactoredUsers.end());
  for (model::ModelObject& user : refactoredUsersVector) {
    IdfObjectVector::const_iterator it = std::find_if(schedulesToFixup->originalUsers.begin(),
                                                      schedulesToFixup->originalUsers.end(),
                                                      std::bind(handleEquals<IdfObject,Handle>,std::placeholders::_1,user.handle()));
    OS_ASSERT(it != schedulesToFixup->originalUsers.end());
    m_refactored.push_back( RefactoredObjectData(*it, user.idfObject()) );
  }

}

std::string VersionTranslator::update_0_9_1_to_0_9_2(const IdfFile& idf_0_9_1, const IddFileAndFactoryWrapper& idd_0_9_2)
{
  // use for version increments with no IDD changes
  std::stringstream ss;

  ss << idf_0_9_1.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_9_2.iddFile());
  ss << targetIdf.versionObject().get();

  // Fixup all thermal zone objects
  for (const IdfObject& object : idf_0_9_1.objects()) {
    if( object.iddObject().name() == "OS:ThermalZone" )
    {
      boost::optional<std::string> s;

      IdfObject newInletPortList(idd_0_9_2.getObject("OS:PortList").get());
      newInletPortList.setString(0,toString(createUUID()));

      IdfObject newExhaustPortList(idd_0_9_2.getObject("OS:PortList").get());
      newExhaustPortList.setString(0,toString(createUUID()));

      IdfObject newZoneHVACEquipmentList(idd_0_9_2.getObject("OS:ZoneHVAC:EquipmentList").get());

      IdfObject newThermalZone(idd_0_9_2.getObject("OS:ThermalZone").get());

      boost::optional<IdfObject> fanPowerTerminal;
      boost::optional<IdfObject> newFPTSecondaryInletConn;

      for( unsigned i = 0; i < object.numFields(); i++ )
      {
        s = object.getString(i);
        if( s )
        {
          switch (i)
          {
            case 0: // Handle
            {
              newZoneHVACEquipmentList.setString(2,s.get());

              newThermalZone.setString(i,s.get());

              break;
            }
            case 9: // Inlet Port List
            {
              newThermalZone.setString(i,newInletPortList.getString(0).get());

              newInletPortList.setString(2,s.get());

              // Find the connection object associated with the inlet port
              std::vector<IdfObject> connections = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Connection").get());
              for( auto & connection : connections )
              {
                if( connection.getString(0).get() == s.get() )
                {
                  // Fixup the connection's target object and port
                  connection.setString(4,newInletPortList.getString(0).get());
                  connection.setUnsigned(5,2);

                  // Find the inlet model object to the connection - assume it is a node
                  if( boost::optional<std::string> sourceObject1Handle = connection.getString(2) )
                  {
                    std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                    for( const auto & node : nodes )
                    {
                      if( sourceObject1Handle.get() == node.getString(0).get() )
                      {
                        // Find the connection object associated with the inlet node
                        if( boost::optional<std::string> nodeInletConnectionHandle = node.getString(2) )
                        {
                          for( const auto & connection2 : connections )
                          {
                            if( nodeInletConnectionHandle.get() == connection2.getString(0).get() )
                            {
                              // Find the inlet model object to connection2
                              if( boost::optional<std::string> sourceObject2Handle = connection2.getString(2) )
                              {
                                std::vector<IdfObject> objects = idf_0_9_1.objects();
                                for( const auto & object2 : objects )
                                {
                                  if( object2.getString(0).get() == sourceObject2Handle.get() )
                                  {
                                    if( object2.iddObject() != idf_0_9_1.iddFile().getObject("OS:AirLoopHVAC:ZoneSplitter").get() )
                                    {
                                      // If there is zone equipment add it to the zone equipment list
                                      newZoneHVACEquipmentList.setString(3,sourceObject2Handle.get());
                                      newZoneHVACEquipmentList.setUnsigned(4,1);
                                      newZoneHVACEquipmentList.setUnsigned(5,1);
                                    }

                                    if( object2.iddObject() == idf_0_9_1.iddFile().getObject("OS:AirTerminal:SingleDuct:ParallelPIU:Reheat").get() )
                                    {
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
            case 10: // Exhaust Port List
            {
              newThermalZone.setString(i,newExhaustPortList.getString(0).get());

              // Find the connection object associated with the exhaust port
              std::vector<IdfObject> connections = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Connection").get());
              for( auto & connection : connections )
              {
                if( connection.getString(0).get() == s.get() )
                {
                  // Find the target model object of the connection - assume it is a node
                  if( boost::optional<std::string> target1Handle = connection.getString(4) )
                  {
                    std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                    for( auto & node : nodes )
                    {
                      if( target1Handle.get() == node.getString(0).get() )
                      {
                        // Does the exhaust node have a outlet connection?
                        if( boost::optional<std::string> connection2Handle = node.getString(3) )
                        {
                          if( connection2Handle && connection2Handle.get() != "" )
                          {
                            // We fix up the connection to the thermal zone.
                            // Now actually it is a connection to the port list.
                            newExhaustPortList.setString(2,s.get());
                            connection.setString(2,newExhaustPortList.getString(0).get());
                            connection.setUnsigned(3,2);
                          }
                        }
                        else if( fanPowerTerminal )
                        {
                          newExhaustPortList.setString(2,s.get());
                          connection.setString(2,newExhaustPortList.getString(0).get());
                          connection.setUnsigned(3,2);

                          newFPTSecondaryInletConn = IdfObject(idd_0_9_2.getObject("OS:Connection").get());
                          newFPTSecondaryInletConn->setString(0,toString(createUUID()));

                          newFPTSecondaryInletConn->setString(2,node.getString(0).get());
                          newFPTSecondaryInletConn->setUnsigned(3,3);
                          newFPTSecondaryInletConn->setString(4,fanPowerTerminal->getString(0).get());
                          newFPTSecondaryInletConn->setUnsigned(5,8);

                          node.setString(3,newFPTSecondaryInletConn->getString(0).get());
                          fanPowerTerminal->setString(8,newFPTSecondaryInletConn->getString(0).get());
                        }
                        else
                        {
                          // Exhaust nodes don't exist by default anymore.
                          // They are only needed if there is zone equipment hooked up to them.
                          // We break the connection to the thermal zone.
                          connection.setString(2,"");
                          connection.setString(3,"");
                          connection.setString(4,"");
                          connection.setString(5,"");

                          node.setString(2,"");
                          node.setString(3,"");
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
            default:
            {
              newThermalZone.setString(i,s.get());
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

      if( newFPTSecondaryInletConn )
      {
        ss << newFPTSecondaryInletConn.get();
      }
    }
  }

  for (const IdfObject& object : idf_0_9_1.objects()) {
    if( object.iddObject().name() != "OS:ThermalZone" )
    {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_0_9_5_to_0_9_6(const IdfFile& idf_0_9_5, const IddFileAndFactoryWrapper& idd_0_9_6)
{
  // if multiple OS:RunPeriod objects remove them all
  bool skipRunPeriods = false;
  unsigned numRunPeriods = 0;
  for (const IdfObject& object : idf_0_9_5.objects())
  {
    if( object.iddObject().name() == "OS:RunPeriod" )
    {
      ++numRunPeriods;

      if (numRunPeriods > 1)
      {
        LOG(Warn, "Multiple OS:RunPeriod objects are no longer supported, these have been removed");
        skipRunPeriods = true;
        break;
      }
    }
  }

  // use for version increments with no IDD changes
  std::stringstream ss;

  ss << idf_0_9_5.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_9_6.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_0_9_5.objects()) {
    if( object.iddObject().name() == "OS:PlantLoop" )
    {
      IdfObject newSizingPlant(idd_0_9_6.getObject("OS:Sizing:Plant").get());

      newSizingPlant.setString(0,toString(createUUID()));

      newSizingPlant.setString(1,object.getString(0).get());

      newSizingPlant.setString(2,"Heating");

      newSizingPlant.setDouble(3,0.001);

      newSizingPlant.setDouble(4,0.001);

      ss << newSizingPlant;

      m_new.push_back(newSizingPlant);

      ss << object;
    }
    else if( object.iddObject().name() == "OS:Sizing:Parameters" )
    {
      IdfObject newSizingParameters = object.clone(true);

      if( ! object.getDouble(1) )
      {
        newSizingParameters.setDouble(1,1.25);
      }

      if( ! object.getDouble(2) )
      {
        newSizingParameters.setDouble(2,1.15);
      }

      ss << newSizingParameters;
    }
    else if( object.iddObject().name() == "OS:RunPeriod" )
    {
      if (skipRunPeriods){
        // put the object in the untranslated list
        m_untranslated.push_back(object);
      }
      else
      {
        ss << object;
      }
    }
    else
    {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_0_9_6_to_0_10_0(const IdfFile& idf_0_9_6, const IddFileAndFactoryWrapper& idd_0_10_0)
{
std::stringstream ss;

  ss << idf_0_9_6.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_10_0.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_0_9_6.objects()) {

    if( object.iddObject().name() == "OS:RadianceParameters" ) {
      boost::optional<std::string> value = object.getString(14);

      if (!value){
        ss << object;
      }else if (*value == "146" || *value == "581" || *value == "2321"){
        ss << object;
      } else {
        IdfObject newParameters = object.clone(true);
        newParameters.setString(14, "");
        m_refactored.push_back( RefactoredObjectData(object,  newParameters) );

        ss << newParameters;
      }
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_0_11_0_to_0_11_1(const IdfFile& idf_0_11_0, const IddFileAndFactoryWrapper& idd_0_11_1)
{
  // use for version increments with no IDD changes
  std::stringstream ss;

  ss << idf_0_11_0.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_11_1.iddFile());
  ss << targetIdf.versionObject().get();

  // hold OS:ComponentData objects for later
  std::vector<IdfObject> componentDataObjects;
  std::vector<std::string> removedComponentCostLineItemHandles;

  for (const IdfObject& object : idf_0_11_0.objects()) {
    if( object.iddObject().name() == "OS:ComponentCost:LineItem" )
    {
      removedComponentCostLineItemHandles.push_back(toString(object.handle()));
      m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:ComponentData" )
    {
      componentDataObjects.push_back(object);
    }
    else
    {
      ss << object;
    }
  }

  if (removedComponentCostLineItemHandles.size() > 0){
    LOG(Warn, "OS:ComponentCost:LineItem objects created before 0.11.1 are no longer supported, " << removedComponentCostLineItemHandles.size() << " objects have been removed.");
  }

  // remove these handles from any component data objects
  // DLM: this is probably a standard thing we want to do in every version translation function
  for (const IdfObject& componentDataObject : componentDataObjects) {

    // if object was primary component remove the component data completely
    boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
    if (primaryObjectHandle){
      if (std::find(removedComponentCostLineItemHandles.begin(), removedComponentCostLineItemHandles.end(), *primaryObjectHandle) != removedComponentCostLineItemHandles.end()){
        LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
        m_untranslated.push_back(componentDataObject);
        continue;
      }
    }

    // translate base fields
    componentDataObject.printName(ss,true);
    componentDataObject.printField(ss, 0, false); // Handle
    componentDataObject.printField(ss, 1, false); // Name
    componentDataObject.printField(ss, 2, false); // UUID
    componentDataObject.printField(ss, 3, false); // Version UUID
    componentDataObject.printField(ss, 4, false); // Creation Timestamp
    componentDataObject.printField(ss, 5, false); // Version Timestamp

    // make list of fields to keep
    std::vector<unsigned> extensibleIndicesToKeep;
    for(unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i){
      boost::optional<std::string> objectHandle = componentDataObject.getString(i);
      if (objectHandle){
        if (std::find(removedComponentCostLineItemHandles.begin(), removedComponentCostLineItemHandles.end(), *objectHandle) == removedComponentCostLineItemHandles.end()){
          extensibleIndicesToKeep.push_back(i);
        }
      }
    }

    // write out remaining fields
    for(std::vector<unsigned>::const_iterator it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it){
      if (it == itend-1){
        componentDataObject.printField(ss, *it, true);
      }else{
        componentDataObject.printField(ss, *it, false);
      }
    }

  }

  return ss.str();
}

std::string VersionTranslator::update_0_11_1_to_0_11_2(const IdfFile& idf_0_11_1, const IddFileAndFactoryWrapper& idd_0_11_2)
{
  // This version update has two things to do.
  // Make updates for new control related objects.
  // Make updates for component costs.

  std::stringstream ss;

  ss << idf_0_11_1.header() << std::endl << std::endl;

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

  if( ! m_isComponent )
  {
    // Add an always on discrete schedule if one does not already exist

    for (const IdfObject& object : idf_0_11_1.objects()) {
      if( object.iddObject().name() == "OS:Schedule:Constant" )
      {
        if( boost::optional<std::string> name = object.getString(1) )
        {
          if( istringEqual(name.get(),"Always On Discrete") )
          {
            if( boost::optional<double> value = object.getDouble(3) )
            {
              if( equal<double>(value.get(),1.0) )
              {
                alwaysOnSchedule = object;
              }
            }
          }
        }
      }
    }

    if( ! alwaysOnSchedule )
    {
      alwaysOnSchedule = IdfObject(idd_0_11_2.getObject("OS:Schedule:Constant").get());

      alwaysOnSchedule->setString(0,toString(createUUID()));

      alwaysOnSchedule->setString(1,"Always On Discrete");

      alwaysOnSchedule->setDouble(3,1.0);


      IdfObject typeLimits(idd_0_11_2.getObject("OS:ScheduleTypeLimits").get());

      typeLimits.setString(0,toString(createUUID()));

      typeLimits.setString(1,"Always On Discrete Limits");

      typeLimits.setDouble(2,0.0);

      typeLimits.setDouble(3,1.0);

      typeLimits.setString(4,"Discrete");

      typeLimits.setString(5,"Availability");

      alwaysOnSchedule->setString(2,typeLimits.getString(0).get());


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
    if( object.iddObject().name() == "OS:Controller:OutdoorAir" && ! m_isComponent )
    {
      IdfObject newOAController = object.clone(true);

      IdfObject newMechVentController(idd_0_11_2.getObject("OS:Controller:MechanicalVentilation").get());

      newMechVentController.setString(0,toString(createUUID()));

      newMechVentController.setString(4,"ZoneSum");

      newMechVentController.setString(2,alwaysOnSchedule->getString(0).get());

      newOAController.setString(20,newMechVentController.getString(0).get());


      ss << newOAController;

      ss << newMechVentController;

      m_new.push_back(newMechVentController);
    }
    else if( object.iddObject().name() == "OS:AirLoopHVAC" && ! m_isComponent )
    {
      IdfObject newAirLoopHVAC = object.clone(true);


      IdfObject newAvailList(idd_0_11_2.getObject("OS:AvailabilityManagerAssignmentList").get());

      newAvailList.setString(0,toString(createUUID()));

      newAirLoopHVAC.setString(3,newAvailList.getString(0).get());


      IdfObject newAvailabilityManagerScheduled(idd_0_11_2.getObject("OS:AvailabilityManager:Scheduled").get());

      newAvailabilityManagerScheduled.setString(0,toString(createUUID()));

      newAvailabilityManagerScheduled.setString(2,alwaysOnSchedule->getString(0).get());

      IdfExtensibleGroup eg = newAvailList.insertExtensibleGroup(0);

      eg.setString(0,newAvailabilityManagerScheduled.getString(0).get());


      IdfObject newAvailabilityManagerNightCycle(idd_0_11_2.getObject("OS:AvailabilityManager:NightCycle").get());

      newAvailabilityManagerNightCycle.setString(0,toString(createUUID()));

      eg = newAvailList.insertExtensibleGroup(1);

      eg.setString(0,newAvailabilityManagerNightCycle.getString(0).get());


      ss << newAirLoopHVAC;

      ss << newAvailList;

      ss << newAvailabilityManagerScheduled;

      ss << newAvailabilityManagerNightCycle;

      m_new.push_back(newAvailList);

      m_new.push_back(newAvailabilityManagerScheduled);

      m_new.push_back(newAvailabilityManagerNightCycle);
    }
    else if( object.iddObject().name() == "OS:ComponentCost:Adjustment" )
    {
      numComponentCostAdjustment += 1;

      // this was made unique, remove if more than 1
      if (numComponentCostAdjustment == 1){
        ss << object;
      }else{
        numComponentCostAdjustmentRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
      }
    }
    else if( object.iddObject().name() == "OS:ComponentCost:Reference" )
    {
        numComponentCostReferenceRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:LifeCycleCost:UseAdjustment" )
    {
        numLifeCycleCostUseAdjustmentRemoved += 1;
        removedItemHandles.push_back(toString(object.handle()));
        m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:LifeCycleCost:Parameters" )
    {
      object.printName(ss,true);
      object.printField(ss, 0, false); // Handle
      ss << "Custom, !- AnalysisType" << std::endl; // Name -> AnalysisType

      for(unsigned i = 2, imax = 12; i < imax; ++i){
        if (i == imax-1){
          object.printField(ss, i, true);
        }else{
          object.printField(ss, i, false);
        }
      }
    }
    else if( object.iddObject().name() == "OS:ComponentData" )
    {
      componentDataObjects.push_back(object);
    }
    else
    {
      ss << object;
    }
  }

  if (numComponentCostAdjustmentRemoved > 0){
    LOG(Warn, "Multiple OS:ComponentCost:Adjustment objects created in a single model before 0.11.1 are no longer supported, " << numComponentCostAdjustmentRemoved << " objects have been removed.");
  }
  if (numComponentCostReferenceRemoved > 0){
    LOG(Warn, "OS:ComponentCost:Reference objects created before 0.11.1 are no longer supported, " << numComponentCostReferenceRemoved << " objects have been removed.");
  }
  if (numLifeCycleCostUseAdjustmentRemoved > 0){
    LOG(Warn, "OS:LifeCycleCos:UseAdjustment objects created before 0.11.1 are no longer supported, " << numLifeCycleCostUseAdjustmentRemoved << " objects have been removed.");
  }

  // remove these handles from any component data objects
  // DLM: this is probably a standard thing we want to do in every version translation function
  for (const IdfObject& componentDataObject : componentDataObjects) {

    // if object was primary component remove the component data completely
    boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
    if (primaryObjectHandle){
      if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *primaryObjectHandle) != removedItemHandles.end()){
        LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
        m_untranslated.push_back(componentDataObject);
        continue;
      }
    }

    // translate base fields
    componentDataObject.printName(ss,true);
    componentDataObject.printField(ss, 0, false); // Handle
    componentDataObject.printField(ss, 1, false); // Name
    componentDataObject.printField(ss, 2, false); // UUID
    componentDataObject.printField(ss, 3, false); // Version UUID
    componentDataObject.printField(ss, 4, false); // Creation Timestamp
    componentDataObject.printField(ss, 5, false); // Version Timestamp

    // make list of fields to keep
    std::vector<unsigned> extensibleIndicesToKeep;
    for(unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i){
      boost::optional<std::string> objectHandle = componentDataObject.getString(i);
      if (objectHandle){
        if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *objectHandle) == removedItemHandles.end()){
          extensibleIndicesToKeep.push_back(i);
        }
      }
    }

    // write out remaining fields
    for(std::vector<unsigned>::const_iterator it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it){
      if (it == itend-1){
        componentDataObject.printField(ss, *it, true);
      }else{
        componentDataObject.printField(ss, *it, false);
      }
    }

  }

  return ss.str();
}


std::string VersionTranslator::update_0_11_4_to_0_11_5(const IdfFile& idf_0_11_4, const IddFileAndFactoryWrapper& idd_0_11_5)
{
  // Make updates for component costs.

  std::stringstream ss;

  ss << idf_0_11_4.header() << std::endl << std::endl;

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

    if( object.iddObject().name() == "OS:ComponentCost:LineItem" )
    {
      numComponentCostLineItemRemoved += 1;
      removedItemHandles.push_back(toString(object.handle()));
      m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:LifeCycleCost:NonrecurringCost" )
    {
      numLifeCycleNonRecurringCostRemoved += 1;
      removedItemHandles.push_back(toString(object.handle()));
      m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:LifeCycleCost:RecurringCosts" )
    {
      numLifeCycleRecurringCostRemoved += 1;
      removedItemHandles.push_back(toString(object.handle()));
      m_untranslated.push_back(object);
    }
    else if( object.iddObject().name() == "OS:ComponentData" )
    {
      componentDataObjects.push_back(object);
    }
    else
    {
      ss << object;
    }
  }


  if (numComponentCostLineItemRemoved > 0){
    LOG(Warn, "OS:ComponentCost:LineItem objects created before 0.11.5 are no longer supported, " << numComponentCostLineItemRemoved << " objects have been removed.");
  }
  if (numLifeCycleNonRecurringCostRemoved > 0){
    LOG(Warn, "OS:LifeCycleCost:NonrecurringCost objects created before 0.11.5 are no longer supported, " << numLifeCycleNonRecurringCostRemoved << " objects have been removed.");
  }
  if (numLifeCycleRecurringCostRemoved > 0){
    LOG(Warn, "OS:LifeCycleCost:RecurringCosts objects created before 0.11.5 are no longer supported, " << numLifeCycleRecurringCostRemoved << " objects have been removed.");
  }

  // remove these handles from any component data objects
  // DLM: this is probably a standard thing we want to do in every version translation function
  for (const IdfObject& componentDataObject : componentDataObjects) {

    // if object was primary component remove the component data completely
    boost::optional<std::string> primaryObjectHandle = componentDataObject.getString(6);
    if (primaryObjectHandle){
      if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *primaryObjectHandle) != removedItemHandles.end()){
        LOG(Warn, "Primary object removed from OS:ComponentData, component will be removed.");
        m_untranslated.push_back(componentDataObject);
        continue;
      }
    }

    // translate base fields
    componentDataObject.printName(ss,true);
    componentDataObject.printField(ss, 0, false); // Handle
    componentDataObject.printField(ss, 1, false); // Name
    componentDataObject.printField(ss, 2, false); // UUID
    componentDataObject.printField(ss, 3, false); // Version UUID
    componentDataObject.printField(ss, 4, false); // Creation Timestamp
    componentDataObject.printField(ss, 5, false); // Version Timestamp

    // make list of fields to keep
    std::vector<unsigned> extensibleIndicesToKeep;
    for(unsigned i = 6, imax = componentDataObject.numFields(); i < imax; ++i){
      boost::optional<std::string> objectHandle = componentDataObject.getString(i);
      if (objectHandle){
        if (std::find(removedItemHandles.begin(), removedItemHandles.end(), *objectHandle) == removedItemHandles.end()){
          extensibleIndicesToKeep.push_back(i);
        }
      }
    }

    // write out remaining fields
    for(std::vector<unsigned>::const_iterator it = extensibleIndicesToKeep.begin(), itend = extensibleIndicesToKeep.end(); it < itend; ++it){
      if (it == itend-1){
        componentDataObject.printField(ss, *it, true);
      }else{
        componentDataObject.printField(ss, *it, false);
      }
    }

  }

  return ss.str();
}

std::string VersionTranslator::update_0_11_5_to_0_11_6(const IdfFile& idf_0_11_5, const IddFileAndFactoryWrapper& idd_0_11_6)
{
  // Update the OS:PortList object to point back to the OS:ThermalZone

  std::stringstream ss;

  ss << idf_0_11_5.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_0_11_6.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_0_11_5.objects()) {

    if( object.iddObject().name() == "OS:ThermalZone" ) {

      boost::optional<std::string> inletPortListString = object.getString(9);
      boost::optional<std::string> exhaustPortListString = object.getString(10);

      for (const IdfObject& object2 : idf_0_11_5.objects()) {

        if( object2.iddObject().name() == "OS:PortList" ) {

          if( boost::optional<std::string> object2Handle = object2.getString(0) ) {

            if( (inletPortListString && object2Handle.get() == inletPortListString.get())
                ||
                (exhaustPortListString && object2Handle.get() == exhaustPortListString.get()) ) {

              // Make the required change to OS:PortList

              IdfObject newPortList(idd_0_11_6.getObject("OS:PortList").get());

              boost::optional<std::string> s;

              if( (s = object2.getString(0)) ) {
                newPortList.setString(0,s.get());
              }

              if( (s = object2.getString(1)) ) {
                newPortList.setString(1,s.get());
              }

              if( (s = object.getString(0)) ) {
                newPortList.setString(2,s.get());
              }

              for( unsigned i = 2; i < object2.numFields(); i++ ) {

                if( (s = object2.getString(i)) ) {
                  newPortList.setString(i + 1, s.get());
                }

              }

              m_refactored.push_back( RefactoredObjectData(object2, newPortList) );

              ss << newPortList;

            }

          }

        }

      }

      ss << object;

    } else if ( object.iddObject().name() == "OS:PortList" ) {

      continue;

    } else {

      ss << object;

    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_0_1_to_1_0_2(const IdfFile& idf_1_0_1, const IddFileAndFactoryWrapper& idd_1_0_2)
{
  std::stringstream ss;

  ss << idf_1_0_1.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_0_2.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_0_1.objects()) {

    if( object.iddObject().name() == "OS:Boiler:HotWater" ) {

      if(object.getString(15) && istringEqual(object.getString(15).get(),"VariableFlow")) {
        // Update Boiler Flow Mode

        IdfObject newBoiler = object.clone(true);

        newBoiler.setString(15,"LeavingSetpointModulated");

        m_refactored.push_back( RefactoredObjectData(object, newBoiler) );

        ss << newBoiler;

      } else {

        ss << object;

      }
    } else if( object.iddObject().name() == "OS:Boiler:HotWater" ) {

      if(object.getString(15) && istringEqual(object.getString(15).get(),"VariableFlow")) {
        // Update Chiller Flow Mode

        IdfObject newChiller = object.clone(true);

        newChiller.setString(15,"LeavingSetpointModulated");

        m_refactored.push_back( RefactoredObjectData(object, newChiller) );

        ss << newChiller;

      } else {

        ss << object;

      }

    } else {

      ss << object;

    }
  }

  return ss.str();
}


std::string VersionTranslator::update_1_0_2_to_1_0_3(const IdfFile& idf_1_0_2, const IddFileAndFactoryWrapper& idd_1_0_3)
{
  std::stringstream ss;

  ss << idf_1_0_2.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_0_3.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_0_2.objects()) {

    if( object.iddObject().name() == "OS:RadianceParameters" ) {
      boost::optional<std::string> value = object.getString(14);

      if (value && (*value == "581" || *value == "2321"))
      {
        IdfObject newParameters = object.clone(true);

        if (*value == "581")
        {
          newParameters.setString(14, "578");
        } else {
          newParameters.setString(14, "2306");
        }

        m_refactored.push_back( RefactoredObjectData(object,  newParameters) );

        ss << newParameters;
      } else {
        ss << object;
      }
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_2_2_to_1_2_3(const IdfFile& idf_1_2_2, const IddFileAndFactoryWrapper& idd_1_2_3)
{
  std::stringstream ss;

  ss << idf_1_2_2.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_2_3.iddFile());
  ss << targetIdf.versionObject().get();

  boost::optional<int> numberOfStories;
  boost::optional<int> numberOfAboveGroundStories;
  boost::optional<std::string> buildingTypeValue;
  boost::optional<IdfObject> buildingObject;

  for (const IdfObject& object : idf_1_2_2.objects()) {

    if( object.iddObject().name() == "OS:StandardsInformation:Construction" ) {
      boost::optional<std::string> value = object.getString(2); // Intended Surface Type

      if (value && (istringEqual(*value, "ExteriorFloor") || istringEqual(*value, "ExposedExteriorFloor"))){
        IdfObject newObject = object.clone(true);
        if (istringEqual(*value, "ExteriorFloor")){
          newObject.setString(2, "GroundContactFloor");
        }else {
          newObject.setString(2, "ExteriorFloor");
        }
        m_refactored.push_back( RefactoredObjectData(object,  newObject) );
        ss << newObject;
      } else {
        ss << object;
      }

    } else if( object.iddObject().name() == "OS:Building" ) {

      buildingObject = object;

    } else if( object.iddObject().name() == "OS:StandardsInformation:Building" ) {

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

  if (buildingObject){
    boost::optional<IddObject> buildingIdd = idd_1_2_3.getObject("OS:Building");
    OS_ASSERT(buildingIdd);
    IdfObject newBuildingObject(*buildingIdd);

    // Handle
    boost::optional<std::string> s = buildingObject->getString(0);
    if (s){
      bool test = newBuildingObject.setString(0, *s);
      OS_ASSERT(test);
    }

    // Name
    s = buildingObject->getString(1);
    if (s){
      bool test = newBuildingObject.setString(1, *s);
      OS_ASSERT(test);
    }

    // Building Sector Type
    s = buildingObject->getString(2);
    if (s){
      bool test = newBuildingObject.setString(2, *s);
      OS_ASSERT(test);
    }

    // North Axis
    s = buildingObject->getString(3);
    if (s){
      bool test = newBuildingObject.setString(3, *s);
      OS_ASSERT(test);
    }

    // Nominal Floor to Floor Height
    s = buildingObject->getString(4);
    if (s){
      bool test = newBuildingObject.setString(4, *s);
      OS_ASSERT(test);
    }

    // Space Type Name
    s = buildingObject->getString(5);
    if (s){
      bool test = newBuildingObject.setString(5, *s);
      OS_ASSERT(test);
    }

    // Default Construction Set Name
    s = buildingObject->getString(6);
    if (s){
      bool test = newBuildingObject.setString(6, *s);
      OS_ASSERT(test);
    }

    // Default Schedule Set Name
    s = buildingObject->getString(7);
    if (s){
      bool test = newBuildingObject.setString(7, *s);
      OS_ASSERT(test);
    }

    // Standards Number of Stories
    if (numberOfStories){
      bool test = newBuildingObject.setInt(8, *numberOfStories);
      OS_ASSERT(test);
    }

    /// Standards Number of Above Ground Stories
    if (numberOfAboveGroundStories){
      bool test = newBuildingObject.setInt(9, *numberOfAboveGroundStories);
      OS_ASSERT(test);
    }

    // Standards Building Type
    if (buildingTypeValue){
      bool test = newBuildingObject.setString(10, *buildingTypeValue);
      OS_ASSERT(test);
    }

    m_refactored.push_back( RefactoredObjectData(*buildingObject, newBuildingObject) );
    ss << newBuildingObject;
  }

  return ss.str();
}

std::string VersionTranslator::update_1_3_4_to_1_3_5(const IdfFile& idf_1_3_4, const IddFileAndFactoryWrapper& idd_1_3_5)
{
  std::stringstream ss;

  ss << idf_1_3_4.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_3_5.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_3_4.objects()) {

    if( object.iddObject().name() == "OS:Refrigeration:WalkIn" ) {

      IdfObject newWalkin = object.clone(true);

      boost::optional<std::string> s = object.getString(22);
      if (s){
        bool test = newWalkin.setString(22, "");
        OS_ASSERT(test);
        test = newWalkin.setString(23, *s);
        OS_ASSERT(test);
      }

      m_refactored.push_back( RefactoredObjectData(object, newWalkin) );

      ss << newWalkin;

    } else {

      ss << object;

    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_5_3_to_1_5_4(const IdfFile& idf_1_5_3, const IddFileAndFactoryWrapper& idd_1_5_4)
{
  std::stringstream ss;

  ss << idf_1_5_3.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_5_4.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_5_3.objects()) {
    if (object.iddObject().name() == "OS:TimeDependentValuation")
    {
      // put the object in the untranslated list
      m_untranslated.push_back(object);
    } else {
      ss << object;

    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_7_1_to_1_7_2(const IdfFile& idf_1_7_1, const IddFileAndFactoryWrapper& idd_1_7_2)
{
  std::stringstream ss;

  ss << idf_1_7_1.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_7_2.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_7_1.objects()) {
    if (object.iddObject().name() == "OS:EvaporativeCooler:Direct:ResearchSpecial") {
      auto iddObject = idd_1_7_2.getObject("OS:EvaporativeCooler:Direct:ResearchSpecial");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < 10; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }

      auto d = object.getDouble(4);
      if( ! d ) {
        newObject.setString(4,"Autosize");
      }
      newObject.setDouble(11,0.1);

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (object.iddObject().name() == "OS:EvaporativeCooler:Indirect:ResearchSpecial") {
      auto iddObject = idd_1_7_2.getObject("OS:EvaporativeCooler:Indirect:ResearchSpecial");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < 19; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }

      auto d = object.getDouble(5);
      if( ! d ) {
        newObject.setString(5,"Autosize");
      }
      newObject.setDouble(22,0.1);
      newObject.setDouble(24,1.0);

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_7_4_to_1_7_5(const IdfFile& idf_1_7_4, const IddFileAndFactoryWrapper& idd_1_7_5)
{
  std::stringstream ss;

  ss << idf_1_7_4.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_7_5.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_7_4.objects()) {
    if (object.iddObject().name() == "OS:Sizing:System") {
      auto iddObject = idd_1_7_5.getObject("OS:Sizing:System");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < 21; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }

      newObject.setDouble(21,1.0);
      newObject.setDouble(22,9.9676501E-3);
      newObject.setDouble(23,1.0);
      newObject.setDouble(24,3.9475456E-5);
      newObject.setDouble(25,9.9676501E-3);
      newObject.setDouble(26,1.0);
      newObject.setDouble(27,1.0);
      newObject.setDouble(28,3.1588213E-5);
      newObject.setString(29,"CoolingDesignCapacity");
      newObject.setString(30,"Autosize");
      newObject.setDouble(31,234.7);
      newObject.setDouble(32,1.0);
      newObject.setString(33,"HeatingDesignCapacity");
      newObject.setString(34,"Autosize");
      newObject.setDouble(35,157.0);
      newObject.setDouble(36,1.0);
      newObject.setString(37,"OnOff");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if(object.iddObject().name() == "OS:Sizing:Plant") {
      auto iddObject = idd_1_7_5.getObject("OS:Sizing:Plant");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < 5; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }

      newObject.setString(5,"NonCoincident");
      newObject.setInt(6,1);
      newObject.setString(7,"None");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if(object.iddObject().name() == "OS:DistrictCooling") {
      IdfObject newObject = object.clone(true);

      if( ! object.getDouble(4) ) {
        newObject.setString(4,"Autosize");
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if(object.iddObject().name() == "OS:DistrictHeating") {
      IdfObject newObject = object.clone(true);

      if( ! object.getDouble(4) ) {
        newObject.setString(4,"Autosize");
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if(object.iddObject().name() == "OS:Humidifier:Steam:Electric") {
      IdfObject newObject = object.clone(true);

      if( ! object.getDouble(3) ) {
        newObject.setString(4,"Autosize");
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_8_3_to_1_8_4(const IdfFile& idf_1_8_3, const IddFileAndFactoryWrapper& idd_1_8_4)
{
  std::stringstream ss;

  ss << idf_1_8_3.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_8_4.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_8_3.objects()) {
    auto iddname = object.iddObject().name();
    if (iddname == "OS:PlantLoop") {
      auto iddObject = idd_1_8_4.getObject("OS:PlantLoop");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < 4; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }

      for( size_t i = 5; i < 23; ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(i + 2,s.get());
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:AirLoopHVAC") {
      auto iddObject = idd_1_8_4.getObject("OS:AirLoopHVAC");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      // Handle
      if( auto value = object.getString(0) ) {
        newObject.setString(0,value.get());
      }

      // Name
      if( auto value = object.getString(1) ) {
        newObject.setString(1,value.get());
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
      newObject.setString(3,toString(availabilitySchedule->handle()));

      // Availability Manager
      if( controlType && (istringEqual("CycleOnAny",controlType.get()) || istringEqual("CycleOnControlZone",controlType.get()) || istringEqual("CycleOnAnyZoneFansOnly",controlType.get())) ) {
        newObject.setString(4,toString(availabilityManagerNightCycle->handle()));
      }

      // All the remaining fields are unchanged but shifted down one to account for new field
      for( size_t i = 4; i < 15; ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(i + 1,value.get());
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if(iddname == "OS:AvailabilityManager:Scheduled") {
      m_deprecated.push_back(object);
    } else if(iddname == "OS:AvailabilityManagerAssignmentList") {
      m_deprecated.push_back(object);
    } else if(iddname == "OS:AvailabilityManager:NightCycle") {
      auto controlType = object.getString(4);
      if( controlType && (istringEqual("CycleOnAny",controlType.get()) || istringEqual("CycleOnControlZone",controlType.get()) || istringEqual("CycleOnAnyZoneFansOnly",controlType.get())) ) {
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

std::string VersionTranslator::update_1_8_4_to_1_8_5(const IdfFile& idf_1_8_4, const IddFileAndFactoryWrapper& idd_1_8_5)
{
  std::stringstream ss;

  ss << idf_1_8_4.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_8_5.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_8_4.objects()) {
    auto iddname = object.iddObject().name();
    if (iddname == "OS:SetpointManager:Scheduled") {
      if( (! object.getString(2)) || object.getString(2).get().empty()  ) {
        auto iddObject = idd_1_8_5.getObject("OS:SetpointManager::Scheduled");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for( size_t i = 0; i < 5; ++i ) {
          if( i == 2 ) {
            newObject.setString(i,"Temperature");
          } else if( auto s = object.getString(i) ) {
            newObject.setString(i,s.get());
          }
        }
        ss << newObject;
      } else {
        ss << object;
      }
    } else if (iddname == "OS:PlantLoop") {
      if( (! object.getString(20)) || object.getString(20).get().empty()  ) {
        auto iddObject = idd_1_8_5.getObject("OS:PlantLoop");
        OS_ASSERT(iddObject);
        IdfObject newObject(iddObject.get());

        for( size_t i = 0; i < 25; ++i ) {
          if( i == 20 ) {
            newObject.setString(i,"Optimal");
          } else if( auto s = object.getString(i) ) {
            newObject.setString(i,s.get());
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

std::string VersionTranslator::update_1_8_5_to_1_9_0(const IdfFile& idf_1_8_5, const IddFileAndFactoryWrapper& idd_1_9_0)
{
  std::stringstream ss;

  ss << idf_1_8_5.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_9_0.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_8_5.objects()) {
    auto iddname = object.iddObject().name();
    if (iddname == "OS:SpaceInfiltration:EffectiveLeakageArea") {
      auto iddObject = idd_1_9_0.getObject("OS:SpaceInfiltration:EffectiveLeakageArea");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( i == 4 ) {
          if( auto value = object.getDouble(i) ) {
            newObject.setDouble(i,convert(value.get(),"m^2","cm^2").get());
          }
        } else if( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }
      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_9_2_to_1_9_3(const IdfFile& idf_1_9_2, const IddFileAndFactoryWrapper& idd_1_9_3)
{
  std::stringstream ss;

  ss << idf_1_9_2.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_9_3.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_9_2.objects()) {
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
      m_refactored.push_back(RefactoredObjectData(object, newObject));

    }else if (iddname == "OS:ZoneAirMassFlowConservation") {
      auto iddObject = idd_1_9_3.getObject("OS:ZoneAirMassFlowConservation");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      if (auto value = object.getString(0)) { // Handle -> Handle
        newObject.setString(0, value.get());
      }
      if (auto value = object.getString(1)) { // Adjust Zone Mixing For Zone Air Mass Flow Balance -> Adjust Zone Mixing For Zone Air Mass Flow Balance
        newObject.setString(1, value.get());
      } else{
        // old default was Yes, new default is No
      }
      if (auto value = object.getString(2)) { // Source Zone Infiltration Treatment -> Infiltration Balancing Method
        // old default was AddInfiltrationFlow, new default is AddInfiltrationFlow
        // old keys AddInfiltrationFlow, AdjustInfiltrationFlow
        // new keys AddInfiltrationFlow, AdjustInfiltrationFlow, None
        newObject.setString(2, value.get());
      }
      // new field Infiltration Balancing Zones is defaulted to MixingSourceZonesOnly
      ss << newObject;
      m_refactored.push_back(RefactoredObjectData(object, newObject));
    }else if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
      auto iddObject = idd_1_9_3.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(18,"No");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:AirTerminal:SingleDuct:VAV:NoReheat") {
      auto iddObject = idd_1_9_3.getObject("OS:AirTerminal:SingleDuct:VAV:NoReheat");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(10,"No");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_9_4_to_1_9_5(const IdfFile& idf_1_9_4, const IddFileAndFactoryWrapper& idd_1_9_5)
{
  std::stringstream ss;

  ss << idf_1_9_4.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_9_5.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_9_4.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:Controller:WaterCoil") {
      auto iddObject = idd_1_9_5.getObject("OS:Controller:WaterCoil");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0,j = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(j,value.get());
        }
        if( i == 1 ) {
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
      if( auto actuatorNodeHandle = object.getString(6) ) {
        auto coilHandle = [&](const IddObject & type, unsigned waterInletIndex){
          boost::optional<std::string> result;
          auto coils = idf_1_9_4.getObjectsByType(type);
          for( const auto & coil : coils ) {
            // waterInletConnection will be a handle to a connection object
            if( auto waterInletConnectionHandle = coil.getString(waterInletIndex) ) {
              if( auto waterInletConnection = idf_1_9_4.getObject(toUUID(waterInletConnectionHandle.get()))) {
                if( auto sourceHandle = waterInletConnection->getString(2) ) {
                  if( sourceHandle.get() == actuatorNodeHandle.get() ) {
                    result = toString(coil.handle());
                    break;
                  }
                }
              }
            }
          }
          return result;
        };
        if( auto handle = coilHandle(idf_1_9_4.iddFile().getObject("OS:Coil:Cooling:Water").get(),10) ) {
          newObject.setString(2,handle.get());
        } else if( auto handle = coilHandle(idf_1_9_4.iddFile().getObject("OS:Coil:Heating:Water").get(),5) ) {
          newObject.setString(2,handle.get());
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_9_5_to_1_10_0(const IdfFile& idf_1_9_5, const IddFileAndFactoryWrapper& idd_1_10_0)
{
  std::stringstream ss;

  ss << idf_1_9_5.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_10_0.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_9_5.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
      auto iddObject = idd_1_10_0.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(i,value.get());
        }
      }

      // This is a redo because some models didn't get updated by the logic in
      // 1.9.2 -> 1.9.3
      {
        auto controlForOA = object.getString(18);
        if( (! controlForOA) || controlForOA->empty() ) {
          newObject.setString(18,"No");
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:AirTerminal:SingleDuct:VAV:NoReheat") {
      auto iddObject = idd_1_10_0.getObject("OS:AirTerminal:SingleDuct:VAV:NoReheat");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto value = object.getString(i) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(10,"No");
      // This is a redo because some models didn't get updated by the logic in
      // 1.9.2 -> 1.9.3
      {
        auto controlForOA = object.getString(10);
        if( (! controlForOA) || controlForOA->empty() ) {
          newObject.setString(10,"No");
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_10_1_to_1_10_2(const IdfFile& idf_1_10_1, const IddFileAndFactoryWrapper& idd_1_10_2) {

  std::stringstream ss;

  ss << idf_1_10_1.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_10_2.iddFile());
  ss << targetIdf.versionObject().get();

  auto zones = idf_1_10_1.getObjectsByType(idf_1_10_1.iddFile().getObject("OS:ThermalZone").get());

  for (const IdfObject& object : idf_1_10_1.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:ThermostatSetpoint:DualSetpoint") {
      // Get all of the zones that point to this thermostat
      std::vector<IdfObject> referencingZones;
      for( const auto & zone : zones ) {
        if( auto thermostatHandle = zone.getString(19) ) {
          if( toUUID(thermostatHandle.get()) == object.handle() ) {
            referencingZones.push_back(zone);
          }
        }
      }

      // Clone the thermostat for every zone after the first
      if( referencingZones.size() > 1u ) {
        for( auto & referencingZone : referencingZones ) {
          // This will leave the original thermostate hanging out alone in most circumstances
          // but since we are messing with the name it is probably best
          auto newThermostat = object.clone();
          newThermostat.setName(referencingZone.nameString() + " Thermostat");
          ss << newThermostat;
          m_new.push_back(newThermostat);
          auto newHandle = newThermostat.getString(0).get();
          referencingZone.setString(19,newHandle);
        }
      }
      ss << object;
    } else if (iddname == "OS:Sizing:Zone") {
      auto iddObject = idd_1_10_2.getObject("OS:Sizing:Zone");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( i == 2 ) {
          newObject.setString(newi,"SupplyAirTemperature");
          ++newi;
          if( auto value = object.getString(i) ) {
            newObject.setString(newi,value.get());
          }
          ++newi;
          newObject.setDouble(newi,11.11);
        } else if( i == 3 ) {
          newObject.setString(newi,"SupplyAirTemperature");
          ++newi;
          if( auto value = object.getString(i) ) {
            newObject.setString(newi,value.get());
          }
          ++newi;
          newObject.setDouble(newi,11.11);
        } else if( auto value = object.getString(i) ) {
          newObject.setString(newi,value.get());
        }
        ++newi;
      }

      newObject.setString(24,"No");
      newObject.setString(25,"NeutralSupplyAir");
      newObject.setString(26,"Autosize");
      newObject.setString(27,"Autosize");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  // Reintroduce m_cbeccSizingObjects
  for( auto const & sizingObject : m_cbeccSizingObjects ) {
    auto iddObject = idd_1_10_2.getObject("OS:Sizing:Zone");
    OS_ASSERT(iddObject);
    IdfObject newObject(iddObject.get());

    for( size_t i = 0; i < sizingObject.numNonextensibleFields(); ++i ) {
      if( auto value = sizingObject.getString(i) ) {
        newObject.setString(i,value.get());
      }
    }
    newObject.setString(24,"No");
    newObject.setString(25,"NeutralSupplyAir");
    newObject.setString(26,"Autosize");
    newObject.setString(27,"Autosize");

    m_new.push_back( newObject );
    ss << newObject;
  }

  return ss.str();
}

std::string VersionTranslator::update_1_10_5_to_1_10_6(const IdfFile& idf_1_10_5, const IddFileAndFactoryWrapper& idd_1_10_6) {
  std::stringstream ss;

  ss << idf_1_10_5.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_10_6.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_10_5.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:PlantLoop") {
      auto iddObject = idd_1_10_6.getObject("OS:PlantLoop");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
         if( auto s = object.getString(i) ) {
           if( i == 20 ) {
             if( istringEqual("Sequential",s.get()) ) {
               newObject.setString(i,"SequentialLoad");
             } else if( istringEqual("Uniform",s.get()) ) {
               newObject.setString(i,"UniformLoad");
             } else {
               newObject.setString(i,s.get());
             }
           } else {
             newObject.setString(i,s.get());
           }
        } else {

        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_11_3_to_1_11_4(const IdfFile& idf_1_11_3, const IddFileAndFactoryWrapper& idd_1_11_4) {
  std::stringstream ss;

  ss << idf_1_11_3.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_11_4.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_11_3.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:Coil:Heating:Water:Baseboard") {
      auto iddObject = idd_1_11_4.getObject("OS:Coil:Heating:Water:Baseboard");
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(newi,s.get());
          if( i == 1 ) {
            newi = newi + 4;
          }
        }
        ++newi;
      }

      newObject.setString(2,"HeatingDesignCapacity");
      newObject.setString(3,"Autosize");
      newObject.setDouble(4,0.0);
      newObject.setDouble(5,0.8);

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_11_4_to_1_11_5(const IdfFile& idf_1_11_4, const IddFileAndFactoryWrapper& idd_1_11_5) {
  std::stringstream ss;

  ss << idf_1_11_4.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_11_5.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_11_4.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:Coil:Heating:DX:SingleSpeed") {
      auto iddObject = idd_1_11_5.getObject("OS:Coil:Heating:DX:SingleSpeed");
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {

        if( i == 6 ) {
          newObject.setDouble(i,773.3);
          newi = i + 1;
        } else if (i > 6) {
          newi = i + 1;
        } else {
          newi = i;
        }

        if( auto s = object.getString(i) ) {
          newObject.setString(newi,s.get());
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_12_0_to_1_12_1(const IdfFile& idf_1_12_0, const IddFileAndFactoryWrapper& idd_1_12_1) {
  std::stringstream ss;

  ss << idf_1_12_0.header() << std::endl << std::endl;

  // new version object
  IdfFile targetIdf(idd_1_12_1.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_12_0.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:Meter") {
      auto iddObject = idd_1_12_1.getObject("OS:Output:Meter");
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(newi,s.get());
        }
        ++newi;
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_1_12_3_to_1_12_4(const IdfFile& idf_1_12_3, const IddFileAndFactoryWrapper& idd_1_12_4) {
  std::stringstream ss;

  ss << idf_1_12_3.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_1_12_4.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_1_12_3.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:AirTerminal:SingleDuct:VAV:Reheat") {
      auto iddObject = idd_1_12_4.getObject("OS:AirTerminal:SingleDuct:VAV:Reheat");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if ( auto s = object.getString(i) ) {
          if( i == 14u ) {
            if ( istringEqual("Reverse",s.get()) ) {
              newObject.setString(i,"ReverseWithLimits");
            } else {
              newObject.setString(i,s.get());
            }
          } else {
            newObject.setString(i,s.get());
          }
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_2_1_0_to_2_1_1(const IdfFile& idf_2_1_0, const IddFileAndFactoryWrapper& idd_2_1_1) {
  std::stringstream ss;

  ss << idf_2_1_0.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_1_1.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_2_1_0.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:WaterHeater:Stratified") {
      auto iddObject = idd_2_1_1.getObject("OS:WaterHeater:Stratified");
      IdfObject newObject(iddObject.get());

      size_t oldi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (i == 64u) || (i == 65u) ) {
          newObject.setDouble(i,0.0);
        } else {
          if ( auto s = object.getString(oldi) ) {
            newObject.setString(i,s.get());
          }
          oldi++;
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Heating") {
      auto iddObject = idd_2_1_1.getObject("OS:HeatPump:WaterToWater:EquationFit:Heating");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if ( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }
      newObject.setDouble(20,7.5);
      newObject.setDouble(21,1.0);
      newObject.setString(22,"");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:HeatPump:WaterToWater:EquationFit:Cooling") {
      auto iddObject = idd_2_1_1.getObject("OS:HeatPump:WaterToWater:EquationFit:Cooling");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if ( auto s = object.getString(i) ) {
          newObject.setString(i,s.get());
        }
      }
      newObject.setDouble(20,8.0);
      newObject.setDouble(21,1.0);
      newObject.setString(22,"");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_2_1_1_to_2_1_2(const IdfFile& idf_2_1_1, const IddFileAndFactoryWrapper& idd_2_1_2) {
  std::stringstream ss;

  ss << idf_2_1_1.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_1_2.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_2_1_1.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:PlantLoop") {
      auto iddObject = idd_2_1_2.getObject("OS:PlantLoop");
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( i == 3u ) {
          newObject.setInt(newi,0);
          ++newi;
        } else {
          if( auto s = object.getString(i) ) {
            newObject.setString(newi,s.get());
          }
        }
        ++newi;
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:ZoneHVAC:FourPipeFanCoil") {
      auto iddObject = idd_2_1_2.getObject("OS:ZoneHVAC:FourPipeFanCoil");
      IdfObject newObject(iddObject.get());

      size_t newi = 0;
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( auto s = object.getString(i) ) {
          newObject.setString(newi,s.get());
        }
        ++newi;
      }
      newObject.setString(23,"Autosize");
      newObject.setString(24,"Autosize");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else {
      ss << object;
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_2_3_0_to_2_3_1(const IdfFile& idf_2_3_0, const IddFileAndFactoryWrapper& idd_2_3_1) {
  std::stringstream ss;

  ss << idf_2_3_0.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_3_1.iddFile());
  ss << targetIdf.versionObject().get();

  boost::optional<std::string> value;

  for (const IdfObject& object : idf_2_3_0.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:Pump:ConstantSpeed") {
      auto iddObject = idd_2_3_1.getObject("OS:Pump:ConstantSpeed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(16,"PowerPerFlowPerPressure");
      newObject.setString(17,"348701.1");
      newObject.setString(18,"1.282051282");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:Pump:VariableSpeed") {
      auto iddObject = idd_2_3_1.getObject("OS:Pump:VariableSpeed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(25,"0.5");
      newObject.setString(26,"PowerPerFlowPerPressure");
      newObject.setString(27,"348701.1");
      newObject.setString(28,"1.282051282");
      newObject.setString(29,"0.0");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:CoolingTower:SingleSpeed") {
      auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:SingleSpeed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(29,"0.1");
      newObject.setString(30,"0.1");
      newObject.setString(31,"1.25");
      newObject.setString(32,"0.1");
      newObject.setString(33,"35.0");
      newObject.setString(34,"25.6");
      newObject.setString(35,"Autosize");
      newObject.setString(36,"Autosize");
      newObject.setString(37,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:CoolingTower:TwoSpeed") {
      auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:TwoSpeed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(41,"35.0");
      newObject.setString(42,"25.6");
      newObject.setString(43,"Autosize");
      newObject.setString(44,"Autosize");
      newObject.setString(45,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:CoolingTower:VariableSpeed") {
      auto iddObject = idd_2_3_1.getObject("OS:CoolingTower:VariableSpeed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      newObject.setString(31,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if (iddname == "OS:Chiller:Electric:EIR") {
      auto iddObject = idd_2_3_1.getObject("OS:Chiller:Electric:EIR");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      if( object.getString(17) && (! object.getString(17).get().empty()) ) {
        newObject.setString(19,"WaterCooled");
      } else {
        newObject.setString(19,"AirCooled");
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;
    } else if (iddname == "OS:AirLoopHVAC") {
      auto iddObject = idd_2_3_1.getObject("OS:AirLoopHVAC");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      /* We need handle the change from the old behavior (one AvailabilityManager:xxx) to the new AVMList
       * First, we must create an AVMList for each loop, then if an AVM was assigned, we put it on the AVM list
       */

      // Create an AVMList with a handle and a name
      std::string  avmName = "";
      if( auto loopName = object.getString(1) ) {
        avmName = loopName.get();
      }
      avmName = avmName + " AvailabilityManagerAssignmentList";

      IdfObject avmList(idd_2_3_1.getObject("OS:AvailabilityManagerAssignmentList").get());
      std::string avmHandle = toString(createUUID());
      avmList.setString(0,avmHandle);
      avmList.setString(1,avmName);

      // Check if there was an AVM assigned to the Loop, it so, put it on the newly created AVMList
      if (auto existingAVMUUid = object.getString(4)) {
        avmList.pushExtensibleGroup(StringVector(1u, existingAVMUUid.get()));
      }

      // Assign AVM list to loop
      newObject.setString(4,avmHandle);

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      m_new.push_back(avmList);

      ss << newObject;
      ss << avmList;

    } else if (iddname == "OS:PlantLoop") {
      auto iddObject = idd_2_3_1.getObject("OS:PlantLoop");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      // Create an AVMList with a handle and a name
      std::string  avmName = "";
      if( auto loopName = object.getString(1) ) {
        avmName = loopName.get();
      }
      avmName = avmName + " AvailabilityManagerAssignmentList";

      IdfObject avmList(idd_2_3_1.getObject("OS:AvailabilityManagerAssignmentList").get());
      std::string avmHandle = toString(createUUID());
      avmList.setString(0,avmHandle);
      avmList.setString(1,avmName);

      // Check if there was an AVM assigned to the Loop, it so, put it on the newly created AVMList
      if (auto existingAVMUUid = object.getString(22)) {
        avmList.pushExtensibleGroup(StringVector(1u, existingAVMUUid.get()));
      }

      // Assign AVM list to loop
      newObject.setString(22,avmHandle);

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      m_new.push_back(avmList);

      ss << newObject;
      ss << avmList;

    } else if (iddname == "OS:AvailabilityManager:NightCycle") {
      auto iddObject = idd_2_3_1.getObject("OS:AvailabilityManager:NightCycle");
      IdfObject newObject(iddObject.get());

      // Cycling Run Time Control Type was placed before Cycling Run Time
      for( size_t i = 0; i < object.numNonextensibleFields() - 1; ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      // No need to set the default Cycling Run Time Control Type, it has a defaulted option
      // But we need to carry the cycling Run Time (seconds) in the right field
      if (auto cyclingRunTime = newObject.getDouble(6)) {
        newObject.setDouble(7, cyclingRunTime.get());
      }

      // We need to create the four ModelObjectLists for the control zones
      // 8 = Control Zone or Zone List Name
      std::string  controlThermalZoneListName = "";
      if( auto avmName = object.getString(1) ) {
        controlThermalZoneListName = avmName.get();
      }
      controlThermalZoneListName = controlThermalZoneListName + " Control Zone List";

      IdfObject controlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
      std::string controlThermalZoneListHandle = toString(createUUID());
      controlThermalZoneList.setString(0, controlThermalZoneListHandle);
      controlThermalZoneList.setString(1, controlThermalZoneListName);
      newObject.setString(8, controlThermalZoneListHandle);

      // 9 = Cooling Control Zone or Zone List Name
      std::string  coolingControlThermalZoneListName = "";
      if( auto avmName = object.getString(1) ) {
        coolingControlThermalZoneListName = avmName.get();
      }
      coolingControlThermalZoneListName = coolingControlThermalZoneListName + " Cooling Control Zone List";

      IdfObject coolingControlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
      std::string coolingControlThermalZoneListHandle = toString(createUUID());
      coolingControlThermalZoneList.setString(0, coolingControlThermalZoneListHandle);
      coolingControlThermalZoneList.setString(1, coolingControlThermalZoneListName);
      newObject.setString(9, coolingControlThermalZoneListHandle);


      // 10 = Heating Control Zone or Zone List Name
      std::string  heatingControlThermalZoneListName = "";
      if( auto avmName = object.getString(1) ) {
        heatingControlThermalZoneListName = avmName.get();
      }
      heatingControlThermalZoneListName = heatingControlThermalZoneListName + " Heating Control Zone List";

      IdfObject heatingControlThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
      std::string heatingControlThermalZoneListHandle = toString(createUUID());
      heatingControlThermalZoneList.setString(0, heatingControlThermalZoneListHandle);
      heatingControlThermalZoneList.setString(1, heatingControlThermalZoneListName);
      newObject.setString(10, heatingControlThermalZoneListHandle);


      // 11 = Heating Control Zone or Zone List Name
      std::string  heatingZoneFansOnlyThermalZoneListName = "";
      if( auto avmName = object.getString(1) ) {
        heatingZoneFansOnlyThermalZoneListName = avmName.get();
      }
      heatingZoneFansOnlyThermalZoneListName = heatingZoneFansOnlyThermalZoneListName + " Heating Zone Fans Only Zone List";

      IdfObject heatingZoneFansOnlyThermalZoneList(idd_2_3_1.getObject("OS:ModelObjectList").get());
      std::string heatingZoneFansOnlyThermalZoneListHandle = toString(createUUID());
      heatingZoneFansOnlyThermalZoneList.setString(0, heatingZoneFansOnlyThermalZoneListHandle);
      heatingZoneFansOnlyThermalZoneList.setString(1, heatingZoneFansOnlyThermalZoneListName);
      newObject.setString(11, heatingZoneFansOnlyThermalZoneListHandle);


      m_refactored.push_back( RefactoredObjectData(object, newObject) );
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

  ss << idf_2_4_1.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_4_2.iddFile());
  ss << targetIdf.versionObject().get();

  boost::optional<std::string> value;

  for (const IdfObject& object : idf_2_4_1.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:BuildingUnit") {
      auto iddObject = idd_2_4_2.getObject("OS:BuildingUnit");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

      iddObject = idd_2_4_2.getObject("OS:AdditionalProperties");
      IdfObject additionalProperties(iddObject.get());
      additionalProperties.setString(0, toString(createUUID()));
      additionalProperties.setString(1, newObject.getString(0).get()); // point additional properties to new object

      size_t newIdx = 2;
      for( size_t oldIdx = object.numNonextensibleFields(); oldIdx < object.numFields(); ++oldIdx, ++newIdx ) {
        if( (value = object.getString(oldIdx)) ) {
          additionalProperties.setString(newIdx, value.get());
        }
      }

      m_new.push_back(additionalProperties);
      ss << additionalProperties;

    } else if (iddname == "OS:Boiler:HotWater") {
      auto iddObject = idd_2_4_2.getObject("OS:Boiler:HotWater");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }
      newObject.setString(18,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if (iddname == "OS:Boiler:Steam") {
      auto iddObject = idd_2_4_2.getObject("OS:Boiler:Steam");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }
      newObject.setString(16,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if (iddname == "OS:WaterHeater:Mixed") {
      auto iddObject = idd_2_4_2.getObject("OS:WaterHeater:Mixed");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      // Source Side Flow Control Mode
      newObject.setString(40,"IndirectHeatPrimarySetpoint");

      // Indirect Alternate Setpoint Temperature Schedule Name: nothing to do, leave empty

      // End Use Subcategory
      newObject.setString(42,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if (iddname == "OS:Chiller:Electric:EIR") {
      auto iddObject = idd_2_4_2.getObject("OS:Chiller:Electric:EIR");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          newObject.setString(i,value.get());
        }
      }

      // Condenser Heat Recovery Relative Capacity Fraction: this is an optional
      // but in E+ code (ChillerElectricEIR.cc > GetElectricEIRChillerInput() around line 655)
      // if omitted (and heat recovery is used), it defaults to 1.0
      // newObject.setDouble(32, 1.0);

      // Heat Recovery Inlet High Temperature Limit Schedule Name: leave empty


      // Heat Recovery Leaving Temperature Setpoint Node Name: leave empty

      // endUseSubcategory
      newObject.setString(34,"General");

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    // Default case
    } else {
      ss << object;
    }
  }

  return ss.str();
}


std::string VersionTranslator::update_2_4_3_to_2_5_0(const IdfFile& idf_2_4_3, const IddFileAndFactoryWrapper& idd_2_5_0){
  std::stringstream ss;

  ss << idf_2_4_3.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_5_0.iddFile());
  ss << targetIdf.versionObject().get();

  boost::optional<std::string> value;

  for (const IdfObject& object : idf_2_4_3.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:AirflowNetworkZone") {
      // In 2.5.0, a field "Name" was inserted right after the handle
      auto iddObject = idd_2_5_0.getObject("OS:AirflowNetworkZone");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
          if (i == 0) {
            // Handle
            newObject.setString(i, value.get());
          } else if (i == 1) {
            // Thermal Zone Name field

            // We place the Thermal Zone's handle into the right field
            newObject.setString(i+1, value.get());

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
            newObject.setString(i+1, value.get());
          }
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

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

  ss << idf_2_6_0.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_6_1.iddFile());
  ss << targetIdf.versionObject().get();


  struct ConnectionInfo {
    std::string zoneHandle;
    std::string connectionHandle;
    std::string newPortListHandle;
  };
  // map of a connection object handle to a ConnectionInfo instance
  std::map<std::string, ConnectionInfo> connectionsToFix;

  // Find the connection object associated with the return air port
  auto zones = idf_2_6_0.getObjectsByType(idf_2_6_0.iddFile().getObject("OS:ThermalZone").get());
  for ( auto & zone : zones ) {
    // index 12 is the handle of a connection that will need fixing
    value = zone.getString(12);
    if ( value ) {
      ConnectionInfo info;
      info.zoneHandle = zone.getString(0).get();
      info.connectionHandle = value.get();
      connectionsToFix[value.get()] = info;
    }
  }

  for (const IdfObject& object : idf_2_6_0.objects()) {
    auto iddname = object.iddObject().name();

    if (iddname == "OS:ThermalZone") {
      auto iddObject = idd_2_6_1.getObject("OS:ThermalZone");
      IdfObject newObject(iddObject.get());

      IdfObject newReturnPortList(idd_2_6_1.getObject("OS:PortList").get());

      auto h = toString(createUUID());
      newReturnPortList.setString(0,h);
      newReturnPortList.setString(2,object.getString(0).get());

      for ( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        value = object.getString(i);
        if ( value ) {
          if ( i == 12 ) {
            auto eg = newReturnPortList.pushExtensibleGroup();
            eg.setString(0,value.get());
            connectionsToFix[value.get()].newPortListHandle = h;
            newObject.setString(i, h);
          } else {
            newObject.setString(i, value.get());
          }
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      m_new.push_back(newReturnPortList);
      ss << newObject;
      ss << newReturnPortList;
    } else if ( iddname == "OS:Connection" ) {
      value = object.getString(0);
      OS_ASSERT(value);
      auto c = connectionsToFix.find(value.get());
      if ( c != connectionsToFix.end() ) {
        IdfObject newConnection(idd_2_6_1.getObject("OS:Connection").get());
        for ( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
          value = object.getString(i);
          if ( value ) {
            newConnection.setString(i, value.get());
          }
        }
        // index 3 is the source object port,
        // it needs to specify a port on the PortList instead of the ThermalZone now
        newConnection.setString(2, c->second.newPortListHandle);
        newConnection.setUnsigned(3, 3);
        m_refactored.push_back( RefactoredObjectData(object, newConnection) );
        ss << newConnection;
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

  ss << idf_2_6_1.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_6_2.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_2_6_1.objects()) {
    auto iddname = object.iddObject().name();

    if ( iddname == "OS:EvaporativeCooler:Direct:ResearchSpecial" ) {

      auto iddObject = idd_2_6_2.getObject("OS:EvaporativeCooler:Direct:ResearchSpecial");
      IdfObject newObject(iddObject.get());

      for ( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        auto value = object.getString(i);
        if ( value ) {
          newObject.setString(i, value.get());
        }
      }
      // The last three fields were added in #3118 as NON optional doubles, so default to extreme values
      // to make it behave like when blank = no control

      // Evaporative Operation Minimum Drybulb Temperature
      if( !newObject.getDouble(14) ) {
        newObject.setDouble(14, -99);
      }
      // Evaporative Operation Maximum Limit Wetbulb Temperature
      if( !newObject.getDouble(15) ) {
        newObject.setDouble(15, 99);
      }
      // Evaporative Operation Maximum Limit Drybulb Temperature
      if( !newObject.getDouble(16) ) {
        newObject.setDouble(16, 99);
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if (iddname == "OS:ZoneHVAC:EquipmentList") {
      // In 2.6.2, a field "Load Distribution Scheme" was inserted right after the thermal zone
      auto iddObject = idd_2_6_2.getObject("OS:ZoneHVAC:EquipmentList");
      IdfObject newObject(iddObject.get());

      for( size_t i = 0; i < object.numFields(); ++i ) {
        auto value = object.getString(i);
        if (value) {
          if (i < 3) {
            // Handle
            newObject.setString(i, value.get());
          } else {
            // Every other is shifted by one field
            newObject.setString(i+1, value.get());
          }
        }
      }

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else {
      ss << object;
    }
  }

  return ss.str();
}


std::string VersionTranslator::update_2_6_2_to_2_7_0(const IdfFile& idf_2_6_2, const IddFileAndFactoryWrapper& idd_2_7_0) {
  std::stringstream ss;

  ss << idf_2_6_2.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_7_0.iddFile());
  ss << targetIdf.versionObject().get();


  struct ConnectionInfo {
    std::string atuHandle;
    std::string connectionHandle;
    std::string newNodeHandle;
  };
  // map of a connection object handle to a ConnectionInfo instance
  std::map<std::string, ConnectionInfo> connectionsToFix;

  // Find the connections object associated with the ATU Uncontrolled
  auto atus = idf_2_6_2.getObjectsByType(idf_2_6_2.iddFile().getObject("OS:AirTerminal:SingleDuct:Uncontrolled").get());
  for ( auto & atu : atus ) {
    // index 3 = Air Inlet Node Name
    // It's the handle of a connection that will need fixing
    // Because ATU Uncontrolled was directly connected to ZoneSplitter; now we want a node in between
    auto value = atu.getString(3);
    if ( value ) {
      ConnectionInfo info;
      info.atuHandle = atu.getString(0).get();
      info.connectionHandle = value.get();
      connectionsToFix[value.get()] = info;
    }
  }

  for (const IdfObject& object : idf_2_6_2.objects()) {
    auto iddname = object.iddObject().name();

    // ATU:SingleDuct:Uncontrolled got made obsolete by ATU:SingleDuct:ConstantVolume:NoReheat in E+ 9.0.0
    // in order to be more consistent with the naming of other ATUs, but it also isn't exactly laid out the same
    if ( iddname == "OS:AirTerminal:SingleDuct:Uncontrolled" ) {
      // We just create a new object, and copy every field but one.
      auto iddObject = idd_2_7_0.getObject("OS:AirTerminal:SingleDuct:ConstantVolume:NoReheat");
      OS_ASSERT(iddObject);
      IdfObject newObject(iddObject.get());

      for ( size_t i = 0; i < object.numFields(); ++i ) {
        if ( auto s = object.getString(i) ) {
          if ( i == 3 ) {
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

      m_refactored.push_back( RefactoredObjectData(object, newObject) );
      ss << newObject;

    } else if ( iddname == "OS:Connection" ) {
      // No-Op for now
      auto value = object.getString(0);
      OS_ASSERT(value);
      if ( connectionsToFix.find(value.get()) == connectionsToFix.end() ) {
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

      m_refactored.push_back(RefactoredObjectData(object, newObject));
      ss << newObject;

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

      m_refactored.push_back(RefactoredObjectData(object, newObject));
      ss << newObject;

    } else {
      ss << object;
    }
  }

  // Do a second pass, though you'd expect connections to be after the objects it connects, technically it's possible that it wouldn't
  // if the user moved objects manually inside the file
  // So I do want connectionInfos to be properly populated with newNodeHandle
  for (const IdfObject& object : idf_2_6_2.objects()) {
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
        m_refactored.push_back(RefactoredObjectData(object, newConnection));
        ss << newConnection;
      }
    }
  }

  return ss.str();
}

std::string VersionTranslator::update_2_7_0_to_2_7_1(const IdfFile& idf_2_7_0, const IddFileAndFactoryWrapper& idd_2_7_1) {
  std::stringstream ss;
  boost::optional<std::string> value;

  ss << idf_2_7_0.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_7_1.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_2_7_0.objects()) {
    auto iddname = object.iddObject().name();

    if ( iddname == "OS:Sizing:System" ) {
      // Note JM 2018-11-05:
      // We deprecated the 'Latent' option which is no longer used by E+ (and hasn't been for a long time)
      // It should have produced a crash anyways before, but if we find 'Latent', we replace by 'Total'
      // Why did I choose 'Total' and not 'Sensible' (or VentilationRequirement)?
      // 'Sensible' is the default choice, but if the user had picked 'Latent', they probably didn't want 'Sensible', so I went with 'Total' instead.
      value = object.getString(2);
      if( value && istringEqual(value.get(), "Latent") ) {
        IdfObject newObject = object.clone(true);
        newObject.setString(2, "Total");
        LOG(Warn, "OS:Sizing:System does not support 'Latent' as 'Type of Load To Size On'. "
               << "It was replaced by 'Total' instead for object with handle '"
               << newObject.getString(0).get() << "'. Please review carefully.");

        m_refactored.push_back( RefactoredObjectData(object,  newObject) );
        ss << newObject;
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

  ss << idf_2_7_1.header() << std::endl << std::endl;
  IdfFile targetIdf(idd_2_7_2.iddFile());
  ss << targetIdf.versionObject().get();

  for (const IdfObject& object : idf_2_7_1.objects()) {
    auto iddname = object.iddObject().name();

    if ( iddname == "OS:WeatherFile" ) {
      // Note JM 2019-01-08: We remove the file:// prefix from QUrl
      value = object.getString(10);
      // If there's a filepath, and it starts with file://
      if( value && (value.get().rfind("file://", 0) == 0)) {
        IdfObject newObject = object.clone(true);
        newObject.setString(10, value.get().substr(7));
        m_refactored.push_back( RefactoredObjectData(object,  newObject) );
        ss << newObject;
      } else {
        // Nothing to do here
        ss << object;
      }

    // Both of these happen to have the url field at pos 2 (note: neither of these are actually implemented in the SDK, but let's be safe)
    } else if (( iddname == "OS:Construction:WindowDataFile" ) || ( iddname == "OS:Luminaire:Definition" ) ){
      // Note JM 2019-01-08: We remove the file:// prefix from QUrl
      value = object.getString(2);
      // If there's a filepath, and it starts with file://
      if( value && (value.get().rfind("file://", 0) == 0)) {
        IdfObject newObject = object.clone(true);
        newObject.setString(2, value.get().substr(7));
        m_refactored.push_back( RefactoredObjectData(object,  newObject) );
        ss << newObject;
      } else {
        // Nothing to do here
        ss << object;
      }

    } else if (iddname == "OS:ZoneHVAC:EquipmentList") {

      auto iddObject = idd_2_7_2.getObject("OS:ZoneHVAC:EquipmentList");
      IdfObject newObject(iddObject.get());

      // Copy non extensible fields in place
      for( size_t i = 0; i < object.numNonextensibleFields(); ++i ) {
        if( (value = object.getString(i)) ) {
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

      m_refactored.push_back( RefactoredObjectData(object,  newObject) );
      ss << newObject;

    // No-op
    } else {
      ss << object;
    }
  }

  return ss.str();

}


} // osversion
} // openstudio

