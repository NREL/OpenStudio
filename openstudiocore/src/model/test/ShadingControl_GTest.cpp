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

#include "../ShadingControl.hpp"
#include "../Construction.hpp"
#include "../Schedule.hpp"
#include "../Blind.hpp"
#include "../SimpleGlazing.hpp"
#include "../SubSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,ShadingControl_Construction) {
  Model model;

  Construction construction(model);
  
  SimpleGlazing glazing(model);
  EXPECT_TRUE(construction.insertLayer(0,glazing));

  Blind blind(model);
  EXPECT_TRUE(construction.insertLayer(1,blind));

  ShadingControl shadingControl(construction);
  ASSERT_TRUE(shadingControl.construction());
  EXPECT_EQ(construction.handle(), shadingControl.construction()->handle());
  EXPECT_FALSE(shadingControl.shadingMaterial());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  SubSurface subSurface(vertices, model); 
  EXPECT_TRUE(subSurface.setConstruction(construction));
  EXPECT_FALSE(subSurface.shadingControl());
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl));
  ASSERT_TRUE(subSurface.shadingControl());
  ASSERT_TRUE(subSurface.shadingControl()->construction());
  EXPECT_EQ(construction.handle(), subSurface.shadingControl()->construction()->handle());
}


TEST_F(ModelFixture,ShadingControl_Material) {
  Model model;

  Blind blind(model);

  ShadingControl shadingControl(blind);
  EXPECT_FALSE(shadingControl.construction());
  ASSERT_TRUE(shadingControl.shadingMaterial());
  EXPECT_EQ(blind.handle(), shadingControl.shadingMaterial()->handle());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  SubSurface subSurface(vertices, model); 
  EXPECT_FALSE(subSurface.shadingControl());
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl));
  ASSERT_TRUE(subSurface.shadingControl());
  ASSERT_TRUE(subSurface.shadingControl()->shadingMaterial());
  EXPECT_EQ(blind.handle(), subSurface.shadingControl()->shadingMaterial()->handle());
}
