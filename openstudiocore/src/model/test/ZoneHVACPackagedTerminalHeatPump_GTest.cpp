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

#include "../AirLoopHVAC.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include <utilities/idd/OS_Curve_Biquadratic_FieldEnums.hxx>
#include "../../utilities/units/Quantity.hpp"

using namespace openstudio;

using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACPackagedTerminalHeatPump_ZoneHVACPackagedTerminalHeatPump)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     model::Model m;

     model::ScheduleCompact availabilitySchedule(m);

     model::FanConstantVolume fan(m,availabilitySchedule);

     model::CoilHeatingElectric supplementalHeatingCoil(m,availabilitySchedule);

     model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(m);
     coolingCurveFofTemp.setCoefficient1Constant(0.42415);
     coolingCurveFofTemp.setCoefficient2x(0.04426);
     coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
     coolingCurveFofTemp.setCoefficient4y(0.00333);
     coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
     coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
     coolingCurveFofTemp.setMinimumValueofx(17.0);
     coolingCurveFofTemp.setMaximumValueofx(22.0);
     coolingCurveFofTemp.setMinimumValueofy(13.0);
     coolingCurveFofTemp.setMaximumValueofy(46.0);

     CurveQuadratic coolingCurveFofFlow = CurveQuadratic(m);
     coolingCurveFofFlow.setCoefficient1Constant(0.77136);
     coolingCurveFofFlow.setCoefficient2x(0.34053);
     coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
     coolingCurveFofFlow.setMinimumValueofx(0.75918);
     coolingCurveFofFlow.setMaximumValueofx(1.13877);

     CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(m);
     energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
     energyInputRatioFofTemp.setCoefficient2x(-0.02431);
     energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
     energyInputRatioFofTemp.setCoefficient4y(-0.01434);
     energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
     energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
     energyInputRatioFofTemp.setMinimumValueofx(17.0);
     energyInputRatioFofTemp.setMaximumValueofx(22.0);
     energyInputRatioFofTemp.setMinimumValueofy(13.0);
     energyInputRatioFofTemp.setMaximumValueofy(46.0);

     CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(m);
     energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
     energyInputRatioFofFlow.setCoefficient2x(-0.32953);
     energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
     energyInputRatioFofFlow.setMinimumValueofx(0.75918);
     energyInputRatioFofFlow.setMaximumValueofx(1.13877);

     CurveQuadratic partLoadFraction = CurveQuadratic(m);
     partLoadFraction.setCoefficient1Constant(0.77100);
     partLoadFraction.setCoefficient2x(0.22900);
     partLoadFraction.setCoefficient3xPOW2(0.0);
     partLoadFraction.setMinimumValueofx(0.0);
     partLoadFraction.setMaximumValueofx(1.0);

     CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( m,
                                                                      availabilitySchedule,
                                                                      coolingCurveFofTemp,
                                                                      coolingCurveFofFlow,
                                                                      energyInputRatioFofTemp,
                                                                      energyInputRatioFofFlow,
                                                                      partLoadFraction );

     CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.0);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4y(0.027626);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000148716);
     totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);

     CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
     totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
     totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
     totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
     totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
     totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

     CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
     energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
     energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
     energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
     energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.0);
     energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
     energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

     CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
     energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
     energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
     energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
     energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
     energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

     CurveQuadratic  partLoadFractionCorrelationCurve(m);
     partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
     partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
     partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
     partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
     partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

     CoilHeatingDXSingleSpeed heatingCoil( m,
                                           availabilitySchedule,
                                           totalHeatingCapacityFunctionofTemperatureCurve,
                                           totalHeatingCapacityFunctionofFlowFractionCurve,
                                           energyInputRatioFunctionofTemperatureCurve,
                                           energyInputRatioFunctionofFlowFractionCurve,
                                           partLoadFractionCorrelationCurve );

     model::ZoneHVACPackagedTerminalHeatPump pthp( m,
                                                   availabilitySchedule,
                                                   fan,
                                                   heatingCoil,
                                                   coolingCoil,
                                                   supplementalHeatingCoil );

     pthp.availabilitySchedule();

     pthp.supplyAirFan();

     pthp.heatingCoil();

     pthp.coolingCoil();

     pthp.supplementalHeatingCoil();

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,ZoneHVACPackagedTerminalHeatPump_Clone)
{
  model::Model m;

  model::ScheduleCompact availabilitySchedule(m);

  model::FanConstantVolume fan(m,availabilitySchedule);

  model::CoilHeatingElectric supplementalHeatingCoil(m,availabilitySchedule);

  model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(m);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(m);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(m);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(m);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(m);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( m,
                                                                   availabilitySchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );

  CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4y(0.027626);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000148716);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);

  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient1Constant,true));
  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient2x,true));
  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient3x_POW_2,true));
  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient4y,true));
  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient5y_POW_2,true));
  //ASSERT_NO_THROW(totalHeatingCapacityFunctionofTemperatureCurve.getQuantity(
  //  OS_Curve_BiquadraticFields::Coefficient6x_TIMES_y,true));

  CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

  CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.0);
  energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
  energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
  energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

  CurveQuadratic  partLoadFractionCorrelationCurve(m);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

  CoilHeatingDXSingleSpeed heatingCoil( m,
                                        availabilitySchedule,
                                        totalHeatingCapacityFunctionofTemperatureCurve,
                                        totalHeatingCapacityFunctionofFlowFractionCurve,
                                        energyInputRatioFunctionofTemperatureCurve,
                                        energyInputRatioFunctionofFlowFractionCurve,
                                        partLoadFractionCorrelationCurve );

  model::ZoneHVACPackagedTerminalHeatPump pthp( m,
                                                availabilitySchedule,
                                                fan,
                                                heatingCoil,
                                                coolingCoil,
                                                supplementalHeatingCoil );

  model::Model m2;

  model::ZoneHVACPackagedTerminalHeatPump pthp2 = pthp.clone(m2).cast<model::ZoneHVACPackagedTerminalHeatPump>();

  pthp.remove();

  pthp2.remove();
}

