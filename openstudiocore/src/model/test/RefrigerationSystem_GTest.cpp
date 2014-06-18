/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationSystem.hpp"
#include "../RefrigerationSystem_Impl.hpp"
#include "../RefrigerationAirChiller.hpp"
#include "../RefrigerationAirChiller_Impl.hpp"
#include "../RefrigerationCompressor.hpp"
#include "../RefrigerationCompressor_Impl.hpp"
#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../RefrigerationCondenserEvaporativeCooled.hpp"
#include "../RefrigerationCondenserEvaporativeCooled_Impl.hpp"
#include "../RefrigerationCondenserWaterCooled.hpp"
#include "../RefrigerationCondenserWaterCooled_Impl.hpp"
#include "../RefrigerationCondenserCascade.hpp"
#include "../RefrigerationCondenserCascade_Impl.hpp"
#include "../RefrigerationCase.hpp"
#include "../RefrigerationCase_Impl.hpp"
#include "../RefrigerationWalkIn.hpp"
#include "../RefrigerationWalkIn_Impl.hpp"
#include "../RefrigerationSecondarySystem.hpp"
#include "../RefrigerationSecondarySystem_Impl.hpp"
#include "../RefrigerationSubcoolerLiquidSuction.hpp"
#include "../RefrigerationSubcoolerLiquidSuction_Impl.hpp"
#include "../RefrigerationSubcoolerMechanical.hpp"
#include "../RefrigerationSubcoolerMechanical_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationSystem_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationSystem testObject = RefrigerationSystem(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationSystem_Remove)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcool = RefrigerationSubcoolerLiquidSuction(model);

  testObject.setRefrigerationCondenser(condenser);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcool);

  std::vector<RefrigerationSystem> refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
  EXPECT_EQ(1, refrigerationSystems.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(4, refrigerationModelObjectLists.size());

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());

  std::vector<RefrigerationSubcoolerMechanical> refrigerationSubcoolerMechanicals = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, refrigerationSubcoolerMechanicals.size());

  std::vector<RefrigerationSubcoolerLiquidSuction> refrigerationSubcoolerLiquidSuctions = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, refrigerationSubcoolerLiquidSuctions.size());

  testObject.remove();

  refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
  EXPECT_EQ(0, refrigerationSystems.size());

  refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());

  refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(0, refrigerationCondensers.size());

  refrigerationSubcoolerMechanicals = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(0, refrigerationSubcoolerMechanicals.size());

  refrigerationSubcoolerLiquidSuctions = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(0, refrigerationSubcoolerLiquidSuctions.size());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_EQ("R404a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("ConstantSuctionTemperature", testObjectClone.suctionTemperatureControlType());
  EXPECT_DOUBLE_EQ(21.0, testObjectClone.minimumCondensingTemperature());
  EXPECT_TRUE(testObjectClone.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.sumUASuctionPiping());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneOneModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor compressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor highStageCompressor1 = RefrigerationCompressor(model);
  RefrigerationCondenserCascade condenser1 = RefrigerationCondenserCascade(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcool = RefrigerationSubcoolerLiquidSuction(model);

  RefrigerationSystem testObject = RefrigerationSystem(model);
  testObject.setMinimumCondensingTemperature(999.0);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSuctionTemperatureControlType("FloatSuctionTemperature");
  testObject.setSumUASuctionPiping(999.0);
  testObject.setSuctionPipingZone(thermalZone);
  testObject.addCase(case1);
  testObject.addWalkin(walkin1);
  testObject.addCompressor(compressor1);
  testObject.addHighStageCompressor(highStageCompressor1);
  testObject.setRefrigerationCondenser(condenser1);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcool);

  std::vector<RefrigerationCase> _cases = testObject.cases();
  std::vector<RefrigerationWalkIn> _walkins = testObject.walkins();
  std::vector<RefrigerationCompressor> _compressors = testObject.compressors();
  std::vector<RefrigerationCompressor> _highStageCompressors = testObject.highStageCompressors();

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumCondensingTemperature());
  EXPECT_FALSE(testObjectClone.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.sumUASuctionPiping());
  EXPECT_EQ("R410a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("FloatSuctionTemperature", testObjectClone.suctionTemperatureControlType());
  EXPECT_FALSE(testObjectClone.suctionPipingZone());
  std::vector<RefrigerationCase> casesClone = testObjectClone.cases();
  EXPECT_EQ(1, casesClone.size());
  EXPECT_NE(casesClone[0].handle(), _cases[0].handle());
  std::vector<RefrigerationWalkIn> walkinsClone = testObjectClone.walkins();
  EXPECT_EQ(1, walkinsClone.size());
  EXPECT_NE(walkinsClone[0].handle(), _walkins[0].handle());
  std::vector<RefrigerationCompressor> compressorsClone = testObjectClone.compressors();
  EXPECT_EQ(1, compressorsClone.size());
  EXPECT_NE(compressorsClone[0].handle(), _compressors[0].handle());
  std::vector<RefrigerationCompressor> highStageCompressorsClone = testObjectClone.highStageCompressors();
  EXPECT_EQ(1, highStageCompressorsClone.size());
  EXPECT_NE(highStageCompressorsClone[0].handle(), _highStageCompressors[0].handle());
  EXPECT_NE(testObjectClone.refrigerationCondenser().get().handle(), condenser1.handle());
  EXPECT_NE(testObjectClone.mechanicalSubcooler().get().handle(), mechSubcooler.handle());
  EXPECT_NE(testObjectClone.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcool.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneTwoModelsWithDefaultData)
{
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();

  Model model2;

  RefrigerationSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();
  EXPECT_EQ("R404a", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("ConstantSuctionTemperature", testObjectClone2.suctionTemperatureControlType());
  EXPECT_DOUBLE_EQ(21.0, testObjectClone2.minimumCondensingTemperature());
  EXPECT_TRUE(testObjectClone2.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.sumUASuctionPiping());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneTwoModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor compressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor highStageCompressor1 = RefrigerationCompressor(model);
  RefrigerationCondenserCascade condenser1 = RefrigerationCondenserCascade(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcooler = RefrigerationSubcoolerLiquidSuction(model);

  RefrigerationSystem testObject = RefrigerationSystem(model);
  testObject.setMinimumCondensingTemperature(999.0);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSuctionTemperatureControlType("FloatSuctionTemperature");
  testObject.setSumUASuctionPiping(999.0);
  testObject.setSuctionPipingZone(thermalZone);
  testObject.addCase(case1);
  testObject.addWalkin(walkin1);
  testObject.addCompressor(compressor1);
  testObject.addHighStageCompressor(highStageCompressor1);
  testObject.setRefrigerationCondenser(condenser1);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcooler);

  std::vector<RefrigerationCase> _cases = testObject.cases();
  std::vector<RefrigerationWalkIn> _walkins = testObject.walkins();
  std::vector<RefrigerationCompressor> _compressors = testObject.compressors();
  std::vector<RefrigerationCompressor> _highStageCompressors = testObject.highStageCompressors();

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();

  Model model2;
  RefrigerationSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.minimumCondensingTemperature());
  EXPECT_FALSE(testObjectClone2.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.sumUASuctionPiping());
  EXPECT_EQ("R410a", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("FloatSuctionTemperature", testObjectClone2.suctionTemperatureControlType());
  EXPECT_FALSE(testObjectClone2.suctionPipingZone());
  std::vector<RefrigerationCase> casesClone = testObjectClone2.cases();
  EXPECT_EQ(1, casesClone.size());
  EXPECT_NE(casesClone[0].handle(), _cases[0].handle());
  std::vector<RefrigerationWalkIn> walkinsClone = testObjectClone2.walkins();
  EXPECT_EQ(1, walkinsClone.size());
  EXPECT_NE(walkinsClone[0].handle(), _walkins[0].handle());
  std::vector<RefrigerationCompressor> compressorsClone = testObjectClone2.compressors();
  EXPECT_EQ(1, compressorsClone.size());
  EXPECT_NE(compressorsClone[0].handle(), _compressors[0].handle());
  std::vector<RefrigerationCompressor> highStageCompressorsClone = testObjectClone2.highStageCompressors();
  EXPECT_EQ(1, highStageCompressorsClone.size());
  EXPECT_NE(highStageCompressorsClone[0].handle(), _highStageCompressors[0].handle());
  EXPECT_NE(testObjectClone2.refrigerationCondenser().get().handle(), condenser1.handle());
  EXPECT_NE(testObjectClone2.mechanicalSubcooler().get().handle(), mechSubcooler.handle());
  EXPECT_NE(testObjectClone2.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_MechanicalSubcooler)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);

  EXPECT_FALSE(testObject.mechanicalSubcooler());
  EXPECT_TRUE(testObject.setMechanicalSubcooler(mechSubcooler));
  EXPECT_TRUE(testObject.mechanicalSubcooler());

  std::vector<RefrigerationSubcoolerMechanical> testMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_NE(testObjectClone.mechanicalSubcooler().get().handle(), mechSubcooler.handle());

  testMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(2, testMechanicalSubcoolers.size());

  Model model2;
  RefrigerationSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  testMechanicalSubcoolers = model2.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());

  EXPECT_NE(testObjectClone2.mechanicalSubcooler().get().handle(), mechSubcooler.handle());

  mechSubcooler.remove();

  EXPECT_FALSE(testObject.mechanicalSubcooler());
  EXPECT_TRUE(testObjectClone.mechanicalSubcooler());
  EXPECT_TRUE(testObjectClone2.mechanicalSubcooler());

  testMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationSubcoolerLiquidSuction)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcooler = RefrigerationSubcoolerLiquidSuction(model);

  EXPECT_FALSE(testObject.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcooler));
  EXPECT_TRUE(testObject.liquidSuctionHeatExchangerSubcooler());

  std::vector<RefrigerationSubcoolerLiquidSuction> testLiqSuctionSubcoolers = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_NE(testObjectClone.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());

  testLiqSuctionSubcoolers = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(2, testLiqSuctionSubcoolers.size());

  Model model2;
  RefrigerationSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  testLiqSuctionSubcoolers = model2.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());

  EXPECT_NE(testObjectClone2.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());

  liqSuctionSubcooler.remove();

  EXPECT_FALSE(testObject.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObjectClone.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObjectClone2.liquidSuctionHeatExchangerSubcooler());

  testLiqSuctionSubcoolers = model.getModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSystem_Compressors)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_TRUE(compressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCompressor)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addCompressor(testCompressor));

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_EQ(1, compressors.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCompressor)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addCompressor(testCompressor1);
  testObject.addCompressor(testCompressor2);
  testObject.removeCompressor(testCompressor1);

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_EQ(1, compressors.size());
  EXPECT_EQ(testCompressor2.handle(), compressors[0].handle());

  testObject.removeCompressor(testCompressor2);
  testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  compressors = testObject.compressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(compressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCompressors)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addCompressor(testCompressor1);
  testObject.addCompressor(testCompressor2);
  testObject.removeAllCompressors();

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_TRUE(compressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->compressorList());
}

TEST_F(ModelFixture, RefrigerationSystem_HighStageCompressors)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_TRUE(highStageCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_AddHighStageCompressor)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addHighStageCompressor(testCompressor));

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(1, highStageCompressors.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveHighStageCompressor)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighStageCompressor(testCompressor1);
  testObject.addHighStageCompressor(testCompressor2);
  testObject.removeHighStageCompressor(testCompressor1);

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(1, highStageCompressors.size());
  EXPECT_EQ(testCompressor2.handle(), highStageCompressors[0].handle());

  testObject.removeHighStageCompressor(testCompressor2);
  testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(highStageCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllHighStageCompressors)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighStageCompressor(testCompressor1);
  testObject.addHighStageCompressor(testCompressor2);
  testObject.removeAllHighStageCompressors();

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_TRUE(highStageCompressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->highStageCompressorList());
}

TEST_F(ModelFixture, RefrigerationSystem_Cases)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCase)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  EXPECT_TRUE(testObject.addCase(case1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCase)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCases)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  
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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AirChillers)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addAirChiller(airChiller1);
  airChillers = testObject.airChillers();
  EXPECT_EQ(1, airChillers.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddAirChiller)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  EXPECT_TRUE(testObject.addAirChiller(airChiller1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAirChiller)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllAirChillers)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  
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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_Walkins)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddWalkIn)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  EXPECT_TRUE(testObject.addWalkin(walkin1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveWalkIn)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllWalkIns)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_CasesAndWalkins)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(1, cases.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCaseAndWalkIn)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
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

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCasesAndWalkIns)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

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

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
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

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_SecondarySystemLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());

  testObject.addSecondarySystemLoad(secondarySystem1);
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddSecondarySystemLoad)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  EXPECT_TRUE(testObject.addSecondarySystemLoad(secondarySystem1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveSecondarySystemLoad)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  testObject.removeSecondarySystemLoad(secondarySystem1);

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_EQ(secondarySystem2.handle(), secondarySystemLoads[0].handle());

  testObject.removeSecondarySystemLoad(secondarySystem2);
  testRefrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllSecondarySystemLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  testObject.removeAllSecondarySystemLoads();

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_CascadeCondenserLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCascadeCondenserLoad)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);

  EXPECT_TRUE(testObject.addCascadeCondenserLoad(condenserCascade1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCascadeCondenserLoad)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.removeCascadeCondenserLoad(condenserCascade1);

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(condenserCascade2.handle(), cascadeCondenserLoads[0].handle());

  testObject.removeCascadeCondenserLoad(condenserCascade2);
  testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCascadeCondenserLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.removeAllCascadeCondenserLoads();

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_TransferLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  testObject.addSecondarySystemLoad(secondarySystem1);
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(1, secondarySystemLoads.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveTransferLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  testObject.removeSecondarySystemLoad(secondarySystem1);

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_EQ(secondarySystem2.handle(), secondarySystemLoads[0].handle());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.removeCascadeCondenserLoad(condenserCascade1);

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(condenserCascade2.handle(), cascadeCondenserLoads[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeSecondarySystemLoad(secondarySystem2);
  testRefrigerationSecondarySystems = model.getModelObjects<RefrigerationSecondarySystem>();
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());
  EXPECT_TRUE(secondarySystemLoads.empty());

  testObject.removeCascadeCondenserLoad(condenserCascade2);
  testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  EXPECT_TRUE(cascadeCondenserLoads.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllTransferLoads)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllCascadeCondenserLoads();
  testObject.removeAllSecondarySystemLoads();

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  std::vector<RefrigerationSecondarySystem> testSecondarySystemLoads = model.getModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testSecondarySystemLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationSystemWorkingFluidType)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  EXPECT_EQ("R404a", testObject.refrigerationSystemWorkingFluidType());
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R410a"));
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R22"));
  EXPECT_FALSE(testObject.setRefrigerationSystemWorkingFluidType("Not Valid"));
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationCondenser)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserAirCooled condenserAirCooled = RefrigerationCondenserAirCooled(model);
  RefrigerationCondenserEvaporativeCooled condenserEvaporativeCooled = RefrigerationCondenserEvaporativeCooled(model);
  RefrigerationCondenserCascade condenserCascade = RefrigerationCondenserCascade(model);
  RefrigerationCondenserWaterCooled condenserWaterCooled = RefrigerationCondenserWaterCooled(model);

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserAirCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserAirCooled.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserEvaporativeCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserEvaporativeCooled.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserCascade));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserCascade.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserWaterCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserWaterCooled.handle());

  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);
  EXPECT_FALSE(testObject.setRefrigerationCondenser(testCompressor));
}
