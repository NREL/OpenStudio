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

#include "ModelFixture.hpp"

#include "../RefrigerationAirChiller.hpp"
#include "../RefrigerationAirChiller_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationAirChiller_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationAirChiller_Remove)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  std::vector<RefrigerationAirChiller> refrigerationAirChillers = m.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(1, refrigerationAirChillers.size());

  testObject.remove();

  refrigerationAirChillers = m.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(0, refrigerationAirChillers.size());
}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneOneModelWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

  EXPECT_EQ("UnitLoadFactorSensibleOnly", testObjectClone.capacityRatingType());
  EXPECT_DOUBLE_EQ(15000.0, testObjectClone.ratedUnitLoadFactor().get());
  EXPECT_DOUBLE_EQ(-30.0, testObjectClone.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(5.6, testObjectClone.ratedTemperatureDifferenceDT1());
  EXPECT_DOUBLE_EQ(7.3, testObjectClone.maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature().get());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.coilMaterialCorrectionFactor());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.refrigerantCorrectionFactor());
  EXPECT_DOUBLE_EQ(1.5, testObjectClone.sHR60CorrectionFactor());
  EXPECT_DOUBLE_EQ(275.0, testObjectClone.ratedTotalHeatingPower());
  EXPECT_EQ("Fixed", testObjectClone.fanSpeedControlType());
  EXPECT_DOUBLE_EQ(1500.0, testObjectClone.ratedFanPower());
  EXPECT_DOUBLE_EQ(20.0, testObjectClone.ratedAirFlow());
  EXPECT_DOUBLE_EQ(0.2, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_EQ("Electric", testObjectClone.defrostType());
  EXPECT_EQ("TimeSchedule", testObjectClone.defrostControlType());
  EXPECT_DOUBLE_EQ(75000.0, testObjectClone.defrostPower().get());
  EXPECT_DOUBLE_EQ(0.7, testObjectClone.temperatureTerminationDefrostFractiontoIce().get());
  EXPECT_EQ("Middle", testObjectClone.verticalLocation());
}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneOneModelWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  testObject.setCapacityRatingType("EuropeanSC1Standard");
  testObject.setRatedUnitLoadFactor(999.0);
  testObject.setRatedCoolingSourceTemperature(40.0);
  testObject.setRatedTemperatureDifferenceDT1(20.0);
  testObject.setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(25.0);
  testObject.setCoilMaterialCorrectionFactor(999.0);
  testObject.setRefrigerantCorrectionFactor(999.0);
  testObject.setSHR60CorrectionFactor(1.67);
  testObject.setRatedTotalHeatingPower(999.0);
  testObject.setFanSpeedControlType("VariableSpeed");
  testObject.setRatedFanPower(999.0);
  testObject.setRatedAirFlow(999.0);
  testObject.setMinimumFanAirFlowRatio(0.99);
  testObject.setDefrostType("OffCycle");
  testObject.setDefrostControlType("TimeSchedule");
  testObject.setDefrostPower(999.0);
  testObject.setTemperatureTerminationDefrostFractiontoIce(0.99);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

  EXPECT_EQ("EuropeanSC1Standard", testObjectClone.capacityRatingType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedUnitLoadFactor().get());
  EXPECT_DOUBLE_EQ(40.0, testObjectClone.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(20.0, testObjectClone.ratedTemperatureDifferenceDT1());
  EXPECT_DOUBLE_EQ(25.0, testObjectClone.maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.coilMaterialCorrectionFactor());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.refrigerantCorrectionFactor());
  EXPECT_DOUBLE_EQ(1.67, testObjectClone.sHR60CorrectionFactor());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedTotalHeatingPower());
  EXPECT_EQ("VariableSpeed", testObjectClone.fanSpeedControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedFanPower());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedAirFlow());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_EQ("OffCycle", testObjectClone.defrostType());
  EXPECT_EQ("TimeSchedule", testObjectClone.defrostControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.defrostPower().get());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone.temperatureTerminationDefrostFractiontoIce().get());
  EXPECT_EQ("Middle", testObjectClone.verticalLocation());

}

TEST_F(ModelFixture, RefrigerationAirChiller_CloneTwoModelsWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);

  testObject.setCapacityRatingType("EuropeanSC1Standard");
  testObject.setRatedUnitLoadFactor(999.0);
  testObject.setRatedCoolingSourceTemperature(40.0);
  testObject.setRatedTemperatureDifferenceDT1(20.0);
  testObject.setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(25.0);
  testObject.setCoilMaterialCorrectionFactor(999.0);
  testObject.setRefrigerantCorrectionFactor(999.0);
  testObject.setSHR60CorrectionFactor(1.67);
  testObject.setRatedTotalHeatingPower(999.0);
  testObject.setFanSpeedControlType("VariableSpeed");
  testObject.setRatedFanPower(999.0);
  testObject.setRatedAirFlow(999.0);
  testObject.setMinimumFanAirFlowRatio(0.99);
  testObject.setDefrostType("OffCycle");
  testObject.setDefrostControlType("TimeSchedule");
  testObject.setDefrostPower(999.0);
  testObject.setTemperatureTerminationDefrostFractiontoIce(0.99);

  RefrigerationAirChiller testObjectClone = testObject.clone(m).cast<RefrigerationAirChiller>();

  Model m2;

  RefrigerationAirChiller testObjectClone2 = testObject.clone(m2).cast<RefrigerationAirChiller>();

  EXPECT_EQ("EuropeanSC1Standard", testObjectClone2.capacityRatingType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedUnitLoadFactor().get());
  EXPECT_DOUBLE_EQ(40.0, testObjectClone2.ratedCoolingSourceTemperature());
  EXPECT_DOUBLE_EQ(20.0, testObjectClone2.ratedTemperatureDifferenceDT1());
  EXPECT_DOUBLE_EQ(25.0, testObjectClone2.maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.coilMaterialCorrectionFactor());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.refrigerantCorrectionFactor());
  EXPECT_DOUBLE_EQ(1.67, testObjectClone2.sHR60CorrectionFactor());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedTotalHeatingPower());
  EXPECT_EQ("VariableSpeed", testObjectClone2.fanSpeedControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedFanPower());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedAirFlow());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone2.minimumFanAirFlowRatio());
  EXPECT_EQ("OffCycle", testObjectClone2.defrostType());
  EXPECT_EQ("TimeSchedule", testObjectClone2.defrostControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.defrostPower().get());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone2.temperatureTerminationDefrostFractiontoIce().get());
  EXPECT_EQ("Middle", testObjectClone2.verticalLocation());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone, testObject);
}

TEST_F(ModelFixture, RefrigerationAirChiller_AddToThermalZone)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);
  ThermalZone thermalZone = ThermalZone(m);

  EXPECT_EQ(0, thermalZone.equipment().size());
  EXPECT_TRUE(testObject.addToThermalZone(thermalZone));
  EXPECT_EQ(1, thermalZone.equipment().size());
}

TEST_F(ModelFixture, RefrigerationAirChiller_RemoveFromThermalZone)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);
  ThermalZone thermalZone = ThermalZone(m);

  testObject.addToThermalZone(thermalZone);
  testObject.removeFromThermalZone();
  EXPECT_EQ(0, thermalZone.equipment().size());
}

TEST_F(ModelFixture, RefrigerationAirChiller_ThermalZone)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  RefrigerationAirChiller testObject = RefrigerationAirChiller(m, s);
  ThermalZone thermalZone = ThermalZone(m);

  testObject.addToThermalZone(thermalZone);
  ASSERT_TRUE(testObject.thermalZone());
  EXPECT_EQ(thermalZone, testObject.thermalZone().get());
}
