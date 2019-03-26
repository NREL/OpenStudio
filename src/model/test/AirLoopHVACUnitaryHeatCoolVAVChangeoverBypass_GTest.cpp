/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

CoilCoolingDXSingleSpeed makeCoolingCoil(Model & model)
{
  CurveBiquadratic cooling_curve_1 = CurveBiquadratic(model);
  cooling_curve_1.setCoefficient1Constant(0.766956);
  cooling_curve_1.setCoefficient2x(0.0107756);
  cooling_curve_1.setCoefficient3xPOW2(-0.0000414703);
  cooling_curve_1.setCoefficient4y(0.00134961);
  cooling_curve_1.setCoefficient5yPOW2(-0.000261144);
  cooling_curve_1.setCoefficient6xTIMESY(0.000457488);
  cooling_curve_1.setMinimumValueofx(17.0);
  cooling_curve_1.setMaximumValueofx(22.0);
  cooling_curve_1.setMinimumValueofy(13.0);
  cooling_curve_1.setMaximumValueofy(46.0);

  CurveQuadratic cooling_curve_2 = CurveQuadratic(model);
  cooling_curve_2.setCoefficient1Constant(0.8);
  cooling_curve_2.setCoefficient2x(0.2);
  cooling_curve_2.setCoefficient3xPOW2(0.0);
  cooling_curve_2.setMinimumValueofx(0.5);
  cooling_curve_2.setMaximumValueofx(1.5);

  CurveBiquadratic cooling_curve_3 = CurveBiquadratic(model);
  cooling_curve_3.setCoefficient1Constant(0.297145);
  cooling_curve_3.setCoefficient2x(0.0430933);
  cooling_curve_3.setCoefficient3xPOW2(-0.000748766);
  cooling_curve_3.setCoefficient4y(0.00597727);
  cooling_curve_3.setCoefficient5yPOW2(0.000482112);
  cooling_curve_3.setCoefficient6xTIMESY(-0.000956448);
  cooling_curve_3.setMinimumValueofx(17.0);
  cooling_curve_3.setMaximumValueofx(22.0);
  cooling_curve_3.setMinimumValueofy(13.0);
  cooling_curve_3.setMaximumValueofy(46.0);

  CurveQuadratic cooling_curve_4 = CurveQuadratic(model);
  cooling_curve_4.setCoefficient1Constant(1.156);
  cooling_curve_4.setCoefficient2x(-0.1816);
  cooling_curve_4.setCoefficient3xPOW2(0.0256);
  cooling_curve_4.setMinimumValueofx(0.5);
  cooling_curve_4.setMaximumValueofx(1.5);

  CurveQuadratic cooling_curve_5 = CurveQuadratic(model);
  cooling_curve_5.setCoefficient1Constant(0.75);
  cooling_curve_5.setCoefficient2x(0.25);
  cooling_curve_5.setCoefficient3xPOW2(0.0);
  cooling_curve_5.setMinimumValueofx(0.0);
  cooling_curve_5.setMaximumValueofx(1.0);

  CurveBiquadratic cooling_curve_6 = CurveBiquadratic(model);
  cooling_curve_6.setCoefficient1Constant(0.42415);
  cooling_curve_6.setCoefficient2x(0.04426);
  cooling_curve_6.setCoefficient3xPOW2(-0.00042);
  cooling_curve_6.setCoefficient4y(0.00333);
  cooling_curve_6.setCoefficient5yPOW2(-0.00008);
  cooling_curve_6.setCoefficient6xTIMESY(-0.00021);
  cooling_curve_6.setMinimumValueofx(17.0);
  cooling_curve_6.setMaximumValueofx(22.0);
  cooling_curve_6.setMinimumValueofy(13.0);
  cooling_curve_6.setMaximumValueofy(46.0);

  CurveBiquadratic cooling_curve_7 = CurveBiquadratic(model);
  cooling_curve_7.setCoefficient1Constant(1.23649);
  cooling_curve_7.setCoefficient2x(-0.02431);
  cooling_curve_7.setCoefficient3xPOW2(0.00057);
  cooling_curve_7.setCoefficient4y(-0.01434);
  cooling_curve_7.setCoefficient5yPOW2(0.00063);
  cooling_curve_7.setCoefficient6xTIMESY(-0.00038);
  cooling_curve_7.setMinimumValueofx(17.0);
  cooling_curve_7.setMaximumValueofx(22.0);
  cooling_curve_7.setMinimumValueofy(13.0);
  cooling_curve_7.setMaximumValueofy(46.0);

  Schedule schedule = model.alwaysOnDiscreteSchedule();

  return CoilCoolingDXSingleSpeed(model, schedule, cooling_curve_1, cooling_curve_2, cooling_curve_3, cooling_curve_4, cooling_curve_5);
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Constructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan = FanConstantVolume(m,s);
    CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
    CoilCoolingDXSingleSpeed coolingCoil = makeCoolingCoil(m);
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass testObject = AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(m,fan,coolingCoil,heatingCoil);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_AddToNode)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan = FanConstantVolume(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingDXSingleSpeed coolingCoil = makeCoolingCoil(m);
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass testObject = AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(m,fan,coolingCoil,heatingCoil);

  AirLoopHVAC airLoopHVAC(m);
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  ASSERT_TRUE(testObject.addToNode(supplyOutletNode));
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Children)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan = FanConstantVolume(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingDXSingleSpeed coolingCoil = makeCoolingCoil(m);
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass testObject = AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(m,fan,coolingCoil,heatingCoil);

  ASSERT_EQ(heatingCoil,testObject.heatingCoil());
  ASSERT_EQ(coolingCoil,testObject.coolingCoil());
  ASSERT_EQ(fan,testObject.supplyAirFan());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Clone)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan = FanConstantVolume(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingDXSingleSpeed coolingCoil = makeCoolingCoil(m);
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass testObject = AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(m,fan,coolingCoil,heatingCoil);

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass clone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

  ASSERT_FALSE(clone.heatingCoil().handle().isNull());
  ASSERT_FALSE(clone.coolingCoil().handle().isNull());
  ASSERT_FALSE(clone.supplyAirFan().handle().isNull());
}


