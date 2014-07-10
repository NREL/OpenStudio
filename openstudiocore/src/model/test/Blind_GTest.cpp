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

#include "../Blind.hpp"
#include "../Blind_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,Blind_SlatWidth_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatWidth(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setSlatWidth(testQ));
  Quantity q = blind.getSlatWidth(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatSeparation_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatSeparation(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setSlatSeparation(testQ));
  Quantity q = blind.getSlatSeparation(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatThickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatThickness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setSlatThickness(testQ));
  Quantity q = blind.getSlatThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatAngle_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatAngle(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setSlatAngle(testQ));
  Quantity q = blind.getSlatAngle(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatConductivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatConductivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setSlatConductivity(testQ));
  Quantity q = blind.getSlatConductivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatBeamSolarTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatBeamSolarTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setSlatBeamSolarTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setSlatBeamSolarTransmittance(testQ2));
  Quantity q2 = blind.getSlatBeamSolarTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());

}

TEST_F(ModelFixture,Blind_FrontSideSlatBeamSolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getFrontSideSlatBeamSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setFrontSideSlatBeamSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setFrontSideSlatBeamSolarReflectance(testQ2));
  Quantity q2 = blind.getFrontSideSlatBeamSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_BackSideSlatBeamSolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBackSideSlatBeamSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setBackSideSlatBeamSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setBackSideSlatBeamSolarReflectance(testQ2));
  Quantity q2 = blind.getBackSideSlatBeamSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatDiffuseSolarTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatDiffuseSolarTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setSlatDiffuseSolarTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setSlatDiffuseSolarTransmittance(testQ2));
  Quantity q2 = blind.getSlatDiffuseSolarTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_FrontSideSlatDiffuseSolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getFrontSideSlatDiffuseSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setFrontSideSlatDiffuseSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setFrontSideSlatDiffuseSolarReflectance(testQ2));
  Quantity q2 = blind.getFrontSideSlatDiffuseSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_BackSideSlatDiffuseSolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBackSideSlatDiffuseSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setBackSideSlatDiffuseSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setBackSideSlatDiffuseSolarReflectance(testQ2));
  Quantity q2 = blind.getBackSideSlatDiffuseSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatBeamVisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatBeamVisibleTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setSlatBeamVisibleTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setSlatBeamVisibleTransmittance(testQ2));
  Quantity q2 = blind.getSlatBeamVisibleTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_FrontSideSlatBeamVisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getFrontSideSlatBeamVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setFrontSideSlatBeamVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setFrontSideSlatBeamVisibleReflectance(testQ2));
  OSOptionalQuantity q = blind.getFrontSideSlatBeamVisibleReflectance(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BackSideSlatBeamVisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBackSideSlatBeamVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setBackSideSlatBeamVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setBackSideSlatBeamVisibleReflectance(testQ2));
  OSOptionalQuantity q = blind.getBackSideSlatBeamVisibleReflectance(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatDiffuseVisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getSlatDiffuseVisibleTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setSlatDiffuseVisibleTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setSlatDiffuseVisibleTransmittance(testQ2));
  Quantity q2 = blind.getSlatDiffuseVisibleTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_FrontSideSlatDiffuseVisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getFrontSideSlatDiffuseVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setFrontSideSlatDiffuseVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setFrontSideSlatDiffuseVisibleReflectance(testQ2));
  OSOptionalQuantity q = blind.getFrontSideSlatDiffuseVisibleReflectance(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BackSideSlatDiffuseVisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBackSideSlatDiffuseVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setBackSideSlatDiffuseVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setBackSideSlatDiffuseVisibleReflectance(testQ2));
  OSOptionalQuantity q = blind.getBackSideSlatDiffuseVisibleReflectance(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_SlatInfraredHemisphericalTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);  

  Unit units = blind.getSlatInfraredHemisphericalTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setSlatInfraredHemisphericalTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setSlatInfraredHemisphericalTransmittance(testQ2));
  Quantity q2 = blind.getSlatInfraredHemisphericalTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_FrontSideSlatInfraredHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getFrontSideSlatInfraredHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setFrontSideSlatInfraredHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setFrontSideSlatInfraredHemisphericalEmissivity(testQ2));
  Quantity q2 = blind.getFrontSideSlatInfraredHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_BackSideSlatInfraredHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBackSideSlatInfraredHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(blind.setBackSideSlatInfraredHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(blind.setBackSideSlatInfraredHemisphericalEmissivity(testQ2));
  Quantity q2 = blind.getBackSideSlatInfraredHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Blind_BlindtoGlassDistance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBlindtoGlassDistance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setBlindtoGlassDistance(testQ));
  Quantity q = blind.getBlindtoGlassDistance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BlindTopOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBlindTopOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setBlindTopOpeningMultiplier(testQ));
  Quantity q = blind.getBlindTopOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BlindBottomOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBlindBottomOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setBlindBottomOpeningMultiplier(testQ));
  Quantity q = blind.getBlindBottomOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BlindLeftSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBlindLeftSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setBlindLeftSideOpeningMultiplier(testQ));
  Quantity q = blind.getBlindLeftSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_BlindRightSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getBlindRightSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setBlindRightSideOpeningMultiplier(testQ));
  Quantity q = blind.getBlindRightSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_MinimumSlatAngle_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getMinimumSlatAngle(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setMinimumSlatAngle(testQ));
  Quantity q = blind.getMinimumSlatAngle(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Blind_MaximumSlatAngle_Quantity) {
  Model model;
  // TODO: Check constructor.
  Blind blind(model);

  Unit units = blind.getMaximumSlatAngle(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(blind.setMaximumSlatAngle(testQ));
  Quantity q = blind.getMaximumSlatAngle(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}
