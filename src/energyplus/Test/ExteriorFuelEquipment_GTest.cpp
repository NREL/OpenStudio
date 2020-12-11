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

#include "../../model/ExteriorFuelEquipment.hpp"
#include "../../model/ExteriorFuelEquipment_Impl.hpp"
#include "../../model/ExteriorFuelEquipmentDefinition.hpp"
#include "../../model/ExteriorFuelEquipmentDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Exterior_FuelEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ExteriorFuelEquipment) {
  Model m;

  ExteriorFuelEquipmentDefinition exteriorFuelEquipmentDefinition(m);
  exteriorFuelEquipmentDefinition.setDesignLevel(2303.3);

  ScheduleConstant sch(m);
  sch.setValue(1.0);

  ExteriorFuelEquipment exteriorFuelEquipment(exteriorFuelEquipmentDefinition, sch);
  exteriorFuelEquipment.setName("My ExteriorFuelEquipment");
  exteriorFuelEquipment.setFuelType("NaturalGas");
  exteriorFuelEquipment.setEndUseSubcategory("Water Pump");

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Exterior_FuelEquipment));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_extEq(idfObjs[0]);

  // Name
  EXPECT_EQ("My ExteriorFuelEquipment", idf_extEq.getString(Exterior_FuelEquipmentFields::Name).get());

  // Fuel Use Type
  EXPECT_EQ("NaturalGas", idf_extEq.getString(Exterior_FuelEquipmentFields::FuelUseType).get());

  // Schedule Name
  ASSERT_EQ(sch.name(), idf_extEq.getString(Exterior_FuelEquipmentFields::ScheduleName).get());

  // Design Level
  EXPECT_DOUBLE_EQ(2303.3, idf_extEq.getDouble(Exterior_FuelEquipmentFields::DesignLevel).get());

  // End Use Subcategory
  EXPECT_EQ("Water Pump", idf_extEq.getString(Exterior_FuelEquipmentFields::EndUseSubcategory).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ExteriorFuelEquipment) {
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType iddFileType(IddFileType::EnergyPlus);
  Workspace workspace(level, iddFileType);

  IdfObject idf_extEq(IddObjectType::Exterior_FuelEquipment);
  idf_extEq.setName("My ExteriorFuelEquipment");

  // Fuel Use Type
  EXPECT_TRUE(idf_extEq.setString(Exterior_FuelEquipmentFields::FuelUseType, "NaturalGas"));

  // Schedule Name
  IdfObject idf_sch(IddObjectType::Schedule_Constant);
  idf_sch.setName("My Schedule");
  EXPECT_TRUE(idf_extEq.setString(Exterior_FuelEquipmentFields::ScheduleName, idf_sch.name().get()));

  // Design Level
  EXPECT_TRUE(idf_extEq.setDouble(Exterior_FuelEquipmentFields::DesignLevel, 2303.3));

  // End Use Subcategory
  EXPECT_TRUE(idf_extEq.setString(Exterior_FuelEquipmentFields::EndUseSubcategory, "My EndUseSubcategory"));

  IdfObjectVector objects;
  objects.push_back(idf_extEq);
  objects.push_back(idf_sch);
  EXPECT_EQ(2u, workspace.addObjects(objects).size());

  ReverseTranslator rt;
  Model m = rt.translateWorkspace(workspace);

  ASSERT_EQ(1u, m.getModelObjects<ScheduleConstant>().size());
  ASSERT_EQ(1u, m.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
  ASSERT_EQ(1u, m.getModelObjects<ExteriorFuelEquipment>().size());
  ExteriorFuelEquipment extEq = m.getModelObjects<ExteriorFuelEquipment>()[0];

  EXPECT_EQ("My ExteriorFuelEquipment", extEq.name().get());

  EXPECT_EQ("NaturalGas", extEq.fuelType());

  EXPECT_EQ("My Schedule", extEq.schedule().name().get());

  ExteriorFuelEquipmentDefinition extEqDef = extEq.exteriorFuelEquipmentDefinition();
  EXPECT_DOUBLE_EQ(2303.3, extEqDef.designLevel());

  EXPECT_EQ("My EndUseSubcategory", extEq.endUseSubcategory());
}
