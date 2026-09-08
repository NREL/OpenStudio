/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ComfortViewFactorAngles.hpp"
#include "../../model/ComfortViewFactorAngles_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/People.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ComfortViewFactorAngles) {
  Model model;
  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface surface(points, model);
  surface.setName("Radiant Surface");
  EXPECT_TRUE(surface.setSpace(space));

  ComfortViewFactorAngles comfortViewFactorAngles(model);
  comfortViewFactorAngles.setName("Angle Factors");
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 1.0));

  PeopleDefinition definition(model);
  EXPECT_TRUE(definition.setSurfaceNameAngleFactorListName(comfortViewFactorAngles));
  People people(definition);
  EXPECT_TRUE(people.setSpace(space));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  const auto angleFactorObjects = workspace.getObjectsByType(IddObjectType::ComfortViewFactorAngles);
  ASSERT_EQ(1u, angleFactorObjects.size());
  const auto& angleFactorObject = angleFactorObjects.front();
  EXPECT_EQ("Angle Factors", angleFactorObject.nameString());
  ASSERT_EQ(1u, angleFactorObject.numExtensibleGroups());
  const auto group = angleFactorObject.extensibleGroups().front();
  EXPECT_EQ("Radiant Surface", group.getString(0).get());
  EXPECT_DOUBLE_EQ(1.0, group.getDouble(1).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ComfortViewFactorAngles_Empty) {
  Model model;
  ComfortViewFactorAngles comfortViewFactorAngles(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModelObject(comfortViewFactorAngles);
  EXPECT_TRUE(workspace.getObjectsByType(IddObjectType::ComfortViewFactorAngles).empty());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ComfortViewFactorAngles_InvalidSum) {
  Model model;
  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface surface(points, model);
  EXPECT_TRUE(surface.setSpace(space));

  ComfortViewFactorAngles comfortViewFactorAngles(model);
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 0.5));

  PeopleDefinition definition(model);
  EXPECT_TRUE(definition.setSurfaceNameAngleFactorListName(comfortViewFactorAngles));
  People people(definition);
  EXPECT_TRUE(people.setSpace(space));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_TRUE(workspace.getObjectsByType(IddObjectType::ComfortViewFactorAngles).empty());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ComfortViewFactorAngles_UntranslatedSurface) {
  Model model;
  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));
  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface surface(points, model);

  ComfortViewFactorAngles comfortViewFactorAngles(model);
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 1.0));

  PeopleDefinition definition(model);
  EXPECT_TRUE(definition.setSurfaceNameAngleFactorListName(comfortViewFactorAngles));
  People people(definition);
  EXPECT_TRUE(people.setSpace(space));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_TRUE(workspace.getObjectsByType(IddObjectType::ComfortViewFactorAngles).empty());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ComfortViewFactorAngles_SkipsUntranslatedSurface) {
  Model model;
  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface translatedSurface(points, model);
  translatedSurface.setName("Translated Surface");
  EXPECT_TRUE(translatedSurface.setSpace(space));
  Surface untranslatedSurface(points, model);

  ComfortViewFactorAngles comfortViewFactorAngles(model);
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(translatedSurface, 1.0));
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(untranslatedSurface, 0.0));

  PeopleDefinition definition(model);
  EXPECT_TRUE(definition.setSurfaceNameAngleFactorListName(comfortViewFactorAngles));
  People people(definition);
  EXPECT_TRUE(people.setSpace(space));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  const auto angleFactorObjects = workspace.getObjectsByType(IddObjectType::ComfortViewFactorAngles);
  ASSERT_EQ(1u, angleFactorObjects.size());
  ASSERT_EQ(1u, angleFactorObjects.front().numExtensibleGroups());
  const auto group = angleFactorObjects.front().extensibleGroups().front();
  EXPECT_EQ("Translated Surface", group.getString(0).get());
  EXPECT_DOUBLE_EQ(1.0, group.getDouble(1).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ComfortViewFactorAngles) {
  Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto zone = workspace.addObject(IdfObject(IddObjectType::Zone)).get();
  zone.setName("Thermal Zone 1");

  auto surface = workspace.addObject(IdfObject(IddObjectType::BuildingSurface_Detailed)).get();
  surface.setName("Surface 1");
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall"));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::ConstructionName, ""));
  EXPECT_TRUE(surface.setPointer(BuildingSurface_DetailedFields::ZoneName, zone.handle()));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors"));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, ""));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed"));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed"));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::ViewFactortoGround, ""));
  EXPECT_TRUE(surface.setString(BuildingSurface_DetailedFields::NumberofVertices, ""));
  for (const Point3d& point : Point3dVector{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}}) {
    auto group = surface.pushExtensibleGroup();
    group.setDouble(0, point.x());
    group.setDouble(1, point.y());
    group.setDouble(2, point.z());
  }

  auto angleFactorObject = workspace.addObject(IdfObject(IddObjectType::ComfortViewFactorAngles)).get();
  angleFactorObject.setName("Angle Factors");
  auto group = angleFactorObject.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(group.setPointer(0, surface.handle()));
  EXPECT_TRUE(group.setDouble(1, 1.0));

  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(workspace);

  const auto comfortViewFactorAngleObjects = model.getConcreteModelObjects<ComfortViewFactorAngles>();
  ASSERT_EQ(1u, comfortViewFactorAngleObjects.size());
  const auto& comfortViewFactorAngles = comfortViewFactorAngleObjects.front();
  EXPECT_EQ("Angle Factors", comfortViewFactorAngles.nameString());
  ASSERT_EQ(1u, comfortViewFactorAngles.numberofAngleFactors());
  const auto angleFactors = comfortViewFactorAngles.angleFactors();
  ASSERT_EQ(1u, angleFactors.size());
  EXPECT_EQ("Surface 1", angleFactors.front().surface().nameString());
  EXPECT_DOUBLE_EQ(1.0, angleFactors.front().angleFactor());
}