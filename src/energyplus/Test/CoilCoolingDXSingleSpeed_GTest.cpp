/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Model.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXSingleSpeed_MinOATCompressor) {
  Model m;

  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");
  CurveBiquadratic totalCoolingCapacityFunctionOfTemperatureCurve(m);
  totalCoolingCapacityFunctionOfTemperatureCurve.setName("totalCoolingCapacityFunctionOfTemperatureCurve");
  CurveQuadratic totalCoolingCapacityFunctionOfFlowFractionCurve(m);
  totalCoolingCapacityFunctionOfFlowFractionCurve.setName("totalCoolingCapacityFunctionOfFlowFractionCurve");
  CurveBiquadratic energyInputRatioFunctionOfTemperatureCurve(m);
  energyInputRatioFunctionOfTemperatureCurve.setName("energyInputRatioFunctionOfTemperatureCurve");
  CurveQuadratic energyInputRatioFunctionOfFlowFractionCurve(m);
  energyInputRatioFunctionOfFlowFractionCurve.setName("energyInputRatioFunctionOfFlowFractionCurve");
  CurveQuadratic partLoadFractionCorrelationCurve(m);
  partLoadFractionCorrelationCurve.setName("partLoadFractionCorrelationCurve");

  CoilCoolingDXSingleSpeed coil(m, availSch, totalCoolingCapacityFunctionOfTemperatureCurve, totalCoolingCapacityFunctionOfFlowFractionCurve,
                                energyInputRatioFunctionOfTemperatureCurve, energyInputRatioFunctionOfFlowFractionCurve,
                                partLoadFractionCorrelationCurve);

  AirLoopHVAC a(m);
  auto supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));

  a.supplyInletNode().setName("Supply Inlet Node");
  a.supplyOutletNode().setName("Supply Outlet Node");

  EXPECT_TRUE(coil.setName("Coil Cooling DX Single Speed"));
  EXPECT_EQ(availSch, coil.availabilitySchedule());
  EXPECT_TRUE(coil.setRatedTotalCoolingCapacity(2000.0));
  EXPECT_TRUE(coil.setRatedSensibleHeatRatio(0.8));
  EXPECT_TRUE(coil.setRatedCOP(4.0));
  EXPECT_TRUE(coil.setRatedAirFlowRate(0.9));
  EXPECT_TRUE(coil.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(817.0));
  EXPECT_TRUE(coil.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(823.0));
  EXPECT_EQ(totalCoolingCapacityFunctionOfTemperatureCurve, coil.totalCoolingCapacityFunctionOfTemperatureCurve());
  EXPECT_EQ(totalCoolingCapacityFunctionOfFlowFractionCurve, coil.totalCoolingCapacityFunctionOfFlowFractionCurve());
  EXPECT_EQ(energyInputRatioFunctionOfTemperatureCurve, coil.energyInputRatioFunctionOfTemperatureCurve());
  EXPECT_EQ(energyInputRatioFunctionOfFlowFractionCurve, coil.energyInputRatioFunctionOfFlowFractionCurve());
  EXPECT_EQ(partLoadFractionCorrelationCurve, coil.partLoadFractionCorrelationCurve());
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);
  EXPECT_TRUE(coil.setNominalTimeForCondensateRemovalToBegin(6.0));
  EXPECT_TRUE(coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(0.2));
  EXPECT_TRUE(coil.setMaximumCyclingRate(3.2));
  EXPECT_TRUE(coil.setLatentCapacityTimeConstant(62.0));
  EXPECT_TRUE(coil.setCondenserType("EvaporativelyCooled"));
  EXPECT_TRUE(coil.setEvaporativeCondenserEffectiveness(0.85));
  EXPECT_TRUE(coil.setEvaporativeCondenserAirFlowRate(1.2));
  EXPECT_TRUE(coil.setEvaporativeCondenserPumpRatedPowerConsumption(51.0));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));

  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(9.0));
  EXPECT_TRUE(coil.setBasinHeaterCapacity(90.0));
  EXPECT_TRUE(coil.setBasinHeaterSetpointTemperature(5.0));
  ScheduleConstant basinSch(m);
  basinSch.setName("Coil Basin Sch");
  coil.setBasinHeaterOperatingSchedule(basinSch);

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  // Note JM 2020-05-13: I find it weird that the coil gets translated when it's not even used by anything, but trying to fix #3976 only here
  WorkspaceObjectVector idf_coils = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed);
  ASSERT_EQ(1u, idf_coils.size());
  const WorkspaceObject& idf_coil = idf_coils[0];

  EXPECT_EQ("Coil Cooling DX Single Speed", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName).get());
  EXPECT_EQ(2000.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity).get());
  EXPECT_EQ(0.8, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio).get());
  EXPECT_EQ(4.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedCoolingCOP).get());
  EXPECT_EQ(0.9, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate).get());
  EXPECT_EQ(817.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017).get());
  EXPECT_EQ(823.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023).get());
  EXPECT_EQ("Supply Inlet Node", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get());
  EXPECT_EQ("Supply Outlet Node", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get());
  EXPECT_EQ("totalCoolingCapacityFunctionOfTemperatureCurve",
            idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("totalCoolingCapacityFunctionOfFlowFractionCurve",
            idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("energyInputRatioFunctionOfTemperatureCurve",
            idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ("energyInputRatioFunctionOfFlowFractionCurve",
            idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("partLoadFractionCorrelationCurve", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(-7.5, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(6.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin).get());
  EXPECT_EQ(0.2, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity).get());
  EXPECT_EQ(3.2, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate).get());
  EXPECT_EQ(62.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName));
  EXPECT_EQ("EvaporativelyCooled", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::CondenserType).get());
  EXPECT_EQ(0.85, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness).get());
  EXPECT_EQ(1.2, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate).get());
  EXPECT_EQ(51.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_SingleSpeedFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_SingleSpeedFields::CondensateCollectionWaterStorageTankName));
  EXPECT_EQ(90.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity).get());
  EXPECT_EQ(5.0, idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("Coil Basin Sch", idf_coil.getString(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName).get());
}
