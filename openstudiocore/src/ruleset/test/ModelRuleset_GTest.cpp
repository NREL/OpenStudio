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
#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterType_Impl.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute_Impl.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute_Impl.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute_Impl.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetAttribute_Impl.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>
#include <ruleset/ModelObjectActionSetRelationship_Impl.hpp>

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

#include <QDomElement>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::model;

std::ostream& operator<<(std::ostream& os, const QDomElement& element)
{
  QString str;
  QTextStream qts(&str);
  element.save(qts, 2);
  os << str.toStdString();
  return os;
}

std::ostream& operator<<(std::ostream& os, const QDomDocument& document)
{
  QString str;
  QTextStream qts(&str);
  document.save(qts, 2);
  os << str.toStdString();
  return os;
}
TEST_F(RulesetFixture, ModelRuleset) {

  openstudio::path wallComponentPath = toPath("./modelruleset_wall_construction.osc");
  openstudio::path roofComponentPath = toPath("./modelruleset_roof_construction.osc");

  {
    Model model;
    StandardOpaqueMaterial material(model);

    Construction wallConstruction(model);
    wallConstruction.insertLayer(0,material);
    wallConstruction.setName("ModelRulesetWallConstruction");
    wallConstruction.createComponent().save(wallComponentPath, true);

    Construction roofConstruction(model);
    roofConstruction.insertLayer(0,material);
    roofConstruction.setName("ModelRulesetRoofConstruction");
    roofConstruction.createComponent().save(roofComponentPath, true);
  }

  for (unsigned i = 0; i < 100; ++i){
    QDomDocument document;

    {
      ModelObjectFilterType typeFilter(IddObjectType::OS_Surface);
      ModelObjectFilterStringAttribute isWallFilter("surfaceType", RulesetStringPredicate::IEquals, "Wall");
      ModelObjectActionSetAttribute setWallName("name", "New Wall");
      ModelObjectActionSetRelationship setWallConstruction("construction", wallComponentPath);

      ModelObjectFilterStringAttribute isRoofFilter("surfaceType", RulesetStringPredicate::IEquals, "RoofCeiling");
      ModelObjectActionSetAttribute setRoofName("name", "New Roof");
      ModelObjectActionSetRelationship setRoofConstruction("construction", roofComponentPath);

      ModelRule wallRule("WallRule");
      wallRule.add(typeFilter);
      wallRule.add(isWallFilter);
      wallRule.add(setWallName);
      wallRule.add(setWallConstruction);

      ModelRule roofRule("RoofRule");
      roofRule.add(typeFilter);
      roofRule.add(isRoofFilter);
      roofRule.add(setRoofName);
      roofRule.add(setRoofConstruction);

      ModelRuleset ruleset("ruleset");
      ruleset.add(wallRule);
      ruleset.add(roofRule);
      
      ASSERT_EQ(2u, ruleset.rules().size());
      EXPECT_EQ("WallRule", ruleset.rules()[0].name());
      EXPECT_EQ("RoofRule", ruleset.rules()[1].name());

      document = ruleset.toXml();
    }

    EXPECT_TRUE(Ruleset::factoryFromXml(document.documentElement()));
  
    // deserialize from xml
    ModelRuleset ruleset(document.documentElement());
    ASSERT_EQ(2u, ruleset.rules().size()) << "Failure on iteration " << i << std::endl << document << std::endl << ruleset;
    EXPECT_EQ("WallRule", ruleset.rules()[0].name());
    EXPECT_EQ("RoofRule", ruleset.rules()[1].name());

    ASSERT_EQ(2u, ruleset.rules()[0].filters().size()) << "Failure on iteration " << i << std::endl << document << std::endl << ruleset.rules()[0];
    EXPECT_TRUE(ruleset.rules()[0].filters()[0].optionalCast<ModelObjectFilterType>());
    EXPECT_TRUE(ruleset.rules()[0].filters()[1].optionalCast<ModelObjectFilterStringAttribute>());
    ASSERT_EQ(2u, ruleset.rules()[0].actions().size());
    EXPECT_TRUE(ruleset.rules()[0].actions()[0].optionalCast<ModelObjectActionSetAttribute>());
    EXPECT_TRUE(ruleset.rules()[0].actions()[1].optionalCast<ModelObjectActionSetRelationship>());

    ASSERT_EQ(2u, ruleset.rules()[1].filters().size()) << "Failure on iteration " << i << std::endl << document << std::endl << ruleset.rules()[1];
    EXPECT_TRUE(ruleset.rules()[1].filters()[0].optionalCast<ModelObjectFilterType>());
    EXPECT_TRUE(ruleset.rules()[1].filters()[1].optionalCast<ModelObjectFilterStringAttribute>());
    ASSERT_EQ(2u, ruleset.rules()[1].actions().size());
    EXPECT_TRUE(ruleset.rules()[1].actions()[0].optionalCast<ModelObjectActionSetAttribute>());
    EXPECT_TRUE(ruleset.rules()[1].actions()[1].optionalCast<ModelObjectActionSetRelationship>());

    // make up a model
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
    EXPECT_TRUE(roof.setAttribute("name", "Roof"));
    roof.setName("Roof");
    roof.setSpace(space);
    roof.setSurfaceType("RoofCeiling");
    roof.setConstruction(construction);

    ASSERT_TRUE(roof.name());
    EXPECT_EQ("Roof", roof.name().get());
    ASSERT_TRUE(roof.construction());
    EXPECT_EQ(construction.handle(), roof.construction()->handle());

    points.clear();
    points.push_back(Point3d(0,0,1));
    points.push_back(Point3d(0,0,0));
    points.push_back(Point3d(0,1,0));
    points.push_back(Point3d(0,1,1));

    Surface wall(points, model);
    EXPECT_TRUE(wall.setAttribute("name", "Wall"));
    wall.setName("Wall");
    wall.setSpace(space);
    wall.setSurfaceType("Wall");
    wall.setConstruction(construction);

    ASSERT_TRUE(wall.name());
    EXPECT_EQ("Wall", wall.name().get());
    ASSERT_TRUE(wall.construction());
    EXPECT_EQ(construction.handle(), wall.construction()->handle());

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

    ASSERT_TRUE(floor.name());
    EXPECT_EQ("Floor", floor.name().get());
    ASSERT_TRUE(floor.construction());
    EXPECT_EQ(construction.handle(), floor.construction()->handle());

    // apply rule set
    EXPECT_TRUE(ruleset.apply(model));

    ASSERT_TRUE(construction.name());
    EXPECT_EQ("Construction", construction.name().get());

    ASSERT_TRUE(space.name());
    EXPECT_EQ("Space", space.name().get());

    ASSERT_TRUE(roof.name());
    EXPECT_EQ("New Roof", roof.name().get());
    ASSERT_TRUE(roof.construction());
    EXPECT_NE(construction.handle(), roof.construction()->handle());
    ASSERT_TRUE(roof.construction()->name());
    EXPECT_EQ("ModelRulesetRoofConstruction", roof.construction()->name().get());

    ASSERT_TRUE(wall.name());
    EXPECT_EQ("New Wall", wall.name().get());
    ASSERT_TRUE(wall.construction());
    EXPECT_NE(construction.handle(), wall.construction()->handle());
    ASSERT_TRUE(wall.construction()->name());
    EXPECT_EQ("ModelRulesetWallConstruction", wall.construction()->name().get());

    ASSERT_TRUE(window.name());
    EXPECT_EQ("Window", window.name().get());
    ASSERT_TRUE(window.construction());
    EXPECT_EQ(construction.handle(), window.construction()->handle());

    ASSERT_TRUE(floor.name());
    EXPECT_EQ("Floor", floor.name().get());
    ASSERT_TRUE(floor.construction());
    EXPECT_EQ(construction.handle(), floor.construction()->handle());
  }
}


TEST_F(RulesetFixture, ModelRuleset_VersionUUID) {

  ModelObjectFilterType typeFilter(IddObjectType::OS_Surface);
  ModelObjectFilterStringAttribute isWallFilter("surfaceType", RulesetStringPredicate::IEquals, "Wall");
  ModelObjectActionSetAttribute setWallName("name", "New Wall");
  ModelObjectActionSetRelationship setWallConstruction("construction", toPath("./wall.osm"));

  UUID typeFilterUUID = typeFilter.uuid();
  UUID typeFilterVersionUUID = typeFilter.versionUUID();
  UUID isWallFilterUUID = isWallFilter.uuid();
  UUID isWallFilterVersionUUID = isWallFilter.versionUUID();
  UUID setNameActionUUID = setWallName.uuid();
  UUID setNameActionVersionUUID = setWallName.versionUUID();
  UUID setConstructionActionUUID = setWallConstruction.uuid();
  UUID setConstructionActionVersionUUID = setWallConstruction.versionUUID();

  ModelRule wallRule("WallRule");

  UUID ruleUUID = wallRule.uuid();
  UUID ruleVersionUUID = wallRule.versionUUID();

  wallRule.add(typeFilter);

  EXPECT_TRUE(typeFilter.uuid() == typeFilterUUID);
  EXPECT_TRUE(typeFilter.versionUUID() == typeFilterVersionUUID);
  EXPECT_TRUE(isWallFilter.uuid() == isWallFilterUUID);
  EXPECT_TRUE(isWallFilter.versionUUID() == isWallFilterVersionUUID);
  EXPECT_TRUE(setWallName.uuid() == setNameActionUUID);
  EXPECT_TRUE(setWallName.versionUUID() == setNameActionVersionUUID);
  EXPECT_TRUE(setWallConstruction.uuid() == setConstructionActionUUID);
  EXPECT_TRUE(setWallConstruction.versionUUID() == setConstructionActionVersionUUID);
  EXPECT_TRUE(wallRule.uuid() == ruleUUID);
  EXPECT_TRUE(wallRule.versionUUID() != ruleVersionUUID);
  ruleVersionUUID = wallRule.versionUUID();

  wallRule.add(isWallFilter);

  EXPECT_TRUE(typeFilter.uuid() == typeFilterUUID);
  EXPECT_TRUE(typeFilter.versionUUID() == typeFilterVersionUUID);
  EXPECT_TRUE(isWallFilter.uuid() == isWallFilterUUID);
  EXPECT_TRUE(isWallFilter.versionUUID() == isWallFilterVersionUUID);
  EXPECT_TRUE(setWallName.uuid() == setNameActionUUID);
  EXPECT_TRUE(setWallName.versionUUID() == setNameActionVersionUUID);
  EXPECT_TRUE(setWallConstruction.uuid() == setConstructionActionUUID);
  EXPECT_TRUE(setWallConstruction.versionUUID() == setConstructionActionVersionUUID);
  EXPECT_TRUE(wallRule.uuid() == ruleUUID);
  EXPECT_TRUE(wallRule.versionUUID() != ruleVersionUUID);
  ruleVersionUUID = wallRule.versionUUID();

  wallRule.add(setWallName);

  EXPECT_TRUE(typeFilter.uuid() == typeFilterUUID);
  EXPECT_TRUE(typeFilter.versionUUID() == typeFilterVersionUUID);
  EXPECT_TRUE(isWallFilter.uuid() == isWallFilterUUID);
  EXPECT_TRUE(isWallFilter.versionUUID() == isWallFilterVersionUUID);
  EXPECT_TRUE(setWallName.uuid() == setNameActionUUID);
  EXPECT_TRUE(setWallName.versionUUID() == setNameActionVersionUUID);
  EXPECT_TRUE(setWallConstruction.uuid() == setConstructionActionUUID);
  EXPECT_TRUE(setWallConstruction.versionUUID() == setConstructionActionVersionUUID);
  EXPECT_TRUE(wallRule.uuid() == ruleUUID);
  EXPECT_TRUE(wallRule.versionUUID() != ruleVersionUUID);
  ruleVersionUUID = wallRule.versionUUID();

  wallRule.add(setWallConstruction);

  EXPECT_TRUE(typeFilter.uuid() == typeFilterUUID);
  EXPECT_TRUE(typeFilter.versionUUID() == typeFilterVersionUUID);
  EXPECT_TRUE(isWallFilter.uuid() == isWallFilterUUID);
  EXPECT_TRUE(isWallFilter.versionUUID() == isWallFilterVersionUUID);
  EXPECT_TRUE(setWallName.uuid() == setNameActionUUID);
  EXPECT_TRUE(setWallName.versionUUID() == setNameActionVersionUUID);
  EXPECT_TRUE(setWallConstruction.uuid() == setConstructionActionUUID);
  EXPECT_TRUE(setWallConstruction.versionUUID() == setConstructionActionVersionUUID);
  EXPECT_TRUE(wallRule.uuid() == ruleUUID);
  EXPECT_TRUE(wallRule.versionUUID() != ruleVersionUUID);
  ruleVersionUUID = wallRule.versionUUID();

  ModelRuleset ruleset("ruleset");
  
  UUID rulesetUUID = ruleset.uuid();
  UUID rulesetVersionUUID = ruleset.versionUUID();
  
  ruleset.add(wallRule);

  EXPECT_TRUE(typeFilter.uuid() == typeFilterUUID);
  EXPECT_TRUE(typeFilter.versionUUID() == typeFilterVersionUUID);
  EXPECT_TRUE(isWallFilter.uuid() == isWallFilterUUID);
  EXPECT_TRUE(isWallFilter.versionUUID() == isWallFilterVersionUUID);
  EXPECT_TRUE(setWallName.uuid() == setNameActionUUID);
  EXPECT_TRUE(setWallName.versionUUID() == setNameActionVersionUUID);
  EXPECT_TRUE(setWallConstruction.uuid() == setConstructionActionUUID);
  EXPECT_TRUE(setWallConstruction.versionUUID() == setConstructionActionVersionUUID);
  EXPECT_TRUE(wallRule.uuid() == ruleUUID);
  EXPECT_TRUE(wallRule.versionUUID() == ruleVersionUUID);
  EXPECT_TRUE(ruleset.uuid() == rulesetUUID);
  EXPECT_TRUE(ruleset.versionUUID() != rulesetVersionUUID);
  rulesetVersionUUID = ruleset.versionUUID();

}
