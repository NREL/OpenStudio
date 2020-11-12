/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RadianceParameters_GettersSetters) {

  Model model;
  auto radianceParameters = model.getUniqueModelObject<RadianceParameters>();

  // Accumulated Rays per Record:  Integer
  // Check Idd default: 1
  EXPECT_EQ(1, radianceParameters.accumulatedRaysperRecord());
  EXPECT_TRUE(radianceParameters.setAccumulatedRaysperRecord(2));
  EXPECT_EQ(2, radianceParameters.accumulatedRaysperRecord());
  radianceParameters.resetAccumulatedRaysperRecord();
  EXPECT_EQ(1, radianceParameters.accumulatedRaysperRecord());

  // Direct Threshold:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, radianceParameters.directThreshold());
  EXPECT_TRUE(radianceParameters.setDirectThreshold(0.0));
  EXPECT_EQ(0.0, radianceParameters.directThreshold());
  radianceParameters.resetDirectThreshold();
  EXPECT_EQ(0.0, radianceParameters.directThreshold());

  // Direct Certainty:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, radianceParameters.directCertainty());
  EXPECT_TRUE(radianceParameters.setDirectCertainty(0.6));
  EXPECT_EQ(0.6, radianceParameters.directCertainty());
  radianceParameters.resetDirectCertainty();
  EXPECT_EQ(1.0, radianceParameters.directCertainty());

  // Direct Jitter:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, radianceParameters.directJitter());
  EXPECT_TRUE(radianceParameters.setDirectJitter(0.7));
  EXPECT_EQ(0.7, radianceParameters.directJitter());
  radianceParameters.resetDirectJitter();
  EXPECT_EQ(1.0, radianceParameters.directJitter());

  // Direct Pretest:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, radianceParameters.directPretest());
  EXPECT_TRUE(radianceParameters.setDirectPretest(0.8));
  EXPECT_EQ(0.8, radianceParameters.directPretest());
  radianceParameters.resetDirectPretest();
  EXPECT_EQ(1.0, radianceParameters.directPretest());

  // Ambient Bounces VMX:  Integer
  // Check Idd default: 6
  EXPECT_EQ(6, radianceParameters.ambientBouncesVMX());
  EXPECT_TRUE(radianceParameters.setAmbientBouncesVMX(3));
  EXPECT_EQ(3, radianceParameters.ambientBouncesVMX());
  radianceParameters.resetAmbientBouncesVMX();
  EXPECT_EQ(6, radianceParameters.ambientBouncesVMX());

  // Ambient Bounces DMX:  Integer
  // Check Idd default: 2
  EXPECT_EQ(2, radianceParameters.ambientBouncesDMX());
  EXPECT_TRUE(radianceParameters.setAmbientBouncesDMX(1));
  EXPECT_EQ(1, radianceParameters.ambientBouncesDMX());
  radianceParameters.resetAmbientBouncesDMX();
  EXPECT_EQ(2, radianceParameters.ambientBouncesDMX());

  // Ambient Divisions VMX:  Integer
  // Check Idd default: 4050
  EXPECT_EQ(4050, radianceParameters.ambientDivisionsVMX());
  EXPECT_TRUE(radianceParameters.setAmbientDivisionsVMX(2025));
  EXPECT_EQ(2025, radianceParameters.ambientDivisionsVMX());
  radianceParameters.resetAmbientDivisionsVMX();
  EXPECT_EQ(4050, radianceParameters.ambientDivisionsVMX());

  // Ambient Divisions DMX:  Integer
  // Check Idd default: 512
  EXPECT_EQ(512, radianceParameters.ambientDivisionsDMX());
  EXPECT_TRUE(radianceParameters.setAmbientDivisionsDMX(256));
  EXPECT_EQ(256, radianceParameters.ambientDivisionsDMX());
  radianceParameters.resetAmbientDivisionsDMX();
  EXPECT_EQ(512, radianceParameters.ambientDivisionsDMX());

  // Ambient Supersamples:  Integer
  // Check Idd default: 256
  EXPECT_EQ(256, radianceParameters.ambientSupersamples());
  EXPECT_TRUE(radianceParameters.setAmbientSupersamples(128));
  EXPECT_EQ(128, radianceParameters.ambientSupersamples());
  radianceParameters.resetAmbientSupersamples();
  EXPECT_EQ(256, radianceParameters.ambientSupersamples());

  // Limit Weight VMX:  Double
  // Check Idd default: 0.001
  EXPECT_EQ(0.001, radianceParameters.limitWeightVMX());
  EXPECT_TRUE(radianceParameters.setLimitWeightVMX(0.21));
  EXPECT_EQ(0.21, radianceParameters.limitWeightVMX());
  radianceParameters.resetLimitWeightVMX();
  EXPECT_EQ(0.001, radianceParameters.limitWeightVMX());

  // Limit Weight DMX:  Double
  // Check Idd default: 0.001
  EXPECT_EQ(0.001, radianceParameters.limitWeightDMX());
  EXPECT_TRUE(radianceParameters.setLimitWeightDMX(0.005));
  EXPECT_EQ(0.005, radianceParameters.limitWeightDMX());
  radianceParameters.resetLimitWeightDMX();
  EXPECT_EQ(0.001, radianceParameters.limitWeightDMX());

  // Klems Sampling Density:  Integer
  // Check Idd default: 500
  EXPECT_EQ(500, radianceParameters.klemsSamplingDensity());
  EXPECT_TRUE(radianceParameters.setKlemsSamplingDensity(499));
  EXPECT_EQ(499, radianceParameters.klemsSamplingDensity());
  radianceParameters.resetKlemsSamplingDensity();
  EXPECT_EQ(500, radianceParameters.klemsSamplingDensity());

  // Sky Discretization Resolution:  String
  // Check Idd default: "146"
  EXPECT_EQ("146", radianceParameters.skyDiscretizationResolution());
  // Test a valid choice
  EXPECT_TRUE(radianceParameters.setSkyDiscretizationResolution("578"));
  EXPECT_EQ("578", radianceParameters.skyDiscretizationResolution());
  // Test an invalid choice
  EXPECT_FALSE(radianceParameters.setSkyDiscretizationResolution("BadChoice"));
  EXPECT_EQ("578", radianceParameters.skyDiscretizationResolution());
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
