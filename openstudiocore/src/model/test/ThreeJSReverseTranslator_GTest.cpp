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
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

#include "../../utilities/geometry/ThreeJS.hpp"
#include "../../utilities/geometry/FloorplanJS.hpp"

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