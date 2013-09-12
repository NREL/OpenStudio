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

#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/LinearFunction.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>
#include <analysis/NormalDistribution.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/OutputAttributeVariable.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable_Impl.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/RubyMeasure_Impl.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/TriangularDistribution.hpp>

#include <runmanager/lib/JobFactory.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/data/Tag.hpp>
#include <utilities/units/QuantityFactory.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;

void AnalysisFixture::SetUp() {}

void AnalysisFixture::TearDown() {}

void AnalysisFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./AnalysisFixture.log"));
  logFile->setLogLevel(Debug);  
  openstudio::Logger::instance().standardOutLogger().disable();

  // set up data folder
  if (!boost::filesystem::exists(toPath("AnalysisFixtureData"))) {
    boost::filesystem::create_directory(toPath("AnalysisFixtureData"));
  }
}

void AnalysisFixture::TearDownTestCase() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> AnalysisFixture::logFile;

openstudio::analysis::Analysis AnalysisFixture::analysis1(AnalysisState state) {
  // Create problem and analysis
  Problem problem("My Problem");

  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/SetWindowToWallRatioByFacade"));
  RubyMeasure measure(bclMeasure);

  // Measure Group
  StringVector choices;
  choices.push_back("North");
  choices.push_back("South");
  choices.push_back("East");
  choices.push_back("West");
  OSArgument facade = OSArgument::makeChoiceArgument("facade",choices);
  OSArgument arg = facade.clone();
  arg.setValue("South");
  measure.setArgument(arg);
  OSArgument wwr = OSArgument::makeDoubleArgument("wwr");
  MeasureVector measures(1u,NullMeasure());
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.1);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.2);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.3);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  problem.push(MeasureGroup("South Windows",measures));

  // Continuous Variables Attached to Arguments
  arg = facade.clone();
  arg.setValue("North");
  measure.setArgument(arg);
  arg = wwr.clone();
  RubyContinuousVariable wwrCV("Window to Wall Ratio",arg,measure);
  wwrCV.setMinimum(0.0);
  wwrCV.setMaximum(1.0);
  TriangularDistribution td(0.2,0.0,0.5);
  wwrCV.setUncertaintyDescription(td);
  problem.push(wwrCV);
  OSArgument offset = OSArgument::makeDoubleArgument("offset");
  RubyContinuousVariable offsetCV("Offset",offset,measure);
  offsetCV.setMinimum(0.0);
  offsetCV.setMaximum(1.5);
  NormalDistribution nd(0.9,0.05);
  offsetCV.setUncertaintyDescription(nd);
  problem.push(offsetCV);

  // Simulation
  problem.push(WorkItem(JobType::ModelToIdf));
  problem.push(WorkItem(JobType::EnergyPlusPreProcess));
  problem.push(WorkItem(JobType::EnergyPlus));
  problem.push(WorkItem(JobType::OpenStudioPostProcess));

  // Responses
  LinearFunction response1("Energy Use Intensity",
                           VariableVector(1u,OutputAttributeVariable("EUI","site.eui")));
  problem.pushResponse(response1);
  VariableVector vars;
  vars.push_back(OutputAttributeVariable("Heating Energy","heating.energy.gas"));
  vars.push_back(OutputAttributeVariable("Cooling Energy","cooling.energy.elec"));
  DoubleVector coeffs;
  coeffs.push_back(1.0); // approx. source factor
  coeffs.push_back(2.5); // approx. source factor
  LinearFunction response2("Approximate Source Energy",vars,coeffs);
  problem.pushResponse(response2);
  LinearFunction response3("North WWR",VariableVector(1u,wwrCV)); // input variable as output
  problem.pushResponse(response3);

  Analysis analysis("My Analysis",problem,FileReferenceType::OSM);

  if (state == PreRun) {
    // Add three DataPoints
    std::vector<QVariant> values;
    values.push_back(0);
    values.push_back(0.2);
    values.push_back(0.9);
    OptionalDataPoint dataPoint = problem.createDataPoint(values);
    analysis.addDataPoint(*dataPoint);
    values[0] = 1; values[1] = 0.21851789; values[2] = 1.1681938;
    dataPoint = problem.createDataPoint(values);
    analysis.addDataPoint(*dataPoint);
    values[0] = 2; values[1] = 0.0; values[2] = 0.581563892;
    dataPoint = problem.createDataPoint(values);
    analysis.addDataPoint(*dataPoint);
  }
  else {
    // state == PostRun
    // Add one complete DataPoint
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
    std::vector<FileInfo> inFiles = wi.files.files();
    inFiles.push_back(FileInfo("eplusout.sql",
                               DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,33,32)),
                               "",
                               toPath("myProject/fakeDataPoint/75-OpenStudioPostProcess-0/eplusout.sql")));                             
    Files inFilesObject(inFiles);
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
          inFilesObject,
          std::vector<openstudio::URLSearchPath>(),
          false,
          createUUID(),
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,34,21)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // OpenStudioPostProcess

    Job jobLast = job;
    wi = workItems[4];
    inFiles = wi.files.files();
    inFiles.push_back(FileInfo("in.idf",
                                DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,23,05)),
                                "",
                                toPath("myProject/fakeDataPoint/74-EnergyPlus-0/in.idf")));
    inFilesObject = Files(inFiles);
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
          inFilesObject,
          std::vector<openstudio::URLSearchPath>(),
          false,
          createUUID(),
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,33,42)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // EnergyPlus
    job.addChild(jobLast);

    jobLast = job;
    wi = workItems[3];
    inFiles = wi.files.files();
    inFiles.push_back(FileInfo("in.idf",
                                DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,22,30)),
                                "",
                                toPath("myProject/fakeDataPoint/73-EnergyPlusPreProcess-0/in.idf")));
    inFilesObject = Files(inFiles);
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
          inFilesObject,
          std::vector<openstudio::URLSearchPath>(),
          false,
          createUUID(),
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,23,12)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // EnergyPlusPreProcess
    job.addChild(jobLast);

    jobLast = job;
    wi = workItems[2];
    inFiles = wi.files.files();
    inFiles.push_back(FileInfo("in.osm",
                                DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,22,01)),
                                "",
                                toPath("myProject/fakeDataPoint/72-ModelToIdf-0/in.osm")));
    inFilesObject = Files(inFiles);
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
          inFilesObject,
          std::vector<openstudio::URLSearchPath>(),
          false,
          createUUID(),
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,22,32)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // ModelToIdf
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
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,21,52)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // Variables 2 & 3
    job.addChild(jobLast);

    jobLast = job;
    wi = workItems[0];
    errors.clear();
    errors.push_back(std::make_pair(ErrorType::InitialCondition,"Started with a window to wall ratio of ..."));
    errors.push_back(std::make_pair(ErrorType::FinalCondition,"Set the window to wall ratio ..."));
    errorsObject = JobErrors(OSResultValue::Success,errors);
    outFiles.clear();
    outFilesObject = Files(outFiles);
    // add outdir job param
    JobParams params = wi.params;
    params.append("outdir","myProject/fakeDataPoint");
    job = JobFactory::createJob(
          wi.type,
          wi.tools,
          params,
          wi.files,
          std::vector<openstudio::URLSearchPath>(),
          false,
          createUUID(),
          JobState(
            DateTime(Date(MonthOfYear::Mar,21,2018),Time(0,8,21,10)),
            errorsObject,
            outFilesObject,
            AdvancedStatus())
          ); // Variable 1
    job.addChild(jobLast);

    DataPoint dataPoint(createUUID(),
                        createUUID(),
                        "fakeDataPoint",
                        "Fake Data Point",
                        "Demonstrating json serialization of complete DataPoint.",
                        problem,
                        true,
                        false,
                        true,
                        DataPointRunType::Local,
                        values,
                        responseValues,
                        toPath("myProject/fakeDataPoint/"),
                        FileReference(toPath("myProject/fakeDataPoint/71-Ruby-0/out.osm")),
                        FileReference(toPath("myProject/fakeDataPoint/72-ModelToIdf-0/out.idf")),
                        FileReference(toPath("myProject/fakeDataPoint/74-EnergyPlus-0/eplusout.sql")),
                        FileReferenceVector(1u,FileReference(toPath("myProject/fakeDataPoint/75-OpenStudioPostProcess-0/report.xml"))),
                        job,
                        std::vector<openstudio::path>(),
                        tags,
                        attributes);
    EXPECT_TRUE(analysis.addDataPoint(dataPoint));
  }

  return analysis;
}
