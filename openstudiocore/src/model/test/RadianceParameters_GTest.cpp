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

#include "../RadianceParameters.hpp"
#include "../RadianceParameters_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RadianceParameters_AccumulatedRaysperRecord) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.accumulatedRaysperRecord();
  EXPECT_EQ(1, value);

  EXPECT_TRUE(radianceParameters.setAccumulatedRaysperRecord(2));
  value = radianceParameters.accumulatedRaysperRecord();
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture,RadianceParameters_DirectThreshold_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getDirectThreshold(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setDirectThreshold(testQ));
  Quantity q = radianceParameters.getDirectThreshold(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_DirectCertainty_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getDirectCertainty(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setDirectCertainty(testQ));
  Quantity q = radianceParameters.getDirectCertainty(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_DirectJitter_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getDirectJitter(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setDirectJitter(testQ));
  Quantity q = radianceParameters.getDirectJitter(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_DirectPretest_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getDirectPretest(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setDirectPretest(testQ));
  Quantity q = radianceParameters.getDirectPretest(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_AmbientBouncesVMX) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.ambientBouncesVMX();
  EXPECT_EQ(10, value);

  EXPECT_TRUE(radianceParameters.setAmbientBouncesVMX(2));
  value = radianceParameters.ambientBouncesVMX();
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture,RadianceParameters_AmbientDivisionsVMX) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.ambientDivisionsVMX(); 
  EXPECT_EQ(65536, value);

  EXPECT_TRUE(radianceParameters.setAmbientDivisionsVMX(2));
  value = radianceParameters.ambientDivisionsVMX(); 
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture,RadianceParameters_AmbientSupersamples_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.ambientSupersamples();
  EXPECT_EQ(256, value);

  EXPECT_TRUE(radianceParameters.setAmbientSupersamples(2));
  value = radianceParameters.ambientSupersamples();
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture,RadianceParameters_LimitWeightVMX_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getLimitWeightVMX(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setLimitWeightVMX(testQ));
  Quantity q = radianceParameters.getLimitWeightVMX(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_LimitWeightDMX_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  Unit units = radianceParameters.getLimitWeightDMX(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(radianceParameters.setLimitWeightDMX(testQ));
  Quantity q = radianceParameters.getLimitWeightDMX(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RadianceParameters_KlemsSamplingDensity_Quantity) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.klemsSamplingDensity();
  EXPECT_EQ(1000, value);

  EXPECT_TRUE(radianceParameters.setKlemsSamplingDensity(2));
  value = radianceParameters.klemsSamplingDensity();
  EXPECT_EQ(2, value);
}


