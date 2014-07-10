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
#include "../ErrorEstimation.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/Building.hpp"

#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/IdfObject.hpp"
#include "../../../utilities/data/EndUses.hpp"
#include "../../../utilities/data/Attribute.hpp"
#include "../../../utilities/sql/SqlFile.hpp"

#include "../../../isomodel/UserModel.hpp"
#include "../../../isomodel/ForwardTranslator.hpp"

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

double compare(double v1, double v2)
{
  return v1-v2;
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

  openstudio::path weatherpath = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
  openstudio::isomodel::ForwardTranslator translator;
  openstudio::isomodel::UserModel userModel = translator.translateModel(m);
  userModel.setWeatherFilePath(weatherpath);
  openstudio::isomodel::SimModel simModel = userModel.toSimModel();
  openstudio::isomodel::ISOResults isoResults = simModel.simulate();

  LOG_FREE(Info, "runSimulation", "OriginalTime " << originaltime << " reduced " << reducedtime);

  std::vector<double> variables;
  variables.push_back(t_rotation);
  openstudio::runmanager::FuelUses fuses0(0);
  try {
    fuses0 = t_ee.approximate(variables);
  } catch (const std::exception &e) {
    LOG_FREE(Info, "runSimulation", "Unable to generate estimate: " << e.what());
  }

  openstudio::runmanager::FuelUses fuses3 = t_ee.add(userModel, isoResults, "ISO", variables);
  openstudio::runmanager::FuelUses fuses2 = t_ee.add(sqlfile2, "Estimation", variables);
  openstudio::runmanager::FuelUses fuses1 = t_ee.add(sqlfile1, "FullRun", variables);

  LOG_FREE(Info, "runSimulation", "Comparing Full Run to linear approximation");
  compareUses(fuses1, fuses0);
  LOG_FREE(Info, "runSimulation", "Comparing Full Run to error adjusted ISO run");
  return std::make_pair(compare(*sqlfile1.netSiteEnergy(), isoResults.totalEnergyUse()), compareUses(fuses1, fuses3));
}

TEST_F(RunManagerTestFixture, ErrorEstimationTest)
{
  openstudio::runmanager::ErrorEstimation ee(1);
  ee.setConfidence("FullRun", 1.0);
  ee.setConfidence("Estimation", 0.75);
  ee.setConfidence("ISO", 0.50);

  std::pair<double, double> run1 = runSimulation(ee, 0);
  LOG(Info, "Run1 initialerror: " << run1.first*1000000000 << " adjustederror: " << run1.second);

  std::pair<double, double> run2 = runSimulation(ee, 90);
  LOG(Info, "Run2 initialerror: " << run2.first*1000000000 << " adjustederror: " << run2.second);

  std::pair<double, double> run3 = runSimulation(ee, 180);
  LOG(Info, "Run3 initialerror: " << run3.first*1000000000 << " adjustederror: " << run3.second);

  std::pair<double, double> run4 = runSimulation(ee, 270);
  LOG(Info, "Run4 initialerror: " << run4.first*1000000000 << " adjustederror: " << run4.second);

//  std::pair<double, double> run5 = runSimulation(ee, 225);
//  LOG(Info, "Run5 initialerror: " << run5.first*1000000000 << " adjustederror: " << run5.second);

}


TEST_F(RunManagerTestFixture, LinearApproximationTestSimple)
{
    LinearApproximation la(1);
    
    std::vector<double> vals;
    vals.push_back(0);
    la.addVals(vals, 0);
    vals[0] = 2;
    la.addVals(vals, 2);

    EXPECT_DOUBLE_EQ(2.0, la.approximate(vals));
    vals[0] = 0;
    EXPECT_DOUBLE_EQ(0.0, la.approximate(vals));
    vals[0] = 1;
    EXPECT_DOUBLE_EQ(1.0, la.approximate(vals));

}

TEST_F(RunManagerTestFixture, LinearApproximationTestHuge)
{
  const size_t size = 200;

  LinearApproximation la(size);
  
  std::vector<double> vals(size);

  // just establish a baseline
  for (size_t i = 0; i < size; ++i)
  {
    vals[i] = i;
  }

  // let's say that this equals 100
  la.addVals(vals, 100);

  // and we should be able to get back the value we just put in
  EXPECT_EQ(100.0, la.approximate(vals));

  // now we'll modify one variable at a time
  for (size_t i = 0; i < size; ++i)
  {
    std::vector<double> newvals(vals);

    double origVariable = newvals[i];
    double newVariable = origVariable * 2.0;

    newvals[i] = newVariable;

    double valueAtThisPoint = 100.0 + newvals[i];

    la.addVals(newvals, valueAtThisPoint);
    EXPECT_DOUBLE_EQ(valueAtThisPoint, la.approximate(newvals));

    newvals[i] = (origVariable + newVariable) / 2;

    EXPECT_DOUBLE_EQ((valueAtThisPoint + 100.0) / 2, la.approximate(newvals));
  }

  vals[size/10] = 62.4;
  vals[size/8] = 99;
  vals[size/6] = 99;
  vals[size/4] = 102;
  vals[size/2] = 102;


  la.approximate(vals);
}


