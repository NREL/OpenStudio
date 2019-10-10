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

#include "BCLMeasure.hpp"
#include "LocalBCL.hpp"

#include "../core/PathHelpers.hpp"
#include "../core/FilesystemHelpers.hpp"
#include "../core/StringHelpers.hpp"
#include "../core/FileReference.hpp"
#include "../core/Assert.hpp"

#include <OpenStudio.hxx>

#include <QDir>
#include <QSettings>
#include <QRegularExpression>


#include <src/utilities/embedded_files.hxx>

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

    openstudio::path xmlPath = openstudio::filesystem::system_complete(m_bclXML.path());

    for (const auto &path : openstudio::filesystem::directory_files(source))
    {
      const auto srcItemPath = source / path;
      const auto dstItemPath = destination / path;

      if (m_bclXML.hasFile(srcItemPath) || (xmlPath == openstudio::filesystem::system_complete(srcItemPath)))
      {
        if (!openstudio::filesystem::copy_file_no_throw(srcItemPath, dstItemPath))
        {
          return false;
        }
      }
    }

    return true;
  }

  BCLMeasure::BCLMeasure(const std::string& name, const std::string& className, const openstudio::path& dir,
                         const std::string& taxonomyTag, MeasureType measureType,
                         const std::string& description, const std::string& modelerDescription)
    : m_directory(openstudio::filesystem::system_complete(dir)),
      m_bclXML(BCLXMLType::MeasureXML)
  {
    openstudio::path measureDocDir = dir / toPath("docs");
    openstudio::path measureTestDir = dir / toPath("tests");
    std::string lowerClassName = toUnderscoreCase(className);

    createDirectory(dir);
    createDirectory(measureDocDir);
    createDirectory(measureTestDir);

    // read in template files
    std::string measureTemplate;
    std::string licenseTemplate = ":/templates/common/LICENSE.md";
    std::string readmeTemplate = ":/templates/common/README.md.erb";
    std::string docTemplate = ":/templates/common/docs/.gitkeep";;
    std::string testTemplate;

    QString templateClassName;
    QString templateName = "NAME_TEXT";
    QString templateDescription = "DESCRIPTION_TEXT";
    QString templateModelerDescription = "MODELER_DESCRIPTION_TEXT";
    std::vector<BCLMeasureArgument> arguments;
    std::vector<BCLMeasureOutput> outputs;
    std::string testOSM;
    std::string testEPW;
    std::string resourceFile;

    openstudio::path testOSMPath;
    openstudio::path testEPWPath;
    openstudio::path resourceFilePath;
    if (measureType == MeasureType::ModelMeasure){
      measureTemplate = ":/templates/ModelMeasure/measure.rb";
      testTemplate = ":/templates/ModelMeasure/tests/model_measure_test.rb";
      testOSM = ":/templates/ModelMeasure/tests/example_model.osm";
      templateClassName = "ModelMeasureName";

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
      templateClassName = "EnergyPlusMeasureName";

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
      templateClassName = "UtilityMeasureName";

      createDirectory(dir / toPath("tests"));

    }else if (measureType == MeasureType::ReportingMeasure){
      measureTemplate = ":/templates/ReportingMeasure/measure.rb";
      testTemplate = ":/templates/ReportingMeasure/tests/reporting_measure_test.rb";
      testOSM = ":/templates/ReportingMeasure/tests/example_model.osm";
      testEPW = ":/templates/ReportingMeasure/tests/USA_CO_Golden-NREL.724666_TMY3.epw";
      resourceFile = ":/templates/ReportingMeasure/resources/report.html.in";
      templateClassName = "ReportingMeasureName";

      createDirectory(dir / toPath("tests"));
      testOSMPath = dir / toPath("tests/example_model.osm");
      testEPWPath = dir / toPath("tests/USA_CO_Golden-NREL.724666_TMY3.epw");

      createDirectory(dir / toPath("resources"));
      resourceFilePath = dir / toPath("resources/report.html.in");

    }

    QString measureString;
    if (!measureTemplate.empty()){
      measureString = toQString(::openstudio::embedded_files::getFileAsString(measureTemplate));
      measureString.replace(templateClassName, toQString(className));
      measureString.replace(templateName, toQString(name));
      measureString.replace(templateModelerDescription, toQString(modelerDescription)); // put first as this includes description tag
      measureString.replace(templateDescription, toQString(description));
    }

    QString licenseString;
    if (!licenseTemplate.empty()){
      licenseString = toQString(::openstudio::embedded_files::getFileAsString(licenseTemplate));
    }

    QString readmeString;
    if (!readmeTemplate.empty()){
      readmeString = toQString(::openstudio::embedded_files::getFileAsString(readmeTemplate));
    }

    QString docString;
    if (!docTemplate.empty()){
      docString = toQString(::openstudio::embedded_files::getFileAsString(docTemplate));
    }

    QString testString;
    if (!testTemplate.empty()){
      testString = toQString(::openstudio::embedded_files::getFileAsString(testTemplate));
      testString.replace(templateClassName, toQString(className));
    }

    QString testOSMString;
    if (!testOSM.empty()){
      testOSMString = toQString(::openstudio::embedded_files::getFileAsString(testOSM));
    }

    QString testEPWString;
    if (!testEPW.empty()){
      testEPWString = toQString(::openstudio::embedded_files::getFileAsString(testEPW));
    }

    QString resourceFileString;
    if (!resourceFile.empty()){
      resourceFileString = toQString(::openstudio::embedded_files::getFileAsString(resourceFile));
    }

    // write files
    openstudio::path measureXMLPath = dir / toPath("measure.xml");
    openstudio::path measureScriptPath = dir / toPath("measure.rb");
    openstudio::path measureLicensePath = dir / toPath("LICENSE.md");
    openstudio::path measureReadmePath = dir / toPath("README.md.erb");
    openstudio::path measureDocPath = measureDocDir / toPath(".gitkeep");
    openstudio::path measureTestPath = measureTestDir / toPath(lowerClassName + "_test.rb");

    // write measure.rb
    {
      openstudio::filesystem::ofstream file(measureScriptPath, std::ios_base::binary);
      if (!file.is_open()){
        LOG_AND_THROW("Cannot write measure.rb to '" << toString(measureScriptPath) << "'");
      }
      openstudio::filesystem::write(file, measureString);
      file.close();

      BCLFileReference measureScriptFileReference(measureScriptPath, true);
      measureScriptFileReference.setUsageType("script");
      measureScriptFileReference.setSoftwareProgramVersion(openStudioVersion());
      m_bclXML.addFile(measureScriptFileReference);
    }

    // write LICENSE.md
    {
      openstudio::filesystem::ofstream file(measureLicensePath, std::ios_base::binary);
      if (!file.is_open()){
        LOG_AND_THROW("Cannot write LICENSE.md to '" << toString(measureLicensePath) << "'");
      }
      openstudio::filesystem::write(file, licenseString);
      file.close();

      BCLFileReference measureLicenseFileReference(measureLicensePath, true);
      measureLicenseFileReference.setUsageType("license");
      m_bclXML.addFile(measureLicenseFileReference);
    }

    // write README.md.erb
    {
      openstudio::filesystem::ofstream file(measureReadmePath, std::ios_base::binary);
      if (!file.is_open()){
        LOG_AND_THROW("Cannot write README.md.erb to '" << toString(measureReadmePath) << "'");
      }
      openstudio::filesystem::write(file, readmeString);
      file.close();

      BCLFileReference measureReadmeFileReference(measureReadmePath, true);
      measureReadmeFileReference.setUsageType("readmeerb");
      m_bclXML.addFile(measureReadmeFileReference);
    }

    // write docs
    {
      openstudio::filesystem::ofstream file(measureDocPath, std::ios_base::binary);
      if (!file.is_open()){
        LOG_AND_THROW("Cannot write doc file to '" << toString(measureDocPath) << "'");
      }
      openstudio::filesystem::write(file, docString);
      file.close();

      BCLFileReference measureDocFileReference(measureDocPath, true);
      measureDocFileReference.setUsageType("doc");
      m_bclXML.addFile(measureDocFileReference);
    }

    // write test
    {
      openstudio::filesystem::ofstream file(measureTestPath, std::ios_base::binary);
      if (!file.is_open()){
        LOG_AND_THROW("Cannot write test file to '" << toString(measureTestPath) << "'");
      }
      openstudio::filesystem::write(file, testString);
      file.close();

      BCLFileReference measureTestFileReference(measureTestPath, true);
      measureTestFileReference.setUsageType("test");
      m_bclXML.addFile(measureTestFileReference);
    }

    // write test osm
    {
      if (!testOSMString.isEmpty()){
        openstudio::filesystem::ofstream file(testOSMPath, std::ios_base::binary);
        if (!file.is_open()){
          LOG_AND_THROW("Cannot write test osm file to '" << toString(testOSMPath) << "'");
        }
        openstudio::filesystem::write(file, testOSMString);
        file.close();

        BCLFileReference measureTestOSMFileReference(testOSMPath, true);
        measureTestOSMFileReference.setUsageType("test");
        m_bclXML.addFile(measureTestOSMFileReference);
      }
    }

    // write test epw
    {
      if (!testEPWString.isEmpty()){
        QFile file(toQString(testEPWPath));
        bool opened = file.open(QIODevice::WriteOnly);
        if (!opened){
          LOG_AND_THROW("Cannot write test epw file to '" << toString(testEPWPath) << "'");
        }
        QTextStream textStream(&file);
        textStream << testEPWString;
        file.close();

        BCLFileReference measureTestEPWFileReference(testEPWPath, true);
        measureTestEPWFileReference.setUsageType("test");
        m_bclXML.addFile(measureTestEPWFileReference);
      }
    }

    // write resource
    {
      if (!resourceFileString.isEmpty()){
        openstudio::filesystem::ofstream file(resourceFilePath, std::ios_base::binary);
        if (!file.is_open()){
          LOG_AND_THROW("Cannot write resource file to '" << toString(resourceFilePath) << "'");
        }
        openstudio::filesystem::write(file, resourceFileString);
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
    m_bclXML.setOutputs(outputs);
    m_bclXML.addTag(taxonomyTag);
    this->setMeasureType(measureType);

    // reset the checksum to trigger update even if nothing has changed
    m_bclXML.resetXMLChecksum();

    m_bclXML.saveAs(measureXMLPath);
  }

  BCLMeasure::BCLMeasure(const openstudio::path& dir)
    : m_directory(openstudio::filesystem::system_complete(dir)),
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
      result = QString("A") + openstudio::toQString(createUUID());
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
  /*
  std::vector<BCLMeasure> BCLMeasure::patApplicationMeasures()
  {
    openstudio::path path = patApplicationMeasuresDir();
    return getMeasuresInDir(path);
  }
  */
  /*
  openstudio::path BCLMeasure::patApplicationMeasuresDir()
  {
    openstudio::path result;
    if (applicationIsRunningFromBuildDirectory()){
      result = getApplicationSourceDirectory() / toPath("src/pat_app/Measures");
    }else{
      result = getApplicationRunDirectory().parent_path() / openstudio::toPath("share/openstudio-" + openStudioVersion() + "/pat/Measures");
    }
    return openstudio::filesystem::system_complete(result);
  }
  */
  /*
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
  */
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
    return openstudio::filesystem::system_complete(result);
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
      openstudio::filesystem::directory_iterator endit; // default construction yields past-the-end
      openstudio::filesystem::directory_iterator it(dir);
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

  boost::optional<DateTime> BCLMeasure::versionModified() const
  {
    return m_bclXML.versionModified();
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

  std::vector<BCLMeasureOutput> BCLMeasure::outputs() const
  {
    return m_bclXML.outputs();
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

  void BCLMeasure::setArguments(const std::vector<BCLMeasureArgument>& args)
  {
    m_bclXML.setArguments(args);
  }

  void BCLMeasure::setOutputs(const std::vector<BCLMeasureOutput>& outputs)
  {
    m_bclXML.setOutputs(outputs);
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
      openstudio::filesystem::ifstream file(*path, std::ios_base::binary);
      if (file.is_open()){

        QRegularExpression::PatternOptions opts = QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption;
        QString nameFunction = "\\1def name\n\\1  return \"" + toQString(name) + "\"\n\\1end";
        QString descriptionFunction = "\\1def description\n\\1  return \"" + toQString(description) + "\"\n\\1end";
        QString modelerDescriptionFunction = "\\1def modeler_description\n\\1  return \"" + toQString(modelerDescription) + "\"\n\\1end";

        fileString = openstudio::filesystem::read_as_QByteArray(file);

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

        openstudio::filesystem::ofstream ofile(*path, std::ios_base::binary);
        if (ofile.is_open()){
          openstudio::filesystem::write(ofile, fileString);
          ofile.close();
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

        openstudio::path oldPath = fileRef.path();
        openstudio::path newPath = oldPath;

        QString oldFileName = toQString(fileRef.path().filename());
        QString newFileName = oldFileName;
        if (!oldLowerClassName.empty() && !newLowerClassName.empty() && oldLowerClassName != newLowerClassName){
          QString temp = toQString(oldLowerClassName);
          int index = newFileName.lastIndexOf(temp);
          if (index >= 0){
            newFileName.replace(index, temp.size(), toQString(newLowerClassName));
            newPath = oldPath.parent_path() / toPath(newFileName);

            if (openstudio::filesystem::exists(newPath)) {
              // somehow this file already exists (e.g. no name change), don't clobber it
              newPath = oldPath;
            }else{
              openstudio::filesystem::copy_file(oldPath, newPath);
              openstudio::filesystem::remove(oldPath);
            }
          }
        }

        QString fileString;
        openstudio::filesystem::ifstream file(newPath, std::ios_base::binary);
        if (file.is_open()){

          fileString = openstudio::filesystem::read_as_QByteArray(file);
          if (!oldClassName.empty() && !newClassName.empty() && oldClassName != newClassName){
            // DLM: might also want to check that oldClassName is greater than 3 characters long?
            // should we be doing a more selective replace (like require leading space and trailing space, ., or :)?
            fileString.replace(toQString(oldClassName), toQString(newClassName));
          }
          file.close();

          openstudio::filesystem::ofstream file2(newPath, std::ios_base::binary);
          if (file2.is_open()){
            openstudio::filesystem::write(file2, fileString);
            file2.close();

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

  bool BCLMeasure::missingRequiredFields() const
  {
    bool missing = (
      uid().empty() ||
      versionId().empty() ||
      xmlChecksum().empty() ||
      name().empty() ||
      displayName().empty() ||
      className().empty() ||
      description().empty() ||
      modelerDescription().empty()
      );
    return missing;
  }

  bool BCLMeasure::checkForUpdatesFiles()
  {
    bool result = false;

    std::vector<BCLFileReference> filesToRemove;
    std::vector<BCLFileReference> filesToAdd;
    for (BCLFileReference file : m_bclXML.files()) {
      std::string filename = file.fileName();
      if (!exists(file.path())){
        result = true;
        // what if this is the measure.rb file?
        filesToRemove.push_back(file);
      }else if (filename.empty() || boost::starts_with(filename, ".")){
        if (filename == ".gitkeep") {
          // allow this file
        } else {
          result = true;
          filesToRemove.push_back(file);
        }
      }else if (file.checkForUpdate()){
        result = true;
        filesToAdd.push_back(file);
      }
    }

    // look for new files and add them
    openstudio::path srcDir = m_directory / "tests";
    openstudio::path ignoreDir = srcDir / "output";

    if (openstudio::filesystem::is_directory(srcDir)) {
      for (const auto &file : openstudio::filesystem::directory_files(srcDir))
      {
        openstudio::path srcItemPath = srcDir / file;
        openstudio::path parentPath = srcItemPath.parent_path();
        bool ignore = false;

        std::string filename = toString(file.filename());
        if (filename.empty() || boost::starts_with(filename, ".")){
          ignore = true;
        }

        while (!ignore && !parentPath.empty()){
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
    }

    srcDir = m_directory / "resources";
    if (openstudio::filesystem::is_directory(srcDir)) {
      for (const auto &file : openstudio::filesystem::directory_files(srcDir))
      {
        openstudio::path srcItemPath = srcDir / file;

        std::string filename = toString(file.filename());
        if (filename.empty() || boost::starts_with(filename, ".")){
          continue;
        }

        if (!m_bclXML.hasFile(srcItemPath)){
          BCLFileReference file(srcItemPath, true);
          file.setUsageType("resource");
          result = true;
          filesToAdd.push_back(file);
        }
      }
    }

    srcDir = m_directory / "docs";
    if (openstudio::filesystem::is_directory(srcDir)) {
      for (const auto &file : openstudio::filesystem::directory_files(srcDir))
      {
        openstudio::path srcItemPath = srcDir / file;

        std::string filename = toString(file.filename());
        if (filename.empty() || boost::starts_with(filename, ".")){
          if (filename == ".gitkeep") {
            // allow this file
          } else {
            continue;
          }

        }

        if (!m_bclXML.hasFile(srcItemPath)){
          BCLFileReference file(srcItemPath, true);
          file.setUsageType("doc");
          result = true;
          filesToAdd.push_back(file);
        }
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

    // check for LICENSE.md
    srcItemPath = m_directory / toPath("LICENSE.md");
    if (!m_bclXML.hasFile(srcItemPath)){
      if (exists(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("license");
        result = true;
        filesToAdd.push_back(file);
      }
    }

    // check for README.me
    srcItemPath = m_directory / toPath("README.md");
    if (!m_bclXML.hasFile(srcItemPath)){
      if (exists(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("readme");
        result = true;
        filesToAdd.push_back(file);
      }
    }

    // check for README.me.erb
    srcItemPath = m_directory / toPath("README.md.erb");
    if (!m_bclXML.hasFile(srcItemPath)){
      if (exists(srcItemPath)){
        BCLFileReference file(srcItemPath, true);
        file.setUsageType("readmeerb");
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
    if (openstudio::filesystem::exists(newDir)){
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
