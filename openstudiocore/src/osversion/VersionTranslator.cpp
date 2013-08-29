/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <osversion/VersionTranslator.hpp>

#include <model/Model.hpp>
#include <model/Component.hpp>
#include <model/Component_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeLimits_Impl.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/ScheduleDay.hpp>
#include <model/ScheduleDay_Impl.hpp>
#include <model/ScheduleRule.hpp>
#include <model/ScheduleRule_Impl.hpp>
#include <model/ScheduleRuleset.hpp>
#include <model/ScheduleRuleset_Impl.hpp>
#include <model/ComponentData.hpp>
#include <model/ComponentData_Impl.hpp>
#include <model/ModelExtensibleGroup.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/ValidityReport.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/URLHelpers.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/plot/ProgressBar.hpp>
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include <utilities/math/FloatCompare.hpp>

#include <OpenStudio.hxx>

#include <QThread>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/fstream.hpp>

namespace openstudio {
namespace osversion {

VersionTranslator::VersionTranslator()
  : m_originalVersion("0.0.0")
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
  //     replace that version number with the new one. (At most there should only be one occurance
  //     of defaultUpdate in this list.)
  m_updateMethods[VersionString("0.7.2")] = &VersionTranslator::update_0_7_1_to_0_7_2;
  m_updateMethods[VersionString("0.7.3")] = &VersionTranslator::update_0_7_2_to_0_7_3;
  m_updateMethods[VersionString("0.7.4")] = &VersionTranslator::update_0_7_3_to_0_7_4;
  m_updateMethods[VersionString("0.9.2")] = &VersionTranslator::update_0_9_1_to_0_9_2;
  m_updateMethods[VersionString("0.9.6")] = &VersionTranslator::update_0_9_5_to_0_9_6;
  m_updateMethods[VersionString("0.11.1")] = &VersionTranslator::update_0_11_0_to_0_11_1;
  m_updateMethods[VersionString("0.11.2")] = &VersionTranslator::update_0_11_1_to_0_11_2;
  m_updateMethods[VersionString("0.11.5")] = &VersionTranslator::update_0_11_4_to_0_11_5;
  m_updateMethods[VersionString("0.11.6")] = &VersionTranslator::update_0_11_5_to_0_11_6;
  m_updateMethods[VersionString("1.0.2")] = &VersionTranslator::update_1_0_1_to_1_0_2;
  m_updateMethods[VersionString("1.0.3")] = &VersionTranslator::update_1_0_2_to_1_0_3;
  m_updateMethods[VersionString("1.0.5")] = &VersionTranslator::defaultUpdate;

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
  boost::filesystem::ifstream inFile(wp);
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
  boost::filesystem::ifstream inFile(wp);
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
  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }
  return result;
}

std::vector<LogMessage> VersionTranslator::errors() const {
  std::vector<LogMessage> result;
  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
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

std::vector< std::pair<IdfObject,IdfObject> > VersionTranslator::refactoredObjects() const {
  return m_refactored;
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
  BOOST_FOREACH(const VersionString& startVersion,m_startVersions) {
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
  std::vector<boost::shared_ptr<InterobjectIssueInformation> > issueInfo = fixInterobjectIssuesStage1(
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
    LOG(Error,"Version extracted from file '" << currentVersion.str()
        << "' is not supported by OpenStudio Version " << openStudioVersion()
        << ". Please check http://openstudio.nrel.gov for updates.");
    return;
  }

  // load IdfFile with correct IddFile and save
  OptionalIdfFile oIdfFile;
  IddFileAndFactoryWrapper iddFile = getIddFile(currentVersion);
  if (iddFile.iddFileType() == IddFileType::UserCustom) {
    oIdfFile = IdfFile::load(is,iddFile.iddFile());
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
  BOOST_FOREACH(const IdfObject& object,idf.objects()) {
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
  BOOST_FOREACH(const IdfObject& object,idf_0_7_1.objects()) {
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
  BOOST_FOREACH(const IdfObject& object,idf_0_7_2.objects()) {
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
  BOOST_FOREACH(IdfObject object, idf_0_7_3.objects()) {
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
      BOOST_FOREACH(const IdfExtensibleGroup& eg,object.extensibleGroups()) {
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
          IdfObjectVector::iterator it(candidates.begin());
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
          IdfObjectVector::iterator it(candidates.begin());
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
      m_refactored.push_back( std::pair<IdfObject,IdfObject>(object,componentDataIdf) );
    }
    else
    {
      // modify the object if neccesary
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

std::vector< boost::shared_ptr<VersionTranslator::InterobjectIssueInformation> >
VersionTranslator::fixInterobjectIssuesStage1(model::Model& model,
                                              const VersionString& startVersion)
{
  OS_ASSERT(model.strictnessLevel() == StrictnessLevel::None);
  std::vector<boost::shared_ptr<InterobjectIssueInformation> > result;

  if (startVersion < VersionString("0.8.4")) {
    boost::shared_ptr<InterobjectIssueInformation> info = fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model);
    result.push_back(info);
  }

  return result;
}

void VersionTranslator::fixInterobjectIssuesStage2(
    model::Model& model,
    std::vector<boost::shared_ptr<InterobjectIssueInformation> >& stage1Information)
{
  OS_ASSERT(model.strictnessLevel() == StrictnessLevel::Draft);

  BOOST_FOREACH(boost::shared_ptr<InterobjectIssueInformation>& info,stage1Information) {
    if (info->endVersion == VersionString("0.8.4")) {
      fixInterobjectIssuesStage2_0_8_3_to_0_8_4(model,info);
    }
  }
}

boost::shared_ptr<VersionTranslator::InterobjectIssueInformation>
VersionTranslator::fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model::Model& model) {
  boost::shared_ptr<InterobjectIssueInformation_0_8_3_to_0_8_4> result(
      new InterobjectIssueInformation_0_8_3_to_0_8_4());


  // deal with component data
  model::ComponentDataVector allComponentData = model.getModelObjects<model::ComponentData>();
  std::vector<IdfObject> allIdfComponentData; // make sure idf versions share data
  std::vector<model::ModelObjectVector> allComponentDataObjects;
  BOOST_FOREACH(const model::ComponentData& cd,allComponentData) {
    allComponentDataObjects.push_back(cd.componentObjects());
    allIdfComponentData.push_back(cd.idfObject());
  }
  std::set<int> found;
  // loop through all schedules
  model::ScheduleVector schedules = model.getModelObjects<model::Schedule>();
  BOOST_FOREACH(model::Schedule& schedule, schedules) {
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
        BOOST_FOREACH(IdfObject& cd,thisSchedulesComponentData) {
          model::ModelObjectVector::iterator it = std::find_if(result->users.back().begin(),
                                                               result->users.back().end(),
                                                               boost::bind(handleEquals<IdfObject,Handle>,_1,cd.handle()));
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
        model::ModelObjectVector::iterator it = result->users.back().begin();
        while (it != result->users.back().end()) {
          UnsignedVector thisUsersIndices = it->getSourceIndices(schedule.handle());
          std::vector<model::ScheduleTypeKey> thisUsersKeys = it->getScheduleTypeKeys(schedule);

          if (thisUsersKeys.empty()) {
            it = result->users.back().erase(it);
            continue;
          }

          OS_ASSERT(thisUsersIndices.size() == thisUsersKeys.size());
          BOOST_FOREACH(unsigned index,thisUsersIndices) {
            it->setString(index,""); // clear for now--only change to be made at none strictness
          }
          thisSchedulesIndices.push_back(thisUsersIndices);
          thisSchedulesKeys.push_back(thisUsersKeys);
          if (std::find_if(result->originalUsers.begin(),
                           result->originalUsers.end(),
                           boost::bind(handleEquals<IdfObject,Handle>,_1,it->handle())) == result->originalUsers.end())
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
  BOOST_FOREACH(int index,found) {
    m_untranslated.push_back(allIdfComponentData[index]);
    if (allComponentData[index].initialized()) {
      allComponentData[index].remove();
    }
  }

  return result;
}

void VersionTranslator::fixInterobjectIssuesStage2_0_8_3_to_0_8_4(
    model::Model& model,
    boost::shared_ptr<InterobjectIssueInformation>& info)
{
  boost::shared_ptr<InterobjectIssueInformation_0_8_3_to_0_8_4> schedulesToFixup =
    boost::dynamic_pointer_cast<InterobjectIssueInformation_0_8_3_to_0_8_4>(info);

  // make sure ScheduleDays are owned by their ScheduleRules and ScheduleRulesets
  model::ScheduleDayVector daySchedules = model.getModelObjects<model::ScheduleDay>();
  BOOST_FOREACH(model::ScheduleDay& daySchedule,daySchedules) {
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
      for (unsigned i = 0, ni = ruleUsers.size(); i < ni; ++i) {
        UnsignedVector indices = ruleUsers[i].getSourceIndices(daySchedule.handle());
        unsigned j = 0;
        if (!primaryUser) {
          primaryUser = ruleUsers[i];
          j = 1;
        }
        for (unsigned nj = indices.size(); j < nj; ++j) {
          model::ModelObject dayScheduleClone = daySchedule.clone();
          ruleUsers[i].setPointer(indices[j],dayScheduleClone.handle());
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
          BOOST_FOREACH(model::Schedule& candidate,candidates) {
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
    m_refactored.push_back(std::pair<IdfObject,IdfObject>(originalSchedule,schedule.idfObject()));
    for (unsigned j = 0, n = candidates.size(); j < n; ++j) {
      model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidates[j]);
      m_new.push_back(candidates[j].idfObject());
      m_new.push_back(candidates[j].scheduleTypeLimits().get().idfObject());
      for (unsigned k = 1, nc = wholeCandidate.size(); k < nc; ++k) {
        m_new.push_back(wholeCandidate[k].idfObject());
      }
    }
    // start to fix-up component data--add objects
    std::set<model::ModelObject> usersSet(schedulesToFixup->users[i].begin(),schedulesToFixup->users[i].end());
    BOOST_FOREACH(IdfObject& cd,schedulesToFixup->componentDataObjects[i]) {
      LOG(Debug,"Adding ScheduleTypeLimits '" << scheduleLimits->name().get() << "' to component data list.");
      cd.pushExtensibleGroup(StringVector(1u,toString(scheduleLimits->handle())));
      for (unsigned j = 0, n = candidates.size(); j < n; ++j) {
        model::ModelObjectVector wholeCandidate = getRecursiveChildren(candidates[j]);
        LOG(Debug,"Adding Schedule '" << candidates[j].name().get()
            << "', its ScheduleTypeLimits, and all children to component data list.");
        cd.pushExtensibleGroup(StringVector(1u,toString(candidates[j].handle())));
        cd.pushExtensibleGroup(StringVector(1u,toString(candidates[j].scheduleTypeLimits()->handle())));
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
  BOOST_FOREACH(const std::vector<IdfObject>& scheduleCD,schedulesToFixup->componentDataObjects) {
    BOOST_FOREACH(const IdfObject& cd,scheduleCD) {
      uniqueComponentData.insert(cd);
    }
  }
  IdfObjectVector uniqueComponentDataVector(uniqueComponentData.begin(),uniqueComponentData.end());
  BOOST_FOREACH(IdfObject& cd,uniqueComponentDataVector) {
    // remove duplicates and objects not in model
    model::ModelObjectVector componentObjects;
    unsigned i = 0;
    while (i < cd.numExtensibleGroups()) {
      Handle h = toUUID(*cd.getExtensibleGroup(i).getString(0));
      if (std::find_if(componentObjects.begin(),
                       componentObjects.end(),
                       boost::bind(handleEquals<model::ModelObject,Handle>,_1,h)) != componentObjects.end())
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
        model::ModelObjectVector::iterator intersectionEnd = std::set_intersection(
            users.begin(),users.end(),
            componentObjects.begin(),componentObjects.end(),
            intersection.begin(),IdfObjectImplLess());
        // subtract children from intersection
        model::ModelObjectVector::iterator resultEnd = std::set_difference(
            intersection.begin(),intersectionEnd,
            children.begin(),children.end(),
            result.begin(),IdfObjectImplLess());
        if (int(resultEnd - result.begin()) == 0) {
          toRemove.insert(toRemove.end(),children.begin(),children.end());
        }
      }
    }

    model::ModelObjectVector::iterator it = componentObjects.begin();
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
        model::ModelObjectVector::iterator intersectionEnd = std::set_intersection(
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
  BOOST_FOREACH(model::ModelObject& user,refactoredUsersVector) {
    IdfObjectVector::const_iterator it = std::find_if(schedulesToFixup->originalUsers.begin(),
                                                      schedulesToFixup->originalUsers.end(),
                                                      boost::bind(handleEquals<IdfObject,Handle>,_1,user.handle()));
    OS_ASSERT(it != schedulesToFixup->originalUsers.end());
    m_refactored.push_back(std::pair<IdfObject,IdfObject>(*it,user.idfObject()));
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
  BOOST_FOREACH(const IdfObject& object,idf_0_9_1.objects()) {
    if( object.iddObject().name() == "OS:ThermalZone" )
    {
      boost::optional<std::string> s;

      IdfObject newInletPortList(idd_0_9_2.getObject("OS:PortList").get());
      newInletPortList.setString(0,createUUID().toString().toStdString());

      IdfObject newExhaustPortList(idd_0_9_2.getObject("OS:PortList").get());
      newExhaustPortList.setString(0,createUUID().toString().toStdString());

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
              for( std::vector<IdfObject>::iterator connection = connections.begin();
                   connection != connections.end();
                   connection++ )
              {
                if( connection->getString(0).get() == s.get() )
                {
                  // Fixup the connection's target object and port
                  connection->setString(4,newInletPortList.getString(0).get());
                  connection->setUnsigned(5,2);

                  // Find the inlet model object to the connection - assume it is a node
                  if( boost::optional<std::string> sourceObject1Handle = connection->getString(2) )
                  {
                    std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                    for( std::vector<IdfObject>::iterator node = nodes.begin();
                         node != nodes.end();
                         node++ )
                    {
                      if( sourceObject1Handle.get() == node->getString(0).get() )
                      {
                        // Find the connection object associated with the inlet node
                        if( boost::optional<std::string> nodeInletConnectionHandle = node->getString(2) )
                        {
                          for( std::vector<IdfObject>::iterator connection2 = connections.begin();
                               connection2 != connections.end();
                               connection2++ )
                          {
                            if( nodeInletConnectionHandle.get() == connection2->getString(0).get() )
                            {
                              // Find the inlet model object to connection2
                              if( boost::optional<std::string> sourceObject2Handle = connection2->getString(2) )
                              {
                                std::vector<IdfObject> objects = idf_0_9_1.objects();
                                for( std::vector<IdfObject>::iterator object2 = objects.begin();
                                    object2 != objects.end();
                                    object2++ )
                                {
                                  if( object2->getString(0).get() == sourceObject2Handle.get() )
                                  {
                                    if( object2->iddObject() != idf_0_9_1.iddFile().getObject("OS:AirLoopHVAC:ZoneSplitter").get() )
                                    {
                                      // If there is zone equipment add it to the zone equipment list
                                      newZoneHVACEquipmentList.setString(3,sourceObject2Handle.get());
                                      newZoneHVACEquipmentList.setUnsigned(4,1);
                                      newZoneHVACEquipmentList.setUnsigned(5,1);
                                    }

                                    if( object2->iddObject() == idf_0_9_1.iddFile().getObject("OS:AirTerminal:SingleDuct:ParallelPIU:Reheat").get() )
                                    {
                                      fanPowerTerminal = *object2;
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
              for( std::vector<IdfObject>::iterator connection = connections.begin();
                   connection != connections.end();
                   connection++ )
              {
                if( connection->getString(0).get() == s.get() )
                {
                  // Find the target model object of the connection - assume it is a node
                  if( boost::optional<std::string> target1Handle = connection->getString(4) )
                  {
                    std::vector<IdfObject> nodes = idf_0_9_1.getObjectsByType(idf_0_9_1.iddFile().getObject("OS:Node").get());
                    for( std::vector<IdfObject>::iterator node = nodes.begin();
                         node != nodes.end();
                         node++ )
                    {
                      if( target1Handle.get() == node->getString(0).get() )
                      {
                        // Does the exhaust node have a outlet connection?
                        if( boost::optional<std::string> connection2Handle = node->getString(3) )
                        {
                          if( connection2Handle && connection2Handle.get() != "" )
                          {
                            // We fix up the connection to the thermal zone.
                            // Now actually it is a connection to the port list.
                            newExhaustPortList.setString(2,s.get());
                            connection->setString(2,newExhaustPortList.getString(0).get());
                            connection->setUnsigned(3,2);
                          }
                        }
                        else if( fanPowerTerminal )
                        {
                          newExhaustPortList.setString(2,s.get());
                          connection->setString(2,newExhaustPortList.getString(0).get());
                          connection->setUnsigned(3,2);

                          newFPTSecondaryInletConn = IdfObject(idd_0_9_2.getObject("OS:Connection").get());
                          newFPTSecondaryInletConn->setString(0,createUUID().toString().toStdString());

                          newFPTSecondaryInletConn->setString(2,node->getString(0).get());
                          newFPTSecondaryInletConn->setUnsigned(3,3);
                          newFPTSecondaryInletConn->setString(4,fanPowerTerminal->getString(0).get());
                          newFPTSecondaryInletConn->setUnsigned(5,8);

                          node->setString(3,newFPTSecondaryInletConn->getString(0).get());
                          fanPowerTerminal->setString(8,newFPTSecondaryInletConn->getString(0).get());
                        }
                        else
                        {
                          // Exhaust nodes don't exist by default anymore.
                          // They are only needed if there is zone equipment hooked up to them.
                          // We break the connection to the thermal zone.
                          connection->setString(2,"");
                          connection->setString(3,"");
                          connection->setString(4,"");
                          connection->setString(5,"");

                          node->setString(2,"");
                          node->setString(3,"");
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

  BOOST_FOREACH(const IdfObject& object,idf_0_9_1.objects()) {
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
  BOOST_FOREACH(const IdfObject& object,idf_0_9_5.objects()) 
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

  BOOST_FOREACH(const IdfObject& object,idf_0_9_5.objects()) {
    if( object.iddObject().name() == "OS:PlantLoop" )
    {
      IdfObject newSizingPlant(idd_0_9_6.getObject("OS:Sizing:Plant").get());

      newSizingPlant.setString(0,createUUID().toString().toStdString());

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

  BOOST_FOREACH(const IdfObject& object, idf_0_11_0.objects()) {
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
  BOOST_FOREACH(const IdfObject& componentDataObject, componentDataObjects) {

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

    BOOST_FOREACH(const IdfObject& object,idf_0_11_1.objects()) {
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

      alwaysOnSchedule->setString(0,createUUID().toString().toStdString());

      alwaysOnSchedule->setString(1,"Always On Discrete");

      alwaysOnSchedule->setDouble(3,1.0);


      IdfObject typeLimits(idd_0_11_2.getObject("OS:ScheduleTypeLimits").get());

      typeLimits.setString(0,createUUID().toString().toStdString());

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

  BOOST_FOREACH(const IdfObject& object,idf_0_11_1.objects()) {
    // There are no official components with HVAC at this version.
    // We don't do this translation because it breaks tests related to OS:ComponentData when new objects are added.
    // Future version will do something better.
    if( object.iddObject().name() == "OS:Controller:OutdoorAir" && ! m_isComponent )
    {
      IdfObject newOAController = object.clone(true);

      IdfObject newMechVentController(idd_0_11_2.getObject("OS:Controller:MechanicalVentilation").get());

      newMechVentController.setString(0,createUUID().toString().toStdString());

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

      newAvailList.setString(0,createUUID().toString().toStdString());

      newAirLoopHVAC.setString(3,newAvailList.getString(0).get());


      IdfObject newAvailabilityManagerScheduled(idd_0_11_2.getObject("OS:AvailabilityManager:Scheduled").get());

      newAvailabilityManagerScheduled.setString(0,createUUID().toString().toStdString());

      newAvailabilityManagerScheduled.setString(2,alwaysOnSchedule->getString(0).get());

      IdfExtensibleGroup eg = newAvailList.insertExtensibleGroup(0);

      eg.setString(0,newAvailabilityManagerScheduled.getString(0).get());


      IdfObject newAvailabilityManagerNightCycle(idd_0_11_2.getObject("OS:AvailabilityManager:NightCycle").get());

      newAvailabilityManagerNightCycle.setString(0,createUUID().toString().toStdString());

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
  BOOST_FOREACH(const IdfObject& componentDataObject, componentDataObjects) {

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

  BOOST_FOREACH(const IdfObject& object,idf_0_11_4.objects()) {

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
  BOOST_FOREACH(const IdfObject& componentDataObject, componentDataObjects) {

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

  BOOST_FOREACH(const IdfObject& object,idf_0_11_5.objects()) {

    if( object.iddObject().name() == "OS:ThermalZone" ) {

      boost::optional<std::string> inletPortListString = object.getString(9);
      boost::optional<std::string> exhaustPortListString = object.getString(10);

      BOOST_FOREACH(const IdfObject& object2,idf_0_11_5.objects()) {

        if( object2.iddObject().name() == "OS:PortList" ) {

          if( boost::optional<std::string> object2Handle = object2.getString(0) ) {

            if( (inletPortListString && object2Handle.get() == inletPortListString.get())
                ||
                (exhaustPortListString && object2Handle.get() == exhaustPortListString.get()) ) {
              
              // Make the required change to OS:PortList
              
              IdfObject newPortList(idd_0_11_6.getObject("OS:PortList").get());

              boost::optional<std::string> s;

              if( s = object2.getString(0) ) {
                newPortList.setString(0,s.get());
              }

              if( s = object2.getString(1) ) {
                newPortList.setString(1,s.get());
              }
              
              if( s = object.getString(0) ) {
                newPortList.setString(2,s.get());
              }

              for( unsigned i = 2; i < object2.numFields(); i++ ) {

                if( s = object2.getString(i) ) {
                  newPortList.setString(i + 1, s.get());
                }

              }

              m_refactored.push_back( std::pair<IdfObject,IdfObject>(object2,newPortList) );

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

  BOOST_FOREACH(const IdfObject& object,idf_1_0_1.objects()) {

    if( object.iddObject().name() == "OS:Boiler:HotWater" ) {

      if(object.getString(15) && istringEqual(object.getString(15).get(),"VariableFlow")) {
        // Update Boiler Flow Mode

        IdfObject newBoiler = object.clone(true);

        newBoiler.setString(15,"LeavingSetpointModulated");

        m_refactored.push_back( std::pair<IdfObject,IdfObject>(object,newBoiler) );

        ss << newBoiler;

      } else {

        ss << object;

      }
    } else if( object.iddObject().name() == "OS:Boiler:HotWater" ) {

      if(object.getString(15) && istringEqual(object.getString(15).get(),"VariableFlow")) {
        // Update Chiller Flow Mode

        IdfObject newChiller = object.clone(true);

        newChiller.setString(15,"LeavingSetpointModulated");

        m_refactored.push_back( std::pair<IdfObject,IdfObject>(object,newChiller) );

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

  BOOST_FOREACH(const IdfObject& object,idf_1_0_2.objects()) {

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

        m_refactored.push_back( std::pair<IdfObject,IdfObject>(object, newParameters) );

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

} // osversion
} // openstudio

