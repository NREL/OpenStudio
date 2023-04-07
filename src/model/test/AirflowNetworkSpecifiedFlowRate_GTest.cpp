/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
