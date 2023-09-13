/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXTwoSpeed) {
  Model m;

  CoilCoolingDXTwoSpeed coil(m);
  coil.setName("CoilCoolingDXTwoSpeed");

  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");
  EXPECT_TRUE(coil.setAvailabilitySchedule(availSch));

  EXPECT_TRUE(coil.setRatedHighSpeedTotalCoolingCapacity(8000.0));
  EXPECT_TRUE(coil.setRatedHighSpeedSensibleHeatRatio(0.79));
  EXPECT_TRUE(coil.setRatedHighSpeedCOP(4.0));
  EXPECT_TRUE(coil.setRatedHighSpeedAirFlowRate(1.0));
  EXPECT_TRUE(coil.setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017(917.0));
  EXPECT_TRUE(coil.setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023(923.0));
  EXPECT_TRUE(coil.setUnitInternalStaticAirPressure(769.3));

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  coil.addToNode(supplyOutletNode);
  a.supplyInletNode().setName("Supply Inlet Node");
  a.supplyOutletNode().setName("Supply Outlet Node");

  coil.totalCoolingCapacityFunctionOfTemperatureCurve().setName("CoolCapFT");
  coil.totalCoolingCapacityFunctionOfFlowFractionCurve().setName("TotCoolCapfFlow");
  coil.energyInputRatioFunctionOfTemperatureCurve().setName("EIRFT");
  coil.energyInputRatioFunctionOfFlowFractionCurve().setName("EIRfFlow");
  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  EXPECT_TRUE(coil.setRatedLowSpeedTotalCoolingCapacity(4000.0));
  EXPECT_TRUE(coil.setRatedLowSpeedSensibleHeatRatio(0.75));
  EXPECT_TRUE(coil.setRatedLowSpeedCOP(4.5));
  EXPECT_TRUE(coil.setRatedLowSpeedAirFlowRate(0.5));
  EXPECT_TRUE(coil.setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017(817.0));
  EXPECT_TRUE(coil.setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023(823.0));

  coil.lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve().setName("LowSpeedCoolCapFT");
  coil.lowSpeedEnergyInputRatioFunctionOfTemperatureCurve().setName("LowSpeedEIRfFlow");

  EXPECT_TRUE(coil.setCondenserType("EvaporativelyCooled"));
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5));
  EXPECT_TRUE(coil.setHighSpeedEvaporativeCondenserAirFlowRate(1.2));
  EXPECT_TRUE(coil.setHighSpeedEvaporativeCondenserEffectiveness(0.65));
  EXPECT_TRUE(coil.setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(400.0));
  EXPECT_TRUE(coil.setLowSpeedEvaporativeCondenserAirFlowRate(0.7));
  EXPECT_TRUE(coil.setLowSpeedEvaporativeCondenserEffectiveness(0.67));
  EXPECT_TRUE(coil.setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(200.0));

  EXPECT_TRUE(coil.setBasinHeaterCapacity(560.0));
  EXPECT_TRUE(coil.setBasinHeaterSetpointTemperature(4.0));
  ScheduleConstant basinSch(m);
  basinSch.setName("Basin Heater Sch");
  EXPECT_TRUE(coil.setBasinHeaterOperatingSchedule(basinSch));

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  // Note JM 2020-05-13: I find it weird that the coil gets translated when it's not even used by anything, but trying to fix #3976 only here
  const auto idf_coils = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_TwoSpeed);
  ASSERT_EQ(1u, idf_coils.size());
  const WorkspaceObject& idf_coil = idf_coils.front();

  EXPECT_EQ("CoilCoolingDXTwoSpeed", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName).get());
  EXPECT_EQ(8000.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedTotalCoolingCapacity).get());
  EXPECT_EQ(0.79, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedSensibleHeatRatio).get());
  EXPECT_EQ(4.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedCoolingCOP).get());
  EXPECT_EQ(1.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedAirFlowRate).get());
  EXPECT_EQ(917.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedEvaporatorFanPowerPerVolumeFlowRate2017).get());
  EXPECT_EQ(923.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedEvaporatorFanPowerPerVolumeFlowRate2023).get());
  EXPECT_EQ(769.3, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::UnitInternalStaticAirPressure).get());
  EXPECT_EQ("Supply Inlet Node", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName).get());
  EXPECT_EQ("Supply Outlet Node", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName).get());
  EXPECT_EQ("CoolCapFT", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("TotCoolCapfFlow", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("EIRFT", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ("EIRfFlow", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::PartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedTotalCoolingCapacity).get());
  EXPECT_EQ(0.75, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedSensibleHeatRatio).get());
  EXPECT_EQ(4.5, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedCoolingCOP).get());
  EXPECT_EQ(0.5, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedAirFlowRate).get());
  EXPECT_EQ(817.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedEvaporatorFanPowerPerVolumeFlowRate2017).get());
  EXPECT_EQ(823.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedEvaporatorFanPowerPerVolumeFlowRate2023).get());
  EXPECT_EQ("LowSpeedCoolCapFT",
            idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedTotalCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("LowSpeedEIRfFlow", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName));
  EXPECT_EQ("EvaporativelyCooled", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::CondenserType).get());
  EXPECT_EQ(-7.5, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(0.65, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness).get());
  EXPECT_EQ(1.2, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate).get());
  EXPECT_EQ(400.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_EQ(0.67, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness).get());
  EXPECT_EQ(0.7, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate).get());
  EXPECT_EQ(200.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_TwoSpeedFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_TwoSpeedFields::CondensateCollectionWaterStorageTankName));
  EXPECT_EQ(560.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity).get());
  EXPECT_EQ(4.0, idf_coil.getDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("Basin Heater Sch", idf_coil.getString(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName).get());
}
