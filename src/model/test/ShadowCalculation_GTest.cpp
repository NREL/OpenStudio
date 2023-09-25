/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  auto sc = model.getUniqueModelObject<ShadowCalculation>();

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

  auto sc = model.getUniqueModelObject<ShadowCalculation>();

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
