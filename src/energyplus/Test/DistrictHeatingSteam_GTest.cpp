/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DistrictHeatingSteam.hpp"

#include <utilities/idd/DistrictHeating_Steam_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DistrictHeatingSteam) {
  Model m;

  PlantLoop plant_loop(m);

  DistrictHeatingSteam districtHeatingSteam(m);

  EXPECT_TRUE(districtHeatingSteam.setNominalCapacity(1.0));
  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(districtHeatingSteam.setCapacityFractionSchedule(scheduleConstant));

  EXPECT_TRUE(plant_loop.addSupplyBranchForComponent(districtHeatingSteam));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DistrictHeating_Steam).size());

  IdfObject idf_dhs = w.getObjectsByType(IddObjectType::DistrictHeating_Steam)[0];

  EXPECT_EQ(districtHeatingSteam.nameString(), idf_dhs.getString(DistrictHeating_SteamFields::Name, false).get());
  EXPECT_EQ(districtHeatingSteam.inletModelObject().get().nameString(),
            idf_dhs.getString(DistrictHeating_SteamFields::SteamInletNodeName, false).get());
  EXPECT_EQ(districtHeatingSteam.outletModelObject().get().nameString(),
            idf_dhs.getString(DistrictHeating_SteamFields::SteamOutletNodeName, false).get());
  EXPECT_EQ(1.0, idf_dhs.getDouble(DistrictHeating_SteamFields::NominalCapacity, false).get());
  EXPECT_EQ(scheduleConstant.nameString(), idf_dhs.getString(DistrictHeating_SteamFields::CapacityFractionScheduleName, false).get());
}
