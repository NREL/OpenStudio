/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DistrictCooling.hpp"

#include <utilities/idd/DistrictCooling_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DistrictCooling) {
  Model m;

  PlantLoop plant_loop(m);

  DistrictCooling districtCooling(m);

  EXPECT_TRUE(districtCooling.setNominalCapacity(1.0));
  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(districtCooling.setCapacityFractionSchedule(scheduleConstant));

  EXPECT_TRUE(plant_loop.addSupplyBranchForComponent(districtCooling));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DistrictCooling).size());

  IdfObject idf_dc = w.getObjectsByType(IddObjectType::DistrictCooling)[0];

  EXPECT_EQ(districtCooling.nameString(), idf_dc.getString(DistrictCoolingFields::Name, false).get());
  EXPECT_EQ(districtCooling.inletModelObject().get().nameString(), idf_dc.getString(DistrictCoolingFields::ChilledWaterInletNodeName, false).get());
  EXPECT_EQ(districtCooling.outletModelObject().get().nameString(), idf_dc.getString(DistrictCoolingFields::ChilledWaterOutletNodeName, false).get());
  EXPECT_EQ(1.0, idf_dc.getDouble(DistrictCoolingFields::NominalCapacity, false).get());
  EXPECT_EQ(scheduleConstant.nameString(), idf_dc.getString(DistrictCoolingFields::CapacityFractionScheduleName, false).get());
}
