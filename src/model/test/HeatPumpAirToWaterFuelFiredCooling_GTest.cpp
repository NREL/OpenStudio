/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"

#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_HeatPumpAirToWaterFuelFiredCooling) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatPumpAirToWaterFuelFiredCooling hp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatPumpAirToWaterFuelFiredCooling hp(m);

  EXPECT_FALSE(hp.companionHeatingHeatPump());
  EXPECT_EQ("NaturalGas", hp.fuelType());
  EXPECT_EQ("General", hp.endUseSubcategory());
  EXPECT_TRUE(hp.isEndUseSubcategoryDefaulted());
  EXPECT_FALSE(hp.nominalCoolingCapacity());
  EXPECT_TRUE(hp.isNominalCoolingCapacityAutosized());
  EXPECT_EQ(1.0, hp.nominalCOP());
  EXPECT_FALSE(hp.designFlowRate());
  EXPECT_TRUE(hp.isDesignFlowRateAutosized());
  EXPECT_EQ(7.0, hp.designSupplyTemperature());
  ASSERT_TRUE(hp.designTemperatureLift());
  EXPECT_EQ(11.1, hp.designTemperatureLift().get());
  EXPECT_FALSE(hp.isDesignTemperatureLiftAutosized());
  EXPECT_EQ(1.0, hp.sizingFactor());
  EXPECT_EQ("NotModulated", hp.flowMode());
  EXPECT_EQ("DryBulb", hp.outdoorAirTemperatureCurveInputVariable());
  EXPECT_EQ("EnteringEvaporator", hp.waterTemperatureCurveInputVariable());
  boost::optional<Curve> normalizedCapacityFunctionofTemperatureCurve = hp.normalizedCapacityFunctionofTemperatureCurve();
  EXPECT_TRUE(normalizedCapacityFunctionofTemperatureCurve);
  boost::optional<Curve> fuelEnergyInputRatioFunctionofTemperatureCurve = hp.fuelEnergyInputRatioFunctionofTemperatureCurve();
  EXPECT_TRUE(fuelEnergyInputRatioFunctionofTemperatureCurve);
  boost::optional<Curve> fuelEnergyInputRatioFunctionofPLRCurve = hp.fuelEnergyInputRatioFunctionofPLRCurve();
  EXPECT_TRUE(fuelEnergyInputRatioFunctionofPLRCurve);
  EXPECT_EQ(0.1, hp.minimumPartLoadRatio());
  EXPECT_EQ(1.0, hp.maximumPartLoadRatio());
  EXPECT_FALSE(hp.cyclingRatioFactorCurve());
  EXPECT_EQ(0, hp.nominalAuxiliaryElectricPower());
  EXPECT_FALSE(hp.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve());
  EXPECT_EQ(0, hp.standbyElectricPower());
}

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_GettersSetters) {
  Model m;
  HeatPumpAirToWaterFuelFiredCooling hp(m);

  HeatPumpAirToWaterFuelFiredHeating companionHP(m);
  EXPECT_TRUE(hp.setCompanionHeatingHeatPump(companionHP));
  EXPECT_TRUE(hp.setFuelType("Propane"));
  EXPECT_TRUE(hp.setEndUseSubcategory("AnyText"));
  EXPECT_TRUE(hp.setNominalCoolingCapacity(1.0));
  EXPECT_TRUE(hp.setNominalCOP(2.0));
  EXPECT_TRUE(hp.setDesignFlowRate(3.0));
  EXPECT_TRUE(hp.setDesignSupplyTemperature(4.0));
  EXPECT_TRUE(hp.setDesignTemperatureLift(5.0));
  EXPECT_TRUE(hp.setSizingFactor(6.0));
  EXPECT_TRUE(hp.setFlowMode("ConstantFlow"));
  EXPECT_TRUE(hp.setOutdoorAirTemperatureCurveInputVariable("WetBulb"));
  EXPECT_TRUE(hp.setWaterTemperatureCurveInputVariable("LeavingEvaporator"));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(hp.setNormalizedCapacityFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(hp.setFuelEnergyInputRatioFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(hp.setFuelEnergyInputRatioFunctionofPLRCurve(curve3));
  EXPECT_TRUE(hp.setMinimumPartLoadRatio(0.5));
  EXPECT_TRUE(hp.setMaximumPartLoadRatio(0.75));
  CurveQuadratic curve4(m);
  EXPECT_TRUE(hp.setCyclingRatioFactorCurve(curve4));
  EXPECT_TRUE(hp.setNominalAuxiliaryElectricPower(12.0));
  CurveBiquadratic curve5(m);
  EXPECT_TRUE(hp.setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(curve5));
  CurveQuadratic curve6(m);
  EXPECT_TRUE(hp.setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(curve6));
  EXPECT_TRUE(hp.setStandbyElectricPower(13.0));

  ASSERT_TRUE(hp.companionHeatingHeatPump());
  EXPECT_EQ(companionHP.handle(), hp.companionHeatingHeatPump().get().handle());
  EXPECT_EQ("Propane", hp.fuelType());
  EXPECT_EQ("AnyText", hp.endUseSubcategory());
  EXPECT_FALSE(hp.isEndUseSubcategoryDefaulted());
  ASSERT_TRUE(hp.nominalCoolingCapacity());
  EXPECT_EQ(1.0, hp.nominalCoolingCapacity().get());
  EXPECT_FALSE(hp.isNominalCoolingCapacityAutosized());
  EXPECT_EQ(2.0, hp.nominalCOP());
  ASSERT_TRUE(hp.designFlowRate());
  EXPECT_EQ(3.0, hp.designFlowRate().get());
  EXPECT_FALSE(hp.isDesignFlowRateAutosized());
  EXPECT_EQ(4.0, hp.designSupplyTemperature());
  ASSERT_TRUE(hp.designTemperatureLift());
  EXPECT_EQ(5.0, hp.designTemperatureLift().get());
  EXPECT_FALSE(hp.isDesignTemperatureLiftAutosized());
  EXPECT_EQ(6.0, hp.sizingFactor());
  EXPECT_EQ("ConstantFlow", hp.flowMode());
  EXPECT_EQ("WetBulb", hp.outdoorAirTemperatureCurveInputVariable());
  EXPECT_EQ("LeavingEvaporator", hp.waterTemperatureCurveInputVariable());
  EXPECT_EQ(curve1.handle(), hp.normalizedCapacityFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve2.handle(), hp.fuelEnergyInputRatioFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve3.handle(), hp.fuelEnergyInputRatioFunctionofPLRCurve().handle());
  EXPECT_EQ(0.5, hp.minimumPartLoadRatio());
  EXPECT_EQ(0.75, hp.maximumPartLoadRatio());
  ASSERT_TRUE(hp.cyclingRatioFactorCurve());
  EXPECT_EQ(curve4.handle(), hp.cyclingRatioFactorCurve().get().handle());
  EXPECT_EQ(12.0, hp.nominalAuxiliaryElectricPower());
  ASSERT_TRUE(hp.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(curve5.handle(), hp.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve().get().handle());
  ASSERT_TRUE(hp.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve());
  EXPECT_EQ(curve6.handle(), hp.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve().get().handle());
  EXPECT_EQ(13.0, hp.standbyElectricPower());

  hp.autosizeNominalCoolingCapacity();
  hp.autosizeDesignFlowRate();
  hp.autosizeDesignTemperatureLift();

  EXPECT_FALSE(hp.nominalCoolingCapacity());
  EXPECT_TRUE(hp.isNominalCoolingCapacityAutosized());
  EXPECT_FALSE(hp.designFlowRate());
  EXPECT_TRUE(hp.isDesignFlowRateAutosized());
  EXPECT_FALSE(hp.designTemperatureLift());
  EXPECT_TRUE(hp.isDesignTemperatureLiftAutosized());

  hp.resetCompanionHeatingHeatPump();
  hp.resetEndUseSubcategory();
  hp.resetNominalCoolingCapacity();
  hp.resetDesignFlowRate();
  hp.resetCyclingRatioFactorCurve();
  hp.resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
  hp.resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();

  EXPECT_FALSE(hp.companionHeatingHeatPump());
  EXPECT_EQ("General", hp.endUseSubcategory());
  EXPECT_TRUE(hp.isEndUseSubcategoryDefaulted());
  EXPECT_FALSE(hp.nominalCoolingCapacity());
  EXPECT_FALSE(hp.designFlowRate());
  EXPECT_FALSE(hp.cyclingRatioFactorCurve());
  EXPECT_FALSE(hp.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve());
}

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_remove) {
  Model m;
  HeatPumpAirToWaterFuelFiredCooling hp(m);

  HeatPumpAirToWaterFuelFiredHeating companionHP(m);
  EXPECT_TRUE(hp.setCompanionHeatingHeatPump(companionHP));
  EXPECT_TRUE(companionHP.setCompanionCoolingHeatPump(hp));

  auto size = m.modelObjects().size();
  EXPECT_FALSE(hp.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());
  EXPECT_FALSE(companionHP.companionCoolingHeatPump());
  EXPECT_EQ(0, m.getConcreteModelObjects<HeatPumpAirToWaterFuelFiredCooling>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<HeatPumpAirToWaterFuelFiredHeating>().size());
}

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_clone) {
  Model m;
  CurveBiquadratic curve1(m);
  CurveBiquadratic curve2(m);
  CurveQuadratic curve3(m);
  HeatPumpAirToWaterFuelFiredCooling hp(m, curve1, curve2, curve3);
  EXPECT_EQ(2u, m.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CurveQuadratic>().size());

  HeatPumpAirToWaterFuelFiredHeating companionHP(m);
  EXPECT_TRUE(hp.setCompanionHeatingHeatPump(companionHP));
  EXPECT_TRUE(companionHP.setCompanionCoolingHeatPump(hp));

  EXPECT_EQ(4u, m.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<CurveQuadratic>().size());

  PlantLoop p(m);
  EXPECT_TRUE(p.addSupplyBranchForComponent(hp));
  EXPECT_TRUE(hp.inletModelObject());
  EXPECT_TRUE(hp.outletModelObject());
  EXPECT_TRUE(hp.plantLoop());

  {
    Model m2;
    auto hpClone = hp.clone(m2).cast<HeatPumpAirToWaterFuelFiredCooling>();
    EXPECT_FALSE(hpClone.companionHeatingHeatPump());
    EXPECT_FALSE(hpClone.inletModelObject());
    EXPECT_FALSE(hpClone.outletModelObject());
    EXPECT_FALSE(hpClone.plantLoop());
    EXPECT_NE(curve1.handle(), hpClone.normalizedCapacityFunctionofTemperatureCurve().handle());
    EXPECT_NE(curve2.handle(), hpClone.fuelEnergyInputRatioFunctionofTemperatureCurve().handle());
    EXPECT_NE(curve3.handle(), hpClone.fuelEnergyInputRatioFunctionofPLRCurve().handle());
    EXPECT_EQ(2u, m2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<CurveQuadratic>().size());
  }

  {
    auto hpClone = hp.clone(m).cast<HeatPumpAirToWaterFuelFiredCooling>();
    EXPECT_FALSE(hpClone.companionHeatingHeatPump());
    EXPECT_FALSE(hpClone.inletModelObject());
    EXPECT_FALSE(hpClone.outletModelObject());
    EXPECT_FALSE(hpClone.plantLoop());
    EXPECT_EQ(curve1.handle(), hpClone.normalizedCapacityFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hpClone.fuelEnergyInputRatioFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hpClone.fuelEnergyInputRatioFunctionofPLRCurve().handle());
    EXPECT_EQ(4u, m.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<CurveQuadratic>().size());
  }
}

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_addToNode) {
  Model m;
  HeatPumpAirToWaterFuelFiredCooling hp(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(hp.addToNode(supplyOutletNode));
  EXPECT_EQ(2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(hp.addToNode(inletNode));
  EXPECT_EQ(5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(hp.addToNode(supplyOutletNode));
  EXPECT_EQ(7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(hp.addToNode(demandOutletNode));
  EXPECT_EQ(5, plantLoop.demandComponents().size());

  auto hpClone = hp.clone(m).cast<HeatPumpAirToWaterFuelFiredCooling>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(hpClone.addToNode(supplyOutletNode));
  EXPECT_EQ(9, plantLoop.supplyComponents().size());
}
