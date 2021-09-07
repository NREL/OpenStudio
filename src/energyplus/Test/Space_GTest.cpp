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
#include "../../model/Space.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ThermalZone.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Space) {
  Model model;

  Space space(model);

  ForwardTranslator ft;

  // space not translated when not in thermal zone
  Workspace workspace = ft.translateModel(model);
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  // space translated when in thermal zone
  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Space).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  // add another space to the thermal zone
  Space space2(model);
  space2.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  WorkspaceObjectVector idf_spaces(workspace.getObjectsByType(IddObjectType::Spaces));
  EXPECT_EQ(2u, idf_spaces.size());
  WorkspaceObject idf_space1(idf_spaces[0]);
  WorkspaceObject idf_space2(idf_spaces[1]);

  boost::optional<WorkspaceObject> idf_zone1(idf_space1.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone1);
  EXPECT_EQ(idf_zone1->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone1->name().get());
  boost::optional<WorkspaceObject> idf_zone2(idf_space2.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone2);
  EXPECT_EQ(idf_zone2->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone2->name().get());
  EXPECT_EQ(idf_zone1, idf_zone2);

  EXPECT_EQ(0, idf_space1.getDouble(SpaceFields::FloorArea, false).get());
  EXPECT_EQ(0, idf_space2.getDouble(SpaceFields::FloorArea, false).get());

  EXPECT_EQ("", idf_space1.getString(SpaceFields::SpaceType, false).get());
  EXPECT_EQ("", idf_space2.getString(SpaceFields::SpaceType, false).get());
}
