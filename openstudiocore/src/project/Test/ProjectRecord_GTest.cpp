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

#include "../ProjectDatabase.hpp"
#include "../ProjectRecord.hpp"
#include "../AnalysisRecord.hpp"
#include "../SimpleAnalysisRecord.hpp"
#include "../SimpleAnalysisRecord_Impl.hpp"
#include "../WorkflowRecord.hpp"
#include "../ModelRecord.hpp"
#include "../BaselineModelRecord.hpp"
#include "../ModelAttributeRecord.hpp"

#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Workflow.hpp"
#include "../../runmanager/lib/RubyJob.hpp"
#include "../../runmanager/lib/RubyJobUtils.hpp"
#include <runmanager/Test/EnergyPlusBin.hxx>

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/core/String.hpp"
#include "../../utilities/core/Optional.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>

using namespace openstudio;
using namespace openstudio::project;

TEST(Project, ProjectRecord)
{
  openstudio::path outdir = openstudio::toPath(".");
  openstudio::path runPath = outdir / openstudio::toPath("run.db");
  if(boost::filesystem::exists(runPath)){
    boost::filesystem::remove(runPath);
  }
  openstudio::runmanager::RunManager runManager(runPath, true, false, false);

  openstudio::path projectPath = outdir / openstudio::toPath("project.osp");
  if(boost::filesystem::exists(projectPath)){
    boost::filesystem::remove(projectPath);
  }
  ProjectDatabase database(projectPath, runManager, true);


  openstudio::path modelPath = outdir / openstudio::toPath("ProjectRecordTest.osm");
  openstudio::model::exampleModel().save(modelPath, true);

  ProjectRecord project("project", database);
  EXPECT_EQ("project", project.name());
  EXPECT_EQ("project", project.displayName());
  EXPECT_EQ("", project.description());

  EXPECT_EQ(static_cast<unsigned>(0), project.analysisRecords().size());

  SimpleAnalysisRecord analysis("analysis", project);

  EXPECT_EQ(static_cast<unsigned>(1), project.analysisRecords().size());
}

TEST(Project, ProjectRecord_Example)
{
  openstudio::path outdir = openstudio::toPath(".");

  openstudio::path runPath = outdir / openstudio::toPath("run.db");
  if(exists(runPath)){
    remove(runPath);
  }

  openstudio::path projectPath = outdir / openstudio::toPath("project.osp");
  if(exists(projectPath)){
    remove(projectPath);
  }

  openstudio::path model1Path = outdir / openstudio::toPath("ProjectRecord_ExampleTest.osm");
  openstudio::model::exampleModel().save(modelPath, true);

  openstudio::path model2Path = outdir / openstudio::toPath("ProjectRecord_ExampleTest2.osm");

  if(exists(model2Path)){
    remove(model2Path);
  }
  boost::filesystem::copy_file(model1Path, model2Path);
  remove(model1Path);
  boost::filesystem::copy_file(model2Path, model1Path);

  // limit scope so database is closed during test
  {
    openstudio::runmanager::RunManager runManager(runPath, true, false, false);
    ProjectDatabase database(projectPath, runManager, true);
    EXPECT_EQ(static_cast<unsigned>(0), database.projectRecords().size());

    // configure workflow to include user defined ruby post process job
    openstudio::runmanager::Workflow rmWorkflow;
    rmWorkflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
    //rmWorkflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
    rmWorkflow.add(openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), rubyExePath().parent_path()));
      
    //Create a new ruby job
    openstudio::runmanager::RubyJobBuilder rubyJobBuilder;
    openstudio::path rubyScriptFile = toPath( rubyLibDir() + "openstudio/runmanager/rubyscripts/PostProcess.rb" );
    rubyJobBuilder.setScriptFile(rubyScriptFile);
    //rubyjobbuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
    //                            openstudio::runmanager::FileSource("All"),
    //                            "eplusout\\.sql",
    //                            "eplusout.sql")
    rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
                                openstudio::runmanager::FileSource("All"),
                                ".*\\.osm",
                                "in.osm");
    rubyJobBuilder.addToolArgument("-I" + rubyOpenStudioDir()) ;
    rmWorkflow.addJob(rubyJobBuilder.toWorkItems());

    ProjectRecord project("Project", database);
    WorkflowRecord workflow(rmWorkflow, database);
    SimpleAnalysisRecord analysis("analysis", project);
    analysis.setWorkflowRecord(workflow);
    ModelRecord model1("model1", model1Path, analysis);
    ModelRecord model2("model2", model2Path, analysis);

    BaselineModelRecord baselineModel("BaselineModel", analysis);
    EXPECT_TRUE(baselineModel.addModelRecord(model1));

    database.save();
  }

  // close the database above before running, reopen here, and wait to finish in this scope
  {
    boost::optional<ProjectDatabase> database = ProjectDatabase::open(projectPath);
    ASSERT_TRUE(database);
    ASSERT_EQ(static_cast<unsigned>(1), database->projectRecords().size());
    ASSERT_EQ(static_cast<unsigned>(1), database->projectRecords()[0].analysisRecords().size());
    boost::optional<SimpleAnalysisRecord> simpleAnalysis = database->projectRecords()[0].analysisRecords()[0].optionalCast<SimpleAnalysisRecord>();
    ASSERT_TRUE(simpleAnalysis);
    EXPECT_TRUE(simpleAnalysis->run());
    EXPECT_TRUE(simpleAnalysis->waitForFinished());
  }

  // database has been closed after running
  {
    boost::optional<ProjectDatabase> database = ProjectDatabase::open(projectPath);
    ASSERT_TRUE(database);
    ASSERT_EQ(static_cast<unsigned>(1), database->projectRecords().size());
    ASSERT_EQ(static_cast<unsigned>(1), database->projectRecords()[0].analysisRecords().size());
    ASSERT_EQ(static_cast<unsigned>(2), database->projectRecords()[0].analysisRecords()[0].modelRecords().size());
    ModelRecord model1 = database->projectRecords()[0].analysisRecords()[0].modelRecords()[0];
    ModelRecord model2 = database->projectRecords()[0].analysisRecords()[0].modelRecords()[1];
    EXPECT_NE(model1.path(), model2.path());

    boost::optional<SimpleAnalysisRecord> simpleAnalysis = database->projectRecords()[0].analysisRecords()[0].optionalCast<SimpleAnalysisRecord>();
    ASSERT_TRUE(simpleAnalysis);
    ASSERT_TRUE(simpleAnalysis->workflowRecord());
    WorkflowRecord workflow = simpleAnalysis->workflowRecord().get();

    // can now get results from each model
    EXPECT_TRUE(model1.modelAttributeRecords().size() > 0);
    boost::optional<ModelAttributeRecord> attributeRecord = model1.getModelAttributeRecord("numberOfZones");
    ASSERT_TRUE(attributeRecord);
    Attribute attribute = attributeRecord->attribute();
    ASSERT_EQ(AttributeValueType::Integer, attribute.valueType().value());
    EXPECT_EQ(static_cast<unsigned>(6), attribute.valueAsInteger());
    EXPECT_FALSE(attribute.units());

    EXPECT_TRUE(model2.modelAttributeRecords().size() > 0);
    attributeRecord = model2.getModelAttributeRecord("numberOfZones");
    ASSERT_TRUE(attributeRecord);
    attribute = attributeRecord->attribute();
    ASSERT_EQ(AttributeValueType::Integer, attribute.valueType().value());
    EXPECT_EQ(static_cast<unsigned>(6), attribute.valueAsInteger());
    EXPECT_FALSE(attribute.units());

    // if running energyplus
    /*
    boost::optional<ModelAttribute> attribute = model.getAttribute("netSiteEnergy", AttributeValueType::Double, workflow);
    ASSERT_TRUE(attribute);
    ASSERT_EQ(AttributeValueType::Double, attribute->valueType().value());
    EXPECT_TRUE(attribute->valueAsDouble() > 0);
    ASSERT_TRUE(attribute->units());
    EXPECT_EQ("GJ", attribute->units().get());

    attribute = model.getAttribute("netSourceEnergy", AttributeValueType::Double, workflow);
    ASSERT_TRUE(attribute);
    ASSERT_EQ(AttributeValueType::Double, attribute->valueType().value());
    EXPECT_TRUE(attribute->valueAsDouble() > 0);
    ASSERT_TRUE(attribute->units());
    EXPECT_EQ("GJ", attribute->units().get());
    */
  } 

}

TEST(Project, ProjectRecord_Transactions)
{
  openstudio::path runPath = openstudio::toPath("./run.db");
  if(boost::filesystem::exists(runPath)){
    boost::filesystem::remove(runPath);
  }
  openstudio::runmanager::RunManager runManager(runPath, true, false, false);

  openstudio::path projectPath = openstudio::toPath("./project.osp");
  if(boost::filesystem::exists(projectPath)){
    boost::filesystem::remove(projectPath);
  }
  ProjectDatabase database(projectPath, runManager, true);

  EXPECT_FALSE(database.commitTransaction());

  EXPECT_TRUE(database.startTransaction());
  EXPECT_FALSE(database.startTransaction());
  ProjectRecord project1("project 1", database);
  EXPECT_EQ(1u, ProjectRecord::getProjectRecords(database).size());
  EXPECT_TRUE(database.commitTransaction());
  EXPECT_FALSE(database.commitTransaction());
  EXPECT_EQ(1u, ProjectRecord::getProjectRecords(database).size());

  EXPECT_TRUE(database.startTransaction());
  ProjectRecord project2("project 2", database);
  EXPECT_EQ(2u, ProjectRecord::getProjectRecords(database).size());
  EXPECT_TRUE(database.save());
  EXPECT_EQ(2u, ProjectRecord::getProjectRecords(database).size());
  EXPECT_TRUE(database.commitTransaction());
  EXPECT_EQ(2u, ProjectRecord::getProjectRecords(database).size());

  EXPECT_TRUE(database.startTransaction());
  ProjectRecord project3("project 3", database);
  EXPECT_EQ(3u, ProjectRecord::getProjectRecords(database).size());
  EXPECT_TRUE(database.commitTransaction());
  EXPECT_EQ(3u, ProjectRecord::getProjectRecords(database).size());

  EXPECT_TRUE(database.save());
  EXPECT_EQ(3u, ProjectRecord::getProjectRecords(database).size());
}
