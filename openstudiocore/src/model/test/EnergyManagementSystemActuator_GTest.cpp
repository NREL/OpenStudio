/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  std::string equipControlType = "Electric Power Level";
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

