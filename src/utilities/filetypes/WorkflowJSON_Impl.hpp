/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "WorkflowStep.hpp"
#include "RunOptions.hpp"

#include "../core/Path.hpp"

#include <nano/nano_signal_slot.hpp>

#include <json/json.h>

namespace openstudio {

class DateTime;
class WorkflowJSON;
class BCLMeasure;

namespace detail {

  class UTILITIES_API WorkflowJSON_Impl
  {
   public:
    WorkflowJSON_Impl();

    WorkflowJSON_Impl(const std::string& s);

    WorkflowJSON_Impl(const openstudio::path& p);

    // Note: I'm keeping this one because I don't want to bother writting the copy/move constructors and assignment operators to handle the connection
    // of the nano signals
    WorkflowJSON clone() const;

    Json::Value toJSON(bool includeHash = true) const;

    std::string string(bool includeHash = true) const;

    std::string hash() const;

    // cppcheck-suppress functionStatic
    std::string computeHash() const;

    bool checkForUpdates();

    bool save() const;

    bool saveAs(const openstudio::path& p);

    void reset();

    void start();

    unsigned currentStepIndex() const;

    boost::optional<WorkflowStep> currentStep() const;

    bool incrementStep();

    boost::optional<std::string> completedStatus() const;

    void setCompletedStatus(const std::string& status);

    boost::optional<DateTime> createdAt() const;

    boost::optional<DateTime> startedAt() const;

    boost::optional<DateTime> updatedAt() const;

    boost::optional<DateTime> completedAt() const;

    boost::optional<std::string> eplusoutErr() const;

    void setEplusoutErr(const std::string& eplusoutErr);

    boost::optional<openstudio::path> oswPath() const;

    bool setOswPath(const openstudio::path& path, bool emitChange);

    openstudio::path oswDir() const;

    bool setOswDir(const openstudio::path& path);

    openstudio::path rootDir() const;
    openstudio::path absoluteRootDir() const;

    openstudio::path runDir() const;
    openstudio::path absoluteRunDir() const;

    openstudio::path outPath() const;
    openstudio::path absoluteOutPath() const;

    std::vector<openstudio::path> filePaths() const;
    std::vector<openstudio::path> absoluteFilePaths() const;

    bool addFilePath(const openstudio::path& path);
    void resetFilePaths();

    boost::optional<openstudio::path> findFile(const openstudio::path& file) const;
    boost::optional<openstudio::path> findFile(const std::string& fileName) const;

    std::vector<openstudio::path> measurePaths() const;
    std::vector<openstudio::path> absoluteMeasurePaths() const;

    bool addMeasurePath(const openstudio::path& path);
    void resetMeasurePaths();

    boost::optional<openstudio::path> findMeasure(const openstudio::path& measureDir) const;
    boost::optional<openstudio::path> findMeasure(const std::string& measureDirName) const;

    boost::optional<openstudio::path> seedFile() const;

    void resetSeedFile();

    bool setSeedFile(const openstudio::path& seedFile);

    boost::optional<openstudio::path> weatherFile() const;

    bool setWeatherFile(const openstudio::path& weatherFile);

    void resetWeatherFile();

    std::vector<WorkflowStep> workflowSteps() const;

    bool setWorkflowSteps(const std::vector<WorkflowStep>& workflowSteps);

    void resetWorkflowSteps();

    std::vector<MeasureStep> getMeasureSteps(const MeasureType& measureType) const;

    std::vector<std::pair<unsigned, MeasureStep>> getMeasureStepsWithIndex(const MeasureType& measureType) const;

    bool setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps);

    boost::optional<BCLMeasure> getBCLMeasure(const MeasureStep& step) const;

    boost::optional<BCLMeasure> getBCLMeasureByUUID(const UUID& id) const;

    boost::optional<BCLMeasure> addMeasure(const BCLMeasure& bclMeasure);

    boost::optional<RunOptions> runOptions() const;

    bool setRunOptions(const RunOptions& options);

    void resetRunOptions();

    // Emitted on any change
    Nano::Signal<void()> onChange;

    bool validateMeasures() const;

   private:
    REGISTER_LOGGER("openstudio.WorkflowJSON");

    void onUpdate();

    void parseSteps();

    void disconnectSteps();

    void connectSteps();

    void parseRunOptions();

    void disconnectRunOptions();

    void connectRunOptions();

    // synchronize m_measureTypes with m_steps
    void setMeasureTypes();

    openstudio::path m_oswDir;
    openstudio::path m_oswFilename;
    Json::Value m_value;
    std::vector<WorkflowStep> m_steps;
    std::vector<int> m_measureTypes;
    boost::optional<RunOptions> m_runOptions;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
