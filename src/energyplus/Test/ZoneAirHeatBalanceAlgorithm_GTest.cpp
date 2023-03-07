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
#include "../../model/ZoneAirHeatBalanceAlgorithm.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneAirHeatBalanceAlgorithm) {
  Model model;
  ZoneAirHeatBalanceAlgorithm zoneAHBA = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  EXPECT_TRUE(zoneAHBA.setAlgorithm("AnalyticalSolution"));
  EXPECT_TRUE(zoneAHBA.setDoSpaceHeatBalanceforSizing(false));
  EXPECT_TRUE(zoneAHBA.setDoSpaceHeatBalanceforSimulation(true));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneAirHeatBalanceAlgorithm).size());

  IdfObject idf_zoneAHBA = workspace.getObjectsByType(IddObjectType::ZoneAirHeatBalanceAlgorithm)[0];

  EXPECT_EQ("AnalyticalSolution", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, false).get());
  EXPECT_EQ("No", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, false).get());
  EXPECT_EQ("Yes", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneAirHeatBalanceAlgorithm) {
  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  Model model;
  model = rt.translateWorkspace(w);
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());

  OptionalWorkspaceObject idf_zoneAHBA = w.addObject(IdfObject(IddObjectType::ZoneAirHeatBalanceAlgorithm));
  ASSERT_TRUE(idf_zoneAHBA);

  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, "EulerMethod"));
  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "Yes"));
  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "No"));

  model = rt.translateWorkspace(w);
  ZoneAirHeatBalanceAlgorithm zoneAHBA = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  EXPECT_EQ("EulerMethod", zoneAHBA.algorithm());
  EXPECT_TRUE(zoneAHBA.doSpaceHeatBalanceforSizing());
  EXPECT_FALSE(zoneAHBA.doSpaceHeatBalanceforSimulation());
}
