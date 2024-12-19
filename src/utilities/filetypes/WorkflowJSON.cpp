/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkflowJSON.hpp"
#include "WorkflowJSON_Impl.hpp"

#include "WorkflowStep_Impl.hpp"
#include "RunOptions_Impl.hpp"

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Checksum.hpp"
#include "../time/DateTime.hpp"

namespace openstudio {
namespace detail {

  const int NO_MEASURE_TYPE = -1;

  path canonicalOrAbsolute(const path& p, const path& base = boost::filesystem::current_path()) {
    path result;
    if (p.is_absolute()) {
      result = p;
    } else {
      result = boost::filesystem::absolute(p, base);
    }

    if (exists(result)) {
      result = boost::filesystem::canonical(result);
    }

    return result;
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl() {
    m_value["created_at"] = DateTime::nowUTC().toISO8601();
    m_value["steps"] = Json::Value(Json::arrayValue);
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl(const std::string& s) {

    Json::CharReaderBuilder rbuilder;
    std::istringstream ss(s);
    std::string formattedErrors;
    bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &m_value, &formattedErrors);

    openstudio::path p;

    if (!parsingSuccessful) {

      // see if this is a path
      p = toPath(s);
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)) {
        // open file
        std::ifstream ifs(openstudio::toSystemFilename(p));
        m_value.clear();
        formattedErrors.clear();
        parsingSuccessful = Json::parseFromStream(rbuilder, ifs, &m_value, &formattedErrors);
      }

      if (!parsingSuccessful) {
        LOG_AND_THROW("WorkflowJSON cannot be processed, " << formattedErrors);
      }
    }

    parseSteps();
    parseRunOptions();

    if (!p.empty()) {
      setOswPath(p, false);
    }
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl(const openstudio::path& p) {
    if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p)) {
      LOG_AND_THROW("Path '" << p << "' is not a WorkflowJSON file");
    }

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(p));

    Json::CharReaderBuilder rbuilder;
    std::string formattedErrors;
    bool parsingSuccessful = Json::parseFromStream(rbuilder, ifs, &m_value, &formattedErrors);
    if (!parsingSuccessful) {
      LOG_AND_THROW("WorkflowJSON '" << toString(p) << "' cannot be processed, " << formattedErrors);
    }

    parseSteps();
    parseRunOptions();

    setOswPath(p, false);
  }

  WorkflowJSON WorkflowJSON_Impl::clone() const {
    WorkflowJSON result(this->string(true));

    boost::optional<openstudio::path> oswPath = this->oswPath();
    if (oswPath) {
      result.setOswPath(*oswPath);
    } else {
      result.setOswDir(this->oswDir());
    }

    return result;
  }

  Json::Value WorkflowJSON_Impl::toJSON(bool includeHash) const {

    Json::Value clone(m_value);
    if (!includeHash) {
      clone.removeMember("hash");
    }

    Json::Value steps(Json::arrayValue);
    for (const auto& step : m_steps) {
      steps.append(step.toJSON());
    }
    clone["steps"] = std::move(steps);

    if (m_runOptions) {
      clone["run_options"] = m_runOptions->toJSON();
    }

    return clone;
  }

  std::string WorkflowJSON_Impl::string(bool includeHash) const {
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, toJSON(includeHash));

    return result;
  }

  std::string WorkflowJSON_Impl::hash() const {
    Json::Value defaultValue("");
    Json::Value h = m_value.get("hash", defaultValue);

    return h.asString();
  }

  std::string WorkflowJSON_Impl::computeHash() const {
    return checksum(string(false));
  }

  bool WorkflowJSON_Impl::checkForUpdates() {
    std::string h1 = hash();
    std::string h2 = computeHash();
    bool result = (h1 != h2);
    if (result) {
      onUpdate();
      h2 = computeHash();  // recompute hash after updating timestamps in onUpdate
      m_value["hash"] = h2;
    }
    return result;
  }

  bool WorkflowJSON_Impl::save() const {
    boost::optional<openstudio::path> p = oswPath();
    if (!p) {
      return false;
    }

    if (makeParentFolder(*p)) {
      std::ofstream outFile(openstudio::toSystemFilename(*p));

      if (outFile) {
        try {
          outFile << string();
          outFile.close();
          return true;
        } catch (...) {
          LOG(Error, "Unable to write file to path '" << toString(*p) << "'.");
          return false;
        }
      }
    }

    LOG(Error, "Unable to write file to path '" << toString(*p) << "', because parent directory " << "could not be created.");

    return false;
  }

  bool WorkflowJSON_Impl::saveAs(const openstudio::path& p) {
    // cppcheck-suppress knownConditionTrueFalse
    if (setOswPath(p, true)) {
      checkForUpdates();
      return save();
    }
    return false;
  }

  void WorkflowJSON_Impl::reset() {
    m_value.removeMember("started_at");
    m_value.removeMember("completed_at");
    m_value.removeMember("completed_status");
    m_value.removeMember("current_step");
    onUpdate();
  }

  void WorkflowJSON_Impl::start() {
    m_value["started_at"] = DateTime::nowUTC().toISO8601();
    m_value["current_step"] = 0;
    for (auto& step : workflowSteps()) {
      step.resetResult();
    }
    onUpdate();
  }

  unsigned WorkflowJSON_Impl::currentStepIndex() const {
    Json::Value defaultValue(0);
    Json::Value index = m_value.get("current_step", defaultValue);
    return index.asUInt();
  }

  boost::optional<WorkflowStep> WorkflowJSON_Impl::currentStep() const {
    unsigned index = currentStepIndex();
    std::vector<WorkflowStep> steps = this->workflowSteps();

    if (index >= steps.size()) {
      return boost::none;
    }

    return steps[index];
  }

  bool WorkflowJSON_Impl::incrementStep() {
    unsigned index = currentStepIndex();
    m_value["current_step"] = index + 1;
    onUpdate();
    return currentStep().is_initialized();
  }

  boost::optional<std::string> WorkflowJSON_Impl::completedStatus() const {
    Json::Value defaultValue("");
    Json::Value status = m_value.get("completed_status", defaultValue);
    std::string result = status.asString();
    if (result.empty()) {
      return boost::none;
    }
    return result;
  }

  void WorkflowJSON_Impl::setCompletedStatus(const std::string& status) {
    m_value["completed_status"] = status;
    m_value["completed_at"] = DateTime::nowUTC().toISO8601();
    onUpdate();
  }

  boost::optional<DateTime> WorkflowJSON_Impl::createdAt() const {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("created_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()) {
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::startedAt() const {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("started_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()) {
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::updatedAt() const {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("updated_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()) {
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::completedAt() const {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("completed_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()) {
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<std::string> WorkflowJSON_Impl::eplusoutErr() const {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("eplusout_err", defaultValue);
    std::string str = value.asString();
    if (str.empty()) {
      return boost::none;
    }
    return str;
  }

  void WorkflowJSON_Impl::setEplusoutErr(const std::string& eplusoutErr) {
    m_value["eplusout_err"] = eplusoutErr;
    onUpdate();
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::oswPath() const {
    if (m_oswFilename.empty() || m_oswDir.empty()) {
      return boost::none;
    }
    return m_oswDir / m_oswFilename;
  }

  bool WorkflowJSON_Impl::setOswPath(const openstudio::path& path, bool emitChange) {
    openstudio::path p = canonicalOrAbsolute(path);

    m_oswFilename = p.filename();
    m_oswDir = p.parent_path();
    setMeasureTypes();
    if (emitChange) {
      onUpdate();  // onUpdate does not happen in setMeasureTypes
    }
    return true;
  }

  openstudio::path WorkflowJSON_Impl::oswDir() const {
    if (m_oswDir.empty()) {
      return boost::filesystem::current_path();
    }
    return m_oswDir;
  }

  bool WorkflowJSON_Impl::setOswDir(const openstudio::path& path) {
    m_oswDir = canonicalOrAbsolute(path);
    setMeasureTypes();
    onUpdate();  // onUpdate does not happen in setMeasureTypes
    return true;
  }

  openstudio::path WorkflowJSON_Impl::rootDir() const {
    Json::Value defaultValue(".");
    Json::Value root = m_value.get("root", defaultValue);
    return toPath(root.asString()).remove_trailing_separator();
  }

  openstudio::path WorkflowJSON_Impl::absoluteRootDir() const {
    openstudio::path result = rootDir();
    if (result.is_relative()) {
      return canonicalOrAbsolute(result, oswDir());
    }
    return result;
  }

  openstudio::path WorkflowJSON_Impl::runDir() const {
    Json::Value defaultValue("./run");
    Json::Value runDirectory = m_value.get("run_directory", defaultValue);
    return toPath(runDirectory.asString()).remove_trailing_separator();
  }

  openstudio::path WorkflowJSON_Impl::absoluteRunDir() const {
    openstudio::path result = runDir();
    if (result.is_relative()) {
      return canonicalOrAbsolute(result, absoluteRootDir());
    }
    return result;
  }

  openstudio::path WorkflowJSON_Impl::outPath() const {
    Json::Value defaultValue("out.osw");
    Json::Value path = m_value.get("out_name", defaultValue);
    return toPath(path.asString());
  }

  openstudio::path WorkflowJSON_Impl::absoluteOutPath() const {
    openstudio::path result = outPath();
    if (result.is_relative()) {
      return canonicalOrAbsolute(result, oswDir());
    }
    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::filePaths() const {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);

    Json::Value paths = m_value.get("file_paths", defaultValue);
    paths.append("./files");
    paths.append("./weather");
    paths.append("../../files");
    paths.append("../../weather");
    paths.append("./");

    Json::ArrayIndex n = paths.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.push_back(toPath(paths[i].asString()));
    }

    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::absoluteFilePaths() const {
    std::vector<openstudio::path> result;
    for (const auto& path : filePaths()) {
      if (path.is_absolute()) {
        result.push_back(path);
      } else {
        result.push_back(canonicalOrAbsolute(path, absoluteRootDir()));
      }
    }
    return result;
  }

  bool WorkflowJSON_Impl::addFilePath(const openstudio::path& path) {
    if (!m_value.isMember("file_paths") || !m_value["file_paths"].isArray()) {
      m_value["file_paths"] = Json::Value(Json::arrayValue);
    }
    m_value["file_paths"].append(toString(path));
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::resetFilePaths() {
    m_value["file_paths"] = Json::Value(Json::arrayValue);
    onUpdate();
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const openstudio::path& file) const {
    // if absolute check if exists
    if (file.is_absolute()) {
      if (boost::filesystem::exists(file)) {
        return file;
      }

      // absolute path does not exist
      return boost::none;
    }

    for (const auto& path : absoluteFilePaths()) {
      openstudio::path p = path / file;
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)) {
        return canonicalOrAbsolute(p);
      }
    }

    // Extra check: if it starts with file://
    std::string fileName = toString(file);
    if (fileName.rfind("file://", 0) == 0) {
      // We strip it, and try again
      return findFile(toPath(fileName.substr(7)));

      // if it starts with 'file:' (some test files - at least seb.osm - on OpenStudio-workflow-gem have that...)
      // eg: 'file:files/USA_CO_Golden-NREL.724666_TMY3.epw'
    } else if (fileName.rfind("file:", 0) == 0) {
      // We strip it, and try again
      return findFile(toPath(fileName.substr(5)));
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const std::string& fileName) const {
    return findFile(toPath(fileName));
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::measurePaths() const {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);

    Json::Value paths = m_value.get("measure_paths", defaultValue);
    paths.append("./measures");
    paths.append("../../measures");
    paths.append("./");

    Json::ArrayIndex n = paths.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.push_back(toPath(paths[i].asString()).remove_trailing_separator());
    }

    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::absoluteMeasurePaths() const {
    std::vector<openstudio::path> result;
    for (const auto& path : measurePaths()) {
      if (path.is_absolute()) {
        result.push_back(path);
      } else {
        result.push_back(canonicalOrAbsolute(path, absoluteRootDir()));
      }
    }
    return result;
  }

  bool WorkflowJSON_Impl::addMeasurePath(const openstudio::path& path) {
    if (!m_value.isMember("measure_paths") || !m_value["measure_paths"].isArray()) {
      m_value["measure_paths"] = Json::Value(Json::arrayValue);
    }
    m_value["measure_paths"].append(toString(path));
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::resetMeasurePaths() {
    m_value["measure_paths"] = Json::Value(Json::arrayValue);
    onUpdate();
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const openstudio::path& measureDir) const {
    // if absolute check if exists
    if (measureDir.is_absolute()) {
      if (boost::filesystem::exists(measureDir)) {
        return measureDir;
      }

      // absolute path does not exist
      return boost::none;
    }

    for (const auto& path : absoluteMeasurePaths()) {
      openstudio::path p = path / measureDir;
      if (boost::filesystem::exists(p) && boost::filesystem::is_directory(p)) {
        return canonicalOrAbsolute(p);
      }
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const std::string& measureDirName) const {
    return findMeasure(toPath(measureDirName));
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::seedFile() const {
    Json::Value defaultValue("");
    Json::Value seed = m_value.get("seed_file", defaultValue);
    std::string result = seed.asString();
    if (result.empty()) {
      return boost::none;
    }
    return toPath(result);
  }

  bool WorkflowJSON_Impl::setSeedFile(const openstudio::path& seedFile) {
    m_value["seed_file"] = toString(seedFile);
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::resetSeedFile() {
    m_value.removeMember("seed_file");
    onUpdate();
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::weatherFile() const {
    Json::Value defaultValue("");
    Json::Value weather = m_value.get("weather_file", defaultValue);
    std::string result = weather.asString();
    if (result.empty()) {
      return boost::none;
    }
    return toPath(result);
  }

  bool WorkflowJSON_Impl::setWeatherFile(const openstudio::path& weatherFile) {
    m_value["weather_file"] = toString(weatherFile);
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::resetWeatherFile() {
    m_value.removeMember("weather_file");
    onUpdate();
  }

  std::vector<WorkflowStep> WorkflowJSON_Impl::workflowSteps() const {
    return m_steps;
  }

  bool WorkflowJSON_Impl::setWorkflowSteps(const std::vector<WorkflowStep>& workflowSteps) {
    disconnectSteps();
    m_steps = workflowSteps;
    setMeasureTypes();
    connectSteps();
    onUpdate();  // onUpdate does not happen in setMeasureTypes
    return true;
  }

  void WorkflowJSON_Impl::resetWorkflowSteps() {
    bool test = setWorkflowSteps(std::vector<WorkflowStep>());  // will hit onUpdate in setWorkflowSteps
    OS_ASSERT(test);
  }

  std::vector<MeasureStep> WorkflowJSON_Impl::getMeasureSteps(const MeasureType& measureType) const {
    std::vector<MeasureStep> result;
    size_t n = m_steps.size();
    OS_ASSERT(m_measureTypes.size() == n);
    for (size_t i = 0; i < n; ++i) {
      if (m_steps[i].optionalCast<MeasureStep>()) {
        if (m_measureTypes[i] == measureType.value()) {
          result.push_back(m_steps[i].cast<MeasureStep>());
        }
      }
    }
    return result;
  }

  std::vector<std::pair<unsigned, MeasureStep>> WorkflowJSON_Impl::getMeasureStepsWithIndex(const MeasureType& measureType) const {
    std::vector<std::pair<unsigned, MeasureStep>> result;
    size_t n = m_steps.size();
    OS_ASSERT(m_measureTypes.size() == n);
    for (size_t i = 0; i < n; ++i) {
      if (m_steps[i].optionalCast<MeasureStep>()) {
        if (m_measureTypes[i] == measureType.value()) {
          result.emplace_back(i, m_steps[i].cast<MeasureStep>());
        }
      }
    }
    return result;
  }

  bool WorkflowJSON_Impl::setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps) {

    // verify steps
    for (const auto& step : steps) {
      boost::optional<BCLMeasure> bclMeasure = getBCLMeasure(step);
      if (!bclMeasure) {
        return false;
      }
      if (bclMeasure->measureType() != measureType) {
        return false;
      }
    }

    // DLM: this implementation may evolve
    std::vector<WorkflowStep> newSteps;

    if (measureType == MeasureType::ModelMeasure) {
      newSteps.insert(newSteps.end(), steps.begin(), steps.end());
    } else {
      std::vector<MeasureStep> oldSteps = getMeasureSteps(MeasureType::ModelMeasure);
      newSteps.insert(newSteps.end(), oldSteps.begin(), oldSteps.end());
    }

    if (measureType == MeasureType::EnergyPlusMeasure) {
      newSteps.insert(newSteps.end(), steps.begin(), steps.end());
    } else {
      std::vector<MeasureStep> oldSteps = getMeasureSteps(MeasureType::EnergyPlusMeasure);
      newSteps.insert(newSteps.end(), oldSteps.begin(), oldSteps.end());
    }

    if (measureType == MeasureType::ReportingMeasure) {
      newSteps.insert(newSteps.end(), steps.begin(), steps.end());
    } else {
      std::vector<MeasureStep> oldSteps = getMeasureSteps(MeasureType::ReportingMeasure);
      newSteps.insert(newSteps.end(), oldSteps.begin(), oldSteps.end());
    }

    return setWorkflowSteps(newSteps);  // onUpdate in setWorkflowSteps
  }

  boost::optional<BCLMeasure> WorkflowJSON_Impl::getBCLMeasure(const MeasureStep& step) const {
    boost::optional<openstudio::path> path = findMeasure(step.measureDirName());
    if (path) {
      return BCLMeasure::load(*path);
    }
    return boost::none;
  }

  boost::optional<BCLMeasure> WorkflowJSON_Impl::getBCLMeasureByUUID(const UUID& id) const {
    for (const auto& step : m_steps) {
      if (step.optionalCast<MeasureStep>()) {
        boost::optional<BCLMeasure> bclMeasure = getBCLMeasure(step.cast<MeasureStep>());
        if (bclMeasure && (bclMeasure->uuid() == id)) {
          return bclMeasure;
        }
      }
    }
    return boost::none;
  }

  boost::optional<BCLMeasure> WorkflowJSON_Impl::addMeasure(const BCLMeasure& bclMeasure) {
    boost::optional<BCLMeasure> existingMeasure = getBCLMeasureByUUID(bclMeasure.uuid());
    boost::optional<openstudio::path> existingMeasureDirName;
    if (existingMeasure) {
      // TODO: check that measure type has not changed?

      existingMeasureDirName = existingMeasure->directory();
      boost::filesystem::remove_all(*existingMeasureDirName);
    }

    std::vector<openstudio::path> paths = absoluteMeasurePaths();
    OS_ASSERT(!paths.empty());

    // Get the name of the directory (=last level directory name), eg: /path/to/measure_folder => measure_folder
    openstudio::path lastLevelDirectoryName = getLastLevelDirectoryName(bclMeasure.directory());
    if (!toUUID(toString(lastLevelDirectoryName)).isNull()) {
      // directory name is convertible to uuid, use the class name
      lastLevelDirectoryName = toPath(bclMeasure.className());
    }

    int i = 1;
    while (boost::filesystem::exists(paths[0] / lastLevelDirectoryName)) {
      std::stringstream ss;
      ss << toString(lastLevelDirectoryName) << " " << i;
      lastLevelDirectoryName = toPath(ss.str());
    }
    openstudio::path newMeasureDirName = paths[0] / lastLevelDirectoryName;

    // If we have an existing measure
    if (existingMeasureDirName) {
      openstudio::path lastLevelExistingDirectoryName = getLastLevelDirectoryName(*existingMeasureDirName);
      // And the previous directory name isn't the same as the new one
      if (lastLevelExistingDirectoryName != lastLevelDirectoryName) {
        // update steps
        for (auto& step : m_steps) {
          if (auto measureStep = step.optionalCast<MeasureStep>()) {
            if (measureStep->measureDirName() == toString(lastLevelExistingDirectoryName)) {
              measureStep->setMeasureDirName(toString(lastLevelDirectoryName));
            }
          }
        }
      }
    }

    onUpdate();

    return bclMeasure.clone(newMeasureDirName);
  }

  boost::optional<RunOptions> WorkflowJSON_Impl::runOptions() const {
    return m_runOptions;
  }

  bool WorkflowJSON_Impl::setRunOptions(const RunOptions& options) {
    disconnectRunOptions();
    m_runOptions = options;
    connectRunOptions();
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::resetRunOptions() {
    disconnectRunOptions();
    m_runOptions.reset();
    onUpdate();
  }

  void WorkflowJSON_Impl::onUpdate() {
    m_value["updated_at"] = DateTime::nowUTC().toISO8601();
    this->onChange.nano_emit();
  }

  void WorkflowJSON_Impl::parseSteps() {
    disconnectSteps();
    m_steps.clear();

    Json::Value defaultValue(Json::arrayValue);
    Json::Value steps = m_value.get("steps", defaultValue);

    Json::ArrayIndex n = steps.size();

    if (n > 0) {
      Json::StreamWriterBuilder wbuilder;

      // mimic the old FastWriter behavior:
      wbuilder["commentStyle"] = "None";
      wbuilder["indentation"] = "";

      for (Json::ArrayIndex i = 0; i < n; ++i) {
        Json::Value step = steps[i];

        std::string s = Json::writeString(wbuilder, step);

        boost::optional<WorkflowStep> workflowStep = WorkflowStep::fromString(s);
        if (workflowStep) {
          m_steps.push_back(*workflowStep);
        } else {
          LOG_AND_THROW("Step " << i << " cannot be processed");
        }
      }
    }

    connectSteps();

    m_value.removeMember("steps");
  }

  void WorkflowJSON_Impl::disconnectSteps() {
    for (const auto& step : m_steps) {
      step.getImpl<detail::WorkflowStep_Impl>().get()->WorkflowStep_Impl::onChange.disconnect<WorkflowJSON_Impl, &WorkflowJSON_Impl::onUpdate>(this);
    }
  }

  void WorkflowJSON_Impl::connectSteps() {
    for (const auto& step : m_steps) {
      step.getImpl<detail::WorkflowStep_Impl>().get()->WorkflowStep_Impl::onChange.connect<WorkflowJSON_Impl, &WorkflowJSON_Impl::onUpdate>(this);
    }
  }

  void WorkflowJSON_Impl::parseRunOptions() {
    if (m_runOptions) {
      disconnectRunOptions();
      m_runOptions.reset();
    }

    if (m_value.isMember("run_options")) {
      Json::Value options = m_value["run_options"];

      Json::StreamWriterBuilder wbuilder;
      // mimic the old FastWriter behavior:
      wbuilder["commentStyle"] = "None";
      wbuilder["indentation"] = "";

      std::string s = Json::writeString(wbuilder, options);

      m_runOptions = RunOptions::fromString(s);
      if (!m_runOptions) {
        LOG_AND_THROW("Run options cannot be processed");
      }

      connectRunOptions();
      m_value.removeMember("run_options");
    }
  }

  void WorkflowJSON_Impl::disconnectRunOptions() {
    if (m_runOptions) {
      m_runOptions->getImpl<detail::RunOptions_Impl>().get()->RunOptions_Impl::onChange.disconnect<WorkflowJSON_Impl, &WorkflowJSON_Impl::onUpdate>(
        this);
    }
  }

  void WorkflowJSON_Impl::connectRunOptions() {
    if (m_runOptions) {
      m_runOptions->getImpl<detail::RunOptions_Impl>().get()->RunOptions_Impl::onChange.connect<WorkflowJSON_Impl, &WorkflowJSON_Impl::onUpdate>(
        this);
    }
  }

  void WorkflowJSON_Impl::setMeasureTypes() {
    m_measureTypes.clear();
    m_measureTypes.reserve(m_steps.size());
    for (const auto& step : m_steps) {
      if (step.optionalCast<MeasureStep>()) {
        boost::optional<BCLMeasure> bclMeasure = getBCLMeasure(step.cast<MeasureStep>());
        if (bclMeasure) {
          m_measureTypes.push_back(bclMeasure->measureType().value());
        } else {
          m_measureTypes.push_back(NO_MEASURE_TYPE);
        }
      } else {
        m_measureTypes.push_back(NO_MEASURE_TYPE);
      }
    }
  }

  bool WorkflowJSON_Impl::validateMeasures() const {
    // TODO: should we exit early, or return all problems found?

    bool result = true;
    MeasureType state = MeasureType::ModelMeasure;

    for (size_t i = 0; const auto& step : m_steps) {
      LOG(Debug, "Validating step " << i);
      if (auto step_ = step.optionalCast<MeasureStep>()) {
        // Not calling getBCLMeasure because I want to mimic workflow-gem and be as explicit as possible about what went wrong
        const auto measureDirName = step_->measureDirName();
        auto measurePath_ = findMeasure(measureDirName);
        if (!measurePath_) {
          LOG(Error, "Cannot find measure '" << measureDirName << "'");
          result = false;
          continue;
        }
        auto bclMeasure_ = BCLMeasure::load(*measurePath_);
        if (!bclMeasure_) {
          LOG(Error, "Cannot load measure '" << measureDirName << "' at '" << *measurePath_ << "'");
          result = false;
          continue;
        }

        // Ensure that measures are in order, i.e. no OS after E+, E+ or OS after Reporting
        const auto measureType = bclMeasure_->measureType();

        if (measureType == MeasureType::ModelMeasure) {
          if (state == MeasureType::EnergyPlusMeasure) {
            LOG(Error, "OpenStudio measure '" << measureDirName << "' called after transition to EnergyPlus.");
            result = false;
          }
          if (state == MeasureType::ReportingMeasure) {
            LOG(Error, "OpenStudio measure '" << measureDirName << "' called after Energyplus simulation.");
            result = false;
          }

        } else if (measureType == MeasureType::EnergyPlusMeasure) {
          if (state == MeasureType::ReportingMeasure) {
            LOG(Error, "EnergyPlus measure '" << measureDirName << "' called after Energyplus simulation.");
            result = false;
          }
          if (state == MeasureType::ModelMeasure) {
            state = MeasureType::EnergyPlusMeasure;
          }

        } else if (measureType == MeasureType::ReportingMeasure) {
          state = MeasureType::ReportingMeasure;

        } else {
          LOG(Error, "MeasureType " << measureType.valueName() << " of measure '" << measureDirName << "' is not supported");
          result = false;
        }
      }
      ++i;
    }

    return result;
  }
}  // namespace detail

WorkflowJSON::WorkflowJSON() : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl())) {}

WorkflowJSON::WorkflowJSON(const std::string& s) : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl(s))) {}

WorkflowJSON::WorkflowJSON(const openstudio::path& p) : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl(p))) {}

WorkflowJSON::WorkflowJSON(std::shared_ptr<detail::WorkflowJSON_Impl> impl) : m_impl(impl) {}

WorkflowJSON WorkflowJSON::clone() const {
  return getImpl<detail::WorkflowJSON_Impl>()->clone();
}

boost::optional<WorkflowJSON> WorkflowJSON::load(const std::string& s) {
  boost::optional<WorkflowJSON> result;
  try {
    result = WorkflowJSON(s);
  } catch (const std::exception&) {
  }
  return result;
}

boost::optional<WorkflowJSON> WorkflowJSON::load(const openstudio::path& p) {
  boost::optional<WorkflowJSON> result;
  try {
    result = WorkflowJSON(p);
  } catch (const std::exception&) {
  }
  return result;
}

Json::Value WorkflowJSON::toJSON(bool includeHash) const {
  return getImpl<detail::WorkflowJSON_Impl>()->toJSON(includeHash);
}

std::string WorkflowJSON::string(bool includeHash) const {
  return getImpl<detail::WorkflowJSON_Impl>()->string(includeHash);
}

std::string WorkflowJSON::hash() const {
  return getImpl<detail::WorkflowJSON_Impl>()->hash();
}

std::string WorkflowJSON::computeHash() const {
  return getImpl<detail::WorkflowJSON_Impl>()->computeHash();
}

bool WorkflowJSON::checkForUpdates() {
  return getImpl<detail::WorkflowJSON_Impl>()->checkForUpdates();
}

bool WorkflowJSON::save() const {
  return getImpl<detail::WorkflowJSON_Impl>()->save();
}

bool WorkflowJSON::saveAs(const openstudio::path& p) {
  return getImpl<detail::WorkflowJSON_Impl>()->saveAs(p);
}

void WorkflowJSON::reset() {
  getImpl<detail::WorkflowJSON_Impl>()->reset();
}

void WorkflowJSON::start() {
  getImpl<detail::WorkflowJSON_Impl>()->start();
}

unsigned WorkflowJSON::currentStepIndex() const {
  return getImpl<detail::WorkflowJSON_Impl>()->currentStepIndex();
}

boost::optional<WorkflowStep> WorkflowJSON::currentStep() const {
  return getImpl<detail::WorkflowJSON_Impl>()->currentStep();
}

bool WorkflowJSON::incrementStep() {
  return getImpl<detail::WorkflowJSON_Impl>()->incrementStep();
}

boost::optional<std::string> WorkflowJSON::completedStatus() const {
  return getImpl<detail::WorkflowJSON_Impl>()->completedStatus();
}

void WorkflowJSON::setCompletedStatus(const std::string& status) {
  return getImpl<detail::WorkflowJSON_Impl>()->setCompletedStatus(status);
}

boost::optional<DateTime> WorkflowJSON::createdAt() const {
  return getImpl<detail::WorkflowJSON_Impl>()->createdAt();
}

boost::optional<DateTime> WorkflowJSON::startedAt() const {
  return getImpl<detail::WorkflowJSON_Impl>()->startedAt();
}

boost::optional<DateTime> WorkflowJSON::updatedAt() const {
  return getImpl<detail::WorkflowJSON_Impl>()->updatedAt();
}

boost::optional<DateTime> WorkflowJSON::completedAt() const {
  return getImpl<detail::WorkflowJSON_Impl>()->completedAt();
}

boost::optional<std::string> WorkflowJSON::eplusoutErr() const {
  return getImpl<detail::WorkflowJSON_Impl>()->eplusoutErr();
}

void WorkflowJSON::setEplusoutErr(const std::string& eplusoutErr) {
  getImpl<detail::WorkflowJSON_Impl>()->setEplusoutErr(eplusoutErr);
}

boost::optional<openstudio::path> WorkflowJSON::oswPath() const {
  return getImpl<detail::WorkflowJSON_Impl>()->oswPath();
}

bool WorkflowJSON::setOswPath(const openstudio::path& path) {
  return getImpl<detail::WorkflowJSON_Impl>()->setOswPath(path, true);
}

openstudio::path WorkflowJSON::oswDir() const {
  return getImpl<detail::WorkflowJSON_Impl>()->oswDir();
}

bool WorkflowJSON::setOswDir(const openstudio::path& path) {
  return getImpl<detail::WorkflowJSON_Impl>()->setOswDir(path);
}

openstudio::path WorkflowJSON::rootDir() const {
  return getImpl<detail::WorkflowJSON_Impl>()->rootDir();
}

openstudio::path WorkflowJSON::absoluteRootDir() const {
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteRootDir();
}

openstudio::path WorkflowJSON::runDir() const {
  return getImpl<detail::WorkflowJSON_Impl>()->runDir();
}

openstudio::path WorkflowJSON::absoluteRunDir() const {
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteRunDir();
}

openstudio::path WorkflowJSON::outPath() const {
  return getImpl<detail::WorkflowJSON_Impl>()->outPath();
}

openstudio::path WorkflowJSON::absoluteOutPath() const {
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteOutPath();
}

std::vector<openstudio::path> WorkflowJSON::filePaths() const {
  return getImpl<detail::WorkflowJSON_Impl>()->filePaths();
}

std::vector<openstudio::path> WorkflowJSON::absoluteFilePaths() const {
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteFilePaths();
}

bool WorkflowJSON::addFilePath(const openstudio::path& path) {
  return getImpl<detail::WorkflowJSON_Impl>()->addFilePath(path);
}

void WorkflowJSON::resetFilePaths() {
  getImpl<detail::WorkflowJSON_Impl>()->resetFilePaths();
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const openstudio::path& file) const {
  return getImpl<detail::WorkflowJSON_Impl>()->findFile(file);
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const std::string& fileName) const {
  return getImpl<detail::WorkflowJSON_Impl>()->findFile(fileName);
}

std::vector<openstudio::path> WorkflowJSON::measurePaths() const {
  return getImpl<detail::WorkflowJSON_Impl>()->measurePaths();
}

std::vector<openstudio::path> WorkflowJSON::absoluteMeasurePaths() const {
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteMeasurePaths();
}

bool WorkflowJSON::addMeasurePath(const openstudio::path& path) {
  return getImpl<detail::WorkflowJSON_Impl>()->addMeasurePath(path);
}

void WorkflowJSON::resetMeasurePaths() {
  return getImpl<detail::WorkflowJSON_Impl>()->resetMeasurePaths();
}

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const openstudio::path& measureDir) const {
  return getImpl<detail::WorkflowJSON_Impl>()->findMeasure(measureDir);
}

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const std::string& measureDirName) const {
  return getImpl<detail::WorkflowJSON_Impl>()->findMeasure(measureDirName);
}

boost::optional<openstudio::path> WorkflowJSON::seedFile() const {
  return getImpl<detail::WorkflowJSON_Impl>()->seedFile();
}

bool WorkflowJSON::setSeedFile(const openstudio::path& seedFile) {
  return getImpl<detail::WorkflowJSON_Impl>()->setSeedFile(seedFile);
}

void WorkflowJSON::resetSeedFile() {
  return getImpl<detail::WorkflowJSON_Impl>()->resetSeedFile();
}

boost::optional<openstudio::path> WorkflowJSON::weatherFile() const {
  return getImpl<detail::WorkflowJSON_Impl>()->weatherFile();
}

bool WorkflowJSON::setWeatherFile(const openstudio::path& weatherFile) {
  return getImpl<detail::WorkflowJSON_Impl>()->setWeatherFile(weatherFile);
}

void WorkflowJSON::resetWeatherFile() {
  return getImpl<detail::WorkflowJSON_Impl>()->resetWeatherFile();
}

std::vector<WorkflowStep> WorkflowJSON::workflowSteps() const {
  return getImpl<detail::WorkflowJSON_Impl>()->workflowSteps();
}

bool WorkflowJSON::setWorkflowSteps(const std::vector<WorkflowStep>& steps) {
  return getImpl<detail::WorkflowJSON_Impl>()->setWorkflowSteps(steps);
}

void WorkflowJSON::resetWorkflowSteps() {
  getImpl<detail::WorkflowJSON_Impl>()->resetWorkflowSteps();
}

std::vector<MeasureStep> WorkflowJSON::getMeasureSteps(const MeasureType& measureType) const {
  return getImpl<detail::WorkflowJSON_Impl>()->getMeasureSteps(measureType);
}

std::vector<std::pair<unsigned, MeasureStep>> WorkflowJSON::getMeasureStepsWithIndex(const MeasureType& measureType) const {
  return getImpl<detail::WorkflowJSON_Impl>()->getMeasureStepsWithIndex(measureType);
}

bool WorkflowJSON::setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps) {
  return getImpl<detail::WorkflowJSON_Impl>()->setMeasureSteps(measureType, steps);
}

boost::optional<BCLMeasure> WorkflowJSON::getBCLMeasure(const MeasureStep& step) const {
  return getImpl<detail::WorkflowJSON_Impl>()->getBCLMeasure(step);
}

boost::optional<BCLMeasure> WorkflowJSON::getBCLMeasureByUUID(const UUID& id) const {
  return getImpl<detail::WorkflowJSON_Impl>()->getBCLMeasureByUUID(id);
}

boost::optional<BCLMeasure> WorkflowJSON::addMeasure(const BCLMeasure& bclMeasure) {
  return getImpl<detail::WorkflowJSON_Impl>()->addMeasure(bclMeasure);
}

boost::optional<RunOptions> WorkflowJSON::runOptions() const {
  return getImpl<detail::WorkflowJSON_Impl>()->runOptions();
}

bool WorkflowJSON::setRunOptions(const RunOptions& options) {
  return getImpl<detail::WorkflowJSON_Impl>()->setRunOptions(options);
}

void WorkflowJSON::resetRunOptions() {
  getImpl<detail::WorkflowJSON_Impl>()->resetRunOptions();
}

bool WorkflowJSON::validateMeasures() const {
  return getImpl<detail::WorkflowJSON_Impl>()->validateMeasures();
}

std::ostream& operator<<(std::ostream& os, const WorkflowJSON& workflowJSON) {
  os << workflowJSON.string();
  return os;
}

}  // namespace openstudio
