/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
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
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlowFluidTemperatureControl) {
  Model model;
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(vrf.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(vrf.setRefrigerantType("R11"));
  EXPECT_TRUE(vrf.setRatedEvaporativeCapacity(1));
  EXPECT_TRUE(vrf.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(2));
  EXPECT_TRUE(vrf.setMinimumOutdoorAirTemperatureinCoolingMode(3));
  EXPECT_TRUE(vrf.setMaximumOutdoorAirTemperatureinCoolingMode(4));
  EXPECT_TRUE(vrf.setMinimumOutdoorAirTemperatureinHeatingMode(5));
  EXPECT_TRUE(vrf.setMaximumOutdoorAirTemperatureinHeatingMode(6));
  EXPECT_TRUE(vrf.setReferenceOutdoorUnitSuperheating(7));
  EXPECT_TRUE(vrf.setReferenceOutdoorUnitSubcooling(8));
  EXPECT_TRUE(vrf.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("ConstantTemp"));
  EXPECT_TRUE(vrf.setReferenceEvaporatingTemperatureforIndoorUnit(9));
  EXPECT_TRUE(vrf.setReferenceCondensingTemperatureforIndoorUnit(10));
  EXPECT_TRUE(vrf.setVariableEvaporatingTemperatureMinimumforIndoorUnit(11));
  EXPECT_TRUE(vrf.setVariableEvaporatingTemperatureMaximumforIndoorUnit(12));
  EXPECT_TRUE(vrf.setVariableCondensingTemperatureMinimumforIndoorUnit(13));
  EXPECT_TRUE(vrf.setVariableCondensingTemperatureMaximumforIndoorUnit(14));
  EXPECT_TRUE(vrf.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(15));
  EXPECT_TRUE(vrf.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(16));
  CurveCubic curve1(model);
  EXPECT_TRUE(vrf.setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve1));
  CurveCubic curve2(model);
  EXPECT_TRUE(vrf.setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve2));
  EXPECT_TRUE(vrf.setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(17));
  EXPECT_TRUE(vrf.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(18));
  EXPECT_TRUE(vrf.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(19));
  EXPECT_TRUE(vrf.setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(20));
  EXPECT_TRUE(vrf.setMainPipeInsulationThickness(21));
  EXPECT_TRUE(vrf.setMainPipeInsulationThermalConductivity(22));
  EXPECT_TRUE(vrf.setCrankcaseHeaterPowerperCompressor(23));
  EXPECT_TRUE(vrf.setNumberofCompressors(24));
  EXPECT_TRUE(vrf.setRatioofCompressorSizetoTotalCompressorCapacity(25));
  EXPECT_TRUE(vrf.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(26));
  EXPECT_TRUE(vrf.setDefrostStrategy("ReverseCycle"));
  EXPECT_TRUE(vrf.setDefrostControl("OnDemand"));
  CurveBiquadratic curve3(model);
  EXPECT_TRUE(vrf.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve3));
  EXPECT_TRUE(vrf.setDefrostTimePeriodFraction(27));
  EXPECT_TRUE(vrf.setResistiveDefrostHeaterCapacity(28));
  EXPECT_TRUE(vrf.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(29));
  EXPECT_TRUE(vrf.setCompressorMaximumDeltaPressure(30));

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

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(13u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Fan_SystemModel).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  IdfObject idf_vrf = workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl)[0];

  EXPECT_EQ(vrf.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::Name, false).get());
  EXPECT_EQ(scheduleConstant.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(vrf.nameString() + " Terminal List",
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitListName, false).get());
  EXPECT_EQ("R11", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType, false).get());
  EXPECT_EQ(1, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, false).get());
  EXPECT_EQ(2, idf_vrf
                 .getDouble(
                   AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity, false)
                 .get());
  EXPECT_EQ(
    3,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinCoolingMode, false).get());
  EXPECT_EQ(
    4,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinCoolingMode, false).get());
  EXPECT_EQ(
    5,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinHeatingMode, false).get());
  EXPECT_EQ(
    6,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinHeatingMode, false).get());
  EXPECT_EQ(7,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSuperheating, false).get());
  EXPECT_EQ(8, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSubcooling, false).get());
  EXPECT_EQ(
    "ConstantTemp",
    idf_vrf
      .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    9, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceEvaporatingTemperatureforIndoorUnit, false)
         .get());
  EXPECT_EQ(
    10, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceCondensingTemperatureforIndoorUnit, false)
          .get());
  EXPECT_EQ(
    11,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    12,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    13,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMinimumforIndoorUnit, false)
      .get());
  EXPECT_EQ(
    14,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMaximumforIndoorUnit, false)
      .get());
  EXPECT_EQ(15, idf_vrf
                  .getDouble(
                    AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, false)
                  .get());
  EXPECT_EQ(
    16,
    idf_vrf
      .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity, false)
      .get());
  EXPECT_EQ(
    curve1.nameString(),
    idf_vrf
      .getString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName, false)
      .get());
  EXPECT_EQ(
    curve2.nameString(),
    idf_vrf
      .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
                 false)
      .get());
  EXPECT_EQ(
    17, idf_vrf
          .getDouble(
            AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, false)
          .get());
  EXPECT_EQ(
    18, idf_vrf
          .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
                     false)
          .get());
  EXPECT_EQ(
    19,
    idf_vrf
      .getDouble(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        false)
      .get());
  EXPECT_EQ(
    20,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, false)
      .get());
  EXPECT_EQ(21, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThickness, false).get());
  EXPECT_EQ(
    22, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThermalConductivity, false).get());
  EXPECT_EQ(23,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor, false).get());
  EXPECT_EQ(24, idf_vrf.getInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, false).get());
  EXPECT_EQ(
    25, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity, false)
          .get());
  EXPECT_EQ(
    26,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, false)
      .get());
  EXPECT_EQ("ReverseCycle", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, false).get());
  EXPECT_EQ("OnDemand", idf_vrf.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, false).get());
  EXPECT_EQ(
    curve3.nameString(),
    idf_vrf
      .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
                 false)
      .get());
  EXPECT_EQ(27, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, false).get());
  EXPECT_EQ(28, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, false).get());
  EXPECT_EQ(
    29, idf_vrf
          .getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, false)
          .get());
  EXPECT_EQ(30, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressormaximumdeltaPressure, false).get());
  EXPECT_EQ(3,
            idf_vrf.getInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressorLoadingIndexEntries, false).get());

  ASSERT_EQ(3u, idf_vrf.numExtensibleGroups());

  auto egs = idf_vrf.extensibleGroups();

  IdfExtensibleGroup eg1 = egs[0];
  EXPECT_EQ(1, eg1.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve1.nameString(),
            eg1
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve1.nameString(),
            eg1
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                           LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
              .get());

  IdfExtensibleGroup eg2 = egs[1];
  EXPECT_EQ(2, eg2.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve2.nameString(),
            eg2
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve2.nameString(),
            eg2
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                           LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
              .get());

  IdfExtensibleGroup eg3 = egs[2];
  EXPECT_EQ(3, eg3.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::CompressorSpeedatLoadingIndex).get());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve3.nameString(),
            eg3
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                           LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
              .get());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve3.nameString(),
            eg3
              .getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
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

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlowFluidTemperatureControl_NoTerminals) {
  Model model;
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(6u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_SystemModel).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  ASSERT_EQ(1, ft.warnings().size());
  EXPECT_NE(std::string::npos, ft.warnings().front().logMessage().find("will not be translated as it has no terminals"));
  EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl).size());

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model, true);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl).size());
}
