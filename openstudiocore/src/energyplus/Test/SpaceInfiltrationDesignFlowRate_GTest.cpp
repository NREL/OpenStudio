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
#include "../../model/ThermalZone.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_SpaceInfiltrationDesignFlowRate)
{
  Model model;

  ThermalZone zone(model);

  Space space(model);
  space.setThermalZone(zone);

  SpaceInfiltrationDesignFlowRate infiltration(model);
  infiltration.setSpace(space);
  infiltration.setFlowperExteriorWallArea(1.0);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::ZoneInfiltration_DesignFlowRate);
  ASSERT_EQ(1u, objects.size());

  ASSERT_TRUE(objects[0].getString(ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod));
  EXPECT_EQ("Flow/ExteriorWallArea", objects[0].getString(ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod).get());
  EXPECT_FALSE(objects[0].getDouble(ZoneInfiltration_DesignFlowRateFields::DesignFlowRate));
  EXPECT_FALSE(objects[0].getDouble(ZoneInfiltration_DesignFlowRateFields::FlowperZoneFloorArea));
  ASSERT_TRUE(objects[0].getDouble(ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea));
  EXPECT_EQ(1.0, objects[0].getDouble(ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea).get());
  EXPECT_FALSE(objects[0].getDouble(ZoneInfiltration_DesignFlowRateFields::AirChangesperHour));

}


