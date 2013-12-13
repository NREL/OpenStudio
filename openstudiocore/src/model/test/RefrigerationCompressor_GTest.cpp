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

#include <model/RefrigerationCompressor.hpp>
#include <model/RefrigerationCompressor_Impl.hpp>
#include <model/CurveBicubic.hpp>
#include <model/CurveBicubic_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCompressor_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationCompressor testObject = RefrigerationCompressor(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationCompressor_Remove)
{
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);

  std::vector<RefrigerationCompressor> refrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(1, refrigerationCompressors.size());

  testObject.remove();

  refrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(0, refrigerationCompressors.size());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneOneModelWithDefaultData)
{
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();

  std::vector<CurveBicubic> refrigerationCompressorCurves = model.getModelObjects<CurveBicubic>();
  for(std::vector<CurveBicubic>::iterator it = refrigerationCompressorCurves.begin(); it != refrigerationCompressorCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(18.3, testObjectClone.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ratedSubcooling().get());
  EXPECT_NE(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_NE(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneOneModelWithCustomData)
{
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);
  testObject.setRatedReturnGasTemperature(999.0);
  testObject.setRatedSubcooling(999.0);

  CurveBicubic powerCurve = CurveBicubic(model);
  powerCurve.setName("Refrigeration Compressor Power Curve Test");
  powerCurve.setCoefficient1Constant(4451.46);
  powerCurve.setCoefficient2x(-166.108);
  powerCurve.setCoefficient3xPOW2(-5.35664);
  powerCurve.setCoefficient4y(263.553);
  powerCurve.setCoefficient5yPOW2(-0.958849);
  powerCurve.setCoefficient6xTIMESY(8.09252);
  powerCurve.setCoefficient7xPOW3(-0.0239354);
  powerCurve.setCoefficient8yPOW3(0.000633306);
  powerCurve.setCoefficient9xPOW2TIMESY(0.0778798);
  powerCurve.setCoefficient10xTIMESYPOW2(-0.0167386);
  powerCurve.setMinimumValueofx(-23.3);
  powerCurve.setMaximumValueofx(7.2);
  powerCurve.setMinimumValueofy(10.0);
  powerCurve.setMaximumValueofy(60.0);
  powerCurve.setInputUnitTypeforX("Temperature");
  powerCurve.setInputUnitTypeforY("Temperature");
  powerCurve.setOutputUnitType("Power");

  testObject.setRefrigerationCompressorPowerCurve(powerCurve);

  CurveBicubic capacityCurve = CurveBicubic(model);
  capacityCurve.setName("Refrigeration Compressor Capacity Curve Test");
  capacityCurve.setCoefficient1Constant(83249.9);
  capacityCurve.setCoefficient2x(3147.96);
  capacityCurve.setCoefficient3xPOW2(44.9276);
  capacityCurve.setCoefficient4y(-1035.51);
  capacityCurve.setCoefficient5yPOW2(3.74692);
  capacityCurve.setCoefficient6xTIMESY(-37.3907);
  capacityCurve.setCoefficient7xPOW3(0.179897);
  capacityCurve.setCoefficient8yPOW3(-0.0199971);
  capacityCurve.setCoefficient9xPOW2TIMESY(-0.481641);
  capacityCurve.setCoefficient10xTIMESYPOW2(0.0428673);
  capacityCurve.setMinimumValueofx(-23.3);
  capacityCurve.setMaximumValueofx(7.2);
  capacityCurve.setMinimumValueofy(10.0);
  capacityCurve.setMaximumValueofy(60.0);
  capacityCurve.setInputUnitTypeforX("Temperature");
  capacityCurve.setInputUnitTypeforY("Temperature");
  capacityCurve.setOutputUnitType("Capacity");
  
  testObject.setRefrigerationCompressorCapacityCurve(capacityCurve);

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedSubcooling().get());
  EXPECT_NE(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_NE(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
  EXPECT_NE(powerCurve.handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_NE(capacityCurve.handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneTwoModelsWithDefaultData)
{
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();

  Model model2;

  std::vector<CurveBicubic> refrigerationCompressorCurves2 = model2.getModelObjects<CurveBicubic>();
  EXPECT_EQ(0, refrigerationCompressorCurves2.size());

  RefrigerationCompressor testObjectClone2 = testObject.clone(model2).cast<RefrigerationCompressor>();

  std::vector<CurveBicubic> refrigerationCompressorCurves = model.getModelObjects<CurveBicubic>();
  EXPECT_EQ(4, refrigerationCompressorCurves.size());

  refrigerationCompressorCurves2 = model2.getModelObjects<CurveBicubic>();
  EXPECT_EQ(2, refrigerationCompressorCurves2.size());

  for(std::vector<CurveBicubic>::iterator it = refrigerationCompressorCurves.begin(); it != refrigerationCompressorCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for(std::vector<CurveBicubic>::iterator it = refrigerationCompressorCurves2.begin(); it != refrigerationCompressorCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(18.3, testObjectClone2.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.ratedSubcooling().get());
  EXPECT_NE(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone2.refrigerationCompressorPowerCurve().handle());
  EXPECT_NE(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone2.refrigerationCompressorCapacityCurve().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
