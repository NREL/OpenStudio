/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCLMeasure.hpp"
#include "LocalBCL.hpp"
#include "BCLFileReference.hpp"

#include "../core/Filesystem.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/FilesystemHelpers.hpp"
#include "../core/StringHelpers.hpp"
#include "../core/FileReference.hpp"
#include "../core/Assert.hpp"

#include <OpenStudio.hxx>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <src/utilities/embedded_files.hxx>

#include <array>
#include <algorithm>
#include <cctype>  // std::isalpha, std::isdigit
#include <string_view>
#include <vector>

#include <json/json.h>

namespace openstudio {

/*****************************************************************************************************************************************************
*                                    F I L T E R I N G    R U L E S    F O R    W H A T    I S    A L L O W E D                                      *
*****************************************************************************************************************************************************/

/***************
*  R U L E S  *
***************/

static constexpr std::array<std::pair<std::string_view, std::string_view>, 5> rootToUsageTypeMap{{
  // fileName, usageType
  {"measure.rb", "script"},
  {"measure.py", "script"},
  {"LICENSE.md", "license"},
  {"README.md", "readme"},
  {"README.md.erb", "readmeerb"}
  // ".gitkeep"      // assuming .gitkeep outside a subfolder makes zero sense...
  // "measure.xml"   // not included in itself!
}};

static constexpr std::array<std::pair<std::string_view, std::string_view>, 3> approvedSubFolderToUsageMap{{
  {"docs", "doc"},
  {"tests", "test"},
  {"resources", "resource"},
}};

static constexpr std::array<std::string_view, 3> ignoredSubFolders{"tests/output", "__pycache__", "tests/__pycache__"};

// TODO: do we want to keep ignoring the docs/ directory?
static constexpr std::array<std::string_view, 1> usageTypesIgnoredOnClone{"doc"};

/******************************************************************
*  C A L C U L A T E D    T H I N G S    A N D    H E L P E R S  *
******************************************************************/

static constexpr std::array<std::string_view, rootToUsageTypeMap.size()> approvedRootFiles = []() {
  std::array<std::string_view, rootToUsageTypeMap.size()> result;
  for (size_t i = 0; i < rootToUsageTypeMap.size(); ++i) {
    result[i] = rootToUsageTypeMap[i].first;
  }
  return result;
}();

static constexpr std::array<std::string_view, approvedSubFolderToUsageMap.size()> approvedSubFolders = []() {
  std::array<std::string_view, approvedSubFolderToUsageMap.size()> result;
  for (size_t i = 0; i < approvedSubFolderToUsageMap.size(); ++i) {
    result[i] = approvedSubFolderToUsageMap[i].first;
  }
  return result;
}();

bool BCLMeasure::isIgnoredFileName(const std::string& fileName) {
  return (fileName.empty() || (boost::starts_with(fileName, ".") && (fileName != ".gitkeep")));
}

bool BCLMeasure::isApprovedFile(const openstudio::path& absoluteFilePath, const openstudio::path& measureDir) {

  std::string fileName = toString(absoluteFilePath.filename());
  if (isIgnoredFileName(fileName)) {
    return false;
  }

  openstudio::path parentPath = absoluteFilePath.parent_path();

  if (parentPath == measureDir) {
    if (std::find_if(approvedRootFiles.cbegin(), approvedRootFiles.cend(), [&fileName](const auto& sv) { return fileName == std::string(sv); })
        == approvedRootFiles.cend()) {
      LOG(Warn, absoluteFilePath << " is not one of the approved top level files");
      return false;
    }
  } else {
    bool ignored = true;
    // Keep going back up the directory structure, until you either find one excluded folder or an approved one

    auto isPathInContainer = [&measureDir, &parentPath](const auto& container) -> bool {
      return std::find_if(container.begin(), container.end(),
                          [&measureDir, &parentPath](const auto& subFolderPath) {
                            auto fullSubFolderPath = measureDir / toPath(subFolderPath);
                            return parentPath == fullSubFolderPath;
                          })
             != container.end();
    };

    // Don't go back up further than the measure directory root
    while (!parentPath.empty() && (parentPath != measureDir)) {
      if (isPathInContainer(ignoredSubFolders)) {
        ignored = true;
        break;
      } else if (isPathInContainer(approvedSubFolders)) {
        ignored = false;
        break;
      }
      parentPath = parentPath.parent_path();
    }

    if (ignored) {
      LOG(Warn, absoluteFilePath << " is not inside an approved subfolder.");
      return false;
    }
  }

  return true;
}
/****************************************************************************************************************************************************/

void BCLMeasure::createDirectory(const openstudio::path& dir) {
  if (exists(dir)) {
    if (!isEmptyDirectory(dir)) {
      LOG_AND_THROW("'" << toString(dir) << "' exists but is not an empty directory");
    }
  } else {
    if (!openstudio::filesystem::create_directories(dir)) {
      LOG_AND_THROW("'" << toString(dir) << "' cannot be created as an empty directory");
    }
  }
}

BCLMeasure::BCLMeasure(const std::string& name, const std::string& className, const openstudio::path& dir, const std::string& taxonomyTag,
                       MeasureType measureType, const std::string& description, const std::string& modelerDescription,
                       MeasureLanguage measureLanguage)
  : m_directory(openstudio::filesystem::system_complete(dir)), m_bclXML(BCLXMLType::MeasureXML) {

  // Avoid potential problems, since toPath("/path/to/dir/") != toPath("/path/to/dir")
  m_directory.remove_trailing_separator();

  openstudio::path measureDocRelativeDir = toPath("docs");
  openstudio::path measureTestRelativeDir = toPath("tests");
  std::string lowerClassName = toUnderscoreCase(className);

  createDirectory(m_directory);
  createDirectory(m_directory / measureDocRelativeDir);
  createDirectory(m_directory / measureTestRelativeDir);

  // read in template files
  std::string measureTemplate;
  std::string licenseTemplate = ":/templates/common/LICENSE.md";
  std::string readmeTemplate;
  if (measureLanguage == MeasureLanguage::Ruby) {
    readmeTemplate = ":/templates/common/README.md.erb";
  }
  std::string docTemplate = ":/templates/common/docs/.gitkeep";
  std::string testTemplate;

  std::string templateClassName;
  std::vector<BCLMeasureArgument> arguments;
  std::vector<BCLMeasureOutput> outputs;
  std::string testOSM;
  std::string testEPW;
  std::string resourceFile;

  openstudio::path testOSMRelativePath;
  openstudio::path testEPWRelativePath;
  openstudio::path resourceFileRelativePath;
  if (measureType == MeasureType::ModelMeasure) {
    if (measureLanguage == MeasureLanguage::Ruby) {
      measureTemplate = ":/templates/ModelMeasure/measure.rb";
      testTemplate = ":/templates/ModelMeasure/tests/model_measure_test.rb";
    } else if (measureLanguage == MeasureLanguage::Python) {
      measureTemplate = ":/templates/ModelMeasure/measure.py";
      testTemplate = ":/templates/ModelMeasure/tests/test_model_measure.py";
    }
    testOSM = ":/templates/ModelMeasure/tests/example_model.osm";
    templateClassName = "ModelMeasureName";

    testOSMRelativePath = toPath("tests/example_model.osm");

    std::string argName("space_name");
    std::string argDisplayName("New space name");
    std::string argDescription("This name will be used as the name of the new space.");
    std::string argType("String");
    BCLMeasureArgument arg(argName, argDisplayName, argDescription, argType, boost::none, true, false, boost::none, std::vector<std::string>(),
                           std::vector<std::string>(), boost::none, boost::none);
    arguments.push_back(arg);

  } else if (measureType == MeasureType::EnergyPlusMeasure) {
    if (measureLanguage == MeasureLanguage::Ruby) {
      measureTemplate = ":/templates/EnergyPlusMeasure/measure.rb";
      testTemplate = ":/templates/EnergyPlusMeasure/tests/energyplus_measure_test.rb";
    } else if (measureLanguage == MeasureLanguage::Python) {
      measureTemplate = ":/templates/EnergyPlusMeasure/measure.py";
      testTemplate = ":/templates/EnergyPlusMeasure/tests/test_energyplus_measure.py";
    }
    templateClassName = "EnergyPlusMeasureName";

    std::string argName("zone_name");
    std::string argDisplayName("New zone name");
    std::string argDescription("This name will be used as the name of the new zone.");
    std::string argType("String");
    BCLMeasureArgument arg(argName, argDisplayName, argDescription, argType, boost::none, true, false, boost::none, std::vector<std::string>(),
                           std::vector<std::string>(), boost::none, boost::none);
    arguments.push_back(arg);

  } else if (measureType == MeasureType::UtilityMeasure) {
    measureTemplate = ":/templates/UtilityMeasure/measure.rb";
    testTemplate = ":/templates/UtilityMeasure/tests/utility_measure_test.rb";
    templateClassName = "UtilityMeasureName";

  } else if (measureType == MeasureType::ReportingMeasure) {
    if (measureLanguage == MeasureLanguage::Ruby) {
      measureTemplate = ":/templates/ReportingMeasure/measure.rb";
      testTemplate = ":/templates/ReportingMeasure/tests/reporting_measure_test.rb";
      resourceFile = ":/templates/ReportingMeasure/resources/report.html.in";
    } else if (measureLanguage == MeasureLanguage::Python) {
      measureTemplate = ":/templates/ReportingMeasure/measure.py";
      testTemplate = ":/templates/ReportingMeasure/tests/test_reporting_measure.py";
    }
    testOSM = ":/templates/ReportingMeasure/tests/example_model.osm";
    testEPW = ":/templates/ReportingMeasure/tests/USA_CO_Golden-NREL.724666_TMY3.epw";
    templateClassName = "ReportingMeasureName";

    testOSMRelativePath = toPath("tests/example_model.osm");
    testEPWRelativePath = toPath("tests/USA_CO_Golden-NREL.724666_TMY3.epw");

    createDirectory(m_directory / toPath("resources"));
    resourceFileRelativePath = toPath("resources/report.html.in");
  }

  std::string measureString;
  if (!measureTemplate.empty()) {
    measureString = ::openstudio::embedded_files::getFileAsString(measureTemplate);
    boost::replace_all(measureString, templateClassName, className);
    std::string templateName = "NAME_TEXT";
    std::string templateDescription = "DESCRIPTION_TEXT";
    std::string templateModelerDescription = "MODELER_DESCRIPTION_TEXT";
    boost::replace_all(measureString, templateName, name);
    boost::replace_all(measureString, templateModelerDescription, modelerDescription);  // put first as this includes description tag
    boost::replace_all(measureString, templateDescription, description);
  }

  std::string licenseString;
  // cppcheck-suppress knownConditionTrueFalse
  if (!licenseTemplate.empty()) {
    licenseString = ::openstudio::embedded_files::getFileAsString(licenseTemplate);
  }

  std::string readmeString;
  // cppcheck-suppress knownConditionTrueFalse
  if (!readmeTemplate.empty()) {
    readmeString = ::openstudio::embedded_files::getFileAsString(readmeTemplate);
  }

  std::string docString;
  // cppcheck-suppress knownConditionTrueFalse
  if (!docTemplate.empty()) {
    docString = ::openstudio::embedded_files::getFileAsString(docTemplate);
  }

  std::string testString;
  if (!testTemplate.empty()) {
    testString = openstudio::replace(::openstudio::embedded_files::getFileAsString(testTemplate), templateClassName, className);
  }

  std::string testOSMString;
  if (!testOSM.empty()) {
    testOSMString = ::openstudio::embedded_files::getFileAsString(testOSM);
  }

  std::string testEPWString;
  if (!testEPW.empty()) {
    testEPWString = ::openstudio::embedded_files::getFileAsString(testEPW);
  }

  std::string resourceFileString;
  if (!resourceFile.empty()) {
    resourceFileString = ::openstudio::embedded_files::getFileAsString(resourceFile);
  }

  // write files
  auto writeFile = [this](const openstudio::path& relativeFilePath, const std::string& usageType, const std::string& contentText,
                          bool addVersion = false) {
    openstudio::path absoluteFilePath = m_directory / relativeFilePath;
    openstudio::filesystem::ofstream file(absoluteFilePath, std::ios_base::binary);
    if (!file.is_open()) {
      LOG_AND_THROW("Cannot write " << usageType << " file to '" << toString(absoluteFilePath) << "'");
    }
    file << contentText;
    file.close();

    BCLFileReference fileRef(m_directory, relativeFilePath, true);
    fileRef.setUsageType(usageType);
    if (addVersion) {
      fileRef.setSoftwareProgramVersion(openStudioVersion());
    }
    m_bclXML.addFile(fileRef);
  };

  if (measureLanguage == MeasureLanguage::Ruby) {
    writeFile(toPath("measure.rb"), "script", measureString, true);
    writeFile(measureTestRelativeDir / toPath(lowerClassName + "_test.rb"), "test", testString);
    writeFile(toPath("README.md.erb"), "readmeerb", readmeString);
  } else if (measureLanguage == MeasureLanguage::Python) {
    writeFile(toPath("measure.py"), "script", measureString, true);
    writeFile(measureTestRelativeDir / toPath("test_" + lowerClassName + ".py"), "test", testString);
  }
  writeFile(toPath("LICENSE.md"), "license", licenseString);
  writeFile(measureDocRelativeDir / toPath(".gitkeep"), "doc", docString);

  if (!testOSMString.empty()) {
    writeFile(testOSMRelativePath, "test", testOSMString);
  }
  if (!testEPWString.empty()) {
    writeFile(testEPWRelativePath, "test", testEPWString);
  }

  if (!resourceFileString.empty()) {
    writeFile(resourceFileRelativePath, "resource", resourceFileString);
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
  this->setMeasureLanguage(measureLanguage);

  // reset the checksum to trigger update even if nothing has changed
  m_bclXML.resetXMLChecksum();

  openstudio::path measureXMLPath = m_directory / toPath("measure.xml");
  m_bclXML.saveAs(measureXMLPath);
}

BCLMeasure::BCLMeasure(const openstudio::path& dir) : m_directory(openstudio::filesystem::system_complete(dir)), m_bclXML(BCLXMLType::MeasureXML) {
  // Avoid potential problems, since toPath("/path/to/dir/") != toPath("/path/to/dir")
  m_directory.remove_trailing_separator();

  openstudio::path xmlPath = m_directory / toPath("measure.xml");
  boost::optional<BCLXML> bclXML = BCLXML::load(xmlPath);

  if (!bclXML) {
    LOG_AND_THROW("'" << toString(dir) << "' is not a measure");
  }

  m_bclXML = *bclXML;

  // check for required attributes
  std::vector<Attribute> measureTypes = m_bclXML.getAttributes("Measure Type");
  if (measureTypes.empty()) {
    LOG_AND_THROW("'" << toString(dir) << "' is missing the required attribute \"Measure Type\"");
  } else if (measureTypes.size() > 1) {
    LOG_AND_THROW("'" << toString(dir) << "' has multiple copies of required attribute \"Measure Type\"");
  } else if (measureTypes[0].valueType() != AttributeValueType::String) {
    LOG_AND_THROW("'" << toString(dir) << "' has wrong type for required attribute \"Measure Type\"");
  }

  std::vector<Attribute> measureLanguages = m_bclXML.getAttributes("Measure Language");
  boost::optional<MeasureLanguage> measureLanguage_;
  if (measureLanguages.empty()) {
    // No-op
  } else if (measureLanguages.size() > 1) {
    LOG_AND_THROW("'" << toString(dir) << "' has multiple copies of required attribute \"Measure Language\"");
  } else if (measureLanguages[0].valueType() != AttributeValueType::String) {
    LOG_AND_THROW("'" << toString(dir) << "' has wrong type for required attribute \"Measure Language\"");
  } else {
    measureLanguage_ = MeasureLanguage(measureLanguages[0].valueAsString());
  }

  // Don't allow having both a ruby measure.rb and a python measure.py listed in the same XML
  const bool hasRuby = m_bclXML.hasFile(m_directory / "measure.rb");
  const bool hasPython = m_bclXML.hasFile(m_directory / "measure.py");
  if (hasRuby && hasPython) {
    LOG_AND_THROW("'" << toString(dir) << "' has both measure.rb and measure.py, and they cannot be used at the same time.");
  } else if (hasRuby) {
    if (measureLanguage_ && measureLanguage_.get() != MeasureLanguage::Ruby) {
      LOG_AND_THROW("'" << toString(dir) << "' has a measure.rb; but \"Measure Language\" is not 'Ruby', it's '" << measureLanguage_->valueName()
                        << "'.");
    }
  } else if (hasPython) {
    if (measureLanguage_ && measureLanguage_.get() != MeasureLanguage::Python) {
      LOG_AND_THROW("'" << toString(dir) << "' has a measure.py; but \"Measure Language\" is not 'Python', it's '" << measureLanguage_->valueName()
                        << "'.");
    }
  } else {
    LOG_AND_THROW("'" << toString(dir) << "' has neither measure.rb nor measure.py");
  }

  if (m_bclXML.xmlChecksum().empty()) {
    // this will set the checksum but not increment the version id
    m_bclXML.checkForUpdatesXML();
  }

  // check that opening the file did not modify it
  if (m_bclXML.versionId() != bclXML->versionId()) {
    // DLM: should this be an assert instead?
    LOG_AND_THROW("Measure version_id is no longer valid");
  }
}

std::string BCLMeasure::makeClassName(const std::string& name) {
  std::string str(name);

  bool startsWithLetter = false;
  for (std::size_t i = 0; i < str.size(); ++i) {
    if (!((std::isalpha(str[i]) != 0) || (std::isdigit(str[i]) != 0))) {
      str[i] = ' ';
    } else if (i == 0) {
      startsWithLetter = (std::isalpha(str[i]) != 0);
    }
  }

  std::string result;
  std::vector<std::string> parts;
  boost::split(parts, str, boost::is_any_of(" "));
  for (auto& part : parts) {
    boost::replace_all(part, " ", "");
    boost::replace_all(part, "\n", "");
    boost::replace_all(part, "\r", "");
    boost::replace_all(part, "\t", "");
    if (!part.empty()) {
      part[0] = toupper(part[0]);
      result.append(part);
    }
  }

  if (result.empty()) {
    result = std::string("A") + openstudio::toString(createUUID());
    boost::replace_all(result, "{", "");
    boost::replace_all(result, "}", "");
    boost::replace_all(result, "-", "");
  } else if (!startsWithLetter) {
    result = std::string("A") + result;
  }

  return result;
}

boost::optional<BCLMeasure> BCLMeasure::load(const openstudio::path& dir) {
  boost::optional<BCLMeasure> result;
  try {
    result = BCLMeasure(dir);
  } catch (const std::exception&) {
  }
  return result;
}

std::vector<std::string> BCLMeasure::suggestedIntendedSoftwareTools() {
  std::vector<std::string> result{"Apply Measure Now", "OpenStudio Application", "Parametric Analysis Tool", "Analysis Spreadsheet"};
  return result;
}

std::vector<std::string> BCLMeasure::suggestedIntendedUseCases() {
  std::vector<std::string> result{"Model Articulation",  "Calibration", "Sensitivity Analysis",
                                  "New Construction EE", "Retrofit EE", "Automatic Report Generation"};

  return result;
}

std::vector<std::string> BCLMeasure::suggestedFirstLevelTaxonomyTerms() {
  std::vector<std::string> result{
    "Envelope",       "Electric Lighting", "Equipment", "People", "HVAC", "Refrigeration", "Service Water Heating", "Onsite Power Generation",
    "Whole Building", "Economics",         "Reporting"};

  return result;
}

std::vector<std::string> BCLMeasure::suggestedSecondLevelTaxonomyTerms(const std::string& firstLevelTaxonomyTerm) {
  std::vector<std::string> result;
  result.reserve(8);

  if (firstLevelTaxonomyTerm == "Envelope") {
    result.emplace_back("Form");
    result.emplace_back("Opaque");
    result.emplace_back("Fenestration");
    result.emplace_back("Construction Sets");
    result.emplace_back("Daylighting");
    result.emplace_back("Infiltration");
  } else if (firstLevelTaxonomyTerm == "Electric Lighting") {
    result.emplace_back("Electric Lighting Controls");
    result.emplace_back("Lighting Equipment");
  } else if (firstLevelTaxonomyTerm == "Equipment") {
    result.emplace_back("Equipment Controls");
    result.emplace_back("Electric Equipment");
    result.emplace_back("Gas Equipment");
  } else if (firstLevelTaxonomyTerm == "People") {
    result.emplace_back("Characteristics");
    result.emplace_back("People Schedules");
  } else if (firstLevelTaxonomyTerm == "HVAC") {
    result.emplace_back("HVAC Controls");
    result.emplace_back("Heating");
    result.emplace_back("Cooling");
    result.emplace_back("Heat Rejection");
    result.emplace_back("Energy Recovery");
    result.emplace_back("Distribution");
    result.emplace_back("Ventilation");
    result.emplace_back("Whole System");
  } else if (firstLevelTaxonomyTerm == "Refrigeration") {
    result.emplace_back("Refrigeration Controls");
    result.emplace_back("Cases and Walkins");
    result.emplace_back("Compressors");
    result.emplace_back("Condensers");
    result.emplace_back("Heat Reclaim");
  } else if (firstLevelTaxonomyTerm == "Service Water Heating") {
    result.emplace_back("Water Use");
    result.emplace_back("Water Heating");
    result.emplace_back("Distribution");
  } else if (firstLevelTaxonomyTerm == "Onsite Power Generation") {
    result.emplace_back("Photovoltaic");
  } else if (firstLevelTaxonomyTerm == "Whole Building") {
    result.emplace_back("Whole Building Schedules");
    result.emplace_back("Space Types");
  } else if (firstLevelTaxonomyTerm == "Economics") {
    result.emplace_back("Life Cycle Cost Analysis");
  } else if (firstLevelTaxonomyTerm == "Reporting") {
    result.emplace_back("QAQC");
    result.emplace_back("Troubleshooting");
  }

  return result;
}

std::vector<BCLMeasure> BCLMeasure::getMeasuresInDir(const openstudio::path& dir) {
  LOG(Debug, "Loading measures in path: " << openstudio::toString(dir));
  std::vector<BCLMeasure> result;

  if (!exists(dir) || !is_directory(dir)) {
    LOG(Debug, "Error Loading measures in path: " << openstudio::toString(dir));
    return result;
  }

  if (isNetworkPath(dir) && !isNetworkPathAvailable(dir)) {
    LOG(Debug, "Error Loading measures in unavailable network location: " << openstudio::toString(dir));
    return result;
  }

  try {
    openstudio::filesystem::directory_iterator endit;  // default construction yields past-the-end
    openstudio::filesystem::directory_iterator it(dir);
    for (; it != endit; ++it) {
      if (is_directory(it->status())) {
        LOG(Debug, "Attempting to load measure: " << openstudio::toString(it->path()));
        boost::optional<BCLMeasure> measure = load(it->path());
        if (measure) {
          LOG(Debug, "Success loading measure: " << openstudio::toString(it->path()));
          result.push_back(*measure);
        }
      }
    }
  } catch (const std::exception&) {
  }

  return result;
}

openstudio::path BCLMeasure::directory() const {
  return m_directory;
}

boost::optional<std::string> BCLMeasure::error() const {
  return m_bclXML.error();
}

std::string BCLMeasure::uid() const {
  return m_bclXML.uid();
}

UUID BCLMeasure::uuid() const {
  return toUUID("{" + uid() + "}");
}

std::string BCLMeasure::versionId() const {
  return m_bclXML.versionId();
}

UUID BCLMeasure::versionUUID() const {
  return toUUID("{" + versionId() + "}");
}

boost::optional<DateTime> BCLMeasure::versionModified() const {
  return m_bclXML.versionModified();
}

std::string BCLMeasure::xmlChecksum() const {
  return m_bclXML.xmlChecksum();
}

std::string BCLMeasure::name() const {
  return m_bclXML.name();
}

std::string BCLMeasure::displayName() const {
  return m_bclXML.displayName();
}

std::string BCLMeasure::className() const {
  return m_bclXML.className();
}

std::string BCLMeasure::description() const {
  return m_bclXML.description();
}

std::string BCLMeasure::modelerDescription() const {
  return m_bclXML.modelerDescription();
}

std::vector<BCLMeasureArgument> BCLMeasure::arguments() const {
  return m_bclXML.arguments();
}

std::vector<BCLMeasureOutput> BCLMeasure::outputs() const {
  return m_bclXML.outputs();
}

std::vector<std::string> BCLMeasure::tags() const {
  return m_bclXML.tags();
}

std::vector<Attribute> BCLMeasure::attributes() const {
  return m_bclXML.attributes();
}

std::vector<BCLFileReference> BCLMeasure::files() const {
  return m_bclXML.files();
}

void BCLMeasure::setError(const std::string& error) {
  m_bclXML.setError(error);
}

void BCLMeasure::resetError() {
  m_bclXML.resetError();
}

void BCLMeasure::setName(const std::string& name) {
  m_bclXML.setName(name);
}

void BCLMeasure::setDisplayName(const std::string& displayName) {
  m_bclXML.setDisplayName(displayName);
}

void BCLMeasure::setClassName(const std::string& className) {
  m_bclXML.setClassName(className);
}

void BCLMeasure::setDescription(const std::string& description) {
  m_bclXML.setDescription(description);
}

void BCLMeasure::setModelerDescription(const std::string& modelerDescription) {
  m_bclXML.setModelerDescription(modelerDescription);
}

void BCLMeasure::setArguments(const std::vector<BCLMeasureArgument>& arguments) {
  m_bclXML.setArguments(arguments);
}

void BCLMeasure::setOutputs(const std::vector<BCLMeasureOutput>& outputs) {
  m_bclXML.setOutputs(outputs);
}

std::string BCLMeasure::taxonomyTag() const {
  std::string result;
  std::vector<std::string> tags = m_bclXML.tags();
  if (!tags.empty()) {
    result = tags[0];
  }
  return result;
}

void BCLMeasure::setTaxonomyTag(const std::string& taxonomyTag) {
  m_bclXML.clearTags();
  m_bclXML.addTag(taxonomyTag);
}

MeasureType BCLMeasure::measureType() const {
  std::vector<Attribute> measureTypes = m_bclXML.getAttributes("Measure Type");
  OS_ASSERT(measureTypes.size() == 1);
  return MeasureType(measureTypes[0].valueAsString());
}

void BCLMeasure::setMeasureType(const MeasureType& measureType) {
  const std::string attributeName("Measure Type");
  Attribute attribute(attributeName, measureType.valueName());
  m_bclXML.removeAttributes(attributeName);
  m_bclXML.addAttribute(attribute);
}

MeasureLanguage BCLMeasure::measureLanguage() const {
  std::vector<Attribute> measureLanguages = m_bclXML.getAttributes("Measure Language");
  if (measureLanguages.empty()) {
    // This was added at 3.5.0, when Python measures came in, so fall back to Ruby if not there
    return MeasureLanguage::Ruby;
  }
  OS_ASSERT(measureLanguages.size() == 1);
  return MeasureLanguage(measureLanguages[0].valueAsString());
}

void BCLMeasure::setMeasureLanguage(const MeasureLanguage& measureLanguage) {
  const std::string attributeName("Measure Language");
  Attribute attribute(attributeName, measureLanguage.valueName());
  m_bclXML.removeAttributes(attributeName);
  m_bclXML.addAttribute(attribute);
}

std::vector<std::string> BCLMeasure::intendedSoftwareTools() const {
  std::vector<std::string> result;
  std::vector<Attribute> attributes = m_bclXML.getAttributes("Intended Software Tool");
  result.reserve(attributes.size());
  std::transform(attributes.cbegin(), attributes.cend(), std::back_inserter(result),
                 [](const Attribute& attribute) { return attribute.valueAsString(); });
  return result;
}

/// Returns values of any "Intended Use Case" attributes
std::vector<std::string> BCLMeasure::intendedUseCases() const {
  std::vector<std::string> result;
  std::vector<Attribute> attributes = m_bclXML.getAttributes("Intended Use Case");
  result.reserve(attributes.size());
  std::transform(attributes.cbegin(), attributes.cend(), std::back_inserter(result),
                 [](const Attribute& attribute) { return attribute.valueAsString(); });
  return result;
}

boost::optional<openstudio::path> BCLMeasure::primaryRubyScriptPath() const {
  openstudio::path result = m_directory / toPath("measure.rb");
  if (exists(result) && is_regular_file(result)) {
    return result;
  }
  return boost::none;
}

boost::optional<openstudio::path> BCLMeasure::primaryPythonScriptPath() const {
  openstudio::path result = m_directory / toPath("measure.py");
  if (exists(result) && is_regular_file(result)) {
    return result;
  }
  return boost::none;
}

boost::optional<openstudio::path> BCLMeasure::primaryScriptPath() const {
  return (measureLanguage() == MeasureLanguage::Ruby) ? primaryRubyScriptPath() : primaryPythonScriptPath();
}

FileReferenceType BCLMeasure::inputFileType() const {
  FileReferenceType result = FileReferenceType::Unknown;
  MeasureType measureType = this->measureType();
  if (measureType == MeasureType::ModelMeasure) {
    result = FileReferenceType::OSM;
  } else if (measureType == MeasureType::EnergyPlusMeasure) {
    result = FileReferenceType::IDF;
  } else if (measureType == MeasureType::UtilityMeasure) {
    // no-op
  } else if (measureType == MeasureType::ReportingMeasure) {
    // DLM: is this right?
    // no-op
  }
  return result;
}

FileReferenceType BCLMeasure::outputFileType() const {
  FileReferenceType result = FileReferenceType::Unknown;
  MeasureType measureType = this->measureType();
  if (measureType == MeasureType::ModelMeasure) {
    result = FileReferenceType::OSM;
  } else if (measureType == MeasureType::EnergyPlusMeasure) {
    result = FileReferenceType::IDF;
  } else if (measureType == MeasureType::UtilityMeasure) {
    // no-op
  } else if (measureType == MeasureType::ReportingMeasure) {
    // DLM: is this right?
    // no-op
  }
  return result;
}

void BCLMeasure::changeUID() {
  m_bclXML.changeUID();
}

void BCLMeasure::incrementVersionId() {
  m_bclXML.incrementVersionId();
}

bool BCLMeasure::updateMeasureScript(const MeasureType& oldMeasureType, const MeasureType& newMeasureType, const MeasureLanguage& oldMeasureLanguage,
                                     const MeasureLanguage& newMeasureLanguage, const std::string& oldClassName, const std::string& newClassName,
                                     const std::string& name, const std::string& description, const std::string& modelerDescription) {
  if (oldMeasureLanguage != newMeasureLanguage) {
    LOG_AND_THROW("Cannot update a measure after a Language Change from " << oldMeasureLanguage.valueName() << " to "
                                                                          << newMeasureLanguage.valueName());
  }
  // TODO: deal with Python
  boost::optional<openstudio::path> rubyScriptPath_ = primaryRubyScriptPath();
  if (rubyScriptPath_ && exists(*rubyScriptPath_)) {

    openstudio::filesystem::ifstream file(*rubyScriptPath_, std::ios_base::binary);
    if (file.is_open()) {

      // Replacement paterns
      std::string nameFunction = "$1def name\n$1  return \"" + name + "\"\n$1end";
      std::string descriptionFunction = "$1def description\n$1  return \"" + description + "\"\n$1end";
      std::string modelerDescriptionFunction = "$1def modeler_description\n$1  return \"" + modelerDescription + "\"\n$1end";

      std::string fileString = openstudio::filesystem::read_as_string(file);

      boost::regex re;

      if (oldMeasureType != newMeasureType) {
        boost::replace_all(fileString, oldMeasureType.valueName(), newMeasureType.valueName());
      }

      if (!oldClassName.empty() && !newClassName.empty() && oldClassName != newClassName) {
        // DLM: might also want to check that oldClassName is greater than 3 characters long?
        // should we be doing a more selective replace (like require leading space and trailing space, ., or :)?
        boost::replace_all(fileString, oldClassName, newClassName);
      }

      // Note JM 2018-11-06: Use the iterator overload of regex_replace for inplace replacement? Actually don't.
      // There's undefined behavior when you end up replacing with something that doesn't have the same length
      re.set_expression("^([\t ]*?)def\\s+name(.*?)end[\\s#]?$");
      fileString = boost::regex_replace(fileString, re, nameFunction);

      re.set_expression("^([\t ]*?)def\\s+description(.*?)end[\\s#]?$");
      fileString = boost::regex_replace(fileString, re, descriptionFunction);

      re.set_expression("^([\t ]*?)def\\s+modeler_description(.*?)end[\\s#]?$");
      fileString = boost::regex_replace(fileString, re, modelerDescriptionFunction);

      file.close();

      openstudio::filesystem::ofstream ofile(*rubyScriptPath_, std::ios_base::binary);
      if (ofile.is_open()) {
        ofile << fileString;
        // openstudio::filesystem::write(ofile, fileString);
        ofile.close();
        return true;
      }
    }
  }

  return false;
}

bool BCLMeasure::updateMeasureTests(const std::string& oldClassName, const std::string& newClassName) {
  bool result = true;
  for (const BCLFileReference& fileRef : files()) {
    std::string usageType = fileRef.usageType();
    if (usageType != "test") {
      continue;
    }

    if (exists(fileRef.path())) {

      std::string oldLowerClassName = toUnderscoreCase(oldClassName);
      std::string newLowerClassName = toUnderscoreCase(newClassName);

      openstudio::path oldPath = fileRef.path();
      openstudio::path newPath = oldPath;

      const auto oldFileName = fileRef.path().filename();
      auto newFileName = toString(oldFileName);
      if (!oldLowerClassName.empty() && !newLowerClassName.empty() && oldLowerClassName != newLowerClassName) {
        auto temp = oldLowerClassName;
        const auto index = newFileName.rfind(temp);
        if (index != std::string::npos) {
          newFileName.replace(index, temp.size(), newLowerClassName);
          newPath = oldPath.parent_path() / toPath(newFileName);

          if (openstudio::filesystem::exists(newPath)) {
            // somehow this file already exists (e.g. no name change), don't clobber it
            newPath = oldPath;
          } else {
            openstudio::filesystem::copy_file(oldPath, newPath);
            openstudio::filesystem::remove(oldPath);
          }
        }
      }

      openstudio::filesystem::ifstream file(newPath, std::ios_base::binary);
      if (file.is_open()) {

        auto fileString = openstudio::filesystem::read_as_string(file);
        if (!oldClassName.empty() && !newClassName.empty() && oldClassName != newClassName) {
          // DLM: might also want to check that oldClassName is greater than 3 characters long?
          // should we be doing a more selective replace (like require leading space and trailing space, ., or :)?
          boost::replace_all(fileString, oldClassName, newClassName);
        }
        file.close();

        openstudio::filesystem::ofstream file2(newPath, std::ios_base::binary);
        if (file2.is_open()) {
          file2 << fileString;
          file2.close();

        } else {
          result = false;
        }
      } else {
        result = false;
      }
    } else {
      result = false;
    }
  }
  return result;
}

void BCLMeasure::clearFiles() {
  m_bclXML.clearFiles();
}

void BCLMeasure::addAttribute(const Attribute& attribute) {
  m_bclXML.addAttribute(attribute);
}

std::vector<Attribute> BCLMeasure::getAttributes(const std::string& name) const {
  return m_bclXML.getAttributes(name);
}

bool BCLMeasure::removeAttributes(const std::string& name) {
  return m_bclXML.removeAttributes(name);
}

void BCLMeasure::clearAttributes() {
  m_bclXML.clearAttributes();
}

bool BCLMeasure::missingRequiredFields() const {
  bool missing = (uid().empty() || versionId().empty() || xmlChecksum().empty() || name().empty() || displayName().empty() || className().empty()
                  || description().empty() || modelerDescription().empty());
  return missing;
}

bool BCLMeasure::checkForUpdatesFiles() {
  bool result = false;

  std::vector<BCLFileReference> filesToRemove;
  std::vector<BCLFileReference> filesToAdd;

  // For all files we have on reference in the measure.xml
  for (BCLFileReference& file : m_bclXML.files()) {

    openstudio::path filePath = file.path();
    // If the file has been deleted from disk, mark it for removal
    if (!openstudio::filesystem::exists(filePath)) {
      LOG(Info, filePath << " has been deleted");
      result = true;
      // what if this is the measure.rb file?
      filesToRemove.push_back(file);

      // Otherwise, if it's to be ignored
    } else if (!isApprovedFile(filePath, m_directory)) {
      // The only way this file could have been added to the XML is if the user had manually added it,
      // or we have changed the rules in the source code. In either case, I think a log message is helpful...
      // We filter for ALL allowed cases here, so that we catch all mistakes, eg any file that the user manually added that isn't one of the
      // approved root files or under an approved subdirectory
      LOG(Warn, filePath << " was present in your measure.xml but it is not an approved file and will be removed from tracking.");
      result = true;
      filesToRemove.push_back(file);

      // otherwise, compute new checksum, and if not the same: mark it for addition
    } else if (file.checkForUpdate()) {
      LOG(Info, filePath << " has been updated");
      result = true;
      filesToAdd.push_back(file);
    }
  }

  auto addWithUsageTypeIfNotExisting = [this, &filesToAdd](const openstudio::path& relativeFilePath, const std::string& usageType) -> bool {
    if (!m_bclXML.hasFile(m_directory / relativeFilePath)) {
      BCLFileReference fileref(m_directory, relativeFilePath, true);
      fileref.setUsageType(usageType);
      filesToAdd.push_back(fileref);
      return true;
    } else {
      return false;
    }
  };

  // I could loop on all recursive_directory_files for the entire measureDir,
  // but this is likely faster by prefiltering on what has potential to be added
  for (const auto& [subFolderName, usageType] : approvedSubFolderToUsageMap) {
    openstudio::path approvedSubFolderAbsolutePath = m_directory / toPath(subFolderName);
    if (openstudio::filesystem::exists(approvedSubFolderAbsolutePath)) {
      for (const auto& relativeFilePath : openstudio::filesystem::recursive_directory_files(approvedSubFolderAbsolutePath)) {
        openstudio::path absoluteFilePath = approvedSubFolderAbsolutePath / relativeFilePath;
        if (!isApprovedFile(absoluteFilePath, m_directory)) {
          continue;
        }
        result |= addWithUsageTypeIfNotExisting(toPath(subFolderName) / relativeFilePath, std::string(usageType));
      }
    }
  }

  for (const auto& [fileName, usageType] : rootToUsageTypeMap) {
    openstudio::path relativeFilePath = toPath(fileName);
    openstudio::path absoluteFilePath = m_directory / relativeFilePath;
    if (openstudio::filesystem::exists(absoluteFilePath)) {
      bool thisResult = addWithUsageTypeIfNotExisting(relativeFilePath, std::string(usageType));
      if (thisResult && ((fileName == "measure.rb") || (fileName == "measure.py"))) {
        // we don't know what the actual version this was created for, we also don't know minimum version
        filesToAdd.back().setSoftwareProgramVersion(openStudioVersion());
      }
      result |= thisResult;
    }
  }

  for (const BCLFileReference& file : filesToRemove) {
    m_bclXML.removeFile(file.path());
  }

  for (const BCLFileReference& file : filesToAdd) {
    m_bclXML.addFile(file);
  }

  // Don't allow having both a ruby and a python measure in the same folder
  bool hasRuby = m_bclXML.hasFile(m_directory / "measure.rb");
  bool hasPython = m_bclXML.hasFile(m_directory / "measure.py");
  if (hasRuby && hasPython) {
    LOG(Warn, "Both measure.rb and measure.py cannot be in the same measure folder. Keeping only the ruby one");
    setMeasureLanguage(MeasureLanguage::Ruby);
    m_bclXML.removeFile(m_directory / "measure.py");
  } else if (hasRuby) {
    if (measureLanguage() != MeasureLanguage::Ruby) {
      setMeasureLanguage(MeasureLanguage::Ruby);
    }
  } else if (hasPython) {
    if (measureLanguage() != MeasureLanguage::Python) {
      setMeasureLanguage(MeasureLanguage::Python);
    }
  } else {
    LOG_AND_THROW("measure at " << m_directory << " has neither measure.rb nor measure.py");
  }

  // increment version if anything changed
  if (result) {
    m_bclXML.incrementVersionId();
  }

  return result;
}

bool BCLMeasure::checkForUpdatesXML() {
  return m_bclXML.checkForUpdatesXML();
}

bool BCLMeasure::operator==(const BCLMeasure& other) const {
  return ((this->uid() == other.uid()) && (this->versionId() == other.versionId()));
}

bool BCLMeasure::operator!=(const BCLMeasure& other) const {
  return !(operator==(other));
}

bool BCLMeasure::operator<(const BCLMeasure& other) const {
  return (uid() + versionId()) < (other.uid() + other.versionId());
}

bool BCLMeasure::save() const {
  return m_bclXML.save();
}

boost::optional<BCLMeasure> BCLMeasure::clone(const openstudio::path& newDir) const {

  if (openstudio::filesystem::exists(newDir)) {
    if (!isEmptyDirectory(newDir)) {
      LOG(Warn, "Cannot clone measure to newDir=" << newDir << " since it already exists and is not empty.");
      return boost::none;
    }
  } else {
    if (!openstudio::filesystem::create_directories(newDir)) {
      LOG(Warn, "Cannot clone measure to newDir=" << newDir << " since we cannot create the directory. Check file permissions.");
      return boost::none;
    }
  }

  // TODO: isn't it better to copy only what's inside measure.xml?
  constexpr bool weCareAboutOutdatedMeasureXML = true;
  // The only caveat is what if the measure.xml is outdated with regards to <files>?
  // Currently the BCLMeasure(const path&) ctor **does NOT** call checkUpdatesFiles()
  //
  // We could call it here by that would require a const_cast and it would increment the measure.xml versionId of the original measure,
  // and update the list of files, which isn't a big deal if the user doesn't then save it too? Even if the user did it, wouldn't we be helping out anyways?

  // If this is really not wanted, we can go back to scanning the files on disk, with a similar logic as in checkForUpdatesFiles:
  // loop on all files on disk, and if isApproved is true, then copy them

  // Copy the measure.xml
  openstudio::filesystem::copy_file_no_throw(m_directory / openstudio::path{"measure.xml"}, newDir / openstudio::path{"measure.xml"});

  if constexpr (!weCareAboutOutdatedMeasureXML) {
    // Then copy whatever is referneced in the measure.xml
    for (const BCLFileReference& file : this->files()) {

      if (std::find_if(usageTypesIgnoredOnClone.cbegin(), usageTypesIgnoredOnClone.cend(),
                       [&file](const auto& sv) { return file.usageType() == std::string(sv); })
          == usageTypesIgnoredOnClone.cend()) {

        // BCLFileReference::path() is absolute
        openstudio::path oriPath = file.path();
        openstudio::path relPath = openstudio::filesystem::relative(oriPath, m_directory);
        openstudio::path destination = newDir / relPath;
        // Create parent directories in destination if need be
        openstudio::filesystem::create_directories(destination.parent_path());
        openstudio::filesystem::copy_file_no_throw(oriPath, destination);
      }
    }
  } else {

    std::vector<std::pair<openstudio::path, openstudio::path>> filesToCopy;

    // I could loop on all recursive_directory_files for the entire measureDir,
    // but this is likely faster by prefiltering on what has potential to be added
    for (const auto& subFolderToUsagePair : approvedSubFolderToUsageMap) {
      // Structure bindings are never names of variables so they can't be captured in lambda, hence call to first and second on the pair
      std::string_view subFolderName = subFolderToUsagePair.first;
      std::string_view usageType = subFolderToUsagePair.second;
      if (std::find_if(usageTypesIgnoredOnClone.cbegin(), usageTypesIgnoredOnClone.cend(),
                       [usageType](const auto& sv) { return usageType == std::string(sv); })
          != usageTypesIgnoredOnClone.cend()) {
        continue;
      }
      openstudio::path subFolderPath = toPath(subFolderName);
      openstudio::path approvedSubFolderAbsolutePath = m_directory / subFolderPath;
      if (openstudio::filesystem::exists(approvedSubFolderAbsolutePath)) {
        for (const auto& relativeFilePath : openstudio::filesystem::recursive_directory_files(approvedSubFolderAbsolutePath)) {
          openstudio::path absoluteFilePath = approvedSubFolderAbsolutePath / relativeFilePath;
          if (isApprovedFile(absoluteFilePath, m_directory)) {
            filesToCopy.emplace_back(absoluteFilePath, newDir / subFolderPath / relativeFilePath);
          }
        }
      }
    }

    for (std::string_view fileName : approvedRootFiles) {
      openstudio::path relativeFilePath = toPath(fileName);
      openstudio::path absoluteFilePath = m_directory / relativeFilePath;
      if (openstudio::filesystem::exists(absoluteFilePath)) {
        filesToCopy.emplace_back(absoluteFilePath, newDir / relativeFilePath);
      }
    }

    for (const auto& [oriPath, destPath] : filesToCopy) {
      openstudio::filesystem::create_directories(destPath.parent_path());
      openstudio::filesystem::copy_file_no_throw(oriPath, destPath);
    }
  }

  return BCLMeasure::load(newDir);
}

std::string BCLMeasure::xmlString() const {
  return m_bclXML.toString();
}

Json::Value BCLMeasure::toJSON() const {
  return m_bclXML.toJSON();
}

std::string BCLMeasure::toJSONString() const {
  return m_bclXML.toJSONString();
}

}  // namespace openstudio
