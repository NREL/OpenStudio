/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR.hpp"
#include "../../model/LoadingIndex.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "../../model/FanSystemModel.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR) {
  Model model;
  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR vrf(model);

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(vrf.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(vrf.setRefrigerantType("R11"));
  EXPECT_TRUE(vrf.setRatedEvaporativeCapacity(1));
  EXPECT_TRUE(vrf.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(2));
  EXPECT_TRUE(vrf.setMinimumOutdoorAirTemperatureinCoolingOnlyMode(3));
  EXPECT_TRUE(vrf.setMaximumOutdoorAirTemperatureinCoolingOnlyMode(4));
  EXPECT_TRUE(vrf.setMinimumOutdoorAirTemperatureinHeatingOnlyMode(5));
  EXPECT_TRUE(vrf.setMaximumOutdoorAirTemperatureinHeatingOnlyMode(6));
  EXPECT_TRUE(vrf.setMinimumOutdoorTemperatureinHeatRecoveryMode(7));
  EXPECT_TRUE(vrf.setMaximumOutdoorTemperatureinHeatRecoveryMode(8));
  EXPECT_TRUE(vrf.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("ConstantTemp"));
  EXPECT_TRUE(vrf.setReferenceEvaporatingTemperatureforIndoorUnit(9));
  EXPECT_TRUE(vrf.setReferenceCondensingTemperatureforIndoorUnit(10));
  EXPECT_TRUE(vrf.setVariableEvaporatingTemperatureMinimumforIndoorUnit(11));
  EXPECT_TRUE(vrf.setVariableEvaporatingTemperatureMaximumforIndoorUnit(12));
  EXPECT_TRUE(vrf.setVariableCondensingTemperatureMinimumforIndoorUnit(13));
  EXPECT_TRUE(vrf.setVariableCondensingTemperatureMaximumforIndoorUnit(14));
  EXPECT_TRUE(vrf.setOutdoorUnitEvaporatorReferenceSuperheating(15));
  EXPECT_TRUE(vrf.setOutdoorUnitCondenserReferenceSubcooling(16));
  EXPECT_TRUE(vrf.setOutdoorUnitEvaporatorRatedBypassFactor(17));
  EXPECT_TRUE(vrf.setOutdoorUnitCondenserRatedBypassFactor(18));
  EXPECT_TRUE(vrf.setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(19));
  EXPECT_TRUE(vrf.setOutdoorUnitHeatExchangerCapacityRatio(20));
  EXPECT_TRUE(vrf.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(21));
  EXPECT_TRUE(vrf.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(22));
  CurveCubic curve1(model);
  EXPECT_TRUE(vrf.setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve1));
  CurveCubic curve2(model);
  EXPECT_TRUE(vrf.setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve2));
  EXPECT_TRUE(vrf.setDiameterofMainPipeforSuctionGas(23));
  EXPECT_TRUE(vrf.setDiameterofMainPipeforDischargeGas(24));
  EXPECT_TRUE(vrf.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(25));
  EXPECT_TRUE(vrf.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(26));
  EXPECT_TRUE(vrf.setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(27));
  EXPECT_TRUE(vrf.setMainPipeInsulationThickness(28));
  EXPECT_TRUE(vrf.setMainPipeInsulationThermalConductivity(29));
  EXPECT_TRUE(vrf.setCrankcaseHeaterPowerperCompressor(30));
  EXPECT_TRUE(vrf.setNumberofCompressors(31));
  EXPECT_TRUE(vrf.setRatioofCompressorSizetoTotalCompressorCapacity(32));
  EXPECT_TRUE(vrf.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(33));
  EXPECT_TRUE(vrf.setDefrostStrategy("ReverseCycle"));
  EXPECT_TRUE(vrf.setDefrostControl("OnDemand"));
  CurveBiquadratic curve3(model);
  EXPECT_TRUE(vrf.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve3));
  EXPECT_TRUE(vrf.setDefrostTimePeriodFraction(34));
  EXPECT_TRUE(vrf.setResistiveDefrostHeaterCapacity(35));
  EXPECT_TRUE(vrf.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(36));
  EXPECT_TRUE(vrf.setInitialHeatRecoveryCoolingCapacityFraction(37));
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingCapacityTimeConstant(38));
  EXPECT_TRUE(vrf.setInitialHeatRecoveryCoolingEnergyFraction(39));
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingEnergyTimeConstant(40));
  EXPECT_TRUE(vrf.setInitialHeatRecoveryHeatingCapacityFraction(41));
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingCapacityTimeConstant(42));
  EXPECT_TRUE(vrf.setInitialHeatRecoveryHeatingEnergyFraction(43));
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingEnergyTimeConstant(44));
  EXPECT_TRUE(vrf.setCompressorMaximumDeltaPressure(45));
  EXPECT_TRUE(vrf.setCompressorInverterEfficiency(0.5));
  EXPECT_TRUE(vrf.setCompressorEvaporativeCapacityCorrectionFactor(47));

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve1(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve1(model);

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve2(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve2(model);

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve3(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve3(model);

  vrf.removeAllLoadingIndexes();
  LoadingIndex loadingIndex1(model, 1, evaporativeCapacityMultiplierFunctionofTemperatureCurve1,
                             compressorPowerMultiplierFunctionofTemperatureCurve1);
  EXPECT_TRUE(vrf.addLoadingIndex(loadingIndex1));
  LoadingIndex loadingIndex2(model, 2, evaporativeCapacityMultiplierFunctionofTemperatureCurve2,
                             compressorPowerMultiplierFunctionofTemperatureCurve2);
  EXPECT_TRUE(vrf.addLoadingIndex(loadingIndex2));
  LoadingIndex loadingIndex3(model, 3, evaporativeCapacityMultiplierFunctionofTemperatureCurve3,
                             compressorPowerMultiplierFunctionofTemperatureCurve3);
  EXPECT_TRUE(vrf.addLoadingIndex(loadingIndex3));

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coolingCoil(model);
  EXPECT_TRUE(coolingCoil.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(coolingCoil.setRatedTotalCoolingCapacity(10));
  EXPECT_TRUE(coolingCoil.setRatedSensibleHeatRatio(20));
  EXPECT_TRUE(coolingCoil.setIndoorUnitReferenceSuperheating(30));
  CurveCubic curve4(model);
  EXPECT_TRUE(coolingCoil.setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve4));

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl heatingCoil(model);
  EXPECT_TRUE(heatingCoil.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(heatingCoil.setRatedTotalHeatingCapacity(40));
  EXPECT_TRUE(heatingCoil.setIndoorUnitReferenceSubcooling(50));
  CurveCubic curve5(model);
  EXPECT_TRUE(heatingCoil.setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve5));

  FanSystemModel fan(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model, coolingCoil, heatingCoil, fan);

  EXPECT_TRUE(vrf.addTerminal(term));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(13u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Fan_SystemModel).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  IdfObject idf_vrf = workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR)[0];

  EXPECT_EQ(vrf.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::Name, false).get());
  EXPECT_EQ(scheduleConstant.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(vrf.nameString() + " Terminal List",
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ZoneTerminalUnitListName, false).get());
  EXPECT_EQ("R11", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantType, false).get());
  EXPECT_EQ(1, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, false).get());
  EXPECT_EQ(
    2, idf_vrf
         .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity,
                    false)
         .get());
  EXPECT_EQ(
    3,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinCoolingOnlyMode, false)
      .get());
  EXPECT_EQ(
    4,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinCoolingOnlyMode, false)
      .get());
  EXPECT_EQ(
    5,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinHeatingOnlyMode, false)
      .get());
  EXPECT_EQ(
    6,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinHeatingOnlyMode, false)
      .get());
  EXPECT_EQ(
    7, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorTemperatureinHeatRecoveryMode, false)
         .get());
  EXPECT_EQ(
    8, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorTemperatureinHeatRecoveryMode, false)
         .get());
  EXPECT_EQ(
    "ConstantTemp",
    idf_vrf
      .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    9, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceEvaporatingTemperatureforIndoorUnit, false)
         .get());
  EXPECT_EQ(
    10, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceCondensingTemperatureforIndoorUnit, false)
          .get());
  EXPECT_EQ(
    11,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    12,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, false)
      .get());
  EXPECT_EQ(13, idf_vrf
                  .getDouble(
                    AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMinimumforIndoorUnit, false)
                  .get());
  EXPECT_EQ(14, idf_vrf
                  .getDouble(
                    AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMaximumforIndoorUnit, false)
                  .get());
  EXPECT_EQ(
    15, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorReferenceSuperheating, false)
          .get());
  EXPECT_EQ(
    16,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserReferenceSubcooling, false).get());
  EXPECT_EQ(
    17,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorRatedBypassFactor, false).get());
  EXPECT_EQ(
    18,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserRatedBypassFactor, false).get());
  EXPECT_EQ(19, idf_vrf
                  .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                               DifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode,
                             false)
                  .get());
  EXPECT_EQ(
    20,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitHeatExchangerCapacityRatio, false).get());
  EXPECT_EQ(
    21,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, false)
      .get());
  EXPECT_EQ(
    22, idf_vrf
          .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity,
                     false)
          .get());
  EXPECT_EQ(
    curve1.nameString(),
    idf_vrf
      .getString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
        false)
      .get());
  EXPECT_EQ(
    curve2.nameString(),
    idf_vrf
      .getString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName, false)
      .get());
  EXPECT_EQ(23,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforSuctionGas, false).get());
  EXPECT_EQ(
    24, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforDischargeGas, false).get());
  EXPECT_EQ(
    25,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
                 false)
      .get());
  EXPECT_EQ(
    26,
    idf_vrf
      .getDouble(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        false)
      .get());
  EXPECT_EQ(
    27, idf_vrf
          .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, false)
          .get());
  EXPECT_EQ(28, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThickness, false).get());
  EXPECT_EQ(
    29,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThermalConductivity, false).get());
  EXPECT_EQ(
    30, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CrankcaseHeaterPowerperCompressor, false).get());
  EXPECT_EQ(31, idf_vrf.getInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressors, false).get());
  EXPECT_EQ(
    32,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatioofCompressorSizetoTotalCompressorCapacity, false)
      .get());
  EXPECT_EQ(
    33,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, false)
      .get());
  EXPECT_EQ("ReverseCycle", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostStrategy, false).get());
  EXPECT_EQ("OnDemand", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostControl, false).get());
  EXPECT_EQ(
    curve3.nameString(),
    idf_vrf
      .getString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, false)
      .get());
  EXPECT_EQ(34, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostTimePeriodFraction, false).get());
  EXPECT_EQ(35,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, false).get());
  EXPECT_EQ(
    36,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, false)
      .get());
  EXPECT_EQ(
    37, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingCapacityFraction, false)
          .get());
  EXPECT_EQ(
    38,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingCapacityTimeConstant, false).get());
  EXPECT_EQ(
    39, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingEnergyFraction, false)
          .get());
  EXPECT_EQ(
    40,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingEnergyTimeConstant, false).get());
  EXPECT_EQ(
    41, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingCapacityFraction, false)
          .get());
  EXPECT_EQ(
    42,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingCapacityTimeConstant, false).get());
  EXPECT_EQ(
    43, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingEnergyFraction, false)
          .get());
  EXPECT_EQ(
    44,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingEnergyTimeConstant, false).get());
  EXPECT_EQ(45,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressormaximumdeltaPressure, false).get());
  EXPECT_EQ(0.5,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorInverterEfficiency, false).get());
  EXPECT_EQ(
    47,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorEvaporativeCapacityCorrectionFactor, false)
      .get());
  EXPECT_EQ(
    3, idf_vrf.getInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressorLoadingIndexEntries, false).get());

  ASSERT_EQ(3u, idf_vrf.numExtensibleGroups());

  auto egs = idf_vrf.extensibleGroups();

  IdfExtensibleGroup eg1 = egs[0];
  EXPECT_EQ(1, eg1.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve1.nameString(),
            eg1
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve1.nameString(),
            eg1
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
              .get());

  IdfExtensibleGroup eg2 = egs[1];
  EXPECT_EQ(2, eg2.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve2.nameString(),
            eg2
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve2.nameString(),
            eg2
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
              .get());

  IdfExtensibleGroup eg3 = egs[2];
  EXPECT_EQ(3, eg3.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve3.nameString(),
            eg3
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve3.nameString(),
            eg3
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRExtensibleFields::
                           LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
              .get());

  IdfObject idf_term = workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow)[0];

  EXPECT_EQ("DrawThrough", idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, false).get());
  EXPECT_EQ("Fan:SystemModel", idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectType, false).get());
  EXPECT_EQ(fan.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName, false).get());
  EXPECT_EQ("Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl",
            idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectType, false).get());
  EXPECT_EQ(coolingCoil.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectName, false).get());
  EXPECT_EQ("Coil:Heating:DX:VariableRefrigerantFlow:FluidTemperatureControl",
            idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectType, false).get());
  EXPECT_EQ(heatingCoil.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectName, false).get());

  IdfObject idf_cool = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl)[0];
  EXPECT_EQ(scheduleConstant.nameString(),
            idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("Zone HVAC Terminal Unit Variable Refrigerant Flow 1 Mixer Outlet Node",
            idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, false).get());
  EXPECT_EQ("Zone HVAC Terminal Unit Variable Refrigerant Flow 1 Cooling Coil Outlet Node",
            idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, false).get());
  EXPECT_EQ(10, idf_cool.getDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, false).get());
  EXPECT_EQ(20, idf_cool.getDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, false).get());
  EXPECT_EQ(30,
            idf_cool.getDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating, false).get());
  EXPECT_EQ(
    curve4.nameString(),
    idf_cool
      .getString(
        Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName, false)
      .get());

  IdfObject idf_heat = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl)[0];
  EXPECT_EQ(scheduleConstant.nameString(),
            idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule, false).get());
  EXPECT_EQ("Zone HVAC Terminal Unit Variable Refrigerant Flow 1 Cooling Coil Outlet Node",
            idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, false).get());
  EXPECT_EQ("Zone HVAC Terminal Unit Variable Refrigerant Flow 1 Heating Coil Outlet Node",
            idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, false).get());
  EXPECT_EQ(40, idf_heat.getDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, false).get());
  EXPECT_EQ(50,
            idf_heat.getDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling, false).get());
  EXPECT_EQ(
    curve5.nameString(),
    idf_heat
      .getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
                 false)
      .get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_NoTerminals) {
  Model model;
  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR vrf(model);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(6u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_SystemModel).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  ASSERT_EQ(1, ft.warnings().size());
  EXPECT_NE(std::string::npos, ft.warnings().front().logMessage().find("will not be translated as it has no terminals"));
  EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR).size());

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model, true);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR).size());
}
