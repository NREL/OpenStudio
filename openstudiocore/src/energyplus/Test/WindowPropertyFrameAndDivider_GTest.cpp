/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::energyplus;
using namespace openstudio::model;

TEST_F(EnergyPlusFixture, WindowPropertyFrameAndDivider)
{

  Model model;

  ThermalZone thermalZone(model);
  Space space(model);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 10));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(10, 0, 0));
  vertices.push_back(Point3d(10, 0, 10));

  Surface surface(vertices, model);

  vertices.clear();
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

  SubSurface subSurface1(vertices, model);
  subSurface1.setName("No Offset");

  SubSurface subSurface2(vertices, model);
  subSurface2.setName("Offset");

  Vector3d normal = subSurface1.outwardNormal();
  EXPECT_DOUBLE_EQ(0.0, normal.x());
  EXPECT_DOUBLE_EQ(-1.0, normal.y());
  EXPECT_DOUBLE_EQ(0.0, normal.z());

  WindowPropertyFrameAndDivider frameAndDivider(model);
  frameAndDivider.setOutsideRevealDepth(1.0);

  subSurface2.setWindowPropertyFrameAndDivider(frameAndDivider);
  subSurface2.setName("Offset");

  ForwardTranslator forwardTranslator;
  OptionalWorkspace outWorkspace = forwardTranslator.translateModel(model);
  ASSERT_TRUE(outWorkspace);

  ReverseTranslator reverseTranslator;
  OptionalModel outModel = reverseTranslator.translateWorkspace(*outWorkspace);
  ASSERT_TRUE(outModel);

  EXPECT_EQ(1u, outModel->getConcreteModelObjects<WindowPropertyFrameAndDivider>().size());

  boost::optional<SubSurface> testSubSurface = outModel->getModelObjectByName<SubSurface>("Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_TRUE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());

  testSubSurface = outModel->getModelObjectByName<SubSurface>("No Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_FALSE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());

}

