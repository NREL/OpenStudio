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
  ASSERT_EQ(schedule.name(), idf_extEq.getString(Exterior_LightsFields::ScheduleName).get());

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

  ASSERT_EQ(1u, m.getModelObjects<ScheduleConstant>().size());
  ASSERT_EQ(1u, m.getModelObjects<ExteriorLightsDefinition>().size());
  ASSERT_EQ(1u, m.getModelObjects<ExteriorLights>().size());
  ExteriorLights extEq = m.getModelObjects<ExteriorLights>()[0];

  EXPECT_EQ("My ExteriorLights", extEq.name().get());

  EXPECT_EQ("AstronomicalClock", extEq.controlOption());

  ASSERT_TRUE(extEq.schedule());
  EXPECT_EQ("My Schedule", extEq.schedule().get().name().get());

  ExteriorLightsDefinition extEqDef = extEq.exteriorLightsDefinition();
  EXPECT_DOUBLE_EQ(2303.3, extEqDef.designLevel());

  EXPECT_EQ("My EndUseSubcategory", extEq.endUseSubcategory());
}
