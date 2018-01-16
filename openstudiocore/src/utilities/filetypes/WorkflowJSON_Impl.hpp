/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "WorkflowStep.hpp"
#include "RunOptions.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <nano/nano_signal_slot.hpp>

#include <jsoncpp/json.h>

namespace openstudio{

class DateTime;

namespace detail {

    class UTILITIES_API WorkflowJSON_Impl
    {
    public:

      WorkflowJSON_Impl();

      WorkflowJSON_Impl(const std::string& s);

      WorkflowJSON_Impl(const openstudio::path& p);

      WorkflowJSON clone() const;

      std::string string(bool includeHash = true) const;

      std::string hash() const;

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

      bool setWorkflowSteps(const std::vector<WorkflowStep>& steps);

      void resetWorkflowSteps();

      std::vector<MeasureStep> getMeasureSteps(const MeasureType& measureType) const;

      bool setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps);

      boost::optional<BCLMeasure> getBCLMeasure(const MeasureStep& step) const;

      boost::optional<BCLMeasure> getBCLMeasureByUUID(const UUID& id) const;

      boost::optional<BCLMeasure> addMeasure(const BCLMeasure& bclMeasure);

      boost::optional<RunOptions> runOptions() const;

      bool setRunOptions(const RunOptions& options);

      void resetRunOptions();

      // Emitted on any change
      Nano::Signal<void()> onChange;

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

} // detail
} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
