/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/ShadingControl.hpp>
#include <model/Construction.hpp>
#include <model/Schedule.hpp>
#include <model/Blind.hpp>
#include <model/SimpleGlazing.hpp>
#include <model/SubSurface.hpp>

#include <utilities/geometry/Point3d.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ShadingControl_Construction) {
  Model model;
 
  Construction construction(model);
  boost::optional<ShadingControl> shadingControl = construction.addShadingControl();
  EXPECT_FALSE(shadingControl);

  SimpleGlazing glazing(model);
  EXPECT_TRUE(construction.insertLayer(0,glazing));
  shadingControl = construction.addShadingControl();
  EXPECT_FALSE(shadingControl);

  Blind blind(model);
  EXPECT_TRUE(construction.insertLayer(1,blind));
  shadingControl = construction.addShadingControl();
  ASSERT_TRUE(shadingControl);

  EXPECT_EQ(construction.handle(), shadingControl->construction().handle());
  EXPECT_EQ("InteriorBlind", shadingControl->shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl->shadingControlType());
  EXPECT_FALSE(shadingControl->schedule());
}


TEST_F(ModelFixture,ShadingControl_SubSurface) {
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  SubSurface subSurface(vertices, model);
  boost::optional<ShadingControl> shadingControl = subSurface.addShadingControl();

  Construction construction(model);
  EXPECT_TRUE(subSurface.setConstruction(construction));
  shadingControl = subSurface.addShadingControl();
  EXPECT_FALSE(shadingControl);

  SimpleGlazing glazing(model);
  EXPECT_TRUE(construction.insertLayer(0,glazing));
  shadingControl = subSurface.addShadingControl();
  EXPECT_FALSE(shadingControl);

  Blind blind(model);
  EXPECT_TRUE(construction.insertLayer(1,blind));
  shadingControl = subSurface.addShadingControl();
  ASSERT_TRUE(shadingControl);

  EXPECT_EQ(construction.handle(), shadingControl->construction().handle());
  EXPECT_EQ("InteriorBlind", shadingControl->shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl->shadingControlType());
  EXPECT_FALSE(shadingControl->schedule());
}
