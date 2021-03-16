/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ElectricEquipmentITEAirCooled.hpp"
#include "../../model/ElectricEquipmentITEAirCooled_Impl.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/DefaultScheduleSet.hpp"
#include "../../model/DefaultScheduleSet_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/StraightComponent.hpp"

#include "../../model/LifeCycleCost.hpp"
#include "../../model/LifeCycleCost_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/ElectricEquipment_ITE_AirCooled_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/ComponentCost_LineItem_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_RecurringCosts_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

// OS only has definition
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooledDefinition) {
  Model model;
  ElectricEquipmentITEAirCooledDefinition definition(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  std::string errors;
  for (const LogMessage& error : forwardTranslator.errors()) {
    errors += error.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.errors().size()) << errors;

  std::string warnings;
  for (const LogMessage& warning : forwardTranslator.warnings()) {
    warnings += warning.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.warnings().size()) << warnings;

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());

  //model.save(toPath("./ITE_translator_definition.osm"), true);
  //workspace.save(toPath("./ITE_translator_definition.idf"), true);
}

// OS has IT object and definition, but no space associated => supposed to not be translated
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_NoSpace) {
  Model model;
  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());

  //model.save(toPath("./ITE_translator_NoSpace.osm"), true);
  //workspace.save(toPath("./ITE_translator_NoSpace.idf"), true);
}

// OS has IT object and definition, and a valid space associated => Expected: to be translated
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_Space) {
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  electricEquipmentITEAirCooled.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());

  //model.save(toPath("./ITE_translator_Space.osm"), true);
  //workspace.save(toPath("./ITE_translator_Space.idf"), true);
}

// OS has IT object and definition, and a valid spaceType associated => Expected: to be translated, spacetype in OS but space in E+
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_SpaceType) {
  Model model;

  ThermalZone thermalZone1(model);
  ThermalZone thermalZone2(model);
  Space space1(model);
  Space space2(model);
  SpaceType spaceType(model);
  space1.setSpaceType(spaceType);
  space2.setSpaceType(spaceType);
  space1.setThermalZone(thermalZone1);
  space2.setThermalZone(thermalZone2);

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  electricEquipmentITEAirCooled.setSpaceType(spaceType);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  //EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());

  //model.save(toPath("./ITE_translator_SpaceType.osm"), true);
  //workspace.save(toPath("./ITE_translator_SpaceType.idf"), true);
}

//two spaces, one spaceType, one thermalZone => Expected: IT object associated with the spaceType in OS,
//but associated with the thermal zone in E+ after two zones being combined.
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_OneSpaceType_OneThermalZone) {
  Model model;

  // 20 W/m^2
  SpaceType spaceType(model);
  ElectricEquipmentITEAirCooledDefinition definition(model);
  definition.setWattsperZoneFloorArea(20.0);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  electricEquipmentITEAirCooled.setSpaceType(spaceType);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points;
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(20.0, space1->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(2000.0, space1->electricEquipmentITEAirCooledPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(20.0, space2->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(2000.0, space2->electricEquipmentITEAirCooledPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
  //WorkspaceObject zoneListObject = workspace.getObjectsByType(IddObjectType::ZoneList)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
  EXPECT_EQ("Watts/Area",
            electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get());

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false));
  EXPECT_EQ(20.0, electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false).get());

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());

  //model.save(toPath("./ITE_translator_OneSpaceType_OneThermalZone.osm"), true);
  //workspace.save(toPath("./ITE_translator_OneSpaceType_OneThermalZone.idf"), true);
}

// two spaces, two spaceTypes, one thermalZone => Expected: IT object associated with each spaceType in OS,
// but associated with the thermal zone in E+ after two zones being combined.
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_TwoSpaceTypes_OneThermalZone) {
  Model model;

  // 20 W/m^2
  SpaceType spaceType1(model);
  ElectricEquipmentITEAirCooledDefinition definition1(model);
  definition1.setWattsperZoneFloorArea(20.0);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled1(definition1);
  electricEquipmentITEAirCooled1.setSpaceType(spaceType1);

  // 30 W/m^2
  SpaceType spaceType2(model);
  ElectricEquipmentITEAirCooledDefinition definition2(model);
  definition2.setWattsperZoneFloorArea(30.0);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled2(definition2);
  electricEquipmentITEAirCooled2.setSpaceType(spaceType2);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points;
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(20.0, space1->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(2000.0, space1->electricEquipmentITEAirCooledPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(30.0, space2->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(3000.0, space2->electricEquipmentITEAirCooledPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  bool foundITEquipmentPower2000 = false;
  bool foundITEquipmentPower3000 = false;
  for (int i = 0; i < 2; ++i) {
    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[i].getString(
      ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
    EXPECT_EQ("Watts/Unit", workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[i]
                              .getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false)
                              .get());

    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[i].getDouble(
      ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false));
    double wattsperUnit = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[i]
                            .getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false)
                            .get();

    if (wattsperUnit == 2000.0) {
      foundITEquipmentPower2000 = true;
    } else if (wattsperUnit == 3000.0) {
      foundITEquipmentPower3000 = true;
    }
  }

  EXPECT_TRUE(foundITEquipmentPower2000);
  EXPECT_TRUE(foundITEquipmentPower3000);

  //model.save(toPath("./ITE_translator_TwoSpaceTypes_OneThermalZone.osm"), true);
  //workspace.save(toPath("./ITE_translator_TwoSpaceTypes_OneThermalZone.idf"), true);
}

// test the connection of supply air node under default air flow calculation method "FlowFromSystem", no terminal => expect fail
// two spaces, one spaceType, two thermalZone => Expected: IT object associated with the spaceType in OS,
// but associated with each thermal zone in E+
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_SupplyAirNodeConnection) {
  Model model;

  AirLoopHVAC airLoopHVAC(model);
  ThermalZone thermalZone(model);
  ScheduleCompact schedule(model);
  AirTerminalSingleDuctConstantVolumeNoReheat singleDuctTerminal(model, schedule);
  Space space(model);
  space.setThermalZone(thermalZone);

  // connect the thermal zone to airloopHVAC
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone, singleDuctTerminal));

  ElectricEquipmentITEAirCooledDefinition definition(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled(definition);
  electricEquipmentITEAirCooled.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  // get the thermal zone inlet node
  ASSERT_TRUE(thermalZone.inletPortList().airLoopHVACModelObject()->optionalCast<Node>());
  std::string nodeName = thermalZone.inletPortList().airLoopHVACModelObject()->optionalCast<Node>()->name().get();
  std::cout << "node name = " << nodeName << "\n";

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  IdfObject electricEquipmentITEAirCooledIdfObject = electricEquipmentITEAirCooledObject.idfObject();
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  ASSERT_EQ(nodeName, electricEquipmentITEAirCooledIdfObject.getString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName));
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());

  //model.save(toPath("./ITE_translator_SupplyAirNodeConnection.osm"), true);
  //workspace.save(toPath("./ITE_translator_SupplyAirNodeConnection.idf"), true);
}

// test constraint1: different air flow calculation methods on IT objects of a same space
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_Constraint1) {
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  AirLoopHVAC airLoopHVAC(model);
  ScheduleCompact schedule(model);
  CoilHeatingElectric reheatCoil(model);
  AirTerminalSingleDuctVAVReheat singleDuctVAVTerminal(model, schedule, reheatCoil);

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone, singleDuctVAVTerminal));

  ElectricEquipmentITEAirCooledDefinition definition1(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled1(definition1);
  electricEquipmentITEAirCooled1.setSpace(space);
  definition1.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");

  ElectricEquipmentITEAirCooledDefinition definition2(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled2(definition2);
  electricEquipmentITEAirCooled2.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject1 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject electricEquipmentITEAirCooledObject2 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[1];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  ASSERT_TRUE(electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneName)->handle());

  ASSERT_TRUE(electricEquipmentITEAirCooledObject1.getString(ElectricEquipment_ITE_AirCooledFields::AirFlowCalculationMethod, false));
  EXPECT_EQ("FlowControlWithApproachTemperatures",
            electricEquipmentITEAirCooledObject1.getString(ElectricEquipment_ITE_AirCooledFields::AirFlowCalculationMethod, false).get());
  ASSERT_TRUE(electricEquipmentITEAirCooledObject2.getString(ElectricEquipment_ITE_AirCooledFields::AirFlowCalculationMethod, false));
  EXPECT_EQ("FlowControlWithApproachTemperatures",
            electricEquipmentITEAirCooledObject2.getString(ElectricEquipment_ITE_AirCooledFields::AirFlowCalculationMethod, false).get());

  //model.save(toPath("./ITE_translator_constraint1.osm"), true);
  //workspace.save(toPath("./ITE_translator_constraint1.idf"), true);
}

// test constraint2: the connection of supply air node under air flow calculation method "FlowControlWithApproachTemperatures" - single duct VAV reheat terminal
TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_Constraint2) {
  Model model;

  AirLoopHVAC airLoopHVAC(model);
  ThermalZone thermalZone1(model);
  ThermalZone thermalZone2(model);
  ThermalZone thermalZone3(model);
  Space space1(model);
  Space space2(model);
  Space space3(model);
  space1.setThermalZone(thermalZone1);
  space2.setThermalZone(thermalZone2);
  space3.setThermalZone(thermalZone3);
  ScheduleCompact schedule(model);
  CoilHeatingElectric reheatCoil(model);
  CoilHeatingElectric heatCoil(model);
  FanConstantVolume fan(model);
  CoilCoolingDXSingleSpeed coolCoil(model);
  AirTerminalSingleDuctVAVReheat singleDuctVAVTerminal(model, schedule, reheatCoil);
  AirTerminalSingleDuctConstantVolumeNoReheat singleDuctTerminal(model, schedule);
  ZoneHVACPackagedTerminalAirConditioner ptac(model, schedule, fan, heatCoil, coolCoil);

  // connect the thermal zone to airloopHVAC
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone1, singleDuctVAVTerminal));
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2, singleDuctTerminal));
  EXPECT_TRUE(ptac.addToThermalZone(thermalZone3));

  ElectricEquipmentITEAirCooledDefinition definition1(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled1(definition1);
  electricEquipmentITEAirCooled1.setSpace(space1);
  definition1.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");

  ElectricEquipmentITEAirCooledDefinition definition2(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled2(definition2);
  electricEquipmentITEAirCooled2.setSpace(space2);
  definition2.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");

  ElectricEquipmentITEAirCooledDefinition definition3(model);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled3(definition3);
  electricEquipmentITEAirCooled3.setSpace(space3);
  definition3.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  // electricEquipmentITEAirCooled2 and 3 were not translated because the terminal is not single duct VAV terminal
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(3u, workspace.getObjectsByType(IddObjectType::Zone).size());

  // should return warning that "The FlowControlWithApproachTemperatures only applies to ITE zones with single duct VAV terminal unit."
  std::string warnings;
  for (const LogMessage& warning : forwardTranslator.warnings()) {
    warnings += warning.logMessage() + "\n";
  }
  std::cout << "Warnings: \n" << warnings;

  //model.save(toPath("./ITE_translator_constraint2.osm"), true);
  //workspace.save(toPath("./ITE_translator_constraint2.idf"), true);
}
