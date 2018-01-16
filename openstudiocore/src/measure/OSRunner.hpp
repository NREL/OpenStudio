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

#ifndef MEASURE_OSRUNNER_HPP
#define MEASURE_OSRUNNER_HPP

#include "MeasureAPI.hpp"

#include "../model/Model.hpp"

#include "../utilities/filetypes/WorkflowStepResult.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {

class Workspace;
class WorkspaceObject;

namespace model {
  class ModelObject;
}

namespace measure {

class OSArgument;
class OSMeasure;

/** OSRunner is a concrete base class for application-specific classes that run \link OSMeasure
 *  OSMeasures\endlink. */
class MEASURE_API OSRunner {
 public:
  /** @name Constructors and Destructors */
  //@{

  OSRunner(const WorkflowJSON& workflow);

  virtual ~OSRunner();

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns a clone of the workflow currently being run. New in OS 2.0. */
  WorkflowJSON workflow() const;

  /** Returns preferred unit system, either 'IP' or 'SI'. New in OS 2.0. */
  std::string unitsPreference() const;

  /** Returns preferred language, e.g. 'en' or 'fr'. New in OS 2.0. */
  std::string languagePreference() const;

  /** Returns the result for the current/last OSMeasure run by this OSRunner. (prepareForMeasureRun
   *  should be called prior to each run to ensure that result() corresponds to a single script, and
   *  is not instead a running result over multiple scripts. One way to ensure that this happens is
   *  to call the default version of run in ModelMeasure, etc. at the beginning of any particular
   *  run method.) */
  WorkflowStepResult result() const;

  /** Returns true if the simulation has been halted. */
  bool halted() const;

  /** Returns a copy of the last Model generated in the workflow if available. */
  boost::optional<openstudio::model::Model> lastOpenStudioModel() const;

  /** Returns a copy of the last EnergyPlus Workspace generated in the workflow if available. */
  boost::optional<openstudio::Workspace> lastEnergyPlusWorkspace() const;

  /** Returns a copy of the last EnergyPlus SqlFile generated in the workflow if available. */
  boost::optional<openstudio::SqlFile> lastEnergyPlusSqlFile() const;

  /** Returns a copy of the last EpwFile generated in the workflow if available. */
  boost::optional<openstudio::EpwFile> lastEpwFile() const;

  /** Returns a path to the last EpwFile generated in the workflow if available. */
  boost::optional<openstudio::path> lastEpwFilePath() const;

  /** Tests if the given ModelObject is in the application's current selection. Base class
   *  implementation always returns true. */
  virtual bool inSelection(const openstudio::model::ModelObject& modelObject) const;

  //@}
  /** @name Actions */
  //@{

  /** Gets user input using dialog boxes, etc. Result is map of name to argument. Result is empty
   *  if user cancels or user did not provide values for all required arguments. Base class
   *  implementation simply creates a map, with no information added, from arguments. */
  virtual std::map<std::string, OSArgument> getUserInput(std::vector<OSArgument>& arguments) const;

  /** Initializes OSRunner for capturing data on a particular measure. Replaces result
   *  with a default-constructed result, and keeps the measure name for use in log messages.
   *  Should be called at the beginning of every OSMeasure. Is called by the OSMeasure C++
   *  classes' run methods. */
  virtual void prepareForMeasureRun(const OSMeasure& measure);

  /** Registers error message with result, and sets result value to "Fail". OSMeasures
   *  should return false after calling this method. Workflow completed status will be set to "Fail".*/
  virtual void registerError(const std::string& message);

  /** Registers warning message with result. Base class returns true. Derived classes
   *  may choose to present a prompt to the user. The OSMeasure should exit (return false) if
   *  false is returned. */
  virtual bool registerWarning(const std::string& message);

  /** Registers info message with result. Base class returns true. Derived classes
   *  may choose to present a prompt to the user. The OSMeasure should exit (return false) if
   *  false is returned. */
  virtual bool registerInfo(const std::string& message);

  /** Sets result value to "NA", and registers a corresponding info message. In most circumstances,
   *  OSMeasures should return true after calling this method. Does not affect workflow completed status.*/
  virtual void registerAsNotApplicable(const std::string& message);

  /** Sets the result initial condition to message. */
  virtual void registerInitialCondition(const std::string& message);

  /** Sets the result final condition to message. */
  virtual void registerFinalCondition(const std::string& message);

  /** \overload Shortcut method for registering boolean attribute. */
  virtual void registerValue(const std::string& name, bool value);
  /** \overload Shortcut method for registering boolean attribute. */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             bool value);

  /** \overload Shortcut method for registering double attribute. */
  virtual void registerValue(const std::string& name, double value);
  /** \overload Shortcut method for registering double attribute. */
  virtual void registerValue(const std::string& name, double value, const std::string& units);
  /** \overload Shortcut method for registering double attribute. */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             double value);
  /** \overload Shortcut method for registering double attribute. */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             double value,
                             const std::string& units);

  /** \overload Shortcut method for registering int attribute */
  virtual void registerValue(const std::string& name, int value);
  /** \overload Shortcut method for registering int attribute */
  virtual void registerValue(const std::string& name, int value, const std::string& units);
  /** \overload Shortcut method for registering int attribute */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             int value);
  /** \overload Shortcut method for registering int attribute */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             int value,
                             const std::string& units);

  /** \overload Shortcut method for registering string attribute */
  virtual void registerValue(const std::string& name, const std::string& value);
  /** \overload Shortcut method for registering string attribute */
  virtual void registerValue(const std::string& name,
                             const std::string& displayName,
                             const std::string& value);

  /** Halts the simulation with the provided completed status, does not set the current measure's step result.
   *  Measure writers can call this with "Success" if all required results have been generated.
   *  Measure writers should not call this with "Fail", runner.registerError should be used instead.
   *  Measure writers can call this with "Invalid" if the workflow requests parameter combinations that are not valid.
   *  Measure writers should not call this with "Cancel", this is reserved for external cancel operations.
   */
  virtual void haltWorkflow(const std::string& completedStatus = "Invalid");

  /** Creates a progress bar with the text label. Base class implementation does nothing. */
  virtual void createProgressBar(const std::string& text) const;

  /** Updates the progress bar to value (between 0 and 100). Base class implementation does
   *  nothing. */
  virtual void updateProgress(const double& value) const;

  /** Destroys the progress bar. Base class implementation does nothing. */
  virtual void destroyProgressBar() const;

  //@}
  /** @name Common Error Checking Functions */
  //@{

  /** Returns true, logs no messages, and registers a value
   *  for each argument with a value or default value if all script_arguments are in
   *  user_arguments, and if all required script_arguments have been set or have defaults
   *  in user_arguments. Otherwise, returns false and \link registerError registers an
   *  error\endlink if there are any type mismatches, or if any required or defaulted
   *  arguments are missing (entirely, or their values are not set). All other discrepancies
   *  are \link registerWarning logged as warnings\endlink. */
  bool validateUserArguments(const std::vector<OSArgument>& script_arguments,
                             const std::map<std::string, OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type boolean that is either
   *  required or has a default. */
  bool getBoolArgumentValue(const std::string& argument_name,
                            const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type double that is either
   *  required or has a default. */
  double getDoubleArgumentValue(const std::string& argument_name,
                                const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type double that is optional
   *  (not required and does not have a default). */
  boost::optional<double> getOptionalDoubleArgumentValue(
      const std::string& argument_name,
      const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type integer that is either
   *  required or has a default. */
  int getIntegerArgumentValue(const std::string& argument_name,
                              const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type integer that is optional
   *  (not required and does not have a default). */
  boost::optional<int> getOptionalIntegerArgumentValue(
      const std::string& argument_name,
      const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type string, choice, or path
   *  that is either required or has a default. */
  std::string getStringArgumentValue(const std::string& argument_name,
                                     const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type string, choice, or path
   *  that is optional (not required and does not have a default). */
  boost::optional<std::string> getOptionalStringArgumentValue(
      const std::string& argument_name,
      const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type path that is either
   *  required or has a default. */
  openstudio::path getPathArgumentValue(const std::string& argument_name,
                                        const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument of type path that is optional
   *  (not required and does not have a default). */
  boost::optional<openstudio::path> getOptionalPathArgumentValue(
      const std::string& argument_name,
      const std::map<std::string,OSArgument>& user_arguments);

  /** Call this method to retrieve the value of an OSArgument that was created by
   *  makeChoiceArgumentOfWorkspaceObjects. */
  boost::optional<openstudio::WorkspaceObject> getOptionalWorkspaceObjectChoiceValue(
      const std::string& argument_name,
      const std::map<std::string,OSArgument>& user_arguments,
      const openstudio::Workspace& workspace);

  //@}

  // reset the runner to re-run the workflow
  void reset();

  // increments step to run next, returns true if there is another step
  // should be called immediately after the last step completes
  bool incrementStep();

  // supports in-memory job chaining
  void setLastOpenStudioModel(const openstudio::model::Model& lastOpenStudioModel);
  void resetLastOpenStudioModel();

  // clears m_lastOpenStudioModel
  void setLastOpenStudioModelPath(const openstudio::path& lastOpenStudioModelPath);
  void resetLastOpenStudioModelPath();

  // supports in-memory job chaining
  void setLastEnergyPlusWorkspace(const openstudio::Workspace& lastEnergyPlusWorkspace);
  void resetLastEnergyPlusWorkspace();

  // clears m_lastEnergyPlusWorkspace
  void setLastEnergyPlusWorkspacePath(const openstudio::path& lastEnergyPlusWorkspacePath);
  void resetLastEnergyPlusWorkspacePath();

  // clears m_lastEnergyPlusSqlFile
  void setLastEnergyPlusSqlFilePath(const openstudio::path& lastEnergyPlusSqlFilePath);
  void resetLastEnergyPlusSqlFilePath();

  // clears m_lastEpwFilePath
  void setLastEpwFilePath(const openstudio::path& lastEpwFilePath);
  void resetLastEpwFilePath();

  /** Sets preferred unit system, either 'IP' or 'SI'. New in OS 2.0. */
  bool setUnitsPreference(const std::string& unitsPreference);

  void resetUnitsPreference();

  /** Sets preferred language, e.g. 'en' or 'fr'. New in OS 2.0. */
  bool setLanguagePreference(const std::string& languagePreference);

  void resetLanguagePreference();

 private:
  REGISTER_LOGGER("openstudio.measure.OSRunner");

  std::string cleanValueName(const std::string& name) const;

  void captureStreams();
  void restoreStreams();

  WorkflowJSON m_workflow;
  bool m_startedStep;
  bool m_streamsCaptured;
  std::string m_unitsPreference;
  std::string m_languagePreference;

  bool m_halted;

  // current data
  WorkflowStepResult m_result;

  mutable boost::optional<openstudio::model::Model> m_lastOpenStudioModel;
  boost::optional<openstudio::path> m_lastOpenStudioModelPath;
  mutable boost::optional<openstudio::Workspace> m_lastEnergyPlusWorkspace;
  boost::optional<openstudio::path> m_lastEnergyPlusWorkspacePath;
  mutable boost::optional<openstudio::SqlFile> m_lastEnergyPlusSqlFile;
  boost::optional<openstudio::path> m_lastEnergyPlusSqlFilePath;
  mutable boost::optional<openstudio::EpwFile> m_lastEpwFile;
  boost::optional<openstudio::path> m_lastEpwFilePath;

  std::streambuf* m_originalStdOut;
  std::streambuf* m_originalStdErr;
  std::stringstream m_bufferStdOut;
  std::stringstream m_bufferStdErr;

  boost::optional<openstudio::path> m_currentDir;
  std::set<openstudio::path> m_currentDirFiles;
};

} // measure
} // openstudio

#endif // MEASURE_OSRUNNER_HPP
