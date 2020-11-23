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

#include "../ShadowCalculation.hpp"
#include "../ShadowCalculation_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ShadowCalculation) {
  Model model;

  ShadowCalculation sc = model.getUniqueModelObject<ShadowCalculation>();

  // Shading Calculation Update Frequency Method
  EXPECT_TRUE(sc.isShadingCalculationUpdateFrequencyMethodDefaulted());
  EXPECT_EQ("Periodic", sc.shadingCalculationUpdateFrequencyMethod());
  EXPECT_TRUE(sc.setShadingCalculationUpdateFrequencyMethod("Timestep"));
  EXPECT_FALSE(sc.isShadingCalculationUpdateFrequencyMethodDefaulted());
  EXPECT_EQ("Timestep", sc.shadingCalculationUpdateFrequencyMethod());
  EXPECT_FALSE(sc.setShadingCalculationUpdateFrequencyMethod("BADENUM"));
  EXPECT_EQ("Timestep", sc.shadingCalculationUpdateFrequencyMethod());
  sc.resetShadingCalculationUpdateFrequencyMethod();
  EXPECT_TRUE(sc.isShadingCalculationUpdateFrequencyMethodDefaulted());
  EXPECT_EQ("Periodic", sc.shadingCalculationUpdateFrequencyMethod());

  // Shading Calculation Update Frequency
  EXPECT_FALSE(sc.isShadingCalculationUpdateFrequencyDefaulted());  // set in constructor
  EXPECT_EQ(20, sc.shadingCalculationUpdateFrequency());
  EXPECT_TRUE(sc.setShadingCalculationUpdateFrequency(1));
  EXPECT_EQ(1, sc.shadingCalculationUpdateFrequency());

  // Maximum Figures in Shadow Overlap Calculations
  EXPECT_FALSE(sc.isMaximumFiguresInShadowOverlapCalculationsDefaulted());  // set in constructor
  EXPECT_EQ(15000, sc.maximumFiguresInShadowOverlapCalculations());
  EXPECT_TRUE(sc.setMaximumFiguresInShadowOverlapCalculations(200));
  EXPECT_EQ(200, sc.maximumFiguresInShadowOverlapCalculations());

  // Polygon Clipping Algorithm
  EXPECT_EQ("SutherlandHodgman", sc.polygonClippingAlgorithm());
  EXPECT_TRUE(sc.setPolygonClippingAlgorithm("ConvexWeilerAtherton"));
  EXPECT_EQ("ConvexWeilerAtherton", sc.polygonClippingAlgorithm());
  EXPECT_FALSE(sc.setPolygonClippingAlgorithm("BADENUM"));
  EXPECT_EQ("ConvexWeilerAtherton", sc.polygonClippingAlgorithm());

  // Sky Diffuse Modeling Algorithm
  EXPECT_EQ("SimpleSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm());
  EXPECT_TRUE(sc.setSkyDiffuseModelingAlgorithm("DetailedSkyDiffuseModeling"));
  EXPECT_EQ("DetailedSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm());
  EXPECT_FALSE(sc.setSkyDiffuseModelingAlgorithm("BADENUM"));
  EXPECT_EQ("DetailedSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm());

  // NEW FIELDS: ALL REQUIRED

  // Shading Calculation Method: set in Ctor
  EXPECT_EQ("PolygonClipping", sc.shadingCalculationMethod());
  EXPECT_TRUE(sc.setShadingCalculationMethod("PixelCounting"));
  EXPECT_EQ("PixelCounting", sc.shadingCalculationMethod());
  EXPECT_FALSE(sc.setShadingCalculationMethod("BADENUM"));
  EXPECT_EQ("PixelCounting", sc.shadingCalculationMethod());

  // Pixel Counting Resolution
  EXPECT_EQ(512, sc.pixelCountingResolution());
  EXPECT_TRUE(sc.setPixelCountingResolution(499));
  EXPECT_EQ(499, sc.pixelCountingResolution());

  // Output External Shading Calculation Results
  EXPECT_FALSE(sc.outputExternalShadingCalculationResults());
  EXPECT_TRUE(sc.setOutputExternalShadingCalculationResults(true));
  EXPECT_TRUE(sc.outputExternalShadingCalculationResults());

  // Disable Self-Shading Within Shading Zone Groups
  EXPECT_FALSE(sc.disableSelfShadingWithinShadingZoneGroups());
  EXPECT_TRUE(sc.setDisableSelfShadingWithinShadingZoneGroups(true));
  EXPECT_TRUE(sc.disableSelfShadingWithinShadingZoneGroups());

  // Disable Self-Shading From Shading Zone Groups to Other Zones
  EXPECT_FALSE(sc.disableSelfShadingFromShadingZoneGroupstoOtherZones());
  EXPECT_TRUE(sc.setDisableSelfShadingFromShadingZoneGroupstoOtherZones(true));
  EXPECT_TRUE(sc.disableSelfShadingFromShadingZoneGroupstoOtherZones());
}

TEST_F(ModelFixture, ShadowCalculation_ShadingZoneGroup) {
  Model model;

  ShadowCalculation sc = model.getUniqueModelObject<ShadowCalculation>();

  ThermalZone z1(model);
  ThermalZone z2(model);
  ThermalZone z3(model);

  EXPECT_EQ(0, sc.numberofShadingZoneGroups());
  EXPECT_FALSE(sc.removeShadingZoneGroup(0));
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());

  sc.addShadingZoneGroup({z1, z2});
  EXPECT_EQ(1, sc.numberofShadingZoneGroups());
  std::vector<ThermalZone> zones = sc.getShadingZoneGroup(0);
  ASSERT_EQ(2, zones.size());
  EXPECT_EQ(z1, zones[0]);
  EXPECT_EQ(z2, zones[1]);
  EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());

  sc.addShadingZoneGroup({z3});
  EXPECT_EQ(2, sc.numberofShadingZoneGroups());
  zones = sc.getShadingZoneGroup(0);
  ASSERT_EQ(2, zones.size());
  EXPECT_EQ(z1, zones[0]);
  EXPECT_EQ(z2, zones[1]);
  zones = sc.getShadingZoneGroup(1);
  ASSERT_EQ(1, zones.size());
  EXPECT_EQ(z3, zones[0]);
  EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());

  EXPECT_TRUE(sc.removeShadingZoneGroup(0));
  EXPECT_EQ(1, sc.numberofShadingZoneGroups());
  zones = sc.getShadingZoneGroup(0);
  ASSERT_EQ(1, zones.size());
  EXPECT_EQ(z3, zones[0]);
  EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());

  sc.addShadingZoneGroup({z1, z2, z3});  // a zone can be added in two groups right now...
  EXPECT_EQ(2, sc.numberofShadingZoneGroups());
  zones = sc.getShadingZoneGroup(0);
  ASSERT_EQ(1, zones.size());
  EXPECT_EQ(z3, zones[0]);
  zones = sc.getShadingZoneGroup(1);
  ASSERT_EQ(3, zones.size());
  EXPECT_EQ(z1, zones[0]);
  EXPECT_EQ(z2, zones[1]);
  EXPECT_EQ(z3, zones[2]);
  EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());

  sc.removeAllShadingZoneGroups();
  EXPECT_EQ(0, sc.numberofShadingZoneGroups());
  // ModelObjectList will remove any object it lists, so ensure we do not delete ThermalZones...
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(3, model.getConcreteModelObjects<ThermalZone>().size());
}
