/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../bcl/BCLMeasure.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

class Filetypes_WorkflowJSON_Signals_Test;

namespace openstudio{

class WorkflowStep;
class RunOptions;
class MeasureStep;
class DateTime;
class OSDocument;

namespace detail{
  class WorkflowJSON_Impl;
}
  
/** Class for accessing the OpenStudio Workflow (OSW) JSON format. */
class UTILITIES_API WorkflowJSON
{
public:

  /** Create a new, empty workflow. */
  WorkflowJSON();

  /** Constructor with string, will throw if string is incorrect. */
  WorkflowJSON(const std::string& s);

  /** Constructor with path, will throw if path does not exist or file is incorrect. */
  WorkflowJSON(const openstudio::path& p);
  
  /** Clones this WorkflowJSON into a separate one. */
  WorkflowJSON clone() const;

  /** Attempt to load a WorkflowJSON from string */
  static boost::optional<WorkflowJSON> load(const std::string& s);

  /** Attempt to load a WorkflowJSON from path */
  static boost::optional<WorkflowJSON> load(const openstudio::path& p);

  /** Get the workflow as a string. */
  std::string string(bool includeHash=true) const;

  /** Get a stored hash of the workflow. */
  std::string hash() const;

  /** Compute the current hash of the workflow. */
  std::string computeHash() const;

  /** Check for updates and return true if there are any, updates value of the stored hash. */
  bool checkForUpdates();

  /** Saves this file to the current location. */
  bool save() const;

  /** Saves this file to a new location. Updates the OSW path. */
  bool saveAs(const openstudio::path& p);

  /** Reset to re-run the workflow, does not delete steps. */
  void reset();

  /** Sets the started at time. */
  void start();

  /** Get the zero based index of the current step. 
  *   If a simulation completes normally, current step index will be one higher than index of the last step.
  *   If a simulation is halted, current step index will point to the step after the last run step.
  */
  unsigned currentStepIndex() const;

  /** Get the current step.*/
  boost::optional<WorkflowStep> currentStep() const;

  /** Increments current step, returns true if there is another step. */
  bool incrementStep();

  /** Returns the completion status, "Success", "Fail", "Invalid", or "Cancel". 
  *   "Success" will be set automatically if all steps in the workflow complete successfully.
  *   "Fail" will be set automatically if any step in the workflow reports an error.  
  *   "Invalid" can be set by a measure if the workflow requests parameter combinations that are not valid.
  *   "Cancel" will be set automatically if a workflow is cancelled externally during execution.
  */
  boost::optional<std::string> completedStatus() const;

  /** Sets the completion status, "Success", "Fail", "Invalid", or "Cancel". 
  *   Measure writers can call this with "Success" if all required results have been generated.
  *   Measure writers should not call this with "Fail", runner.registerError should be used instead.  
  *   Measure writers can call this with "Invalid" if the workflow requests parameter combinations that are not valid.
  *   Measure writers should not call this with "Cancel", this is reserved for external cancel operations. 
  */
  void setCompletedStatus(const std::string& status);

  /** Returns the time this WorkflowJSON was created at. */
  boost::optional<DateTime> createdAt() const;

  /** Returns the time this WorkflowJSON was started at. */
  boost::optional<DateTime> startedAt() const;

  /** Returns the time this WorkflowJSON was updated at. */
  boost::optional<DateTime> updatedAt() const;

  /** Returns the time this WorkflowJSON was completed at. */
  boost::optional<DateTime> completedAt() const;

  /** Returns the content of eplusout.err file. */
  boost::optional<std::string> eplusoutErr() const;

  /** Sets the content of eplusout.err file. */
  void setEplusoutErr(const std::string& eplusoutErr);

  /** Returns the absolute path this workflow was loaded from or saved to, empty for new WorkflowJSON. */
  boost::optional<openstudio::path> oswPath() const;

  /** Sets the absolute path for this workflow. */
  bool setOswPath(const openstudio::path& path);

  /** Returns the absolute path to the directory this workflow was loaded from or saved to.  Returns current working dir for new WorkflowJSON. */
  openstudio::path oswDir() const;

  /** Sets the oswDir.  If oswPath is empty this does not set it.  If oswPath is set, this changes directories but preserves file name. */
  bool setOswDir(const openstudio::path& path);

  /** Returns the root directory, default value is '.'. Evaluated relative to oswDir if not absolute. */
  openstudio::path rootDir() const;
  openstudio::path absoluteRootDir() const;

  /** Returns the run directory, default value is './run'. Evaluated relative to rootDir if not absolute. */
  openstudio::path runDir() const;
  openstudio::path absoluteRunDir() const;

  /** Returns the path to write output OSW, default value is 'out.osw'. Evaluated relative to oswDir to ensure relative paths remain valid. */
  openstudio::path outPath() const;
  openstudio::path absoluteOutPath() const;

  /** Returns the paths that will be searched in order for files, default value is './files/'. Evaluated relative to rootDir if not absolute. */
  std::vector<openstudio::path> filePaths() const;
  std::vector<openstudio::path> absoluteFilePaths() const;

  /** Add a path to the paths searched for files. */
  bool addFilePath(const openstudio::path& path);

  /** Clear paths searched for files. */
  void resetFilePaths();

  /** Attempts to find a file by path.
  *   If file is relative, searches through filePaths in order and returns first match that exists. 
  *   If file is absolute, does not search for file in filePaths, only returns true it file exists.
  */
  boost::optional<openstudio::path> findFile(const openstudio::path& file) const;
  boost::optional<openstudio::path> findFile(const std::string& fileName) const;

  /** Returns the paths that will be searched in order for measures, default value is './measures/'. Evaluated relative to rootDir if not absolute. */
  std::vector<openstudio::path> measurePaths() const;
  std::vector<openstudio::path> absoluteMeasurePaths() const;

  /** Add a path to the paths searched for measures. */
  bool addMeasurePath(const openstudio::path& path);

  /** Clear paths searched for measures. */
  void resetMeasurePaths();

  /** Attempts to find a measure by path.
  *   If measureDir is relative, searches through measurePaths in order and returns first match that exists. 
  *   If measureDir is absolute, does not search for measureDir in measurePaths, only returns true it measureDir exists.
  */
  boost::optional<openstudio::path> findMeasure(const openstudio::path& measureDir) const;
  boost::optional<openstudio::path> findMeasure(const std::string& measureDirName) const;

  /** Returns the seed file path. Evaluated relative to filePaths if not absolute. */
  boost::optional<openstudio::path> seedFile() const;

  /** Sets the seed file path. */
  bool setSeedFile(const openstudio::path& seedFile);
    
  /** Resets the seed file path. */
  void resetSeedFile();

  /** Returns the weather file path. Evaluated relative to filePaths if not absolute. */
  boost::optional<openstudio::path> weatherFile() const;
    
  /** Sets the weather file path. */
  bool setWeatherFile(const openstudio::path& weatherFile);
    
  /** Resets the weather file path. */
  void resetWeatherFile();

  /** Returns the workflow steps. */
  std::vector<WorkflowStep> workflowSteps() const;

  /** Assigns the workflow steps. */
  bool setWorkflowSteps(const std::vector<WorkflowStep>& steps);

  /** Resets the workflow steps. */
  void resetWorkflowSteps();

  /** Gets measure steps by measure type. */
  std::vector<MeasureStep> getMeasureSteps(const MeasureType& measureType) const;

  /** Sets measure steps of a given type. Does not change other measure steps. */
  bool setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps);

  /** Attempts to find the BCLMeasure for a given MeasureStep. */
  boost::optional<BCLMeasure> getBCLMeasure(const MeasureStep& step) const;

  /** Checks BCLMeasure for each MeasureStep attempting to find by id, does not check measures that are not in the workflow. */
  boost::optional<BCLMeasure> getBCLMeasureByUUID(const UUID& id) const;

  /** Add a measure to the measure dir, replaces existing measure with same id, does not add a step to the workflow. */
  boost::optional<BCLMeasure> addMeasure(const BCLMeasure& bclMeasure);

  /** Get RunOptions for this workflow. */
  boost::optional<RunOptions> runOptions() const;

  /** Set RunOptions for this workflow. */
  bool setRunOptions(const RunOptions& options);

  /** Reset RunOptions for this workflow. */
  void resetRunOptions();

protected:
  
  // get the impl
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }
  
  friend class OSDocument;
  friend class detail::WorkflowJSON_Impl;
  friend class ::Filetypes_WorkflowJSON_Signals_Test; // for testing

  /** Protected constructor from impl. */
  WorkflowJSON(std::shared_ptr<detail::WorkflowJSON_Impl> impl);
  
private:

  // configure logging
  REGISTER_LOGGER("openstudio.WorkflowJSON");

  // pointer to implementation
  std::shared_ptr<detail::WorkflowJSON_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const WorkflowJSON& workflowJSON);

} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWJSON_HPP
