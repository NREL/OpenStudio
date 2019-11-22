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
