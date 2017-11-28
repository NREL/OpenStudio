/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirTerminalSingleDuctUncontrolled.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../FanConstantVolume.hpp"
#include "../LifeCycleCost.hpp"
#include "../LifeCycleCost_Impl.hpp"
#include "../Lights.hpp"
#include "../LightsDefinition.hpp"
#include "../Model_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SizingZone.hpp"
#include "../SizingZone_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../StraightComponent.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../ThermostatSetpointDualSetpoint.hpp"
#include "../ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../ZoneControlHumidistat.hpp"
#include "../ZoneControlHumidistat_Impl.hpp"
#include "../ZoneControlContaminantController.hpp"
#include "../ZoneControlContaminantController_Impl.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"


#include "../ScheduleConstant.hpp"
#include "../ZoneHVACUnitHeater.hpp"
#include "../ZoneHVACUnitHeater_Impl.hpp"

#include "../../utilities/core/Containers.hpp"
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ThermalZone_Spaces)
{
  Model model;
  ThermalZone thermalZone(model);
  Space space1(model);
  Space space2(model);

  EXPECT_FALSE(space1.thermalZone());
  EXPECT_FALSE(space2.thermalZone());
  EXPECT_EQ(0u, thermalZone.spaces().size());

  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  ASSERT_TRUE(space1.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space1.thermalZone()->handle());
  EXPECT_FALSE(space2.thermalZone());
  EXPECT_EQ(1u, thermalZone.spaces().size());

  ASSERT_TRUE(space1.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space1.thermalZone()->handle());
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  ASSERT_TRUE(space2.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space2.thermalZone()->handle());
  EXPECT_EQ(2u, thermalZone.spaces().size());
}

TEST_F(ModelFixture,ThermalZone_Remove)
{
  Model model;
  ThermalZone zone1(model);
  AirLoopHVAC airLoopHVAC(model);
  std::vector<ModelObject> modelObjects;

  modelObjects = airLoopHVAC.demandComponents();
  EXPECT_EQ(5u,modelObjects.size());

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone1, boost::none));

  modelObjects = airLoopHVAC.demandComponents();
  EXPECT_EQ(7u,modelObjects.size());
  ThermalZoneVector thermalZones = subsetCastVector<ThermalZone>(modelObjects);
  ASSERT_EQ(1u,thermalZones.size());
  EXPECT_EQ(zone1.handle(), thermalZones[0].handle());

  ASSERT_NO_THROW(zone1.remove());

  modelObjects = airLoopHVAC.demandComponents();
  EXPECT_EQ(5u,modelObjects.size());
}

TEST_F(ModelFixture,ThermalZone_AddToNode_SPM)
{
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  ThermalZone thermalZone(m);
  ThermalZone thermalZone2(m);
  ScheduleCompact s(m);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal(m,s);
  SetpointManagerSingleZoneReheat spm(m);

  Node outletNode = airLoopHVAC.supplyOutletNode();
  spm.addToNode(outletNode);
  EXPECT_FALSE(spm.controlZone());

  Node inletNode = airLoopHVAC.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(thermalZone.addToNode(inletNode));

  EXPECT_TRUE(spm.controlZone());
  EXPECT_EQ(thermalZone, spm.controlZone());

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,singleDuctTerminal));

  EXPECT_TRUE(spm.controlZone());
  EXPECT_EQ(thermalZone, spm.controlZone());
  EXPECT_NE(thermalZone2, spm.controlZone());
}

TEST_F(ModelFixture,ThermalZone_sizingZone)
{
  Model model;
  ThermalZone zone1(model);

  ASSERT_NO_THROW(zone1.sizingZone());
}

TEST_F(ModelFixture,ThermalZone_CeilingHeight_Quantity) {
  Model model;
  ThermalZone thermalZone(model);

  Unit units = thermalZone.getCeilingHeight(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(thermalZone.setCeilingHeight(testQ));
  OSOptionalQuantity q = thermalZone.getCeilingHeight(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.get().units().standardString());
}

TEST_F(ModelFixture,ThermalZone_Volume_Quantity) {
  Model model;
  ThermalZone thermalZone(model);

  Unit units = thermalZone.getVolume(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(thermalZone.setVolume(testQ));
  OSOptionalQuantity q = thermalZone.getVolume(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.get().units().standardString());
}

TEST_F(ModelFixture,ThermalZone_FractionofZoneControlledbyPrimaryDaylightingControl_Quantity) {
  Model model;
  ThermalZone thermalZone(model);

  Unit units = thermalZone.getFractionofZoneControlledbyPrimaryDaylightingControl(true).units(); // Get IP units.
  double value(0.5);
  Quantity testQ(value,units);
  EXPECT_TRUE(thermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(testQ));
  Quantity q = thermalZone.getFractionofZoneControlledbyPrimaryDaylightingControl(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ThermalZone_FractionofZoneControlledbySecondaryDaylightingControl_Quantity) {
  Model model;
  ThermalZone thermalZone(model);

  Unit units = thermalZone.getFractionofZoneControlledbySecondaryDaylightingControl(true).units(); // Get IP units.
  double value(0.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(thermalZone.setFractionofZoneControlledbySecondaryDaylightingControl(testQ));
  Quantity q = thermalZone.getFractionofZoneControlledbySecondaryDaylightingControl(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture,ThermalZone_FractionofZoneControlledbyDaylightingControl_PriSecLimits) {
  Model m;
  ThermalZone z(m);

  // As Fraction
  ASSERT_TRUE(z.setFractionofZoneControlledbyPrimaryDaylightingControl(0.5));
  ASSERT_TRUE(z.setFractionofZoneControlledbySecondaryDaylightingControl(0.5));
  ASSERT_FALSE(z.setFractionofZoneControlledbySecondaryDaylightingControl(0.75));
  ASSERT_FALSE(z.setFractionofZoneControlledbyPrimaryDaylightingControl(0.75));

  // As Quantity
  Unit units = z.getFractionofZoneControlledbySecondaryDaylightingControl(true).units(); // Get IP units.
  double value(0.5);
  double value2(0.7);
  Quantity testQ(value, units);
  Quantity testQ2(value2, units);

  ASSERT_TRUE(z.setFractionofZoneControlledbyPrimaryDaylightingControl(testQ));
  ASSERT_TRUE(z.setFractionofZoneControlledbySecondaryDaylightingControl(testQ));
  ASSERT_FALSE(z.setFractionofZoneControlledbyPrimaryDaylightingControl(testQ2));
  ASSERT_FALSE(z.setFractionofZoneControlledbySecondaryDaylightingControl(testQ2));

}


TEST_F(ModelFixture, CombinedInfiltration) {
  Model model;
  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points;
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));

  // 100 m^2, 200 m^2
  boost::optional<Space> space1 = Space::fromFloorPrint(points, 2, model);
  ASSERT_TRUE(space1);
  EXPECT_EQ(200.0, space1->volume());
  space1->setThermalZone(thermalZone);

  SpaceInfiltrationDesignFlowRate infiltration1(model);
  infiltration1.setAirChangesperHour(1.0);
  infiltration1.setSpace(*space1);

  // 100 m^2, 300 m^2
  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  EXPECT_EQ(300.0, space2->volume());
  space2->setThermalZone(thermalZone);

  SpaceInfiltrationDesignFlowRate infiltration2(model);
  infiltration2.setAirChangesperHour(1.0);
  infiltration2.setSpace(*space2);

  boost::optional<Space> newSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(newSpace);
  std::vector<SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates = newSpace->spaceInfiltrationDesignFlowRates();
  ASSERT_EQ(2u, spaceInfiltrationDesignFlowRates.size());
  EXPECT_EQ("Flow/Space", spaceInfiltrationDesignFlowRates[0].designFlowRateCalculationMethod());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRates[0].designFlowRate());
  EXPECT_EQ("Flow/Space", spaceInfiltrationDesignFlowRates[1].designFlowRateCalculationMethod());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRates[1].designFlowRate());

  double totalRate = 0;
  totalRate += spaceInfiltrationDesignFlowRates[0].designFlowRate().get();
  totalRate += spaceInfiltrationDesignFlowRates[1].designFlowRate().get();
  EXPECT_EQ(500.0/3600.0, totalRate);

}

TEST_F(ModelFixture,ThermalZone_ThermalZone) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;

    ThermalZone thermalZone(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,ThermalZone_equipment) {
  Model model;

  ThermalZone thermalZone(model);

  AirLoopHVAC airLoopHVAC(model);

  ScheduleCompact scheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal(model,scheduleCompact);

  airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal);

  EXPECT_EQ(1u,thermalZone.equipment().size());

  FanConstantVolume fan(model,scheduleCompact);

  CoilHeatingWater heatingCoil(model,scheduleCompact);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
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

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
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

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( model,
                                                                   scheduleCompact,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );


  ZoneHVACPackagedTerminalAirConditioner ptac( model,
                                               scheduleCompact,
                                               fan,
                                               heatingCoil,
                                               coolingCoil );

  EXPECT_TRUE(ptac.addToThermalZone(thermalZone));

  EXPECT_EQ(2u,thermalZone.equipment().size());
}

TEST_F(ModelFixture,ThermalZone_Cost) {
  Model model;
  ThermalZone thermalZone(model);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Controls Upgrade", thermalZone, 10.0, "CostPerEach", "Operation", 0, 10);
  ASSERT_TRUE(cost);
  EXPECT_EQ(0, cost->repeatPeriodYears());
  EXPECT_TRUE(cost->isRepeatPeriodYearsDefaulted());
  EXPECT_EQ(10, cost->yearsFromStart());
  EXPECT_FALSE(cost->isYearsFromStartDefaulted());

  EXPECT_DOUBLE_EQ(10.0, cost->totalCost());

  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  cost = LifeCycleCost::createLifeCycleCost("Controls Upgrade", thermalZone, 10.0, "CostPerArea", "Operation", 0, 10);
  EXPECT_TRUE(cost);
  cost = LifeCycleCost::createLifeCycleCost("Controls Upgrade", thermalZone, 10.0, "CostPerThermalZone", "Operation", 0, 10);
  EXPECT_FALSE(cost);

  EXPECT_EQ(2u, thermalZone.lifeCycleCosts().size());

  thermalZone.remove();

  EXPECT_EQ(0u, model.getModelObjects<LifeCycleCost>().size());
}

TEST_F(ModelFixture, ThermalZone_CombineSpaces_Cost)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Cost", *space1, 1, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Cost", *space2, 1, "CostPerArea", "Construction");
  ASSERT_TRUE(cost2);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  space2->setThermalZone(thermalZone);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());

  boost::optional<Space> newSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(newSpace);
  ASSERT_EQ(2u, model.getModelObjects<LifeCycleCost>().size());
  ASSERT_EQ(2u, newSpace->lifeCycleCosts().size());
  EXPECT_EQ("CostPerEach", newSpace->lifeCycleCosts()[0].costUnits());
  EXPECT_EQ(100, newSpace->lifeCycleCosts()[0].totalCost());
  EXPECT_EQ("CostPerEach", newSpace->lifeCycleCosts()[1].costUnits());
  EXPECT_EQ(100, newSpace->lifeCycleCosts()[1].totalCost());
}

TEST_F(ModelFixture, ThermalZone_CombineSpaces_Cost2)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  LightsDefinition definition(model);
  Lights lights1(definition);
  lights1.setSpace(*space1);
  Lights lights2(definition);
  lights2.setSpace(*space2);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Cost", definition, 1, "CostPerArea", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(200, cost->totalCost());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  space2->setThermalZone(thermalZone);

  EXPECT_DOUBLE_EQ(200, cost->totalCost());

  boost::optional<Space> newSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(newSpace);
  ASSERT_EQ(2u, model.getModelObjects<LifeCycleCost>().size());
  EXPECT_EQ(0u, newSpace->lifeCycleCosts().size());
  ASSERT_EQ(2u, newSpace->lights().size());
  ASSERT_EQ(1u, newSpace->lights()[0].definition().lifeCycleCosts().size());
  EXPECT_EQ("CostPerEach", newSpace->lights()[0].definition().lifeCycleCosts()[0].costUnits());
  EXPECT_EQ(100, newSpace->lights()[0].definition().lifeCycleCosts()[0].totalCost());
  ASSERT_EQ(1u, newSpace->lights()[1].definition().lifeCycleCosts().size());
  EXPECT_EQ("CostPerEach", newSpace->lights()[1].definition().lifeCycleCosts()[0].costUnits());
  EXPECT_EQ(100, newSpace->lights()[1].definition().lifeCycleCosts()[0].totalCost());
}

TEST_F(ModelFixture, ThermalZone_CombineSpaces_Cost3)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  ElectricEquipmentDefinition definition1(model);
  ElectricEquipment equip1(definition1);
  equip1.setSpace(*space1);

  ElectricEquipmentDefinition definition2(model);
  ElectricEquipment equip2(definition2);
  equip2.setSpace(*space2);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Cost", definition1, 1, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Cost", definition2, 1, "CostPerArea", "Construction");
  ASSERT_TRUE(cost2);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  space2->setThermalZone(thermalZone);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());

  boost::optional<Space> newSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(newSpace);
  ASSERT_EQ(2u, model.getModelObjects<LifeCycleCost>().size());
  EXPECT_EQ(0u, newSpace->lifeCycleCosts().size());
  ASSERT_EQ(2u, newSpace->electricEquipment().size());
  ASSERT_EQ(1u, newSpace->electricEquipment()[0].definition().lifeCycleCosts().size());
  EXPECT_EQ("CostPerEach", newSpace->electricEquipment()[0].definition().lifeCycleCosts()[0].costUnits());
  EXPECT_EQ(100, newSpace->electricEquipment()[0].definition().lifeCycleCosts()[0].totalCost());
  ASSERT_EQ(1u, newSpace->electricEquipment()[1].definition().lifeCycleCosts().size());
  EXPECT_EQ("CostPerEach", newSpace->electricEquipment()[1].definition().lifeCycleCosts()[0].costUnits());
  EXPECT_EQ(100, newSpace->electricEquipment()[1].definition().lifeCycleCosts()[0].totalCost());
}

TEST_F(ModelFixture, ThermalZone_Clone)
{
  Model m;
  ThermalZone thermalZone(m);

  auto zoneAirNode = thermalZone.zoneAirNode();

  ZoneControlHumidistat humidistat(m);
  thermalZone.setZoneControlHumidistat(humidistat);
  ScheduleRuleset humidSchedule(m);
  humidistat.setHumidifyingRelativeHumiditySetpointSchedule(humidSchedule);
  ScheduleRuleset dehumidSchedule(m);
  humidistat.setDehumidifyingRelativeHumiditySetpointSchedule(dehumidSchedule);

  ThermostatSetpointDualSetpoint thermostat(m);
  thermalZone.setThermostat(thermostat);
  ScheduleRuleset coolingSchedule(m);
  thermostat.setCoolingSchedule(coolingSchedule);
  ScheduleRuleset heatingSchedule(m);
  thermostat.setHeatingSchedule(heatingSchedule);

  auto thermalZoneClone = thermalZone.clone(m).cast<ThermalZone>();

  auto humidistatClone = thermalZoneClone.zoneControlHumidistat();
  ASSERT_TRUE(humidistatClone);
  ASSERT_NE(humidistatClone.get(),humidistat);
  auto humidSchedule2 = humidistatClone->humidifyingRelativeHumiditySetpointSchedule();
  ASSERT_TRUE(humidSchedule2);
  ASSERT_EQ(humidSchedule,humidSchedule2.get());
  auto dehumidSchedule2 = humidistatClone->dehumidifyingRelativeHumiditySetpointSchedule();
  ASSERT_TRUE(dehumidSchedule2);
  ASSERT_EQ(dehumidSchedule,dehumidSchedule2.get());

  auto thermostatClone = thermalZoneClone.thermostat();
  ASSERT_TRUE(thermostatClone);
  ASSERT_NE(thermostatClone.get(),thermostat);
  auto coolingSchedule2 = thermostatClone->cast<ThermostatSetpointDualSetpoint>().coolingSetpointTemperatureSchedule();
  ASSERT_TRUE(coolingSchedule2);
  ASSERT_EQ(coolingSchedule,coolingSchedule2.get());
  auto heatingSchedule2 = thermostatClone->cast<ThermostatSetpointDualSetpoint>().heatingSetpointTemperatureSchedule();
  ASSERT_TRUE(heatingSchedule2);
  ASSERT_EQ(heatingSchedule,heatingSchedule2.get());

  EXPECT_FALSE(thermalZoneClone.zoneAirNode().handle().isNull());
  EXPECT_EQ(zoneAirNode,thermalZone.zoneAirNode());
  EXPECT_FALSE(thermalZone.zoneAirNode().handle().isNull());
}

TEST_F(ModelFixture, ThermalZone_SonOfClone)
{
  Model model;
  ScheduleConstant schedule(model);
  schedule.setValue(1.0); // Always on
  FanConstantVolume fan(model, schedule);
  CoilHeatingWater heatingCoil(model, schedule);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model, schedule, fan, heatingCoil);
  ASSERT_FALSE(zoneHVACUnitHeater.thermalZone());

  ThermalZone thermalZone(model);
  auto success = zoneHVACUnitHeater.addToThermalZone(thermalZone);
  ASSERT_TRUE(success);
  ASSERT_TRUE(zoneHVACUnitHeater.thermalZone());

  boost::optional<model::ModelObject> clone1 = zoneHVACUnitHeater.clone(model);
  ASSERT_FALSE(clone1->cast<model::ZoneHVACComponent>().thermalZone());
  ASSERT_TRUE(zoneHVACUnitHeater.thermalZone());
  success = clone1->cast<model::ZoneHVACComponent>().addToThermalZone(thermalZone);
  ASSERT_TRUE(success);
  ASSERT_TRUE(zoneHVACUnitHeater.thermalZone());
  ASSERT_TRUE(clone1->cast<model::ZoneHVACComponent>().thermalZone());

  boost::optional<model::ModelObject> clone2 = zoneHVACUnitHeater.clone(model);
  ASSERT_FALSE(clone2->cast<model::ZoneHVACComponent>().thermalZone());
  ASSERT_TRUE(clone1->cast<model::ZoneHVACComponent>().thermalZone());
  ASSERT_TRUE(zoneHVACUnitHeater.thermalZone());
  success = clone2->cast<model::ZoneHVACComponent>().addToThermalZone(thermalZone);
  ASSERT_TRUE(success);
  ASSERT_TRUE(zoneHVACUnitHeater.thermalZone());
  ASSERT_TRUE(clone1->cast<model::ZoneHVACComponent>().thermalZone());
  ASSERT_TRUE(clone2->cast<model::ZoneHVACComponent>().thermalZone());
}

TEST_F(ModelFixture, ThermalZone_Ports)
{
  Model m;
  ThermalZone zone(m);

  auto inletPortList = zone.inletPortList();
  auto exhaustPortList = zone.exhaustPortList();

  auto inletPortListZone = inletPortList.thermalZone();
  auto exhaustPortListZone = exhaustPortList.thermalZone();

  EXPECT_EQ(zone.handle(),inletPortListZone.handle());
  EXPECT_EQ(zone.handle(),exhaustPortListZone.handle());

  zone.remove();
  EXPECT_TRUE(inletPortList.handle().isNull());
  EXPECT_TRUE(exhaustPortList.handle().isNull());
}

TEST_F(ModelFixture, ThermalZone_Thermostat)
{
  {
    Model m;

    ThermalZone zone1(m);
    ThermostatSetpointDualSetpoint thermostat1(m);

    EXPECT_TRUE(zone1.setThermostat(thermostat1));
    EXPECT_TRUE(zone1.setThermostat(thermostat1));

    ASSERT_TRUE(thermostat1.thermalZone());
    EXPECT_EQ(zone1,thermostat1.thermalZone().get());

    auto returnvalue = zone1.thermostat();
    ASSERT_TRUE(returnvalue);
    EXPECT_EQ(thermostat1,returnvalue.get());

    ThermalZone zone2(m);

    EXPECT_TRUE(zone2.setThermostat(thermostat1));
    returnvalue = zone2.thermostat();
    ASSERT_TRUE(returnvalue);
    EXPECT_NE(returnvalue.get(),thermostat1);
    auto thermostats = m.getModelObjects<Thermostat>();
    EXPECT_EQ(2u,thermostats.size());

    ThermostatSetpointDualSetpoint themostat2(m);
    EXPECT_TRUE(zone2.setThermostat(themostat2));
    EXPECT_TRUE(returnvalue->handle().isNull());
    ASSERT_TRUE(zone2.thermostat());
    EXPECT_EQ(themostat2.handle(), zone2.thermostat()->handle());
    EXPECT_EQ(2u,m.getModelObjects<Thermostat>().size());
  }

  {
    Model m;
    ThermalZone zone(m);
    ThermostatSetpointDualSetpoint thermostat(m);
    zone.setThermostat(thermostat);
    auto zone2 = zone.clone().cast<model::ThermalZone>();

    ASSERT_TRUE(zone.thermostat());
    ASSERT_TRUE(zone2.thermostat());
    EXPECT_NE(zone.thermostat()->handle(), zone2.thermostat()->handle());
    EXPECT_EQ(2u,m.getModelObjects<model::Thermostat>().size());
  }
}

TEST_F(ModelFixture, ThermalZone_ZoneControlContaminantController)
{
  {
    Model m;

    ThermalZone zone1(m);
    ZoneControlContaminantController controller1(m);

    EXPECT_TRUE(zone1.setZoneControlContaminantController(controller1));
    EXPECT_TRUE(zone1.setZoneControlContaminantController(controller1));

    ASSERT_TRUE(controller1.controlledZone());
    EXPECT_EQ(zone1,controller1.controlledZone().get());

    auto returnvalue = zone1.zoneControlContaminantController();
    ASSERT_TRUE(returnvalue);
    EXPECT_EQ(controller1,returnvalue.get());

    ThermalZone zone2(m);

    EXPECT_TRUE(zone2.setZoneControlContaminantController(controller1));
    returnvalue = zone2.zoneControlContaminantController();
    ASSERT_TRUE(returnvalue);
    EXPECT_NE(returnvalue.get(),controller1);
    auto controllers = m.getModelObjects<ZoneControlContaminantController>();
    EXPECT_EQ(2u,controllers.size());

    ZoneControlContaminantController controller2(m);
    EXPECT_TRUE(zone2.setZoneControlContaminantController(controller2));
    EXPECT_TRUE(returnvalue->handle().isNull());
    ASSERT_TRUE(zone2.zoneControlContaminantController());
    EXPECT_EQ(controller2.handle(), zone2.zoneControlContaminantController()->handle());
    EXPECT_EQ(2u,m.getModelObjects<ZoneControlContaminantController>().size());
  }

  {
    Model m;
    ThermalZone zone(m);
    ZoneControlContaminantController controller(m);
    zone.setZoneControlContaminantController(controller);
    auto zone2 = zone.clone().cast<model::ThermalZone>();

    ASSERT_TRUE(zone.zoneControlContaminantController());
    ASSERT_TRUE(zone2.zoneControlContaminantController());
    EXPECT_NE(zone.zoneControlContaminantController()->handle(), zone2.zoneControlContaminantController()->handle());
    EXPECT_EQ(2u,m.getModelObjects<model::ZoneControlContaminantController>().size());
  }
}

TEST_F(ModelFixture, ThermalZone_ZoneControlHumidistat)
{
  {
    Model m;

    ThermalZone zone1(m);
    ZoneControlHumidistat controller1(m);

    EXPECT_TRUE(zone1.setZoneControlHumidistat(controller1));
    EXPECT_TRUE(zone1.setZoneControlHumidistat(controller1));

    ASSERT_TRUE(controller1.controlledZone());
    EXPECT_EQ(zone1,controller1.controlledZone().get());

    auto returnvalue = zone1.zoneControlHumidistat();
    ASSERT_TRUE(returnvalue);
    EXPECT_EQ(controller1,returnvalue.get());

    ThermalZone zone2(m);

    EXPECT_TRUE(zone2.setZoneControlHumidistat(controller1));
    returnvalue = zone2.zoneControlHumidistat();
    ASSERT_TRUE(returnvalue);
    EXPECT_NE(returnvalue.get(),controller1);
    auto controllers = m.getModelObjects<ZoneControlHumidistat>();
    EXPECT_EQ(2u,controllers.size());

    ZoneControlHumidistat controller2(m);
    EXPECT_TRUE(zone2.setZoneControlHumidistat(controller2));
    EXPECT_TRUE(returnvalue->handle().isNull());
    ASSERT_TRUE(zone2.zoneControlHumidistat());
    EXPECT_EQ(controller2.handle(), zone2.zoneControlHumidistat()->handle());
    EXPECT_EQ(2u,m.getModelObjects<ZoneControlHumidistat>().size());
  }

  {
    Model m;
    ThermalZone zone(m);
    ZoneControlHumidistat controller(m);
    zone.setZoneControlHumidistat(controller);
    auto zone2 = zone.clone().cast<model::ThermalZone>();

    ASSERT_TRUE(zone.zoneControlHumidistat());
    ASSERT_TRUE(zone2.zoneControlHumidistat());
    EXPECT_NE(zone.zoneControlHumidistat()->handle(), zone2.zoneControlHumidistat()->handle());
    EXPECT_EQ(2u,m.getModelObjects<model::ZoneControlHumidistat>().size());
  }
}

