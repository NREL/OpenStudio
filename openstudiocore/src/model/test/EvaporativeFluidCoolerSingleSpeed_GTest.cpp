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

#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_addToLoop) {
  Model model;

  P
}
lantLoop loop(model);

  ASSERT_EQ( (unsigned)5,loop.supplyComponents().size() );

	EvaporativeFluidCoolerSingleSpeed evapCooler(model);

	loop.addSupplyBranchForComponent(evapCooler);

  ASSERT_EQ( (unsigned)7,loop.supplyComponents().size() );

    loop.removeSupplyBranchWithComponent(evapCooler);

	ASSERT_EQ( (unsigned)5,loop.supplyComponents().size() );






//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignAirFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignAirFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
// // EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignAirFlowRate(testQ));
//  OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getDesignAirFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_FanPoweratDesignAirFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getFanPoweratDesignAirFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setFanPoweratDesignAirFlowRate(testQ));
////  OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getFanPoweratDesignAirFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}













//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignSprayWaterFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignSprayWaterFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignSprayWaterFlowRate(testQ));
////  Quantity q = evaporativeFluidCoolerSingleSpeed.getDesignSprayWaterFlowRate(true);
//  EXPECT_NEAR(value,q.value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_StandardDesignCapacity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getStandardDesignCapacity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setStandardDesignCapacity(testQ));
//  // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getStandardDesignCapacity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_UfactorTimesAreaValueatDesignAirFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getUfactorTimesAreaValueatDesignAirFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setUfactorTimesAreaValueatDesignAirFlowRate(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getUfactorTimesAreaValueatDesignAirFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignWaterFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignWaterFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignWaterFlowRate(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getDesignWaterFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_UserSpecifiedDesignCapacity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getUserSpecifiedDesignCapacity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setUserSpecifiedDesignCapacity(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getUserSpecifiedDesignCapacity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignEnteringWaterTemperature_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignEnteringWaterTemperature(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignEnteringWaterTemperature(testQ));
////  OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getDesignEnteringWaterTemperature(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignEnteringAirTemperature_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignEnteringAirTemperature(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignEnteringAirTemperature(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getDesignEnteringAirTemperature(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DesignEnteringAirWetbulbTemperature_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDesignEnteringAirWetbulbTemperature(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDesignEnteringAirWetbulbTemperature(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getDesignEnteringAirWetbulbTemperature(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_SizingFactor_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getSizingFactor(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setSizingFactor(testQ));
// // Quantity q = evaporativeFluidCoolerSingleSpeed.getSizingFactor(true);
//  EXPECT_NEAR(value,q.value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_EvaporationLossFactor_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getEvaporationLossFactor(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setEvaporationLossFactor(testQ));
// // OSOptionalQuantity q = evaporativeFluidCoolerSingleSpeed.getEvaporationLossFactor(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_DriftLossPercent_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getDriftLossPercent(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setDriftLossPercent(testQ));
// // Quantity q = evaporativeFluidCoolerSingleSpeed.getDriftLossPercent(true);
//  EXPECT_NEAR(value,q.value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}
//
//TEST_F(ModelFixture,EvaporativeFluidCoolerSingleSpeed_BlowdownConcentrationRatio_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  EvaporativeFluidCoolerSingleSpeed evaporativeFluidCoolerSingleSpeed(model);
//
//  Unit units = evaporativeFluidCoolerSingleSpeed.getBlowdownConcentrationRatio(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
// // Quantity testQ(value,units);
//  EXPECT_TRUE(evaporativeFluidCoolerSingleSpeed.setBlowdownConcentrationRatio(testQ));
// // Quantity q = evaporativeFluidCoolerSingleSpeed.getBlowdownConcentrationRatio(true);
//  EXPECT_NEAR(value,q.value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

