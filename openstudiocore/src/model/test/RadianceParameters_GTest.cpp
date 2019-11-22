/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  EXPECT_EQ(6, value);

  EXPECT_TRUE(radianceParameters.setAmbientBouncesVMX(2));
  value = radianceParameters.ambientBouncesVMX();
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture,RadianceParameters_AmbientDivisionsVMX) {
  Model model;
  // TODO: Check constructor.
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  int value = radianceParameters.ambientDivisionsVMX();
  EXPECT_EQ(4050, value);

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
  EXPECT_EQ(500, value);

  EXPECT_TRUE(radianceParameters.setKlemsSamplingDensity(2));
  value = radianceParameters.klemsSamplingDensity();
  EXPECT_EQ(2, value);
}

TEST_F(ModelFixture, RadianceParameters_FineAndCoarseSettings) {
  Model model;

  auto radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  // Test CTOR setting coarse
  EXPECT_TRUE(radianceParameters.isCoarseSettings());

  radianceParameters.applyFineSettings();
  EXPECT_TRUE(radianceParameters.isFineSettings());

  radianceParameters.setSkyDiscretizationResolution("578");
  EXPECT_EQ(radianceParameters.skyDiscretizationResolution(), "578");
  EXPECT_FALSE(radianceParameters.isCoarseSettings());
  EXPECT_FALSE(radianceParameters.isFineSettings());
}

