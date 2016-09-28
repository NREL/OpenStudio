/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

#include "../JobFactory.hpp"
#include "../EnergyPlusPostProcessJob.hpp"
#include "../RunManager.hpp"
#include "../Workflow.hpp"

#include "../../../model/Model.hpp"

#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/IdfObject.hpp"
#include "../../../utilities/data/EndUses.hpp"
#include "../../../utilities/data/Attribute.hpp"
#include "../../../utilities/sql/SqlFile.hpp"

#include <boost/filesystem/path.hpp>

#include <QDir>
#include <utilities/idd/IddEnums.hxx>

using openstudio::Attribute;
using openstudio::IdfFile;
using openstudio::IdfObject;
using openstudio::IddObjectType;
using openstudio::SqlFile;

TEST_F(RunManagerTestFixture, EnergyPlusPostProcessJob)
{
  openstudio::Application::instance().application(false);
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusPostProcessJobRunTest");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("EnergyPlusPostProcessJobRunDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::path originalfile = outdir / openstudio::toPath("in.osm");
  openstudio::path infile = outdir / openstudio::toPath("in_short_run.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

  openstudio::model::Model m = openstudio::model::exampleModel();
  m.save(originalfile, true);


  boost::optional<IdfFile> idfFile = IdfFile::load(originalfile);
  ASSERT_TRUE(idfFile);
  for (IdfObject idfObject : idfFile->getObjectsByType(IddObjectType::RunPeriod)){
    idfObject.setInt(1, 1);
    idfObject.setInt(2, 1);
    idfObject.setInt(3, 1);
    idfObject.setInt(4, 2);
  }
  idfFile->save(infile, true);
  
  openstudio::runmanager::Workflow workflow("modeltoidf->energyplus->energypluspostprocess");

  workflow.setInputFiles(infile, weatherdir);
  
  // Build list of tools
  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(), 
          openstudio::path(), 
          openstudio::path(), 
          openstudio::path(),
          openstudio::path());
  workflow.add(tools);

  openstudio::runmanager::Job job = workflow.create(outdir);

  kit.enqueue(job, true);

  kit.waitForFinished();

  openstudio::runmanager::FileInfo file;
 
  ASSERT_NO_THROW(file = job.treeOutputFiles().getLastByFilename("report.xml"));

  boost::optional<Attribute> report = Attribute::loadFromXml(file.fullPath);
  ASSERT_TRUE(report);

  boost::optional<Attribute> attribute = report->findChildByName("Net Site Energy");
  ASSERT_TRUE(attribute);

  attribute = report->findChildByName("EndUses.Electricity.Cooling.General");
  ASSERT_TRUE(attribute);

  file = job.treeOutputFiles().getLastByFilename("eplusout.sql");

  SqlFile sqlFile(file.fullPath);
  ASSERT_TRUE(sqlFile.connectionOpen());
}

TEST_F(RunManagerTestFixture, EnergyPlusPostProcessJobRerun)
{
  openstudio::Application::instance().application(false);
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusPostProcessJobRerunTest");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("EnergyPlusPostProcessJobRerunDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::path originalfile = outdir / openstudio::toPath("in.osm");
  openstudio::path infile = outdir / openstudio::toPath("in_short_run.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

  openstudio::model::Model m = openstudio::model::exampleModel();
  m.save(originalfile, true);


  boost::optional<IdfFile> idfFile = IdfFile::load(originalfile);
  ASSERT_TRUE(idfFile);
  for (IdfObject idfObject : idfFile->getObjectsByType(IddObjectType::RunPeriod)){
    idfObject.setInt(1, 1);
    idfObject.setInt(2, 1);
    idfObject.setInt(3, 1);
    idfObject.setInt(4, 2);
  }
  idfFile->save(infile, true);
  
  openstudio::runmanager::Workflow workflow("modeltoidf->energyplus->energypluspostprocess");

  workflow.setInputFiles(infile, weatherdir);
  
  // Build list of tools
  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(), 
          openstudio::path(), 
          openstudio::path(), 
          openstudio::path(),
          openstudio::path());
  workflow.add(tools);

  openstudio::runmanager::Job job = workflow.create(outdir);

  kit.enqueue(job, true);

  kit.waitForFinished();

  openstudio::runmanager::FileInfo file;
 
  ASSERT_NO_THROW(file = job.treeOutputFiles().getLastByFilename("report.xml"));

  openstudio::DateTime firsttime = file.lastModified;

  // force rerun of job
  kit.enqueue(job, true);

  kit.waitForFinished();

  ASSERT_NO_THROW(file = job.treeOutputFiles().getLastByFilename("report.xml"));

  openstudio::DateTime secondtime = file.lastModified;

  //Ensure that the report.xml created from the second run has a later lastModified time than the first,
  // confirming that the job tree actually ran a second time.
  EXPECT_GT(secondtime, firsttime);
}

TEST_F(RunManagerTestFixture, EnergyPlusPreAndPostProcessJob)
{
  openstudio::Application::instance().application(false);
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusPreAndPostProcessJobRunTest");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("EnergyPlusPreAndPostProcessJobRunDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::path originalfile = outdir / openstudio::toPath("in.osm");
  openstudio::path infile = outdir / openstudio::toPath("in_short_run.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

  openstudio::model::Model m = openstudio::model::exampleModel();
  m.save(originalfile, true);


  boost::optional<IdfFile> idfFile = IdfFile::load(originalfile);
  ASSERT_TRUE(idfFile);
  for (IdfObject idfObject : idfFile->getObjectsByType(IddObjectType::RunPeriod)){
    idfObject.setInt(1, 1);
    idfObject.setInt(2, 1);
    idfObject.setInt(3, 1);
    idfObject.setInt(4, 2);
  }
  idfFile->save(infile, true);
  
  openstudio::runmanager::Workflow workflow("modeltoidf->energypluspreprocess->energyplus->energypluspostprocess");

  workflow.setInputFiles(infile, weatherdir);
  
  // Build list of tools
  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(), 
          openstudio::path(), 
          openstudio::path(), 
          openstudio::path(),
          openstudio::path());
  workflow.add(tools);

  openstudio::runmanager::Job job = workflow.create(outdir);

  kit.enqueue(job, true);

  kit.waitForFinished();

  openstudio::runmanager::FileInfo file;
 
  ASSERT_NO_THROW(file = job.treeOutputFiles().getLastByFilename("report.xml"));

  boost::optional<Attribute> report = Attribute::loadFromXml(file.fullPath);
  ASSERT_TRUE(report);

  boost::optional<Attribute> attribute = report->findChildByName("Net Site Energy");
  ASSERT_TRUE(attribute);

  attribute = report->findChildByName("EndUses.Electricity.Cooling.General");
  ASSERT_TRUE(attribute);

  file = job.treeOutputFiles().getLastByFilename("eplusout.sql");

  SqlFile sqlFile(file.fullPath);
  ASSERT_TRUE(sqlFile.connectionOpen());
}
