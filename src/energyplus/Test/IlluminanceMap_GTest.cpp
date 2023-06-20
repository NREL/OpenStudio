/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/IlluminanceMap.hpp"
#include "../../model/IlluminanceMap_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/Output_IlluminanceMap_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_IlluminanceMap_NoZone) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  IlluminanceMap illuminanceMap(model);
  illuminanceMap.setSpace(space);

  ForwardTranslator forwardTranslator;
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::Output_IlluminanceMap).size());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::Output_IlluminanceMap).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_IlluminanceMap) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  IlluminanceMap illuminanceMap(model);
  illuminanceMap.setSpace(space);

  EXPECT_TRUE(thermalZone.setIlluminanceMap(illuminanceMap));

  ForwardTranslator forwardTranslator;
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Output_IlluminanceMap).size());

    // automatically added
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }

  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Output_IlluminanceMap).size());

    // automatically added
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
  }
}
