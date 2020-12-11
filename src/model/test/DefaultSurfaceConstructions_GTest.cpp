/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../Material.hpp"
#include "../Material_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultSurfaceConstructions) {
  Model model;

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultSurfaceConstructions.floorConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.floorConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.floorConstruction()->handle());
  defaultSurfaceConstructions.resetFloorConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.floorConstruction());

  EXPECT_FALSE(defaultSurfaceConstructions.wallConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.wallConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.wallConstruction()->handle());
  defaultSurfaceConstructions.resetWallConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.wallConstruction());

  EXPECT_FALSE(defaultSurfaceConstructions.roofCeilingConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.roofCeilingConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.roofCeilingConstruction()->handle());
  defaultSurfaceConstructions.resetRoofCeilingConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.roofCeilingConstruction());
}

TEST_F(ModelFixture, DefaultSurfaceConstructions_Clone) {
  Model model;
  Model exampleModel = model::exampleModel();

  unsigned numConstructions(0);
  unsigned numMaterials(0);
  for (const DefaultSurfaceConstructions& object : exampleModel.getModelObjects<DefaultSurfaceConstructions>()) {
    object.clone(model);
    for (const ResourceObject& construction : object.resources()) {
      EXPECT_TRUE(construction.optionalCast<ConstructionBase>());
      ++numConstructions;
      numMaterials += construction.resources().size();
    }
  }
  EXPECT_EQ(exampleModel.getModelObjects<DefaultSurfaceConstructions>().size(), model.getModelObjects<DefaultSurfaceConstructions>().size());
  EXPECT_EQ(numConstructions, model.getModelObjects<ConstructionBase>().size());
  EXPECT_LE(model.getModelObjects<Material>().size(), numMaterials);  // some materials are reused

  // model.save(toPath("./model.osm"),true);
}
