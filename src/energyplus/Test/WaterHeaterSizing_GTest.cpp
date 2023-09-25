/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/WaterHeaterSizing.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/PlantLoop.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/WaterHeater_Sizing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_WaterHeaterSizing) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterStratified wh(m);
  PlantLoop p(m);
  EXPECT_TRUE(p.addSupplyBranchForComponent(wh));

  WaterHeaterSizing waterHeaterSizing = wh.waterHeaterSizing();

  // PeakDraw
  waterHeaterSizing.setDesignMode("PeakDraw");
  waterHeaterSizing.setTimeStorageCanMeetPeakDraw(0.54);
  waterHeaterSizing.setTimeforTankRecovery(0.05);
  waterHeaterSizing.setNominalTankVolumeforAutosizingPlantConnections(1.25);

  // ResidentialHUD-FHAMinimum
  waterHeaterSizing.setNumberofBedrooms(50);
  waterHeaterSizing.setNumberofBathrooms(20);

  // PerPerson
  waterHeaterSizing.setStorageCapacityperPerson(0.50);
  waterHeaterSizing.setRecoveryCapacityperPerson(0.25);

  // PerFloorArea
  waterHeaterSizing.setStorageCapacityperFloorArea(0.05);
  waterHeaterSizing.setRecoveryCapacityperFloorArea(0.0025);

  // PerUnit
  waterHeaterSizing.setNumberofUnits(60);
  waterHeaterSizing.setStorageCapacityperUnit(0.60);
  waterHeaterSizing.setRecoveryCapacityPerUnit(0.30);

  // PerSolarCollectorArea
  waterHeaterSizing.setStorageCapacityperCollectorArea(10.0);

  // For WaterHeaterStratified only, if VerticalCylinder and Tank Height is autosized
  waterHeaterSizing.setHeightAspectRatio(4.5);

  {
    Workspace w = ft.translateModel(m);
    auto whs = w.getObjectsByType(IddObjectType::WaterHeater_Stratified);
    ASSERT_EQ(1u, whs.size());
    auto idf_wh = whs[0];

    auto idfSizings = w.getObjectsByType(IddObjectType::WaterHeater_Sizing);
    ASSERT_EQ(1u, idfSizings.size());
    auto idfSizing = idfSizings[0];

    EXPECT_EQ(idf_wh.nameString(), idfSizing.getString(WaterHeater_SizingFields::WaterHeaterName).get());
    EXPECT_EQ("PeakDraw", idfSizing.getString(WaterHeater_SizingFields::DesignMode).get());
    EXPECT_EQ(0.54, idfSizing.getDouble(WaterHeater_SizingFields::TimeStorageCanMeetPeakDraw).get());
    EXPECT_EQ(0.05, idfSizing.getDouble(WaterHeater_SizingFields::TimeforTankRecovery).get());
    EXPECT_EQ(1.25, idfSizing.getDouble(WaterHeater_SizingFields::NominalTankVolumeforAutosizingPlantConnections).get());
    EXPECT_EQ(50, idfSizing.getInt(WaterHeater_SizingFields::NumberofBedrooms).get());
    EXPECT_EQ(20, idfSizing.getInt(WaterHeater_SizingFields::NumberofBathrooms).get());
    EXPECT_EQ(0.5, idfSizing.getDouble(WaterHeater_SizingFields::StorageCapacityperPerson).get());
    EXPECT_EQ(0.25, idfSizing.getDouble(WaterHeater_SizingFields::RecoveryCapacityperPerson).get());
    EXPECT_EQ(0.05, idfSizing.getDouble(WaterHeater_SizingFields::StorageCapacityperFloorArea).get());
    EXPECT_EQ(0.0025, idfSizing.getDouble(WaterHeater_SizingFields::RecoveryCapacityperFloorArea).get());
    EXPECT_EQ(60.0, idfSizing.getDouble(WaterHeater_SizingFields::NumberofUnits).get());
    EXPECT_EQ(0.6, idfSizing.getDouble(WaterHeater_SizingFields::StorageCapacityperUnit).get());
    EXPECT_EQ(0.3, idfSizing.getDouble(WaterHeater_SizingFields::RecoveryCapacityPerUnit).get());
    EXPECT_EQ(10.0, idfSizing.getDouble(WaterHeater_SizingFields::StorageCapacityperCollectorArea).get());
    EXPECT_EQ(4.5, idfSizing.getDouble(WaterHeater_SizingFields::HeightAspectRatio).get());
  }
}
