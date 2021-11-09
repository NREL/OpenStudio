/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../GltfForwardTranslator.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ConstructionAirBoundary.hpp"
#include "../Construction.hpp"

#include "../tiny_gltf.h"

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GltfForwardTranslator_ExampleModel) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/exampleModel_3rd.gltf");
  Model model = exampleModel();
  model.save(resourcesPath() / toPath("model/exampleModel.osm"), true);
  //model.save(resourcesPath() / toPath("model/example_updated.osm"), true);
  bool result = ft.modelToGLTF(model, true,output);
  ASSERT_TRUE(result);
}

TEST_F(ModelFixture, GltfForwardTranslator_LoadTest) {
  GltfForwardTranslator ft;
  openstudio::path input;
  input = resourcesPath() / toPath("utilities/Geometry/minimal_GLTF_File.gltf");
  bool result = ft.loadGLTF(input);
  ASSERT_TRUE(result);
}

TEST_F(ModelFixture, GltfForwardTranslator_CreateTriangleGLTFTest) {
  GltfForwardTranslator ft;
  openstudio::path output;
  openstudio::path output_2;
  output = resourcesPath() / toPath("utilities/Geometry/triangle.gltf");
  output_2 = resourcesPath() / toPath("utilities/Geometry/triangle_2.gltf");
  bool result = ft.CreateTriangleGLTF(output);
  ASSERT_TRUE(result);
  bool result_2 = ft.CreateTriangleGLTF_2(output_2);
  ASSERT_TRUE(result_2);
}


