/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include "BCLMeasure.hpp"
#include "LocalBCL.hpp"

#include "../core/ApplicationPathHelpers.hpp"
#include "../core/System.hpp"
#include "../core/Path.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/FileReference.hpp"
#include "../data/Attribute.hpp"
#include "../core/Assert.hpp"

#include <OpenStudio.hxx>

#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QSettings>

#include <boost/filesystem.hpp>

namespace openstudio{

  void BCLMeasure::createDirectory(const openstudio::path& dir){
    if (exists(dir)){
      if (!isEmptyDirectory(dir)){
        LOG_AND_THROW("'" << toString(dir) << "' exists but is not an empty directory");
      }
    }else{
      if (!QDir().mkpath(toQString(dir))){
        LOG_AND_THROW("'" << toString(dir) << "' cannot be created as an empty directory");
      }
    }
  }

  BCLMeasure::BCLMeasure(const std::string& name, const std::string& className, const openstudio::path& dir,
    const std::string& taxonomyTag, MeasureType measureType, bool usesSketchUpAPI)
    : m_directory(boost::filesystem::system_complete(dir)),
      m_bclXML(BCLXMLType::MeasureXML)
  {

    openstudio::path measureTestDir = dir / toPath("tests");

    createDirectory(dir);
    createDirectory(measureTestDir);

    // read in template files
    QString measureTemplate;
    QString testTemplate;
    QString templateName;
    QString testOSM;
    QString resourceFile;
    openstudio::path testOSMPath;
    openstudio::path resourceFilePath;
    if (measureType == MeasureType::ModelMeasure){
      measureTemplate = ":/templates/ModelMeasure/measure.rb";
      testTemplate = ":/templates/ModelMeasure/tests/ModelMeasure_Test.rb";
      templateName = "ModelMeasure";
    }else if (measureType == MeasureType::EnergyPlusMeasure){
      measureTemplate = ":/templates/EnergyPlusMeasure/measure.rb";
      testTemplate = ":/templates/EnergyPlusMeasure/tests/EnergyPlusMeasure_Test.rb";
      templateName = "EnergyPlusMeasure";
    }else if (measureType == MeasureType::UtilityMeasure){
      measureTemplate = ":/templates/UtilityMeasure/measure.rb";
      testTemplate = ":/templates/UtilityMeasure/tests/UtilityMeasure_Test.rb";
      templateName = "UtilityMeasure";
    }else if (measureType == MeasureType::ReportingMeasure){
      measureTemplate = ":/templates/ReportingMeasure/measure.rb";
      testTemplate = ":/templates/ReportingMeasure/tests/ReportingMeasure_Test.rb";
      templateName = "ReportingMeasure";

      testOSM = ":/templates/ReportingMeasure/tests/ExampleModel.osm";
      resourceFile = ":/templates/ReportingMeasure/resources/report.html.in";

      createDirectory(dir / toPath("tests"));
      createDirectory(dir / toPath("resources"));

      testOSMPath = dir / toPath("tests/ExampleModel.osm");
      resourceFilePath = dir / toPath("resources/report.html.in");
    }

    QString measureString;
    if (!measureTemplate.isEmpty()){
      QFile file(measureTemplate);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        measureString = docIn.readAll();
        measureString.replace(templateName, toQString(className));
        file.close();
      }
    }

    QString testString;
    if (!testTemplate.isEmpty()){
      QFile file(testTemplate);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        testString = docIn.readAll();
        testString.replace(templateName, toQString(className));
        file.close();
      }
    }

    QString testOSMString;
    if (!testOSM.isEmpty()){
      QFile file(testOSM);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        testOSMString = docIn.readAll();
        file.close();
      }
    }

    QString resourceFileString;
    if (!resourceFile.isEmpty()){
      QFile file(resourceFile);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        resourceFileString = docIn.readAll();
        file.close();
      }
    }

    // write files
    openstudio::path measureXMLPath = dir / toPath("measure.xml");
    openstudio::path measureScriptPath = dir / toPath("measure.rb");
    openstudio::path measureTestPath = measureTestDir / toPath(className + "_Test.rb");

    // write measure.rb
    {
      QFile file(toQString(measureScriptPath));
      bool opened = file.open(QIODevice::WriteOnly);
      if (!opened){
        LOG_AND_THROW("Cannot write measure.rb to '" << toString(measureScriptPath) << "'");
      }
      QTextStream textStream(&file);
      textStream << measureString;
      file.close();

      BCLFileReference measureScriptFileReference(measureScriptPath, true);
      measureScriptFileReference.setUsageType("script");
      measureScriptFileReference.setSoftwareProgramVersion(openStudioVersion());
      m_bclXML.addFile(measureScriptFileReference);
    }

    // write test
    {
      QFile file(toQString(measureTestPath));
      bool opened = file.open(QIODevice::WriteOnly);
      if (!opened){
        LOG_AND_THROW("Cannot write test file to '" << toString(measureTestPath) << "'");
      }
      QTextStream textStream(&file);
      textStream << testString;
      file.close();

      BCLFileReference measureTestFileReference(measureTestPath, true);
      measureTestFileReference.setUsageType("test");
      m_bclXML.addFile(measureTestFileReference);
    }

    // write test osm
    { 
      if (!testOSMString.isEmpty()){
        QFile file(toQString(testOSMPath));
        bool opened = file.open(QIODevice::WriteOnly);
        if (!opened){
          LOG_AND_THROW("Cannot write test osm file to '" << toString(testOSMPath) << "'");
        }
        QTextStream textStream(&file);
        textStream << testOSMString;
        file.close();

        BCLFileReference measureTestOSMFileReference(testOSMPath, true);
        measureTestOSMFileReference.setUsageType("test");
        m_bclXML.addFile(measureTestOSMFileReference);
      }
    }

    // write resource
    {
      if (!resourceFileString.isEmpty()){
        QFile file(toQString(resourceFilePath));
        bool opened = file.open(QIODevice::WriteOnly);
        if (!opened){
          LOG_AND_THROW("Cannot write resource file to '" << toString(resourceFilePath) << "'");
        }
        QTextStream textStream(&file);
        textStream << resourceFileString;
        file.close();

        BCLFileReference resourceFileReference(resourceFilePath, true);
        resourceFileReference.setUsageType("resource");
        m_bclXML.addFile(resourceFileReference);
      }
    }

    // set rest of measure fields
    m_bclXML.setName(name);
    m_bclXML.addTag(taxonomyTag);
    this->setMeasureType(measureType);
    this->setUsesSketchUpAPI(usesSketchUpAPI);
    m_bclXML.saveAs(measureXMLPath);
  }

  BCLMeasure::BCLMeasure(const openstudio::path& dir)
    : m_directory(boost::filesystem::system_complete(dir)),
      m_bclXML(BCLXMLType::MeasureXML)
  {
    openstudio::path xmlPath = m_directory / toPath("measure.xml");
    boost::optional<BCLXML> bclXML = BCLXML::load(xmlPath);

    if (!bclXML){
      LOG_AND_THROW("'" << toString(dir) << "' is not a measure");
    }

    m_bclXML = *bclXML;

    // check for required attributes
    boost::optional<Attribute> measureType = m_bclXML.getAttribute("Measure Type");
    if (!measureType){
      LOG_AND_THROW("'" << toString(dir) << "' is missing the required attribute \"Measure Type\"");
    }

    boost::optional<Attribute> usesSketchUpAPI = m_bclXML.getAttribute("Uses SketchUp API");
    if (!usesSketchUpAPI){
      LOG_AND_THROW("'" << toString(dir) << "' is missing the required attribute \"Uses SketchUp API\"");
    }

    // these may throw later, test here now
    usesSketchUpAPI->valueAsBoolean();

    if (m_bclXML.versionId() != bclXML->versionId()){
      LOG_AND_THROW("Measure version_id is no longer valid");
    }
  }

  BCLMeasure::~BCLMeasure()
  {}

  std::string BCLMeasure::className(const std::string& name)
  {
    QString str = toQString(name);

    bool startsWithLetter = false;
    for (int i = 0; i < str.size(); ++i){
      if (!str[i].isLetterOrNumber()){
        str[i] = ' ';
      }else if(i == 0){
        startsWithLetter = str[i].isLetter();
      }
    }

    QString result;
    QStringList parts = str.split(' ', QString::SkipEmptyParts);
    for (const QString& part : parts) {
      part[0].toUpper();
      result.append(part);
    }

    if (result.isEmpty()){
      result = QString("A") + createUUID().toString();
      result.replace('{',"").replace('}',"").replace('-',"");
    }else if (!startsWithLetter){
      result.prepend("A");
    }

    return result.toStdString();
  }

  boost::optional<BCLMeasure> BCLMeasure::load(const openstudio::path& dir)
  {
    boost::optional<BCLMeasure> result;
    try{
      result = BCLMeasure(dir);
    }catch(const std::exception&){
    }
    return result;
  }

  std::vector<BCLMeasure> BCLMeasure::patApplicationMeasures()
  {
    openstudio::path path = patApplicationMeasuresDir();
    return getMeasuresInDir(path);
  }

  openstudio::path BCLMeasure::patApplicationMeasuresDir()
  {
    openstudio::path result;
    if (applicationIsRunningFromBuildDirectory()){
      result = getApplicationSourceDirectory() / toPath("src/pat_app/Measures/");
    }else{
      result = getApplicationRunDirectory().parent_path() / openstudio::toPath("share/openstudio-" + openStudioVersion() + "/pat/Measures/");
    }
    return result;
  }

  BCLMeasure BCLMeasure::alternativeModelMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("ReplaceModel"));
  }

  BCLMeasure BCLMeasure::standardReportMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("StandardReports"));
  }

  BCLMeasure BCLMeasure::calibrationReportMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("CalibrationReports"));
  }

  std::vector<BCLMeasure> BCLMeasure::localBCLMeasures()
  {
    return LocalBCL::instance().measures();
  }

  std::vector<BCLMeasure> BCLMeasure::userMeasures()
  {
    openstudio::path path = userMeasuresDir();
    return getMeasuresInDir(path);
  }

  openstudio::path BCLMeasure::userMeasuresDir()
  {
    QSettings settings("OpenStudio", "BCLMeasure");
    QString value = settings.value("userMeasuresDir", QDir::homePath().append("/OpenStudio/Measures")).toString();
    openstudio::path result = toPath(value);
    return result;
  }

  bool BCLMeasure::setUserMeasuresDir(const openstudio::path& userMeasuresDir)
  {
    if (!userMeasuresDir.is_complete()){
      return false;
    }
    if (!exists(userMeasuresDir)){
      if (!QDir().mkpath(toQString(userMeasuresDir))){
        return false;
      }
    }
    if (!is_directory(userMeasuresDir)){
      return false;
    }

    QSettings settings("OpenStudio", "BCLMeasure");
    settings.setValue("userMeasuresDir", toQString(userMeasuresDir));
    return true;
  }

  void BCLMeasure::clearUserMeasuresDir()
  {
    QSettings settings("OpenStudio", "BCLMeasure");
    settings.remove("userMeasuresDir");
  }

  std::vector<BCLMeasure> BCLMeasure::getMeasuresInDir(openstudio::path dir)
  {
    LOG(Debug, "Loading measures in path: " << openstudio::toString(dir));
    std::vector<BCLMeasure> result;

    if (!exists(dir) || !is_directory(dir)){
      LOG(Debug, "Error Loading measures in path: " << openstudio::toString(dir));
      return result;
    }

    try{
      boost::filesystem::directory_iterator endit; // default construction yields past-the-end
      boost::filesystem::directory_iterator it(dir);
      for( ; it != endit; ++it )
      {
        if ( is_directory(it->status()) )
        {
          LOG(Debug, "Attempting to load measure: " << openstudio::toString(it->path()));
          boost::optional<BCLMeasure> measure = load(it->path());
          if (measure){
            LOG(Debug, "Success loading measure: " << openstudio::toString(it->path()));
            result.push_back(*measure);
          }
        }
      }
    }catch(const std::exception&){
    }

    return result;
  }

  std::string BCLMeasure::uid() const
  {
    return m_bclXML.uid();
  }

  UUID BCLMeasure::uuid() const {
    return toUUID("{" + uid() + "}");
  }

  std::string BCLMeasure::versionId() const
  {
    return m_bclXML.versionId();
  }

  UUID BCLMeasure::versionUUID() const {
    return toUUID("{" + versionId() + "}");
  }

  std::string BCLMeasure::name() const
  {
    return m_bclXML.name();
  }

  std::string BCLMeasure::description() const
  {
    return m_bclXML.description();
  }

  std::string BCLMeasure::modelerDescription() const
  {
    return m_bclXML.modelerDescription();
  }

  std::string BCLMeasure::taxonomyTag() const
  {
    std::string result;
    std::vector<std::string> tags = m_bclXML.tags();
    if (!tags.empty()){
      result = tags[0];
    }
    return result;
  }

  MeasureType BCLMeasure::measureType() const
  {
    boost::optional<Attribute> measureType = m_bclXML.getAttribute("Measure Type");
    OS_ASSERT(measureType);
    return MeasureType(measureType->valueAsString());
  }

  bool BCLMeasure::usesSketchUpAPI() const
  {
    boost::optional<Attribute> usesSketchUpAPI = m_bclXML.getAttribute("Uses SketchUp API");
    OS_ASSERT(usesSketchUpAPI);
    return usesSketchUpAPI->valueAsBoolean();
  }

  boost::optional<openstudio::path> BCLMeasure::primaryRubyScriptPath() const
  {
    openstudio::path result = m_directory / toPath("measure.rb");
    if (exists(result) && is_regular_file(result)){
      return result;
    }
    return boost::none;
  }

  std::vector<BCLFileReference> BCLMeasure::files() const
  {
    return m_bclXML.files();
  }

  std::vector<Attribute> BCLMeasure::attributes() const
  {
    return m_bclXML.attributes();
  }

  std::vector<std::string> BCLMeasure::tags() const
  {
    return m_bclXML.tags();
  }

  FileReferenceType BCLMeasure::inputFileType() const
  {
    FileReferenceType result = FileReferenceType::Unknown;
    MeasureType measureType = this->measureType();
    if (measureType == MeasureType::ModelMeasure){
      result = FileReferenceType::OSM;
    }else if (measureType == MeasureType::EnergyPlusMeasure){
      result = FileReferenceType::IDF;
    }else if (measureType == MeasureType::UtilityMeasure){
      // no-op
    }else if (measureType == MeasureType::ReportingMeasure){
      // DLM: is this right?
      // no-op
    }
    return result;
  }

  FileReferenceType BCLMeasure::outputFileType() const
  {
    FileReferenceType result = FileReferenceType::Unknown;
    MeasureType measureType = this->measureType();
    if (measureType == MeasureType::ModelMeasure){
      result = FileReferenceType::OSM;
    }else if (measureType == MeasureType::EnergyPlusMeasure){
      result = FileReferenceType::IDF;
    }else if (measureType == MeasureType::UtilityMeasure){
      // no-op
    }else if (measureType == MeasureType::ReportingMeasure){
      // DLM: is this right?
      // no-op
    }
    return result;
  }

  openstudio::path BCLMeasure::directory() const
  {
    return m_directory;
  }

  void BCLMeasure::changeUID()
  {
    m_bclXML.changeUID();
  }

  void BCLMeasure::incrementVersionId()
  {
    m_bclXML.incrementVersionId();
  }

  void BCLMeasure::setName(const std::string& name)
  {
    m_bclXML.setName(name);
  }

  void BCLMeasure::setDescription(const std::string& description)
  {
    m_bclXML.setDescription(description);
  }

  void BCLMeasure::setModelerDescription(const std::string& description)
  {
    m_bclXML.setModelerDescription(description);
  }

  void BCLMeasure::setTaxonomyTag(const std::string& taxonomyTag)
  {
    m_bclXML.clearTags();
    m_bclXML.addTag(taxonomyTag);
  }

  void BCLMeasure::setMeasureType(const MeasureType& measureType)
  {
    Attribute attribute("Measure Type", measureType.valueName());
    m_bclXML.addAttribute(attribute);
  }

  void BCLMeasure::setUsesSketchUpAPI(bool usesSketchUpAPI)
  {
    Attribute attribute("Uses SketchUp API", usesSketchUpAPI);
    m_bclXML.addAttribute(attribute);
  }

  bool BCLMeasure::checkForUpdates()
  {
    bool result = false;

    std::vector<BCLFileReference> filesToRemove;
    std::vector<BCLFileReference> filesToAdd;
    for (BCLFileReference file : m_bclXML.files()) {
      if (!exists(file.path())){
        result = true;
        filesToRemove.push_back(file);
      }else if (file.checkForUpdate()){
        result = true;
        filesToAdd.push_back(file);
      }
    }

    // look for new files and add them
    openstudio::path srcDir = m_directory / "tests";
    for (const QFileInfo &info : QDir(toQString(srcDir)).entryInfoList(QDir::Files))
    {
      openstudio::path srcItemPath = srcDir / toPath(info.fileName());
      if (!m_bclXML.hasFile(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("test");
        result = true;
        filesToAdd.push_back(file);
      }
    }

    srcDir = m_directory / "resources";
    for (const QFileInfo &info : QDir(toQString(srcDir)).entryInfoList(QDir::Files))
    {
      openstudio::path srcItemPath = srcDir / toPath(info.fileName());
      if (!m_bclXML.hasFile(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("resource");
        result = true;
        filesToAdd.push_back(file);
      }
    }

    for (const BCLFileReference& file : filesToRemove) {
      m_bclXML.removeFile(file.path());
    }

    for (const BCLFileReference& file : filesToAdd) {
      m_bclXML.addFile(file);
    }

    // increment version if anything changed
    if (result){
      m_bclXML.incrementVersionId();
    }

    return result;
  }

  bool BCLMeasure::operator==(const BCLMeasure& other) const
  {
    return ((this->uid() == other.uid()) && (this->versionId() == other.versionId()));
  }

  bool BCLMeasure::operator!=(const BCLMeasure& other) const {
    return !(operator==(other));
  }

  bool BCLMeasure::save() const
  {
    return m_bclXML.save();
  }

  boost::optional<BCLMeasure> BCLMeasure::clone(const openstudio::path& newDir) const
  {
    if (QFile::exists(toQString(newDir))){
      if (!isEmptyDirectory(newDir)){
        return boost::none;
      }
    }else{
      if (!QDir().mkpath(toQString(newDir))){
        return boost::none;
      }
    }

    removeDirectory(newDir);
    if (!copyDirectory(this->directory(), newDir)){
      return boost::none;
    }

    return BCLMeasure::load(newDir);
  }


} // openstudio
