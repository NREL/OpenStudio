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

#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed_Impl.hpp>
#include <model/ScheduleConstant.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveQuadratic.hpp>


using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilCoolingDXSingleSpeed_RatedTotalCoolingCapacity_Quantity) {
  Model model;
  
  ScheduleConstant scheduleConstant(model);
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
  CurveQuadratic partLoadFractionCorrelationCurve(model);

  CoilCoolingDXSingleSpeed coil(model,
                                scheduleConstant,
                                totalCoolingCapacityFunctionofTemperatureCurve,
                                totalCoolingCapacityFunctionofFlowFractionCurve,
                                energyInputRatioFunctionofTemperatureCurve,
                                energyInputRatioFunctionofFlowFractionCurve,
                                partLoadFractionCorrelationCurve);

  coil.setRatedTotalCoolingCapacity(1.0);
  coil.setRatedSensibleHeatRatio(0.6);
  coil.setRatedCOP(3.6);
  coil.setRatedAirFlowRate(1.1);
  coil.setRatedEvaporatorFanPowerPerVolumeFlowRate(600.0);
  coil.setNominalTimeForCondensateRemovalToBegin(50.0);
  coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(3.0);
  coil.setMaximumCyclingRate(3.2);
  coil.setLatentCapacityTimeConstant(499.8);
  coil.setEvaporativeCondenserEffectiveness(0.8);
  coil.setEvaporativeCondenserAirFlowRate(1.2);
  coil.setEvaporativeCondenserPumpRatedPowerConsumption(1.3);
  coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(1.4);
  coil.setBasinHeaterCapacity(1.5);
  coil.setBasinHeaterSetpointTemperature(2.5);
  
  EXPECT_EQ(coil.ratedTotalCoolingCapacity(),1.0);
  EXPECT_EQ(coil.ratedSensibleHeatRatio(),0.6);
  EXPECT_EQ(coil.ratedCOP(),3.6);
  EXPECT_EQ(coil.ratedAirFlowRate(),1.1);
  EXPECT_EQ(coil.ratedEvaporatorFanPowerPerVolumeFlowRate(),600.0);
  EXPECT_EQ(coil.nominalTimeForCondensateRemovalToBegin(),50.0);
  EXPECT_EQ(coil.ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(),3.0);
  EXPECT_EQ(coil.maximumCyclingRate(),3.2);
  EXPECT_EQ(coil.latentCapacityTimeConstant(),499.8);
  EXPECT_EQ(coil.evaporativeCondenserEffectiveness(),0.8);
  EXPECT_EQ(coil.evaporativeCondenserAirFlowRate(),1.2);
  EXPECT_EQ(coil.evaporativeCondenserPumpRatedPowerConsumption(),1.3);
  EXPECT_EQ(coil.maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(),1.4);
  EXPECT_EQ(coil.basinHeaterCapacity(),1.5);
  EXPECT_EQ(coil.basinHeaterSetpointTemperature(),2.5);
  
}
