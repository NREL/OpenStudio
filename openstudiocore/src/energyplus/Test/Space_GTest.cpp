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
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ThermalZone.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_Space)
{
  Model model;

  Space space(model);

  ForwardTranslator ft;

  // space not translated when not in thermal zone
  Workspace workspace = ft.translateModel(model);
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  // space not translated when not in thermal zone
  workspace = ft.translateModel(model);
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  // TODO: add another space to the thermal zone
  Space space2(model);
  space2.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
}


