/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <numeric>  // std::accumulate
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
  forwardTranslator.setExcludeSpaceTranslation(true);

  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());

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

  {
    // Translate to EnergyPlus Space
    ForwardTranslator forwardTranslator;
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

    const auto electricEquipmentITEAirCooledObject1 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    const auto zoneOrSpaceTarget1 = electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
    ASSERT_TRUE(zoneOrSpaceTarget1);

    const auto electricEquipmentITEAirCooledObject2 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[1];
    const auto zoneOrSpaceTarget2 = electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
    ASSERT_TRUE(zoneOrSpaceTarget2);

    ASSERT_NE(zoneOrSpaceTarget1.get(), zoneOrSpaceTarget2.get());
  }

  {
    // Historical method prior to EnergyPlus Space
    ForwardTranslator forwardTranslator;
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

    const auto electricEquipmentITEAirCooledObject1 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    const auto zoneOrSpaceTarget1 = electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
    ASSERT_TRUE(zoneOrSpaceTarget1);

    const auto electricEquipmentITEAirCooledObject2 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[1];
    const auto zoneOrSpaceTarget2 = electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
    ASSERT_TRUE(zoneOrSpaceTarget2);

    ASSERT_NE(zoneOrSpaceTarget1.get(), zoneOrSpaceTarget2.get());
  }
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
  std::vector<Point3d> points{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

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

  {
    // Translate to EnergyPlus Space
    ForwardTranslator forwardTranslator;
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

    WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
    EXPECT_EQ("Watts/Area",
              electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false));
    EXPECT_EQ(20.0, electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false).get());

    EXPECT_TRUE(electricEquipmentITEAirCooledObject.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperUnit));

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
    EXPECT_NE(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());
    EXPECT_EQ(openstudio::IddObjectType("Space"),
              electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->iddObject().type());
  }

  {
    // Historical method prior to EnergyPlus Space
    ForwardTranslator forwardTranslator;
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

    WorkspaceObject electricEquipmentITEAirCooledObject = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
    EXPECT_EQ("Watts/Area",
              electricEquipmentITEAirCooledObject.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false));
    EXPECT_EQ(20.0, electricEquipmentITEAirCooledObject.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false).get());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
    EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_TwoSpaces_OneThermalZone) {
  Model model;

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  // 20 W/m^2
  ElectricEquipmentITEAirCooledDefinition definition(model);
  definition.setWattsperZoneFloorArea(20.0);

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled1(definition);
  electricEquipmentITEAirCooled1.setSpace(*space1);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(20.0, space1->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(2000.0, space1->electricEquipmentITEAirCooledPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  ElectricEquipmentITEAirCooled electricEquipmentITEAirCooled2(definition);
  electricEquipmentITEAirCooled2.setSpace(*space2);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(20.0, space2->electricEquipmentITEAirCooledPowerPerFloorArea());
  EXPECT_EQ(2000.0, space2->electricEquipmentITEAirCooledPower());

  {
    // Historical method prior to EnergyPlus Space
    ForwardTranslator forwardTranslator;
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

    WorkspaceObject electricEquipmentITEAirCooledObject1 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    WorkspaceObject electricEquipmentITEAirCooledObject2 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

    ASSERT_TRUE(electricEquipmentITEAirCooledObject1.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
    EXPECT_EQ("Watts/Unit",
              electricEquipmentITEAirCooledObject1.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject1.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false));
    EXPECT_EQ(2000.0, electricEquipmentITEAirCooledObject1.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false).get());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
    EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());

    ASSERT_TRUE(electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
    EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());
  }
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
  forwardTranslator.setExcludeSpaceTranslation(true);

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

  ASSERT_TRUE(electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName));
  ASSERT_EQ(nodeName, electricEquipmentITEAirCooledIdfObject.getString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName).get());
  EXPECT_EQ(zoneObject.handle(), electricEquipmentITEAirCooledObject.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName)->handle());

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
  forwardTranslator.setExcludeSpaceTranslation(true);
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject electricEquipmentITEAirCooledObject1 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[0];
  WorkspaceObject electricEquipmentITEAirCooledObject2 = workspace.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled)[1];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  boost::optional<WorkspaceObject> target1_ = electricEquipmentITEAirCooledObject1.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
  ASSERT_TRUE(target1_);
  EXPECT_EQ(zoneObject, target1_.get()) << "Expected zoneObject '" << zoneObject.nameString() << "', but got '" << target1_->nameString() << "'";

  boost::optional<WorkspaceObject> target2_ = electricEquipmentITEAirCooledObject2.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
  ASSERT_TRUE(target2_);
  EXPECT_EQ(zoneObject, target2_.get()) << "Expected zoneObject '" << zoneObject.nameString() << "', but got '" << target2_->nameString() << "'";

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

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipmentITEAirCooled_SpaceTypes) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  constexpr double spaceFloorArea = width * width;
  // constexpr double spaceVolume = spaceFloorArea * height;
  // constexpr double oneWallArea = width * height;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┼────────┤
  //   │        │        │        │        │
  //   │        │        │        │        │
  //   │ Space 1│ Space 2│ Space 3│ Space 4│
  //   │        │        │        │        │
  //   └────────┴────────┴────────┴────────┴──► x
  //  0        10       20        30       40

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space3.setXOrigin(width * 2);
  auto space4 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space4.setXOrigin(width * 3);

  ThermalZone z(m);
  EXPECT_TRUE(space1.setThermalZone(z));
  EXPECT_TRUE(space2.setThermalZone(z));
  EXPECT_TRUE(space3.setThermalZone(z));
  EXPECT_TRUE(space4.setThermalZone(z));

  AirLoopHVAC a(m);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  // connect the thermal zone to airloopHVAC
  EXPECT_TRUE(a.addBranchForZone(z, atu));
  ASSERT_TRUE(atu.outletModelObject());
  std::string nodeName = "ATU to Zone Node";
  atu.outletModelObject()->setName(nodeName);

  // Assign a default space type at building-level, with its own infiltration object
  SpaceType buildingSpaceType(m);
  buildingSpaceType.setName("buildingSpaceType");

  ElectricEquipmentITEAirCooledDefinition iteBuildingDefinition(m);
  ElectricEquipmentITEAirCooled iteBuilding(iteBuildingDefinition);
  iteBuilding.setName("iteBuilding");
  EXPECT_TRUE(iteBuilding.setSpaceType(buildingSpaceType));

  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));

  // Create an Office Space Type. Assign to Space 1 & 2 only (not 3 nor 4), it also has an infiltration object
  SpaceType officeSpaceType(m);
  officeSpaceType.setName("officeSpaceType");
  ElectricEquipmentITEAirCooledDefinition iteOfficeDefinition(m);
  ElectricEquipmentITEAirCooled iteOffice(iteOfficeDefinition);
  iteOffice.setName("iteOffice");
  EXPECT_TRUE(iteOffice.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space1.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space2.setSpaceType(officeSpaceType));

  // Space 1 and 3 have a space-level infiltration
  ElectricEquipmentITEAirCooledDefinition iteSpace1Definition(m);
  ElectricEquipmentITEAirCooled iteSpace1(iteSpace1Definition);
  iteSpace1.setName("iteSpace1");
  EXPECT_TRUE(iteSpace1.setSpace(space1));

  ElectricEquipmentITEAirCooledDefinition iteSpace3Definition(m);
  ElectricEquipmentITEAirCooled iteSpace3(iteSpace3Definition);
  iteSpace3.setName("iteSpace3");
  EXPECT_TRUE(iteSpace3.setSpace(space3));

  // ITEAirCooled Characteristics
  // =============================
  //
  // |  ITEAirCooled | Watts/Area | Absolute W           | Calculated  Total    |
  // |     Name      |   (W/s)    |   (W/Units, 1 Unit)  | For One Space (W)    |
  // |---------------|------------|----------------------|----------------------|
  // | iteSpace1     | 1.0        |                      | 100.0                |
  // | iteSpace3     |            | 150.0                | 150.0                |
  // | iteOffice     | 1.2        |                      | 120.0                |
  // | iteBuilding   |            | 130.0                | 130.0                |

  // Which ITEAirCooled object(s) apply to each space
  // ================================================
  //
  // | Space    | Space Specific ITEAirCooled | Space specific W    ║ SpaceType                     | Additional ITEAirCooled | W     |
  // |----------|-----------------------------|---------------------║-------------------------------|-------------------------|-------|
  // | Space 1  | iteSpace1                   | 100.0               ║ officeSpaceType               | iteOffice               | 120.0 |
  // | Space 2  |                             |                     ║ officeSpaceType               | iteOffice               | 120.0 |
  // | Space 3  | iteSpace3                   | 150.0               ║ Inherited: buildingSpaceType  | iteBuilding             | 130.0 |
  // | Space 4  |                             |                     ║ Inherited: buildingSpaceType  | iteBuilding             | 130.0 |
  // |--------------------------------------------------------------------------------------------------------------------------------|
  // | Subtotal |                               250.0               ║                                                           500.0 |
  // |================================================================================================================================|
  // | Total    |                                                 750.0                                                               |
  // |================================================================================================================================|

  EXPECT_TRUE(iteSpace1Definition.setWattsperZoneFloorArea(1.0));
  EXPECT_TRUE(iteSpace3Definition.setWattsperUnit(150.0));
  EXPECT_TRUE(iteOfficeDefinition.setWattsperZoneFloorArea(1.2));
  EXPECT_TRUE(iteBuildingDefinition.setWattsperUnit(130.0));

  auto spaces = z.spaces();
  double modelWatts = std::accumulate(spaces.cbegin(), spaces.cend(), 0.0,
                                      [](double sum, const Space& space) { return sum + space.electricEquipmentITEAirCooledPower(); });
  EXPECT_EQ(750.0, modelWatts);

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto ites = w.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled);
    // I expect iteSpace1, iteSpace3, two iteOffice and two iteBuilding, so 6 total
    ASSERT_EQ(6, ites.size());

    double totalWatts = 0.0;  // m3/s
    for (const auto& ite : ites) {
      auto name = ite.nameString();
      auto z_ = ite.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
      ASSERT_TRUE(z_);
      EXPECT_EQ(zone, z_.get());
      // Everything is converted to Absolute Watts/Units
      ASSERT_TRUE(ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));
      EXPECT_EQ("Watts/Unit", ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get());

      EXPECT_TRUE(ite.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea));

      EXPECT_EQ(nodeName, ite.getString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName).get());

      ASSERT_TRUE(ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false));
      double w = ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit).get();
      totalWatts += w;

      // These two are absolute, no issue whatsoever
      if (name.find(iteBuilding.nameString()) != std::string::npos) {
        EXPECT_EQ(130.0, w);
        EXPECT_EQ(iteBuildingDefinition.wattsperUnit().get(), w);
      } else if (name.find(iteSpace3.nameString()) != std::string::npos) {
        EXPECT_EQ(150.0, w);
        EXPECT_EQ(iteSpace3Definition.wattsperUnit().get(), w);
        // These two are per floor area
      } else if (name.find(iteOffice.nameString()) != std::string::npos) {
        EXPECT_EQ(120.0, w);
        EXPECT_EQ(iteOfficeDefinition.wattsperZoneFloorArea().get() * spaceFloorArea, w);
        EXPECT_EQ(iteOffice.getWattsperUnit(spaceFloorArea), w);
      } else if (name.find(iteSpace1.nameString()) != std::string::npos) {
        EXPECT_EQ(100.0, w);
        EXPECT_EQ(iteSpace1Definition.wattsperZoneFloorArea().get() * spaceFloorArea, w);
        EXPECT_EQ(iteSpace1.getWattsperUnit(spaceFloorArea), w);
      }
    }

    EXPECT_EQ(750.0, totalWatts);
    EXPECT_EQ(modelWatts, totalWatts);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    // This object actually accepts a Zone or Space as a target... But not ZoneList/SpaceList
    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto idf_space1_ = w.getObjectByTypeAndName(IddObjectType::Space, space1.nameString());
    ASSERT_TRUE(idf_space1_);
    auto idf_space2_ = w.getObjectByTypeAndName(IddObjectType::Space, space2.nameString());
    ASSERT_TRUE(idf_space2_);
    auto idf_space3_ = w.getObjectByTypeAndName(IddObjectType::Space, space3.nameString());
    ASSERT_TRUE(idf_space3_);
    auto idf_space4_ = w.getObjectByTypeAndName(IddObjectType::Space, space4.nameString());
    ASSERT_TRUE(idf_space4_);

    auto ites = w.getObjectsByType(IddObjectType::ElectricEquipment_ITE_AirCooled);
    // I expect iteSpace1, iteSpace3, two iteOffice and two iteBuilding, so 6 total
    ASSERT_EQ(6, ites.size());

    bool foundSpace1 = false;
    bool foundSpace3 = false;

    bool foundSpace1OfficeSpaceType = false;
    bool foundSpace2OfficeSpaceType = false;
    bool foundSpace3BuildingSpaceType = false;
    bool foundSpace4BuildingSpaceType = false;

    double totalWatts = 0.0;
    for (const auto& ite : ites) {
      auto name = ite.nameString();
      auto spaceTarget_ = ite.getTarget(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName);
      ASSERT_TRUE(spaceTarget_);

      ASSERT_TRUE(ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false));

      EXPECT_EQ(nodeName, ite.getString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName).get());

      // These two are absolute, no issue whatsoever
      if (name.find(iteBuilding.nameString()) != std::string::npos) {
        EXPECT_EQ("Watts/Unit", ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get())
          << "Failed for " << name << " pointing to " << spaceTarget_->briefDescription();

        ASSERT_TRUE(ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false));
        double w = ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit).get();

        EXPECT_EQ(130.0, w);
        EXPECT_EQ(iteBuildingDefinition.wattsperUnit().get(), w);

        EXPECT_TRUE(ite.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea));

        totalWatts += w;

        if (spaceTarget_.get() == idf_space3_.get()) {
          foundSpace3BuildingSpaceType = true;
        } else if (spaceTarget_.get() == idf_space4_.get()) {
          foundSpace4BuildingSpaceType = true;
        } else {
          EXPECT_TRUE(false) << iteBuilding.nameString() << " maps to an unexpected entity: " << spaceTarget_->briefDescription();
        }
      } else if (name.find(iteSpace3.nameString()) != std::string::npos) {

        EXPECT_EQ("Watts/Unit", ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get())
          << "Failed for " << name << " pointing to " << spaceTarget_->briefDescription();

        ASSERT_TRUE(ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, false));
        double w = ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit).get();

        EXPECT_EQ(150.0, w);
        EXPECT_EQ(iteSpace3Definition.wattsperUnit().get(), w);

        EXPECT_TRUE(ite.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea));

        totalWatts += w;

        if (spaceTarget_.get() == idf_space3_.get()) {
          foundSpace3 = true;
        } else {
          EXPECT_TRUE(false) << iteBuilding.nameString() << " maps to an unexpected entity: " << spaceTarget_->briefDescription();
        }

        // These two are per floor area
      } else if (name.find(iteOffice.nameString()) != std::string::npos) {

        EXPECT_EQ("Watts/Area", ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get())
          << "Failed for " << name << " pointing to " << spaceTarget_->briefDescription();

        EXPECT_TRUE(ite.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperUnit));

        ASSERT_TRUE(ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false));
        double w_perArea = ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea).get();

        EXPECT_EQ(iteOffice.wattsperZoneFloorArea().get(), w_perArea);

        double w = w_perArea * spaceFloorArea;

        totalWatts += w;

        EXPECT_EQ(120.0, w);

        if (spaceTarget_.get() == idf_space1_.get()) {
          foundSpace1OfficeSpaceType = true;
        } else if (spaceTarget_.get() == idf_space2_.get()) {
          foundSpace2OfficeSpaceType = true;
        } else {
          EXPECT_TRUE(false) << iteBuilding.nameString() << " maps to an unexpected entity: " << spaceTarget_->briefDescription();
        }
      } else if (name.find(iteSpace1.nameString()) != std::string::npos) {

        EXPECT_EQ("Watts/Area", ite.getString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, false).get())
          << "Failed for " << name << " pointing to " << spaceTarget_->briefDescription();

        EXPECT_TRUE(ite.isEmpty(ElectricEquipment_ITE_AirCooledFields::WattsperUnit));

        ASSERT_TRUE(ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, false));
        double w_perArea = ite.getDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea).get();

        EXPECT_EQ(iteSpace1.wattsperZoneFloorArea().get(), w_perArea);

        double w = w_perArea * spaceFloorArea;

        totalWatts += w;

        EXPECT_EQ(100.0, w);

        if (spaceTarget_.get() == idf_space1_.get()) {
          foundSpace1 = true;
        } else {
          EXPECT_TRUE(false) << iteBuilding.nameString() << " maps to an unexpected entity: " << spaceTarget_->briefDescription();
        }
      }
    }

    EXPECT_TRUE(foundSpace1);
    EXPECT_TRUE(foundSpace3);

    EXPECT_TRUE(foundSpace1OfficeSpaceType);
    EXPECT_TRUE(foundSpace2OfficeSpaceType);
    EXPECT_TRUE(foundSpace3BuildingSpaceType);
    EXPECT_TRUE(foundSpace4BuildingSpaceType);
    EXPECT_EQ(750.0, totalWatts);
    EXPECT_EQ(modelWatts, totalWatts);
  }
}
