/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/LifeCycleCostParameters.hpp"
#include "../../model/LifeCycleCostParameters_Impl.hpp"

#include <utilities/idd/LifeCycleCost_Parameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_LifeCycleCostParameters) {
  Model model;
  LifeCycleCostParameters lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();

  ForwardTranslator ft;
  Workspace workspace = ft.translateModelObject(lifeCycleCostParameters);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_Parameters).size());
  EXPECT_EQ(5u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_UsePriceEscalation).size());

  const char* c_regions[] = {"U.S. Avg", "NorthEast", "MidWest", "South", "West"};
  const char* c_sectors[] = {"Commercial", "Residential", "Industrial"};

  std::vector<std::string> regions(c_regions, c_regions + 5);
  std::vector<std::string> sectors(c_sectors, c_sectors + 3);
  EXPECT_EQ(5u, regions.size());
  EXPECT_EQ(3u, sectors.size());

  EXPECT_TRUE(lifeCycleCostParameters.setUseNISTFuelEscalationRates(true));

  for (const std::string& region : regions) {
    for (const std::string& sector : sectors) {
      EXPECT_TRUE(lifeCycleCostParameters.setNISTRegion(region));
      EXPECT_TRUE(lifeCycleCostParameters.setNISTSector(sector));

      unsigned numExpected = 5;
      if (sector == "Residential") {
        numExpected = 4;
      }

      workspace = ft.translateModelObject(lifeCycleCostParameters);
      EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_Parameters).size());
      EXPECT_EQ(numExpected, workspace.getObjectsByType(IddObjectType::LifeCycleCost_UsePriceEscalation).size()) << region << " " << sector;
    }
  }

  EXPECT_TRUE(lifeCycleCostParameters.setUseNISTFuelEscalationRates(false));

  workspace = ft.translateModelObject(lifeCycleCostParameters);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_Parameters).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_UsePriceEscalation).size());

  EXPECT_TRUE(lifeCycleCostParameters.setElectricityInflation(0.01));

  workspace = ft.translateModelObject(lifeCycleCostParameters);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_Parameters).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_UsePriceEscalation).size());
}
