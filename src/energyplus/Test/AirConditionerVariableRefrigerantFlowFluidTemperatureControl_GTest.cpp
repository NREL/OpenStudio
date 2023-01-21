/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
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
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"

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
  EXPECT_TRUE(vrf.addLoadingIndex(1, evaporativeCapacityMultiplierFunctionofTemperatureCurve1, compressorPowerMultiplierFunctionofTemperatureCurve1));
  EXPECT_TRUE(vrf.addLoadingIndex(2, evaporativeCapacityMultiplierFunctionofTemperatureCurve2, compressorPowerMultiplierFunctionofTemperatureCurve2));
  EXPECT_TRUE(vrf.addLoadingIndex(3, evaporativeCapacityMultiplierFunctionofTemperatureCurve3, compressorPowerMultiplierFunctionofTemperatureCurve3));

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coolingCoil(model);
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl heatingCoil(model);
  FanVariableVolume fan(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model, coolingCoil, heatingCoil, fan);

  vrf.addTerminal(term);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(19u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(5u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  IdfObject idf_vrf = workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl)[0];

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

  EXPECT_EQ(3u, idf_vrf.numExtensibleGroups());
  auto egs = idf_vrf.extensibleGroups();
  auto loadingIndexes = vrf.loadingIndexes();
  for (size_t i = 0; i < idf_vrf.extensibleGroups().size(); ++i) {
    const IdfExtensibleGroup& eg = egs[i];
    const auto loadingIndex = loadingIndexes[i];

    EXPECT_EQ(i + 1,
              eg.getDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::CompressorSpeedatLoadingIndex).get());
    EXPECT_NE("", eg.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                                 LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName)
                    .get());
    EXPECT_NE("", eg.getString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                                 LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName)
                    .get());
  }

  IdfObject idf_term = workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow)[0];

  EXPECT_EQ("DrawThrough", idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, false).get());
  EXPECT_EQ("Fan:VariableVolume", idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectType, false).get());
  EXPECT_EQ(fan.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName, false).get());
  EXPECT_EQ("Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl",
            idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectType, false).get());
  EXPECT_EQ(coolingCoil.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectName, false).get());
  EXPECT_EQ("Coil:Heating:DX:VariableRefrigerantFlow:FluidTemperatureControl",
            idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectType, false).get());
  EXPECT_EQ(heatingCoil.nameString(), idf_term.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectName, false).get());

  IdfObject idf_cool = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl)[0];
  EXPECT_EQ("", idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("", idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, false).get());
  EXPECT_EQ("", idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, false).get());
  EXPECT_EQ("", idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, false).get());
  EXPECT_EQ("", idf_cool.getString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, false).get());
  EXPECT_EQ(0,
            idf_cool.getDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating, false).get());
  EXPECT_EQ(
    "",
    idf_cool
      .getString(
        Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName, false)
      .get());

  IdfObject idf_heat = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl)[0];
  EXPECT_EQ("", idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule, false).get());
  EXPECT_EQ("", idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, false).get());
  EXPECT_EQ("", idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, false).get());
  EXPECT_EQ("", idf_heat.getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, false).get());
  EXPECT_EQ(0, idf_heat.getDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling, false).get());
  EXPECT_EQ(
    "",
    idf_heat
      .getString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
                 false)
      .get());
}
