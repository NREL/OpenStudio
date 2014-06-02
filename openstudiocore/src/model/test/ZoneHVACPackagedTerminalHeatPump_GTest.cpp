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

TEST(ZoneHVACPackagedTerminalHeatPump,ZoneHVACPackagedTerminalHeatPump_ZoneHVACPackagedTerminalHeatPump)
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

