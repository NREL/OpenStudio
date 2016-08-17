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

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../bcl/BCLMeasure.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

namespace openstudio{

class WorkflowStep;
class MeasureStep;
class DateTime;

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

  /** Saves this file to a new location. */
  bool saveAs(const openstudio::path& p) const;

  /** Reset to re-run the workflow, does not delete steps. */
  void reset();

  /** Sets the started at time. */
  void start();

  /** Get the current step index. */
  unsigned currentStepIndex() const;

  /** Get the current step. */
  boost::optional<WorkflowStep> currentStep() const;

  /** Increments current step, returns true if there is another step. */
  bool incrementStep();

  /** Returns the completion status, "Success" or "Fail". */
  boost::optional<std::string> completedStatus() const;

  /** Sets the completion status, "Success" or "Fail". */
  void setCompletedStatus(const std::string& status);

  /** Returns the time this WorkflowJSON was created at. */
  boost::optional<DateTime> createdAt() const;

  /** Returns the time this WorkflowJSON was started at. */
  boost::optional<DateTime> startedAt() const;

  /** Returns the time this WorkflowJSON was updated at. */
  boost::optional<DateTime> updatedAt() const;

  /** Returns the time this WorkflowJSON was completed at. */
  boost::optional<DateTime> completedAt() const;

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

  /** Attempts to find a file by path.
  *   If file is relative, searches through filePaths in order and returns first match that exists. 
  *   If file is absolute, does not search for file in filePaths, only returns true it file exists.
  */
  boost::optional<openstudio::path> findFile(const openstudio::path& file) const;
  boost::optional<openstudio::path> findFile(const std::string& fileName) const;

  /** Returns the paths that will be searched in order for measures, default value is './measures/'. Evaluated relative to rootDir if not absolute. */
  std::vector<openstudio::path> measurePaths() const;
  std::vector<openstudio::path> absoluteMeasurePaths() const;

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

  /** Gets measure steps by measure type. */
  std::vector<MeasureStep> getMeasureSteps(const MeasureType& measureType);

  /** Sets measure steps of a given type. Does not change other measure steps. */
  bool setMeasureSteps(const MeasureType& measureType, const std::vector<MeasureStep>& steps);

  /** Attempts to find the BCLMeasure for a given MeasureStep. */
  boost::optional<BCLMeasure> getBCLMeasure(const MeasureStep& step);

protected:
  
  // get the impl
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }
  
  friend class detail::WorkflowJSON_Impl;

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
