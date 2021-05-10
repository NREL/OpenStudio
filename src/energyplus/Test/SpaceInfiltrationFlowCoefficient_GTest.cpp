/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SpaceInfiltrationFlowCoefficient) {
  Model model;
  SpaceInfiltrationFlowCoefficient infiltration(model);
  infiltration.setName("My Infiltration");

  infiltration.resetSchedule();
  EXPECT_FALSE(infiltration.schedule());
  EXPECT_TRUE(infiltration.setFlowCoefficient(0.1));
  EXPECT_TRUE(infiltration.setStackCoefficient(0.2));
  EXPECT_TRUE(infiltration.setPressureExponent(0.3));
  EXPECT_TRUE(infiltration.setWindCoefficient(0.4));
  EXPECT_TRUE(infiltration.setShelterFactor(0.5));

  ForwardTranslator ft;
  // Not assigned a space or spacetype? => not translated
  {

    Workspace w = ft.translateModel(model);

    std::string errors;
    for (const auto& error : ft.errors()) {
      errors += error.logMessage() + "\n";
    }
    EXPECT_EQ(0u, ft.errors().size()) << errors;

    std::string warnings;
    for (const auto& warning : ft.warnings()) {
      warnings += warning.logMessage() + "\n";
    }
    EXPECT_EQ(1u, ft.warnings().size()) << warnings;

    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient).size());
  }

  {
    ThermalZone zone(model);
    zone.setName("Zone1");
    Space space(model);
    space.setThermalZone(zone);
    infiltration.setSpace(space);

    Workspace w = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(0u, ft.warnings().size());

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idfObject(idfObjs[0]);

    EXPECT_EQ("My Infiltration", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::Name).get());
    EXPECT_EQ("Zone1", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ZoneName).get());

    EXPECT_FALSE(infiltration.schedule());
    EXPECT_EQ("Always On Discrete", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ScheduleName).get());

    EXPECT_EQ(0.1, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
    EXPECT_EQ(0.2, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
    EXPECT_EQ(0.3, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
    EXPECT_EQ(0.4, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
    EXPECT_EQ(0.5, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
  }

  {
    ScheduleConstant sch(model);
    sch.setName("My Infiltration Schedule");
    EXPECT_TRUE(infiltration.setSchedule(sch));

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idfObject(idfObjs[0]);

    ASSERT_TRUE(infiltration.schedule());
    EXPECT_EQ(infiltration.schedule()->nameString(), idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ScheduleName).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SpaceInfiltrationFlowCoefficient) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

  OptionalWorkspaceObject _i_infiltration = w.addObject(IdfObject(IddObjectType::ZoneInfiltration_FlowCoefficient));
  ASSERT_TRUE(_i_infiltration);
  _i_infiltration->setName("Infiltration");

  OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_sch);
  _i_sch->setName("InfiltrationSchedule");
  EXPECT_TRUE(_i_sch->setDouble(Schedule_ConstantFields::HourlyValue, 1.0));
  EXPECT_TRUE(_i_infiltration->setPointer(ZoneInfiltration_FlowCoefficientFields::ScheduleName, _i_sch->handle()));

  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient, 0.1));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient, 0.2));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent, 0.3));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient, 0.4));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor, 0.5));

  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  // No Zone assigned: not RT'ed
  {
    ASSERT_NO_THROW(rt.translateWorkspace(w));
    Model model = rt.translateWorkspace(w);
    EXPECT_EQ(1, rt.errors().size());
    EXPECT_TRUE(rt.warnings().empty());

    auto infiltrations = model.getModelObjects<openstudio::model::SpaceInfiltrationFlowCoefficient>();
    EXPECT_EQ(0, infiltrations.size());
  }

  // Zone Name assigned: RT'ed
  {
    OptionalWorkspaceObject _i_zone = w.addObject(IdfObject(IddObjectType::Zone));
    ASSERT_TRUE(_i_zone);
    _i_zone->setName("Zone1");

    EXPECT_TRUE(_i_infiltration->setPointer(ZoneInfiltration_FlowCoefficientFields::ZoneName, _i_zone->handle()));

    ASSERT_NO_THROW(rt.translateWorkspace(w));
    Model model = rt.translateWorkspace(w);
    EXPECT_TRUE(rt.errors().empty());
    EXPECT_TRUE(rt.warnings().empty());

    auto infiltrations = model.getModelObjects<openstudio::model::SpaceInfiltrationFlowCoefficient>();
    ASSERT_EQ(1, infiltrations.size());
    auto infiltration = infiltrations[0];

    EXPECT_EQ("Infiltration", infiltration.nameString());
    ASSERT_TRUE(infiltration.schedule());
    EXPECT_EQ("InfiltrationSchedule", infiltration.schedule()->nameString());
    ASSERT_TRUE(infiltration.space());
    EXPECT_FALSE(infiltration.spaceType());
    EXPECT_EQ("Zone1", infiltration.space()->nameString());
    EXPECT_EQ(0.1, infiltration.flowCoefficient());
    EXPECT_EQ(0.2, infiltration.stackCoefficient());
    EXPECT_EQ(0.3, infiltration.pressureExponent());
    EXPECT_EQ(0.4, infiltration.windCoefficient());
    EXPECT_EQ(0.5, infiltration.shelterFactor());
  }
}
