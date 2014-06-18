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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../Workflow.hpp"

#include "../../../model/Model.hpp"

#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/IdfObject.hpp"
#include "../../../utilities/data/EndUses.hpp"
#include "../../../utilities/data/Attribute.hpp"
#include "../../../utilities/sql/SqlFile.hpp"

#include <boost/filesystem/path.hpp>

#include <QDir>
#include <QElapsedTimer>
#include <boost/filesystem.hpp>

using openstudio::Attribute;
using openstudio::IdfFile;
using openstudio::IdfObject;
using openstudio::IddObjectType;
using openstudio::SqlFile;

TEST_F(RunManagerTestFixture, ParallelEnergyPlusJobTest)
{
  double originalSiteEnergy = 0;
  double parallelSiteEnergy = 0;

  QElapsedTimer et;
  et.start();

  {
    openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("ParallelEnergyPlusJobRunTest");
    boost::filesystem::create_directories(outdir);
    openstudio::path db = outdir / openstudio::toPath("ParallelEnergyPlusJobRunDB");
    openstudio::runmanager::RunManager kit(db, true);

    openstudio::path infile = outdir / openstudio::toPath("in.osm");
    openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

    openstudio::model::Model m = openstudio::model::exampleModel();
    m.save(infile, true);


    openstudio::runmanager::Workflow workflow("modeltoidf->expandobjects->energyplus");

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

    openstudio::path sqlpath = job.treeOutputFiles().getLastByExtension("sql").fullPath;

    openstudio::SqlFile sqlfile(sqlpath);

    ASSERT_TRUE(sqlfile.netSiteEnergy());
    originalSiteEnergy = *sqlfile.netSiteEnergy();
  }

  qint64 originaltime = et.restart();

  {
    openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("ParallelEnergyPlusJobRunTest-part2");
    boost::filesystem::create_directories(outdir);
    openstudio::path db = outdir / openstudio::toPath("ParallelEnergyPlusJobRunDB");
    openstudio::runmanager::RunManager kit(db, true);

    openstudio::path infile = outdir / openstudio::toPath("in.osm");
    openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

    openstudio::model::Model m = openstudio::model::exampleModel();
//    openstudio::runmanager::RunManager::simplifyModelForPerformance(m);
    m.save(infile, true);


    openstudio::runmanager::Workflow workflow("modeltoidf->expandobjects->energyplus");

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

    workflow.parallelizeEnergyPlus(kit.getConfigOptions().getMaxLocalJobs(), 1);
    openstudio::runmanager::Job job = workflow.create(outdir);

    kit.enqueue(job, true);

    kit.waitForFinished();
    openstudio::path sqlpath = job.treeOutputFiles().getLastByExtension("sql").fullPath;

    openstudio::SqlFile sqlfile(sqlpath);

    ASSERT_TRUE(sqlfile.netSiteEnergy());
    parallelSiteEnergy = *sqlfile.netSiteEnergy();
    ASSERT_TRUE(sqlfile.hoursSimulated());
    EXPECT_EQ(8760, *sqlfile.hoursSimulated());
  }  

  qint64 paralleltime = et.restart();

  EXPECT_GT(paralleltime, 0);
  EXPECT_GT(originaltime, 0);

  EXPECT_LT(paralleltime, originaltime);

  LOG(Debug, "Paralleltime " << paralleltime << " originaltime " << originaltime);

  EXPECT_NE(originalSiteEnergy, parallelSiteEnergy);
  EXPECT_LT(fabs(originalSiteEnergy - parallelSiteEnergy), .1);
}


