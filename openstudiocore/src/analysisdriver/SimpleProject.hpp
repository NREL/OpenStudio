/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSISDRIVER_SIMPLEPROJECT_HPP
#define ANALYSISDRIVER_SIMPLEPROJECT_HPP

#include <analysisdriver/AnalysisDriverAPI.hpp>
#include <analysisdriver/AnalysisDriver.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>

#include <QObject>

namespace openstudio {

class BCLMeasure;
class CloudSession;
class CloudSettings;
class FileReference;
class ProgressBar;
class Workspace;

namespace model {
  class Model;
}

namespace ruleset {
  class OSArgument;
}

namespace analysis {
  class Analysis;
  class DataPoint;
  class MeasureGroup;
  class WorkflowStep;
}

namespace runmanager {
  class RunManager;
}

namespace project {
  class ProjectDatabase;
  class AnalysisRecord;
}

namespace analysisdriver {

namespace detail {
  class SimpleProject_Impl;
}

/** Options class for opening or creating a SimpleProject. */
class ANALYSISDRIVER_API SimpleProjectOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  SimpleProjectOptions();

  //@}
  /** @name Getters */
  //@{

  /** Defaults to Warn and applies to the project.log file in a SimpleProject::projectDir(). */
  LogLevel logLevel() const;

  /** Defaults to false. */
  bool initializeRunManagerUI() const;

  /** Defaults to false, in which case, the RunManager queue is unpaused by the end of
   *  SimpleProject::open or SimpleProject::create. */
  bool pauseRunManagerQueue() const;

  //@}
  /** @name Setters */
  //@{

  void setLogLevel(LogLevel logLevel);

  void setInitializeRunManagerUI(bool value);

  void setPauseRunManagerQueue(bool value);

  //@}
 private:
  LogLevel m_logLevel;
  bool m_initializeRunManagerUI;
  bool m_pauseRunManagerQueue;
};

/** SimpleProject provides an interface for working with the project structure used for the 
 *  Parametric Analysis Tool (PAT). It is also appropriate for many other analysis situations. 
 *  \link SimpleProject SimpleProjects\endlink are fully contained in a single directory, in which 
 *  there is a ProjectDatabase and a RunManager database, and which also serves as the RunManager 
 *  run directory for the lone analysis stored in project.osp. */
class ANALYSISDRIVER_API SimpleProject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SimpleProject() {};

  /** Open an existing SimpleProject, fails if cant find osp or if has 0 or > 1 analyses. 
   *  Purposefully does not deserialize the analysis::Analysis from project::AnalysisRecord. */
  static boost::optional<SimpleProject> open(
      const openstudio::path& projectDir,
      const SimpleProjectOptions& options = SimpleProjectOptions());

  /** Create a new SimpleProject, fails if directory exists and is not empty or cannot be 
   *  created. */
  static boost::optional<SimpleProject> create(
      const openstudio::path& projectDir,
      const SimpleProjectOptions& options = SimpleProjectOptions(),
      bool ignoreExistingFiles = false);

  //@}
  /** @name Getters and Queries */
  //@{

  /** Directory containing this SimpleProject. */
  openstudio::path projectDir() const;

  /** Returns true if the analysis is already being held in memory. */
  bool analysisIsLoaded() const;

  /** Returns the AnalysisRecord in projectDatabase corresponding to analysis(). Always re-
   *  retrieves from the database. */
  project::AnalysisRecord analysisRecord() const;

  /** Returns this SimpleProject's analysis::Analysis, of which there is always exactly one. Will
   *  deserialize the analysis for the first time if not analysisIsLoaded(). */
  analysis::Analysis analysis() const;

  /** Returns file reference to seed model. */
  FileReference seed() const;

  /** Returns the analysis seed model, if it is positively specified, can be located, and is an 
   *  OpenStudio Model (not an EnergyPlus IDF). */
  boost::optional<model::Model> seedModel(ProgressBar* progressBar = NULL) const;

  /** Returns the analysis seed if it is an IDF, or the seed model translated to IDF
   *  otherwise. */
  boost::optional<Workspace> seedIdf(ProgressBar* progressBar = NULL) const;

  /** Returns true if the seed model or any alternate models need version translation, or if
   *  the analysis weather file cannot be located. */
  bool modelsRequireUpdate() const;

  /** Returns the BCLMeasures in this project's scripts directory. */
  std::vector<BCLMeasure> measures() const;

  /** Returns the BCLMeasure in this project's scripts directory with uuid, if it exists. */
  boost::optional<BCLMeasure> getMeasureByUUID(const UUID& uuid) const;

  bool hasStoredArguments(const BCLMeasure& measure) const;

  std::vector<ruleset::OSArgument> getStoredArguments(const BCLMeasure& measure) const;

  /** Returns this SimpleProject's AnalysisDriver. Can be used to run analysis(). */
  AnalysisDriver analysisDriver() const;

  /** Returns true if the analysis() is being run by analysisDriver(). */
  bool isRunning() const;

  boost::optional<CloudSession> cloudSession() const;

  boost::optional<CloudSettings> cloudSettings() const;

  project::ProjectDatabase projectDatabase() const;

  runmanager::RunManager runManager() const;

  //@}
  /** @name PAT-Specific Getters and Queries */
  //@{

  /** Returns true if this project only contains structures that can be displayed in PAT. */
  bool isPATProject() const;

  /** Returns the analysis::MeasureGroup in analysis().problem() that implements the alternate
   *  model option in PAT, if it exists. The variable is identified by its name and (exclusive) 
   *  use of a specific BCLMeasure (identified by UUID). */
  boost::optional<analysis::MeasureGroup> getAlternativeModelVariable() const;

  //@}
  /** @name Actions */
  //@{
  
  /** Attempts to copy the OSM or IDF model at currentSeedLocation into this project's
   *  directory structure. Returns false if currentSeedLocation does not point to an
   *  OSM or IDF, or cannot be located. IDF files are copied over directly; OSM files
   *  are also version translated, bring over a subset of their auxillary files, and have
   *  their weather files hard-assigned to the analysis. 
   *
   *  Since OpenStudio Models may also have measures attached, such measures are imported
   *  into the project and placed in the analysis().problem().workflow(). OpenStudio 
   *  measures are placed after the alternative model variable, if it exists. Otherwise,
   *  they are placed at the top of the workflow. EnergyPlus measures are placed after the
   *  ExpandObjects job, if it exists. Otherwise, they are placed after ModelToIdf.
   *
   *  In importing measures, if there are no duplicates between this project and the new 
   *  seed model, then the new measure and its arguments are simply copied over and used
   *  as-is. (This assumes that the seed model is internally consistent.) If there are 
   *  duplicates, we initially set up the imported measure to use the project's copy of 
   *  the measure but the seed model's arguments. The new measure path is then passed out
   *  so the user application can, if desired, update the measure to the version used by
   *  the seed.
   *
   *  At this time, we do not attempt to delete any measures brought in with earlier seed
   *  models. The user is responsible for deleting any such measures that are no longer
   *  needed/desired. */
  std::pair<bool,std::vector<BCLMeasure> > setSeed(const FileReference& currentSeedLocation, 
                                                   ProgressBar* progressBar = NULL);

  /** Looks for measure by UUID, returning this project's copy if found, adding and returning
   *  the new project-specific copy otherwise. */
  BCLMeasure insertMeasure(const BCLMeasure& measure);

  /** Registers measure's arguments by measure UUID and returns true if measure is in this 
   *  project. Otherwise, returns false. */
  bool registerArguments(const BCLMeasure& measure, const std::vector<ruleset::OSArgument>& arguments);

  /** First looks to see if measure is used in this project. If it is, compares current and
   *  new version UUIDs. (The caller is responsible for calling measure.checkForUpdates() and 
   *  measure.save() before calling this method.) If they are different, the new version is 
   *  copied over, any analysis objects that use it are asked to update themselves accordingly, 
   *  and true is returned. Otherwise, no changes are made and the method returns false.
   *
   *  Note that if false is returned, the caller may want to instead call insertMeasure and then
   *  registerArguments.
   *
   *  As a pre-condition, the caller should extract arguments from measure using an appropriate
   *  method. C++ applications can use the embedded Ruby interpreter (see 
   *  ruleset/test/EmbeddedRuby_GTest.cpp and analysisdriver/test/SimpleProject_GTest.cpp 
   *  SimpleProject_*Measure for examples). Ruby applications can use the helper function
   *  \code
   *  arguments = OpenStudio::Ruleset::getArguments(measure,project.seedModel,project.seedIdf)
   *  \endcode */
  bool updateMeasure(const BCLMeasure& measure, const std::vector<ruleset::OSArgument>& arguments);

  /** Version translates seed model and alternate models; looks for seed model weather file and
   *  explicitly sets on analysis. Returns true if all operations were successful. */
  bool updateModels();

  /** Copies seed model and BCLMeasures into the project structure as necessary and possible in 
   *  an attempt to make this project self-contained. Returns true if the end state is clean (all 
   *  files copied over into project structure). Returns false otherwise. Versions of measures 
   *  (identified by UUID) already in the project are given precedence. */
  bool makeSelfContained();

  /** Stops the analysis if it is running */
  void stop();

  /** Stops the analysis if it is running and clears all results. Clears results from in-memory
   *  objects, removes DataPoint jobs from RunManager database, and removes result/run folders from 
   *  file system. Does not save to ProjectDatabase. Returns false if operation is incomplete (if 
   *  not all files can be removed from the file system). */
  bool clearAllResults();

  /** Stops the analysis if it is running and removes all data points (including their results).
   *  Removes in-memory DataPoint objects, removes DataPoint jobs from RunManager database, and
   *  removes result/run folders from file system. Does not save to ProjectDatabase. Returns false
   *  if operation is incomplete (if not all files can be removed from the file system). */
  bool removeAllDataPoints();

  /** Sets this project's CloudSession to session, which ensures that it will be stored in 
   *  projectDatabase() upon save(). */
  void setCloudSession(const CloudSession& session);

  void clearCloudSession();

  /** Sets this project's CloudSettings to settings, which ensures that it will be stored in 
   *  projectDatabase() upon save(). */
  void setCloudSettings(const CloudSettings& settings);

  void clearCloudSettings();

  /** Creates a zip file of the items needed to run individual DataPoints on a remote system, and
   *  returns the path to that (temporary) file. The file is deleted by SimpleProject's destructor. */
  openstudio::path zipFileForCloud() const;

  //@}
  /** @name PAT-Specific Actions */
  //@{

  /** Returns the baseline data point for the analysis, creates one if does not
   *  currently exist. */
  analysis::DataPoint baselineDataPoint() const;

  /** Creates DataPoint that swaps out seed model for altModel. Fails if
   *  analysis().dataPointsAreInvalid() or if !isPATProject() or if altModel is not an OSM or
   *  cannot be located. altModel is copied over into projectDir, as are its auxillary files, and
   *  the OSM is version translated if necessary. */
  bool addAlternateModel(const openstudio::path& altModel);

  /** Returns false if cannot insert the variable. Otherwise should succeed and 
   *  getAlternativeModelVariable() will return an initialized value after this method is called. */
  bool insertAlternativeModelVariable();

  //@}
  /** @name Serialization */
  //@{

  /** Saves this SimpleProject, that is, saves all in-memory objects to ProjectDatabase. */
  void save() const;

  /** Copies this SimpleProject to newProjectDir. Requirements on newProjectDir are same as
   *  those for create(projectDir). Returns false if the action is not successfully completed
   *  for any reason. CAUTION: This method should not be called while the underlying
   *  analysisDriver is running. */
  bool saveAs(const openstudio::path& newProjectDir) const;

  //@}
 protected:
  boost::shared_ptr<detail::SimpleProject_Impl> getImpl() const;

  /// @cond
  typedef detail::SimpleProject_Impl ImplType;

  explicit SimpleProject(boost::shared_ptr<detail::SimpleProject_Impl> impl);

  /// @endcond
 private:

  boost::shared_ptr<detail::SimpleProject_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysisdriver.SimpleProject");
};

/** \relates SimpleProject*/
typedef boost::optional<SimpleProject> OptionalSimpleProject;

/** \relates SimpleProject*/
typedef std::vector<SimpleProject> SimpleProjectVector;

/** Adds additional starting data to the result of SimpleProject::createProject. */
ANALYSISDRIVER_API boost::optional<SimpleProject> createPATProject(
    const openstudio::path& projectDir,
    const SimpleProjectOptions& options = SimpleProjectOptions());

/** Verifies and tries to recover, if necessary, the additional structure required by PAT. */
ANALYSISDRIVER_API boost::optional<SimpleProject> openPATProject(
    const openstudio::path& projectDir,
    const SimpleProjectOptions& options = SimpleProjectOptions());

/** Implementation of expected PAT save-as behavior. In particular, project remains unchanged; and 
 *  the returned SimpleProject is in newProjectDir, has the same in-memory data as project, and has 
 *  been saved. (The analysis data in the two projects is the same, project.analyss() may be dirty, 
 *  and the returned SimpleProject::analysis() is not dirty, that is, it has been saved to its 
 *  ProjectDatabase at newProjectDir / toPath("project.osp").)
 *  
 *  TODO: Evaluate deprecation of this in favor of refactoring SimpleProject::saveAs. */
ANALYSISDRIVER_API boost::optional<SimpleProject> saveAs(const SimpleProject& project,
                                                         const openstudio::path& newProjectDir);

/** Returns basic run options for project. */
ANALYSISDRIVER_API AnalysisRunOptions standardRunOptions(const SimpleProject& project);

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_SIMPLEPROJECT_HPP
