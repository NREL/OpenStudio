/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ThreeJSReverseTranslator.hpp"
#include "../ModelMerger.hpp"
#include "../Model.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../DaylightingControl.hpp"
#include "../DaylightingControl_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"

#include "../../utilities/geometry/ThreeJS.hpp"
#include "../../utilities/geometry/FloorplanJS.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/units/QuantityConverter.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, ThreeJSReverseTranslator_FloorplanJS) {

  ThreeJSReverseTranslator rt;

  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan.json");
  ASSERT_TRUE(exists(p));

  boost::optional<FloorplanJS> floorPlan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorPlan);

  // triangulated, for display
  ThreeScene scene = floorPlan->toThreeScene(false);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  // not triangulated, for model transport/translation
  scene = floorPlan->toThreeScene(true);
  json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  boost::optional<Model> model = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / toPath("model/ThreeJS_FloorplanJS.osm"), true);

  openstudio::path out = resourcesPath() / toPath("model/ThreeJS_FloorplanJS.json");
  json = scene.toJSON(true);
  openstudio::filesystem::ofstream file(out);
  ASSERT_TRUE(file.is_open());
  file << json;
  file.close();
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

  EXPECT_EQ(4u, model->getModelObjects<Space>().size());
  EXPECT_EQ(24u, model->getModelObjects<Surface>().size());

  unsigned numMatched = 0;
  for (const auto& surface : model->getModelObjects<Surface>()) {
    if (surface.outsideBoundaryCondition() == "Surface"){
      EXPECT_TRUE(surface.adjacentSurface());
      ++numMatched;
    }
  }
  EXPECT_EQ(8u, numMatched);

  // merge export model into newModel
  model::Model newModel;
  model::ModelMerger mm;
  mm.mergeModels(newModel, *model, rt.handleMapping());

  EXPECT_EQ(4u, newModel.getModelObjects<Space>().size());
  EXPECT_EQ(24u, newModel.getModelObjects<Surface>().size());

  numMatched = 0;
  for (const auto& surface : newModel.getModelObjects<Surface>()) {
    if (surface.outsideBoundaryCondition() == "Surface"){
      EXPECT_TRUE(surface.adjacentSurface());
      ++numMatched;
    }
  }
  EXPECT_EQ(8u, numMatched);

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

  struct SpaceInfo{
    std::string name;
    unsigned numExteriorWalls;
    boost::optional<Surface> southSurface;
    std::vector<SubSurface> windows;
    std::vector<DaylightingControl> dcs;
  };

  std::vector<SpaceInfo> infos;
  for (const auto& space : spaces){
    SpaceInfo info;
    info.name = space.nameString();
    info.numExteriorWalls = 0;
    for (const auto& surface : space.surfaces()){
      if (surface.surfaceType() == "Wall"){
        if (surface.outsideBoundaryCondition() == "Outdoors"){
          info.numExteriorWalls += 1;
          if ((surface.azimuth() > degToRad(179)) && (surface.azimuth() < degToRad(181))){
            // only one
            EXPECT_FALSE(info.southSurface) << info.name;
            info.southSurface = surface;
            for (const auto& subSurface : surface.subSurfaces()){
              info.windows.push_back(subSurface);
            }
          }
        }
      }
    }

    for (const auto& dc : space.daylightingControls()){
      info.dcs.push_back(dc);
    }

    infos.push_back(info);
  }

  std::sort(std::begin(infos), std::end(infos), [](SpaceInfo a, SpaceInfo b) {return a.name < b.name; });

  ASSERT_EQ(3u, infos.size());

  EXPECT_EQ("Space 1", infos[0].name);
  EXPECT_EQ(3, infos[0].numExteriorWalls);
  ASSERT_TRUE(infos[0].southSurface);
  EXPECT_EQ(9, infos[0].windows.size());
  for (const auto& window : infos[0].windows){
    EXPECT_EQ(convert(8.0, "ft^2", "m^2").get(), window.grossArea());
  }
  EXPECT_EQ(convert(800.0, "ft^2", "m^2").get(), infos[0].southSurface->grossArea());
  EXPECT_EQ(convert(800.0 - 9*8.0, "ft^2", "m^2").get(), infos[0].southSurface->netArea());
  EXPECT_EQ(1, infos[0].dcs.size());

  EXPECT_EQ("Space 2", infos[1].name);
  EXPECT_EQ(2, infos[1].numExteriorWalls);
  ASSERT_TRUE(infos[1].southSurface);
  EXPECT_EQ(17, infos[1].windows.size());
  for (const auto& window : infos[1].windows){
    EXPECT_EQ(convert(8.0, "ft^2", "m^2").get(), window.grossArea());
  }
  EXPECT_EQ(convert(800.0, "ft^2", "m^2").get(), infos[1].southSurface->grossArea());
  EXPECT_EQ(convert(800.0 - 17*8.0, "ft^2", "m^2").get(), infos[1].southSurface->netArea());
  EXPECT_EQ(1, infos[1].dcs.size());

  EXPECT_EQ("Space 3", infos[2].name);
  EXPECT_EQ(3, infos[2].numExteriorWalls);
  ASSERT_TRUE(infos[2].southSurface);
  EXPECT_EQ(1, infos[2].windows.size());
  for (const auto& window : infos[2].windows){
    EXPECT_EQ(convert(0.4*800.0, "ft^2", "m^2").get(), window.grossArea());
  }
  EXPECT_EQ(convert(800.0, "ft^2", "m^2").get(), infos[2].southSurface->grossArea());
  EXPECT_EQ(convert(0.6*800.0, "ft^2", "m^2").get(), infos[2].southSurface->netArea());
  EXPECT_EQ(1, infos[2].dcs.size());

}
