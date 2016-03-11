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
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_DaylightingControl_NoZone)
{
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_DaylightingControl_OneControl)
{
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);
  
  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_DaylightingControl_TwoControl)
{
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl1(model);
  daylightingControl1.setSpace(space);
  DaylightingControl daylightingControl2(model);
  daylightingControl2.setSpace(space);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl1));
  EXPECT_TRUE(thermalZone.setSecondaryDaylightingControl(daylightingControl2));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}
