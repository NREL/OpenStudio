/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  Model m;

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

  // #3837: These two reference each other, and we want to avoid a recursion problem (each FT function calling each other)
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
