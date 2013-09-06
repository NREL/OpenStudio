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

#ifndef ANALYSISDRIVER_SIMPLEPROJECT_IMPL_HPP
#define ANALYSISDRIVER_SIMPLEPROJECT_IMPL_HPP

#include <analysisdriver/AnalysisDriverAPI.hpp>
#include <analysisdriver/AnalysisDriver.hpp>

#include <analysis/Analysis.hpp>

#include <ruleset/OSArgument.hpp>

#include <model/Model.hpp>

#include <utilities/idf/Workspace.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

#include <utilities/cloud/CloudProvider.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/FileLogSink.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>

#include <QObject>

namespace openstudio {

  class ProgressBar;

namespace analysis {
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

class SimpleProjectOptions;

namespace detail {

  /** SimpleProject_Impl is the implementation class for SimpleProject. */
  class ANALYSISDRIVER_API SimpleProject_Impl : public QObject {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Called by SimpleProject::open and SimpleProject::create. */
    SimpleProject_Impl(const openstudio::path& projectDir,
                       const AnalysisDriver& analysisDriver,
                       const boost::optional<analysis::Analysis>& analysis,
                       const SimpleProjectOptions& options);

    virtual ~SimpleProject_Impl();

    //@}
    /** @name Getters and Queries */
    //@{

    openstudio::path projectDir() const;

    /** Returns this SimpleProject's AnalysisDriver. Should be used to run analysis(). */
    AnalysisDriver analysisDriver() const;

    project::ProjectDatabase projectDatabase() const;

    runmanager::RunManager runManager() const;

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

    /** Returns the BCLMeasures in this project's scripts directory. */
    std::vector<BCLMeasure> measures() const;

    /** Returns the BCLMeasure in this project's scripts directory with uuid, if it exists. */
    boost::optional<BCLMeasure> getMeasureByUUID(const UUID& uuid) const;

    bool hasStoredArguments(const BCLMeasure& measure) const;

    std::vector<ruleset::OSArgument> getStoredArguments(const BCLMeasure& measure) const;

    /** Returns true if the analysis is already being held in memory. */
    bool analysisIsLoaded() const;

    /** Returns true if the analysis() is being run by analysisDriver(). */
    bool isRunning() const;

    boost::optional<CloudSession> cloudSession() const;

    boost::optional<CloudSettings> cloudSettings() const;

    /** Returns basic run options for this project. */
    AnalysisRunOptions standardRunOptions() const;

    /** Returns true if the seed model or any alternate models need version translation, or if
     *  the analysis weather file cannot be located. */
    bool modelsRequireUpdate() const;

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
   public slots:

    void onSeedChanged() const;

   private:
    REGISTER_LOGGER("openstudio.analysisdriver.SimpleProject");

    openstudio::path m_projectDir;
    analysisdriver::AnalysisDriver m_analysisDriver;
    mutable boost::optional<analysis::Analysis> m_analysis; // mutable for lazy load on open
    mutable boost::optional<CloudSession> m_cloudSession;   // mutable for lazy load on open
    mutable boost::optional<CloudSettings> m_cloudSettings; // mutable for lazy load on open
    mutable bool m_cloudSessionSettingsDirty; // don't lazy load if user has explicitly set
    FileLogSink m_logFile;

    mutable boost::optional<model::Model> m_seedModel; // clear optional when analysis's seed changes
    mutable boost::optional<Workspace> m_seedIdf;      // clear optional when analysis's seed changes
    mutable std::map<UUID,BCLMeasure> m_measures;
    mutable std::map<UUID,std::vector<ruleset::OSArgument> > m_measureArguments;
    mutable openstudio::path m_zipFileForCloud; // zip file always re-created, but reuse tempDir

    openstudio::path alternateModelsDir() const;

    openstudio::path scriptsDir() const;

    openstudio::path seedDir() const;

    /** Returns paths to all OSM files in alternateModelsDir(). */
    std::vector<openstudio::path> alternateModelPaths() const;

    /** Copy the OSM at modelPath into destinationDirectory. */
    bool copyModel(const openstudio::path& modelPath,
                   const openstudio::path& destinationDirectory,
                   bool minimal=false) const;

    /** Return true if the OSM at modelPath needs version translation. */
    bool requiresVersionTranslation(const openstudio::path& modelPath) const;

    /** Version translates the OSM at modelPath, and saves it back to disk. Returns true if model
     *  successfully translated and saved. */
    bool upgradeModel(const openstudio::path& modelPath, ProgressBar* progressBar = NULL);

    /** Look for the seedModel's weather file and set the analysis weather file to match if
     *  located. */
    bool setAnalysisWeatherFile(ProgressBar* progressBar = NULL);

    /** Import measures in the seed model workflow as fixed measures. */
    std::vector<BCLMeasure> importSeedModelMeasures(ProgressBar* progressBar = NULL);

    bool isPATFixedMeasure(const analysis::WorkflowStep& step) const;

    /** Removes any orphaned dataPointX or Dakota folders. Such folders can become orphaned if
     *  clearAllResults or removeAllDataPoints is called on Windows when one or more of the files
     *  in those folders are open. */
    void removeOrphanedResultFiles();

    /** Copies measure into this project's scripts directory and returns a BCLMeasure object
     *  pointing to the new copy. */
    BCLMeasure addMeasure(const BCLMeasure& measure);

    void removeMeasure(const BCLMeasure& measure);

    /** Copies measure into this project's scripts directory, overwriting the existing measure
     *  with the same UUID. */
    BCLMeasure overwriteMeasure(const BCLMeasure& measure);

    static openstudio::UUID alternativeModelMeasureUUID();
  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_SIMPLEPROJECT_IMPL_HPP
