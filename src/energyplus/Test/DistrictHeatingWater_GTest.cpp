/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DistrictHeatingWater.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/DistrictHeating_Water_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DistrictHeatingWater) {
  Model m;

  PlantLoop plant_loop(m);

  DistrictHeatingWater districtHeatingWater(m);

  EXPECT_TRUE(districtHeatingWater.setNominalCapacity(1.0));
  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(districtHeatingWater.setCapacityFractionSchedule(scheduleConstant));

  EXPECT_TRUE(plant_loop.addSupplyBranchForComponent(districtHeatingWater));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DistrictHeating_Water).size());

  IdfObject idf_dhw = w.getObjectsByType(IddObjectType::DistrictHeating_Water)[0];

  EXPECT_EQ(districtHeatingWater.nameString(), idf_dhw.getString(DistrictHeating_WaterFields::Name, false).get());
  EXPECT_EQ(districtHeatingWater.inletModelObject().get().nameString(),
            idf_dhw.getString(DistrictHeating_WaterFields::HotWaterInletNodeName, false).get());
  EXPECT_EQ(districtHeatingWater.outletModelObject().get().nameString(),
            idf_dhw.getString(DistrictHeating_WaterFields::HotWaterOutletNodeName, false).get());
  EXPECT_EQ(1.0, idf_dhw.getDouble(DistrictHeating_WaterFields::NominalCapacity, false).get());
  EXPECT_EQ(scheduleConstant.nameString(), idf_dhw.getString(DistrictHeating_WaterFields::CapacityFractionScheduleName, false).get());
}
