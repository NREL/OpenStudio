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

#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_Remove)
{
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondenserAirCooleds = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondenserAirCooleds.size());

  testObject.remove();

  refrigerationCondenserAirCooleds = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(0, refrigerationCondenserAirCooleds.size());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getModelObjects<CurveLinear>();
  for(std::vector<CurveLinear>::iterator it = refrigerationCondenserCurves.begin(); it != refrigerationCondenserCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(7000.0, testObjectClone.ratedFanPower());
  EXPECT_EQ("VariableSpeed", testObjectClone.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(0.25, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_NE(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(), testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneOneModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);
  testObject.setRatedSubcoolingTemperatureDifference(999.0);
  testObject.setCondenserFanSpeedControlType("TwoSpeed");
  testObject.setRatedFanPower(999.0);
  testObject.setMinimumFanAirFlowRatio(999.0);
  testObject.setAirInletZone(thermalZone);

  CurveLinear heatRejectionCurve = CurveLinear(model);
  heatRejectionCurve.setName("Refrigeration Condenser Air Cooled HR Curve Test");
  heatRejectionCurve.setCoefficient1Constant(0.0);
  heatRejectionCurve.setCoefficient2x(22000.0);
  heatRejectionCurve.setMinimumValueofx(5.0);
  heatRejectionCurve.setMaximumValueofx(22.2);
  
  testObject.setRatedEffectiveTotalHeatRejectionRateCurve(heatRejectionCurve);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedFanPower());
  EXPECT_EQ("TwoSpeed", testObjectClone.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_FALSE(testObjectClone.airInletZone());
  EXPECT_NE(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(), testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
  EXPECT_NE(heatRejectionCurve.handle(), testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneTwoModelsWithDefaultData)
{
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  Model model2;

  std::vector<CurveLinear> refrigerationCondenserCurves2 = model2.getModelObjects<CurveLinear>();
  EXPECT_EQ(0, refrigerationCondenserCurves2.size());

  RefrigerationCondenserAirCooled testObjectClone2 = testObject.clone(model2).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getModelObjects<CurveLinear>();
  EXPECT_EQ(2, refrigerationCondenserCurves.size());

  refrigerationCondenserCurves2 = model2.getModelObjects<CurveLinear>();
  EXPECT_EQ(1, refrigerationCondenserCurves2.size());

  for(std::vector<CurveLinear>::iterator it = refrigerationCondenserCurves.begin(); it != refrigerationCondenserCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for(std::vector<CurveLinear>::iterator it = refrigerationCondenserCurves2.begin(); it != refrigerationCondenserCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(7000.0, testObjectClone2.ratedFanPower());
  EXPECT_EQ("VariableSpeed", testObjectClone2.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(0.25, testObjectClone2.minimumFanAirFlowRatio());
  EXPECT_NE(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(), testObjectClone2.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
