/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  ASSERT_EQ(sch.nameString(), idf_extEq.getString(Exterior_FuelEquipmentFields::ScheduleName).get());

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

  ASSERT_EQ(1u, m.getConcreteModelObjects<ScheduleConstant>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorFuelEquipment>().size());
  ExteriorFuelEquipment extEq = m.getConcreteModelObjects<ExteriorFuelEquipment>()[0];

  EXPECT_EQ("My ExteriorFuelEquipment", extEq.name().get());

  EXPECT_EQ("NaturalGas", extEq.fuelType());

  EXPECT_EQ("My Schedule", extEq.schedule().name().get());

  ExteriorFuelEquipmentDefinition extEqDef = extEq.exteriorFuelEquipmentDefinition();
  EXPECT_DOUBLE_EQ(2303.3, extEqDef.designLevel());

  EXPECT_EQ("My EndUseSubcategory", extEq.endUseSubcategory());
}
