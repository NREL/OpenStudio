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

