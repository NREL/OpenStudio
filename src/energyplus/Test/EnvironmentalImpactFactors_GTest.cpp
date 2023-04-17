/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/EnvironmentalImpactFactors.hpp"
#include "../../model/EnvironmentalImpactFactors_Impl.hpp"

#include "../../model/OutputEnvironmentalImpactFactors.hpp"
#include "../../model/FuelFactors.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_EnvironmentalImpactFactors) {

  ForwardTranslator ft;

  Model m;

  // Get the unique object
  auto environmentalImpactFactors = m.getUniqueModelObject<EnvironmentalImpactFactors>();

  EXPECT_TRUE(environmentalImpactFactors.setDistrictHeatingEfficiency(0.35));
  EXPECT_TRUE(environmentalImpactFactors.setDistrictCoolingCOP(3.3));
  EXPECT_TRUE(environmentalImpactFactors.setSteamConversionEfficiency(0.27));
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromN2O(79.2));
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromCH4(6.1));
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromCO2(0.31));

  // Without the OutputEnvironmentalImpactFactors: not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::EnvironmentalImpactFactors);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Create one
  OutputEnvironmentalImpactFactors o(m);
  // It also needs at least one FuelFactor to be translated
  FuelFactors fuelFactors(m);

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::EnvironmentalImpactFactors);
    ASSERT_EQ(1u, idfObjs.size());
    auto idfObject = idfObjs[0];

    EXPECT_EQ(0.35, idfObject.getDouble(EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency).get());
    EXPECT_EQ(3.3, idfObject.getDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP).get());
    EXPECT_EQ(0.27, idfObject.getDouble(EnvironmentalImpactFactorsFields::SteamConversionEfficiency).get());
    EXPECT_EQ(79.2, idfObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O).get());
    EXPECT_EQ(6.1, idfObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4).get());
    EXPECT_EQ(0.31, idfObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_EnvironmentalImpactFactors) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<EnvironmentalImpactFactors>());
  }

  OptionalWorkspaceObject _i_environmentalImpactFactors = w.addObject(IdfObject(IddObjectType::EnvironmentalImpactFactors));
  ASSERT_TRUE(_i_environmentalImpactFactors);
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, 0.35));
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP, 3.3));
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::SteamConversionEfficiency, 0.27));
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, 79.2));
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, 6.1));
  EXPECT_TRUE(_i_environmentalImpactFactors->setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, 0.31));

  {
    Model m = rt.translateWorkspace(w);
    ASSERT_TRUE(m.getOptionalUniqueModelObject<EnvironmentalImpactFactors>());
    auto environmentalImpactFactors = m.getUniqueModelObject<EnvironmentalImpactFactors>();

    EXPECT_EQ(0.35, environmentalImpactFactors.districtHeatingEfficiency());
    EXPECT_EQ(3.3, environmentalImpactFactors.districtCoolingCOP());
    EXPECT_EQ(0.27, environmentalImpactFactors.steamConversionEfficiency());
    EXPECT_EQ(79.2, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromN2O());
    EXPECT_EQ(6.1, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromCH4());
    EXPECT_EQ(0.31, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromCO2());
  }
}
