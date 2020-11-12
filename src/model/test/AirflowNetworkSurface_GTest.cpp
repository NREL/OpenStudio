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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ResourceObject.hpp"
#include "../ResourceObject_Impl.hpp"
#include "../ComponentData.hpp"
#include "../ComponentData_Impl.hpp"
#include "../ConstructionBase.hpp"
#include "../ConstructionBase_Impl.hpp"
#include "../LayeredConstruction.hpp"
#include "../LayeredConstruction_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../Material.hpp"
#include "../Material_Impl.hpp"
#include "../AirGap.hpp"
#include "../AirGap_Impl.hpp"
#include "../AirWallMaterial.hpp"
#include "../AirWallMaterial_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../DaylightingDeviceShelf.hpp"
#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../SurfacePropertyOtherSideCoefficients.hpp"
#include "../SurfacePropertyOtherSideConditionsModel.hpp"

#include "../AirflowNetworkSurface.hpp"
#include "../AirflowNetworkSurface_Impl.hpp"
#include "../AirflowNetworkCrack.hpp"
#include "../AirflowNetworkCrack_Impl.hpp"
#include "../AirflowNetworkReferenceCrackConditions.hpp"
#include "../AirflowNetworkReferenceCrackConditions_Impl.hpp"
#include "../AirflowNetworkExternalNode.hpp"
#include "../AirflowNetworkExternalNode_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceWatcher.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include "../../utilities/math/FloatCompare.hpp"

#include "../../utilities/core/Finder.hpp"

#include "../../utilities/core/Assert.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/math/constants/constants.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_Surface_SetVertices) {
  Model model;

  Point3dVector points;
  Point3dVector testPoints;

  // square with unit area
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  Surface surface(points, model);
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_TRUE(!surface.airflowNetworkSurface());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  EXPECT_FALSE(crack0.referenceCrackConditions());

  AirflowNetworkCrack crack1(model, 1.0, 0.65);
  EXPECT_EQ(1, crack1.airMassFlowCoefficient());
  EXPECT_EQ(0.65, crack1.airMassFlowExponent());
  EXPECT_FALSE(crack1.referenceCrackConditions());

  AirflowNetworkSurface afnsurf = surface.getAirflowNetworkSurface(crack0);
  EXPECT_EQ(surface, afnsurf.surface());
  boost::optional<AirflowNetworkSurface> optsurf = surface.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());
  ASSERT_TRUE(afnsurf.leakageComponent());
  EXPECT_EQ(crack0, afnsurf.leakageComponent().get());

  ASSERT_TRUE(afnsurf.setLeakageComponent(crack1));
  ASSERT_TRUE(afnsurf.leakageComponent());
  EXPECT_EQ(crack1, afnsurf.leakageComponent().get());

  EXPECT_FALSE(afnsurf.externalNode());

  AirflowNetworkExternalNode extnode(model);
  ASSERT_TRUE(afnsurf.setExternalNode(extnode));
  EXPECT_EQ(extnode, afnsurf.externalNode());
}

TEST_F(ModelFixture, AirflowNetwork_Surface_Clone) {
  Model model;

  // construct Surface
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));
  Surface surface(vertices, model);

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  EXPECT_FALSE(crack0.referenceCrackConditions());

  // construct AFN Surface
  auto afnsurf = surface.getAirflowNetworkSurface(crack0);
  ASSERT_EQ(surface, afnsurf.surface());
  auto optsurf = surface.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());

  // clone should copy the AFN surface
  Surface clone1 = surface.clone().cast<Surface>();
  boost::optional<AirflowNetworkSurface> optsurf1 = clone1.airflowNetworkSurface();
  ASSERT_TRUE(optsurf1);
  ASSERT_NE(optsurf, optsurf1);
  ASSERT_TRUE(optsurf1.get().leakageComponent());
  EXPECT_EQ(crack0, optsurf1.get().leakageComponent().get());

  // even if through ModelObject
  Surface clone2 = surface.cast<ModelObject>().clone().cast<Surface>();
  boost::optional<AirflowNetworkSurface> optsurf2 = clone2.airflowNetworkSurface();
  ASSERT_TRUE(optsurf2);
  ASSERT_TRUE(optsurf2.get().leakageComponent());
  EXPECT_EQ(crack0, optsurf2.get().leakageComponent().get());
}

TEST_F(ModelFixture, AirflowNetwork_AdjacentSurface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall1.airflowNetworkSurface());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_FALSE(wall2.airflowNetworkSurface());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall2.handle(), wall1.adjacentSurface()->handle());
  ASSERT_TRUE(wall2.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall2.adjacentSurface()->handle());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall1.sunExposure());
  EXPECT_EQ("NoWind", wall1.windExposure());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  EXPECT_EQ("NoSun", wall2.sunExposure());
  EXPECT_EQ("NoWind", wall2.windExposure());
  EXPECT_FALSE(wall1.airflowNetworkSurface());
  EXPECT_FALSE(wall2.airflowNetworkSurface());

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  EXPECT_FALSE(crack0.referenceCrackConditions());

  // construct AFN Surface
  auto afnsurf = wall1.getAirflowNetworkSurface(crack0);
  ASSERT_EQ(wall1, afnsurf.surface());
  auto optsurf = wall1.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());
  optsurf = wall2.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());

  wall2.resetAdjacentSurface();
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall1.sunExposure());
  EXPECT_EQ("WindExposed", wall1.windExposure());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_EQ("SunExposed", wall2.sunExposure());
  EXPECT_EQ("WindExposed", wall2.windExposure());
  optsurf = wall1.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());
  optsurf = wall2.airflowNetworkSurface();
  ASSERT_FALSE(optsurf);
}

TEST_F(ModelFixture, AirflowNetwork_AdjacentSubSurface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 3));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(3, 0, 0));
  vertices.push_back(Point3d(3, 0, 3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices.clear();
  vertices.push_back(Point3d(1, 0, 2));
  vertices.push_back(Point3d(1, 0, 1));
  vertices.push_back(Point3d(2, 0, 1));
  vertices.push_back(Point3d(2, 0, 2));

  SubSurface window1(vertices, model);
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  EXPECT_TRUE(wall2.setAdjacentSurface(wall1));
  EXPECT_TRUE(wall2.setAdjacentSurface(wall1));

  EXPECT_FALSE(window1.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  window1.setSurface(wall1);
  window2.setSurface(wall2);

  EXPECT_FALSE(window1.airflowNetworkSurface());
  EXPECT_FALSE(window2.airflowNetworkSurface());

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));
  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  EXPECT_FALSE(crack0.referenceCrackConditions());

  // construct AFN Surface
  auto afnsurf = window1.getAirflowNetworkSurface(crack0);
  ASSERT_EQ(window1, afnsurf.surface());
  auto optsurf = window1.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());

  optsurf = window2.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());

  wall1.resetAdjacentSurface();
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_FALSE(window1.adjacentSubSurface());
  EXPECT_FALSE(window2.adjacentSubSurface());

  optsurf = window1.airflowNetworkSurface();
  ASSERT_TRUE(optsurf);
  EXPECT_EQ(afnsurf, optsurf.get());
  optsurf = window2.airflowNetworkSurface();
  ASSERT_FALSE(optsurf);
}
