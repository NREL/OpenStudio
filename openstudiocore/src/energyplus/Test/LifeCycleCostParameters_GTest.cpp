/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/LifeCycleCostParameters.hpp"
#include "../../model/LifeCycleCostParameters_Impl.hpp"

#include <utilities/idd/LifeCycleCost_Parameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCostParameters)
{
  Model model;
  LifeCycleCostParameters lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();

  ForwardTranslator ft;
  Workspace workspace = ft.translateModelObject(lifeCycleCostParameters);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_Parameters).size());
  EXPECT_EQ(5u, workspace.getObjectsByType(IddObjectType::LifeCycleCost_UsePriceEscalation).size());

  const char *c_regions[] = {"U.S. Avg", "NorthEast", "MidWest", "South", "West"};
  const char *c_sectors[] = {"Commercial", "Residential", "Industrial"};

  std::vector<std::string> regions(c_regions, c_regions + 5);
  std::vector<std::string> sectors(c_sectors, c_sectors + 3);
  EXPECT_EQ(5u, regions.size());
  EXPECT_EQ(3u, sectors.size());

  EXPECT_TRUE(lifeCycleCostParameters.setUseNISTFuelEscalationRates(true));

  for (const std::string& region : regions){
    for (const std::string& sector : sectors){
      EXPECT_TRUE(lifeCycleCostParameters.setNISTRegion(region));
      EXPECT_TRUE(lifeCycleCostParameters.setNISTSector(sector));

      unsigned numExpected = 5;
      if (sector == "Residential"){
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
