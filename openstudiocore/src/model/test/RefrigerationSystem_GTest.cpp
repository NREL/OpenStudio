/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/RefrigerationSystem.hpp>
#include <model/RefrigerationSystem_Impl.hpp>
#include <model/RefrigerationCompressor.hpp>
#include <model/RefrigerationCompressor_Impl.hpp>
#include <model/RefrigerationCondenserAirCooled.hpp>
#include <model/RefrigerationCondenserAirCooled_Impl.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_Impl.hpp>
#include <model/ModelObjectList.hpp>
#include <model/ModelObjectList_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>

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

  std::vector<RefrigerationSystem> refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
  EXPECT_EQ(1, refrigerationSystems.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(2, refrigerationModelObjectLists.size());

  testObject.remove();

  refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
  EXPECT_EQ(0, refrigerationSystems.size());

  refrigerationModelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());
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
  RefrigerationSystem testObject = RefrigerationSystem(model);
  testObject.setMinimumCondensingTemperature(999.0);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSuctionTemperatureControlType("FloatSuctionTemperature");
  testObject.setSumUASuctionPiping(999.0);
  testObject.setSuctionPipingZone(thermalZone);

  RefrigerationSystem testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumCondensingTemperature());
  EXPECT_FALSE(testObjectClone.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.sumUASuctionPiping());
  EXPECT_EQ("R410a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("FloatSuctionTemperature", testObjectClone.suctionTemperatureControlType());
  EXPECT_FALSE(testObjectClone.suctionPipingZone());
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

TEST_F(ModelFixture, RefrigerationSystem_Cases)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  ScheduleCompact s2(model);
  ScheduleCompact s3(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1, s2, s3);

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
  ScheduleCompact s2(model);
  ScheduleCompact s3(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1, s2, s3);

  EXPECT_TRUE(testObject.addCase(case1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCase)
{
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  ScheduleCompact s2(model);
  ScheduleCompact s3(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1, s2, s3);
  RefrigerationCase case2 = RefrigerationCase(model, s1, s2, s3);

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
  ScheduleCompact s2(model);
  ScheduleCompact s3(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1, s2, s3);
  RefrigerationCase case2 = RefrigerationCase(model, s1, s2, s3);

  testObject.addCase(case1);
  testObject.addCase(case2);
  testObject.removeAllCases();

  std::vector<RefrigerationCase> testRefrigerationCases = model.getModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
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
