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
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../ZoneHVACWaterToAirHeatPump.hpp"
#include "../ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
//#include <utilities/units/Quantity.hpp>
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACWaterToAirHeatPump_FanOnOff)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    Schedule availabilitySched = model.alwaysOnDiscreteSchedule();
    CurveExponent fanPowerFtSpeedCurve(model);
    CurveCubic fanEfficiencyFtSpeedCurve(model);

    fanPowerFtSpeedCurve.setCoefficient1Constant(0.0);
    fanPowerFtSpeedCurve.setCoefficient2Constant(1.0);
    fanPowerFtSpeedCurve.setCoefficient3Constant(3.0);
    fanPowerFtSpeedCurve.setMinimumValueofx(0.0);
    fanPowerFtSpeedCurve.setMaximumValueofx(1.5);
    fanPowerFtSpeedCurve.setMinimumCurveOutput(0.01);
    fanPowerFtSpeedCurve.setMaximumCurveOutput(1.5);

    fanEfficiencyFtSpeedCurve.setCoefficient1Constant(0.33856828);
    fanEfficiencyFtSpeedCurve.setCoefficient2x(1.72644131);
    fanEfficiencyFtSpeedCurve.setCoefficient3xPOW2(-1.49280132);
    fanEfficiencyFtSpeedCurve.setCoefficient4xPOW3(0.42776208);
    fanEfficiencyFtSpeedCurve.setMinimumValueofx(0.5);
    fanEfficiencyFtSpeedCurve.setMaximumValueofx(1.5);
    fanEfficiencyFtSpeedCurve.setMinimumCurveOutput(0.3);
    fanEfficiencyFtSpeedCurve.setMaximumCurveOutput(1.0);

    FanOnOff supplyFan(model,availabilitySched,fanPowerFtSpeedCurve,fanEfficiencyFtSpeedCurve);
    CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHP(model);
    CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHP(model);
    CoilHeatingElectric supplementalHC(model,availabilitySched);
    ZoneHVACWaterToAirHeatPump testHP(model,availabilitySched,supplyFan,coilHeatingWaterToAirHP,coilCoolingWaterToAirHP,supplementalHC);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, ZoneHVACWaterToAirHeatPump_FanConstantVolume)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_DEATH (
  {
    Model model;
    Schedule availabilitySched = model.alwaysOnDiscreteSchedule();
    FanConstantVolume supplyFan(model,availabilitySched);
    CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHP(model);
    CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHP(model);
    CoilHeatingElectric supplementalHC(model,availabilitySched);

    ZoneHVACWaterToAirHeatPump testHP(model,availabilitySched,supplyFan,coilHeatingWaterToAirHP,coilCoolingWaterToAirHP,supplementalHC);
  }, ".*" );
}

TEST_F(ModelFixture, ZoneHVACWaterToAirHeatPump_SetGetFields) {
  Model model;

  CurveExponent fanPowerFtSpeedCurve(model);

  CurveCubic fanEfficiencyFtSpeedCurve(model);

  fanPowerFtSpeedCurve.setCoefficient1Constant(0.0);
  fanPowerFtSpeedCurve.setCoefficient2Constant(1.0);
  fanPowerFtSpeedCurve.setCoefficient3Constant(3.0);
  fanPowerFtSpeedCurve.setMinimumValueofx(0.0);
  fanPowerFtSpeedCurve.setMaximumValueofx(1.5);
  fanPowerFtSpeedCurve.setMinimumCurveOutput(0.01);
  fanPowerFtSpeedCurve.setMaximumCurveOutput(1.5);

  fanEfficiencyFtSpeedCurve.setCoefficient1Constant(0.33856828);
  fanEfficiencyFtSpeedCurve.setCoefficient2x(1.72644131);
  fanEfficiencyFtSpeedCurve.setCoefficient3xPOW2(-1.49280132);
  fanEfficiencyFtSpeedCurve.setCoefficient4xPOW3(0.42776208);
  fanEfficiencyFtSpeedCurve.setMinimumValueofx(0.5);
  fanEfficiencyFtSpeedCurve.setMaximumValueofx(1.5);
  fanEfficiencyFtSpeedCurve.setMinimumCurveOutput(0.3);
  fanEfficiencyFtSpeedCurve.setMaximumCurveOutput(1.0);
 
  ScheduleConstant availabilitySched(model);
  availabilitySched.setValue(1.0);
  FanOnOff supplyFan(model,availabilitySched,fanPowerFtSpeedCurve,fanEfficiencyFtSpeedCurve);

  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHP(model);

  CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHP(model);

  CoilHeatingElectric supplementalHC(model,availabilitySched);

  ZoneHVACWaterToAirHeatPump testHP(model,availabilitySched,supplyFan,coilHeatingWaterToAirHP,coilCoolingWaterToAirHP,supplementalHC);

  //test availability schedule
  availabilitySched.setValue(1.0);
  EXPECT_TRUE(testHP.setAvailabilitySchedule(availabilitySched));
  EXPECT_EQ(testHP.availabilitySchedule(),availabilitySched);

  HVACComponent supplyFan1 = supplyFan.cast<HVACComponent>();
  HVACComponent heatingCoil1 = coilHeatingWaterToAirHP.cast<HVACComponent>();
  HVACComponent coolingCoil1 = coilCoolingWaterToAirHP.cast<HVACComponent>();
  HVACComponent supplementalHC1 = supplementalHC.cast<HVACComponent>();

  //test heating coil
  testHP.setHeatingCoil(heatingCoil1);
  EXPECT_EQ(testHP.heatingCoil(),heatingCoil1);

  // test supply fan
  testHP.setSupplyAirFan(supplyFan1);
  EXPECT_EQ(testHP.supplyAirFan(),supplyFan1);


  //test cooling coil
  testHP.setCoolingCoil(coolingCoil1);
  EXPECT_EQ(testHP.coolingCoil(),coolingCoil1);

 //test supplemental electric heating coil
  testHP.setSupplementalHeatingCoil(supplementalHC1);
  EXPECT_EQ(testHP.supplementalHeatingCoil(),supplementalHC1);


  //test fan placement
  EXPECT_TRUE(testHP.setFanPlacement("DrawThrough"));
  boost::optional<std::string> str1 = testHP.fanPlacement();
  EXPECT_EQ(*str1,"DrawThrough");
  EXPECT_FALSE(testHP.isFanPlacementDefaulted());
  testHP.resetFanPlacement();
  boost::optional<std::string> str2 = testHP.fanPlacement();
  EXPECT_EQ(*str2,"BlowThrough");

  //test supply fan flow rate during Cooling,Heating and WhenNoCoolingorHeatingisNeeded
  EXPECT_TRUE(testHP.setSupplyAirFlowRateDuringCoolingOperation(1.0));
  boost::optional<double> value1 = testHP.supplyAirFlowRateDuringCoolingOperation();
  EXPECT_EQ(*value1,1.0);
  testHP.resetSupplyAirFlowRateDuringCoolingOperation();
  EXPECT_FALSE(testHP.supplyAirFlowRateDuringCoolingOperation());
  testHP.autosizeSupplyAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(testHP.isSupplyAirFlowRateDuringCoolingOperationAutosized());

  EXPECT_TRUE(testHP.setSupplyAirFlowRateDuringHeatingOperation(1.0));
  boost::optional<double> value2 = testHP.supplyAirFlowRateDuringHeatingOperation();
  EXPECT_EQ(*value2,1.0);
  EXPECT_FALSE(testHP.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  testHP.resetSupplyAirFlowRateDuringHeatingOperation();
  EXPECT_FALSE(testHP.supplyAirFlowRateDuringHeatingOperation());
  testHP.autosizeSupplyAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(testHP.isSupplyAirFlowRateDuringHeatingOperationAutosized());

  EXPECT_TRUE(testHP.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(1.0));
  boost::optional<double> value3 = testHP.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_EQ(*value3,1.0);
  EXPECT_FALSE(testHP.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());
  testHP.resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_FALSE(testHP.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded());
  testHP.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_TRUE(testHP.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());

  //test outdoor air flow rate during Cooling,Heating and WhenNoCoolingorHeatingisNeeded
  EXPECT_TRUE(testHP.setOutdoorAirFlowRateDuringCoolingOperation(1.0));
  boost::optional<double> value4 = testHP.outdoorAirFlowRateDuringCoolingOperation();
  EXPECT_EQ(*value4,1.0);
  EXPECT_FALSE(testHP.isOutdoorAirFlowRateDuringCoolingOperationAutosized());
  testHP.resetOutdoorAirFlowRateDuringCoolingOperation();
  EXPECT_FALSE(testHP.outdoorAirFlowRateDuringCoolingOperation());
  testHP.autosizeOutdoorAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(testHP.isOutdoorAirFlowRateDuringCoolingOperationAutosized());

  EXPECT_TRUE(testHP.setOutdoorAirFlowRateDuringHeatingOperation(1.0));
  boost::optional<double> value5 = testHP.outdoorAirFlowRateDuringHeatingOperation();
  EXPECT_EQ(*value5,1.0);
  EXPECT_FALSE(testHP.isOutdoorAirFlowRateDuringHeatingOperationAutosized());
  testHP.resetOutdoorAirFlowRateDuringHeatingOperation();
  EXPECT_FALSE(testHP.outdoorAirFlowRateDuringHeatingOperation());
  testHP.autosizeOutdoorAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(testHP.isOutdoorAirFlowRateDuringHeatingOperationAutosized());

  EXPECT_TRUE(testHP.setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(1.0));
  boost::optional<double> value6 = testHP.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_EQ(*value6,1.0);
  EXPECT_FALSE(testHP.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());
  testHP.resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_FALSE(testHP.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded());
  testHP.autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_TRUE(testHP.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());

//test heat pump maximum cycling rate
  EXPECT_TRUE(testHP.setMaximumCyclingRate(5.0)); 
  boost::optional<double> value7 = testHP.maximumCyclingRate();
  EXPECT_EQ(*value7,5.0);
  testHP.resetMaximumCyclingRate();
  EXPECT_TRUE(testHP.isMaximumCyclingRateDefaulted());

//test heat pump time constant
  EXPECT_TRUE(testHP.setHeatPumpTimeConstant(60)); 
  boost::optional<double> value8 = testHP.heatPumpTimeConstant();
  EXPECT_EQ(*value8,60);
  testHP.resetHeatPumpTimeConstant();
  EXPECT_TRUE(testHP.isHeatPumpTimeConstantDefaulted());

//test heat pump fraction of On cycle power use
  EXPECT_TRUE(testHP.setFractionofOnCyclePowerUse(0.02)); 
  boost::optional<double> value9 = testHP.fractionofOnCyclePowerUse();
  EXPECT_EQ(*value9,0.02);
  testHP.resetFractionofOnCyclePowerUse();
  EXPECT_TRUE(testHP.isFractionofOnCyclePowerUseDefaulted());

//test heat pump fan delay time
  EXPECT_TRUE(testHP.setHeatPumpFanDelayTime(60)); 
  boost::optional<double> value10 = testHP.heatPumpFanDelayTime();
  EXPECT_EQ(*value10,60);
  testHP.resetHeatPumpFanDelayTime();
  EXPECT_TRUE(testHP.isHeatPumpFanDelayTimeDefaulted());

//test heat pump maximum supply air temperature from supplemental heater
  EXPECT_TRUE(testHP.setMaximumSupplyAirTemperaturefromSupplementalHeater(90)); 
  boost::optional<double> value11 = testHP.maximumSupplyAirTemperaturefromSupplementalHeater();
  EXPECT_EQ(*value11,90);
  testHP.resetMaximumSupplyAirTemperaturefromSupplementalHeater();
  testHP.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  EXPECT_TRUE(testHP.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());

//test heat pump maximum outdoor air temperature for supplemental heater operation
  EXPECT_TRUE(testHP.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21)); 
  boost::optional<double> value12 = testHP.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  EXPECT_EQ(*value12,21);
  testHP.resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  EXPECT_TRUE(testHP.isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted());

//test add and remove from thermal zone
  ThermalZone thermalZone(model);
  EXPECT_EQ(0u,thermalZone.equipment().size());

  EXPECT_TRUE(testHP.addToThermalZone(thermalZone));
  
  EXPECT_TRUE(testHP.inletNode());
  EXPECT_TRUE(testHP.outletNode());

  EXPECT_EQ(1u,thermalZone.equipment().size());

  EXPECT_TRUE(supplyFan1.containingZoneHVACComponent());
  EXPECT_TRUE(coolingCoil1.containingZoneHVACComponent());
  EXPECT_TRUE(heatingCoil1.containingZoneHVACComponent());
  EXPECT_TRUE(supplementalHC1.containingZoneHVACComponent());
}
