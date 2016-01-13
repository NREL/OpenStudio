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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_SpaceInfiltrationEffectiveLeakageArea)
{
  Model model;
  SpaceInfiltrationEffectiveLeakageArea infiltration(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_SpaceInfiltrationEffectiveLeakageArea_NoSpace)
{
  Model model;
  SpaceInfiltrationEffectiveLeakageArea infiltration(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea)[0];
  EXPECT_TRUE(object.isEmpty(ZoneInfiltration_EffectiveLeakageAreaFields::ZoneName));
}
