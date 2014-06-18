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

#include "AnalysisDriverFixture.hpp"

#include "../SimpleProject.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../analysis/Problem.hpp"
#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyContinuousVariable.hpp"
#include "../../analysis/OutputAttributeVariable.hpp"
#include "../../analysis/LinearFunction.hpp"
#include "../../analysis/NormalDistribution.hpp"
#include "../../analysis/LognormalDistribution.hpp"
#include "../../analysis/TriangularDistribution.hpp"
#include "../../analysis/HistogramPointDistribution.hpp"
#include "../../analysis/HistogramBinDistribution.hpp"
#include "../../analysis/GammaDistribution.hpp"
#include "../../analysis/LoguniformDistribution.hpp"

#include "../../project/ProjectDatabase.hpp"
#include "../../project/ProblemRecord.hpp"
#include "../../project/ProblemRecord_Impl.hpp"

#include "../../runmanager/lib/ConfigOptions.hpp"
#include "../../runmanager/lib/RubyJob.hpp"
#include "../../runmanager/lib/RubyJobUtils.hpp"
#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Workflow.hpp"
#include "../../runmanager/lib/WorkItem.hpp"

#include <runmanager/Test/ToolBin.hxx>

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/core/StringHelpers.hpp"
#include "../../utilities/core/Containers.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/Assert.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <vector>
#include <string>

// includes for embedded Ruby argument getter
// put these in cpp, after all other openstudio includes
// otherwise ruby.h will get you, sometime, somewhere
#include "../../utilities/core/RubyInterpreter.hpp"
#include "../../ruleset/EmbeddedRubyUserScriptArgumentGetter.hpp"

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

// static variables
boost::optional<openstudio::FileLogSink> AnalysisDriverFixture::logFile;
std::shared_ptr<openstudio::ruleset::RubyUserScriptArgumentGetter> AnalysisDriverFixture::argumentGetter;

void AnalysisDriverFixture::SetUp() {}

void AnalysisDriverFixture::TearDown() {}

void AnalysisDriverFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./AnalysisDriverFixture.log"));
  logFile->setLogLevel(Debug); // SimpleProjects get set to same log level
  openstudio::Logger::instance().standardOutLogger().disable();

  // list of Ruby modules we want to load into the interpreter
  std::vector<std::string> modules;
  modules.push_back("openstudioutilitiescore");
  modules.push_back("openstudioutilitiesbcl");
  modules.push_back("openstudioutilitiesidd");
  modules.push_back("openstudioutilitiesidf");
  modules.push_back("openstudioutilities");
  modules.push_back("openstudiomodel");
  modules.push_back("openstudiomodelcore");
  modules.push_back("openstudiomodelsimulation");
  modules.push_back("openstudiomodelresources");
  modules.push_back("openstudiomodelgeometry");
  modules.push_back("openstudiomodelhvac");
  modules.push_back("openstudioenergyplus");
  modules.push_back("openstudioruleset");

  // Initialize the embedded Ruby interpreter
  std::shared_ptr<openstudio::detail::RubyInterpreter> rubyInterpreter(
        new openstudio::detail::RubyInterpreter(getOpenStudioRubyPath(),
                                                getOpenStudioRubyScriptsPath(),
                                                modules));
  // Initialize the argument getter
  argumentGetter = std::shared_ptr<RubyUserScriptArgumentGetter>(
      new ruleset::EmbeddedRubyUserScriptArgumentGetter<openstudio::detail::RubyInterpreter>(rubyInterpreter));
}

void AnalysisDriverFixture::TearDownTestCase() {
  logFile->disable();
}

openstudio::analysis::Problem AnalysisDriverFixture::retrieveProblem(
    const std::string& name,
    bool modelToIdf,
    bool energyPlus)
{
  return retrieveProblem(AnalysisDriverFixtureProblem(name),modelToIdf,energyPlus);
}

openstudio::analysis::Problem AnalysisDriverFixture::retrieveProblem(
    const AnalysisDriverFixtureProblem& name,
    bool modelToIdf,
    bool energyPlus)
{
  OptionalProblem original;
  switch(name.value()) {
    case AnalysisDriverFixtureProblem::MixedOsmIdf :
      original = createMixedOsmIdfProblem();
      break;
    case AnalysisDriverFixtureProblem::IdfOnly :
      original = createIdfOnlyProblem();
      break;
    case AnalysisDriverFixtureProblem::Continuous :
      original = createContinuousProblem();
      break;
    case AnalysisDriverFixtureProblem::UserScriptContinuous :
      original = createUserScriptContinuousProblem();
      break;
    case AnalysisDriverFixtureProblem::SimpleUQ :
      original = createSimpleUQProblem();
      break;
    case AnalysisDriverFixtureProblem::MixedContinuousAndDiscreteUQ :
      original = createMixedContinuousAndDiscreteUQProblem();
      break;
    case AnalysisDriverFixtureProblem::SimpleLognormalUQ :
      original = createSimpleLognormalUQProblem();
      break;
    case AnalysisDriverFixtureProblem::SimpleHistogramBinUQ :
      original = createSimpleHistogramBinUQProblem();
      break;
    case AnalysisDriverFixtureProblem::BuggyBCLMeasure :
      original = createBuggyBCLMeasureProblem();
      break;
    default :
      OS_ASSERT(false);
  }
  OS_ASSERT(original);

  analysis::Problem result = original.get();
  appendDefaultSimulationWorkflow(result,modelToIdf,energyPlus);
  return result;
}

void AnalysisDriverFixture::appendDefaultSimulationWorkflow(
    openstudio::analysis::Problem& problem,
    bool modelToIdf,
    bool energyPlus)
{
  bool test(true);
  if (modelToIdf) {
    test = problem.push(runmanager::WorkItem(runmanager::JobType::ModelToIdf));
    ASSERT_TRUE(test);
  }
  else {
    test = problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlusPreProcess));
    ASSERT_TRUE(test);
  }
  if (energyPlus) {
    test = problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlus));
    ASSERT_TRUE(test);
    if (modelToIdf) {
      test = problem.push(runmanager::WorkItem(runmanager::JobType::OpenStudioPostProcess));
      ASSERT_TRUE(test);
    }
    else {
      test = problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlusPostProcess));
      ASSERT_TRUE(test);
    }
  }
}

openstudio::project::ProjectDatabase AnalysisDriverFixture::getCleanDatabase(
    const std::string& projectDatabaseName)
{
  // place in directory of same name
  std::string dirName = openstudio::toUpperCamelCase(projectDatabaseName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);

  if (!boost::filesystem::exists(toPath("AnalysisDriverFixtureData"))) {
    boost::filesystem::create_directory(toPath("AnalysisDriverFixtureData"));
  }

  // clean out old results
  if (boost::filesystem::exists(workingDir)) {
    boost::filesystem::remove_all(workingDir);
  }
  boost::filesystem::create_directory(workingDir);

  openstudio::path runPath = workingDir / toPath(projectDatabaseName + "Run.db");
  // change last to true if .showStatusDialog().
  runmanager::RunManager runManager(runPath, true, false, false);

  path projectPath = workingDir / toPath(projectDatabaseName + ".osp");
  project::ProjectDatabase database(projectPath, runManager, true);

  return database;
}

openstudio::project::ProjectDatabase AnalysisDriverFixture::getDatabase(
      const std::string& projectDatabaseName)
{
  // mimic above, just don't blow away old databases
  std::string dirName = openstudio::toUpperCamelCase(projectDatabaseName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);
  path projectPath = workingDir / toPath(projectDatabaseName + ".osp");
  project::ProjectDatabase database = project::ProjectDatabase::open(projectPath).get();
  return database;
}

openstudio::analysisdriver::SimpleProject AnalysisDriverFixture::getCleanSimpleProject(
    const std::string& projectName)
{
  // place in directory of same name
  std::string dirName = openstudio::toUpperCamelCase(projectName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);
  LOG(Debug,"Creating working directory at " << toString(workingDir) << ".");

  if (!boost::filesystem::exists(toPath("AnalysisDriverFixtureData"))) {
    boost::filesystem::create_directory(toPath("AnalysisDriverFixtureData"));
  }

  // clean out old results
  if (boost::filesystem::exists(workingDir)) {
    boost::filesystem::remove_all(workingDir);
  }

  SimpleProjectOptions options;
  if (boost::optional<LogLevel> logLevel = logFile->logLevel()) {
    options.setLogLevel(*logLevel);
  }

  LOG(Debug,"Creating SimpleProject in directory " << toString(workingDir) << ".");
  return SimpleProject::create(workingDir,options).get();
}

openstudio::analysisdriver::SimpleProject AnalysisDriverFixture::getCleanPATProject(
    const std::string& projectName)
{
  // place in directory of same name
  std::string dirName = openstudio::toUpperCamelCase(projectName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);
  LOG(Debug,"Creating working directory at " << toString(workingDir) << ".");

  if (!boost::filesystem::exists(toPath("AnalysisDriverFixtureData"))) {
    boost::filesystem::create_directory(toPath("AnalysisDriverFixtureData"));
  }

  // clean out old results
  if (boost::filesystem::exists(workingDir)) {
    boost::filesystem::remove_all(workingDir);
  }

  SimpleProjectOptions options;
  if (boost::optional<LogLevel> logLevel = logFile->logLevel()) {
    options.setLogLevel(*logLevel);
  }

  LOG(Debug,"Creating PAT SimpleProject in directory " << toString(workingDir) << ".");
  return createPATProject(workingDir,options).get();
}

/** Returns an already existing SimpleProject for testing. */
openstudio::analysisdriver::SimpleProject AnalysisDriverFixture::getSimpleProject(
    const std::string& projectName)
{
  // mimic above, just don't blow away old databases
  std::string dirName = openstudio::toUpperCamelCase(projectName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);

  SimpleProjectOptions options;
  if (boost::optional<LogLevel> logLevel = logFile->logLevel()) {
    options.setLogLevel(*logLevel);
  }
  return SimpleProject::open(workingDir,options).get();
}

openstudio::analysisdriver::SimpleProject AnalysisDriverFixture::getPATProject(
    const std::string& projectName)
{
  // mimic above, just don't blow away old databases
  std::string dirName = openstudio::toUpperCamelCase(projectName);
  openstudio::path workingDir = toPath("AnalysisDriverFixtureData") / toPath(dirName);

  SimpleProjectOptions options;
  if (boost::optional<LogLevel> logLevel = logFile->logLevel()) {
    options.setLogLevel(*logLevel);
  }
  return openPATProject(workingDir,options).get();
}

openstudio::analysisdriver::AnalysisRunOptions AnalysisDriverFixture::standardRunOptions(
    const openstudio::path& workingDirectory)
{
  AnalysisRunOptions runOptions(workingDirectory,
                                toPath(rubyOpenStudioDir()),
                                dakotaExePath());
  runOptions.setQueueSize(4);
  runmanager::Tools tools = runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(),
                                                                 openstudio::path(),
                                                                 openstudio::path(),
                                                                 rubyExePath().parent_path(),
                                                                 dakotaExePath().parent_path());
  runOptions.setRunManagerTools(tools);
  return runOptions;
}

openstudio::analysis::Problem AnalysisDriverFixture::createMixedOsmIdfProblem()
{
  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  analysis::Problem problem("MixedOsmIdf");

  // Variable 1: Wall Construction
  analysis::MeasureVector measures1;
  measures1.push_back(analysis::NullMeasure());
  // perturb the wall construction object
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  analysis::RubyMeasure rubyMeasure1(perturbScript,
                                     FileReferenceType::OSM,
                                     FileReferenceType::OSM);
  rubyMeasure1.addArgument("inputPath", "in.osm");
  rubyMeasure1.addArgument("outputPath", "out.osm");
  rubyMeasure1.addArgument("objectType", "OS:Material");
  rubyMeasure1.addArgument("nameRegex", "I02 50mm insulation board");
  rubyMeasure1.addArgument("field", "3");
  rubyMeasure1.addArgument("value", "0.10");
  measures1.push_back(rubyMeasure1);
  bool ok = problem.push(analysis::MeasureGroup("Wall Construction",measures1));
  OS_ASSERT(ok);

  // WorkflowStep: ModelToIdf
  ok = problem.push(WorkflowStep(runmanager::WorkItem(runmanager::JobType::ModelToIdf)));
  OS_ASSERT(ok);

  // Variable 2: Roof Construction
  analysis::MeasureVector measures2;
  measures2.push_back(analysis::NullMeasure());
  // perturb the roof construction object
  analysis::RubyMeasure rubyMeasure2(perturbScript,FileReferenceType::IDF,FileReferenceType::IDF);
  rubyMeasure2.addArgument("inputPath", "in.idf");
  rubyMeasure2.addArgument("outputPath", "out.idf");
  rubyMeasure2.addArgument("objectType", "Material");
  rubyMeasure2.addArgument("nameRegex", "M11 100mm lightweight concrete");
  rubyMeasure2.addArgument("field", "2");
  rubyMeasure2.addArgument("value", "0.2");
  measures2.push_back(rubyMeasure2);
  ok = problem.push(analysis::MeasureGroup("Roof Construction",measures2));
  OS_ASSERT(ok);

  OS_ASSERT(problem.numVariables() == 2);

  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createIdfOnlyProblem()
{
  // Variables
  analysis::VariableVector variables;
  std::string name;
  analysis::MeasureVector perturbations;
  openstudio::path perturbScript =
      openstudio::toPath(rubyLibDir()) /
      openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::IDF,
                                    FileReferenceType::IDF);

  // Variable 1
  name = "Lighting Power Density";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 10 W/m^2
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "Lights");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "4");
  rubyMeasure.addArgument("value", " ");
  rubyMeasure.addArgument("field", "5");
  rubyMeasure.addArgument("value", "10.0");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 2
  name = "Plug Load Density";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 6 W/m^2
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "ElectricEquipment");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "4");
  rubyMeasure.addArgument("value", " ");
  rubyMeasure.addArgument("field", "5");
  rubyMeasure.addArgument("value", "6.0");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 3
  name = "Window Construction";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // Dbl LoE (e2=.1) Clr 6mm/6mm Air
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "FenestrationSurface:Detailed");
  rubyMeasure.addArgument("nameRegex", "W.*");
  rubyMeasure.addArgument("field", "2");
  rubyMeasure.addArgument("value", "Dbl LoE (e2=.1) Clr 6mm/6mm Air");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 4
  name = "Fan Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 0.9
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "Fan:VariableVolume");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "2");
  rubyMeasure.addArgument("value", "0.9");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 5
  name = "Fan Motor Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 0.95
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "Fan:VariableVolume");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "8");
  rubyMeasure.addArgument("value", "0.95");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 6
  name = "Boiler Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 0.95
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "Boiler:HotWater");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "3");
  rubyMeasure.addArgument("value", "0.95");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  // Variable 7
  name = "Chiller COP";
  perturbations.clear();
  perturbations.push_back(analysis::NullMeasure());
  // 5.0
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::IDF,
                                      FileReferenceType::IDF);
  rubyMeasure.addArgument("inputPath", "in.idf");
  rubyMeasure.addArgument("outputPath", "out.idf");
  rubyMeasure.addArgument("objectType", "Chiller:Electric");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "3");
  rubyMeasure.addArgument("value", "5.0");
  perturbations.push_back(rubyMeasure);

  variables.push_back(analysis::MeasureGroup(name,perturbations));

  analysis::Problem problem("IdfOnly",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createContinuousProblem()
{
  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  // Variables
  analysis::VariableVector variables;

  openstudio::path perturbScript =
      openstudio::toPath(rubyLibDir()) /
      openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // Variable 1: Building Rotation
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Building");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "3");
  analysis::RubyContinuousVariable continuousVariable(
        "Building Rotation",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  continuousVariable.setMinimum(0.0);
  continuousVariable.setMaximum(270.0);
  variables.push_back(continuousVariable);

  // Variable 2: Fan Efficiency
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Fan:ConstantVolume");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "3");
  continuousVariable = analysis::RubyContinuousVariable(
        "Fan Efficiency",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  continuousVariable.setMinimum(0.7);
  continuousVariable.setMaximum(0.9);
  variables.push_back(continuousVariable);

  // Variable 3: Wall Construction set as Static Transformation
  // Not counted as variable in communications between analysis and DAKOTA.
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath", "in.osm");
  rubyMeasure.addArgument("outputPath", "out.osm");
  rubyMeasure.addArgument("objectType", "OS:Material");
  rubyMeasure.addArgument("nameRegex", "I02 50mm insulation board");
  rubyMeasure.addArgument("field", "3");
  rubyMeasure.addArgument("value", "0.10");
  variables.push_back(analysis::MeasureGroup("Wall Construction",analysis::MeasureVector(1u,rubyMeasure)));

  // ETH@20130806: Removing response for now because we usually only run post-processes
  // following a simulation. (We were using a ModelRuleset to calculate an input value as
  // the response.)

  analysis::Problem problem("Continuous",variables,analysis::FunctionVector(),runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createUserScriptContinuousProblem()
{
  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  // Variables
  analysis::VariableVector variables;

  // Pertubation 1, Governed by Variables 1 & 2: Set Window to Wall Ratio
  analysis::RubyMeasure wwrScript(rubyLibDirPath/openstudio::toPath("openstudio/sketchup_plugin/user_scripts/Alter or Add Model Elements/Set_Window_to_Wall_Ratio.rb"),
                                  FileReferenceType::OSM,
                                  FileReferenceType::OSM,
                                  true);
  // Choice Argument "application_type" will be defaulted to "Above Floor".

  // Variable 1: Window to Wall Ratio
  ruleset::OSArgument arg = ruleset::OSArgument::makeDoubleArgument("wwr");
  analysis::RubyContinuousVariable wwr("Window to Wall Ratio",arg,wwrScript);
  wwr.setMinimum(0.15);
  wwr.setMaximum(0.60);
  variables.push_back(wwr);

  // Variable 2: Offset (Above Floor)
  arg = ruleset::OSArgument::makeDoubleArgument("offset");
  analysis::RubyContinuousVariable offset("Window Offset from Floor",arg,wwrScript);
  offset.setMinimum(0.5);
  offset.setMaximum(2.0);
  variables.push_back(offset);

  // Measure 2, Governed by Variable 3: Add Overhangs by Projection Factor (Offset Fixed at 0.0)
  analysis::RubyMeasure overhangScript(rubyLibDirPath/openstudio::toPath("openstudio/sketchup_plugin/user_scripts/Alter or Add Model Elements/Add_Overhangs_by_Projection_Factor.rb"),
                                       FileReferenceType::OSM,
                                       FileReferenceType::OSM,
                                       true);
  // Double Argument "offset" will be defaulted to 0.0
  // Change Bool Argument "remove_existing_shading" to false
  arg = ruleset::OSArgument::makeBoolArgument("remove_existing_shading");
  arg.setValue(true);
  overhangScript.addArgument(arg);

  // Variable 3: Projection Factor
  arg = ruleset::OSArgument::makeDoubleArgument("projection_factor");
  analysis::RubyContinuousVariable projectionFactor("Overhang Projection Factor",
                                                    arg,
                                                    overhangScript);
  projectionFactor.setMinimum(0.2);
  projectionFactor.setMaximum(1.5);
  variables.push_back(projectionFactor);

  analysis::Problem problem("UserScriptContinuous",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createSimpleUQProblem()
{
  // Variables
  analysis::VariableVector variables;

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // These variables are based on Corrado and Mechri's description and data on
  // uncertainties in thermophysical properties in the 2009 article "Uncertainty
  // and Sensitivity Analysis for Building Energy Rating".

  // Varying concrete thermal conductivities, because that is what is available as
  // attributes. Using values in file as mean, 0.3 as standard deviation, bounds on
  // distribution as +/- 1 standard deviation.

  // Variable 1: Slab concrete thermal conductivity
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "MAT-CC05 8 HW CONCRETE");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  analysis::RubyContinuousVariable variable(
        "Slab Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  analysis::NormalDistribution boundedNormal(1.311,0.3);
  boundedNormal.setLowerBound(boundedNormal.mean() - boundedNormal.standardDeviation());
  boundedNormal.setUpperBound(boundedNormal.mean() + boundedNormal.standardDeviation());
  variable.setUncertaintyDescription(boundedNormal);
  variables.push_back(variable);

  // Variable 2: Exterior wall concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M15 200mm heavyweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Exterior Wall Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  boundedNormal = analysis::NormalDistribution(1.95,0.3);
  boundedNormal.setLowerBound(boundedNormal.mean() - boundedNormal.standardDeviation());
  boundedNormal.setUpperBound(boundedNormal.mean() + boundedNormal.standardDeviation());
  variable.setUncertaintyDescription(boundedNormal);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M11 100mm lightweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Roof Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  boundedNormal = analysis::NormalDistribution(0.53,0.3);
  boundedNormal.setLowerBound(boundedNormal.mean() - boundedNormal.standardDeviation());
  boundedNormal.setUpperBound(boundedNormal.mean() + boundedNormal.standardDeviation());
  variable.setUncertaintyDescription(boundedNormal);
  variables.push_back(variable);

  analysis::Problem problem("SimpleUQ",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createMixedContinuousAndDiscreteUQProblem()
{
  // Variables
  analysis::VariableVector variables;

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  analysis::MeasureVector measures;

  // Variable 1: Number of printers
  // 50% likely to have 1 printer, 30% likely to have 2, 20% likely to have 3
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:ElectricEquipment");
  rubyMeasure.addArgument("nameRegex", "Printer");
  rubyMeasure.addArgument("field", "5"); // Multiplier
  rubyMeasure.addArgument("value","1"); // 1 Printer
  rubyMeasure.setName("1 Printer");
  measures.push_back(rubyMeasure);
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:ElectricEquipment");
  rubyMeasure.addArgument("nameRegex", "Printer");
  rubyMeasure.addArgument("field", "5"); // Multiplier
  rubyMeasure.addArgument("value","2"); // 2 Printers
  rubyMeasure.setName("2 Printers");
  measures.push_back(rubyMeasure);
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:ElectricEquipment");
  rubyMeasure.addArgument("nameRegex", "Printer");
  rubyMeasure.addArgument("field", "5"); // Multiplier
  rubyMeasure.addArgument("value","3"); // 3 Printers
  rubyMeasure.setName("3 Printers");
  measures.push_back(rubyMeasure);
  analysis::MeasureGroup dv("Number of Printers",measures);
  DoubleVector abscissas, counts;
  abscissas.push_back(0);
  counts.push_back(0.5);
  abscissas.push_back(1);
  counts.push_back(0.3);
  abscissas.push_back(2);
  counts.push_back(0.2);
  analysis::HistogramPointDistribution discreteHistogram(abscissas,counts);
  dv.setUncertaintyDescription(discreteHistogram);
  variables.push_back(dv);

  // Variable 2: Number of people
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:People:Definition");
  rubyMeasure.addArgument("nameRegex", ".*");
  rubyMeasure.addArgument("field", "4"); // People per Space Floor Area
  analysis::RubyContinuousVariable cv(
        "People Density",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  analysis::TriangularDistribution triangularDist(0.05,0.01,0.2);
  cv.setUncertaintyDescription(triangularDist);
  variables.push_back(cv);

  analysis::Problem problem("MixedContinuousAndDiscreteUQ",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createSimpleLognormalUQProblem()
{
  // Variables
  analysis::VariableVector variables;

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // This tests the lognormal distribution, since it requires 2 of 5 optional variables to be
  // specified.

  // Variable 1: Slab concrete thermal conductivity
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "MAT-CC05 8 HW CONCRETE");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  analysis::RubyContinuousVariable variable(
        "Slab Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  analysis::LognormalDistribution lognormal;
  lognormal.setLambda(2.5);
  lognormal.setZeta(1.2);
  lognormal.setLowerBound(0.1);
  lognormal.setUpperBound(10.0);
  variable.setUncertaintyDescription(lognormal);
  variables.push_back(variable);

  // Variable 2: Exterior wall concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M15 200mm heavyweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Exterior Wall Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  lognormal = analysis::LognormalDistribution();
  lognormal.setLambda(3.5);
  lognormal.setZeta(2.2);
  lognormal.setLowerBound(0.1);
  lognormal.setUpperBound(10.0);
  variable.setUncertaintyDescription(lognormal);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M11 100mm lightweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Roof Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  lognormal = analysis::LognormalDistribution();
  lognormal.setLambda(4.5);
  lognormal.setZeta(1.5);
  lognormal.setLowerBound(0.1);
  lognormal.setUpperBound(10.0);
  variable.setUncertaintyDescription(lognormal);
  variables.push_back(variable);

  analysis::Problem problem("SimpleLognormalUQ",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createSimpleHistogramBinUQProblem()
{
  // Variables
  analysis::VariableVector variables;

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // This tests the histogram bin distribution, since it requires 1 of 2 optional variables to be
  // specified.

  // Variable 1: Slab concrete thermal conductivity
  analysis::RubyMeasure rubyMeasure(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "MAT-CC05 8 HW CONCRETE");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  analysis::RubyContinuousVariable variable(
        "Slab Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  DoubleVector abscissas, counts;
  abscissas.push_back(1.1);
  counts.push_back(0.3);
  abscissas.push_back(1.3);
  counts.push_back(0.3);
  abscissas.push_back(1.6);
  counts.push_back(0.2);
  abscissas.push_back(1.8);
  counts.push_back(0.2);
  abscissas.push_back(2.0);
  counts.push_back(0.0);
  analysis::HistogramBinDistribution histogramBin(abscissas, counts);
  variable.setUncertaintyDescription(histogramBin);
  variables.push_back(variable);

  // Variable 2: Exterior wall concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M15 200mm heavyweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Exterior Wall Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  analysis::LoguniformDistribution loguniform;
  loguniform.setLowerBound(1);
  loguniform.setUpperBound(1.6);
  variable.setUncertaintyDescription(loguniform);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  rubyMeasure = analysis::RubyMeasure(perturbScript,
                                      FileReferenceType::OSM,
                                      FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath","in.osm");
  rubyMeasure.addArgument("outputPath","out.osm");
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex", "M11 100mm lightweight concrete");
  rubyMeasure.addArgument("field", "4"); // Conductivity
  variable = analysis::RubyContinuousVariable(
        "Roof Concrete Thermal Conductivity",
        ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure);
  analysis::GammaDistribution gamma(1.5,0.5);
  variable.setUncertaintyDescription(gamma);
  variables.push_back(variable);

  analysis::Problem problem("SimpleHistogramBinUQ",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createBuggyBCLMeasureProblem() {
  Problem problem("Buggy BCLMeasure Problem");

  BCLMeasure measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestFlagAsNotApplicable")).get();
  RubyMeasure rpert(measure);
  ruleset::OSArgument arg = ruleset::OSArgument::makeBoolArgument("applicable", true);
  arg.setValue(false);
  rpert.setArgument(arg);
  problem.push(MeasureGroup("NA Transformation",MeasureVector(1u,rpert)));

  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestCreateWarningMsgs")).get();
  runmanager::RubyJobBuilder rjb(measure);
  rjb.setIncludeDir(toPath(rubyOpenStudioDir()));
  runmanager::WorkItem workItem = rjb.toWorkItem();
  problem.push(workItem);

  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestMissingARequiredArgWithNoDefault")).get();
  rpert = RubyMeasure(measure);
  problem.push(MeasureGroup("Missing Argument",MeasureVector(1u,rpert)));

  // should never get to here b/c previous variable should fail
  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestFlagAsNotApplicable")).get();
  rjb = runmanager::RubyJobBuilder(measure);
  rjb.setIncludeDir(toPath(rubyOpenStudioDir()));
  workItem = rjb.toWorkItem();
  problem.push(workItem);

  return problem;
}
