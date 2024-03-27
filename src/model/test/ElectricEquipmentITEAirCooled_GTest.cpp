/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"

#include "../ElectricEquipmentITEAirCooled.hpp"
#include "../ElectricEquipmentITEAirCooled_Impl.hpp"
#include "../ElectricEquipmentITEAirCooledDefinition.hpp"
#include "../ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../LifeCycleCost.hpp"
#include "../LifeCycleCost_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../StraightComponent.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

//test the constructor with default curves
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_DefaultConstructor1) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  EXPECT_EQ(5u, model.numObjects());

  EXPECT_EQ(definition.handle(), electricEquipmentITEAirCooled.definition().handle());
  EXPECT_EQ(definition.handle(), electricEquipmentITEAirCooled.electricEquipmentITEAirCooledDefinition().handle());
  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(electricEquipmentITEAirCooled.handle(), definition.instances()[0].handle());

  EXPECT_EQ("FlowFromSystem", definition.airFlowCalculationMethod());
  EXPECT_EQ("Watts/Unit", definition.designPowerInputCalculationMethod());
  EXPECT_EQ("None", definition.environmentalClass());
  EXPECT_EQ("AdjustedSupply", definition.airInletConnectionType());

  //model.save(toPath("./ITE1.osm"), true);

  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(0, definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
}

//test the constructor with input curves
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_DefaultConstructor2) {
  Model model;

  // Create the curves
  CurveBiquadratic cPUPowerInputFunctionofLoadingandAirTemperatureCurve(model);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient1Constant(-0.035289);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient2x(1.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient3xPOW2(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient4y(0.0015684);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient5yPOW2(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient6xTIMESY(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMinimumValueofx(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMaximumValueofx(1.5);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMinimumValueofy(-10);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMaximumValueofy(99.0);

  CurveBiquadratic airFlowFunctionofLoadingandAirTemperatureCurve(model);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient1Constant(-1.025);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient2x(0.9);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient3xPOW2(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient4y(0.05);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient5yPOW2(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient6xTIMESY(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMinimumValueofx(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMaximumValueofx(1.5);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMinimumValueofy(-10);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMaximumValueofy(99.0);

  CurveQuadratic fanPowerInputFunctionofFlowCurve(model);
  fanPowerInputFunctionofFlowCurve.setCoefficient1Constant(0.0);
  fanPowerInputFunctionofFlowCurve.setCoefficient2x(1.0);
  fanPowerInputFunctionofFlowCurve.setCoefficient3xPOW2(0.0);
  fanPowerInputFunctionofFlowCurve.setMinimumValueofx(0.0);
  fanPowerInputFunctionofFlowCurve.setMaximumValueofx(99.0);

  ElectricEquipmentITEAirCooledDefinition definition(model, cPUPowerInputFunctionofLoadingandAirTemperatureCurve,
                                                     airFlowFunctionofLoadingandAirTemperatureCurve, fanPowerInputFunctionofFlowCurve);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  EXPECT_EQ(5u, model.numObjects());

  EXPECT_EQ(definition.handle(), electricEquipmentITEAirCooled.definition().handle());
  EXPECT_EQ(definition.handle(), electricEquipmentITEAirCooled.electricEquipmentITEAirCooledDefinition().handle());
  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(electricEquipmentITEAirCooled.handle(), definition.instances()[0].handle());

  ASSERT_EQ("FlowFromSystem", definition.airFlowCalculationMethod());
  EXPECT_EQ("Watts/Unit", definition.designPowerInputCalculationMethod());
  EXPECT_EQ("None", definition.environmentalClass());
  EXPECT_EQ("AdjustedSupply", definition.airInletConnectionType());
  EXPECT_EQ(-0.035289, definition.cPUPowerInputFunctionofLoadingandAirTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());

  ASSERT_EQ(5.0, definition.supplyTemperatureDifference());

  //model.save(toPath("./ITE2.osm"), true);
}

//set equipment level and switch between equipment level input methods
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_equipmentLevel) {
  Model model;

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  EXPECT_DOUBLE_EQ(100, space1.get().floorArea());

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);

  ASSERT_TRUE(electricEquipmentITEAirCooled.setSpace(space1.get()));
  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(0, definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  ASSERT_TRUE(definition.setDesignLevel(50.0));
  ASSERT_EQ(50.0, definition.designLevel().get());

  //change input method to watts/area, see if watts/area is assigned.
  ASSERT_TRUE(definition.setDesignPowerInputCalculationMethod("Watts/Area", electricEquipmentITEAirCooled.floorArea()));
  EXPECT_EQ(0.5, definition.wattsperSpaceFloorArea().get());

  //model.save(toPath("./ITE3.osm"), true);
}

//Set FlowControlWithApproachTemperatures method
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_FlowControlWithApproachTemperatures) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  EXPECT_EQ(5u, model.numObjects());

  ASSERT_TRUE(definition.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures"));
  ASSERT_EQ("FlowControlWithApproachTemperatures", definition.airFlowCalculationMethod());
  ASSERT_TRUE(definition.setSupplyTemperatureDifference(10.0));
  ASSERT_EQ(10.0, definition.supplyTemperatureDifference());

  //model.save(toPath("./ITE4.osm"), true);
}

//remove ITE object
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_Remove) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);

  EXPECT_EQ(5u, model.numObjects());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooled>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooledDefinition>().size());
  std::vector<IdfObject> removed = electricEquipmentITEAirCooled.remove();
  ASSERT_EQ(1u, removed.size());
  EXPECT_EQ(IddObjectType::OS_ElectricEquipment_ITE_AirCooled, removed[0].iddObject().type().value());
  EXPECT_EQ(0u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooled>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooledDefinition>().size());
  EXPECT_EQ(4u, model.numObjects());

  //model.save(toPath("./ITE5.osm"), true);
}

//remove definition
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_RemoveDefinition) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);

  EXPECT_EQ(5u, model.numObjects());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooled>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooledDefinition>().size());
  std::vector<IdfObject> removed = definition.remove();
  ASSERT_EQ(2u, removed.size());
  EXPECT_EQ(IddObjectType::OS_ElectricEquipment_ITE_AirCooled_Definition, removed[0].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_ElectricEquipment_ITE_AirCooled, removed[1].iddObject().type().value());
  EXPECT_EQ(0u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooled>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<ElectricEquipmentITEAirCooledDefinition>().size());
  EXPECT_EQ(3u, model.numObjects());

  //model.save(toPath("./ITE6.osm"), true);
}

// test thermalzone inletportlist
TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_inletportlist) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  ThermalZone thermalZone(m);
  ThermalZone thermalZone2(m);
  ScheduleCompact s(m);
  AirTerminalSingleDuctConstantVolumeNoReheat singleDuctTerminal(m, s);

  Node inletNode = airLoopHVAC.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(thermalZone.addToNode(inletNode));
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2, singleDuctTerminal));

  //m.save(toPath("./ITE7.osm"), true);

  std::cout << thermalZone.inletPortList().airLoopHVACModelObject()->optionalCast<Node>()->name().get() << "\n";
}

// test ITE approach temperature schedule
TEST_F(ModelFixture, ElectricEquipmentITEAirCooledDefinition_approachtemperatureschedule) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  ScheduleCompact supplydeltasch(model, 5.0);
  ScheduleCompact returndeltasch(model, -5.0);

  EXPECT_EQ(7u, model.numObjects());

  EXPECT_TRUE(definition.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures"));
  EXPECT_EQ("FlowControlWithApproachTemperatures", definition.airFlowCalculationMethod());
  EXPECT_TRUE(definition.setSupplyTemperatureDifferenceSchedule(supplydeltasch));
  EXPECT_TRUE(definition.setReturnTemperatureDifferenceSchedule(returndeltasch));

  ASSERT_TRUE(definition.supplyTemperatureDifferenceSchedule());
  EXPECT_EQ(supplydeltasch, definition.supplyTemperatureDifferenceSchedule().get());
  ASSERT_TRUE(definition.returnTemperatureDifferenceSchedule());
  EXPECT_EQ(returndeltasch, definition.returnTemperatureDifferenceSchedule().get());

  // model.save(toPath("./ITE8.osm"), true);
}

TEST_F(ModelFixture, ElectricEquipmentITEAirCooled_validValues) {
  Model model;

  ElectricEquipmentITEAirCooledDefinition definition(model);

  std::vector<std::string> validAirFlowCalculationMethods(definition.validAirFlowCalculationMethodValues());
  EXPECT_NE(std::find(validAirFlowCalculationMethods.begin(), validAirFlowCalculationMethods.end(), "FlowFromSystem"),
            validAirFlowCalculationMethods.end());
  EXPECT_NE(std::find(validAirFlowCalculationMethods.begin(), validAirFlowCalculationMethods.end(), "FlowControlWithApproachTemperatures"),
            validAirFlowCalculationMethods.end());
  EXPECT_EQ(validAirFlowCalculationMethods.size(), 2);

  std::vector<std::string> validDesignPowerInputCalculationMethods(definition.validDesignPowerInputCalculationMethodValues());
  EXPECT_NE(std::find(validDesignPowerInputCalculationMethods.begin(), validDesignPowerInputCalculationMethods.end(), "Watts/Unit"),
            validDesignPowerInputCalculationMethods.end());
  EXPECT_NE(std::find(validDesignPowerInputCalculationMethods.begin(), validDesignPowerInputCalculationMethods.end(), "Watts/Area"),
            validDesignPowerInputCalculationMethods.end());
  EXPECT_EQ(validDesignPowerInputCalculationMethods.size(), 2);

  std::vector<std::string> validEnvironmentalClasses(definition.validEnvironmentalClassValues());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "None"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "A1"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "A2"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "A3"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "A4"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "B"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "C"), validEnvironmentalClasses.end());
  EXPECT_NE(std::find(validEnvironmentalClasses.begin(), validEnvironmentalClasses.end(), "H1"), validEnvironmentalClasses.end());
  EXPECT_EQ(validEnvironmentalClasses.size(), 8);

  std::vector<std::string> validAirInletConnectionTypes(definition.validAirInletConnectionTypeValues());
  EXPECT_NE(std::find(validAirInletConnectionTypes.begin(), validAirInletConnectionTypes.end(), "AdjustedSupply"),
            validAirInletConnectionTypes.end());
  EXPECT_NE(std::find(validAirInletConnectionTypes.begin(), validAirInletConnectionTypes.end(), "ZoneAirNode"), validAirInletConnectionTypes.end());
  EXPECT_EQ(validAirInletConnectionTypes.size(), 2);
}
