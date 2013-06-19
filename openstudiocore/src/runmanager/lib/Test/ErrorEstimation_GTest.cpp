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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

#include <runmanager/lib/JobFactory.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/ErrorEstimation.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>

#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/data/EndUses.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <boost/filesystem/path.hpp>

#include <QDir>
#include <QElapsedTimer>
#include <boost/filesystem.hpp>

using openstudio::Attribute;
using openstudio::IdfFile;
using openstudio::IdfObject;
using openstudio::IddObjectType;
using openstudio::SqlFile;

openstudio::SqlFile runSimulation(openstudio::model::Model t_model, bool t_estimate)
{
  if (t_estimate)
  {
    openstudio::runmanager::RunManager::simplifyModelForPerformance(t_model);
  }

  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("ErrorEstimationRunTest");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("ErrorEstimationRunDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::path infile = outdir / openstudio::toPath("in.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

  t_model.save(infile, true);


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

  int offset = 7;

  if (t_estimate) offset = 1;

  workflow.parallelizeEnergyPlus(kit.getConfigOptions().getMaxLocalJobs(), offset);

  openstudio::runmanager::Job job = workflow.create(outdir);

  kit.enqueue(job, true);

  kit.waitForFinished();

  openstudio::path sqlpath = job.treeOutputFiles().getLastByExtension("sql").fullPath;

  openstudio::SqlFile sqlfile(sqlpath);

  return sqlfile;
}

double compareSqlFile(const openstudio::SqlFile &sf1, const openstudio::SqlFile &sf2)
{
  return *sf1.netSiteEnergy() - *sf2.netSiteEnergy();
}

double compareUses(const openstudio::runmanager::FuelUses &t_fuse1, const openstudio::runmanager::FuelUses &t_fuse2)
{
  double gas1 = t_fuse1.fuelUse(openstudio::FuelType::Gas);
  double elec1 = t_fuse1.fuelUse(openstudio::FuelType::Electricity);


  double gas2 = t_fuse2.fuelUse(openstudio::FuelType::Gas);
  double elec2 = t_fuse2.fuelUse(openstudio::FuelType::Electricity);

  LOG_FREE(Info, "compareUses", "Gas1 " << gas1 << " gas2 " << gas2 << " Gas Difference " << gas1 - gas2);
  LOG_FREE(Info, "compareUses", "Elec1 " << elec1 << " elec2 " << elec2 << " Elec Difference " << elec1 - elec2);

  double totalerror = (gas1+elec1) - (gas2+elec2);
  LOG_FREE(Info, "compareUses", "Total Error" << totalerror);
  return totalerror;
}

std::pair<double, double> runSimulation(openstudio::runmanager::ErrorEstimation &t_ee, double t_rotation)
{
  openstudio::model::Model m = openstudio::model::exampleModel();
  openstudio::model::Building b = *m.building();
  b.setNorthAxis(b.northAxis() + t_rotation);

  QElapsedTimer et;
  et.start();
  openstudio::SqlFile sqlfile1(runSimulation(m, false));
  qint64 originaltime = et.restart();
  openstudio::SqlFile sqlfile2(runSimulation(m, true));
  qint64 reducedtime = et.elapsed();

  LOG_FREE(Info, "compareUses", "OriginalTime " << originaltime << " reduced " << reducedtime);

  openstudio::runmanager::FuelUses fuses1 = t_ee.add(sqlfile1, "FullRun", "Rotation", t_rotation, true);
  openstudio::runmanager::FuelUses fuses2 = t_ee.add(sqlfile2, "Estimation", "Rotation", t_rotation, true);

  return std::make_pair(compareSqlFile(sqlfile1, sqlfile2), compareUses(fuses1, fuses2));
}

TEST_F(RunManagerTestFixture, ErrorEstimationTest)
{
  openstudio::runmanager::ErrorEstimation ee("FullRun");

  std::pair<double, double> run1 = runSimulation(ee, 0);
  LOG(Info, "Run1 initialerror: " << run1.first*1000000000 << " adjustederror: " << run1.second);

  std::pair<double, double> run2 = runSimulation(ee, 90);
  LOG(Info, "Run2 initialerror: " << run2.first*1000000000 << " adjustederror: " << run2.second);

  std::pair<double, double> run3 = runSimulation(ee, 180);
  LOG(Info, "Run3 initialerror: " << run3.first*1000000000 << " adjustederror: " << run3.second);

  std::pair<double, double> run4 = runSimulation(ee, 270);
  LOG(Info, "Run4 initialerror: " << run4.first*1000000000 << " adjustederror: " << run4.second);


}


