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

#include <model/test/ModelFixture.hpp>

#include <model/RefrigerationWalkIn.hpp>
#include <model/RefrigerationWalkIn_Impl.hpp>
#include <model/RefrigerationWalkInZoneBoundary.hpp>
#include <model/RefrigerationWalkInZoneBoundary_Impl.hpp>
#include <model/ScheduleCompact.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationWalkIn_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
	ScheduleCompact wds(model);
	RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationWalkIn_Remove)
{
  Model model;
  ScheduleCompact wds(model);
  RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> refrigerationWalkIn = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(1, refrigerationWalkIn.size());

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(1, refrigerationWalkInZoneBoundaries.size());

  testObject.remove();

  refrigerationWalkIn = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(0, refrigerationWalkIn.size());

  refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(0, refrigerationWalkInZoneBoundaries.size());
}

TEST_F(ModelFixture, RefrigerationWalkIn_CloneOneModelWithDefaultData)
{
  Model model;
  ScheduleCompact wds(model);
  RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);

  RefrigerationWalkIn testObjectClone = testObject.clone(model).cast<RefrigerationWalkIn>();

  EXPECT_DOUBLE_EQ(4690.0 ,testObjectClone.ratedCoilCoolingCapacity());
  EXPECT_DOUBLE_EQ(-2.22 ,testObjectClone.operatingTemperature());
  EXPECT_DOUBLE_EQ(-6.67 ,testObjectClone.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(0.0 ,testObjectClone.ratedTotalHeatingPower());
  EXPECT_DOUBLE_EQ(735.0 ,testObjectClone.ratedCoolingCoilFanPower());
  EXPECT_DOUBLE_EQ(0.0 ,testObjectClone.ratedCirculationFanPower());
  EXPECT_DOUBLE_EQ(120.0 ,testObjectClone.ratedTotalLightingPower());
  EXPECT_EQ("Electric", testObjectClone.defrostType());
  EXPECT_EQ("TimeSchedule", testObjectClone.defrostControlType());
  EXPECT_DOUBLE_EQ(5512.0 ,testObjectClone.defrostPower().get());
  EXPECT_DOUBLE_EQ(13.0 ,testObjectClone.insulatedFloorSurfaceArea());
  EXPECT_DOUBLE_EQ(0.207 ,testObjectClone.insulatedFloorUValue());

  std::vector<RefrigerationWalkInZoneBoundary> testObjectZoneBoundaries = testObject.zoneBoundaries();
  std::vector<RefrigerationWalkInZoneBoundary> testObjectCloneZoneBoundaries = testObjectClone.zoneBoundaries();
  EXPECT_EQ(1, testObjectZoneBoundaries.size());
  EXPECT_EQ(1, testObjectCloneZoneBoundaries.size());

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(2, refrigerationWalkInZoneBoundaries.size());

  EXPECT_NE(testObjectZoneBoundaries[0], testObjectCloneZoneBoundaries[0]);
  EXPECT_NE(testObjectZoneBoundaries[0].handle(), testObjectCloneZoneBoundaries[0].handle());

  EXPECT_EQ(testObjectClone.defrostSchedule().handle(), testObject.defrostSchedule().handle());
  EXPECT_EQ(testObjectClone.defrostSchedule().handle(), wds.handle());
}

TEST_F(ModelFixture, RefrigerationWalkIn_CloneOneModelWithCustomData)
{
  Model model;
  ScheduleCompact wds(model);
  ScheduleCompact wds2(model);
  ScheduleCompact wddds2(model);
  RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);

  testObject.setRatedCoilCoolingCapacity(999.0);
  testObject.setOperatingTemperature(-999.0);
  testObject.setRatedCoolingSourceTemperature(-70.0);
  testObject.setRatedTotalHeatingPower(999.0);
  testObject.setRatedCoolingCoilFanPower(999.0);
  testObject.setRatedCirculationFanPower(999.0);
  testObject.setRatedTotalLightingPower(999.0);
  testObject.setDefrostType("OffCycle");
  testObject.setDefrostControlType("TemperatureTermination");
  testObject.setDefrostSchedule(wds2);
  testObject.setDefrostDripDownSchedule(wddds2);
  testObject.setDefrostPower(999.0);
  testObject.setInsulatedFloorSurfaceArea(999.0);
  testObject.setInsulatedFloorUValue(999.0);

  RefrigerationWalkIn testObjectClone = testObject.clone(model).cast<RefrigerationWalkIn>();
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.ratedCoilCoolingCapacity());
  EXPECT_DOUBLE_EQ(-999.0 ,testObjectClone.operatingTemperature());
  EXPECT_DOUBLE_EQ(-70.0 ,testObjectClone.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.ratedTotalHeatingPower());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.ratedCoolingCoilFanPower());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.ratedCirculationFanPower());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.ratedTotalLightingPower());
  EXPECT_EQ("OffCycle", testObjectClone.defrostType());
  EXPECT_EQ("TemperatureTermination", testObjectClone.defrostControlType());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.defrostPower().get());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.insulatedFloorSurfaceArea());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.insulatedFloorUValue());

  EXPECT_EQ(testObjectClone.defrostSchedule().handle(), testObject.defrostSchedule().handle());
  EXPECT_EQ(testObjectClone.defrostSchedule().handle(), wds2.handle());
  EXPECT_EQ(testObjectClone.defrostDripDownSchedule()->handle(), wddds2.handle());
}

TEST_F(ModelFixture, RefrigerationWalkIn_CloneTwoModelsWithDefaultData)
{
  Model model;
  ScheduleCompact wds(model);
  RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);

  RefrigerationWalkIn testObjectClone = testObject.clone(model).cast<RefrigerationWalkIn>();

  Model model2;

  RefrigerationWalkIn testObjectClone2 = testObject.clone(model2).cast<RefrigerationWalkIn>();
  EXPECT_DOUBLE_EQ(4690.0 ,testObjectClone2.ratedCoilCoolingCapacity());
  EXPECT_DOUBLE_EQ(-2.22 ,testObjectClone2.operatingTemperature());
  EXPECT_DOUBLE_EQ(-6.67 ,testObjectClone2.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(0.0 ,testObjectClone2.ratedTotalHeatingPower());
  EXPECT_DOUBLE_EQ(735.0 ,testObjectClone2.ratedCoolingCoilFanPower());
  EXPECT_DOUBLE_EQ(0.0 ,testObjectClone2.ratedCirculationFanPower());
  EXPECT_DOUBLE_EQ(120.0 ,testObjectClone2.ratedTotalLightingPower());
  EXPECT_EQ("Electric", testObjectClone2.defrostType());
  EXPECT_EQ("TimeSchedule", testObjectClone2.defrostControlType());
  EXPECT_DOUBLE_EQ(5512.0 ,testObjectClone2.defrostPower().get());
  EXPECT_DOUBLE_EQ(13.0 ,testObjectClone2.insulatedFloorSurfaceArea());
  EXPECT_DOUBLE_EQ(0.207 ,testObjectClone2.insulatedFloorUValue());

  std::vector<RefrigerationWalkInZoneBoundary> testObjectZoneBoundaries = testObject.zoneBoundaries();
  std::vector<RefrigerationWalkInZoneBoundary> testObjectCloneZoneBoundaries = testObjectClone.zoneBoundaries();
  std::vector<RefrigerationWalkInZoneBoundary> testObjectCloneZoneBoundaries2 = testObjectClone2.zoneBoundaries();
  EXPECT_EQ(1, testObjectZoneBoundaries.size());
  EXPECT_EQ(1, testObjectCloneZoneBoundaries.size());
  EXPECT_EQ(1, testObjectCloneZoneBoundaries2.size());

  EXPECT_NE(testObjectZoneBoundaries[0], testObjectCloneZoneBoundaries[0]);
  EXPECT_NE(testObjectZoneBoundaries[0].handle(), testObjectCloneZoneBoundaries[0].handle());

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries = model.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(2, refrigerationWalkInZoneBoundaries.size());

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries2 = model2.getModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(1, refrigerationWalkInZoneBoundaries2.size());

  EXPECT_NE(testObjectCloneZoneBoundaries2[0].handle(), testObjectCloneZoneBoundaries[0].handle());
  EXPECT_NE(testObjectClone2.defrostSchedule().handle(), testObject.defrostSchedule().handle());
  EXPECT_NE(testObjectClone2.defrostSchedule().handle(), wds.handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, RefrigerationWalkIn_ZoneBoundaries)
{
  Model model;
  ScheduleCompact wds(model);
  RefrigerationWalkIn testObject = RefrigerationWalkIn(model, wds);
  RefrigerationWalkInZoneBoundary _zoneBoundary1(model);
  RefrigerationWalkInZoneBoundary _zoneBoundary2(model);

  std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries = testObject.zoneBoundaries();
  EXPECT_EQ(1, zoneBoundaries.size());

  testObject.removeAllZoneBoundaries();

  zoneBoundaries = testObject.zoneBoundaries();
  EXPECT_EQ(0, zoneBoundaries.size());

  EXPECT_TRUE(testObject.addZoneBoundary(_zoneBoundary1));
  zoneBoundaries = testObject.zoneBoundaries();
  EXPECT_EQ(1, zoneBoundaries.size());

  testObject.addZoneBoundary(_zoneBoundary2);

  testObject.removeZoneBoundary(0);
  zoneBoundaries = testObject.zoneBoundaries();
  EXPECT_EQ(1, zoneBoundaries.size());

  EXPECT_EQ(zoneBoundaries[0], _zoneBoundary2);
  EXPECT_EQ(zoneBoundaries[0].handle(), _zoneBoundary2.handle());
}
