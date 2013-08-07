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

#include <analysisdriver/test/AnalysisDriverFixture.hpp>

#include <analysisdriver/SimpleProject.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>

#include <analysis/Problem.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/ModelRulesetContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/OutputAttributeVariable.hpp>
#include <analysis/LinearFunction.hpp>
#include <analysis/NormalDistribution.hpp>
#include <analysis/LognormalDistribution.hpp>
#include <analysis/TriangularDistribution.hpp>
#include <analysis/HistogramPointDistribution.hpp>
#include <analysis/HistogramBinDistribution.hpp>
#include <analysis/GammaDistribution.hpp>
#include <analysis/LoguniformDistribution.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ProblemRecord_Impl.hpp>

#include <runmanager/lib/ConfigOptions.hpp>
#include <runmanager/lib/RubyJob.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <runmanager/Test/ToolBin.hxx>

#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/RulesetEnums.hpp>
#include <ruleset/OSArgument.hpp>

#include <utilities/core/StringHelpers.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <vector>
#include <string>

// includes for embedded Ruby argument getter
// put these in cpp, after all other openstudio includes
// otherwise ruby.h will get you, sometime, somewhere
#include <utilities/core/RubyInterpreter.hpp>
#include <ruleset/RubyUserScriptArgumentGetter_Impl.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

// static variables
boost::optional<openstudio::FileLogSink> AnalysisDriverFixture::logFile;
boost::shared_ptr<openstudio::ruleset::RubyUserScriptArgumentGetter> AnalysisDriverFixture::argumentGetter;

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
  boost::shared_ptr<openstudio::detail::RubyInterpreter> rubyInterpreter(
        new openstudio::detail::RubyInterpreter(getOpenStudioRubyPath(),
                                                getOpenStudioRubyScriptsPath(),
                                                modules));
  // Initialize the argument getter
  argumentGetter = boost::shared_ptr<RubyUserScriptArgumentGetter>(
      new ruleset::detail::RubyUserScriptArgumentGetter_Impl<openstudio::detail::RubyInterpreter>(rubyInterpreter));
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

  // Variables
  analysis::VariableVector variables;

  // Variable 1: Wall Construction
  analysis::DiscretePerturbationVector perturbations1;
  perturbations1.push_back(analysis::NullPerturbation());
  // perturb the wall construction object
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  analysis::RubyPerturbation rubyPerturbation1(perturbScript,
                                               FileReferenceType::OSM,
                                               FileReferenceType::OSM);
  rubyPerturbation1.addArgument("inputPath", "in.osm");
  rubyPerturbation1.addArgument("outputPath", "out.osm");
  rubyPerturbation1.addArgument("objectType", "OS:Material");
  rubyPerturbation1.addArgument("nameRegex", "I02 50mm insulation board");
  rubyPerturbation1.addArgument("field", "3");
  rubyPerturbation1.addArgument("value", "0.10");
  perturbations1.push_back(rubyPerturbation1);
  variables.push_back(analysis::DiscreteVariable("Wall Construction",perturbations1));

  // Variable X: ModelToIdf
  openstudio::path modelToIdfScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/ModelToIdf.rb");
  analysis::RubyPerturbation rubyPerturbationX(modelToIdfScript,
                                               FileReferenceType::OSM,
                                               FileReferenceType::IDF);
  rubyPerturbationX.addArgument("inputPath", "in.osm");
  rubyPerturbationX.addArgument("outputPath", "out.idf");
  variables.push_back(analysis::DiscreteVariable(
      "ModelToIdf",
      analysis::DiscretePerturbationVector(1u,rubyPerturbationX)));

  // Variable 2: Roof Construction
  analysis::DiscretePerturbationVector perturbations2;
  perturbations2.push_back(analysis::NullPerturbation());
  // perturb the roof construction object
  analysis::RubyPerturbation rubyPerturbation2(perturbScript,FileReferenceType::IDF,FileReferenceType::IDF);
  rubyPerturbation2.addArgument("inputPath", "in.idf");
  rubyPerturbation2.addArgument("outputPath", "out.idf");
  rubyPerturbation2.addArgument("objectType", "Material");
  rubyPerturbation2.addArgument("nameRegex", "M11 100mm lightweight concrete");
  rubyPerturbation2.addArgument("field", "2");
  rubyPerturbation2.addArgument("value", "0.2");
  perturbations2.push_back(rubyPerturbation2);
  variables.push_back(analysis::DiscreteVariable("Roof Construction",perturbations2));

  analysis::Problem problem("MixedOsmIdf",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createIdfOnlyProblem()
{
  // Variables
  analysis::VariableVector variables;
  std::string name;
  analysis::DiscretePerturbationVector perturbations;
  openstudio::path perturbScript =
      openstudio::toPath(rubyLibDir()) /
      openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  analysis::RubyPerturbation rubyPerturbation(perturbScript,
                                              FileReferenceType::IDF,
                                              FileReferenceType::IDF);

  // Variable 1
  name = "Lighting Power Density";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 10 W/m^2
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "Lights");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "4");
  rubyPerturbation.addArgument("value", " ");
  rubyPerturbation.addArgument("field", "5");
  rubyPerturbation.addArgument("value", "10.0");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 2
  name = "Plug Load Density";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 6 W/m^2
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "ElectricEquipment");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "4");
  rubyPerturbation.addArgument("value", " ");
  rubyPerturbation.addArgument("field", "5");
  rubyPerturbation.addArgument("value", "6.0");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 3
  name = "Window Construction";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // Dbl LoE (e2=.1) Clr 6mm/6mm Air
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "FenestrationSurface:Detailed");
  rubyPerturbation.addArgument("nameRegex", "W.*");
  rubyPerturbation.addArgument("field", "2");
  rubyPerturbation.addArgument("value", "Dbl LoE (e2=.1) Clr 6mm/6mm Air");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 4
  name = "Fan Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 0.9
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "Fan:VariableVolume");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "2");
  rubyPerturbation.addArgument("value", "0.9");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 5
  name = "Fan Motor Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 0.95
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "Fan:VariableVolume");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "8");
  rubyPerturbation.addArgument("value", "0.95");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 6
  name = "Boiler Efficiency";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 0.95
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "Boiler:HotWater");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "3");
  rubyPerturbation.addArgument("value", "0.95");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  // Variable 7
  name = "Chiller COP";
  perturbations.clear();
  perturbations.push_back(analysis::NullPerturbation());
  // 5.0
  rubyPerturbation = analysis::RubyPerturbation(perturbScript,
                                                FileReferenceType::IDF,
                                                FileReferenceType::IDF);
  rubyPerturbation.addArgument("inputPath", "in.idf");
  rubyPerturbation.addArgument("outputPath", "out.idf");
  rubyPerturbation.addArgument("objectType", "Chiller:Electric");
  rubyPerturbation.addArgument("nameRegex", ".*");
  rubyPerturbation.addArgument("field", "3");
  rubyPerturbation.addArgument("value", "5.0");
  perturbations.push_back(rubyPerturbation);

  variables.push_back(analysis::DiscreteVariable(name,perturbations));

  analysis::Problem problem("IdfOnly",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createContinuousProblem()
{
  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  // Variables
  analysis::VariableVector variables;

  // Variable 1: Building Rotation
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Building));
  analysis::ModelRulesetContinuousVariable continuousVariable("Building Rotation",
                                                              filters,
                                                              "northAxis");
  continuousVariable.setMinimum(0.0);
  continuousVariable.setMaximum(270.0);
  variables.push_back(continuousVariable);

  // Variable 2: Fan Efficiency
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Fan_ConstantVolume));
  continuousVariable = analysis::ModelRulesetContinuousVariable("Fan Efficiency",
                                                                filters,
                                                                "fanEfficiency");
  continuousVariable.setMinimum(0.7);
  continuousVariable.setMaximum(0.9);
  variables.push_back(continuousVariable);

  // Variable 3: Wall Construction set as Static Transformation
  // Not counted as variable in communications between analysis and DAKOTA.
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  analysis::RubyPerturbation rubyPerturbation(perturbScript,
                                              FileReferenceType::OSM,
                                              FileReferenceType::OSM);
  rubyPerturbation.addArgument("inputPath", "in.osm");
  rubyPerturbation.addArgument("outputPath", "out.osm");
  rubyPerturbation.addArgument("objectType", "OS:Material");
  rubyPerturbation.addArgument("nameRegex", "I02 50mm insulation board");
  rubyPerturbation.addArgument("field", "3");
  rubyPerturbation.addArgument("value", "0.10");
  variables.push_back(analysis::DiscreteVariable("Wall Construction",analysis::DiscretePerturbationVector(1u,rubyPerturbation)));

  // Responses
  analysis::FunctionVector responses;

  // Response 1: Exterior Wall U-Factor
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_Surface)));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute("surfaceType",
                                                              ruleset::RulesetStringPredicate::IEquals,
                                                              "wall"));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute("outsideBoundaryCondition",
                                                              ruleset::RulesetStringPredicate::IEquals,
                                                              "outdoors"));
  analysis::ModelRulesetContinuousVariable response1Variable("Exterior Wall U-Factor",filters,"uFactor");
  responses.push_back(analysis::LinearFunction(response1Variable.name(),
                                               analysis::VariableVector(1u,response1Variable.cast<analysis::Variable>())));

  analysis::Problem problem("Continuous",variables,responses,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createUserScriptContinuousProblem()
{
  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  // Variables
  analysis::VariableVector variables;

  // Pertubation 1, Governed by Variables 1 & 2: Set Window to Wall Ratio
  analysis::RubyPerturbation wwrScript(rubyLibDirPath/openstudio::toPath("openstudio/sketchup_plugin/user_scripts/Alter or Add Model Elements/Set_Window_to_Wall_Ratio.rb"),
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

  // Perturbation 2, Governed by Variable 3: Add Overhangs by Projection Factor (Offset Fixed at 0.0)
  analysis::RubyPerturbation overhangScript(rubyLibDirPath/openstudio::toPath("openstudio/sketchup_plugin/user_scripts/Alter or Add Model Elements/Add_Overhangs_by_Projection_Factor.rb"),
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

  // These variables are based on Corrado and Mechri's description and data on
  // uncertainties in thermophysical properties in the 2009 article "Uncertainty
  // and Sensitivity Analysis for Building Energy Rating".

  // Varying concrete thermal conductivities, because that is what is available as
  // attributes. Using values in file as mean, 0.3 as standard deviation, bounds on
  // distribution as +/- 1 standard deviation.

  // Variable 1: Slab concrete thermal conductivity
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "MAT-CC05 8 HW CONCRETE"));
  analysis::ModelRulesetContinuousVariable variable("Slab Concrete Thermal Conductivity",
                                                    filters,
                                                    "thermalConductivity");
  analysis::NormalDistribution boundedNormal(1.311,0.3);
  boundedNormal.setLowerBound(boundedNormal.mean() - boundedNormal.standardDeviation());
  boundedNormal.setUpperBound(boundedNormal.mean() + boundedNormal.standardDeviation());
  variable.setUncertaintyDescription(boundedNormal);
  variables.push_back(variable);

  // Variable 2: Exterior wall concrete thermal conductivity
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M15 200mm heavyweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Exterior Wall Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
  boundedNormal = analysis::NormalDistribution(1.95,0.3);
  boundedNormal.setLowerBound(boundedNormal.mean() - boundedNormal.standardDeviation());
  boundedNormal.setUpperBound(boundedNormal.mean() + boundedNormal.standardDeviation());
  variable.setUncertaintyDescription(boundedNormal);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M11 100mm lightweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Roof Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
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

  // Variable 1: Number of printers
  // 50% likely to have 1 printer, 30% likely to have 2, 20% likely to have 3
  analysis::DiscretePerturbationVector perturbations;
  ruleset::ModelRuleset ruleset("1 Printer");
  ruleset::ModelRule rule("1 Printer");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_ElectricEquipment)));
  rule.add(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "Printer"));
  rule.add(ruleset::ModelObjectActionSetAttribute("multiplier",int(1)));
  ruleset.add(rule);
  perturbations.push_back(analysis::ModelRulesetPerturbation(ruleset));
  ruleset = ruleset::ModelRuleset("2 Printers");
  rule = ruleset::ModelRule("2 Printers");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_ElectricEquipment)));
  rule.add(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "Printer"));
  rule.add(ruleset::ModelObjectActionSetAttribute("multiplier",int(2)));
  ruleset.add(rule);
  perturbations.push_back(analysis::ModelRulesetPerturbation(ruleset));
  ruleset = ruleset::ModelRuleset("3 Printers");
  rule = ruleset::ModelRule("3 Printers");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_ElectricEquipment)));
  rule.add(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "Printer"));
  rule.add(ruleset::ModelObjectActionSetAttribute("multiplier",int(3)));
  ruleset.add(rule);
  perturbations.push_back(analysis::ModelRulesetPerturbation(ruleset));
  analysis::DiscreteVariable dv("Number of Printers",perturbations);
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
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_People_Definition));
  analysis::ModelRulesetContinuousVariable cv("People Density",
                                              filters,
                                              "peopleperSpaceFloorArea");
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

  // This tests the lognormal distribution, since it requires 2 of 5 optional variables to be
  // specified.

  // Variable 1: Slab concrete thermal conductivity
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "MAT-CC05 8 HW CONCRETE"));
  analysis::ModelRulesetContinuousVariable variable("Slab Concrete Thermal Conductivity",
                                                    filters,
                                                    "thermalConductivity");
  analysis::LognormalDistribution lognormal;
  lognormal.setLambda(2.5);
  lognormal.setZeta(1.2);
  lognormal.setLowerBound(0.1);
  lognormal.setUpperBound(10.0);
  variable.setUncertaintyDescription(lognormal);
  variables.push_back(variable);

  // Variable 2: Exterior wall concrete thermal conductivity
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M15 200mm heavyweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Exterior Wall Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
  lognormal = analysis::LognormalDistribution();
  lognormal.setLambda(3.5);
  lognormal.setZeta(2.2);
  lognormal.setLowerBound(0.1);
  lognormal.setUpperBound(10.0);
  variable.setUncertaintyDescription(lognormal);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M11 100mm lightweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Roof Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
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

  // This tests the histogram bin distribution, since it requires 1 of 2 optional variables to be
  // specified.

  // Variable 1: Slab concrete thermal conductivity
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "MAT-CC05 8 HW CONCRETE"));
  analysis::ModelRulesetContinuousVariable variable("Slab Concrete Thermal Conductivity",
                                                    filters,
                                                    "thermalConductivity");
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
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M15 200mm heavyweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Exterior Wall Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
  analysis::LoguniformDistribution loguniform;
  loguniform.setLowerBound(1);
  loguniform.setUpperBound(1.6);
  variable.setUncertaintyDescription(loguniform);
  variables.push_back(variable);

  // Variable 3: Roof concrete thermal conductivity
  filters.clear();
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType::OS_Material));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute(
      "name",
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "M11 100mm lightweight concrete"));
  variable = analysis::ModelRulesetContinuousVariable("Roof Concrete Thermal Conductivity",
                                                      filters,
                                                      "thermalConductivity");
  analysis::GammaDistribution gamma(1.5,0.5);
  variable.setUncertaintyDescription(gamma);
  variables.push_back(variable);

  analysis::Problem problem("SimpleHistogramBinUQ",variables,runmanager::Workflow());
  return problem;
}

openstudio::analysis::Problem AnalysisDriverFixture::createBuggyBCLMeasureProblem() {
  Problem problem("Buggy BCLMeasure Problem");

  BCLMeasure measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestFlagAsNotApplicable")).get();
  RubyPerturbation rpert(measure);
  ruleset::OSArgument arg = ruleset::OSArgument::makeBoolArgument("applicable", true);
  arg.setValue(false);
  rpert.setArgument(arg);
  problem.push(DiscreteVariable("NA Transformation",DiscretePerturbationVector(1u,rpert)));

  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestCreateWarningMsgs")).get();
  runmanager::RubyJobBuilder rjb(measure);
  rjb.setIncludeDir(toPath(rubyOpenStudioDir()));
  runmanager::WorkItem workItem = rjb.toWorkItem();
  problem.push(workItem);

  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestMissingARequiredArgWithNoDefault")).get();
  rpert = RubyPerturbation(measure);
  problem.push(DiscreteVariable("Missing Argument",DiscretePerturbationVector(1u,rpert)));

  // should never get to here b/c previous variable should fail
  measure = BCLMeasure::load(resourcesPath() / toPath("utilities/BCL/Measures/TestFlagAsNotApplicable")).get();
  rjb = runmanager::RubyJobBuilder(measure);
  rjb.setIncludeDir(toPath(rubyOpenStudioDir()));
  workItem = rjb.toWorkItem();
  problem.push(workItem);

  return problem;
}
