/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../WindowPropertyFrameAndDivider.hpp"
#include "../WindowPropertyFrameAndDivider_Impl.hpp"
#include "../SubSurface.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, WindowPropertyFrameAndDivider)
{
  Model model;
  
  WindowPropertyFrameAndDivider frameAndDivider(model);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

  SubSurface subSurface(vertices, model);

  EXPECT_FALSE(subSurface.windowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface.allowWindowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface.setWindowPropertyFrameAndDivider(frameAndDivider));
  ASSERT_TRUE(subSurface.windowPropertyFrameAndDivider());

  subSurface.resetWindowPropertyFrameAndDivider();
  EXPECT_FALSE(subSurface.windowPropertyFrameAndDivider());
}
