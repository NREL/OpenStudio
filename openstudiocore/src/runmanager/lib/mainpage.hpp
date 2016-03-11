namespace openstudio {
/** \mainpage OpenStudio RunManager
 *
 * The RunManager library provides a set of tools for queuing and processing of work units called Jobs.
 * Jobs are either tasks that run an external process such as EnergyPlus or an internal process such as
 * ModelToIdf which converts an OSM file to an IDF file.
 *  
 * Jobs can have children, which are able to access the properties of the parents, and 
 * execute after the parents have completed. If desired, job trees can become very complicated with
 * multiple branches of child jobs and jobs configured to complete after all other children have completed.
 *
 * In this page we cover \ref RunManagerExecution, \ref RunManagerWorkflow, \ref RunManagerJobFactory, \ref RunManagerHybrid, 
 * and \ref RunManagerPlaceholderJobs. While the
 * latter ones are necessary for the first one to occur, we first show what Job execution means.
 *
 * Finally, we will cover \ref RunManagerTools which is necessary for telling RunManager where
 * the commonly used tools (such as EnergyPlus) are.
 *
 * \subpage RunManagerJobTypes "Detailed Descriptions of Job Types Available"
 * 
 * <hr>
 * \section RunManagerExecution Job Execution
 *
 * The RunManager processes its jobs asynchronously. The developer using the RunManager library may choose to set up
 * the jobs to execute, perform some other tasks, then wait for the jobs to complete. A RunManager object
 * manages its queue via database file, which you will see referenced in the examples below
 *
 * \code
 * openstudio::runmanager::RunManager rm(path("file.db"), true, true); // Create a RunManager with a new database file
 * rm.enqueue(functionThatCreatesJobs());      // Add some jobs to the runmanager
 * rm.enqueue(functionThatCreatesMoreJobs());  // Add some more jobs to the runmanager
 * rm.setPaused(false);                        // Unpause the runmanager so it begins processing of the jobs
 *
 * // perform foreground work.
 * doSomeWork(); // RunManager processes in the background while doSomeWork() runs in the foreground
 *
 * rm.waitForFinished(); // Block until the runamanger has completed running all jobs
 * \endcode
 *
 * Alternatively, the developer may choose to poll the \c workPending() method.
 *
 * \code
 * while(rm.workPending())
 * {
 *   //Do some other work
 * }
 * \endcode
 *  
 * <hr>
 * \section RunManagerWorkflow Creating Jobs with Workflow Object
 *
 * The primary method for creating jobs is with the Workflow class. Workflow provides repeatable shortcuts for 
 * generating job trees.
 *
 * A typical job process might be to convert an OSM input file to an IDF and process the IDF with
 * EnergyPlus.
 *
 * The Workflow class has a constructor which can parse simple job tree sequences.
 *
 * \code
 * openstudio::runmanager::RunManager rm("newfile.db", true, true);
 * openstudio::runmanager::Workflow wf("ModelToIdf->EnergyPlus");
 * rm.enqueue(wf.create("processingdir", "my.osm", "path/to/epws"))
 * rm.setPaused(false);
 * \endcode
 *
 * For more advanced workflow construction, the individual jobs may be added one at a time:
 *
 * \code
 * openstudio::runmanager::RunManager rm("newfile.db", true, true);
 * openstudio::runmanager::Workflow wf;
 * wf.addJob(openstudio::runmanager::JobType::ModelToIdf);
 * wf.addJob(openstudio::runmanager::JobType::EnergyPlus);
 * rm.enqueue(wf.create("processingdir", "my.osm", "path/to/epws"))
 * rm.setPaused(false);
 * \endcode
 *
 * The above procedure may seem verbose, but is useful in the case where Files, JobParams, or Tools need to be
 * provided for the job. More on that later.
 *
 * <hr>
 * \section RunManagerJobFactory Creating Jobs with JobFactory
 *
 * Workflows can only create jobs trees that are linear. In a \c parent->child->grandchild type of hierarchy with
 * no children that run in parallel. If you need a more complex layout of jobs, or to provide additional parameters
 * to jobs you will need to use the JobFactory.
 *
 * JobFactory has methods for creating each of the Job types. A factory method returns a job that is ready
 * to begin execution.
 *
 * \code
 * openstudio::runmanager::Job j 
 *   = openstudio::runmanager::JobFactory::createModelToIdfJob(openstudio::path("model.osm"), openstudio::path("path/to/outdir")a);
 * j.addChild(
 *   openstudio::runmanager::JobFactory::createEnergyPlusJob(openstudio::runmanager::Tools(),
 *      openstudio::runmanager::JobParams(),
 *      openstudio::runamanger::Files());
 * openstudio::runmanager::RunManager rm("pathto.db", true, true);
 * rm.enqueue(j);
 * rm.setPaused(false);
 * \endcode
 *
 * <hr>
 * \section RunManagerHybrid Creating Jobs with Hybrid Techniques 
 *
 * A third option is to use a combination of techniques to create the job tree you are interested in.
 * Workflow has the ability to create a workflow from an existing job and to append workflows together.
 *
 * \code
 * openstudio::runmanager::Job j
 *   = openstudio::runmanager::JobFactory::createModelObjectPerturbationJob(someRuleset);
 * openstudio::runmanager::Workflow wf(j); // create workflow from the new job we created
 * wf.addWorkflow(openstudio::runmanager::Workflow("ModelToIdf->EnergyPlus"));
 *
 * openstudio::runmanager::RunManager rm("newfile.db", true, true);
 * rm.enqueue(wf.create("processingdir", "my.osm", "path/to/epws"))
 * rm.setPaused(false);
 * \endcode
 *
 * <hr>
 * \section RunManagerPlaceholderJobs Using Placeholder Jobs
 *
 * The workflow class also supports creating jobs that have a tagname, which can be easily swapped out for
 * a new job or job tree. This can be useful if you want to create a reusable workflow.
 *
 * \code
 * openstudio::runmanager::Workflow wf;
 * wf.addJob(openstudio::runmanager::JobType::NullJob, "keyname");
 * wf.addWorkflow(openstudio::runmanager::Workflow("ModelToIdf->EnergyPlus"));
 * // we now have a workflow like: NullJob->ModelToIdf->EnergyPlus
 * // so we can swap out the NullJob with something more specific
 * wf.replaceJobs("keyname",openstudio::runmanager::JobFactory::createModelObjectPerturbationJob(someRuleset));
 * // and now we can create a job from the Workflow and execute it like the previous examples
 * \endcode
 * 
 * <hr>
 * \section RunManagerTools Searching for Tools
 *
 * None of the examples above will actually execute because we have failed to tell the system where the 
 * tools we need are located. Most notably, we need to find EnergyPlus.
 *
 * openstudio::runmanager::Tools provides a mechanism for collecting and passing around 
 * openstudio::runmanager::ToolInfo objects. ToolInfo objects contain information about tools 
 * (xmlpreprocessor, ruby, energyplus, radiance, plus their respective subtools). While ToolInfo objects
 * can be created by hand, it's much more convenient to use one of the provided tool finders.
 *
 * \code
 * ConfigOptions co;
 * co.findTools();
 * openstudio::runmanager::RunManager rm("newfile.db", true, true);
 * wf.add(co.getTools());
 * rm.enqueue(wf.create("processingdir", "my.osm", "path/to/epws"));
 * \endcode
 *
 * Unfortunately, the above procedure can be slow as it searches for all possible tools you have installed
 * on the system. If you do not required Radiance, XMLPreProcessor or Ruby, you can use the 
 * faster mechanism which only looks for EnergyPlus.
 *
 * \code
 * ConfigOptions co;
 * co.fastFindEnergyPlus();
 * openstudio::runmanager::RunManager rm("newfile.db", true, true);
 * wf.add(co.getTools());
 * rm.enqueue(wf.create("processingdir", "my.osm", "path/to/epws"));
 * \endcode
 *
 * <hr>
 *
 *
 * \sa openstudio::runmanager::RunManager for more information of queuing and monitoring jobs
 * \sa openstudio::runmanager::Job for more information on Jobs and job properties
 * \sa openstudio::runmanager::Workflow for more information on creating Jobs with the workflow object
 * \sa openstudio::runmanager::JobFactory for more information on creating Jobs to be processed with RunManager
 * \sa openstudio::runmanager::ConfigOptions for more information regarding the configuration
 * of the tool. 
 */


 /// \page RunManagerJobTypes Job Types
 ///
 /// \section CalculateEconomicsJobType CalculateEconomics Job
 /// Currently unimplemented job type. Throws assertions if it is used.
 ///
 /// \sa openstudio::runmanager::JobFactory::createCalculateEconomicsJob
 ///
 /// \section EnergyPlusJobType EnergyPlus Job
 /// Executes an EnergyPlus simulation using the last specified IDF file in the 
 /// list of input files. If an EPW is attached to the IDF file as a required file 
 /// then the EPW is used. If no EPW is attached, the Job attempts to open the IDF and determine
 /// an appropriate EPW to use.
 /// 
 /// The Job also opens the IDF to determine the version of EnergyPlus required to run the simulation.
 /// If the appropriate version has not been provided in the list of Tools, the job execution fails.
 ///
 /// If a "filename" param is passed to the Job, the value of the param is used to determine which 
 /// file to pass to the simulation.
 ///
 /// \sa openstudio::runmanager::JobFactory::createEnergyPlusJob
 ///
 /// \section EnergyPlusPostProcessJobType EnergyPlusPostProcess Job
 /// Calculates several metrics from the most recent EnergyPlusJob run (requires both the input file
 /// used and the resulting SQL file) and creates a report.xml of the calculation.
 ///
 /// \sa openstudio::runmanager::JobFactory::createEnergyPlusPostProcessJob
 ///
 /// \section OpenStudioPostProcessJobType OpenStudioPostProcess Job
 /// Calculates several metrics from the most recent EnergyPlusJob run from an OSM file
 /// (requires both the input file
 /// used and the resulting SQL file) and creates a report.xml of the calculation.
 ///
 /// \sa openstudio::runmanager::JobFactory::createOpenStudioPostProcessJob
 ///
 /// \section EnergyPlusPreProcessJobType EnergyPlusPreProcess Job
 /// Prepares an input file for simulation and EnergyPlusPostProcess job processing
 /// by enabling the SQLFile output.
 ///
 /// \sa openstudio::runmanager::JobFactory::createEnergyPlusPreProcessJob
 ///
 /// \section ModelToRadPreProcessJobType ModelToRadPreProcess Job
 /// Prepares an input file for simulation and ModelToRad job processing
 /// by enabling the lighting output needed for Radiance.
 ///
 /// \sa openstudio::runmanager::JobFactory::createModelToRadPreProcessJob
 ///
 /// \section ExpandObjectsJobType ExpandObjects Job
 /// Executes the EnergyPlus subtool "expandobjects" on the provided IDF input.
 ///
 /// \sa openstudio::runmanager::JobFactory::createExpandObjectsJob
 ///
 /// \section SlabJobType Slab Job
 /// Executes the EnergyPlus subtool "slab" on the provided IDF input. Must be run 
 /// after ExpandObjects job 
 ///
 /// \sa openstudio::runmanager::JobFactory::createSlabJob
 ///
 /// \section BasementJobType Basement Job
 /// Executes the EnergyPlus subtool "basement" on the provided IDF input. Must be run 
 /// after ExpandObjects job 
 ///
 /// \sa openstudio::runmanager::JobFactory::createBasementJob
 ///
 /// \section ModelObjectPerturbationJobType ModelObjectPerturbation Job
 /// Executes a perturbation of objects on the given input Model and generates a new model
 /// from the changes. If multiple ModelObjectPerturbation jobs are specified in a sequence, they can
 /// be combined for efficiency purposes using the JobFactory::optimizeJobTree method
 ///
 /// \sa openstudio::runmanager::JobFactory::createModelObjectPerturbationJob
 ///
 /// \section ModelToRadJobType ModelToRad Job
 /// Takes an OSM input file and SQL EnergyPlus simulation results file and generates a radiance
 /// input file
 ///
 /// \sa openstudio::runmanager::JobFactory::createModelToRadJob
 ///
 /// \section ModelToIdfJobType ModelToIdf Job
 /// Takes an OSM input file and generates an IDF output file as out.idf
 ///
 /// \sa openstudio::runmanager::JobFactory::createModelToIdfJob
 ///
 /// \section IdfModelJobType IdfToModel Job
 /// Takes an IDF input file and generates an OSM output file as out.osm
 ///
 /// \sa openstudio::runmanager::JobFactory::createIdfToModelJob
 ///
 /// \section NullJobType Null Job
 /// A NullJob does nothing. It can be used as a place holder, for testing, or for job tree organization.
 ///
 /// \sa openstudio::runmanager::JobFactory::createNullJob
 ///
 /// \section ParallelEnergyPlusSplitJobType ParallelEnergyPlusSplit Job
 ///
 /// Splits an IDF several IDF's for simulation, based on simulation period
 ///
 /// \sa openstudio::runmanager::JobFactory::createParallelEnergyPlusJobTree
 /// \sa openstudio::runmanager::JobFactory::createParallelEnergyPlusSplitJob
 /// \sa \ref ParallelEnergyPlusJoinJobType
 ///
 /// \section ParallelEnergyPlusJoinJobType ParallelEnergyPlusJoin Job
 ///
 /// Joins the eplusout.sql files from several EnergyPlus job runs into one compbined output
 ///
 /// \sa openstudio::runmanager::JobFactory::createParallelEnergyPlusJobTree
 /// \sa openstudio::runmanager::JobFactory::createParallelEnergyPlusJoinJob
 /// \sa \ref ParallelEnergyPlusSplitJobType
 ///
 /// \section PreviewIESJobType PreviewIES Job
 /// Executes a series of radiance commands to take an IES input file and generate a TIFF / BMP preview output.
 ///
 /// \sa openstudio::runmanager::JobFactory::createPreviewIESJob
 ///
 /// \section ReadVarsJobType ReadVars Job
 /// Executes the EnergyPlus subtool 'readvars'
 ///
 /// \sa openstudio::runmanager::JobFactory::createReadVarsJob
 ///
 /// \section RubyJobType Ruby Job
 /// Executes ruby, passing in the most recently provided rb file.
 ///
 /// \sa openstudio::runmanager::RubyJobBuilder
 /// \sa openstudio::runmanager::JobFactory::createRubyJob
 ///
 /// \section XMLPreprocessorJobType XMLPreprocessor Job
 /// Executes the xmlpreproc tool on the msot recently provided .xml file
 ///
 /// \sa openstudio::runmanager::JobFactory::createXMLPreprocessorJob
 ///
} // openstudio
