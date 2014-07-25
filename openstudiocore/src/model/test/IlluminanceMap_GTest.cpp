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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../IlluminanceMap.hpp"
#include "../IlluminanceMap_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Geometry.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, IlluminanceMap_Clone)
{
  Model model;

  IlluminanceMap map(model);
  Space space(model);

  EXPECT_FALSE(map.space());
  EXPECT_EQ(0u, space.illuminanceMaps().size());

  map.setSpace(space);

  ASSERT_TRUE(map.space());
  EXPECT_EQ(space.handle(), map.space()->handle());
  EXPECT_EQ(1u, space.illuminanceMaps().size());

  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<IlluminanceMap>().size());
  ModelObject object = map.clone(model);
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<IlluminanceMap>().size());

  ASSERT_TRUE(object.optionalCast<IlluminanceMap>());
  EXPECT_EQ(space.handle(), map.space()->handle());
  EXPECT_EQ(space.handle(), object.cast<IlluminanceMap>().space()->handle());
  EXPECT_EQ(2u, space.illuminanceMaps().size());

}

TEST_F(ModelFixture, IlluminanceMap_Transformation)
{
  Model model;
  IlluminanceMap map(model);
  map.setXLength(2);
  map.setYLength(2);
  map.setNumberofXGridPoints(2);
  map.setNumberofYGridPoints(2);
  map.setOriginXCoordinate(1);
  map.setOriginYCoordinate(0);
  map.setOriginZCoordinate(2);

  Point3dVector testPoints = map.referencePoints();
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_NEAR(0, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[3].z(), 0.000001);

  testPoints = map.transformation()*map.referencePoints();
  EXPECT_NEAR(1, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(3, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].z(), 0.000001);

  Transformation transformation = Transformation::translation(Vector3d(1,0,2));
  EXPECT_TRUE(map.setTransformation(transformation));
  EXPECT_TRUE(transformation.matrix() == map.transformation().matrix());

  testPoints = map.referencePoints();
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_NEAR(0, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[3].z(), 0.000001);

  testPoints = map.transformation()*map.referencePoints();
  EXPECT_NEAR(1, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(3, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].z(), 0.000001);

  transformation = Transformation::translation(Vector3d(1,0,2))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90));
  EXPECT_TRUE(map.setTransformation(transformation));
  EXPECT_TRUE(transformation.matrix() == map.transformation().matrix()) << transformation.matrix() << std::endl << map.transformation().matrix();

  testPoints = map.referencePoints();
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_NEAR(0, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(0, testPoints[3].z(), 0.000001);

  testPoints = map.transformation()*map.referencePoints();
  EXPECT_NEAR(1, testPoints[0].x(), 0.000001);
  EXPECT_NEAR(0, testPoints[0].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[0].z(), 0.000001);
  EXPECT_NEAR(3, testPoints[3].x(), 0.000001);
  EXPECT_NEAR(-2, testPoints[3].y(), 0.000001);
  EXPECT_NEAR(2, testPoints[3].z(), 0.000001);

}

TEST_F(ModelFixture, IlluminanceMap_SpaceSetTransformation)
{
  Model model;
  Space space(model);
  IlluminanceMap map(model);
  map.setXLength(2);
  map.setYLength(2);
  map.setNumberofXGridPoints(2);
  map.setNumberofYGridPoints(2);
  map.setOriginXCoordinate(1);
  map.setOriginYCoordinate(0);
  map.setOriginZCoordinate(2);
  map.setSpace(space);

  Point3dVector testPoints = map.referencePoints();
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_DOUBLE_EQ(0, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(0, testPoints[0].z());
  EXPECT_DOUBLE_EQ(2, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(0, testPoints[3].z());

  testPoints = space.transformation()*map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(1,0,0))));

  testPoints = space.transformation()*map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(2, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(4, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(1,0,0))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90))));

  testPoints = space.transformation()*map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(-1, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(-3, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());
}

TEST_F(ModelFixture, IlluminanceMap_SpaceChangeTransformation)
{
  Model model;
  Space space(model);
  IlluminanceMap map(model);
  map.setXLength(2);
  map.setYLength(2);
  map.setNumberofXGridPoints(2);
  map.setNumberofYGridPoints(2);
  map.setOriginXCoordinate(1);
  map.setOriginYCoordinate(0);
  map.setOriginZCoordinate(2);
  map.setSpace(space);

  Point3dVector testPoints = map.referencePoints();
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_DOUBLE_EQ(0, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(0, testPoints[0].z());
  EXPECT_DOUBLE_EQ(2, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(0, testPoints[3].z());

  testPoints = map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  testPoints = space.transformation()*map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1,0,0))));

  testPoints = map.referencePoints();
  EXPECT_DOUBLE_EQ(0, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(0, testPoints[0].z());
  EXPECT_DOUBLE_EQ(2, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(0, testPoints[3].z());

  testPoints = map.transformation()*map.referencePoints();
  EXPECT_DOUBLE_EQ(0, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(2, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  testPoints = space.transformation()*map.transformation()*map.referencePoints();

  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1,0,0))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90))));

  testPoints = space.transformation()*map.transformation()*map.referencePoints();

  EXPECT_DOUBLE_EQ(1, testPoints[0].x());
  EXPECT_DOUBLE_EQ(0, testPoints[0].y());
  EXPECT_DOUBLE_EQ(2, testPoints[0].z());
  EXPECT_DOUBLE_EQ(3, testPoints[3].x());
  EXPECT_DOUBLE_EQ(2, testPoints[3].y());
  EXPECT_DOUBLE_EQ(2, testPoints[3].z());
}
