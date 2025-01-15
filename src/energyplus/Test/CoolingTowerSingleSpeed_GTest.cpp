/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/CoolingTower_SingleSpeed_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoolingTowerSingleSpeed) {

  ForwardTranslator ft;

  Model m;
  CoolingTowerSingleSpeed ct(m);

  ct.setName("My CoolingTowerSingleSpeed");

  // Autosize
  // ct.autosizeDesignWaterFlowRate();
  EXPECT_TRUE(ct.setDesignWaterFlowRate(0.4));
  // ct.autosizeDesignAirFlowRate();
  EXPECT_TRUE(ct.setDesignAirFlowRate(0.5));
  // ct.autosizeDesignFanPower();
  EXPECT_TRUE(ct.setFanPoweratDesignAirFlowRate(6000));
  // ct.autosizeDesignUFactorTimesAreaValue();
  EXPECT_TRUE(ct.setUFactorTimesAreaValueatDesignAirFlowRate(1837500.0));
  // Autocalculate
  // ct.autocalculateFreeConvectionRegimeAirFlowRate();
  EXPECT_TRUE(ct.setAirFlowRateinFreeConvectionRegime(0.8));
  EXPECT_TRUE(ct.setFreeConvectionAirFlowRateSizingFactor(0.9));
  // Autocalculate
  // ct.autocalculateFreeConvectionRegimeUFactorTimesAreaValue();
  EXPECT_TRUE(ct.setUFactorTimesAreaValueatFreeConvectionAirFlowRate(272727.273));
  EXPECT_TRUE(ct.setFreeConvectionUFactorTimesAreaValueSizingFactor(0.917));
  EXPECT_TRUE(ct.setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate"));
  EXPECT_TRUE(ct.setHeatRejectionCapacityAndNominalCapacitySizingRatio(1.18));
  EXPECT_TRUE(ct.setNominalCapacity(14000));
  // ct.autocalculateFreeConvectionCapacity();
  EXPECT_TRUE(ct.setFreeConvectionCapacity(1.5));
  EXPECT_TRUE(ct.setFreeConvectionNominalCapacitySizingFactor(0.941));
  EXPECT_TRUE(ct.setDesignInletAirDryBulbTemperature(35.01));
  EXPECT_TRUE(ct.setDesignInletAirWetBulbTemperature(25.55));
  // ct.autosizeDesignApproachTemperature();
  EXPECT_TRUE(ct.setDesignApproachTemperature(3.89));
  // ct.autosizeDesignRangeTemperature();
  EXPECT_TRUE(ct.setDesignRangeTemperature(5.55));
  EXPECT_TRUE(ct.setBasinHeaterCapacity(2.1));
  EXPECT_TRUE(ct.setBasinHeaterSetpointTemperature(4.2));
  ScheduleConstant basinHeaterOperatingSchedule(m);
  basinHeaterOperatingSchedule.setName("basinHeaterOperatingSchedule");
  EXPECT_TRUE(ct.setBasinHeaterOperatingSchedule(basinHeaterOperatingSchedule));
  EXPECT_TRUE(ct.setEvaporationLossMode("LossFactor"));
  EXPECT_TRUE(ct.setEvaporationLossFactor(2.5));
  EXPECT_TRUE(ct.setDriftLossPercent(2.6));
  EXPECT_TRUE(ct.setBlowdownCalculationMode("ConcentrationRatio"));
  EXPECT_TRUE(ct.setBlowdownConcentrationRatio(4.8));

  ScheduleConstant blowdownMakeupWaterUsageSchedule(m);
  blowdownMakeupWaterUsageSchedule.setName("blowdownMakeupWaterUsageSchedule");
  EXPECT_TRUE(ct.setBlowdownMakeupWaterUsageSchedule(blowdownMakeupWaterUsageSchedule));
  EXPECT_TRUE(ct.setCapacityControl("FanCycling"));
  EXPECT_TRUE(ct.setNumberofCells(34));
  EXPECT_TRUE(ct.setCellControl("MinimalCell"));
  EXPECT_TRUE(ct.setCellMinimumWaterFlowRateFraction(0.12));
  EXPECT_TRUE(ct.setCellMaximumWaterFlowRateFraction(1.62));
  EXPECT_TRUE(ct.setSizingFactor(3.7));
  EXPECT_TRUE(ct.setEndUseSubcategory("Cooling Towers"));

  PlantLoop p(m);
  p.addSupplyBranchForComponent(ct);
  ct.inletModelObject().get().setName("CT Inlet Node");
  ct.outletModelObject().get().setName("CT Outlet Node");

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::CoolingTower_SingleSpeed);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ("CT Inlet Node", idfObject.getString(CoolingTower_SingleSpeedFields::WaterInletNodeName).get());
  EXPECT_EQ("CT Outlet Node", idfObject.getString(CoolingTower_SingleSpeedFields::WaterOutletNodeName).get());
  EXPECT_EQ(0.4, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignWaterFlowRate).get());
  EXPECT_EQ(0.5, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignAirFlowRate).get());
  EXPECT_EQ(6000.0, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignFanPower).get());
  EXPECT_EQ(1837500.0, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignUFactorTimesAreaValue).get());
  EXPECT_EQ(0.8, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionRegimeAirFlowRate).get());
  EXPECT_EQ(0.9, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor).get());
  EXPECT_EQ(272727.273, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue).get());
  EXPECT_EQ(0.917, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor).get());
  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", idfObject.getString(CoolingTower_SingleSpeedFields::PerformanceInputMethod).get());
  EXPECT_EQ(1.18, idfObject.getDouble(CoolingTower_SingleSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio).get());
  EXPECT_EQ(14000.0, idfObject.getDouble(CoolingTower_SingleSpeedFields::NominalCapacity).get());
  EXPECT_EQ(1.5, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionCapacity).get());
  EXPECT_EQ(0.941, idfObject.getDouble(CoolingTower_SingleSpeedFields::FreeConvectionNominalCapacitySizingFactor).get());
  EXPECT_EQ(35.01, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignInletAirDryBulbTemperature).get());
  EXPECT_EQ(25.55, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignInletAirWetBulbTemperature).get());
  EXPECT_EQ(3.89, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignApproachTemperature).get());
  EXPECT_EQ(5.55, idfObject.getDouble(CoolingTower_SingleSpeedFields::DesignRangeTemperature).get());
  EXPECT_EQ(2.1, idfObject.getDouble(CoolingTower_SingleSpeedFields::BasinHeaterCapacity).get());
  EXPECT_EQ(4.2, idfObject.getDouble(CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("basinHeaterOperatingSchedule", idfObject.getString(CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName).get());
  EXPECT_EQ("LossFactor", idfObject.getString(CoolingTower_SingleSpeedFields::EvaporationLossMode).get());
  EXPECT_EQ(2.5, idfObject.getDouble(CoolingTower_SingleSpeedFields::EvaporationLossFactor).get());
  EXPECT_EQ(2.6, idfObject.getDouble(CoolingTower_SingleSpeedFields::DriftLossPercent).get());
  EXPECT_EQ("ConcentrationRatio", idfObject.getString(CoolingTower_SingleSpeedFields::BlowdownCalculationMode).get());
  EXPECT_EQ(4.8, idfObject.getDouble(CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio).get());
  EXPECT_EQ("blowdownMakeupWaterUsageSchedule", idfObject.getString(CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName).get());
  EXPECT_TRUE(idfObject.isEmpty(CoolingTower_SingleSpeedFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idfObject.isEmpty(CoolingTower_SingleSpeedFields::OutdoorAirInletNodeName));
  EXPECT_EQ("FanCycling", idfObject.getString(CoolingTower_SingleSpeedFields::CapacityControl).get());
  EXPECT_EQ(34, idfObject.getInt(CoolingTower_SingleSpeedFields::NumberofCells).get());
  EXPECT_EQ("MinimalCell", idfObject.getString(CoolingTower_SingleSpeedFields::CellControl).get());
  EXPECT_EQ(0.12, idfObject.getDouble(CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction).get());
  EXPECT_EQ(1.62, idfObject.getDouble(CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction).get());
  EXPECT_EQ(3.7, idfObject.getDouble(CoolingTower_SingleSpeedFields::SizingFactor).get());
  EXPECT_EQ("Cooling Towers", idfObject.getString(CoolingTower_SingleSpeedFields::EndUseSubcategory).get());
}
