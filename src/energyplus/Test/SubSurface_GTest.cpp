/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultSubSurfaceConstructions.hpp"
#include "../../model/SimpleGlazing.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ReverseTranslator_GlassDoorToSubSurface) {

  std::string text = "\
  FenestrationSurface:Detailed,\n\
    Perimeter_ZN_1_wall_south_door,  !- Name\n\
    GlassDoor,               !- Surface Type\n\
    window_south,            !- Construction Name\n\
    Perimeter_ZN_1_wall_south,  !- Building Surface Name\n\
    ,                        !- Outside Boundary Condition Object\n\
    AutoCalculate,           !- View Factor to Ground\n\
    ,                        !- Frame and Divider Name\n\
    1.0000,                  !- Multiplier\n\
    4,                       !- Number of Vertices\n\
    12.930,0.0000,2.1340,  !- X,Y,Z ==> Vertex 1 {m}\n\
    12.930,0.0000,0.0000,  !- X,Y,Z ==> Vertex 2 {m}\n\
    14.760,0.0000,0.0000,  !- X,Y,Z ==> Vertex 3 {m}\n\
    14.760,0.0000,2.1340;  !- X,Y,Z ==> Vertex 4 {m}";

  IdfObject idfObject = IdfObject::load(text).get();
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  OptionalWorkspaceObject owo = ws.addObject(idfObject);
  ASSERT_TRUE(owo);

  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);
  SubSurfaceVector subSurfaces = model.getConcreteModelObjects<SubSurface>();
  ASSERT_EQ(1u, subSurfaces.size());
  EXPECT_EQ("GlassDoor", subSurfaces[0].subSurfaceType());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SubSurface) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices{
    {0, 2, 0},
    {0, 0, 0},
    {2, 0, 0},
    {2, 2, 0},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices = {
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.assignDefaultSubSurfaceType();

  WindowPropertyFrameAndDivider frame(model);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WindowProperty_FrameAndDivider).size());

  WorkspaceObject subSurfaceObject = workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed)[0];
  WorkspaceObject frameObject = workspace.getObjectsByType(IddObjectType::WindowProperty_FrameAndDivider)[0];

  ASSERT_TRUE(subSurfaceObject.getTarget(FenestrationSurface_DetailedFields::FrameandDividerName));
  EXPECT_EQ(frameObject.handle(), subSurfaceObject.getTarget(FenestrationSurface_DetailedFields::FrameandDividerName)->handle());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SubSurface_Construction) {
  Model m;

  ThermalZone z(m);
  Space space(m);
  EXPECT_TRUE(space.setThermalZone(z));

  // An exterior wall
  Surface wall({{0, 0, 2}, {0, 0, 0}, {2, 0, 0}, {2, 0, 2}}, m);
  EXPECT_TRUE(wall.setSpace(space));
  EXPECT_EQ("Wall", wall.surfaceType());
  EXPECT_EQ("Outdoors", wall.outsideBoundaryCondition());

  const Construction opaque(std::vector<OpaqueMaterial>{StandardOpaqueMaterial(m)});
  const Construction glazed(std::vector<FenestrationMaterial>{SimpleGlazing(m)});
  ASSERT_FALSE(opaque.isFenestration());
  ASSERT_TRUE(glazed.isFenestration());

  // Door and OverheadDoor slots get a fenestration construction, GlassDoor an opaque one, to trigger the SubSurfaceType flips
  DefaultSubSurfaceConstructions exteriorSubSurfaceConstructions(m);
  EXPECT_TRUE(exteriorSubSurfaceConstructions.setFixedWindowConstruction(glazed));
  EXPECT_TRUE(exteriorSubSurfaceConstructions.setOperableWindowConstruction(glazed));
  EXPECT_TRUE(exteriorSubSurfaceConstructions.setDoorConstruction(glazed));
  EXPECT_TRUE(exteriorSubSurfaceConstructions.setOverheadDoorConstruction(glazed));
  EXPECT_TRUE(exteriorSubSurfaceConstructions.setGlassDoorConstruction(opaque));
  DefaultConstructionSet dcs(m);
  EXPECT_TRUE(dcs.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructions));
  EXPECT_TRUE(m.getUniqueModelObject<Building>().setDefaultConstructionSet(dcs));

  auto makeSubSurface = [&m, &wall](const std::string& name, const std::string& subSurfaceType) {
    SubSurface subSurface({{0.5, 0, 1.5}, {0.5, 0, 0.5}, {1.5, 0, 0.5}, {1.5, 0, 1.5}}, m);
    subSurface.setName(name);
    EXPECT_TRUE(subSurface.setSurface(wall));
    EXPECT_TRUE(subSurface.setSubSurfaceType(subSurfaceType));
    EXPECT_TRUE(subSurface.isConstructionDefaulted());
    return subSurface;
  };

  makeSubSurface("Fixed Window", "FixedWindow");
  makeSubSurface("Operable Window", "OperableWindow");
  makeSubSurface("Door", "Door");
  makeSubSurface("Overhead Door", "OverheadDoor");
  makeSubSurface("Glass Door", "GlassDoor");
  auto hardAssigned = makeSubSurface("Hard Assigned Window", "FixedWindow");
  EXPECT_TRUE(hardAssigned.setConstruction(glazed));

  struct Expected
  {
    std::string name;
    std::string surfaceType;
    std::string constructionName;  // Empty if not written
  };

  auto checkSubSurfaces = [](const Workspace& w, const std::vector<Expected>& expecteds) {
    for (const auto& [name, surfaceType, constructionName] : expecteds) {
      SCOPED_TRACE(name);
      auto idf_subSurface_ = w.getObjectByTypeAndName(IddObjectType::FenestrationSurface_Detailed, name);
      ASSERT_TRUE(idf_subSurface_);
      EXPECT_EQ(surfaceType, idf_subSurface_->getString(FenestrationSurface_DetailedFields::SurfaceType).get());
      EXPECT_EQ(constructionName, idf_subSurface_->getString(FenestrationSurface_DetailedFields::ConstructionName, false, true).value_or(""));
    }
  };

  ForwardTranslator ft;

  {
    // The SubSurfaceType is not mapped to the legacy Window/Door anymore, and the construction is only written when hard-assigned or when the
    // SubSurfaceType was flipped (E+ would look it up in a different ConstructionAssignmentSet slot)
    const Workspace w = ft.translateModel(m);
    const std::vector<Expected> expecteds{
      {"Fixed Window", "FixedWindow", ""},         {"Operable Window", "OperableWindow", ""},
      {"Door", "GlassDoor", glazed.nameString()},  {"Overhead Door", "GlassDoor", glazed.nameString()},
      {"Glass Door", "Door", opaque.nameString()}, {"Hard Assigned Window", "FixedWindow", glazed.nameString()},
    };
    checkSubSurfaces(w, expecteds);
  }

  {
    // Construction assignment sets excluded: all constructions are written
    ft.setExcludeConstructionAssignmentSets(true);
    const Workspace w = ft.translateModel(m);
    const std::vector<Expected> expecteds{
      {"Fixed Window", "FixedWindow", glazed.nameString()}, {"Operable Window", "OperableWindow", glazed.nameString()},
      {"Door", "GlassDoor", glazed.nameString()},           {"Overhead Door", "GlassDoor", glazed.nameString()},
      {"Glass Door", "Door", opaque.nameString()},          {"Hard Assigned Window", "FixedWindow", glazed.nameString()},
    };
    checkSubSurfaces(w, expecteds);
  }
}
