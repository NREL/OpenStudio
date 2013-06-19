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
#include <ruleset/test/RulesetFixture.hpp>

#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelRule_Impl.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterRelationship.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>

#include <model/Model.hpp>
#include <model/Component.hpp>
#include <model/Space.hpp>
#include <model/Construction.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/Surface.hpp>
#include <model/SubSurface.hpp>
#include <model/StandardOpaqueMaterial.hpp>

#include <utilities/geometry/Point3d.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::model;

TEST_F(RulesetFixture, ModelRule) {


  openstudio::path componentPath = toPath("./modelrule_construction.osc");

  {
    Model model;
    Construction construction(model);
    StandardOpaqueMaterial material(model);
    construction.insertLayer(0,material);
    construction.setName("ModelRuleConstruction");
    construction.createComponent().save(componentPath, true);
  }

  boost::optional<Component> constructionComponent = Component::load(componentPath);
  ASSERT_TRUE(constructionComponent);
  ModelObject primaryObject = constructionComponent->primaryObject();
  EXPECT_TRUE(primaryObject.optionalCast<ConstructionBase>());
  ASSERT_TRUE(primaryObject.name());
  EXPECT_NE("Construction", primaryObject.name().get());

  Model model;

  Construction construction(model);
  construction.setName("Construction");
  ASSERT_TRUE(construction.name());
  EXPECT_EQ("Construction", construction.name().get());

  Space space(model);
  space.setName("Space");
  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space", space.name().get());

  Point3dVector points;
  points.push_back(Point3d(0,1,1));
  points.push_back(Point3d(0,0,1));
  points.push_back(Point3d(1,0,1));
  points.push_back(Point3d(1,1,1));

  Surface roof(points, model); 
  roof.setName("Roof");
  roof.setSpace(space);
  roof.setSurfaceType("RoofCeiling");
  roof.setConstruction(construction);
  roof.setOutsideBoundaryCondition("Outdoors");

  ASSERT_TRUE(roof.name());
  EXPECT_EQ("Roof", roof.name().get());
  ASSERT_TRUE(roof.construction());
  EXPECT_EQ(construction.handle(), roof.construction()->handle());
  EXPECT_EQ("Outdoors", roof.outsideBoundaryCondition());

  points.clear();
  points.push_back(Point3d(0,0,1));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,1,1));

  Surface wall(points, model);
  wall.setName("Wall");
  wall.setSpace(space);
  wall.setSurfaceType("Wall");
  wall.setConstruction(construction);
  wall.setOutsideBoundaryCondition("Outdoors");

  ASSERT_TRUE(wall.name());
  EXPECT_EQ("Wall", wall.name().get());
  ASSERT_TRUE(wall.construction());
  EXPECT_EQ(construction.handle(), wall.construction()->handle());
  EXPECT_EQ("Outdoors", wall.outsideBoundaryCondition());

  points.clear();
  points.push_back(Point3d(0,0.25,0.75));
  points.push_back(Point3d(0,0.25,0.25));
  points.push_back(Point3d(0,0.75,0.25));
  points.push_back(Point3d(0,0.75,0.75));

  SubSurface window(points, model); 
  window.setName("Window");
  window.setSurface(wall);
  window.setSubSurfaceType("FixedWindow");
  window.setConstruction(construction);

  ASSERT_TRUE(window.name());
  EXPECT_EQ("Window", window.name().get());
  ASSERT_TRUE(window.construction());
  EXPECT_EQ(construction.handle(), window.construction()->handle());
  ASSERT_TRUE(window.surface());
  EXPECT_EQ(wall.handle(), window.surface()->handle());

  points.clear();
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));

  Surface floor(points, model); 
  floor.setName("Floor");
  floor.setSpace(space);
  floor.setSurfaceType("Floor");
  floor.setConstruction(construction);
  floor.setOutsideBoundaryCondition("Ground");

  ASSERT_TRUE(floor.name());
  EXPECT_EQ("Floor", floor.name().get());
  ASSERT_TRUE(floor.construction());
  EXPECT_EQ(construction.handle(), floor.construction()->handle());
  EXPECT_EQ("Ground", floor.outsideBoundaryCondition());

  ModelObjectFilterType typeFilter(IddObjectType::OS_Surface);
  ModelObjectFilterStringAttribute isWallFilter("surfaceType", RulesetStringPredicate::IEquals, "Wall");
  ModelObjectFilterStringAttribute isExteriorFilter("outsideBoundaryCondition", RulesetStringPredicate::IEquals, "Outdoors");
  ModelObjectActionSetAttribute setName("name", "New Wall");
  ModelObjectActionSetRelationship setConstruction("construction", componentPath);

  ModelRule rule("rule");
  rule.add(typeFilter);
  rule.add(isWallFilter);
  rule.add(isExteriorFilter);
  rule.add(setName);
  rule.add(setConstruction);

  EXPECT_TRUE(rule.apply(model));

  ASSERT_TRUE(construction.name());
  EXPECT_EQ("Construction", construction.name().get());

  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space", space.name().get());

  ASSERT_TRUE(roof.name());
  EXPECT_EQ("Roof", roof.name().get());
  ASSERT_TRUE(roof.construction());
  EXPECT_EQ(construction.handle(), roof.construction()->handle());

  ASSERT_TRUE(wall.name());
  EXPECT_EQ("New Wall", wall.name().get());
  ASSERT_TRUE(wall.construction());
  EXPECT_NE(construction.handle(), wall.construction()->handle());
  ASSERT_TRUE(wall.construction()->name());
  EXPECT_EQ("ModelRuleConstruction", wall.construction()->name().get());

  ASSERT_TRUE(window.name());
  EXPECT_EQ("Window", window.name().get());
  ASSERT_TRUE(window.construction());
  EXPECT_EQ(construction.handle(), window.construction()->handle());

  ASSERT_TRUE(floor.name());
  EXPECT_EQ("Floor", floor.name().get());
  ASSERT_TRUE(floor.construction());
  EXPECT_EQ(construction.handle(), floor.construction()->handle());

  rule.clearFilters();
  rule.clearActions();
  rule.add(typeFilter);
  rule.add(setConstruction);

  EXPECT_TRUE(rule.apply(model));

  ASSERT_TRUE(construction.name());
  EXPECT_EQ("Construction", construction.name().get());

  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space", space.name().get());

  ASSERT_TRUE(roof.name());
  EXPECT_EQ("Roof", roof.name().get());
  ASSERT_TRUE(roof.construction());
  EXPECT_NE(construction.handle(), roof.construction()->handle());
  ASSERT_TRUE(roof.construction()->name());
  EXPECT_EQ("ModelRuleConstruction", roof.construction()->name().get());

  ASSERT_TRUE(wall.name());
  EXPECT_EQ("New Wall", wall.name().get());
  ASSERT_TRUE(wall.construction());
  EXPECT_NE(construction.handle(), wall.construction()->handle());
  ASSERT_TRUE(wall.construction()->name());
  EXPECT_EQ("ModelRuleConstruction", wall.construction()->name().get());

  ASSERT_TRUE(window.name());
  EXPECT_EQ("Window", window.name().get());
  ASSERT_TRUE(window.construction());
  EXPECT_EQ(construction.handle(), window.construction()->handle());

  ASSERT_TRUE(floor.name());
  EXPECT_EQ("Floor", floor.name().get());
  ASSERT_TRUE(floor.construction());
  EXPECT_NE(construction.handle(), floor.construction()->handle());
  ASSERT_TRUE(floor.construction()->name());
  EXPECT_EQ("ModelRuleConstruction", floor.construction()->name().get());

}

TEST_F(RulesetFixture, ModelRule_FilterByRelationship) {

  Model model;
  Construction construction(model);
  StandardOpaqueMaterial material(model);
  construction.insertLayer(0,material);
  material.setThickness(0.1);
  construction.setInsulation(material);
  
  ModelRule rule("Change Thickness");
  ModelObjectFilterType typeFilter(IddObjectType::OS_Construction);
  rule.add(typeFilter);
  ModelObjectFilterRelationship relationshipFilter("insulation");
  rule.add(relationshipFilter);
  ModelObjectActionSetAttribute setThickness("thickness",0.2);
  rule.add(setThickness);
  
  rule.apply(model);
  EXPECT_EQ(0.2,material.thickness()); 
}

