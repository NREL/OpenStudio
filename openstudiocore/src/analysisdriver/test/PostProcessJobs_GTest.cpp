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

#include "../AnalysisDriver.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../project/ProjectDatabase.hpp"
#include "../../project/DataPointRecord.hpp"
#include "../../project/FileReferenceRecord.hpp"
#include "../../project/AttributeRecord.hpp"

#include "../../analysis/Problem.hpp"
#include "../../analysis/LinearFunction.hpp"
#include "../../analysis/OutputAttributeVariable.hpp"
#include "../../analysis/Analysis.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DDACEAlgorithmOptions.hpp"
#include "../../analysis/DDACEAlgorithm.hpp"
#include "../../analysis/InputVariable.hpp"
#include "../../analysis/WorkflowStep.hpp"

#include "../../runmanager/lib/RubyJobUtils.hpp"
#include "../../runmanager/lib/WorkItem.hpp"

#include "../../osversion/VersionTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/WeatherFile.hpp"

#include "../../utilities/core/Finder.hpp"
#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/filetypes/EpwFile.hpp"

#include <OpenStudio.hxx>
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::osversion;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,PostProcessJobs_PostProcessSecondXML) {

  // CREATE MINIMAL PROBLEM
  openstudio::runmanager::Workflow simulationWorkflow;
  simulationWorkflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::EnergyPlusPreProcess);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
  simulationWorkflow.addJob(openstudio::runmanager::JobType::OpenStudioPostProcess);
  openstudio::path postProcessPath = resourcesPath() / toPath("analysisdriver/PostProcessSecondXML.rb");
  ASSERT_TRUE(boost::filesystem::exists(postProcessPath));
  RubyJobBuilder rjb;
  rjb.setScriptFile(postProcessPath);
  // ETH@20120914 This should not mess things up, but it does. The osm wants to bring its
  // weather file along, which wouldn't be a problem except that the weather file path ends
  // up being absolute and that absolute path ends up being tacked on to the job run directory
  // so that you end up with something like "...\4-Ruby-0\C:\projects\...". Not good.
  rjb.addInputFile(FileSelection::Last,
                   FileSource::All,
                   ".*\\.osm",
                   "in.osm");
  rjb.addInputFile(FileSelection::Last,
                   FileSource::All,
                   "eplusout.sql",
                   "eplusout.sql");
  rjb.addInputFile(FileSelection::Last,
                   FileSource::Parent,
                   ".*\\.xml",
                   "in.xml");
  rjb.setIncludeDir(toPath(rubyOpenStudioDir()));
  simulationWorkflow.addJob(rjb.toWorkItem());
  simulationWorkflow.add(ConfigOptions::makeTools(
      energyPlusExePath().parent_path(),
      openstudio::path(),
      openstudio::path(),
      rubyExePath().parent_path(),
      dakotaExePath().parent_path()));

  Problem problem("No Variables PostProcessSecondXML Workflow",VariableVector(),simulationWorkflow);

  // DEFINE SEED
  model::Model model = model::exampleModel();
  // set weather file
  openstudio::path weatherFilePath = energyPlusWeatherDataPath() / toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
  EpwFile epwFile(weatherFilePath);
  WeatherFile::setWeatherFile(model,epwFile);
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  Analysis analysis("Post-Process Second XML",
                    problem,
                    seedModel);

  // ADD DATAPOINT TO RUN
  OptionalDataPoint oDataPoint = problem.createDataPoint(std::vector<QVariant>());
  ASSERT_TRUE(oDataPoint);
  EXPECT_FALSE(oDataPoint->topLevelJob());
  analysis.addDataPoint(*oDataPoint);
  EXPECT_FALSE(oDataPoint->topLevelJob());

  // CREATE DRIVER
  ProjectDatabase database = getCleanDatabase("PostProcessJobs_PostProcessSecondXML");
  AnalysisDriver analysisDriver(database);

  // RUN ANALYSIS
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  runOptions.setUrlSearchPaths(std::vector<URLSearchPath>(1u,URLSearchPath(energyPlusWeatherDataPath())));
  runOptions.setJobCleanUpBehavior(JobCleanUpBehavior::none); // DLM: there are sporadic failures when standard cleanup is used
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(oDataPoint->topLevelJob());
  analysisDriver.waitForFinished();
  EXPECT_TRUE(oDataPoint->topLevelJob());

  // CHECK DATAPOINT -- SHOULD HAVE floorArea AND peakAnnualDemand.
  ASSERT_EQ(1u,currentAnalysis.analysis().dataPoints().size());
  DataPoint dataPoint = currentAnalysis.analysis().dataPoints()[0];
  EXPECT_TRUE(dataPoint.topLevelJob());
  EXPECT_TRUE(dataPoint.isComplete());
  EXPECT_FALSE(dataPoint.failed());
  OptionalAttribute attribute = dataPoint.getOutputAttribute("floorArea");
  EXPECT_TRUE(attribute);
  if (attribute) {
    ASSERT_TRUE(attribute->valueType() == AttributeValueType::Double);
    LOG(Debug,"Floor area = " << attribute->valueAsDouble());
  }
  attribute = dataPoint.getOutputAttribute("peakAnnualDemand");
  EXPECT_TRUE(attribute);
  if (attribute) {
    ASSERT_TRUE(attribute->valueType() == AttributeValueType::Double);
    LOG(Debug,"Peak annual demand = " << attribute->valueAsDouble());
  }
  DataPointRecordVector dataPointRecords = DataPointRecord::getDataPointRecords(database);
  ASSERT_EQ(1u,dataPointRecords.size());
  DataPointRecord dataPointRecord = dataPointRecords[0];
  AttributeRecordVector attributeRecords = dataPointRecord.attributeRecords();
  NameFinder<AttributeRecord> finder("floorArea",true);
  AttributeRecordVector::const_iterator it = std::find_if(attributeRecords.begin(),
                                                          attributeRecords.end(),
                                                          finder);
  EXPECT_FALSE(it == attributeRecords.end());
  finder = NameFinder<AttributeRecord>("peakAnnualDemand",true);
  it = std::find_if(attributeRecords.begin(),attributeRecords.end(),finder);
  EXPECT_FALSE(it == attributeRecords.end());
}

TEST_F(AnalysisDriverFixture,PostProcessJobs_OpenStudioPostProcessAndResponses) {
  // RETRIEVE PROBLEM
  // add responses after choose them
  Problem problem = retrieveProblem("UserScriptContinuous",true,true);
  FunctionVector responses;
  responses.push_back(LinearFunction("Site Energy",
                                     VariableVector(1u,OutputAttributeVariable(
                                         "Site Energy",
                                         "Total Site Energy"))));
  responses.push_back(LinearFunction("Cooling Electricity",
                                     VariableVector(1u,OutputAttributeVariable(
                                         "Cooling Electricity",
                                         "EndUses.Electricity.Cooling.General"))));
  responses.push_back(LinearFunction("Heating Gas",
                                     VariableVector(1u,OutputAttributeVariable(
                                         "Heating Gas",
                                         "EndUses.Gas.Heating.General"))));
  responses.push_back(LinearFunction("Floor Area",
                                     VariableVector(1u,OutputAttributeVariable(
                                         "Floor Area",
                                         "floorArea"))));
  problem = Problem(problem.name(),
                    problem.workflow(),
                    responses);

  // DEFINE SEED
  Model model = model::exampleModel();
  EpwFile epwFile(energyPlusWeatherDataPath() / toPath("USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw"));
  WeatherFile::setWeatherFile(model,epwFile);
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  Analysis analysis("DDACE Latin Hypercube Sampling - UserScriptContinuousWithResponses",
                    problem,
                    seedModel);

  // CREATE A DATAPOINT TO RUN
  std::vector<QVariant> values;
  values.push_back(QVariant(double(0.2))); // wwr
  values.push_back(QVariant(double(1.0))); // offset of windows from floor
  values.push_back(QVariant(double(1.0))); // overhang projection factor
  OptionalDataPoint oDataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(oDataPoint);
  bool test = analysis.addDataPoint(*oDataPoint);
  EXPECT_TRUE(test);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("PostProcessJobs_OpenStudioPostProcessAndResponses");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());

  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    EXPECT_EQ(problem.responses().size(),dataPoint.responseValues().size());
  }
}
