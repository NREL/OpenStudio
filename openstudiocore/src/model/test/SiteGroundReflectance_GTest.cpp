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

#include "../SiteGroundReflectance.hpp"
#include "../SiteGroundReflectance_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SiteGroundReflectance_JanuaryGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getJanuaryGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setJanuaryGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getJanuaryGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_FebruaryGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getFebruaryGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setFebruaryGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getFebruaryGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_MarchGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getMarchGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setMarchGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getMarchGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_AprilGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getAprilGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setAprilGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getAprilGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_MayGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getMayGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setMayGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getMayGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_JuneGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getJuneGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setJuneGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getJuneGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_JulyGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getJulyGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setJulyGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getJulyGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_AugustGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getAugustGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setAugustGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getAugustGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_SeptemberGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getSeptemberGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setSeptemberGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getSeptemberGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_OctoberGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getOctoberGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setOctoberGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getOctoberGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_NovemberGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getNovemberGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setNovemberGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getNovemberGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SiteGroundReflectance_DecemberGroundReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SiteGroundReflectance siteGroundReflectance(model);

  Unit units = siteGroundReflectance.getDecemberGroundReflectance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(siteGroundReflectance.setDecemberGroundReflectance(testQ));
  Quantity q = siteGroundReflectance.getDecemberGroundReflectance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

