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
#include "../Model_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../SizingZone.hpp"
#include "../SizingZone_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../StraightComponent.hpp"
#include "../Space.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirTerminalSingleDuctUncontrolled.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../LifeCycleCost.hpp"
#include "../LifeCycleCost_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipment.hpp"
#include "../LightsDefinition.hpp"
#include "../Lights.hpp"
#include "../ZoneControlHumidistat.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"
#include "../../utilities/core/Containers.hpp"

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

TEST_F(ModelFixture, ThermalZone_ZoneControlHumidistat)
{
  Model m;
  ThermalZone thermalZone(m);
  ZoneControlHumidistat humidistat(m);

  EXPECT_FALSE(thermalZone.zoneControlHumidistat());
  EXPECT_TRUE(thermalZone.setZoneControlHumidistat(humidistat));
  ASSERT_TRUE(thermalZone.zoneControlHumidistat());
  EXPECT_EQ(humidistat, thermalZone.zoneControlHumidistat().get());
  thermalZone.resetZoneControlHumidistat();
  EXPECT_FALSE(thermalZone.zoneControlHumidistat());
}
