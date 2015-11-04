namespace openstudio {
namespace analysisdriver {

/** \mainpage OpenStudio Analysis Driver
 *
 *  \section quick_links_analysisdriver Quick Links
 *
 *  \li AnalysisDriver
 *  \li AnalysisRunOptions
 *  \li CurrentAnalysis
 *  \li SimpleProject - Implements best practice regarding managing the file system in the case
 *  of running and storing a single analysis per ProjectDatabase. Also provides BCLMeasure
 *  management features, and is the basis for Parametric Analysis Tool (PAT) projects. 
 *
 *  \section overview_analysisdriver Overview
 *
 *  AnalysisDriver runs analyses (analysis::Analysis objects) using a runmanager::RunManager, 
 *  saving the results to a project::ProjectDatabase for future viewing, query, and retrieval. 
 *  Basic usage involves constructing or opening a project::ProjectDatabase (which itself 
 *  requires a RunManager), and then constructing an AnalysisDriver object. At this point, one
 *  or more analysis::Analysis objects may be run. The Analysis may be constructed just before
 *  constructing the AnalysisDriver, or it may have been constructed previously and saved in 
 *  (and subsequently retrieved from) a ProjectDatabase. Behaviorally, AnalysisDriver:
 *
 *  \li Uses the RunManager to run DataPoints already in the list Analysis::dataPointsToQueue
 *      first. This list will be non-empty if the user directly calls Analysis::addDataPoint 
 *      or Analysis::algorithm().get().createNextIteration(analysis) before submitting the 
 *      analysis to the driver.
 *  \li If the Analysis has an analysis::OpenStudioAlgorithm, AnalysisDriver will call 
 *      OpenStudioAlgorithm::createNextIteration and queue up any new DataPoints created by 
 *      that operation.
 *  \li If the Analysis has an analysis::DakotaAlgorithm, AnalysisDriver will queue a 
 *      runmanager::DakotaJob (first ensuring that RunManager allows at least two local jobs), 
 *      and then watch the DakotaJob run directory for new files. When a DAKOTA parameters file 
 *      is dropped, AnalysisDriver manages the process of loading the file, creating a new 
 *      DataPoint or finding the appropriate existing one, and queuing that DataPoint's Workflow
 *      and/or writing out the corresponding DAKOTA results file as appropriate.
 *  \li AnalysisDriver can run multiple Analyses at a time.
 *  \li AnalysisDriver will automatically restart an Analysis where it left off if it is already
 *      in the underlying database. For instance, consider the following code modified slightly 
 *      (for conciseness) from one of our regression tests:
 *
 *  \code
 *  
 *  // SET UP PROJECT
 *  SimpleProject project = getCleanSimpleProject("SequentialSearch_IdfOnly");
 *  Analysis analysis = project.analysis();
 *  
 *  // SET UP PROBLEM
 *  // omitted
 *  
 *  // DEFINE ALGORITHM, INCLUDING OPTIONS
 *  SequentialSearchOptions options(1);
 *  options.setMaxIter(2); // just run baseline and first iteration
 *  SequentialSearch sequentialSearch(options);
 *  analysis.setAlgorithm(sequentialSearch); // analysis constructed above
 *  
 *  // DEFINE SEED
 *  // omitted
 *  
 *  // RUN ANALYSIS
 *  AnalysisDriver analysisDriver = project.analysisDriver();
 *  AnalysisRunOptions runOptions(analysisDriver.database().path().parent_path(),
 *                                toPath(rubyOpenStudioDir()));
 *  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
 *  EXPECT_TRUE(analysisDriver.waitForFinished());
 *  
 *  // EXAMINE RESULTS
 *  table = sequentialSearch.getSummaryTable(analysis);
 *  table.save(project.projectDir() / toPath("SequentialSearchIdfOnly_TwoIterations.csv"),true);
 *  // save DataPoint tags created by getSummaryTable call
 *  project.save();
 *  
 *  // RUN TO COMPLETION
 *  sequentialSearch.sequentialSearchOptions().clearMaxIter();
 *  runOptions = AnalysisRunOptions(analysisDriver.database().path().parent_path(),
 *                                  toPath(rubyOpenStudioDir()));
 *  currentAnalysis = analysisDriver.run(analysis,runOptions);
 *  EXPECT_TRUE(analysisDriver.waitForFinished());
 *  
 *  // EXAMINE RESULTS
 *  table = sequentialSearch.getSummaryTable(analysis);
 *  table.save(project.projectDir() / toPath("SequentialSearchIdfOnly.csv"),true);
 *  // save DataPoint tags created by getSummaryTable call
 *  project.save();
 *  
 *  \endcode
 *
 *  During the second call to analysisDriver.run, the first two iterations are not re-run, as those
 *  DataPoints already exist and are marked as complete in the analysis::Analysis and 
 *  project::AnalysisRecord objects.
 *
 *  DAKOTA restarts in a similar manner, and that restart process has been coordinated with ours
 *  so that DAKOTA algorithm-driven analyses are also restartable. (If the DAKOTA algorithm is
 *  stochastic, it may be necessary for a pseudo-random number generator seed to be explicitly
 *  set for this feature to work properly. The analysis sub-project therefore explicitly sets
 *  such seed values by default. The seed values can be explicitly cleared if so desired.)
 *
 */

} // analysisdriver
} // openstudio
