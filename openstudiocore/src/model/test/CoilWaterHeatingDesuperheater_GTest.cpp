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
#include "../CoilWaterHeatingDesuperheater.hpp"
#include "../CoilWaterHeatingDesuperheater_Impl.hpp"
#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../WaterHeaterMixed.hpp"
#include "../WaterHeaterMixed_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

//Test construction of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_DefaultConstructor)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    ASSERT_EXIT (
    {
        Model model;
        ScheduleCompact schedule = ScheduleCompact(model);
        CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);

        exit(0);
    } ,
        ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_Remove)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
    WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
    CurveBiquadratic curve = CurveBiquadratic(model);
    testObject.setHeatingSource(condenser);
    testObject.addToHeatRejectionTarget(heatRejection);
    testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve);

    std::vector<CoilWaterHeatingDesuperheater> coilWaterHeatingDesuperheaters = model.getModelObjects<CoilWaterHeatingDesuperheater>();
    EXPECT_EQ(1, coilWaterHeatingDesuperheaters.size());

    std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
    EXPECT_EQ(1, refrigerationCondensers.size());

    std::vector<WaterHeaterMixed> heatRejectionTargets = model.getModelObjects<WaterHeaterMixed>();
    EXPECT_EQ(1, heatRejectionTargets.size());

    std::vector<CurveBiquadratic> curves = model.getModelObjects<CurveBiquadratic>();
    EXPECT_EQ(1, curves.size());

    std::vector<Node> nodes = model.getModelObjects<Node>();
    EXPECT_EQ(2, nodes.size());

    testObject.remove();

    coilWaterHeatingDesuperheaters = model.getModelObjects<CoilWaterHeatingDesuperheater>();
    EXPECT_EQ(0, coilWaterHeatingDesuperheaters.size());

    refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
    EXPECT_EQ(1, refrigerationCondensers.size());

    heatRejectionTargets = model.getModelObjects<WaterHeaterMixed>();
    EXPECT_EQ(1, heatRejectionTargets.size());

    curves = model.getModelObjects<CurveBiquadratic>();
    EXPECT_EQ(0, curves.size());

	nodes = model.getModelObjects<Node>();
    EXPECT_EQ(0, nodes.size());

    EXPECT_FALSE(heatRejection.demandInletModelObject());
    EXPECT_FALSE(heatRejection.demandOutletModelObject());
}


TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_addToNode)
{
  Model m;
  ScheduleCompact s(m);
  CoilWaterHeatingDesuperheater testObject(m, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddBranchForHVACComponent_AirLoop)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater desuperheater(model, schedule);
    AirLoopHVAC airLoop = AirLoopHVAC(model);

    EXPECT_FALSE(airLoop.addBranchForHVACComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddDemandBranchForComponent_PlantLoop)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater desuperheater(model, schedule);
    PlantLoop plantLoop = PlantLoop(model);

    EXPECT_FALSE(plantLoop.addDemandBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddSupplyBranchForComponent_PlantLoop)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater desuperheater(model, schedule);
    PlantLoop plantLoop = PlantLoop(model);

    EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneModelWithDefaultData)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);

    CoilWaterHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(5.0, testObjectClone.deadBandTemperatureDifference());
    EXPECT_DOUBLE_EQ(0.8, testObjectClone.ratedHeatReclaimRecoveryEfficiency().get());
    EXPECT_DOUBLE_EQ(50.0, testObjectClone.ratedInletWaterTemperature());
    EXPECT_DOUBLE_EQ(35.0, testObjectClone.ratedOutdoorAirTemperature());
    EXPECT_DOUBLE_EQ(60.0, testObjectClone.maximumInletWaterTemperatureforHeatReclaim());
    EXPECT_DOUBLE_EQ(0.0001, testObjectClone.waterFlowRate());
    EXPECT_DOUBLE_EQ(100.0, testObjectClone.waterPumpPower());
    EXPECT_DOUBLE_EQ(0.2, testObjectClone.fractionofPumpHeattoWater());
    EXPECT_DOUBLE_EQ(0.0, testObjectClone.onCycleParasiticElectricLoad());
    EXPECT_DOUBLE_EQ(0.0, testObjectClone.offCycleParasiticElectricLoad());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneModelWithCustomData)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
    WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
    CurveBiquadratic curve = CurveBiquadratic(model);

    testObject.setDeadBandTemperatureDifference(20.0);
    testObject.setRatedHeatReclaimRecoveryEfficiency(0.99);
    testObject.setRatedInletWaterTemperature(999.0);
    testObject.setRatedOutdoorAirTemperature(999.0);
    testObject.setMaximumInletWaterTemperatureforHeatReclaim(999.0);
    testObject.setWaterFlowRate(999.0);
    testObject.setWaterPumpPower(999.0);
    testObject.setFractionofPumpHeattoWater(0.99);
    testObject.setOnCycleParasiticElectricLoad(999.0);
    testObject.setOffCycleParasiticElectricLoad(999.0);
    testObject.setHeatingSource(condenser);
    testObject.addToHeatRejectionTarget(heatRejection);
    testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve);

    CoilWaterHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(20.0, testObjectClone.deadBandTemperatureDifference());
    EXPECT_DOUBLE_EQ(0.99, testObjectClone.ratedHeatReclaimRecoveryEfficiency().get());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedInletWaterTemperature());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedOutdoorAirTemperature());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumInletWaterTemperatureforHeatReclaim());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.waterFlowRate());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.waterPumpPower());
    EXPECT_DOUBLE_EQ(0.99, testObjectClone.fractionofPumpHeattoWater());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.onCycleParasiticElectricLoad());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone.offCycleParasiticElectricLoad());
    EXPECT_FALSE(testObjectClone.heatingSource());
    EXPECT_FALSE(testObjectClone.heatRejectionTarget());
    ASSERT_TRUE(testObjectClone.heatReclaimEfficiencyFunctionofTemperatureCurve());
    EXPECT_NE(curve, testObjectClone.heatReclaimEfficiencyFunctionofTemperatureCurve().get());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneTwoModelWithCustomData)
{
    Model model;
    ScheduleCompact schedule = ScheduleCompact(model);
    CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
    WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
    CurveBiquadratic curve = CurveBiquadratic(model);

    testObject.setDeadBandTemperatureDifference(20.0);
    testObject.setRatedHeatReclaimRecoveryEfficiency(0.99);
    testObject.setRatedInletWaterTemperature(999.0);
    testObject.setRatedOutdoorAirTemperature(999.0);
    testObject.setMaximumInletWaterTemperatureforHeatReclaim(999.0);
    testObject.setWaterFlowRate(999.0);
    testObject.setWaterPumpPower(999.0);
    testObject.setFractionofPumpHeattoWater(0.99);
    testObject.setOnCycleParasiticElectricLoad(999.0);
    testObject.setOffCycleParasiticElectricLoad(999.0);
    testObject.setHeatingSource(condenser);
    testObject.addToHeatRejectionTarget(heatRejection);
    testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve);

    CoilWaterHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

    Model model2;
    CoilWaterHeatingDesuperheater testObjectClone2 = testObject.clone(model2).cast<CoilWaterHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(20.0, testObjectClone2.deadBandTemperatureDifference());
    EXPECT_DOUBLE_EQ(0.99, testObjectClone2.ratedHeatReclaimRecoveryEfficiency().get());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedInletWaterTemperature());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedOutdoorAirTemperature());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.maximumInletWaterTemperatureforHeatReclaim());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.waterFlowRate());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.waterPumpPower());
    EXPECT_DOUBLE_EQ(0.99, testObjectClone2.fractionofPumpHeattoWater());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.onCycleParasiticElectricLoad());
    EXPECT_DOUBLE_EQ(999.0, testObjectClone2.offCycleParasiticElectricLoad());
    EXPECT_FALSE(testObjectClone2.heatingSource());
    EXPECT_FALSE(testObjectClone2.heatRejectionTarget());
    ASSERT_TRUE(testObjectClone2.heatReclaimEfficiencyFunctionofTemperatureCurve());
    EXPECT_NE(curve, testObjectClone2.heatReclaimEfficiencyFunctionofTemperatureCurve().get());
    EXPECT_NE(testObjectClone2, testObjectClone);
    EXPECT_NE(testObjectClone, testObject);
}

