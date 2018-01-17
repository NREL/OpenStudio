/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipment_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipmentDefinition_Impl.hpp"
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

TEST_F(ModelFixture, EMSActuator_EMSActuator)
{
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

  EXPECT_EQ(fan, fanActuator.actuatedComponent());
  EXPECT_EQ(fanControlType, fanActuator.actuatedComponentControlType());
 
  fanActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());

  fanActuator.setActuatedComponent(fan2);
  EXPECT_EQ(fan2, fanActuator.actuatedComponent());

  //add electric equipment actuator
  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  ComponentType = "ElectricEquipment";
  std::string equipControlType = "Electric Power Level";
  EnergyManagementSystemActuator equipActuator(electricEquipment, ComponentType, equipControlType);
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());

  std::string equipName = electricEquipment.name().get() + "power level Actuator";
  equipActuator.setName(equipName);

  equipActuator.setActuatedComponentControlType(equipControlType);

  EXPECT_EQ(electricEquipment, equipActuator.actuatedComponent());
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());

  equipActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());

}

