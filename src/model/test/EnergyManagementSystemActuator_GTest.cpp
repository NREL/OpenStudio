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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipment_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipmentDefinition_Impl.hpp"
#include "../Lights.hpp"
#include "../Lights_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../LightsDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../EnergyManagementSystemActuator.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSActuator_EMSActuator) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);
  FanConstantVolume fan2(model, s);

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  EXPECT_EQ(fanControlType, fanActuator.actuatedComponentControlType());
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());

  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  fanActuator.setActuatedComponentControlType(fanControlType);

  EXPECT_EQ(fan, fanActuator.actuatedComponent().get());
  EXPECT_EQ(fanControlType, fanActuator.actuatedComponentControlType());

  fanActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());

  fanActuator.setActuatedComponent(fan2);
  EXPECT_EQ(fan2, fanActuator.actuatedComponent().get());

  //add electric equipment actuator
  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  ComponentType = "ElectricEquipment";
  std::string equipControlType = "Electricity Rate";
  EnergyManagementSystemActuator equipActuator(electricEquipment, ComponentType, equipControlType);
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());

  std::string equipName = electricEquipment.name().get() + "power level Actuator";
  equipActuator.setName(equipName);

  equipActuator.setActuatedComponentControlType(equipControlType);

  EXPECT_EQ(electricEquipment, equipActuator.actuatedComponent().get());
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());

  equipActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());
}

TEST_F(ModelFixture, EMSActuator_newEMSActuator) {
  Model model;
  //no use spacetype
  Space space1(model);
  ThermalZone zone1(model);
  space1.setThermalZone(zone1);

  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names
  SpaceType spaceType(model);
  Space space2(model);
  ThermalZone zone2(model);
  Space space3(model);
  ThermalZone zone3(model);
  space2.setSpaceType(spaceType);
  space2.setThermalZone(zone2);
  space3.setSpaceType(spaceType);
  space3.setThermalZone(zone3);

  Building building = model.getUniqueModelObject<Building>();

  //add electric equipment and attach to space
  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  //add to space1
  electricEquipment.setSpace(space1);
  EXPECT_TRUE(electricEquipment.space());
  EXPECT_EQ(space1.handle(), electricEquipment.space()->handle());
  EXPECT_FALSE(electricEquipment.spaceType());

  //actuator settings
  std::string elecComponentType = "ElectricEquipment";
  std::string elecControlType = "Electricity Rate";
  //create actuator
  EnergyManagementSystemActuator elecActuator(electricEquipment, elecComponentType, elecControlType, zone1);
  EXPECT_EQ(elecControlType, elecActuator.actuatedComponentControlType());
  EXPECT_EQ(elecComponentType, elecActuator.actuatedComponentType());
  EXPECT_EQ(electricEquipment, elecActuator.actuatedComponent().get());

  //add lights and attach to spaceType
  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(1.0);
  Lights lights(lightsDefinition);
  //add lights to spaceType which space2 and space3 use
  lights.setSpaceType(spaceType);

  //actuator settings
  std::string lightsComponentType = "Lights";
  std::string lightsControlType = "Electricity Rate";
  //create actuator zone2
  EnergyManagementSystemActuator lightsActuator2(lights, lightsComponentType, lightsControlType, zone2);
  EXPECT_EQ(lightsControlType, lightsActuator2.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator2.actuatedComponentType());
  EXPECT_EQ(lights, lightsActuator2.actuatedComponent().get());

  //create actuator zone3
  EnergyManagementSystemActuator lightsActuator3(lights, lightsComponentType, lightsControlType, zone3);
  EXPECT_EQ(lightsControlType, lightsActuator3.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator3.actuatedComponentType());
  EXPECT_EQ(lights, lightsActuator3.actuatedComponent().get());
}

TEST_F(ModelFixture, EMSActuator_newEMSActuator2) {
  //USE ONLY 1 TZ in constructor
  Model model;

  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names
  SpaceType spaceType(model);
  Space space2(model);
  ThermalZone zone2(model);
  Space space3(model);
  space2.setSpaceType(spaceType);
  space2.setThermalZone(zone2);
  space3.setSpaceType(spaceType);
  space3.setThermalZone(zone2);

  Building building = model.getUniqueModelObject<Building>();

  //add lights and attach to spaceType
  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(1.0);
  Lights lights(lightsDefinition);
  //add lights to spaceType which space2 and space3 use
  lights.setSpaceType(spaceType);

  //actuator settings
  std::string lightsComponentType = "Lights";
  std::string lightsControlType = "Electricity Rate";
  //create actuator zone2
  EnergyManagementSystemActuator lightsActuator2(lights, lightsComponentType, lightsControlType, zone2);
  EXPECT_EQ(lightsControlType, lightsActuator2.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator2.actuatedComponentType());
  EXPECT_EQ(lights, lightsActuator2.actuatedComponent().get());
  ASSERT_TRUE(lightsActuator2.zoneOrSpace());
  EXPECT_EQ(zone2.handle(), lightsActuator2.zoneOrSpace()->handle());

  //create actuator zone3
  EnergyManagementSystemActuator lightsActuator3(lights, lightsComponentType, lightsControlType, zone2);
  EXPECT_EQ(lightsControlType, lightsActuator3.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator3.actuatedComponentType());
  EXPECT_EQ(lights, lightsActuator3.actuatedComponent().get());
  ASSERT_TRUE(lightsActuator3.zoneOrSpace());
  EXPECT_EQ(zone2.handle(), lightsActuator3.zoneOrSpace()->handle());
}

TEST_F(ModelFixture, EMSActuator_newEMSActuator3) {
  //USE spaces in constructor
  Model model;

  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names
  SpaceType spaceType(model);
  Space space2(model);
  ThermalZone zone2(model);
  Space space3(model);
  space2.setSpaceType(spaceType);
  space2.setThermalZone(zone2);
  space3.setSpaceType(spaceType);
  space3.setThermalZone(zone2);

  Building building = model.getUniqueModelObject<Building>();

  //add lights and attach to spaceType
  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(1.0);
  Lights lights(lightsDefinition);
  //add lights to spaceType which space2 and space3 use
  lights.setSpaceType(spaceType);

  //actuator settings
  std::string lightsComponentType = "Lights";
  std::string lightsControlType = "Electricity Rate";
  //create actuator zone2
  EnergyManagementSystemActuator lightsActuator2(lights, lightsComponentType, lightsControlType, space2);
  EXPECT_EQ(lightsControlType, lightsActuator2.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator2.actuatedComponentType());
  EXPECT_EQ(lights, lightsActuator2.actuatedComponent().get());
  ASSERT_TRUE(lightsActuator2.zoneOrSpace());
  EXPECT_EQ(space2.handle(), lightsActuator2.zoneOrSpace()->handle());

  //create actuator zone3
  EnergyManagementSystemActuator lightsActuator3(lights, lightsComponentType, lightsControlType, space3);
  EXPECT_EQ(lightsControlType, lightsActuator3.actuatedComponentControlType());
  EXPECT_EQ(lightsComponentType, lightsActuator3.actuatedComponentType());
  ASSERT_TRUE(lightsActuator3.zoneOrSpace());
  EXPECT_EQ(space3.handle(), lightsActuator3.zoneOrSpace()->handle());
}

TEST_F(ModelFixture, EMSActuator_SpaceLoad_EnforceZoneOrSpace) {
  //USE spaces in constructor
  Model model;

  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names
  SpaceType spaceType(model);
  Space space(model);
  ThermalZone zone(model);
  space.setSpaceType(spaceType);
  space.setThermalZone(zone);

  Building building = model.getUniqueModelObject<Building>();

  //add lights and attach to spaceType
  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(1.0);
  Lights lights(lightsDefinition);
  //add lights to spaceType which space2 and space3 use
  lights.setSpaceType(spaceType);

  ElectricEquipmentDefinition electricEquipmentDefinition(model);
  ElectricEquipment electricEquipment(electricEquipmentDefinition);

  // actuator settings
  const std::string lightsComponentType = "Lights";
  const std::string lightsControlType = "Electricity Rate";

  // If passing a Space or ThermalZone, we throw if the actuated component is not a SpaceLoadInstance
  EXPECT_ANY_THROW(EnergyManagementSystemActuator(building, lightsComponentType, lightsControlType, space));
  EXPECT_ANY_THROW(EnergyManagementSystemActuator(building, lightsComponentType, lightsControlType, zone));
  EXPECT_ANY_THROW(EnergyManagementSystemActuator(lightsDefinition, lightsComponentType, lightsControlType, space));
  EXPECT_ANY_THROW(EnergyManagementSystemActuator(lightsDefinition, lightsComponentType, lightsControlType, zone));

  {
    EnergyManagementSystemActuator lightsActuator(lights, lightsComponentType, lightsControlType, space);
    EXPECT_EQ(lights, lightsActuator.actuatedComponent().get());
    EXPECT_EQ(lightsControlType, lightsActuator.actuatedComponentControlType());
    EXPECT_EQ(lightsComponentType, lightsActuator.actuatedComponentType());
    ASSERT_TRUE(lightsActuator.zoneOrSpace());
    ASSERT_TRUE(lightsActuator.space());
    EXPECT_FALSE(lightsActuator.thermalZone());
    EXPECT_EQ(space.handle(), lightsActuator.zoneOrSpace()->handle());
    EXPECT_EQ(space.handle(), lightsActuator.space()->handle());

    // Setting to something that is a SpaceLoadInstance doesn't reset the ZoneOrSpace, but any other type does
    EXPECT_TRUE(lightsActuator.setActuatedComponent(electricEquipment));
    ASSERT_TRUE(lightsActuator.zoneOrSpace());
    ASSERT_TRUE(lightsActuator.space());
    EXPECT_FALSE(lightsActuator.thermalZone());
    EXPECT_EQ(space.handle(), lightsActuator.zoneOrSpace()->handle());
    EXPECT_EQ(space.handle(), lightsActuator.space()->handle());

    EXPECT_TRUE(lightsActuator.setActuatedComponent(building));
    EXPECT_FALSE(lightsActuator.zoneOrSpace());
    EXPECT_FALSE(lightsActuator.space());
    EXPECT_FALSE(lightsActuator.thermalZone());
  }

  {
    EnergyManagementSystemActuator lightsActuator(lights, lightsComponentType, lightsControlType, zone);
    EXPECT_EQ(lights, lightsActuator.actuatedComponent().get());
    EXPECT_EQ(lightsControlType, lightsActuator.actuatedComponentControlType());
    EXPECT_EQ(lightsComponentType, lightsActuator.actuatedComponentType());
    ASSERT_TRUE(lightsActuator.zoneOrSpace());
    EXPECT_FALSE(lightsActuator.space());
    ASSERT_TRUE(lightsActuator.thermalZone());
    EXPECT_EQ(zone.handle(), lightsActuator.zoneOrSpace()->handle());
    EXPECT_EQ(zone.handle(), lightsActuator.thermalZone()->handle());

    // Setting to something that is a SpaceLoadInstance doesn't reset the ZoneOrSpace, but any other type does
    EXPECT_TRUE(lightsActuator.setActuatedComponent(electricEquipment));
    ASSERT_TRUE(lightsActuator.zoneOrSpace());
    EXPECT_FALSE(lightsActuator.space());
    ASSERT_TRUE(lightsActuator.thermalZone());
    EXPECT_EQ(zone.handle(), lightsActuator.zoneOrSpace()->handle());
    EXPECT_EQ(zone.handle(), lightsActuator.thermalZone()->handle());

    EXPECT_TRUE(lightsActuator.setActuatedComponent(building));
    EXPECT_FALSE(lightsActuator.zoneOrSpace());
    EXPECT_FALSE(lightsActuator.space());
    EXPECT_FALSE(lightsActuator.thermalZone());
  }
}
