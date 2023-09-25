/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/CoilPerformanceDXCooling.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXTwoStageWithHumidityControlMode) {
  Model m;

  CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

  // Need to be used to be translated
  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  coil.addToNode(supplyOutletNode);
  a.supplyInletNode().setName("Supply Inlet Node");
  a.supplyOutletNode().setName("Supply Outlet Node");

  EXPECT_TRUE(coil.setName("CoilCoolingDXTwoStageWithHumidityControlMode"));
  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");
  EXPECT_TRUE(coil.setAvailabilitySchedule(availSch));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));
  EXPECT_TRUE(coil.setNumberofCapacityStages(2));
  EXPECT_TRUE(coil.setNumberofEnhancedDehumidificationModes(1));

  ASSERT_TRUE(coil.normalModeStage1CoilPerformance());
  coil.normalModeStage1CoilPerformance()->setName("normalModeStage1CoilPerformance");
  ASSERT_TRUE(coil.normalModeStage1Plus2CoilPerformance());
  coil.normalModeStage1Plus2CoilPerformance()->setName("normalModeStage1Plus2CoilPerformance");
  ASSERT_TRUE(coil.dehumidificationMode1Stage1CoilPerformance());
  coil.dehumidificationMode1Stage1CoilPerformance()->setName("dehumidificationMode1Stage1CoilPerformance");
  ASSERT_TRUE(coil.dehumidificationMode1Stage1Plus2CoilPerformance());
  coil.dehumidificationMode1Stage1Plus2CoilPerformance()->setName("dehumidificationMode1Stage1Plus2CoilPerformance");

  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  EXPECT_TRUE(coil.setBasinHeaterCapacity(90.0));
  EXPECT_TRUE(coil.setBasinHeaterSetpointTemperature(5.0));
  ScheduleConstant basinSch(m);
  basinSch.setName("Coil Basin Sch");
  coil.setBasinHeaterOperatingSchedule(basinSch);

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_coils = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode);
  ASSERT_EQ(1u, idf_coils.size());
  const WorkspaceObject& idf_coil = idf_coils[0];

  EXPECT_EQ("CoilCoolingDXTwoStageWithHumidityControlMode", idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilityScheduleName).get());
  EXPECT_EQ("Supply Inlet Node", idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName).get());
  EXPECT_EQ("Supply Outlet Node", idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT",
            idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    9.0,
    idf_coil.getDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofCapacityStages).get());
  EXPECT_EQ(1, idf_coil.getInt(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofEnhancedDehumidificationModes).get());
  EXPECT_EQ("normalModeStage1CoilPerformance",
            idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformanceName).get());
  EXPECT_EQ("normalModeStage1Plus2CoilPerformance",
            idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1_PLUS_2CoilPerformanceName).get());
  EXPECT_EQ("dehumidificationMode1Stage1CoilPerformance",
            idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformanceName).get());
  EXPECT_EQ("dehumidificationMode1Stage1Plus2CoilPerformance",
            idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1_PLUS_2CoilPerformanceName).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CondensateCollectionWaterStorageTankName));
  EXPECT_EQ(-7.5,
            idf_coil.getDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(90.0, idf_coil.getDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterCapacity).get());
  EXPECT_EQ(5.0, idf_coil.getDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("Coil Basin Sch", idf_coil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingScheduleName).get());
}
