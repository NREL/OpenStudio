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
#include <model/AirLoopHVAC.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/CoilHeatingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveQuadratic.hpp>

using namespace openstudio;

TEST(AirLoopHVACUnitaryHeatPumpAirToAir,AirLoopHVACUnitaryHeatPumpAirToAir_AirLoopHVACUnitaryHeatPumpAirToAir)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::ScheduleCompact unitarySchedule(m);

     model::ScheduleCompact fanSchedule(m);

     model::ScheduleCompact heatingCoilSchedule(m);

     model::ScheduleCompact coolingCoilSchedule(m);

     model::FanConstantVolume supplyFan(m,fanSchedule);

     model::CurveBiquadratic  ctotalHeatingCapacityFunctionofTemperatureCurve(m);
     model::CurveQuadratic  ctotalHeatingCapacityFunctionofFlowFractionCurve(m);
     model::CurveBiquadratic  cenergyInputRatioFunctionofTemperatureCurve(m);
     model::CurveQuadratic  cenergyInputRatioFunctionofFlowFractionCurve(m);
     model::CurveQuadratic  cpartLoadFractionCorrelationCurve(m);

     model::CoilHeatingDXSingleSpeed coolingCoil( m,
                                           coolingCoilSchedule,
                                           ctotalHeatingCapacityFunctionofTemperatureCurve,
                                           ctotalHeatingCapacityFunctionofFlowFractionCurve,
                                           cenergyInputRatioFunctionofTemperatureCurve,
                                           cenergyInputRatioFunctionofFlowFractionCurve,
                                           cpartLoadFractionCorrelationCurve ); 

     model::CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
     model::CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
     model::CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
     model::CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
     model::CurveQuadratic  partLoadFractionCorrelationCurve(m);

     model::CoilHeatingDXSingleSpeed heatingCoil( m,
                                                  heatingCoilSchedule,
                                                  totalHeatingCapacityFunctionofTemperatureCurve,
                                                  totalHeatingCapacityFunctionofFlowFractionCurve,
                                                  energyInputRatioFunctionofTemperatureCurve,
                                                  energyInputRatioFunctionofFlowFractionCurve,
                                                  partLoadFractionCorrelationCurve ); 

     model::CoilHeatingElectric coilHeatingElectric( m,heatingCoilSchedule);

     model::AirLoopHVACUnitaryHeatPumpAirToAir coil( m,
                                                     unitarySchedule,
                                                     supplyFan,
                                                     heatingCoil,
                                                     coolingCoil,
                                                     coilHeatingElectric );

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}


