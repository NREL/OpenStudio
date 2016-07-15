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

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../Lights.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSActuator_EMSActuator)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  for (const ThermalZone& zone : building.thermalZones()) {

    // all possible variables
    std::vector<std::string> variableNames = zone.outputVariableNames();
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Electric Power") != variableNames.end());
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Radiant Heating Energy") != variableNames.end());

    // variables actually found
    OutputVariableVector variables = zone.outputVariables();
    EXPECT_TRUE(variables.empty());
  }

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electric Power", lightsElectricPower.variableName());

  // add Zone Lights Radiant Heating Energy to only zone1
  OutputVariable lightsRadiantHeatGain("Zone Lights Radiant Heating Energy", model);
  EXPECT_TRUE(lightsRadiantHeatGain.setKeyValue(zone1.name().get()));
  EXPECT_EQ(zone1.name().get(), lightsRadiantHeatGain.keyValue());
  EXPECT_EQ("Zone Lights Radiant Heating Energy", lightsRadiantHeatGain.variableName());

  ASSERT_EQ(2u, zone1.outputVariables().size());
  if (lightsElectricPower.handle() == zone1.outputVariables()[0].handle()){
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[1].handle());
  }else{
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[1].handle());
  }

  ASSERT_EQ(1u, zone2.outputVariables().size());
  EXPECT_EQ(lightsElectricPower.handle(), zone2.outputVariables()[0].handle());

}

