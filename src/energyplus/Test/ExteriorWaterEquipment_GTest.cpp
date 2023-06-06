/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ExteriorWaterEquipment.hpp"
#include "../../model/ExteriorWaterEquipment_Impl.hpp"
#include "../../model/ExteriorWaterEquipmentDefinition.hpp"
#include "../../model/ExteriorWaterEquipmentDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Exterior_WaterEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ExteriorWaterEquipment) {
  Model m;

  ExteriorWaterEquipmentDefinition exteriorWaterEquipmentDefinition(m);
  exteriorWaterEquipmentDefinition.setDesignLevel(2303.3);

  ScheduleConstant sch(m);
  sch.setValue(1.0);

  ExteriorWaterEquipment exteriorWaterEquipment(exteriorWaterEquipmentDefinition, sch);
  exteriorWaterEquipment.setName("My ExteriorWaterEquipment");
  exteriorWaterEquipment.setEndUseSubcategory("Irrigation");

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Exterior_WaterEquipment));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_extEq(idfObjs[0]);

  // Name
  EXPECT_EQ("My ExteriorWaterEquipment", idf_extEq.getString(Exterior_WaterEquipmentFields::Name).get());

  // Fuel Use Type
  EXPECT_EQ("Water", idf_extEq.getString(Exterior_WaterEquipmentFields::FuelUseType).get());

  // Schedule Name
  ASSERT_EQ(sch.nameString(), idf_extEq.getString(Exterior_WaterEquipmentFields::ScheduleName).get());

  // Design Level
  EXPECT_DOUBLE_EQ(2303.3, idf_extEq.getDouble(Exterior_WaterEquipmentFields::DesignLevel).get());

  // End Use Subcategory
  EXPECT_EQ("Irrigation", idf_extEq.getString(Exterior_WaterEquipmentFields::EndUseSubcategory).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ExteriorWaterEquipment) {
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType iddFileType(IddFileType::EnergyPlus);
  Workspace workspace(level, iddFileType);

  IdfObject idf_extEq(IddObjectType::Exterior_WaterEquipment);
  idf_extEq.setName("My ExteriorWaterEquipment");

  // Fuel Use Type
  EXPECT_TRUE(idf_extEq.setString(Exterior_WaterEquipmentFields::FuelUseType, "Water"));

  // Schedule Name
  IdfObject idf_sch(IddObjectType::Schedule_Constant);
  idf_sch.setName("My Schedule");
  EXPECT_TRUE(idf_extEq.setString(Exterior_WaterEquipmentFields::ScheduleName, idf_sch.name().get()));

  // Design Level
  EXPECT_TRUE(idf_extEq.setDouble(Exterior_WaterEquipmentFields::DesignLevel, 2303.3));

  // End Use Subcategory
  EXPECT_TRUE(idf_extEq.setString(Exterior_WaterEquipmentFields::EndUseSubcategory, "My EndUseSubcategory"));

  IdfObjectVector objects;
  objects.push_back(idf_extEq);
  objects.push_back(idf_sch);
  EXPECT_EQ(2u, workspace.addObjects(objects).size());

  ReverseTranslator rt;
  Model m = rt.translateWorkspace(workspace);

  ASSERT_EQ(1u, m.getConcreteModelObjects<ScheduleConstant>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorWaterEquipment>().size());
  ExteriorWaterEquipment extEq = m.getConcreteModelObjects<ExteriorWaterEquipment>()[0];

  EXPECT_EQ("My ExteriorWaterEquipment", extEq.name().get());

  EXPECT_EQ("My Schedule", extEq.schedule().name().get());

  ExteriorWaterEquipmentDefinition extEqDef = extEq.exteriorWaterEquipmentDefinition();
  EXPECT_DOUBLE_EQ(2303.3, extEqDef.designLevel());

  EXPECT_EQ("My EndUseSubcategory", extEq.endUseSubcategory());
}
