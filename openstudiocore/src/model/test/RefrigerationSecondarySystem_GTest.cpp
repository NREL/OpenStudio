/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationSecondarySystem.hpp"
#include "../RefrigerationSecondarySystem_Impl.hpp"
#include "../RefrigerationAirChiller.hpp"
#include "../RefrigerationAirChiller_Impl.hpp"
#include "../RefrigerationCase.hpp"
#include "../RefrigerationCase_Impl.hpp"
#include "../RefrigerationWalkIn.hpp"
#include "../RefrigerationWalkIn_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationSecondarySystem_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_CirculatingFluidName)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  EXPECT_EQ("PropyleneGlycol", testObject.circulatingFluidName());
  EXPECT_TRUE(testObject.setCirculatingFluidName("EthyleneGlycol"));
  EXPECT_TRUE(testObject.setCirculatingFluidName("R410a"));
  EXPECT_TRUE(testObject.setCirculatingFluidName("R22"));
  EXPECT_FALSE(testObject.setCirculatingFluidName("Not Valid"));
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_GlycolConcentration)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  EXPECT_EQ(30, testObject.glycolConcentration().get());
  EXPECT_TRUE(testObject.setGlycolConcentration(0));
  EXPECT_EQ(0, testObject.glycolConcentration().get());
  EXPECT_TRUE(testObject.setGlycolConcentration(100));
  EXPECT_EQ(100, testObject.glycolConcentration().get());
  EXPECT_FALSE(testObject.setGlycolConcentration(-1));
  EXPECT_FALSE(testObject.setGlycolConcentration(101));
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_Remove)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  std::vector<RefrigerationSecondarySystem> refrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(1, refrigerationSecondarySystems.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(1, refrigerationModelObjectLists.size());

  CurveCubic variableSpeedPumpCubicCurve = CurveCubic(model);
  variableSpeedPumpCubicCurve.setCoefficient1Constant(0.06757726281);
  variableSpeedPumpCubicCurve.setCoefficient2x(0.001078032);
  variableSpeedPumpCubicCurve.setCoefficient3xPOW2(-0.0000602558);
  variableSpeedPumpCubicCurve.setCoefficient4xPOW3(0.00000123732);
  variableSpeedPumpCubicCurve.setMinimumValueofx(-35);
  variableSpeedPumpCubicCurve.setMaximumValueofx(20);
  testObject.setVariableSpeedPumpCubicCurve(variableSpeedPumpCubicCurve);

  std::vector<CurveCubic> refrigerationCurveCubics = model.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, refrigerationCurveCubics.size());

  testObject.remove();

  refrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(0, refrigerationSecondarySystems.size());

  refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());

  refrigerationCurveCubics = model.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, refrigerationCurveCubics.size());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  RefrigerationSecondarySystem testObjectClone = testObject.clone(model).cast<RefrigerationSecondarySystem>();

  EXPECT_EQ("PropyleneGlycol", testObjectClone.circulatingFluidName());
  EXPECT_EQ(30, testObjectClone.glycolConcentration().get());
  EXPECT_DOUBLE_EQ(35000.0 ,testObjectClone.evaporatorCapacity().get());
  EXPECT_DOUBLE_EQ(0.0021 ,testObjectClone.evaporatorFlowRateforSecondaryFluid().get());
  EXPECT_DOUBLE_EQ(-12.6 ,testObjectClone.evaporatorEvaporatingTemperature());
  EXPECT_DOUBLE_EQ(2.7 ,testObjectClone.evaporatorApproachTemperatureDifference());
  EXPECT_DOUBLE_EQ(4.0 ,testObjectClone.evaporatorRangeTemperatureDifference().get());
  EXPECT_EQ(3, testObjectClone.numberofPumpsinLoop());
  EXPECT_DOUBLE_EQ(0.0023 ,testObjectClone.totalPumpFlowRate().get());
  EXPECT_DOUBLE_EQ(209000.0 ,testObjectClone.totalPumpHead().get());
  EXPECT_DOUBLE_EQ(1.0 ,testObjectClone.pumpMotorHeattoFluid());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_CloneOneModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);
  CurveCubic variableSpeedPumpCubicCurve = CurveCubic(model);
  variableSpeedPumpCubicCurve.setCoefficient1Constant(0.06757726281);
  variableSpeedPumpCubicCurve.setCoefficient2x(0.001078032);
  variableSpeedPumpCubicCurve.setCoefficient3xPOW2(-0.0000602558);
  variableSpeedPumpCubicCurve.setCoefficient4xPOW3(0.00000123732);
  variableSpeedPumpCubicCurve.setMinimumValueofx(-35);
  variableSpeedPumpCubicCurve.setMaximumValueofx(20);
  testObject.setVariableSpeedPumpCubicCurve(variableSpeedPumpCubicCurve);
  testObject.setDistributionPipingZone(thermalZone);
  testObject.setReceiverSeparatorZone(thermalZone);
  testObject.setCirculatingFluidName("R410a");
  testObject.setGlycolConcentration(0);
  testObject.setEvaporatorCapacity(999.0);
  testObject.setEvaporatorFlowRateforSecondaryFluid(999.0);
  testObject.setEvaporatorEvaporatingTemperature(999.0);
  testObject.setEvaporatorApproachTemperatureDifference(999.0);
  testObject.setEvaporatorRangeTemperatureDifference(999.0);
  testObject.setNumberofPumpsinLoop(999);
  testObject.setTotalPumpFlowRate(999.0);
  testObject.setTotalPumpHead(999.0);
  testObject.setPumpMotorHeattoFluid(0.5);

  RefrigerationSecondarySystem testObjectClone = testObject.clone(model).cast<RefrigerationSecondarySystem>();

  std::vector<CurveCubic> refrigerationSecondarySystemCurves = model.getModelObjects<CurveCubic>();
  for(auto it = refrigerationSecondarySystemCurves.begin(); it != refrigerationSecondarySystemCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_EQ("R410a", testObjectClone.circulatingFluidName());
  EXPECT_EQ(0, testObjectClone.glycolConcentration().get());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.evaporatorCapacity().get());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.evaporatorFlowRateforSecondaryFluid().get());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.evaporatorEvaporatingTemperature());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.evaporatorApproachTemperatureDifference());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.evaporatorRangeTemperatureDifference().get());
  EXPECT_EQ(999, testObjectClone.numberofPumpsinLoop());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.totalPumpFlowRate().get());
  EXPECT_DOUBLE_EQ(999.0 ,testObjectClone.totalPumpHead().get());
  EXPECT_DOUBLE_EQ(0.5 ,testObjectClone.pumpMotorHeattoFluid());
  EXPECT_TRUE(testObjectClone.variableSpeedPumpCubicCurve());
  EXPECT_EQ(testObject.variableSpeedPumpCubicCurve().get().handle(), testObjectClone.variableSpeedPumpCubicCurve().get().handle());
  EXPECT_FALSE(testObjectClone.distributionPipingZone());
  EXPECT_FALSE(testObjectClone.receiverSeparatorZone());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_CloneTwoModelsWithDefaultData)
{
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  RefrigerationSecondarySystem testObjectClone = testObject.clone(model).cast<RefrigerationSecondarySystem>();

  Model model2;

  RefrigerationSecondarySystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationSecondarySystem>();
  EXPECT_EQ("PropyleneGlycol", testObjectClone2.circulatingFluidName());
  EXPECT_EQ(30, testObjectClone2.glycolConcentration().get());
  EXPECT_DOUBLE_EQ(35000.0 ,testObjectClone2.evaporatorCapacity().get());
  EXPECT_DOUBLE_EQ(0.0021 ,testObjectClone2.evaporatorFlowRateforSecondaryFluid().get());
  EXPECT_DOUBLE_EQ(-12.6 ,testObjectClone2.evaporatorEvaporatingTemperature());
  EXPECT_DOUBLE_EQ(2.7 ,testObjectClone2.evaporatorApproachTemperatureDifference());
  EXPECT_DOUBLE_EQ(4.0 ,testObjectClone2.evaporatorRangeTemperatureDifference().get());
  EXPECT_EQ(3, testObjectClone2.numberofPumpsinLoop());
  EXPECT_DOUBLE_EQ(0.0023 ,testObjectClone2.totalPumpFlowRate().get());
  EXPECT_DOUBLE_EQ(209000.0 ,testObjectClone2.totalPumpHead().get());
  EXPECT_DOUBLE_EQ(1.0 ,testObjectClone2.pumpMotorHeattoFluid());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_Cases)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_AddCase)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  EXPECT_TRUE(testObject.addCase(case1));
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveCase)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addCase(case1);
  testObject.addCase(case2);
  testObject.removeCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_EQ(case2.handle(), cases[0].handle());

  testObject.removeCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  cases = testObject.cases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveAllCases)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addCase(case1);
  testObject.addCase(case2);
  testObject.removeAllCases();

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_AirChillers)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addAirChiller(airChiller1);
  airChillers = testObject.airChillers();
  EXPECT_EQ(1, airChillers.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_AddAirChiller)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  EXPECT_TRUE(testObject.addAirChiller(airChiller1));
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveAirChiller)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);
  RefrigerationAirChiller airChiller2 = RefrigerationAirChiller(model, s1);

  testObject.addAirChiller(airChiller1);
  testObject.addAirChiller(airChiller2);
  testObject.removeAirChiller(airChiller1);

  std::vector<RefrigerationAirChiller> testRefrigerationAirChillers = model.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_EQ(1, airChillers.size());
  EXPECT_EQ(airChiller2.handle(), airChillers[0].handle());

  testObject.removeAirChiller(airChiller2);
  testRefrigerationAirChillers = model.getModelObjects<RefrigerationAirChiller>();
  airChillers = testObject.airChillers();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveAllAirChillers)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);
  RefrigerationAirChiller airChiller2 = RefrigerationAirChiller(model, s1);

  testObject.addAirChiller(airChiller1);
  testObject.addAirChiller(airChiller2);
  testObject.removeAllAirChillers();

  std::vector<RefrigerationAirChiller> testRefrigerationAirChillers = model.getModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_Walkins)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_AddWalkIn)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  EXPECT_TRUE(testObject.addWalkin(walkin1));
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveWalkIn)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.removeWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(walkin2.handle(), walkins[0].handle());

  testObject.removeWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  walkins = testObject.walkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(walkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveAllWalkIns)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.removeAllWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_CasesAndWalkins)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(1, cases.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveCaseAndWalkIn)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.removeWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(walkin2.handle(), walkins[0].handle());

  testObject.addCase(case1);
  testObject.addCase(case2);
  testObject.removeCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_EQ(case2.handle(), cases[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  walkins = testObject.walkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(walkins.empty());

  testObject.removeCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  cases = testObject.cases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(cases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSecondarySystem_RemoveAllCasesAndWalkIns)
{
  Model model;
  RefrigerationSecondarySystem testObject = RefrigerationSecondarySystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.addCase(case1);
  testObject.addCase(case2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllCases();
  testObject.removeAllWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSecondarySystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}
