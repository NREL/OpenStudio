/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

TEST_F(RunManagerTestFixture, JobCleanMaximum)
{
  openstudio::Application::instance().application(false);
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("JobCleanMaximumTest");
  boost::filesystem::create_directories(outdir);
  openstudio::runmanager::RunManager kit;

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
  openstudio::runmanager::JobParams params;
  params.append("cleanoutfiles", "maximum");
  workflow.add(params);

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

 
  openstudio::runmanager::Files files = job.treeOutputFiles();

  openstudio::runmanager::FileInfo file;
  ASSERT_NO_THROW(file = files.getLastByExtension("rdd"));

  ASSERT_FALSE(boost::filesystem::exists(file.fullPath));

}


TEST_F(RunManagerTestFixture, JobCleanNone)
{
  openstudio::Application::instance().application(false);
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("JobCleanNoneTest");
  boost::filesystem::create_directories(outdir);
  openstudio::runmanager::RunManager kit;

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
  openstudio::runmanager::JobParams params;
  params.append("cleanoutfiles", "none");
  workflow.add(params);

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

 
  openstudio::runmanager::Files files = job.treeOutputFiles();

  openstudio::runmanager::FileInfo file;
  ASSERT_NO_THROW(file = files.getLastByExtension("rdd"));
  ASSERT_TRUE(boost::filesystem::exists(file.fullPath));

  ASSERT_NO_THROW(file = files.getLastByFilename("in.idf"));
  ASSERT_TRUE(boost::filesystem::exists(file.fullPath));


}

