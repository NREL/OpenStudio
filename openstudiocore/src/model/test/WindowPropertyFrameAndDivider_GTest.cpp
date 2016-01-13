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

#include "ModelFixture.hpp"
#include "../WindowPropertyFrameAndDivider.hpp"
#include "../WindowPropertyFrameAndDivider_Impl.hpp"
#include "../SubSurface.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include <utilities/idd/OS_WindowProperty_FrameAndDivider_FieldEnums.hxx>

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

TEST_F(ModelFixture, WindowPropertyFrameAndDivider_Name)
{
  Model model;

  WindowPropertyFrameAndDivider frameAndDivider1(model);
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Window Property Frame And Divider 1", frameAndDivider1.name().get());

  WindowPropertyFrameAndDivider frameAndDivider2(model);
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Window Property Frame And Divider 2", frameAndDivider2.name().get());

  // setName api protects against empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setName(""));
  EXPECT_TRUE(frameAndDivider2.setName(""));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ(" 1", frameAndDivider2.name().get());

  // setName api protects against non-empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setName("Frame"));
  EXPECT_TRUE(frameAndDivider2.setName("Frame"));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Frame", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Frame 1", frameAndDivider2.name().get());

  // setString api does not protect against non-empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setString(OS_WindowProperty_FrameAndDividerFields::Name, "Divider"));
  EXPECT_TRUE(frameAndDivider2.setString(OS_WindowProperty_FrameAndDividerFields::Name, "Divider"));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Divider", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Divider", frameAndDivider2.name().get());

  // setString api does not protect against empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setString(OS_WindowProperty_FrameAndDividerFields::Name, ""));
  EXPECT_TRUE(frameAndDivider2.setString(OS_WindowProperty_FrameAndDividerFields::Name, ""));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("", frameAndDivider2.name().get());
}