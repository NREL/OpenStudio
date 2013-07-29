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

#include <gtest/gtest.h>
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>
#include <runmanager/lib/JobFactory.hpp>

#include <utilities/data/Attribute.hpp>
#include <utilities/data/Tag.hpp>

#include <utilities/time/Date.hpp>

#include <utilities/units/QuantityFactory.hpp>

#include <utilities/core/Containers.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::runmanager;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PreRun) {
  Analysis analysis = analysis1();
  Problem problem = analysis.problem();

  // Create data point and add to analysis
  std::vector<QVariant> values;
  values.push_back(1);
  values.push_back(0.3);
  values.push_back(0.9);
  OptionalDataPoint dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  EXPECT_TRUE(analysis.addDataPoint(*dataPoint));

  // Serialize data point
  std::string json = dataPoint->toJSON();
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  OptionalAnalysisObject copyAsAnalysisObject = loadJSON(json);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  DataPoint copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_pre_run.json");
  EXPECT_TRUE(dataPoint->saveJSON(p,true));

  // Load and check results
  copyAsAnalysisObject = loadJSON(p);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PostRun) {
  // Create problem
  Analysis analysis = analysis1();
  Problem problem = analysis.problem();

  // Create "simulated" data point
  std::vector<QVariant> values;
  values.push_back(1);
  values.push_back(0.3);
  values.push_back(0.9);
  DoubleVector responseValues;
  responseValues.push_back(58.281967);
  responseValues.push_back(718952.281);
  responseValues.push_back(0.3);
  TagVector tags;
  tags.push_back(Tag("custom"));
  tags.push_back(Tag("faked"));
  // attributes
  AttributeVector attributes;
  attributes.push_back(Attribute("electricity.Cooling",281.281567,"kWh"));
  attributes.push_back(Attribute("electricity.Lighting",19206.291876,"kWh"));
  attributes.push_back(Attribute("electricity.Equipment",5112.125718,"kWh"));
  attributes = AttributeVector(1u,Attribute("enduses.electric",attributes));
  attributes.push_back(Attribute("eui",createQuantity(128.21689,"kBtu/ft^2").get()));
  // complete job
  // 1. get vector of work items
  std::vector<WorkItem> workItems;
  // 0
  workItems.push_back(problem.variables()[0].cast<MeasureGroup>().createWorkItem(QVariant(1),
                                                                                 toPath(rubyOpenStudioDir())));
  RubyContinuousVariable rcv = problem.variables()[1].cast<RubyContinuousVariable>();
  RubyMeasure rm = rcv.measure();
  OSArgument arg = rcv.argument().clone();
  arg.setValue(values[1].toDouble());
  rm.setArgument(arg);
  rcv = problem.variables()[2].cast<RubyContinuousVariable>();
  arg = rcv.argument().clone();
  arg.setValue(values[2].toDouble());
  rm.setArgument(arg);
  // 1
  workItems.push_back(rm.createWorkItem(toPath(rubyOpenStudioDir())));
  // 2
  workItems.push_back(WorkItem(JobType::ModelToIdf));
  // 3
  workItems.push_back(WorkItem(JobType::EnergyPlusPreProcess));
  // 4
  workItems.push_back(WorkItem(JobType::EnergyPlus));
  // 5
  workItems.push_back(WorkItem(JobType::OpenStudioPostProcess));
  // 2. step through work items and create jobs with results
  WorkItem wi = workItems[5];
  std::vector<std::pair<ErrorType, std::string> > errors;
  errors.push_back(std::make_pair(ErrorType::Info,"Post-process completed successfully."));
  JobErrors errorsObject(OSResultValue::Success,errors);
  std::vector<FileInfo> outFiles;
  outFiles.push_back(FileInfo("report.xml",
                              DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,34,21)),
                              "",
                              toPath("myProject/fakeDataPoint/75-OpenStudioPostProcess-0/report.xml")));
  Files outFilesObject(outFiles);
  Job job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,34,21)),
        errorsObject,
        outFilesObject); // OpenStudioPostProcess

  Job jobLast = job;
  wi = workItems[4];
  errors.clear();
  errors.push_back(std::make_pair(ErrorType::Warning,"ENERGYPLUS WARNING: ..."));
  errors.push_back(std::make_pair(ErrorType::Warning,"ENERGYPLUS WARNING: ..."));
  errors.push_back(std::make_pair(ErrorType::Warning,"ENERGYPLUS WARNING: ..."));
  errorsObject = JobErrors(OSResultValue::Success,errors);
  outFiles.clear();
  outFiles.push_back(FileInfo("eplusout.sql",
                              DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,33,32)),
                              "",
                              toPath("myProject/fakeDataPoint/74-EnergyPlus-0/eplusout.sql")));
  outFiles.push_back(FileInfo("eplusout.err",
                              DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,33,34)),
                              "",
                              toPath("myProject/fakeDataPoint/74-EnergyPlus-0/eplusout.err")));
  outFilesObject = Files(outFiles);
  job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,33,42)),
        errorsObject,
        outFilesObject); // EnergyPlus
  job.addChild(jobLast);

  jobLast = job;
  wi = workItems[3];
  errors.clear();
  errorsObject = JobErrors(OSResultValue::Success,errors);
  outFiles.clear();
  outFiles.push_back(FileInfo("out.idf",
                              DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,23,05)),
                              "",
                              toPath("myProject/fakeDataPoint/73-EnergyPlusPreProcess-0/out.idf")));
  outFilesObject = Files(outFiles);
  job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,23,12)),
        errorsObject,
        outFilesObject); // EnergyPlusPreProcess
  job.addChild(jobLast);

  jobLast = job;
  wi = workItems[2];
  errors.clear();
  errors.push_back(std::make_pair(ErrorType::Info,"Did not find ScheduleTypeLimits for Schedule ..."));
  errors.push_back(std::make_pair(ErrorType::Warning,"Unexpectedly did not find a child object of a certain type, replaced with a default one."));
  errorsObject = JobErrors(OSResultValue::Success,errors);
  outFiles.clear();
  outFiles.push_back(FileInfo("out.idf",
                              DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,22,30)),
                              "",
                              toPath("myProject/fakeDataPoint/72-ModelToIdf-0/out.idf")));
  outFilesObject = Files(outFiles);
  job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,22,32)),
        errorsObject,
        outFilesObject); // ModelToIdf
  job.addChild(jobLast);

  jobLast = job;
  wi = workItems[1];
  errors.clear();
  errors.push_back(std::make_pair(ErrorType::InitialCondition,"Started with a window to wall ratio of ..."));
  errors.push_back(std::make_pair(ErrorType::FinalCondition,"Set the window to wall ratio ..."));
  errorsObject = JobErrors(OSResultValue::Success,errors);
  outFiles.clear();
  outFilesObject = Files(outFiles);
  wi.params.append("outdir","myProject/fakeDataPoint/");
  job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,21,52)),
        errorsObject,
        outFilesObject); // Variables 2 & 3
  job.addChild(jobLast);

  jobLast = job;
  wi = workItems[0];
  errors.clear();
  errors.push_back(std::make_pair(ErrorType::InitialCondition,"Started with a window to wall ratio of ..."));
  errors.push_back(std::make_pair(ErrorType::FinalCondition,"Set the window to wall ratio ..."));
  errorsObject = JobErrors(OSResultValue::Success,errors);
  outFiles.clear();
  outFilesObject = Files(outFiles);
  job = JobFactory::createJob(
        wi.type,
        wi.tools,
        wi.params,
        wi.files,
        std::vector<openstudio::URLSearchPath>(),
        false,
        createUUID(),
        DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,21,10)),
        errorsObject,
        outFilesObject); // Variable 1
  job.addChild(jobLast);

  DataPoint dataPoint(createUUID(),
                      createUUID(),
                      "fakeDataPoint",
                      "Fake Data Point",
                      "Demonstrating json serialization of complete DataPoint.",
                      problem,
                      true,
                      false,
                      values,
                      responseValues,
                      toPath("myProject/fakeDataPoint/"),
                      FileReference(toPath("myProject/fakeDataPoint/71-Ruby-0/out.osm")),
                      FileReference(toPath("myProject/fakeDataPoint/72-ModelToIdf-0/out.idf")),
                      FileReference(toPath("myProject/fakeDataPoint/74-EnergyPlus-0/eplusout.sql")),
                      FileReference(toPath("myProject/fakeDataPoint/75-OpenStudioPostProcess-0/report.xml")),
                      job,
                      std::vector<openstudio::path>(),
                      tags,
                      attributes);
  EXPECT_TRUE(analysis.addDataPoint(dataPoint));

  // Serialize data point
  std::string json = dataPoint.toJSON();
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  OptionalAnalysisObject copyAsAnalysisObject = loadJSON(json);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  DataPoint copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_post_run.json");
  EXPECT_TRUE(dataPoint.saveJSON(p,true));

  // Load and check results
  copyAsAnalysisObject = loadJSON(p);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

}
