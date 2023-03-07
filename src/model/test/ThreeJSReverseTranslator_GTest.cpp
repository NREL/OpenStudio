/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ThreeJSReverseTranslator.hpp"
#include "../ThreeJSForwardTranslator.hpp"
#include "../ModelMerger.hpp"
#include "../Model.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../DaylightingControl.hpp"
#include "../DaylightingControl_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../ShadingSurfaceGroup_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurface_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../RenderingColor.hpp"
#include "../RenderingColor_Impl.hpp"
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include "../../utilities/geometry/ThreeJS.hpp"
#include "../../utilities/geometry/FloorplanJS.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/units/QuantityConverter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS) {

  ThreeJSReverseTranslator rt;
  openstudio::path out;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // triangulated, for display
  ThreeScene scene = floorPlan->toThreeScene(false);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/FloorplanToThreeJSTriangulated.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();

  // not triangulated, for model transport/translation
  scene = floorPlan->toThreeScene(true);
  json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/FloorplanToThreeJSNonTriangulated.json");
  openstudio::filesystem::ofstream file2(out);
  ASSERT_TRUE(file2.is_open());
  file2 << json;
  file2.close();

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / toPath("model/FloorplanToThreeJSNonTriangulatedToModel.osm"), true);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_SurfaceMatch) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/surface_match_floorplan.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  std::stringstream ss;
  ss << *model;
  std::string s = ss.str();

  EXPECT_EQ(4u, model->getConcreteModelObjects<Space>().size());
  EXPECT_EQ(24u, model->getConcreteModelObjects<Surface>().size());

  unsigned numMatched = 0;
  for (const auto& surface : model->getConcreteModelObjects<Surface>()) {
    if (surface.outsideBoundaryCondition() == "Surface") {
      EXPECT_TRUE(surface.adjacentSurface());
      ++numMatched;
    }
  }
  EXPECT_EQ(8u, numMatched);

  // merge export model into newModel
  model::Model newModel;
  model::ModelMerger mm;
  mm.mergeModels(newModel, *model, rt.handleMapping());

  EXPECT_EQ(4u, newModel.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(24u, newModel.getConcreteModelObjects<Surface>().size());

  numMatched = 0;
  for (const auto& surface : newModel.getConcreteModelObjects<Surface>()) {
    if (surface.outsideBoundaryCondition() == "Surface") {
      EXPECT_TRUE(surface.adjacentSurface());
      ++numMatched;
    }
  }
  EXPECT_EQ(8u, numMatched);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_Doors) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan_doors.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / toPath("model/floorplan_doors.osm"), true);

  ThreeJSForwardTranslator ft;
  ft.modelToThreeJS(*model, true);

  ASSERT_EQ(1u, model->getConcreteModelObjects<Space>().size());

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Space 1 - 1");
  ASSERT_TRUE(space1);

  unsigned numDoors = 0;
  unsigned numGlassDoors = 0;
  unsigned numOverheadDoors = 0;
  double doorArea = 0;
  double glassDoorArea = 0;
  double overheadDoorArea = 0;

  for (const auto& surface : space1->surfaces()) {
    for (const auto& subSurface : surface.subSurfaces()) {
      if (subSurface.subSurfaceType() == "Door") {
        doorArea += subSurface.grossArea();
        numDoors++;
      } else if (subSurface.subSurfaceType() == "GlassDoor") {
        glassDoorArea += subSurface.grossArea();
        numGlassDoors++;
      } else if (subSurface.subSurfaceType() == "OverheadDoor") {
        overheadDoorArea += subSurface.grossArea();
        numOverheadDoors++;
      }
    }
  }

  EXPECT_EQ(3, numDoors);
  EXPECT_EQ(3, numGlassDoors);
  EXPECT_EQ(2, numOverheadDoors);

  EXPECT_NEAR(convert(3.0 * 3.0 * 6.67, "ft^2", "m^2").get(), doorArea, 0.01);
  EXPECT_NEAR(convert(3.0 * 6.0 * 6.67, "ft^2", "m^2").get(), glassDoorArea, 0.01);
  EXPECT_NEAR(convert(2.0 * 15.0 * 8.0, "ft^2", "m^2").get(), overheadDoorArea, 0.01);

  Model model2;
  ModelMerger merger;
  std::map<UUID, UUID> handleMapping;
  merger.mergeModels(model2, *model, handleMapping);

  ASSERT_EQ(1u, model->getConcreteModelObjects<Space>().size());

  space1 = model2.getConcreteModelObjectByName<Space>("Space 1 - 1");
  ASSERT_TRUE(space1);

  numDoors = 0;
  numGlassDoors = 0;
  numOverheadDoors = 0;
  doorArea = 0;
  glassDoorArea = 0;
  overheadDoorArea = 0;

  for (const auto& surface : space1->surfaces()) {
    for (const auto& subSurface : surface.subSurfaces()) {
      if (subSurface.subSurfaceType() == "Door") {
        doorArea += subSurface.grossArea();
        numDoors++;
      } else if (subSurface.subSurfaceType() == "GlassDoor") {
        glassDoorArea += subSurface.grossArea();
        numGlassDoors++;
      } else if (subSurface.subSurfaceType() == "OverheadDoor") {
        overheadDoorArea += subSurface.grossArea();
        numOverheadDoors++;
      }
    }
  }

  EXPECT_EQ(3, numDoors);
  EXPECT_EQ(3, numGlassDoors);
  EXPECT_EQ(2, numOverheadDoors);

  EXPECT_NEAR(convert(3.0 * 3.0 * 6.67, "ft^2", "m^2").get(), doorArea, 0.01);
  EXPECT_NEAR(convert(3.0 * 6.0 * 6.67, "ft^2", "m^2").get(), glassDoorArea, 0.01);
  EXPECT_NEAR(convert(2.0 * 15.0 * 8.0, "ft^2", "m^2").get(), overheadDoorArea, 0.01);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_Windows) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/window_floorplan.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  std::stringstream ss;
  ss << *model;
  std::string s = ss.str();

  ThreeJSForwardTranslator ft;
  ft.modelToThreeJS(*model, true);

  ASSERT_EQ(3u, model->getConcreteModelObjects<Space>().size());

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Space 1");
  boost::optional<Space> space2 = model->getConcreteModelObjectByName<Space>("Space 2");
  boost::optional<Space> space3 = model->getConcreteModelObjectByName<Space>("Space 3");
  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  ASSERT_TRUE(space3);

  std::vector<Space> spaces;
  spaces.push_back(*space1);
  spaces.push_back(*space2);
  spaces.push_back(*space3);

  struct SpaceInfo
  {
    std::string name;
    unsigned numExteriorWalls;
    boost::optional<Surface> southSurface;
    std::vector<SubSurface> windows;
    std::vector<DaylightingControl> dcs;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    info.numExteriorWalls = 0;
    for (const auto& surface : space.surfaces()) {
      if (surface.surfaceType() == "Wall") {
        if (surface.outsideBoundaryCondition() == "Outdoors") {
          info.numExteriorWalls += 1;
          if ((surface.azimuth() > degToRad(179)) && (surface.azimuth() < degToRad(181))) {
            // only one
            EXPECT_FALSE(info.southSurface) << info.name;
            info.southSurface = surface;
            for (const auto& subSurface : surface.subSurfaces()) {
              info.windows.push_back(subSurface);
            }
          }
        }
      }
    }

    for (const auto& dc : space.daylightingControls()) {
      info.dcs.push_back(dc);
    }

    infos.push_back(info);
  }

  std::sort(std::begin(infos), std::end(infos), [](SpaceInfo a, SpaceInfo b) { return a.name < b.name; });

  ASSERT_EQ(3u, infos.size());

  EXPECT_EQ("Space 1", infos[0].name);
  EXPECT_EQ(3, infos[0].numExteriorWalls);
  ASSERT_TRUE(infos[0].southSurface);
  EXPECT_EQ(9, infos[0].windows.size());
  for (const auto& window : infos[0].windows) {
    EXPECT_NEAR(convert(8.0, "ft^2", "m^2").get(), window.grossArea(), 0.001);
    ASSERT_EQ(1u, window.shadingSurfaceGroups().size());
    EXPECT_EQ(3u, window.shadingSurfaceGroups()[0].shadingSurfaces().size());  // overhang + fins
  }
  EXPECT_NEAR(convert(800.0, "ft^2", "m^2").get(), infos[0].southSurface->grossArea(), 0.001);
  EXPECT_NEAR(convert(800.0 - 9 * 8.0, "ft^2", "m^2").get(), infos[0].southSurface->netArea(), 0.001);
  EXPECT_EQ(1, infos[0].dcs.size());

  EXPECT_EQ("Space 2", infos[1].name);
  EXPECT_EQ(2, infos[1].numExteriorWalls);
  ASSERT_TRUE(infos[1].southSurface);
  EXPECT_EQ(17, infos[1].windows.size());
  for (const auto& window : infos[1].windows) {
    EXPECT_NEAR(convert(8.0, "ft^2", "m^2").get(), window.grossArea(), 0.001);
    EXPECT_EQ(0, window.shadingSurfaceGroups().size());  // no shades
  }
  EXPECT_NEAR(convert(800.0, "ft^2", "m^2").get(), infos[1].southSurface->grossArea(), 0.001);
  EXPECT_NEAR(convert(800.0 - 17 * 8.0, "ft^2", "m^2").get(), infos[1].southSurface->netArea(), 0.001);
  EXPECT_EQ(1, infos[1].dcs.size());

  EXPECT_EQ("Space 3", infos[2].name);
  EXPECT_EQ(3, infos[2].numExteriorWalls);
  ASSERT_TRUE(infos[2].southSurface);
  EXPECT_EQ(1, infos[2].windows.size());
  for (const auto& window : infos[2].windows) {
    EXPECT_NEAR(convert(0.4 * 800.0, "ft^2", "m^2").get(), window.grossArea(), 0.001);
    ASSERT_EQ(1u, window.shadingSurfaceGroups().size());
    EXPECT_EQ(1u, window.shadingSurfaceGroups()[0].shadingSurfaces().size());  // just overhang
  }
  EXPECT_NEAR(convert(800.0, "ft^2", "m^2").get(), infos[2].southSurface->grossArea(), 0.001);
  EXPECT_NEAR(convert(0.6 * 800.0, "ft^2", "m^2").get(), infos[2].southSurface->netArea(), 0.001);
  EXPECT_EQ(1, infos[2].dcs.size());
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_SimAUD_Paper) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan-paper.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/floorplan-paper.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_Shading) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan_shading.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/floorplan_shading.osm");
  model->save(outpath, true);

  ASSERT_EQ(1u, model->getConcreteModelObjects<Space>().size());
  ASSERT_EQ(2u, model->getConcreteModelObjects<ShadingSurfaceGroup>().size());

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Space 1 - 1");
  ASSERT_TRUE(space1);

  boost::optional<ShadingSurfaceGroup> shading1 = model->getConcreteModelObjectByName<ShadingSurfaceGroup>("Shading 1 - 1");
  boost::optional<ShadingSurfaceGroup> shading2 = model->getConcreteModelObjectByName<ShadingSurfaceGroup>("Shading 1 - 2");
  ASSERT_TRUE(shading1);
  ASSERT_TRUE(shading2);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_SplitLevel) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/split_level.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/split_level.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  EXPECT_EQ(2u, model->getConcreteModelObjects<BuildingStory>().size());
  EXPECT_EQ(3u, model->getConcreteModelObjects<Space>().size());

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Main Space");
  boost::optional<Space> space2 = model->getConcreteModelObjectByName<Space>("Split Level");
  boost::optional<Space> space3 = model->getConcreteModelObjectByName<Space>("Upper floor");
  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  ASSERT_TRUE(space3);

  ASSERT_TRUE(space1->buildingStory());
  EXPECT_EQ("Story 1", space1->buildingStory()->nameString());
  ASSERT_TRUE(space2->buildingStory());
  EXPECT_EQ("Story 1", space2->buildingStory()->nameString());
  ASSERT_TRUE(space3->buildingStory());
  EXPECT_EQ("Story 2", space3->buildingStory()->nameString());

  std::vector<Space> spaces;
  spaces.push_back(*space1);
  spaces.push_back(*space2);
  spaces.push_back(*space3);

  struct SpaceInfo
  {
    std::string name;
    unsigned numExteriorWalls;
    unsigned numInteriorWalls;
    double exteriorWallArea;
    double interiorWallArea;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    info.numExteriorWalls = 0;
    info.numInteriorWalls = 0;
    info.exteriorWallArea = 0;
    info.interiorWallArea = 0;
    for (const auto& surface : space.surfaces()) {
      if (surface.surfaceType() == "Wall") {
        if (surface.outsideBoundaryCondition() == "Outdoors") {
          info.numExteriorWalls += 1;
          info.exteriorWallArea += surface.grossArea();
        } else if (surface.outsideBoundaryCondition() == "Surface") {
          info.numInteriorWalls += 1;
          info.interiorWallArea += surface.grossArea();
        }
      }
    }
    infos.push_back(info);
  }

  ASSERT_EQ(3u, infos.size());

  EXPECT_EQ("Main Space", infos[0].name);
  EXPECT_EQ(4, infos[0].numExteriorWalls);
  EXPECT_EQ(1, infos[0].numInteriorWalls);
  EXPECT_NEAR(convert(3 * 800.0 + 400.0, "ft^2", "m^2").get(), infos[0].exteriorWallArea, 0.001);
  EXPECT_NEAR(convert(400.0, "ft^2", "m^2").get(), infos[0].interiorWallArea, 0.001);

  EXPECT_EQ("Split Level", infos[1].name);
  EXPECT_EQ(3, infos[1].numExteriorWalls);
  EXPECT_EQ(2, infos[1].numInteriorWalls);
  EXPECT_NEAR(convert(3 * 800.0, "ft^2", "m^2").get(), infos[1].exteriorWallArea, 0.001);
  EXPECT_NEAR(convert(2 * 400.0, "ft^2", "m^2").get(), infos[1].interiorWallArea, 0.001);

  EXPECT_EQ("Upper floor", infos[2].name);
  EXPECT_EQ(4, infos[2].numExteriorWalls);
  EXPECT_EQ(1, infos[2].numInteriorWalls);
  EXPECT_NEAR(convert(3 * 800.0 + 400.0, "ft^2", "m^2").get(), infos[2].exteriorWallArea, 0.001);
  EXPECT_NEAR(convert(400.0, "ft^2", "m^2").get(), infos[2].interiorWallArea, 0.001);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_StorySpaceHeights) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/story_space_heights.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/story_space_heights.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  EXPECT_EQ(1u, model->getConcreteModelObjects<BuildingStory>().size());
  EXPECT_EQ(4u, model->getConcreteModelObjects<Space>().size());
  EXPECT_EQ(4u, model->getConcreteModelObjects<ThermalZone>().size());

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Story Default");
  boost::optional<Space> space2 = model->getConcreteModelObjectByName<Space>("Story Default Plenum");
  boost::optional<Space> space3 = model->getConcreteModelObjectByName<Space>("Story Default Floor Plenum");
  boost::optional<Space> space4 = model->getConcreteModelObjectByName<Space>("Tall No Plenums");
  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  ASSERT_TRUE(space3);
  ASSERT_TRUE(space4);

  ASSERT_TRUE(space1->buildingStory());
  EXPECT_EQ("Story 1", space1->buildingStory()->nameString());
  ASSERT_TRUE(space2->buildingStory());
  EXPECT_EQ("Story 1", space2->buildingStory()->nameString());
  ASSERT_TRUE(space3->buildingStory());
  EXPECT_EQ("Story 1", space3->buildingStory()->nameString());
  ASSERT_TRUE(space4->buildingStory());
  EXPECT_EQ("Story 1", space4->buildingStory()->nameString());

  ASSERT_TRUE(space1->spaceType());
  EXPECT_EQ("Space Type 1", space1->spaceType()->nameString());
  ASSERT_TRUE(space2->spaceType());
  EXPECT_EQ("Plenum Space Type", space2->spaceType()->nameString());
  EXPECT_EQ(model->plenumSpaceType().handle(), space2->spaceType()->handle());
  ASSERT_TRUE(space3->spaceType());
  EXPECT_EQ("Plenum Space Type", space3->spaceType()->nameString());
  EXPECT_EQ(model->plenumSpaceType().handle(), space3->spaceType()->handle());
  ASSERT_TRUE(space4->spaceType());
  EXPECT_EQ("Space Type 1", space4->spaceType()->nameString());

  // plenums are assigned to unique zones, but the zones are not yet plenums as they are not connected to an air loop
  ASSERT_TRUE(space1->thermalZone());
  EXPECT_EQ("Thermal Zone 1", space1->thermalZone()->nameString());
  EXPECT_FALSE(space1->thermalZone()->isPlenum());
  ASSERT_TRUE(space2->thermalZone());
  EXPECT_EQ("Thermal Zone 1 Plenum", space2->thermalZone()->nameString());
  EXPECT_FALSE(space2->thermalZone()->isPlenum());
  ASSERT_TRUE(space3->buildingStory());
  EXPECT_EQ("Thermal Zone 1 Floor Plenum", space3->thermalZone()->nameString());
  EXPECT_FALSE(space3->thermalZone()->isPlenum());
  ASSERT_TRUE(space4->buildingStory());
  EXPECT_EQ("Thermal Zone 2", space4->thermalZone()->nameString());
  EXPECT_FALSE(space4->thermalZone()->isPlenum());

  std::vector<Space> spaces;
  spaces.push_back(*space1);
  spaces.push_back(*space2);
  spaces.push_back(*space3);
  spaces.push_back(*space4);

  double volume1 = 0;
  volume1 += space1->volume();
  volume1 += space2->volume();
  volume1 += space3->volume();

  EXPECT_NEAR(volume1, space4->volume(), 0.001);

  struct SpaceInfo
  {
    std::string name;
    unsigned numExterior;
    unsigned numInterior;
    double exteriorArea;
    double interiorArea;
    std::set<std::string> adjacentSpaces;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    info.numExterior = 0;
    info.numInterior = 0;
    info.exteriorArea = 0;
    info.interiorArea = 0;
    for (const auto& surface : space.surfaces()) {

      if ((surface.outsideBoundaryCondition() == "Outdoors") || (surface.outsideBoundaryCondition() == "Ground")) {
        info.numExterior += 1;
        info.exteriorArea += surface.grossArea();
      } else if (surface.outsideBoundaryCondition() == "Surface") {
        info.numInterior += 1;
        info.interiorArea += surface.grossArea();
        ASSERT_TRUE(surface.adjacentSurface());
        ASSERT_TRUE(surface.adjacentSurface()->space());
        info.adjacentSpaces.insert(surface.adjacentSurface()->space()->nameString());
      }
    }
    infos.push_back(info);
  }

  ASSERT_EQ(4u, infos.size());

  EXPECT_EQ("Story Default", infos[0].name);
  EXPECT_EQ(3, infos[0].numExterior);
  EXPECT_EQ(3, infos[0].numInterior);
  EXPECT_NEAR(convert(3 * 800.0, "ft^2", "m^2").get(), infos[0].exteriorArea, 0.001);
  EXPECT_NEAR(convert(2 * 100.0 * 100.0 + 800.0, "ft^2", "m^2").get(), infos[0].interiorArea, 0.001);
  EXPECT_EQ(3u, infos[0].adjacentSpaces.size());
  EXPECT_EQ(0u, infos[0].adjacentSpaces.count("Story Default"));
  EXPECT_EQ(1u, infos[0].adjacentSpaces.count("Story Default Plenum"));
  EXPECT_EQ(1u, infos[0].adjacentSpaces.count("Story Default Floor Plenum"));
  EXPECT_EQ(1u, infos[0].adjacentSpaces.count("Tall No Plenums"));

  EXPECT_EQ("Story Default Plenum", infos[1].name);
  EXPECT_EQ(4, infos[1].numExterior);
  EXPECT_EQ(2, infos[1].numInterior);
  EXPECT_NEAR(convert(3 * 200.0 + 100.0 * 100.0, "ft^2", "m^2").get(), infos[1].exteriorArea, 0.001);
  EXPECT_NEAR(convert(200.0 + 100.0 * 100.0, "ft^2", "m^2").get(), infos[1].interiorArea, 0.001);
  EXPECT_EQ(2u, infos[1].adjacentSpaces.size());
  EXPECT_EQ(1u, infos[1].adjacentSpaces.count("Story Default"));
  EXPECT_EQ(0u, infos[1].adjacentSpaces.count("Story Default Plenum"));
  EXPECT_EQ(0u, infos[1].adjacentSpaces.count("Story Default Floor Plenum"));
  EXPECT_EQ(1u, infos[1].adjacentSpaces.count("Tall No Plenums"));

  EXPECT_EQ("Story Default Floor Plenum", infos[2].name);
  EXPECT_EQ(4, infos[2].numExterior);
  EXPECT_EQ(2, infos[2].numInterior);
  EXPECT_NEAR(convert(3 * 200.0 + 100.0 * 100.0, "ft^2", "m^2").get(), infos[2].exteriorArea, 0.001);
  EXPECT_NEAR(convert(200.0 + 100.0 * 100.0, "ft^2", "m^2").get(), infos[2].interiorArea, 0.001);
  EXPECT_EQ(2u, infos[2].adjacentSpaces.size());
  EXPECT_EQ(1u, infos[2].adjacentSpaces.count("Story Default"));
  EXPECT_EQ(0u, infos[2].adjacentSpaces.count("Story Default Plenum"));
  EXPECT_EQ(0u, infos[2].adjacentSpaces.count("Story Default Floor Plenum"));
  EXPECT_EQ(1u, infos[2].adjacentSpaces.count("Tall No Plenums"));

  EXPECT_EQ("Tall No Plenums", infos[3].name);
  EXPECT_EQ(5, infos[3].numExterior);
  EXPECT_EQ(3, infos[3].numInterior);
  EXPECT_NEAR(convert(3 * 1200.0 + 2 * 100.0 * 100.0, "ft^2", "m^2").get(), infos[3].exteriorArea, 0.001);
  EXPECT_NEAR(convert(1200.0, "ft^2", "m^2").get(), infos[3].interiorArea, 0.001);
  EXPECT_EQ(3u, infos[3].adjacentSpaces.size());
  EXPECT_EQ(1u, infos[3].adjacentSpaces.count("Story Default"));
  EXPECT_EQ(1u, infos[3].adjacentSpaces.count("Story Default Plenum"));
  EXPECT_EQ(1u, infos[3].adjacentSpaces.count("Story Default Floor Plenum"));
  EXPECT_EQ(0u, infos[3].adjacentSpaces.count("Tall No Plenums"));
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_Colors) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan_colors.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/floorplan_colors.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0u, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  boost::optional<BuildingStory> story = model->getConcreteModelObjectByName<BuildingStory>("Black Story");
  boost::optional<Space> space = model->getConcreteModelObjectByName<Space>("White Space");
  boost::optional<ThermalZone> thermalZone = model->getConcreteModelObjectByName<ThermalZone>("Red Zone");
  boost::optional<SpaceType> spaceType = model->getConcreteModelObjectByName<SpaceType>("Green Type");
  boost::optional<DefaultConstructionSet> constructionSet = model->getConcreteModelObjectByName<DefaultConstructionSet>("Blue Set");
  ASSERT_TRUE(story);
  ASSERT_TRUE(space);
  ASSERT_TRUE(thermalZone);
  ASSERT_TRUE(spaceType);
  ASSERT_TRUE(constructionSet);

  ASSERT_TRUE(story->renderingColor());
  EXPECT_EQ(0, story->renderingColor()->renderingRedValue());
  EXPECT_EQ(0, story->renderingColor()->renderingGreenValue());
  EXPECT_EQ(0, story->renderingColor()->renderingBlueValue());
  EXPECT_EQ(255, story->renderingColor()->renderingAlphaValue());

  //ASSERT_TRUE(space->renderingColor());
  //EXPECT_EQ(255, space->renderingColor()->renderingRedValue());
  //EXPECT_EQ(255, space->renderingColor()->renderingGreenValue());
  //EXPECT_EQ(255, space->renderingColor()->renderingBlueValue());
  //EXPECT_EQ(255, space->renderingColor()->renderingAlphaValue());

  ASSERT_TRUE(thermalZone->renderingColor());
  EXPECT_EQ(255, thermalZone->renderingColor()->renderingRedValue());
  EXPECT_EQ(0, thermalZone->renderingColor()->renderingGreenValue());
  EXPECT_EQ(0, thermalZone->renderingColor()->renderingBlueValue());
  EXPECT_EQ(255, thermalZone->renderingColor()->renderingAlphaValue());

  ASSERT_TRUE(spaceType->renderingColor());
  EXPECT_EQ(0, spaceType->renderingColor()->renderingRedValue());
  EXPECT_EQ(255, spaceType->renderingColor()->renderingGreenValue());
  EXPECT_EQ(0, spaceType->renderingColor()->renderingBlueValue());
  EXPECT_EQ(255, spaceType->renderingColor()->renderingAlphaValue());

  //ASSERT_TRUE(constructionSet->renderingColor());
  //EXPECT_EQ(0, story->renderingColor()->renderingRedValue());
  //EXPECT_EQ(0, story->renderingColor()->renderingGreenValue());
  //EXPECT_EQ(255, story->renderingColor()->renderingBlueValue());
  //EXPECT_EQ(255, story->renderingColor()->renderingAlphaValue());
}

// Modified this tes as now 4 vertices will be returned for both the ceiling of Space 1-1
// and the floor of Space 2-1. This is a more desirable outcome as Energy+ expects paired
// surfaces to have the same number of vertices/ (Though energy plus does remove collinear
// vertices in my tests it only removed one of the two so this model won't simulate)
TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_DifferingNumVertices) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan_differing_num_vertices.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/floorplan_differing_num_vertices.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0u, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Space 1 - 1");
  boost::optional<Space> space2 = model->getConcreteModelObjectByName<Space>("Space 2 - 1");
  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);

  std::vector<Space> spaces = model->getConcreteModelObjects<Space>();

  struct SpaceInfo
  {
    std::string name;
    boost::optional<Surface> floor;
    boost::optional<Surface> ceiling;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    for (const auto& surface : space.surfaces()) {
      if (surface.surfaceType() == "Floor") {
        EXPECT_FALSE(info.floor);
        info.floor = surface;
      } else if (surface.surfaceType() == "RoofCeiling") {
        EXPECT_FALSE(info.ceiling);
        info.ceiling = surface;
      }
    }
    infos.push_back(info);
  }

  std::sort(std::begin(infos), std::end(infos), [](SpaceInfo a, SpaceInfo b) { return a.name < b.name; });

  ASSERT_EQ(2u, infos.size());

  EXPECT_EQ("Space 1 - 1", infos[0].name);
  ASSERT_TRUE(infos[0].floor);
  EXPECT_EQ(6u, infos[0].floor->vertices().size());
  ASSERT_TRUE(infos[0].ceiling);
  EXPECT_EQ(4u, infos[0].ceiling->vertices().size());
  EXPECT_FALSE(infos[0].floor->adjacentSurface());
  ASSERT_TRUE(infos[0].ceiling->adjacentSurface());
  ASSERT_TRUE(infos[0].ceiling->adjacentSurface()->space());
  EXPECT_EQ("Space 2 - 1", infos[0].ceiling->adjacentSurface()->space()->nameString());

  EXPECT_EQ("Space 2 - 1", infos[1].name);
  ASSERT_TRUE(infos[1].floor);
  EXPECT_EQ(4u, infos[1].floor->vertices().size());
  ASSERT_TRUE(infos[1].ceiling);
  EXPECT_EQ(4u, infos[1].ceiling->vertices().size());
  EXPECT_FALSE(infos[1].ceiling->adjacentSurface());
  ASSERT_TRUE(infos[1].floor->adjacentSurface());
  ASSERT_TRUE(infos[1].floor->adjacentSurface()->space());
  EXPECT_EQ("Space 1 - 1", infos[1].floor->adjacentSurface()->space()->nameString());
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_School) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan_school.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/floorplan_school.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0u, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  std::vector<Space> spaces = model->getConcreteModelObjects<Space>();

  struct SpaceInfo
  {
    std::string name;
    std::vector<Surface> floors;
    std::vector<Surface> walls;
    std::vector<Surface> ceilings;
    unsigned numExteriorWalls = 0;
    unsigned numInteriorWalls = 0;
    double exteriorFloorArea = 0;
    double exteriorRoofArea = 0;
    double exteriorWallArea = 0;
    double interiorFloorArea = 0;
    double interiorRoofArea = 0;
    double interiorWallArea = 0;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    for (const auto& surface : space.surfaces()) {
      if (surface.surfaceType() == "Floor") {
        info.floors.push_back(surface);
        if (surface.outsideBoundaryCondition() == "Ground") {
          info.exteriorFloorArea += surface.grossArea();
        } else if (surface.outsideBoundaryCondition() == "Surface") {
          info.interiorFloorArea += surface.grossArea();
        }
      } else if (surface.surfaceType() == "RoofCeiling") {
        info.ceilings.push_back(surface);
        if (surface.outsideBoundaryCondition() == "Outdoors") {
          info.exteriorRoofArea += surface.grossArea();
        } else if (surface.outsideBoundaryCondition() == "Surface") {
          info.interiorRoofArea += surface.grossArea();
        }
      } else if (surface.surfaceType() == "Wall") {
        info.walls.push_back(surface);
        if (surface.outsideBoundaryCondition() == "Outdoors") {
          info.numExteriorWalls += 1;
          info.exteriorWallArea += surface.grossArea();
        } else if (surface.outsideBoundaryCondition() == "Surface") {
          info.numInteriorWalls += 1;
          info.interiorWallArea += surface.grossArea();
        }
      }
    }
    infos.push_back(info);
  }

  std::sort(std::begin(infos), std::end(infos), [](SpaceInfo a, SpaceInfo b) { return a.name < b.name; });

  ASSERT_EQ(33u, infos.size());

  auto it = std::find_if(infos.begin(), infos.end(), [](SpaceInfo a) { return a.name == "Space 2 - 1"; });
  ASSERT_TRUE(it != infos.end());
  EXPECT_EQ(1u, it->floors.size());
  EXPECT_NEAR(0.0, it->exteriorFloorArea, 1.0);
  EXPECT_NEAR(5796.0, it->interiorFloorArea, 1.0);
  EXPECT_EQ(1u, it->ceilings.size());
  EXPECT_NEAR(5796.0, it->exteriorRoofArea, 1.0);
  EXPECT_NEAR(0.0, it->interiorRoofArea, 1.0);
  EXPECT_EQ(9u, it->walls.size());
  EXPECT_EQ(2u, it->numExteriorWalls);
  EXPECT_EQ(7u, it->numInteriorWalls);
  EXPECT_NEAR(2178.75, it->exteriorWallArea, 1.0);
  EXPECT_NEAR(2546.25, it->interiorWallArea, 1.0);

  it = std::find_if(infos.begin(), infos.end(), [](SpaceInfo a) { return a.name == "Lobby 113"; });
  ASSERT_TRUE(it != infos.end());
  EXPECT_EQ(1u, it->floors.size());
  EXPECT_NEAR(1860.0, it->exteriorFloorArea, 1.0);
  EXPECT_NEAR(0.0, it->interiorFloorArea, 1.0);
  EXPECT_EQ(2u, it->ceilings.size());
  EXPECT_NEAR(600.0, it->exteriorRoofArea, 1.0);
  EXPECT_NEAR(1260.0, it->interiorRoofArea, 1.0);
  EXPECT_EQ(5u, it->walls.size());
  EXPECT_EQ(1u, it->numExteriorWalls);
  EXPECT_EQ(4u, it->numInteriorWalls);
  EXPECT_NEAR(813.75, it->exteriorWallArea, 1.0);
  EXPECT_NEAR(1601.25, it->interiorWallArea, 1.0);
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_StoryMultipliers) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/story_multipliers.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/story_multipliers.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(2u, rt.warnings().size());  // DLM: temporarily expect 2 warnings about non-translated multipliers

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  //for (const auto& warning : rt.warnings()){
  //  EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  //}
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_StoryMultipliers2) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/story_multipliers2.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/story_multipliers2.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(1u, rt.warnings().size());  // DLM: temporarily expect 1 warnings about non-translated multipliers

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  //for (const auto& warning : rt.warnings()){
  //  EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  //}
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_OpenToBelow) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/open_to_below.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  openstudio::path outpath = resourcesPath() / toPath("model/open_to_below.osm");
  model->save(outpath, true);

  EXPECT_EQ(0, rt.errors().size());
  EXPECT_EQ(0, rt.warnings().size());

  for (const auto& error : rt.errors()) {
    EXPECT_TRUE(false) << "Error: " << error.logMessage();
  }

  for (const auto& warning : rt.warnings()) {
    EXPECT_TRUE(false) << "Warning: " << warning.logMessage();
  }

  boost::optional<Space> space1 = model->getConcreteModelObjectByName<Space>("Open 1");
  boost::optional<Space> space2 = model->getConcreteModelObjectByName<Space>("Closed 1");
  boost::optional<Space> space3 = model->getConcreteModelObjectByName<Space>("Open 2");
  boost::optional<Space> space4 = model->getConcreteModelObjectByName<Space>("Closed 2");
  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  ASSERT_TRUE(space3);
  ASSERT_TRUE(space4);

  std::vector<Space> spaces;
  spaces.push_back(*space1);
  spaces.push_back(*space2);
  spaces.push_back(*space3);
  spaces.push_back(*space4);

  EXPECT_NEAR(convert(100.0 * 100.0, "ft^2", "m^2").get(), space1->floorArea(), 0.001);
  EXPECT_NEAR(convert(100.0 * 100.0, "ft^2", "m^2").get(), space2->floorArea(), 0.001);
  EXPECT_NEAR(convert(0.0, "ft^2", "m^2").get(), space3->floorArea(), 0.001);  // air wall floor
  EXPECT_NEAR(convert(100.0 * 100.0, "ft^2", "m^2").get(), space4->floorArea(), 0.001);

  struct SpaceInfo
  {
    std::string name;
    boost::optional<Surface> floor;
    boost::optional<Surface> ceiling;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces) {
    SpaceInfo info;
    info.name = space.nameString();
    for (const auto& surface : space.surfaces()) {
      if (surface.surfaceType() == "Floor") {
        EXPECT_FALSE(info.floor);
        info.floor = surface;
      } else if (surface.surfaceType() == "RoofCeiling") {
        EXPECT_FALSE(info.ceiling);
        info.ceiling = surface;
      }
    }
    infos.push_back(info);
  }

  ASSERT_EQ(4u, infos.size());

  EXPECT_EQ("Open 1", infos[0].name);
  ASSERT_TRUE(infos[0].floor);
  EXPECT_FALSE(infos[0].floor->isAirWall());
  EXPECT_FALSE(infos[0].floor->adjacentSurface());
  ASSERT_TRUE(infos[0].ceiling);
  EXPECT_TRUE(infos[0].ceiling->isAirWall());
  ASSERT_TRUE(infos[0].ceiling->adjacentSurface());
  ASSERT_TRUE(infos[0].ceiling->adjacentSurface()->space());
  EXPECT_EQ("Open 2", infos[0].ceiling->adjacentSurface()->space()->nameString());

  EXPECT_EQ("Closed 1", infos[1].name);
  ASSERT_TRUE(infos[1].floor);
  EXPECT_FALSE(infos[1].floor->isAirWall());
  EXPECT_FALSE(infos[1].floor->adjacentSurface());
  ASSERT_TRUE(infos[1].ceiling);
  EXPECT_FALSE(infos[1].ceiling->isAirWall());
  ASSERT_TRUE(infos[1].ceiling->adjacentSurface());
  ASSERT_TRUE(infos[1].ceiling->adjacentSurface()->space());
  EXPECT_EQ("Closed 2", infos[1].ceiling->adjacentSurface()->space()->nameString());

  EXPECT_EQ("Open 2", infos[2].name);
  ASSERT_TRUE(infos[2].floor);
  EXPECT_TRUE(infos[2].floor->isAirWall());
  ASSERT_TRUE(infos[2].floor->adjacentSurface());
  ASSERT_TRUE(infos[2].floor->adjacentSurface()->space());
  EXPECT_EQ("Open 1", infos[2].floor->adjacentSurface()->space()->nameString());
  ASSERT_TRUE(infos[2].ceiling);
  EXPECT_FALSE(infos[2].ceiling->isAirWall());
  EXPECT_FALSE(infos[2].ceiling->adjacentSurface());

  EXPECT_EQ("Closed 2", infos[3].name);
  ASSERT_TRUE(infos[3].floor);
  EXPECT_FALSE(infos[3].floor->isAirWall());
  ASSERT_TRUE(infos[3].floor->adjacentSurface());
  ASSERT_TRUE(infos[3].floor->adjacentSurface()->space());
  EXPECT_EQ("Closed 1", infos[3].floor->adjacentSurface()->space()->nameString());
  ASSERT_TRUE(infos[3].ceiling);
  EXPECT_FALSE(infos[3].ceiling->isAirWall());
  EXPECT_FALSE(infos[3].ceiling->adjacentSurface());
}

TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS_Site_ClimateZones_4166) {

  // Test for #4166 - Merging FloorSpaceJS strips out climate zone assignment.

  // Start with a base model, put some climate zone in there
  // Add a Climate Zone to model1 only
  Model model;
  auto czs = model.getUniqueModelObject<ClimateZones>();
  ClimateZone cz = czs.setClimateZone(ClimateZones::ashraeInstitutionName(), "4A");
  EXPECT_EQ(1, czs.numClimateZones());
  EXPECT_EQ(1, czs.climateZones().size());

  // To reproduce the original issue, we also need a Site object since it's the fact that the Site object is deleted, and along with it its children
  // and that includes the ClimateZones
  Site site = model.getUniqueModelObject<Site>();
  ASSERT_EQ(1, site.children().size());
  EXPECT_EQ(czs, site.children().front());

  // I'm going to instantiate the Building object as well, to check if the floorplan.json north_axis (30) is properly written anyways
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setNominalFloortoFloorHeight(2.5));

  // Now RT (any) floor plan back to a model
  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // not triangulated, for model transport/translation
  ThreeScene scene = floorPlan->toThreeScene(true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> newModel_ = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(newModel_);

  EXPECT_TRUE(model.getOptionalUniqueModelObject<ClimateZones>());
  EXPECT_FALSE(newModel_->getOptionalUniqueModelObject<ClimateZones>());

  EXPECT_EQ(0.0, building.northAxis());
  EXPECT_EQ(2.5, building.nominalFloortoFloorHeight().get());

  ASSERT_TRUE(newModel_->getOptionalUniqueModelObject<Building>());
  EXPECT_EQ(-30.0, newModel_->getOptionalUniqueModelObject<Building>()->northAxis());
  EXPECT_FALSE(newModel_->getOptionalUniqueModelObject<Building>()->nominalFloortoFloorHeight());

  model::ModelMerger mm;
  mm.mergeModels(model, newModel_.get(), rt.handleMapping());

  // Expect to still have retained the ClimateZone
  ASSERT_TRUE(model.getOptionalUniqueModelObject<ClimateZones>());
  EXPECT_EQ("4A", model.getOptionalUniqueModelObject<ClimateZones>()->climateZones()[0].value());
  EXPECT_EQ(ClimateZones::ashraeInstitutionName(), model.getOptionalUniqueModelObject<ClimateZones>()->climateZones()[0].institution());

  EXPECT_FALSE(newModel_->getOptionalUniqueModelObject<ClimateZones>());

  // It should have overridden only the things that were actually not defaulted, so building name and north axis
  ASSERT_TRUE(model.getOptionalUniqueModelObject<Building>());
  EXPECT_EQ(-30.0, model.getOptionalUniqueModelObject<Building>()->northAxis());
  ASSERT_TRUE(model.getOptionalUniqueModelObject<Building>()->nominalFloortoFloorHeight());
  EXPECT_EQ(2.5, model.getOptionalUniqueModelObject<Building>()->nominalFloortoFloorHeight().get());

  // New Model isn't touched anyways...
  ASSERT_TRUE(newModel_->getOptionalUniqueModelObject<Building>());
  EXPECT_EQ(-30.0, newModel_->getOptionalUniqueModelObject<Building>()->northAxis());
  EXPECT_FALSE(newModel_->getOptionalUniqueModelObject<Building>()->nominalFloortoFloorHeight());
}
