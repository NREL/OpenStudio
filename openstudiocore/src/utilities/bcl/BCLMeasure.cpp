/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
#include "../core/StringHelpers.hpp"
#include "../core/FileReference.hpp"
#include "../data/Attribute.hpp"
#include "../core/Assert.hpp"
#include "../core/Checksum.hpp"

#include <OpenStudio.hxx>

#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QSettings>
#include <QRegularExpression>

#include <boost/filesystem.hpp>

namespace openstudio{

  void BCLMeasure::createDirectory(const openstudio::path& dir) const {
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

  bool BCLMeasure::copyDirectory(const path& source, const path& destination) const {

    if (!QDir().mkpath(toQString(destination)))
    {
      return false;
    }

    openstudio::path xmlPath = boost::filesystem::system_complete(m_bclXML.path());

    QDir srcDir(toQString(source));
    
    for (const QFileInfo &info : srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
      QString srcItemPath = toQString(source) + "/" + info.fileName();
      QString dstItemPath = toQString(destination) + "/" + info.fileName();
      if (info.isFile())
      {
        if (m_bclXML.hasFile(toPath(srcItemPath)) || (xmlPath == boost::filesystem::system_complete(toPath(srcItemPath))))
        {
          if (!QFile::copy(srcItemPath, dstItemPath))
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  BCLMeasure::BCLMeasure(const std::string& name, const std::string& className, const openstudio::path& dir,
                         const std::string& taxonomyTag, MeasureType measureType, 
                         const std::string& description, const std::string& modelerDescription)
    : m_directory(boost::filesystem::system_complete(dir)),
      m_bclXML(BCLXMLType::MeasureXML)
  {

    openstudio::path measureTestDir = dir / toPath("tests");
    std::string lowerClassName = toUnderscoreCase(className);

    createDirectory(dir);
    createDirectory(measureTestDir);

    // read in template files
    QString measureTemplate;
    QString testTemplate;
    QString templateClassName;
    QString templateName = "NAME_TEXT";
    QString templateDescription = "DESCRIPTION_TEXT";
    QString templateModelerDescription = "MODELER_DESCRIPTION_TEXT";
    std::vector<BCLMeasureArgument> arguments;
    QString testOSM;
    QString resourceFile;
    openstudio::path testOSMPath;
    openstudio::path resourceFilePath;
    if (measureType == MeasureType::ModelMeasure){
      measureTemplate = ":/templates/ModelMeasure/measure.rb";
      testTemplate = ":/templates/ModelMeasure/tests/model_measure_test.rb";
      testOSM = ":/templates/ModelMeasure/tests/example_model.osm";
      templateClassName = "ModelMeasure";

      createDirectory(dir / toPath("tests"));
      testOSMPath = dir / toPath("tests/example_model.osm");

      std::string argName("space_name");
      std::string argDisplayName("New space name");
      std::string argDescription("This name will be used as the name of the new space.");
      std::string argType("String");
      BCLMeasureArgument arg(argName, argDisplayName, argDescription, argType,
                             boost::none, true, false, boost::none,
                             std::vector<std::string>(), std::vector<std::string>(),
                             boost::none, boost::none);
      arguments.push_back(arg);

    }else if (measureType == MeasureType::EnergyPlusMeasure){
      measureTemplate = ":/templates/EnergyPlusMeasure/measure.rb";
      testTemplate = ":/templates/EnergyPlusMeasure/tests/energyplus_measure_test.rb";
      templateClassName = "EnergyPlusMeasure";

      createDirectory(dir / toPath("tests"));

      std::string argName("zone_name");
      std::string argDisplayName("New zone name");
      std::string argDescription("This name will be used as the name of the new zone.");
      std::string argType("String");
      BCLMeasureArgument arg(argName, argDisplayName, argDescription, argType, 
                             boost::none, true, false, boost::none, 
                             std::vector<std::string>(), std::vector<std::string>(),
                             boost::none, boost::none);
      arguments.push_back(arg);

    }else if (measureType == MeasureType::UtilityMeasure){
      measureTemplate = ":/templates/UtilityMeasure/measure.rb";
      testTemplate = ":/templates/UtilityMeasure/tests/utility_measure_test.rb";
      templateClassName = "UtilityMeasure";

      createDirectory(dir / toPath("tests"));

    }else if (measureType == MeasureType::ReportingMeasure){
      measureTemplate = ":/templates/ReportingMeasure/measure.rb";
      testTemplate = ":/templates/ReportingMeasure/tests/reporting_measure_test.rb";
      testOSM = ":/templates/ReportingMeasure/tests/example_model.osm";
      resourceFile = ":/templates/ReportingMeasure/resources/report.html.in";
      templateClassName = "ReportingMeasure";

      createDirectory(dir / toPath("tests"));
      testOSMPath = dir / toPath("tests/example_model.osm");

      createDirectory(dir / toPath("resources"));
      resourceFilePath = dir / toPath("resources/report.html.in");

    }

    QString measureString;
    if (!measureTemplate.isEmpty()){
      QFile file(measureTemplate);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        measureString = docIn.readAll();
        measureString.replace(templateClassName, toQString(className));
        measureString.replace(templateName, toQString(name));
        measureString.replace(templateModelerDescription, toQString(modelerDescription)); // put first as this includes description tag
        measureString.replace(templateDescription, toQString(description));
        file.close();
      }
    }

    QString testString;
    if (!testTemplate.isEmpty()){
      QFile file(testTemplate);
      if(file.open(QFile::ReadOnly)){
        QTextStream docIn(&file);
        testString = docIn.readAll();
        testString.replace(templateClassName, toQString(className));
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
    openstudio::path measureTestPath = measureTestDir / toPath(lowerClassName + "_test.rb");

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
    m_bclXML.setName(lowerClassName);
    m_bclXML.setDisplayName(name);
    m_bclXML.setClassName(className);
    m_bclXML.setDescription(description);
    m_bclXML.setModelerDescription(modelerDescription);
    m_bclXML.setArguments(arguments);
    m_bclXML.addTag(taxonomyTag);
    this->setMeasureType(measureType);

    // reset the checksum to trigger update even if nothing has changed
    m_bclXML.resetXMLChecksum();

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
    std::vector<Attribute> measureTypes = m_bclXML.getAttributes("Measure Type");
    if (measureTypes.empty()){
      LOG_AND_THROW("'" << toString(dir) << "' is missing the required attribute \"Measure Type\"");
    } else if (measureTypes.size() > 1) {
      LOG_AND_THROW("'" << toString(dir) << "' has multiple copies of required attribute \"Measure Type\"");
    } else if (measureTypes[0].valueType() != AttributeValueType::String) {
      LOG_AND_THROW("'" << toString(dir) << "' has multiple copies of required attribute \"Measure Type\"");
    }

    if (m_bclXML.xmlChecksum().empty()){
      // this will set the checksum but not increment the version id
      m_bclXML.checkForUpdatesXML();
    }

    // check that opening the file did not modify it
    if (m_bclXML.versionId() != bclXML->versionId()){
      // DLM: should this be an assert instead?
      LOG_AND_THROW("Measure version_id is no longer valid");
    }
  }

  BCLMeasure::~BCLMeasure()
  {}

  std::string BCLMeasure::makeClassName(const std::string& name)
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
    for (QString part : parts) {
      part[0] = part[0].toUpper();
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
      result = getApplicationSourceDirectory() / toPath("src/pat_app/Measures");
    }else{
      result = getApplicationRunDirectory().parent_path() / openstudio::toPath("share/openstudio-" + openStudioVersion() + "/pat/Measures");
    }
    return boost::filesystem::system_complete(result);
  }

  BCLMeasure BCLMeasure::alternativeModelMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("ReplaceModel"));
  }

  BCLMeasure BCLMeasure::reportRequestMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("ReportRequest"));
  }

  BCLMeasure BCLMeasure::standardReportMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("StandardReports"));
  }

  BCLMeasure BCLMeasure::calibrationReportMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("CalibrationReports"));
  }

  BCLMeasure BCLMeasure::radianceMeasure() {
    return BCLMeasure(patApplicationMeasuresDir() / toPath("RadianceMeasure"));
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
    return boost::filesystem::system_complete(result);
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

  std::vector<std::string> BCLMeasure::suggestedIntendedSoftwareTools()
  {
    std::vector<std::string> result;
    result.push_back("Apply Measure Now");
    result.push_back("OpenStudio Application");
    result.push_back("Parametric Analysis Tool");
    result.push_back("Analysis Spreadsheet");
    return result;
  }

  std::vector<std::string> BCLMeasure::suggestedIntendedUseCases()
  {
    std::vector<std::string> result;
    result.push_back("Model Articulation");
    result.push_back("Calibration");
    result.push_back("Sensitivity Analysis");
    result.push_back("New Construction EE");
    result.push_back("Retrofit EE");
    result.push_back("Automatic Report Generation");
    return result;
  }

  std::vector<std::string> BCLMeasure::suggestedFirstLevelTaxonomyTerms()
  {
    std::vector<std::string> result;
    result.reserve(12);

    result.push_back("Envelope");
    result.push_back("Electric Lighting");
    result.push_back("Equipment");
    result.push_back("People");
    result.push_back("HVAC");
    result.push_back("Refrigeration");
    result.push_back("Service Water Heating");
    result.push_back("Onsite Power Generation");
    result.push_back("Whole Building");
    result.push_back("Economics");
    result.push_back("Reporting");

    return result;
  }

  std::vector<std::string> BCLMeasure::suggestedSecondLevelTaxonomyTerms(const std::string& firstLevelTaxonomyTerm)
  {
    std::vector<std::string> result;
    result.reserve(12);

    if (firstLevelTaxonomyTerm == "Envelope"){
      result.push_back("Form");
      result.push_back("Opaque");
      result.push_back("Fenestration");
      result.push_back("Construction Sets");
      result.push_back("Daylighting");
      result.push_back("Infiltration");
    } else if (firstLevelTaxonomyTerm == "Electric Lighting"){
      result.push_back("Electric Lighting Controls");
      result.push_back("Lighting Equipment");
    } else if (firstLevelTaxonomyTerm == "Equipment"){
      result.push_back("Equipment Controls");
      result.push_back("Electric Equipment");
      result.push_back("Gas Equipment");
    } else if (firstLevelTaxonomyTerm == "People"){
      result.push_back("Characteristics");
      result.push_back("People Schedules");
    } else if (firstLevelTaxonomyTerm == "HVAC"){
      result.push_back("HVAC Controls");
      result.push_back("Heating");
      result.push_back("Cooling");
      result.push_back("Heat Rejection");
      result.push_back("Energy Recovery");
      result.push_back("Distribution");
      result.push_back("Ventilation");
      result.push_back("Whole System");
    } else if (firstLevelTaxonomyTerm == "Refrigeration"){
      result.push_back("Refrigeration Controls");
      result.push_back("Cases and Walkins");
      result.push_back("Compressors");
      result.push_back("Condensers");
      result.push_back("Heat Reclaim");
    } else if (firstLevelTaxonomyTerm == "Service Water Heating"){
      result.push_back("Water Use");
      result.push_back("Water Heating");
      result.push_back("Distribution");
    } else if (firstLevelTaxonomyTerm == "Onsite Power Generation"){
      result.push_back("Photovoltaic");
    } else if (firstLevelTaxonomyTerm == "Whole Building"){
      result.push_back("Whole Building Schedules");
      result.push_back("Space Types");
    } else if (firstLevelTaxonomyTerm == "Economics"){
      result.push_back("Life Cycle Cost Analysis");
    } else if (firstLevelTaxonomyTerm == "Reporting"){
      result.push_back("QAQC");
      result.push_back("Troubleshooting");
    }

    return result;
  }

  std::vector<BCLMeasure> BCLMeasure::getMeasuresInDir(openstudio::path dir)
  {
    LOG(Debug, "Loading measures in path: " << openstudio::toString(dir));
    std::vector<BCLMeasure> result;

    if (!exists(dir) || !is_directory(dir)){
      LOG(Debug, "Error Loading measures in path: " << openstudio::toString(dir));
      return result;
    }

    if (isNetworkPath(dir) && !isNetworkPathAvailable(dir)) {
      LOG(Debug, "Error Loading measures in unavailable network location: " << openstudio::toString(dir));
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

  openstudio::path BCLMeasure::directory() const
  {
    return m_directory;
  }

  boost::optional<std::string> BCLMeasure::error() const
  {
    return m_bclXML.error();
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

  std::string BCLMeasure::xmlChecksum() const
  {
    return m_bclXML.xmlChecksum();
  }

  std::string BCLMeasure::name() const
  {
    return m_bclXML.name();
  }

  std::string BCLMeasure::displayName() const
  {
    return m_bclXML.displayName();
  }

  std::string BCLMeasure::className() const
  {
    return m_bclXML.className();
  }

  std::string BCLMeasure::description() const
  {
    return m_bclXML.description();
  }

  std::string BCLMeasure::modelerDescription() const
  {
    return m_bclXML.modelerDescription();
  }

  std::vector<BCLMeasureArgument> BCLMeasure::arguments() const
  {
    return m_bclXML.arguments();
  }

  std::vector<std::string> BCLMeasure::tags() const
  {
    return m_bclXML.tags();
  }

  std::vector<Attribute> BCLMeasure::attributes() const
  {
    return m_bclXML.attributes();
  }

  std::vector<BCLFileReference> BCLMeasure::files() const
  {
    return m_bclXML.files();
  }

  void BCLMeasure::setError(const std::string& error)
  {
    m_bclXML.setError(error);
  }

  void BCLMeasure::resetError()
  {
    m_bclXML.resetError();
  }

  void BCLMeasure::setName(const std::string& name)
  {
    m_bclXML.setName(name);
  }

  void BCLMeasure::setDisplayName(const std::string& displayName)
  {
    m_bclXML.setDisplayName(displayName);
  }

  void BCLMeasure::setClassName(const std::string& className)
  {
    m_bclXML.setClassName(className);
  }

  void BCLMeasure::setDescription(const std::string& description)
  {
    m_bclXML.setDescription(description);
  }

  void BCLMeasure::setModelerDescription(const std::string& description)
  {
    m_bclXML.setModelerDescription(description);
  }

  void BCLMeasure::setArguments(const std::vector<BCLMeasureArgument>& arguments)
  {
    m_bclXML.setArguments(arguments);
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

   void BCLMeasure::setTaxonomyTag(const std::string& taxonomyTag)
   {
     m_bclXML.clearTags();
     m_bclXML.addTag(taxonomyTag);
   }

  MeasureType BCLMeasure::measureType() const
  {
    std::vector<Attribute> measureTypes = m_bclXML.getAttributes("Measure Type");
    OS_ASSERT(measureTypes.size() == 1);
    return MeasureType(measureTypes[0].valueAsString());
  }

  void BCLMeasure::setMeasureType(const MeasureType& measureType)
  {
    const std::string attributeName("Measure Type");
    Attribute attribute(attributeName, measureType.valueName());
    m_bclXML.removeAttributes(attributeName);
    m_bclXML.addAttribute(attribute);
  }

  std::vector<std::string> BCLMeasure::intendedSoftwareTools() const
  {
    std::vector<std::string> result;
    std::vector<Attribute> attributes = m_bclXML.getAttributes("Intended Software Tool");
    for (const Attribute& attribute : attributes){
      result.push_back(attribute.valueAsString());
    }
    return result;
  }

  /// Returns values of any "Intended Use Case" attributes
  std::vector<std::string> BCLMeasure::intendedUseCases() const
  {
    std::vector<std::string> result;
    std::vector<Attribute> attributes = m_bclXML.getAttributes("Intended Use Case");
    for (const Attribute& attribute : attributes){
      result.push_back(attribute.valueAsString());
    }
    return result;
  }

  boost::optional<openstudio::path> BCLMeasure::primaryRubyScriptPath() const
  {
    openstudio::path result = m_directory / toPath("measure.rb");
    if (exists(result) && is_regular_file(result)){
      return result;
    }
    return boost::none;
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

  void BCLMeasure::changeUID()
  {
    m_bclXML.changeUID();
  }

  void BCLMeasure::incrementVersionId()
  {
    m_bclXML.incrementVersionId();
  }

  bool BCLMeasure::updateMeasureScript(const MeasureType& oldMeasureType, const MeasureType& newMeasureType, 
                                       const std::string& oldClassName, const std::string& newClassName, 
                                       const std::string& name, const std::string& description, 
                                       const std::string& modelerDescription)
  {
    boost::optional<openstudio::path> path = primaryRubyScriptPath();
    if (path && exists(*path)){

      QString fileString;
      QFile file(toQString(*path));
      if (file.open(QFile::ReadOnly)){

        QRegularExpression::PatternOptions opts = QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption;
        QString nameFunction = "\\1def name\n\\1  return \" " + toQString(name) + "\"\n\\1end";
        QString descriptionFunction = "\\1def description\n\\1  return \"" + toQString(description) + "\"\n\\1end";
        QString modelerDescriptionFunction = "\\1def modeler_description\n\\1  return \"" + toQString(modelerDescription) + "\"\n\\1end";

        QTextStream docIn(&file);
        fileString = docIn.readAll();

        if (oldMeasureType != newMeasureType){
          fileString.replace(toQString(oldMeasureType.valueName()), toQString(newMeasureType.valueName()));
        }

        if (!oldClassName.empty() && !newClassName.empty() && oldClassName != newClassName){
          // DLM: might also want to check that oldClassName is greater than 3 characters long?
          // should we be doing a more selective replace (like require leading space and trailing space, ., or :)?
          fileString.replace(toQString(oldClassName), toQString(newClassName));
        }

        fileString.replace(QRegularExpression("^(\\s+)def\\s+name(.*?)end[\\s#]?$", opts), nameFunction);
        fileString.replace(QRegularExpression("^(\\s+)def\\s+description(.*?)end[\\s#]?$", opts), descriptionFunction);
        fileString.replace(QRegularExpression("^(\\s+)def\\s+modeler_description(.*?)end[\\s#]?$", opts), modelerDescriptionFunction);
        file.close();

        if (file.open(QIODevice::WriteOnly)){
          QTextStream textStream(&file);
          textStream << fileString;
          file.close();
          return true;
        }
      }
    }

    return false;
  }
  
  bool BCLMeasure::updateMeasureTests(const std::string& oldClassName, const std::string& newClassName)
  {
    bool result = true;
    for (const BCLFileReference& fileRef : files()){
      std::string usageType = fileRef.usageType();
      if (usageType != "test"){
        continue;
      }

      if (exists(fileRef.path())){

        std::string oldLowerClassName = toUnderscoreCase(oldClassName);
        std::string newLowerClassName = toUnderscoreCase(newClassName);

        QString oldPath = toQString(fileRef.path());
        QString newPath = oldPath;
        if (!oldLowerClassName.empty() && !newLowerClassName.empty() && oldLowerClassName != newLowerClassName){
          QString temp = toQString(oldLowerClassName);
          int index = newPath.lastIndexOf(temp);
          if (index >= 0){
            newPath.replace(index, temp.size(), toQString(newLowerClassName));
          }
        }

        if (QFile::exists(newPath)) {
          // somehow this file already exists, don't clobber it
          newPath = oldPath;
        }else{
          QFile::copy(oldPath, newPath);
          QFile::remove(oldPath);
        }
        
        QString fileString;
        QFile file(newPath);
        if (file.open(QFile::ReadOnly)){

          QTextStream docIn(&file);
          fileString = docIn.readAll();
          if (!oldClassName.empty() && !newClassName.empty() && oldClassName != newClassName){
            // DLM: might also want to check that oldClassName is greater than 3 characters long?
            // should we be doing a more selective replace (like require leading space and trailing space, ., or :)?
            fileString.replace(toQString(oldClassName), toQString(newClassName));
          }
          file.close();

          if (file.open(QIODevice::WriteOnly)){
            QTextStream textStream(&file);
            textStream << fileString;
            file.close();

          } else {
            result = false;
          }
        } else {
          result = false;
        }
      } else{
        result = false;
      }
    }
    return result;
  }

  void BCLMeasure::clearFiles()
  {
    m_bclXML.clearFiles();
  }
  
  void BCLMeasure::addAttribute(const Attribute& attribute)
  {
    m_bclXML.addAttribute(attribute);
  }

  std::vector<Attribute> BCLMeasure::getAttributes(const std::string& name) const
  {
    return m_bclXML.getAttributes(name);
  }

  bool BCLMeasure::removeAttributes(const std::string& name)
  {
    return m_bclXML.removeAttributes(name);
  }
 
  void BCLMeasure::clearAttributes()
  {
    m_bclXML.clearAttributes();
  }

  bool BCLMeasure::checkForUpdatesFiles()
  {
    bool result = false;

    std::vector<BCLFileReference> filesToRemove;
    std::vector<BCLFileReference> filesToAdd;
    for (BCLFileReference file : m_bclXML.files()) {
      if (!exists(file.path())){
        result = true;
        // what if this is the measure.rb file?
        filesToRemove.push_back(file);
      }else if (file.checkForUpdate()){
        result = true;
        filesToAdd.push_back(file);
      }
    }

    // look for new files and add them
    openstudio::path srcDir = m_directory / "tests";
    openstudio::path ignoreDir = srcDir / "output";
    for (const QFileInfo &info : QDir(toQString(srcDir)).entryInfoList(QDir::Files))
    {
      openstudio::path srcItemPath = srcDir / toPath(info.fileName());
      openstudio::path parentPath = srcItemPath.parent_path();
      bool ignore = false;
      while (!parentPath.empty()){
        if (parentPath == ignoreDir){
          ignore = true;
          break;
        }
        parentPath = parentPath.parent_path();
      }

      if (ignore){
        continue;
      }

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

    // check for measure.rb
    openstudio::path srcItemPath = m_directory / toPath("measure.rb");
    if (!m_bclXML.hasFile(srcItemPath)){
      if (exists(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("script");
        // we don't know what the actual version this was created for, we also don't know minimum version
        file.setSoftwareProgramVersion(openStudioVersion());
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

  bool BCLMeasure::checkForUpdatesXML()
  {
    return m_bclXML.checkForUpdatesXML();
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

    // DLM: do not copy entire directory, only copy tracked files
    if (!this->copyDirectory(this->directory(), newDir)){
      return boost::none;
    }

    openstudio::path tests = toPath("tests");
    if (exists(this->directory() / tests) && !this->copyDirectory(this->directory() / tests, newDir / tests)){
      return boost::none;
    }

    openstudio::path resources = toPath("resources");
    if (exists(this->directory() / resources) && !this->copyDirectory(this->directory() / resources, newDir / resources)){
      return boost::none;
    }

    return BCLMeasure::load(newDir);
  }

} // openstudio
