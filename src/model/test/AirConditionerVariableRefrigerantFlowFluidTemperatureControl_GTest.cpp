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
#include "ModelFixture.hpp"
#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "../LoadingIndex.hpp"
#include "../LoadingIndex_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"
#include "../FanVariableVolume.hpp"
#include "../FanVariableVolume_Impl.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_AirConditionerVariableRefrigerantFlowFluidTemperatureControl) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  auto alwaysOn = model.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, vrf.availabilitySchedule());
  EXPECT_EQ(0u, vrf.terminals().size());
  EXPECT_EQ("R410a", vrf.refrigerantType());
  ASSERT_TRUE(vrf.ratedEvaporativeCapacity());
  EXPECT_EQ(40000, vrf.ratedEvaporativeCapacity().get());
  EXPECT_FALSE(vrf.isRatedEvaporativeCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedRatedEvaporativeCapacity());
  EXPECT_EQ(0.35, vrf.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());
  EXPECT_EQ(-6.0, vrf.minimumOutdoorAirTemperatureinCoolingMode());
  EXPECT_EQ(43.0, vrf.maximumOutdoorAirTemperatureinCoolingMode());
  EXPECT_EQ(-20.0, vrf.minimumOutdoorAirTemperatureinHeatingMode());
  EXPECT_EQ(16.0, vrf.maximumOutdoorAirTemperatureinHeatingMode());
  EXPECT_EQ(3, vrf.referenceOutdoorUnitSuperheating());
  EXPECT_EQ(5, vrf.referenceOutdoorUnitSubcooling());
  EXPECT_EQ("VariableTemp", vrf.refrigerantTemperatureControlAlgorithmforIndoorUnit());
  EXPECT_EQ(6.0, vrf.referenceEvaporatingTemperatureforIndoorUnit());
  EXPECT_EQ(44.0, vrf.referenceCondensingTemperatureforIndoorUnit());
  EXPECT_EQ(4.0, vrf.variableEvaporatingTemperatureMinimumforIndoorUnit());
  EXPECT_EQ(13.0, vrf.variableEvaporatingTemperatureMaximumforIndoorUnit());
  EXPECT_EQ(42.0, vrf.variableCondensingTemperatureMinimumforIndoorUnit());
  EXPECT_EQ(46.0, vrf.variableCondensingTemperatureMaximumforIndoorUnit());
  EXPECT_EQ(0.00425, vrf.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());
  EXPECT_EQ(0.000075, vrf.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());
  Curve curve1 = vrf.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  boost::optional<CurveQuadratic> outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = curve1.optionalCast<CurveQuadratic>();
  EXPECT_TRUE(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
  Curve curve2 = vrf.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  boost::optional<CurveQuadratic> outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve = curve2.optionalCast<CurveQuadratic>();
  EXPECT_TRUE(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
  EXPECT_EQ(0.0762, vrf.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(30.0, vrf.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(36.0, vrf.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(5.0, vrf.heightDifferenceBetweenOutdoorUnitandIndoorUnits());
  EXPECT_EQ(0.02, vrf.mainPipeInsulationThickness());
  EXPECT_EQ(0.032, vrf.mainPipeInsulationThermalConductivity());
  EXPECT_EQ(33.0, vrf.crankcaseHeaterPowerperCompressor());
  EXPECT_EQ(2, vrf.numberofCompressors());
  EXPECT_EQ(0.5, vrf.ratioofCompressorSizetoTotalCompressorCapacity());
  EXPECT_EQ(5.0, vrf.maximumOutdoorDryBulbTemperatureforCrankcaseHeater());
  EXPECT_EQ("Resistive", vrf.defrostStrategy());
  EXPECT_EQ("Timed", vrf.defrostControl());
  EXPECT_FALSE(vrf.defrostEnergyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(0.058333, vrf.defrostTimePeriodFraction());
  ASSERT_TRUE(vrf.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(0.0, vrf.resistiveDefrostHeaterCapacity().get());
  EXPECT_FALSE(vrf.isResistiveDefrostHeaterCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedResistiveDefrostHeaterCapacity());
  EXPECT_EQ(5.0, vrf.maximumOutdoorDrybulbTemperatureforDefrostOperation());
  EXPECT_EQ(4500000.0, vrf.compressorMaximumDeltaPressure());
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_SetGetFields) {
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

  Schedule schedule = vrf.availabilitySchedule();
  boost::optional<ScheduleConstant> _scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(_scheduleConstant);
  EXPECT_EQ((*_scheduleConstant).value(), 0.5);
  EXPECT_EQ(0u, vrf.terminals().size());
  EXPECT_EQ("R11", vrf.refrigerantType());
  ASSERT_TRUE(vrf.ratedEvaporativeCapacity());
  EXPECT_EQ(1, vrf.ratedEvaporativeCapacity().get());
  EXPECT_FALSE(vrf.isRatedEvaporativeCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedRatedEvaporativeCapacity());
  EXPECT_EQ(2, vrf.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());
  EXPECT_EQ(3, vrf.minimumOutdoorAirTemperatureinCoolingMode());
  EXPECT_EQ(4, vrf.maximumOutdoorAirTemperatureinCoolingMode());
  EXPECT_EQ(5, vrf.minimumOutdoorAirTemperatureinHeatingMode());
  EXPECT_EQ(6, vrf.maximumOutdoorAirTemperatureinHeatingMode());
  EXPECT_EQ(7, vrf.referenceOutdoorUnitSuperheating());
  EXPECT_EQ(8, vrf.referenceOutdoorUnitSubcooling());
  EXPECT_EQ("ConstantTemp", vrf.refrigerantTemperatureControlAlgorithmforIndoorUnit());
  EXPECT_EQ(9, vrf.referenceEvaporatingTemperatureforIndoorUnit());
  EXPECT_EQ(10, vrf.referenceCondensingTemperatureforIndoorUnit());
  EXPECT_EQ(11, vrf.variableEvaporatingTemperatureMinimumforIndoorUnit());
  EXPECT_EQ(12, vrf.variableEvaporatingTemperatureMaximumforIndoorUnit());
  EXPECT_EQ(13, vrf.variableCondensingTemperatureMinimumforIndoorUnit());
  EXPECT_EQ(14, vrf.variableCondensingTemperatureMaximumforIndoorUnit());
  EXPECT_EQ(15, vrf.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());
  EXPECT_EQ(16, vrf.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());
  Curve _curve1 = vrf.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  boost::optional<CurveCubic> outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = _curve1.optionalCast<CurveCubic>();
  EXPECT_TRUE(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
  Curve _curve2 = vrf.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  boost::optional<CurveCubic> outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve = _curve2.optionalCast<CurveCubic>();
  EXPECT_TRUE(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
  EXPECT_EQ(17, vrf.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(18, vrf.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(19, vrf.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  EXPECT_EQ(20, vrf.heightDifferenceBetweenOutdoorUnitandIndoorUnits());
  EXPECT_EQ(21, vrf.mainPipeInsulationThickness());
  EXPECT_EQ(22, vrf.mainPipeInsulationThermalConductivity());
  EXPECT_EQ(23, vrf.crankcaseHeaterPowerperCompressor());
  EXPECT_EQ(24, vrf.numberofCompressors());
  EXPECT_EQ(25, vrf.ratioofCompressorSizetoTotalCompressorCapacity());
  EXPECT_EQ(26, vrf.maximumOutdoorDryBulbTemperatureforCrankcaseHeater());
  EXPECT_EQ("ReverseCycle", vrf.defrostStrategy());
  EXPECT_EQ("OnDemand", vrf.defrostControl());
  boost::optional<Curve> _curve3 = vrf.defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  ASSERT_TRUE(_curve3);
  boost::optional<CurveBiquadratic> defrostEnergyInputRatioModifierFunctionofTemperatureCurve = _curve3->optionalCast<CurveBiquadratic>();
  EXPECT_TRUE(defrostEnergyInputRatioModifierFunctionofTemperatureCurve);
  EXPECT_EQ(27, vrf.defrostTimePeriodFraction());
  ASSERT_TRUE(vrf.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(28, vrf.resistiveDefrostHeaterCapacity().get());
  EXPECT_FALSE(vrf.isResistiveDefrostHeaterCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedResistiveDefrostHeaterCapacity());
  EXPECT_EQ(29, vrf.maximumOutdoorDrybulbTemperatureforDefrostOperation());
  EXPECT_EQ(30, vrf.compressorMaximumDeltaPressure());

  vrf.autosizeRatedEvaporativeCapacity();
  vrf.resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  vrf.autosizeResistiveDefrostHeaterCapacity();

  EXPECT_FALSE(vrf.ratedEvaporativeCapacity());
  EXPECT_TRUE(vrf.isRatedEvaporativeCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedRatedEvaporativeCapacity());
  EXPECT_FALSE(vrf.defrostEnergyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(vrf.resistiveDefrostHeaterCapacity());
  EXPECT_TRUE(vrf.isResistiveDefrostHeaterCapacityAutosized());
  EXPECT_FALSE(vrf.autosizedResistiveDefrostHeaterCapacity());
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_LoadingIndexes) {
  Model model;

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  EXPECT_EQ(3u, vrf.loadingIndexes().size());
  vrf.removeAllLoadingIndexes();

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve1(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve1(model);

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve2(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve2(model);

  CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve3(model);
  CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve3(model);

  EXPECT_EQ(0u, vrf.loadingIndexes().size());
  LoadingIndex loadingIndex1(model, 1, evaporativeCapacityMultiplierFunctionofTemperatureCurve1,
                             compressorPowerMultiplierFunctionofTemperatureCurve1);
  vrf.addLoadingIndex(loadingIndex1);
  EXPECT_EQ(1u, vrf.loadingIndexes().size());
  LoadingIndex loadingIndex2(model, 2, evaporativeCapacityMultiplierFunctionofTemperatureCurve2,
                             compressorPowerMultiplierFunctionofTemperatureCurve2);
  vrf.addLoadingIndex(loadingIndex2);
  EXPECT_EQ(2u, vrf.loadingIndexes().size());
  LoadingIndex loadingIndex3(model, 3, evaporativeCapacityMultiplierFunctionofTemperatureCurve3,
                             compressorPowerMultiplierFunctionofTemperatureCurve3);
  vrf.addLoadingIndex(loadingIndex3);
  EXPECT_EQ(3u, vrf.loadingIndexes().size());

  std::vector<LoadingIndex> loadingIndexes = vrf.loadingIndexes();
  EXPECT_EQ(3u, loadingIndexes.size());
  EXPECT_EQ(1, loadingIndexes[0].compressorSpeed());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve1, loadingIndexes[0].evaporativeCapacityMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve1, loadingIndexes[0].compressorPowerMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(2, loadingIndexes[1].compressorSpeed());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve2, loadingIndexes[1].evaporativeCapacityMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve2, loadingIndexes[1].compressorPowerMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(3, loadingIndexes[2].compressorSpeed());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve3, loadingIndexes[2].evaporativeCapacityMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve3, loadingIndexes[2].compressorPowerMultiplierFunctionofTemperatureCurve());

  LoadingIndex loadingIndex4(model, 4, evaporativeCapacityMultiplierFunctionofTemperatureCurve1,
                             compressorPowerMultiplierFunctionofTemperatureCurve2);
  vrf.addLoadingIndex(loadingIndex4);
  EXPECT_EQ(4u, vrf.loadingIndexes().size());

  vrf.removeLoadingIndex(loadingIndex3);
  std::vector<LoadingIndex> loadingIndexes2 = vrf.loadingIndexes();
  EXPECT_EQ(3u, loadingIndexes2.size());
  EXPECT_EQ(2, loadingIndexes2[1].compressorSpeed());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve2, loadingIndexes2[1].evaporativeCapacityMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve2, loadingIndexes2[1].compressorPowerMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(4, loadingIndexes2[2].compressorSpeed());
  EXPECT_EQ(evaporativeCapacityMultiplierFunctionofTemperatureCurve1, loadingIndexes2[2].evaporativeCapacityMultiplierFunctionofTemperatureCurve());
  EXPECT_EQ(compressorPowerMultiplierFunctionofTemperatureCurve2, loadingIndexes2[2].compressorPowerMultiplierFunctionofTemperatureCurve());
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Terminals) {
  Model model;

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coolingCoil1(model);
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl heatingCoil1(model);
  FanVariableVolume fan1(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term1(model, coolingCoil1, heatingCoil1, fan1);

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coolingCoil2(model);
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl heatingCoil2(model);
  FanVariableVolume fan2(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term2(model, coolingCoil2, heatingCoil2, fan2);

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coolingCoil3(model);
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl heatingCoil3(model);
  FanVariableVolume fan3(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term3(model, coolingCoil3, heatingCoil3, fan3);

  EXPECT_EQ(0u, vrf.terminals().size());

  vrf.addTerminal(term1);
  vrf.addTerminal(term2);
  vrf.addTerminal(term3);

  EXPECT_EQ(3u, vrf.terminals().size());

  vrf.removeTerminal(term2);

  EXPECT_EQ(2u, vrf.terminals().size());
  EXPECT_EQ(term1, vrf.terminals()[0]);
  EXPECT_EQ(term3, vrf.terminals()[1]);

  vrf.removeAllTerminals();

  EXPECT_EQ(0u, vrf.terminals().size());
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Clone) {
  Model model;

  EXPECT_EQ(0, model.getModelObjects<Curve>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

  EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<LoadingIndex>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);
  {
    EXPECT_EQ(8, model.getModelObjects<Curve>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(6, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes
    EXPECT_EQ(3, model.getConcreteModelObjects<LoadingIndex>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    EXPECT_TRUE(vrf.setRefrigerantType("R12"));

    // VRFTU has no curves, but its Cooling Coil and Heating Coil each add a Quadratic and a Biquadratic, the FanOnOff add a Cubic and one Exponent
    ZoneHVACTerminalUnitVariableRefrigerantFlow term1(model);
    vrf.addTerminal(term1);
    EXPECT_EQ(14, model.getModelObjects<Curve>().size());
    EXPECT_EQ(4, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(8, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes
    EXPECT_EQ(3, model.getConcreteModelObjects<LoadingIndex>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    ZoneHVACTerminalUnitVariableRefrigerantFlow term2(model);
    vrf.addTerminal(term2);
    EXPECT_EQ(20, model.getModelObjects<Curve>().size());
    EXPECT_EQ(6, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(10, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes
    EXPECT_EQ(3, model.getConcreteModelObjects<LoadingIndex>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    EXPECT_EQ(2, vrf.terminals().size());
    EXPECT_EQ(3, vrf.loadingIndexes().size());
  }

  // Cloning the VRF will clone the Loading Indexes, but not the underlying Curves (these are ResourceObjects after all!)
  // The Terminals aren't cloned
  auto vrfClone = vrf.clone(model).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();
  {
    EXPECT_EQ("R12", vrfClone.refrigerantType());
    EXPECT_EQ(0, vrfClone.terminals().size());
    EXPECT_EQ(3, vrfClone.loadingIndexes().size());

    EXPECT_EQ(20, model.getModelObjects<Curve>().size());
    EXPECT_EQ(6, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(10, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(2, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(4, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes times 2
    EXPECT_EQ(6, model.getConcreteModelObjects<LoadingIndex>().size());     // 3 for each AirCondVRF
    EXPECT_EQ(2, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }

  {
    Model model2;

    EXPECT_EQ(0, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(0, model2.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<LoadingIndex>().size());  // 3 for each AirCondVRF
    EXPECT_EQ(0, model2.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    auto vrfClone2 = vrf.clone(model2).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();
    EXPECT_EQ("R12", vrfClone2.refrigerantType());
    EXPECT_EQ(0, vrfClone2.terminals().size());
    EXPECT_EQ(3, vrfClone2.loadingIndexes().size());

    EXPECT_EQ(8, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(2, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(6, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model2.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(2, model2.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes
    EXPECT_EQ(3, model2.getConcreteModelObjects<LoadingIndex>().size());     // 3 for each AirCondVRF
    EXPECT_EQ(0, model2.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }

  {
    // VRF + its terminals are gone, but the cloned one should be left intact (its curves and co are still there)
    vrf.remove();
    EXPECT_EQ(8, model.getModelObjects<Curve>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(6, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes
    EXPECT_EQ(3, model.getConcreteModelObjects<LoadingIndex>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    // Curves are no longer used anywhere, so should be removed too
    vrfClone.remove();
    EXPECT_EQ(0, model.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<LoadingIndex>().size());  // 3 for each AirCondVRF
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Remove) {
  Model model;

  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());

  auto size = model.modelObjects().size();
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);
  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  vrf.addTerminal(term);
  EXPECT_EQ(1, vrf.terminals().size());
  EXPECT_EQ(3, vrf.loadingIndexes().size());

  EXPECT_EQ(4, model.getConcreteModelObjects<CurveQuadratic>().size());    // 2 on vrf + 2 on coils
  EXPECT_EQ(8, model.getConcreteModelObjects<CurveBiquadratic>().size());  // 6 on vrf + 2 on coils
  EXPECT_EQ(1, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals + 1 loading indexes

  EXPECT_FALSE(vrf.remove().empty());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(size + 2, model.modelObjects().size());  // Always On Discrete, OnOff
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_addToNode) {
  Model model;

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf(model);

  AirLoopHVAC airLoop(model);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(vrf.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(vrf.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(model);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(vrf.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(vrf));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  EXPECT_FALSE(vrf.plantLoop());
  vrf.disconnect();
  EXPECT_FALSE(vrf.plantLoop());
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
}
