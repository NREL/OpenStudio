/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkSurface.hpp"
#include "../AirflowNetworkSurface_Impl.hpp"
#include "../AirflowNetworkSpecifiedFlowRate.hpp"
#include "../AirflowNetworkSpecifiedFlowRate_Impl.hpp"
#include "../AirflowNetworkExternalNode.hpp"
#include "../AirflowNetworkExternalNode_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_SpecifiedFlowRate) {
  Model model;

  AirflowNetworkSpecifiedFlowRate sfr0(model, 10.0);
  EXPECT_EQ(10.0, sfr0.airFlowValue());
  EXPECT_EQ("MassFlow", sfr0.airFlowUnits());
  EXPECT_TRUE(sfr0.isAirFlowUnitsDefaulted());

  EXPECT_TRUE(sfr0.setAirFlowValue(5.0));
  EXPECT_TRUE(sfr0.setAirFlowUnits("VolumetricFlow"));

  EXPECT_EQ(5.0, sfr0.airFlowValue());
  EXPECT_EQ("VolumetricFlow", sfr0.airFlowUnits());
  EXPECT_FALSE(sfr0.isAirFlowUnitsDefaulted());

  sfr0.resetAirFlowUnits();

  EXPECT_TRUE(sfr0.isAirFlowUnitsDefaulted());

  Point3dVector points;

  // square with unit area
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  Surface surface(points, model);
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_TRUE(!surface.airflowNetworkSurface());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  AirflowNetworkSurface afnsurf = surface.getAirflowNetworkSurface(sfr0);
  EXPECT_EQ(surface, afnsurf.surface());
  boost::optional<AirflowNetworkSurface> optsurf = surface.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());
  ASSERT_TRUE(afnsurf.leakageComponent());
  EXPECT_EQ(sfr0, afnsurf.leakageComponent().get());

  AirflowNetworkSpecifiedFlowRate sfr1(model, 12.0);
  EXPECT_EQ(12.0, sfr1.airFlowValue());
  EXPECT_EQ("MassFlow", sfr1.airFlowUnits());
  EXPECT_TRUE(sfr1.isAirFlowUnitsDefaulted());

  ASSERT_TRUE(afnsurf.setLeakageComponent(sfr1));
  ASSERT_TRUE(afnsurf.leakageComponent());
  EXPECT_EQ(sfr1, afnsurf.leakageComponent().get());

  EXPECT_FALSE(afnsurf.externalNode());

  AirflowNetworkExternalNode extnode(model);
  ASSERT_TRUE(afnsurf.setExternalNode(extnode));
  EXPECT_EQ(extnode, afnsurf.externalNode().get());
}

TEST_F(ModelFixture, AirflowNetwork_SpecifiedFlowRate_AlternateCtor) {
  Model model;

  AirflowNetworkSpecifiedFlowRate sfr0(model);
  EXPECT_EQ(0.0, sfr0.airFlowValue());
  EXPECT_EQ("MassFlow", sfr0.airFlowUnits());
  EXPECT_TRUE(sfr0.isAirFlowUnitsDefaulted());
}
