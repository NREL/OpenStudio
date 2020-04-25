/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Model.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpWaterToWaterEquationFit) {

  Model(m);

  PlantLoop plant_loop_cup_clg(m);
  PlantLoop plant_loop_cup_htg(m);
  PlantLoop plant_loop_wwhp_clg(m);
  PlantLoop plant_loop_wwhp_htg(m);

  HeatPumpWaterToWaterEquationFitCooling wwhp_clg(m);
  HeatPumpWaterToWaterEquationFitHeating wwhp_htg(m);

  EXPECT_TRUE(plant_loop_cup_clg.addDemandBranchForComponent(wwhp_clg));
  EXPECT_TRUE(plant_loop_wwhp_clg.addSupplyBranchForComponent(wwhp_clg));
  EXPECT_TRUE(plant_loop_cup_htg.addDemandBranchForComponent(wwhp_htg));
  EXPECT_TRUE(plant_loop_wwhp_htg.addSupplyBranchForComponent(wwhp_htg));

  // #3837: These two reference each other, and we want to avoid a recusion problem (each FT function calling each other)
  EXPECT_TRUE(wwhp_clg.setCompanionHeatingHeatPump(wwhp_htg));
  EXPECT_TRUE(wwhp_htg.setCompanionCoolingHeatPump(wwhp_clg));

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  {
    WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::HeatPump_WaterToWater_EquationFit_Cooling));
    EXPECT_EQ(1u, idf_ccs.size());
    WorkspaceObject idf_cc(idf_ccs[0]);
    // Companion
    EXPECT_EQ(wwhp_htg.nameString(), idf_cc.getString(HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName).get());
  }

  {
    WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::HeatPump_WaterToWater_EquationFit_Heating));
    EXPECT_EQ(1u, idf_hcs.size());
    WorkspaceObject idf_hc(idf_hcs[0]);
    // Companion
    EXPECT_EQ(wwhp_clg.nameString(), idf_hc.getString(HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName).get());
  }
}
