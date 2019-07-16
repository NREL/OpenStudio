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