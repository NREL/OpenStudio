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

#include "../ThreeJSForwardTranslator.hpp"
#include "../ThreeJSReverseTranslator.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

#include "../../utilities/geometry/ThreeJS.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ThreeJSForwardTranslator_ExampleModel) {

  ThreeJSForwardTranslator ft;
  ThreeJSReverseTranslator rt;
  openstudio::path out;

  Model model = exampleModel();
  model.save(resourcesPath() / toPath("model/exampleModel.osm"), true);

  // triangulated, for display
  ThreeScene scene = ft.modelToThreeJS(model, true);
  std::string json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/ModelToThreeJSTriangulated.json");
  openstudio::filesystem::ofstream file1(out);
  ASSERT_TRUE(file1.is_open());
  file1 << json;
  file1.close();

  // not triangulated, for model transport/translation
  scene = ft.modelToThreeJS(model, false);
  json = scene.toJSON();
  EXPECT_TRUE(ThreeScene::load(json));

  out = resourcesPath() / toPath("model/ModelToThreeJSNonTriangulated.json");
  openstudio::filesystem::ofstream file2(out);
  ASSERT_TRUE(file2.is_open());
  file2 << json;
  file2.close();

  boost::optional<Model> model2 = rt.modelFromThreeJS(scene);
  ASSERT_TRUE(model2);

  model2->save(resourcesPath() / toPath("model/ModelToThreeJSToModel.osm"), true);

  EXPECT_EQ(model.getConcreteModelObjects<Space>().size(), model2->getConcreteModelObjects<Space>().size());
  EXPECT_EQ(model.getConcreteModelObjects<Surface>().size(), model2->getConcreteModelObjects<Surface>().size());
}
