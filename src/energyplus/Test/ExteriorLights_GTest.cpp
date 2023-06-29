/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLights_Impl.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/ExteriorLightsDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Exterior_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ExteriorLights) {
  Model model;

  ExteriorLightsDefinition exteriorLightsDefinition(model);
  exteriorLightsDefinition.setDesignLevel(2303.3);

  ScheduleConstant schedule(model);
  schedule.setValue(1.0);

  ExteriorLights exteriorLights(exteriorLightsDefinition, schedule);
  exteriorLights.setControlOption("AstronomicalClock");
  exteriorLights.setName("My ExteriorLights");
  exteriorLights.setEndUseSubcategory("Exterior Lighting");

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::Exterior_Lights));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_extEq(idfObjs[0]);

  // Name
  EXPECT_EQ("My ExteriorLights", idf_extEq.getString(Exterior_LightsFields::Name).get());

  // ControlOption
  EXPECT_EQ("AstronomicalClock", idf_extEq.getString(Exterior_LightsFields::ControlOption).get());

  // Schedule Name
  ASSERT_EQ(schedule.nameString(), idf_extEq.getString(Exterior_LightsFields::ScheduleName).get());

  // Design Level
  EXPECT_DOUBLE_EQ(2303.3, idf_extEq.getDouble(Exterior_LightsFields::DesignLevel).get());

  // End Use Subcategory
  EXPECT_EQ("Exterior Lighting", idf_extEq.getString(Exterior_LightsFields::EndUseSubcategory).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ExteriorLights) {
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType iddFileType(IddFileType::EnergyPlus);
  Workspace workspace(level, iddFileType);

  IdfObject idf_extEq(IddObjectType::Exterior_Lights);
  idf_extEq.setName("My ExteriorLights");

  // Control Option
  EXPECT_TRUE(idf_extEq.setString(Exterior_LightsFields::ControlOption, "AstronomicalClock"));

  // Schedule Name
  IdfObject idf_sch(IddObjectType::Schedule_Constant);
  idf_sch.setName("My Schedule");
  EXPECT_TRUE(idf_extEq.setString(Exterior_LightsFields::ScheduleName, idf_sch.name().get()));

  // Design Level
  EXPECT_TRUE(idf_extEq.setDouble(Exterior_LightsFields::DesignLevel, 2303.3));

  // End Use Subcategory
  EXPECT_TRUE(idf_extEq.setString(Exterior_LightsFields::EndUseSubcategory, "My EndUseSubcategory"));

  IdfObjectVector objects;
  objects.push_back(idf_extEq);
  objects.push_back(idf_sch);
  EXPECT_EQ(2u, workspace.addObjects(objects).size());

  ReverseTranslator rt;
  Model m = rt.translateWorkspace(workspace);

  ASSERT_EQ(1u, m.getConcreteModelObjects<ScheduleConstant>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorLightsDefinition>().size());
  ASSERT_EQ(1u, m.getConcreteModelObjects<ExteriorLights>().size());
  ExteriorLights extEq = m.getConcreteModelObjects<ExteriorLights>()[0];

  EXPECT_EQ("My ExteriorLights", extEq.name().get());

  EXPECT_EQ("AstronomicalClock", extEq.controlOption());

  ASSERT_TRUE(extEq.schedule());
  EXPECT_EQ("My Schedule", extEq.schedule().get().name().get());

  ExteriorLightsDefinition extEqDef = extEq.exteriorLightsDefinition();
  EXPECT_DOUBLE_EQ(2303.3, extEqDef.designLevel());

  EXPECT_EQ("My EndUseSubcategory", extEq.endUseSubcategory());
}
