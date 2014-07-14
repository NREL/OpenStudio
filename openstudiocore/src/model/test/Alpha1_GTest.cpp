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
/*
#include "ModelFixture.hpp"

#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Zone.hpp"
#include "../Zone_Impl.hpp"
#include "../Lights.hpp"
#include "../Lights_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"

#include "../../energyplus/ErrorFile.hpp"
#include "../../energyplus/ReverseTranslator.hpp"
#include "../../energyplus/ForwardTranslator.hpp"
#include "../../energyplus/ResultsTranslator.hpp"

#include <runmanager/Test/EnergyPlusBin.hxx>
#include "../../runmanager/lib/Job.hpp"
#include "../../runmanager/lib/JobFactory.hpp"
#include "../../runmanager/lib/RunManager.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>
#include "../../utilities/filetypes/EpwFile.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Compare.hpp"

#include <boost/algorithm/string/case_conv.hpp>

using namespace openstudio::model;
using namespace openstudio;
using openstudio::energyplus::ErrorFile;
using openstudio::IddObjectType;
using boost::to_upper_copy;

TEST_F(ModelFixture, Alpha1)
{
  // openstudio::Logger::instance().enableStandardOut();
  // openstudio::Logger::instance().logLevel(Debug);

  // read in EnergyPlus files
  Workspace inWorkspace(idfFile);
  EpwFile epwFile(epDir / openstudio::toPath("in.epw"));
  ErrorFile errorFile(epDir / openstudio::toPath("eplusout.err"));

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // import results
  ASSERT_TRUE(sqlFile.connectionOpen());
  model.setSqlFile(sqlFile);

  // set the weather file
  EXPECT_TRUE(WeatherFile::setWeatherFile(model, epwFile));

  // check that weather file was converted
  OptionalWeatherFile weatherFile = model.getOptionalUniqueModelObject<WeatherFile>();
  ASSERT_TRUE(weatherFile);
  OptionalString city = weatherFile->getString(OS_WeatherFileFields::City);
  ASSERT_TRUE(city);
  EXPECT_EQ("Chicago Ohare Intl Ap", *city);
  OptionalString url = weatherFile->getString(OS_WeatherFileFields::Url);
  ASSERT_TRUE(url);
  EXPECT_EQ(toString(epDir / toPath("in.epw")), *url);

  // make a building
  Building building = model.getUniqueModelObject<Building>();

  // should be 6 zones
  double grossArea = 0;
  EXPECT_EQ(static_cast<size_t>(6), model.getModelObjects<Zone>().size());
  EXPECT_EQ(static_cast<size_t>(6), building.zones().size());
  for (const Zone& zone : model.getModelObjects<Zone>()){

    OptionalString name = zone.name();
    ASSERT_TRUE(name);

    // get floor area from zone calculation
    double floorArea = zone.floorArea();
    EXPECT_TRUE(floorArea > 0);
    grossArea += floorArea;

    // get floor area from sqlite database
    std::string query = "SELECT FloorArea FROM zones WHERE ZoneName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlFloorArea = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlFloorArea);

    // calculated floor area should be same as that in sqlite database
    EXPECT_NEAR(*sqlFloorArea, floorArea, 0.01);
  }

  // get floor area from building calculation
  double floorArea = building.floorArea();
  EXPECT_TRUE(floorArea > 0);

  // get conditioned floor area from building calculation
  boost::optional<double> conditionedFloorArea = building.conditionedFloorArea();
  ASSERT_TRUE(conditionedFloorArea);
  EXPECT_TRUE(*conditionedFloorArea > 0);
  EXPECT_TRUE(floorArea >= *conditionedFloorArea);

  // check building area, will be less because plenum is not included
  std::string query = "SELECT Value FROM tabulardatawithstrings  WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND TableName='Building Area' AND RowName='Total Building Area' AND ColumnName='Area' AND Units='m2'";
  OptionalDouble buildingFloorArea = model.sqlFile()->execAndReturnFirstDouble(query);
  ASSERT_TRUE(buildingFloorArea);
  EXPECT_TRUE(*buildingFloorArea < grossArea);

  // calculated floor area should be same as that in sqlite database
  EXPECT_NEAR(*buildingFloorArea, floorArea, 0.01);

  // should be 5 lights to start with
  EXPECT_EQ(static_cast<size_t>(5), model.getModelObjects<Lights>().size());
  for (const Lights& light : model.getModelObjects<Lights>()){

    OptionalString name = light.name();
    ASSERT_TRUE(name);

    OptionalZone zone = light.zone();
    ASSERT_TRUE(zone);

    // get lighting power from calculation
    double lightingPower = light.getLightingPower(zone->floorArea(), zone->maxPeople());
    EXPECT_TRUE(lightingPower > 0);

    // get lighting power from sqlite database
    std::string query = "SELECT DesignLevel FROM nominallighting WHERE ObjectName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlLightingPower = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlLightingPower);

    // lighting power should be same as that in sqlite database
    EXPECT_NEAR(*sqlLightingPower, lightingPower, 1.0E-6);
  }

  // check number of compact schedules
  unsigned numCompactSchedules = model.getModelObjects<ScheduleCompact>().size();
  EXPECT_TRUE(numCompactSchedules > 0);

  // Add Lights schedule to a temporary mnodel.
  OptionalIdfObject oIdfObject;
  oIdfObject = IdfObject::load("ScheduleTypeLimits, \n\
                                 Any Number;              !- Name",
                               *(IddFactory::instance().getObject(IddObjectType::ScheduleTypeLimits)));
  ASSERT_TRUE(oIdfObject);
  IdfObject idfScheduleLimits = *oIdfObject;

  oIdfObject = IdfObject::load("SCHEDULE:COMPACT, \n\
                                 LIT-SCHED-BldgAvg,       !- Name \n\
                                 Any Number,              !- Schedule Type Limits Name \n\
                                 Through: 12/31,          !- Field 1 \n\
                                 For: Weekdays SummerDesignDay, !- Field 2 \n\
                                 Until:  4:00,0.05,       !- Field 3 \n\
                                 Until:  5:00,0.1,        !- Field 5 \n\
                                 Until:  6:00,0.2,        !- Field 7 \n\
                                 Until:  7:00,0.4,        !- Field 9 \n\
                                 Until:  8:00,0.7,        !- Field 11 \n\
                                 Until:  9:00,0.8,        !- Field 13 \n\
                                 Until:  17:00,0.85,      !- Field 15 \n\
                                 Until:  18:00,0.8,       !- Field 17 \n\
                                 Until:  19:00,0.35,      !- Field 19 \n\
                                 Until: 24:00,0.1,        !- Field 21 \n\
                                 For: Saturday,           !- Field 23 \n\
                                 Until:  5:00,0.05,       !- Field 24 \n\
                                 Until:  6:00,0.1,        !- Field 26 \n\
                                 Until:  7:00,0.15,       !- Field 28 \n\
                                 Until:  9:00,0.21,       !- Field 30 \n\
                                 Until: 14:00,0.25,       !- Field 32 \n\
                                 Until: 17:00,0.2,        !- Field 34 \n\
                                 Until: 18:00,0.15,       !- Field 36 \n\
                                 Until: 24:00,0.1,        !- Field 38 \n\
                                 For: Sunday Holidays WinterDesignDay AllOtherDays, !- Field 40 \n\
                                 Until:  5:00,0.05,       !- Field 41 \n\
                                 Until:  7:00,0.1,        !- Field 43 \n\
                                 Until:  17:00,0.15,      !- Field 45 \n\
                                 Until:  20:00,0.1,       !- Field 47 \n\
                                 Until:  24:00,0.05;      !- Field 49 ",
                                *(IddFactory::instance().getObject(IddObjectType::Schedule_Compact)));
  ASSERT_TRUE(oIdfObject);
  IdfObject idfSchedule = *oIdfObject;
  IdfObjectVector idfObjectVector;
  idfObjectVector.push_back(idfSchedule);
  idfObjectVector.push_back(idfScheduleLimits);
  ASSERT_EQ(static_cast<size_t>(2),idfObjectVector.size());

  // go through each zone
  for (Zone zone : building.zones()){

    // pretend a rule has checked space type and given us lpd and idfObjectVector
    double lpd = 1.0;

    // insert schedule and schedule type limits to full model
    HandleVector handles = getHandles(model.insertObjects(idfObjectVector));
    ASSERT_EQ(static_cast<size_t>(2), handles.size());

    // make sure one of the objects we get back is a schedule
    // DLM@20100719: would be great if we could know something about order of these handles
    // that way we would not have to loop through and test each one
    OptionalSchedule schedule;
    for (Handle h : handles){
      schedule = model.getModelObject<Schedule>(h);
      if (schedule){
        break;
      }
    }
    ASSERT_TRUE(schedule);

    // check if zone is included in area, e.g. not plenum, need better way
    OptionalString partofFloorArea = zone.getString(ZoneFields::PartofTotalFloorArea, true);
    ASSERT_TRUE(partofFloorArea);

    if (istringEqual("Yes", *partofFloorArea)){

      // set the lpd
      zone.setLightingPowerPerFloorArea(lpd);
      EXPECT_EQ(lpd, zone.lightingPowerPerFloorArea());

      // set the lighting schedule
      LightsVector zoneLights = zone.lights();
      ASSERT_EQ(static_cast<size_t>(1), zoneLights.size());
      // DLM@20100702: Elaine I get what you were thinking about preserving the light if it exists
      // but I think this will lead to  unexpected behavior if setting lpd on a zone does one thing
      // if the zone has one light and something different if it has more lights.  We will keep this
      // as is for the demo but I suggest changing it back.
      //EXPECT_FALSE(zoneLights[0].schedule());
      EXPECT_TRUE(zoneLights[0].setSchedule(*schedule));
    }
  }

  // should be 5 lights at the end, plenum does not get lights
  EXPECT_EQ(static_cast<size_t>(5), model.getModelObjects<Lights>().size());

  // number of compact schedules should have increased by only one (duplicate schedules were merged)
  EXPECT_EQ(numCompactSchedules + 1, model.getModelObjects<ScheduleCompact>().size());

  // write the osm file
  // serialize
  model.toIdfFile().save(epDir/toPath("in.osm"),true);

  // translate to EnergyPlus
  energyplus::ForwardTranslator forwardTranslator(model);
  OptionalWorkspace outWorkspace = forwardTranslator.convert();
  ASSERT_TRUE(outWorkspace);

  // write files
  boost::filesystem::remove_all(epDir / toPath("alpha1/"));
  boost::filesystem::create_directory(epDir / toPath("alpha1/"));
  boost::filesystem::ofstream ofs(epDir / toPath("alpha1/in.idf"));
  outWorkspace->toIdfFile().print(ofs);
  ofs.close();

  // close sql lite connection before running
  model.resetSqlFile();
  EXPECT_FALSE(model.sqlFile());

  openstudio::path outdir = openstudio::tempDir() / toPath("Alpha1Test");

  // run simulation
  openstudio::runmanager::Job job = openstudio::runmanager::JobFactory::createEnergyPlusJob(
      openstudio::runmanager::ToolInfo("energyplus", energyPlusExePath()),
      energyPlusIDDPath(),
      epDir / toPath("alpha1/in.idf"),
      epDir / toPath("in.epw"),
      outdir);


  // this file is new and so out of date
  EXPECT_TRUE(job.outOfDate());

  // add to queue
  // TODO: persist queue to path
  openstudio::runmanager::RunManager runManager(openstudio::tempDir() / openstudio::toPath("runmanagerTempDB"), true);
  EXPECT_FALSE(runManager.workPending());

  // start simulation by queuing job
  std::cout << "Starting simulation " << job.description() << std::endl;
  runManager.enqueue(job, true);

  // check run manager status
  EXPECT_TRUE(runManager.workPending());

  EXPECT_EQ(static_cast<size_t>(1), runManager.getJobs().size());
  unsigned index = 0;
  for (const openstudio::runmanager::Job& j : runManager.getJobs()) {
    LOG(Debug,"Run manager job " << index << ": '" << j.description() << "'");
    ++index;
  }
  EXPECT_TRUE(runManager.getJob(job.uuid()) == job);

  // By letting the run manager run the job we have no guarantee
  // on when exactly it will get started, so waitForFinished()
  // is a race to see if it has actually started or not yet.
  // This way, we wait for the runmanager to tell us when it is done
  runManager.waitForFinished();

  EXPECT_EQ(job.errors().succeeded, true);
  if (!job.errors().succeeded) {
    for (const std::string& error : job.errors().errors) {
      LOG(Error,"Error returned from Alpha1 simulation job: " << error);
    }
  }

  EXPECT_FALSE(runManager.workPending());
  std::cout << "Simulation complete" << std::endl;

  // get errorFile and set sqlFile
  errorFile = ErrorFile(job.allFiles().getLastByFilename("eplusout.err").fullPath);
  sqlFile = SqlFile(job.allFiles().getLastByFilename("eplusout.sql").fullPath);
  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());
  ASSERT_TRUE(model.sqlFile()->connectionOpen());

  // check errors
  OptionalInt completed = model.sqlFile()->execAndReturnFirstInt("SELECT Completed FROM simulations");
  ASSERT_TRUE(completed);
  EXPECT_EQ(1, *completed);

  OptionalInt completedSuccessfully = model.sqlFile()->execAndReturnFirstInt("SELECT CompletedSuccessfully FROM simulations");
  ASSERT_TRUE(completedSuccessfully);
  EXPECT_EQ(1, *completedSuccessfully);

  // check simulated hours
  query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' AND ReportForString='Entire Facility' AND TableName='General' AND RowName='Hours Simulated' AND ColumnName='Value' AND Units='hrs'";
  OptionalDouble hoursSimulated = model.sqlFile()->execAndReturnFirstDouble(query);
  ASSERT_TRUE(hoursSimulated);
  EXPECT_EQ(8760, *hoursSimulated);

}
*/
