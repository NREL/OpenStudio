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

#include "../RefrigerationTranscriticalSystem.hpp"
#include "../RefrigerationTranscriticalSystem_Impl.hpp"
#include "../RefrigerationCompressor.hpp"
#include "../RefrigerationCompressor_Impl.hpp"
#include "../RefrigerationCase.hpp"
#include "../RefrigerationCase_Impl.hpp"
#include "../RefrigerationGasCoolerAirCooled.hpp"
#include "../RefrigerationGasCoolerAirCooled_Impl.hpp"
#include "../RefrigerationWalkIn.hpp"
#include "../RefrigerationWalkIn_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_Remove)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationGasCoolerAirCooled gasCooler = RefrigerationGasCoolerAirCooled(model);
  testObject.setRefrigerationGasCooler(gasCooler);

  std::vector<RefrigerationTranscriticalSystem> refrigerationSystems = model.getModelObjects<RefrigerationTranscriticalSystem>();
  EXPECT_EQ(1, refrigerationSystems.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(4, refrigerationModelObjectLists.size());

  std::vector<RefrigerationGasCoolerAirCooled> refrigerationGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(1, refrigerationGasCoolers.size());

  testObject.remove();

  refrigerationSystems = model.getModelObjects<RefrigerationTranscriticalSystem>();
  EXPECT_EQ(0, refrigerationSystems.size());

  refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());

  refrigerationGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(0, refrigerationGasCoolers.size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  RefrigerationTranscriticalSystem testObjectClone = testObject.clone(model).cast<RefrigerationTranscriticalSystem>();

  EXPECT_DOUBLE_EQ(3500000.0, testObjectClone.receiverPressure());
  EXPECT_DOUBLE_EQ(0.4, testObjectClone.subcoolerEffectiveness());
  EXPECT_EQ("R744", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.sumUASuctionPipingforMediumTemperatureLoads());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.sumUASuctionPipingforLowTemperatureLoads());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_CloneOneModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  ScheduleCompact wds(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor highPressureCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor lowPressureCompressor1 = RefrigerationCompressor(model);
  RefrigerationGasCoolerAirCooled gasCooler = RefrigerationGasCoolerAirCooled(model);

  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  testObject.setMediumTemperatureSuctionPipingZone(thermalZone);
  testObject.setLowTemperatureSuctionPipingZone(thermalZone);
  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addLowTemperatureCase(case2);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.addHighPressureCompressor(highPressureCompressor1);
  testObject.addLowPressureCompressor(lowPressureCompressor1);
  testObject.setRefrigerationGasCooler(gasCooler);

  testObject.setReceiverPressure(999.0);
  testObject.setSubcoolerEffectiveness(0.99);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSumUASuctionPipingforMediumTemperatureLoads(999.0);
  testObject.setSumUASuctionPipingforLowTemperatureLoads(999.0);

  std::vector<RefrigerationCase> _mediumTemperatureCases = testObject.mediumTemperatureCases();
  std::vector<RefrigerationWalkIn> _mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  std::vector<RefrigerationCase> _lowTemperatureCases = testObject.lowTemperatureCases();
  std::vector<RefrigerationWalkIn> _lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  std::vector<RefrigerationCompressor> _highPressureCompressors = testObject.highPressureCompressors();
  std::vector<RefrigerationCompressor> _lowPressureCompressors = testObject.lowPressureCompressors();

  RefrigerationTranscriticalSystem testObjectClone = testObject.clone(model).cast<RefrigerationTranscriticalSystem>();

  EXPECT_FALSE(testObjectClone.mediumTemperatureSuctionPipingZone());
  EXPECT_FALSE(testObjectClone.lowTemperatureSuctionPipingZone());
  std::vector<RefrigerationCase> mediumTemperatureCasesClone = testObjectClone.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureCasesClone.size());
  EXPECT_NE(mediumTemperatureCasesClone[0].handle(), _mediumTemperatureCases[0].handle());
  std::vector<RefrigerationCase> lowTemperatureCasesClone = testObjectClone.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureCasesClone.size());
  EXPECT_NE(lowTemperatureCasesClone[0].handle(), _lowTemperatureCases[0].handle());
  std::vector<RefrigerationWalkIn> mediumTemperatureWalkinsClone = testObjectClone.mediumTemperatureWalkins();
  EXPECT_EQ(1, mediumTemperatureWalkinsClone.size());
  EXPECT_NE(mediumTemperatureWalkinsClone[0].handle(), _mediumTemperatureWalkins[0].handle());
  std::vector<RefrigerationWalkIn> lowTemperatureWalkinsClone = testObjectClone.lowTemperatureWalkins();
  EXPECT_EQ(1, lowTemperatureWalkinsClone.size());
  EXPECT_NE(lowTemperatureWalkinsClone[0].handle(), _lowTemperatureWalkins[0].handle());
  std::vector<RefrigerationCompressor> highPressureCompressorsClone = testObjectClone.highPressureCompressors();
  EXPECT_EQ(1, highPressureCompressorsClone.size());
  EXPECT_NE(highPressureCompressorsClone[0].handle(), _highPressureCompressors[0].handle());
  std::vector<RefrigerationCompressor> lowPressureCompressorsClone = testObjectClone.lowPressureCompressors();
  EXPECT_EQ(1, lowPressureCompressorsClone.size());
  EXPECT_NE(lowPressureCompressorsClone[0].handle(), _lowPressureCompressors[0].handle());

  EXPECT_DOUBLE_EQ(999.0, testObjectClone.receiverPressure());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone.subcoolerEffectiveness());
  EXPECT_EQ("R410a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.sumUASuctionPipingforMediumTemperatureLoads());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.sumUASuctionPipingforLowTemperatureLoads());
  ASSERT_TRUE(testObjectClone.refrigerationGasCooler());
  EXPECT_NE(gasCooler, testObjectClone.refrigerationGasCooler().get());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_CloneTwoModelsWithDefaultData)
{
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  RefrigerationTranscriticalSystem testObjectClone = testObject.clone(model).cast<RefrigerationTranscriticalSystem>();

  Model model2;

  RefrigerationTranscriticalSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationTranscriticalSystem>();

  EXPECT_DOUBLE_EQ(3500000.0, testObjectClone2.receiverPressure());
  EXPECT_DOUBLE_EQ(0.4, testObjectClone2.subcoolerEffectiveness());
  EXPECT_EQ("R744", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.sumUASuctionPipingforMediumTemperatureLoads());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.sumUASuctionPipingforLowTemperatureLoads());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2, testObject);
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_CloneTwoModelWithCustomData)
{
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  ScheduleCompact wds(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor highPressureCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor lowPressureCompressor1 = RefrigerationCompressor(model);
  RefrigerationGasCoolerAirCooled gasCooler = RefrigerationGasCoolerAirCooled(model);

  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  testObject.setMediumTemperatureSuctionPipingZone(thermalZone);
  testObject.setLowTemperatureSuctionPipingZone(thermalZone);
  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addLowTemperatureCase(case2);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.addHighPressureCompressor(highPressureCompressor1);
  testObject.addLowPressureCompressor(lowPressureCompressor1);
  testObject.setRefrigerationGasCooler(gasCooler);

  testObject.setReceiverPressure(999.0);
  testObject.setSubcoolerEffectiveness(0.99);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSumUASuctionPipingforMediumTemperatureLoads(999.0);
  testObject.setSumUASuctionPipingforLowTemperatureLoads(999.0);

  std::vector<RefrigerationCase> _mediumTemperatureCases = testObject.mediumTemperatureCases();
  std::vector<RefrigerationWalkIn> _mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  std::vector<RefrigerationCase> _lowTemperatureCases = testObject.lowTemperatureCases();
  std::vector<RefrigerationWalkIn> _lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  std::vector<RefrigerationCompressor> _highPressureCompressors = testObject.highPressureCompressors();
  std::vector<RefrigerationCompressor> _lowPressureCompressors = testObject.lowPressureCompressors();

  RefrigerationTranscriticalSystem testObjectClone = testObject.clone(model).cast<RefrigerationTranscriticalSystem>();

  Model model2;
  RefrigerationTranscriticalSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationTranscriticalSystem>();

  EXPECT_FALSE(testObjectClone2.mediumTemperatureSuctionPipingZone());
  EXPECT_FALSE(testObjectClone2.lowTemperatureSuctionPipingZone());
  std::vector<RefrigerationCase> mediumTemperatureCasesClone = testObjectClone2.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureCasesClone.size());
  EXPECT_NE(mediumTemperatureCasesClone[0].handle(), _mediumTemperatureCases[0].handle());
  std::vector<RefrigerationCase> lowTemperatureCasesClone = testObjectClone2.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureCasesClone.size());
  EXPECT_NE(lowTemperatureCasesClone[0].handle(), _lowTemperatureCases[0].handle());
  std::vector<RefrigerationWalkIn> mediumTemperatureWalkinsClone = testObjectClone2.mediumTemperatureWalkins();
  EXPECT_EQ(1, mediumTemperatureWalkinsClone.size());
  EXPECT_NE(mediumTemperatureWalkinsClone[0].handle(), _mediumTemperatureWalkins[0].handle());
  std::vector<RefrigerationWalkIn> lowTemperatureWalkinsClone = testObjectClone2.lowTemperatureWalkins();
  EXPECT_EQ(1, lowTemperatureWalkinsClone.size());
  EXPECT_NE(lowTemperatureWalkinsClone[0].handle(), _lowTemperatureWalkins[0].handle());
  std::vector<RefrigerationCompressor> highPressureCompressorsClone = testObjectClone2.highPressureCompressors();
  EXPECT_EQ(1, highPressureCompressorsClone.size());
  EXPECT_NE(highPressureCompressorsClone[0].handle(), _highPressureCompressors[0].handle());
  std::vector<RefrigerationCompressor> lowPressureCompressorsClone = testObjectClone2.lowPressureCompressors();
  EXPECT_EQ(1, lowPressureCompressorsClone.size());
  EXPECT_NE(lowPressureCompressorsClone[0].handle(), _lowPressureCompressors[0].handle());

  ASSERT_TRUE(testObjectClone2.refrigerationGasCooler());
  EXPECT_NE(gasCooler, testObjectClone2.refrigerationGasCooler().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.receiverPressure());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone2.subcoolerEffectiveness());
  EXPECT_EQ("R410a", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.sumUASuctionPipingforMediumTemperatureLoads());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.sumUASuctionPipingforLowTemperatureLoads());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2, testObject);
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RefrigerationGasCoolerAirCooled)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationGasCoolerAirCooled gasCooler = RefrigerationGasCoolerAirCooled(model);

  EXPECT_FALSE(testObject.refrigerationGasCooler());
  EXPECT_TRUE(testObject.setRefrigerationGasCooler(gasCooler));
  EXPECT_TRUE(testObject.refrigerationGasCooler());

  std::vector<RefrigerationGasCoolerAirCooled> testGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(1, testGasCoolers.size());

  RefrigerationTranscriticalSystem testObjectClone = testObject.clone(model).cast<RefrigerationTranscriticalSystem>();
  EXPECT_NE(testObjectClone.refrigerationGasCooler().get().handle(), gasCooler.handle());

  testGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(2, testGasCoolers.size());

  Model model2;
  RefrigerationTranscriticalSystem testObjectClone2 = testObject.clone(model2).cast<RefrigerationTranscriticalSystem>();

  testGasCoolers = model2.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(1, testGasCoolers.size());

  EXPECT_NE(testObjectClone2.refrigerationGasCooler().get().handle(), gasCooler.handle());

  gasCooler.remove();

  EXPECT_FALSE(testObject.refrigerationGasCooler());
  EXPECT_TRUE(testObjectClone.refrigerationGasCooler());
  EXPECT_TRUE(testObjectClone2.refrigerationGasCooler());

  testGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(1, testGasCoolers.size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_HighPressureCompressors)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  std::vector<RefrigerationCompressor> highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_TRUE(highPressureCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddHighPressureCompressor)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addHighPressureCompressor(testCompressor));

  std::vector<RefrigerationCompressor> highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_EQ(1, highPressureCompressors.size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveHighPressureCompressor)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighPressureCompressor(testCompressor1);
  testObject.addHighPressureCompressor(testCompressor2);
  testObject.removeHighPressureCompressor(testCompressor1);

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_EQ(1, highPressureCompressors.size());
  EXPECT_EQ(testCompressor2.handle(), highPressureCompressors[0].handle());

  testObject.removeHighPressureCompressor(testCompressor2);
  testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(highPressureCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllHighPressureCompressors)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighPressureCompressor(testCompressor1);
  testObject.addHighPressureCompressor(testCompressor2);
  testObject.removeAllHighPressureCompressors();

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_TRUE(highPressureCompressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->highPressureCompressorList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_LowPressureCompressors)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  std::vector<RefrigerationCompressor> highPressureCompressors = testObject.highPressureCompressors();
  EXPECT_TRUE(highPressureCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddLowPressureCompressor)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addLowPressureCompressor(testCompressor));

  std::vector<RefrigerationCompressor> lowPressureCompressors = testObject.lowPressureCompressors();
  EXPECT_EQ(1, lowPressureCompressors.size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveLowPressureCompressor)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addLowPressureCompressor(testCompressor1);
  testObject.addLowPressureCompressor(testCompressor2);
  testObject.removeLowPressureCompressor(testCompressor1);

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> lowPressureCompressors = testObject.lowPressureCompressors();
  EXPECT_EQ(1, lowPressureCompressors.size());
  EXPECT_EQ(testCompressor2.handle(), lowPressureCompressors[0].handle());

  testObject.removeLowPressureCompressor(testCompressor2);
  testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  lowPressureCompressors = testObject.lowPressureCompressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(lowPressureCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllLowPressureCompressors)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addLowPressureCompressor(testCompressor1);
  testObject.addLowPressureCompressor(testCompressor2);
  testObject.removeAllLowPressureCompressors();

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> lowPressureCompressors = testObject.lowPressureCompressors();
  EXPECT_TRUE(lowPressureCompressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowPressureCompressorList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_MediumTemperatureCases)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_TRUE(mediumTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addMediumTemperatureCase(case1);
  mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureCases.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddMediumTemperatureCase)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  EXPECT_TRUE(testObject.addMediumTemperatureCase(case1));
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveMediumTemperatureCase)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureCase(case2);
  testObject.removeMediumTemperatureCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureCases.size());
  EXPECT_EQ(case2.handle(), mediumTemperatureCases[0].handle());

  testObject.removeMediumTemperatureCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(mediumTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllMediumTemperatureCases)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureCase(case2);
  testObject.removeAllMediumTemperatureCases();

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_TRUE(mediumTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_LowTemperatureCases)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_TRUE(lowTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addLowTemperatureCase(case1);
  lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureCases.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddLowTemperatureCase)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  EXPECT_TRUE(testObject.addLowTemperatureCase(case1));
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveLowTemperatureCase)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addLowTemperatureCase(case1);
  testObject.addLowTemperatureCase(case2);
  testObject.removeLowTemperatureCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureCases.size());
  EXPECT_EQ(case2.handle(), lowTemperatureCases[0].handle());

  testObject.removeLowTemperatureCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(lowTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllLowTemperatureCases)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);
  
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addLowTemperatureCase(case1);
  testObject.addLowTemperatureCase(case2);
  testObject.removeAllLowTemperatureCases();

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_TRUE(lowTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_MediumTemperatureWalkins)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_TRUE(mediumTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addMediumTemperatureWalkin(walkin1);
  mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_EQ(1, mediumTemperatureWalkins.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddMediumTemperatureWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  EXPECT_TRUE(testObject.addMediumTemperatureWalkin(walkin1));
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveMediumTemperatureWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addMediumTemperatureWalkin(walkin2);
  testObject.removeMediumTemperatureWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_EQ(1, mediumTemperatureWalkins.size());
  EXPECT_EQ(walkin2.handle(), mediumTemperatureWalkins[0].handle());

  testObject.removeMediumTemperatureWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(mediumTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllMediumTemperatureWalkIns)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addMediumTemperatureWalkin(walkin2);
  testObject.removeAllMediumTemperatureWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_TRUE(mediumTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_LowTemperatureWalkins)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_TRUE(lowTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addLowTemperatureWalkin(walkin1);
  lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_EQ(1, lowTemperatureWalkins.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_AddLowTemperatureWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  EXPECT_TRUE(testObject.addLowTemperatureWalkin(walkin1));
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveLowTemperatureWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addLowTemperatureWalkin(walkin1);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.removeLowTemperatureWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_EQ(1, lowTemperatureWalkins.size());
  EXPECT_EQ(walkin2.handle(), lowTemperatureWalkins[0].handle());

  testObject.removeLowTemperatureWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(lowTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllLowTemperatureWalkIns)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addLowTemperatureWalkin(walkin1);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.removeAllLowTemperatureWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_TRUE(lowTemperatureWalkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_MediumTemperatureCasesAndWalkins)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_TRUE(mediumTemperatureWalkins.empty());
  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_TRUE(mediumTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addMediumTemperatureWalkin(walkin1);
  mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  testObject.addMediumTemperatureCase(case1);
  mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureWalkins.size());
  EXPECT_EQ(1, mediumTemperatureCases.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveMediumTemperatureCaseAndWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addMediumTemperatureWalkin(walkin2);
  testObject.removeMediumTemperatureWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_EQ(1, mediumTemperatureWalkins.size());
  EXPECT_EQ(walkin2.handle(), mediumTemperatureWalkins[0].handle());

  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureCase(case2);
  testObject.removeMediumTemperatureCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(1, mediumTemperatureCases.size());
  EXPECT_EQ(case2.handle(), mediumTemperatureCases[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeMediumTemperatureWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(mediumTemperatureWalkins.empty());

  testObject.removeMediumTemperatureCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(mediumTemperatureCases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllMediumTemperatureCasesAndWalkIns)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addMediumTemperatureWalkin(walkin1);
  testObject.addMediumTemperatureWalkin(walkin2);
  testObject.addMediumTemperatureCase(case1);
  testObject.addMediumTemperatureCase(case2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllMediumTemperatureCases();
  testObject.removeAllMediumTemperatureWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins = testObject.mediumTemperatureWalkins();
  EXPECT_TRUE(mediumTemperatureWalkins.empty());
  std::vector<RefrigerationCase> mediumTemperatureCases = testObject.mediumTemperatureCases();
  EXPECT_TRUE(mediumTemperatureCases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_LowTemperatureCasesAndWalkins)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_TRUE(lowTemperatureWalkins.empty());
  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_TRUE(lowTemperatureCases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());

  testObject.addLowTemperatureWalkin(walkin1);
  lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  testObject.addLowTemperatureCase(case1);
  lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureWalkins.size());
  EXPECT_EQ(1, lowTemperatureCases.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveLowTemperatureCaseAndWalkIn)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addLowTemperatureWalkin(walkin1);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.removeLowTemperatureWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_EQ(1, lowTemperatureWalkins.size());
  EXPECT_EQ(walkin2.handle(), lowTemperatureWalkins[0].handle());

  testObject.addLowTemperatureCase(case1);
  testObject.addLowTemperatureCase(case2);
  testObject.removeLowTemperatureCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(1, lowTemperatureCases.size());
  EXPECT_EQ(case2.handle(), lowTemperatureCases[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeLowTemperatureWalkin(walkin2);
  testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(lowTemperatureWalkins.empty());

  testObject.removeLowTemperatureCase(case2);
  testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(lowTemperatureCases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RemoveAllLowTemperatureCasesAndWalkIns)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addLowTemperatureWalkin(walkin1);
  testObject.addLowTemperatureWalkin(walkin2);
  testObject.addLowTemperatureCase(case1);
  testObject.addLowTemperatureCase(case2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllLowTemperatureCases();
  testObject.removeAllLowTemperatureWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins = testObject.lowTemperatureWalkins();
  EXPECT_TRUE(lowTemperatureWalkins.empty());
  std::vector<RefrigerationCase> lowTemperatureCases = testObject.lowTemperatureCases();
  EXPECT_TRUE(lowTemperatureCases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureRefrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationTranscriticalSystem_RefrigerationTranscriticalSystemWorkingFluidType)
{
  Model model;
  RefrigerationTranscriticalSystem testObject = RefrigerationTranscriticalSystem(model);

  EXPECT_EQ("R744", testObject.refrigerationSystemWorkingFluidType());
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R410a"));
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R22"));
  EXPECT_FALSE(testObject.setRefrigerationSystemWorkingFluidType("Not Valid"));
}

