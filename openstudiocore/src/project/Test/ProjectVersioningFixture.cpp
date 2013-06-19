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

#include <project/Test/ProjectVersioningFixture.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>

#include <analysis/Problem.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/ModelRulesetContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/LinearFunction.hpp>
#include <analysis/OutputAttributeVariable.hpp>
#include <analysis/FSUDaceAlgorithm.hpp>
#include <analysis/FSUDaceAlgorithmOptions.hpp>
#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>

#include <runmanager/lib/Workflow.hpp>
#include <runmanager/Test/ToolBin.hxx>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>
#include <ruleset/OSArgument.hpp>

#include <utilities/idd/IddEnums.hxx>

#include <utilities/data/Tag.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/SIUnit.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/PathHelpers.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <boost/foreach.hpp>

#include <stdlib.h>

using namespace openstudio;
using namespace openstudio::project;

boost::optional<openstudio::FileLogSink> ProjectVersioningFixture::logFile;
openstudio::UUID ProjectVersioningFixture::uuid = createUUID();

void ProjectVersioningFixture::SetUp() {}

void ProjectVersioningFixture::TearDown() {}

void ProjectVersioningFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./ProjectVersioningFixture.log"));
  logFile->setLogLevel(Info);
  Logger::instance().standardOutLogger().disable();

  // set up folder for data
  if (!boost::filesystem::exists(toPath("ProjectVersioningFixtureData"))) {
    boost::filesystem::create_directory(toPath("ProjectVersioningFixtureData"));
  }

  // create test ProjectDatabase of latest version
  createTestCaseDatabases();
}

void ProjectVersioningFixture::TearDownTestCase() {
  logFile->disable();
  boost::filesystem::remove_all(toPath("ProjectVersioningFixtureData") / toPath(toString(uuid)) );
}

std::vector<openstudio::path> ProjectVersioningFixture::projectDatabasePaths(
    openstudio::path basePath)
{
  std::vector<openstudio::path> result;

  if (basePath.empty()) {
    basePath = toPath("ProjectVersioningFixtureData") / toPath(toString(uuid));
  }

  for (openstudio::directory_iterator it(basePath);
       it != openstudio::directory_iterator(); ++it)
  {
    if (getFileExtension(it->path()) == "osp") {
      result.push_back(it->path());
    }
  }
  return result;
}

std::vector<openstudio::path> ProjectVersioningFixture::copyProjectDatabases(
    const std::string& folderName) const
{
  std::vector<openstudio::path> paths = projectDatabasePaths();
  openstudio::path testFolder = paths[0].parent_path().parent_path() / toPath(folderName);
  boost::filesystem::create_directory(testFolder);
  std::vector<openstudio::path> testPaths;
  BOOST_FOREACH(const openstudio::path& p, paths) {
    testPaths.push_back(testFolder/p.filename());
    boost::filesystem::copy_file(p,testPaths.back(),
                                 boost::filesystem::copy_option::overwrite_if_exists);
    boost::filesystem::copy_file(setFileExtension(p,"db",true,false),
                                 setFileExtension(testPaths.back(),"db",true,false),
                                 boost::filesystem::copy_option::overwrite_if_exists);
  }
  return testPaths;
}

void ProjectVersioningFixture::createTestCaseDatabases() {
  // copy existing databases to unique folder
  std::vector<openstudio::path> paths = projectDatabasePaths(resourcesPath()/toPath("project/version"));
  openstudio::path folder = toPath("ProjectVersioningFixtureData") / toPath(toString(uuid));
  boost::filesystem::create_directory(folder);
  BOOST_FOREACH(const openstudio::path& p, paths) {
    boost::filesystem::copy_file(p,folder/p.filename());
    boost::filesystem::copy_file(setFileExtension(p,"db",true,false),
                                 setFileExtension(folder/p.filename(),"db",true,false));
  }

  // make database for new version
  ProjectDatabase database(projectDatabasePath(openStudioVersion()),true);
  addAnalysis(database);
  database.save();
}

openstudio::path ProjectVersioningFixture::projectDatabasePath(const std::string& versionString) {
  return toPath("ProjectVersioningFixtureData") /
         toPath(toString(uuid)) /
         toPath("project_" + boost::regex_replace(versionString,boost::regex("\\."),"_") + ".osp");
}

void ProjectVersioningFixture::addAnalysis(ProjectDatabase& database) {
  // Add an analysis to the database. Not trying to be a real example, just trying to hit a lot of
  // the possible classes that can be saved to make sure they carry through during version updates.

  // create a Problem
  analysis::VariableVector variables;

  analysis::DiscretePerturbationVector perturbations;

  // discrete variable
  perturbations.push_back(analysis::NullPerturbation());
  analysis::RubyPerturbation rubyPerturbation(toPath("myScript.rb"),
                                              FileReferenceType::OSM,
                                              FileReferenceType::OSM,
                                              false,
                                              false);
  rubyPerturbation.addArgument("cost","50.00");
  rubyPerturbation.addArgument("size","102.11");
  perturbations.push_back(rubyPerturbation);
  rubyPerturbation = analysis::RubyPerturbation(toPath("myUserScript.rb"),
                                                FileReferenceType::OSM,
                                                FileReferenceType::OSM,
                                                true);
  ruleset::OSArgument arg = ruleset::OSArgument::makeBoolArgument("allOrNothing");
  arg.setValue(true);
  rubyPerturbation.addArgument(arg);
  arg = ruleset::OSArgument::makeDoubleArgument("thickness");
  arg.setValue(0.6207);
  rubyPerturbation.addArgument(arg);
  arg = ruleset::OSArgument::makeQuantityArgument("height");
  arg.setValue(Quantity(53.2,createSILength()));
  rubyPerturbation.addArgument(arg);
  perturbations.push_back(rubyPerturbation);
  ruleset::ModelRuleset ruleset("My Ruleset");
  ruleset::ModelRule rule("My Rule");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_Surface)));
  rule.add(ruleset::ModelObjectFilterNumericAttribute(
      "uFactor",
      ruleset::RulesetNumericalPredicate(ruleset::RulesetNumericalPredicate::GreaterThan),
      0.5));
  rule.add(ruleset::ModelObjectActionSetRelationship("construction",toPath("myConstruction.osc")));
  ruleset.add(rule);
  perturbations.push_back(analysis::ModelRulesetPerturbation(ruleset));
  variables.push_back(analysis::DiscreteVariable("Hodgepodge",perturbations));

  // model ruleset continuous variable
  ruleset::ModelObjectFilterClauseVector filters;
  filters.push_back(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_Lights_Definition)));
  filters.push_back(ruleset::ModelObjectFilterStringAttribute("designLevelCalculationMethod",
                                                              ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::IEquals),
                                                              "designLevel"));
  analysis::ModelRulesetContinuousVariable rulesetVariable("Lighting Power",filters,"designLevel");
  rulesetVariable.setMinimum(10.0);
  rulesetVariable.setMaximum(1000.0);
  variables.push_back(rulesetVariable);

  // ruby continuous variable
  rubyPerturbation = analysis::RubyPerturbation(toPath("myUserScript.rb"),
                                                FileReferenceType::OSM,
                                                FileReferenceType::OSM,
                                                true);
  arg = ruleset::OSArgument::makeDoubleArgument("thickness");
  variables.push_back(analysis::RubyContinuousVariable("Thickness",arg,rubyPerturbation));
  arg = ruleset::OSArgument::makeQuantityArgument("height");
  variables.push_back(analysis::RubyContinuousVariable("Height",arg,rubyPerturbation));

  analysis::FunctionVector responses;
  analysis::OutputAttributeContinuousVariable outputVariable("Energy Use","Site Energy Use");
  responses.push_back(analysis::LinearFunction("Energy",
                                               analysis::VariableVector(1u,outputVariable)));

  runmanager::Workflow simulationWorkflow;
  simulationWorkflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::EnergyPlusPreProcess);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::OpenStudioPostProcess);

  // add tools
  simulationWorkflow.add(openstudio::runmanager::ConfigOptions::makeTools(
      energyPlusExePath().parent_path(),
      openstudio::path(),
      openstudio::path(),
      rubyExePath().parent_path(),
      dakotaExePath().parent_path()));

  analysis::Problem problem("My Problem",variables,responses,simulationWorkflow);

  // create an Algorithm
  analysis::FSUDaceAlgorithmOptions options(analysis::FSUDaceAlgorithmType::halton);
  options.setFixedSeed(true);
  options.setNumTrials(10000);
  options.setSamples(100);
  options.setSequenceLeap(IntVector(problem.numVariables(),3));
  analysis::FSUDaceAlgorithm algorithm(options);

  // create an Analysis
  analysis::Analysis analysis("My Test Analysis",problem,algorithm,FileReference(toPath("seed.osm")));

  // add some DataPoints
  for (int i = 0; i < 100; ++i) {
    int n = problem.numVariables();
    std::vector<QVariant> values(n);
    for (int j = 0; j < n; ++j) {
      if (analysis::OptionalDiscreteVariable dv = variables[j].optionalCast<analysis::DiscreteVariable>()) {
        int value = int(floor(double(rand()) * double(dv->numPerturbations(false)) / double(RAND_MAX)));
        if (value == int(dv->numPerturbations(false))) { --value; }
        values[j] = QVariant(value);
      }
      else {
        values[j] = QVariant(double(rand()) / double(RAND_MAX));
      }
    }
    analysis::DataPoint dataPoint(createUUID(),
                                  createUUID(),
                                  "",
                                  "",
                                  "",
                                  true,
                                  false,
                                  problem,
                                  values,
                                  DoubleVector(1u,double(rand()) * 10000.0 / double(RAND_MAX)),
                                  toPath("C:/projectPath/aDataPointPath/"),
                                  OptionalFileReference(),
                                  OptionalFileReference(),
                                  OptionalFileReference(),
                                  OptionalFileReference(),
                                  TagVector(1u,Tag("alg1")),
                                  boost::none,
                                  std::vector<openstudio::path>()); // DLM: Elaine is this ok?
    analysis.addDataPoint(dataPoint);
  }

  project::AnalysisRecord analysisRecord(analysis,database);
}
