/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <gtest/gtest.h>
#include "AnalysisDriverFixture.hpp"

#include "../SimpleProject.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Algorithm.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DDACEAlgorithmOptions.hpp"
#include "../../analysis/DDACEAlgorithm.hpp"
#include "../../analysis/DesignOfExperimentsOptions.hpp"
#include "../../analysis/DesignOfExperiments.hpp"
#include "../../analysis/Measure.hpp"
#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"
#include "../../analysis/WorkflowStep.hpp"

#include "../../project/ProjectDatabase.hpp"
#include "../../project/AnalysisRecord.hpp"
#include "../../project/ProblemRecord.hpp"
#include "../../project/DataPointRecord.hpp"
#include "../../project/InputVariableRecord.hpp"
#include "../../project/DakotaAlgorithmRecord.hpp"
#include "../../project/DakotaAlgorithmRecord_Impl.hpp"
#include "../../project/FileReferenceRecord.hpp"
#include "../../project/WorkflowRecord.hpp"

#include "../../runmanager/lib/Job.hpp"
#include "../../runmanager/lib/RunManager.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include "../../runmanager/lib/Workflow.hpp"
#include "../../runmanager/lib/WorkItem.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/WeatherFile.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/cloud/VagrantProvider.hpp"
#include "../../utilities/cloud/VagrantProvider_Impl.hpp"
#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/core/UnzipFile.hpp"
#include "../../utilities/filetypes/EpwFile.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include <OpenStudio.hxx>
#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,SimpleProject_Create) {
  openstudio::path projectDir = toPath("AnalysisDriverFixtureData");
  if (!boost::filesystem::exists(projectDir)) {
    boost::filesystem::create_directory(projectDir);
  }
  projectDir = projectDir / toPath("NewProject");
  boost::filesystem::remove_all(projectDir);

  OptionalSimpleProject project = SimpleProject::create(projectDir);

  ASSERT_TRUE(project);

  EXPECT_TRUE(boost::filesystem::exists(projectDir));
  EXPECT_TRUE(boost::filesystem::is_directory(projectDir));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("project.osp")));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("run.db")));
  EXPECT_TRUE(boost::filesystem::exists(projectDir / toPath("project.log")));

  Analysis analysis = project->analysis();
  EXPECT_EQ(0,analysis.problem().numVariables());
  EXPECT_FALSE(analysis.algorithm());
  EXPECT_EQ(0u,analysis.dataPoints().size());

  AnalysisRecord analysisRecord = project->analysisRecord();
  EXPECT_EQ(0u,analysisRecord.problemRecord().inputVariableRecords().size());
  EXPECT_EQ(0u,analysisRecord.dataPointRecords().size());
}

TEST_F(AnalysisDriverFixture,SimpleProject_Open_IsNotPATProject) {
  openstudio::path projectDir;
  {
    // Create a non-PAT project
    // RETRIEVE PROBLEM
    Problem problem = retrieveProblem("Continuous",true,false);

    // DEFINE SEED
    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);

    // CREATE ANALYSIS
    DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
    algOptions.setSamples(1);
    Analysis analysis("Non-PAT Project",
                      problem,
                      DDACEAlgorithm(algOptions),
                      seedModel);

    // RUN ANALYSIS
    ProjectDatabase database = getCleanDatabase("Non-PAT Project");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());

    projectDir = analysisDriver.database().path().parent_path();
  }

  // Open project
  OptionalSimpleProject project = SimpleProject::open(projectDir);
  ASSERT_TRUE(project);
  EXPECT_FALSE(project->isPATProject());
}

TEST_F(AnalysisDriverFixture,PATProject_CreateOpenAndSaveAs) {
  openstudio::path projectDir;
  {
    // Create a PAT project
    SimpleProject patProject = getCleanPATProject("PATProject_CreateOpenAndSaveAs");

    // Populate it
    patProject.analysis().setName("PAT Project: Create, Open, and Save As");

    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    patProject.setSeed(seedModel);

    // (feature not in PAT, but not incompatible)
    DesignOfExperimentsOptions algOptions(DesignOfExperimentsType::FullFactorial);
    DesignOfExperiments algorithm(algOptions);
    patProject.analysis().setAlgorithm(algorithm);

    // Save it
    patProject.save();

    projectDir = patProject.projectDir();
  }

  // Open Project
  OptionalSimpleProject project = openPATProject(projectDir);
  ASSERT_TRUE(project);
  EXPECT_TRUE(project->isPATProject());

  // Save Project As
  openstudio::path newProjectDir = toPath("AnalysisDriverFixtureData") / toPath("PATProjectCreateOpenAndSaveAsCopy");
  boost::filesystem::remove_all(newProjectDir);
  ASSERT_TRUE(project->saveAs(newProjectDir));
  OptionalSimpleProject projectCopy = openPATProject(newProjectDir);
  ASSERT_TRUE(projectCopy);
  EXPECT_TRUE(project->isPATProject());
  EXPECT_TRUE(project->analysis().algorithm());
}

TEST_F(AnalysisDriverFixture,SimpleProject_InsertMeasure) {
  // create a new project
  SimpleProject project = getCleanSimpleProject("SimpleProject_InsertMeasure");

  // open a measure
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();

  // insert it into the project
  BCLMeasure projectMeasure = project.insertMeasure(measure);

  // verify that there is a project-specific copy
  EXPECT_NE(toString(measure.directory()),toString(projectMeasure.directory()));
  openstudio::path scriptsDir = project.projectDir() / toPath("scripts");
  EXPECT_EQ(toString(completeAndNormalize(scriptsDir)),
            toString(completeAndNormalize(projectMeasure.directory().parent_path())));
  EXPECT_EQ(measure.uuid(),projectMeasure.uuid());
  EXPECT_EQ(measure.versionUUID(),projectMeasure.versionUUID()); // DLM: should this be not equal?

  // verify that it can be retrieved from the project, but its arguments cannot
  ASSERT_TRUE(project.getMeasureByUUID(measure.uuid()));
  BCLMeasure temp = project.getMeasureByUUID(measure.uuid()).get();
  EXPECT_EQ(toString(projectMeasure.directory()),toString(temp.directory()));
  EXPECT_EQ(projectMeasure.uuid(),temp.uuid());
  EXPECT_EQ(projectMeasure.versionUUID(),temp.versionUUID());
  EXPECT_EQ(1u,project.measures().size());

  // use embedded Ruby to extract the arguments
  // (see AnalysisDriverFixture.cpp for interpreter set-up)
  OSArgumentVector args = argumentGetter->getArguments(projectMeasure,
                                                       project.seedModel(),
                                                       project.seedIdf());
  EXPECT_FALSE(args.empty());

  // register and subsequently retrieve those arguments
  project.registerArguments(measure,args); // doesn't matter which copy of the measure we use
  EXPECT_TRUE(project.hasStoredArguments(temp));
  OSArgumentVector argsCopy = project.getStoredArguments(projectMeasure);
  ASSERT_EQ(args.size(),argsCopy.size());
  for (int i = 0, n = args.size(); i < n; ++i) {
    EXPECT_EQ(args[i].name(),argsCopy[i].name());
    EXPECT_EQ(args[i].type(),argsCopy[i].type());
  }

  // use this measure to create a new variable/ruby measure
  // now it is important to use the copy in the project
  MeasureGroup dv("New Measure Group",MeasureVector());
  Problem problem = project.analysis().problem();
  problem.push(dv);
  // here, expect this test to pass.
  // in pat, if it fails, let user know and call problem.erase(dv).
  EXPECT_TRUE(problem.fileTypesAreCompatible(dv,
                                             projectMeasure.inputFileType(),
                                             projectMeasure.outputFileType()));
  EXPECT_TRUE(dv.push(RubyMeasure(projectMeasure)));
}

TEST_F(AnalysisDriverFixture,SimpleProject_UpdateMeasure) {
  // create a new project
  SimpleProject project = getCleanSimpleProject("SimpleProject_UpdateMeasure");
  Problem problem = project.analysis().problem();

  // insert a measure into the project, extract and register its arguments
  openstudio::path measuresDir = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresDir / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();
  BCLMeasure projectMeasure = project.insertMeasure(measure);
  OSArgumentVector args = argumentGetter->getArguments(projectMeasure,
                                                       project.seedModel(),
                                                       project.seedIdf());
  project.registerArguments(projectMeasure,args);
  EXPECT_EQ(1u,project.measures().size());

  // use the measure to create a new variable/ruby measure
  MeasureGroup dv("New Measure Group",MeasureVector());
  EXPECT_TRUE(problem.push(dv));
  RubyMeasure rp(projectMeasure);
  rp.setArguments(args);
  EXPECT_TRUE(dv.push(rp));
  EXPECT_EQ(args.size(),rp.arguments().size());
  EXPECT_TRUE(rp.hasIncompleteArguments());
  for (const OSArgument& arg : args) {
    if (arg.name() == "wwr") {
      OSArgument temp = arg.clone();
      temp.setValue(0.6);
      rp.setArgument(temp);
    }
    if (arg.name() == "sillHeight") {
      OSArgument temp = arg.clone();
      temp.setValue(1.0);
      rp.setArgument(temp);
    }
    if (arg.name() == "facade") {
      OSArgument temp = arg.clone();
      temp.setValue("South");
      rp.setArgument(temp);
    }
  }
  EXPECT_FALSE(rp.hasIncompleteArguments());

  openstudio::path tempDir = measuresDir / toPath(toString(createUUID()));
  {
    // create fake new version of the measure
    BCLMeasure newVersion = measure.clone(tempDir).get();
    newVersion.incrementVersionId();
    newVersion.save();
    OSArgumentVector newArgs = args;
    newArgs.push_back(OSArgument::makeDoubleArgument("frame_width"));

    // update the measure
    project.updateMeasure(newVersion,newArgs);

    // verify the final state of SimpleProject and RubyMeasure
    EXPECT_EQ(1u,project.measures().size());
    BCLMeasure retrievedMeasure = project.getMeasureByUUID(measure.uuid()).get();
    EXPECT_NE(measure.versionUUID(),retrievedMeasure.versionUUID());
    EXPECT_EQ(newVersion.versionUUID(),retrievedMeasure.versionUUID());
    ASSERT_TRUE(project.hasStoredArguments(retrievedMeasure));
    OSArgumentVector retrievedArgs = project.getStoredArguments(retrievedMeasure);
    EXPECT_EQ(args.size() + 1u,retrievedArgs.size());

    EXPECT_EQ(retrievedArgs.size(),rp.arguments().size());
    EXPECT_TRUE(rp.hasIncompleteArguments());
  }
  boost::filesystem::remove_all(tempDir);
}

TEST_F(AnalysisDriverFixture,SimpleProject_NonPATToPATProject) {
  {
    // create project with one discrete variable
    SimpleProject project = getCleanSimpleProject("SimpleProject_NonPATToPATProject");
    openstudio::path measuresDir = resourcesPath() / toPath("/utilities/BCL/Measures");
    openstudio::path dir = measuresDir / toPath("SetWindowToWallRatioByFacade");
    BCLMeasure measure = BCLMeasure::load(dir).get();
    RubyMeasure rmeasure(measure);
    project.analysis().problem().push(MeasureGroup("My Variable",MeasureVector(1u,rmeasure)));
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(1u,baseline.variableValues().size());
    EXPECT_TRUE(project.analysis().isDirty());
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
    project.save();
  }

  {
    SimpleProject project = getPATProject("SimpleProject_NonPATToPATProject");
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(2u,baseline.variableValues().size());
    EXPECT_FALSE(project.analysis().isDirty()); // openPATProject calls save in this case
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
    project.save();
  }

  {
    SimpleProject project = getPATProject("SimpleProject_NonPATToPATProject");
    DataPoint baseline = project.baselineDataPoint();
    EXPECT_EQ(2u,baseline.variableValues().size());
    EXPECT_FALSE(project.analysis().isDirty()); // nothing to change in this case
    EXPECT_EQ(1u,project.analysis().dataPoints().size());
  }
}

TEST_F(AnalysisDriverFixture,SimpleProject_EditProblemWithTwoWorkflows) {
  // Written to reproduce Bug 1189
  {
    // Create PAT project with an IDF measure
    SimpleProject project = getCleanPATProject("SimpleProject_EditProblemWithTwoWorkflows");

    Problem problem = project.analysis().problem();
    OptionalInt index = problem.getWorkflowStepIndexByJobType(JobType::EnergyPlusPreProcess);
    ASSERT_TRUE(index);
    MeasureGroup dv("Idf Measure",MeasureVector(1u,NullMeasure()));
    problem.insert(*index,dv);
    for (const BCLMeasure& bclMeasure : BCLMeasure::patApplicationMeasures()) {
      if (bclMeasure.inputFileType() == FileReferenceType::IDF) {
        RubyMeasure rubyMeasure(bclMeasure);
        dv.push(rubyMeasure);
        break;
      }
    }

    project.save();

    ProjectDatabase database = project.projectDatabase();
    RunManager runManager = project.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    for (const WorkflowRecord& wr : workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }
  }

  openstudio::path ospPath;
  {
    // Reopen, change and save
    SimpleProject project = getPATProject("SimpleProject_EditProblemWithTwoWorkflows");

    Model model = exampleModel();
    openstudio::path p = toPath("example.osm");
    model.save(p,false);

    EXPECT_TRUE(project.addAlternateModel(p));

    project.save();

    // For some reason, problem manifested below does not show up here, while ProjectDatabase
    // and RunManager still in memory.
    ProjectDatabase database = project.projectDatabase();
    ospPath = database.path();
    RunManager runManager = project.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    for (const WorkflowRecord& wr : workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }
  }

  {
    // underlying problem -- workflow key in ProjectDatabase and RunManager database get out of sync
    // See ProblemRecord::constructRelatedRecords for code where old WorkflowRecords are removed
    // and new ones are created.
    ProjectDatabase database = ProjectDatabase::open(ospPath).get();
    RunManager runManager = database.runManager();
    WorkflowRecordVector workflowRecords = WorkflowRecord::getWorkflowRecords(database);
    EXPECT_EQ(2u,workflowRecords.size());
    for (const WorkflowRecord& wr : workflowRecords) {
      EXPECT_NO_THROW(runManager.loadWorkflow(wr.runManagerWorkflowKey()));
    }

    // symptom in PAT -- BOOST assert on deserialize Problem (buried in call to openPATProject)
    ProblemRecordVector problemRecords = ProblemRecord::getProblemRecords(database);
    ASSERT_EQ(1u,problemRecords.size());
    ASSERT_NO_FATAL_FAILURE(problemRecords[0].problem());
  }
}

TEST_F(AnalysisDriverFixture, SimpleProject_ZipFileForCloud) {
  openstudio::path tempZipFilePath;

  {
    // get project
    SimpleProject project = getCleanSimpleProject("ProjectToZip");
    Problem problem = retrieveProblem(AnalysisDriverFixtureProblem::BuggyBCLMeasure,true,true);
    project.analysis().setProblem(problem);
    Model model = model::exampleModel();
    openstudio::path weatherFilePath = energyPlusWeatherDataPath() / toPath("USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw");
    EpwFile weatherFile(weatherFilePath);
    OptionalWeatherFile oWeatherFileObject = WeatherFile::setWeatherFile(model,weatherFile);
    EXPECT_TRUE(oWeatherFileObject);
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    std::pair<bool,std::vector<BCLMeasure> > result = project.setSeed(seedModel);
    EXPECT_TRUE(result.first);
    project.makeSelfContained();

    // create zip
    tempZipFilePath = project.zipFileForCloud();

    // unzip and check contents
    openstudio::path remoteDir = project.projectDir().parent_path() / toPath("UnzippedProject");
    if (boost::filesystem::exists(remoteDir)) {
      boost::filesystem::remove_all(remoteDir);
    }
    boost::filesystem::create_directory(remoteDir);
    openstudio::path zipFilePath = remoteDir / tempZipFilePath.filename();
    boost::filesystem::copy_file(tempZipFilePath,zipFilePath);
    UnzipFile unzip(zipFilePath);
    unzip.extractAllFiles(remoteDir);
    EXPECT_TRUE(boost::filesystem::exists(remoteDir / toPath("formulation.json")));
    EXPECT_TRUE(boost::filesystem::exists(remoteDir / toPath("seed")));
    EXPECT_TRUE(boost::filesystem::exists(remoteDir / toPath("seed/example/files")));
  }

  // make sure temp files have been removed
  EXPECT_FALSE(boost::filesystem::exists(tempZipFilePath));
  EXPECT_FALSE(boost::filesystem::exists(tempZipFilePath.parent_path() / toPath("formulation.json")));
  EXPECT_FALSE(boost::filesystem::exists(tempZipFilePath.parent_path() / toPath("seed")));
  EXPECT_FALSE(boost::filesystem::exists(tempZipFilePath.parent_path() / toPath("alternatives")));
}

TEST_F(AnalysisDriverFixture,SimpleProject_AnalysisFixUpOfFilePaths) {
  // GET SIMPLE PROJECT
  SimpleProject project = getCleanSimpleProject("SimpleProject_AnalysisFixUpOfFilePaths");
  Analysis analysis = project.analysis();

  // SET PROBLEM
  Problem problem = retrieveProblem("BuggyBCLMeasure",true,false);
  analysis.setProblem(problem);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);
  analysis.setSeed(seedModel);

  // MAKE SELF CONTAINED
  project.makeSelfContained();
  project.save();

  // CREATE A COPY IN ANOTHER LOCATION
  openstudio::path newProjectDir = project.projectDir().parent_path() / toPath("SimpleProjectAnalysisFixUpOfFilePathsCopy");
  boost::filesystem::remove_all(newProjectDir);
  OptionalSimpleProject oProjectCopy = saveAs(project,newProjectDir);
  ASSERT_TRUE(oProjectCopy);
  SimpleProject projectCopy = *oProjectCopy;

  // UPDATE PATHS IN ORIGINAL ANALYSIS
  EXPECT_NE(project.projectDir(),projectCopy.projectDir());
  analysis.updateInputPathData(project.projectDir(),projectCopy.projectDir());

  // BCLMeasure and Seed Paths should be the same.
  Analysis analysisCopy = projectCopy.analysis();
  EXPECT_EQ(analysisCopy.seed().path(),analysis.seed().path());
  InputVariableVector variablesCopy = analysisCopy.problem().variables();
  InputVariableVector variables = analysis.problem().variables();
  ASSERT_EQ(variablesCopy.size(),variables.size());
  for (unsigned i = 0, n = variablesCopy.size(); i < n; ++i) {
    ASSERT_TRUE(variablesCopy[i].optionalCast<MeasureGroup>());
    ASSERT_TRUE(variables[i].optionalCast<MeasureGroup>());
    MeasureGroup measureGroupCopy = variablesCopy[i].cast<MeasureGroup>();
    MeasureGroup measureGroup = variables[i].cast<MeasureGroup>();
    ASSERT_EQ(measureGroupCopy.measures(false).size(),measureGroup.measures(false).size());
    MeasureVector measuresCopy = measureGroupCopy.measures(false);
    MeasureVector measures = measureGroup.measures(false);
    for (unsigned j = 0, m = measuresCopy.size(); j < m; ++j) {
      if (measuresCopy[j].optionalCast<RubyMeasure>()) {
        ASSERT_TRUE(measures[j].optionalCast<RubyMeasure>());
        RubyMeasure rubyMeasureCopy = measuresCopy[j].cast<RubyMeasure>();
        RubyMeasure rubyMeasure = measures[j].cast<RubyMeasure>();
        ASSERT_TRUE(rubyMeasureCopy.usesBCLMeasure());
        ASSERT_TRUE(rubyMeasure.usesBCLMeasure());
        EXPECT_EQ(rubyMeasureCopy.bclMeasureDirectory(),rubyMeasure.bclMeasureDirectory());
      }
    }
  }
}

TEST_F(AnalysisDriverFixture,SimpleProject_SaveCloudData) {
  {
    SimpleProject project = getCleanPATProject("SimpleProject_SaveCloudData");

    // create basic vagrant data
    VagrantSettings settings;
    settings.setServerPath(toPath("C:/projects/openstudio-server/vagrant/server")); // fake, but realistic path
    settings.setServerUrl(Url("http://localhost:8080"));
    settings.setWorkerPath(toPath("C:/projects/openstudio-server/vagrant/worker")); // fake, but realistic path
    settings.setWorkerUrl(Url("http://localhost:8081"));
    settings.setHaltOnStop(true);
    settings.setUsername("vagrant");
    settings.setPassword("vagrant");
    settings.signUserAgreement(true);
    VagrantSession session(toString(createUUID()),
                           Url("http://localhost:8080"),
                           UrlVector(1u,Url("http://localhost:8081")));

    // save in project
    project.setCloudSettings(settings);
    EXPECT_TRUE(project.cloudSettings());
    project.setCloudSession(session);
    EXPECT_TRUE(project.cloudSession());
    project.save();
  }

  {
    // reopen project and verify that can retrieve vagrant data
    SimpleProject project = getPATProject("SimpleProject_SaveCloudData");

    ASSERT_TRUE(project.cloudSettings());
    ASSERT_TRUE(project.cloudSettings()->optionalCast<VagrantSettings>());
    VagrantSettings settings = project.cloudSettings()->cast<VagrantSettings>();
    EXPECT_EQ("VagrantProvider",settings.cloudProviderType());
    EXPECT_TRUE(settings.userAgreementSigned());
    EXPECT_EQ(toPath("C:/projects/openstudio-server/vagrant/server"),settings.serverPath());
    EXPECT_EQ(Url("http://localhost:8080"),settings.serverUrl());
    EXPECT_EQ(toPath("C:/projects/openstudio-server/vagrant/worker"),settings.workerPath());
    EXPECT_EQ(Url("http://localhost:8081"),settings.workerUrl());
    EXPECT_TRUE(settings.haltOnStop());
    EXPECT_EQ("vagrant",settings.username());
    EXPECT_EQ("",settings.password());
    ASSERT_TRUE(project.cloudSession());
    ASSERT_TRUE(project.cloudSession()->optionalCast<VagrantSession>());
    VagrantSession session = project.cloudSession()->cast<VagrantSession>();
    EXPECT_EQ("VagrantProvider",session.cloudProviderType());
    ASSERT_TRUE(session.serverUrl());
    EXPECT_EQ(Url("http://localhost:8080"),session.serverUrl().get());
    ASSERT_EQ(1u,session.workerUrls().size());
    EXPECT_EQ(Url("http://localhost:8081"),session.workerUrls()[0]);

  }
}
