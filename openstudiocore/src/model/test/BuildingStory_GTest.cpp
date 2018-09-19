/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../RenderingColor.hpp"
#include "../RenderingColor_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,BuildingStory_NominalZCoordinate) {
  Model model;
  // TODO: Check constructor.
  BuildingStory buildingStory(model);

  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  buildingStory.setNominalZCoordinate(value);
  boost::optional<double> result = buildingStory.nominalZCoordinate();
  ASSERT_TRUE(result.is_initialized());
  EXPECT_NEAR(value, result.get(), 1.0E-8);
}

TEST_F(ModelFixture,BuildingStory_NominalFloortoFloorHeight) {
  Model model;

  BuildingStory buildingStory(model);
  EXPECT_FALSE(buildingStory.nominalFloortoFloorHeight());

  double value(1.0);
  EXPECT_TRUE(buildingStory.setNominalFloortoFloorHeight(value));
  boost::optional<double> result = buildingStory.nominalFloortoFloorHeight();
  ASSERT_TRUE(result);
  EXPECT_NEAR(value, *result, 1.0E-8);
}

TEST_F(ModelFixture, BuildingStory_RenderingColor) {
  Model m;

  {
    BuildingStory buildingStory(m);
    RenderingColor renderingColor(m);
    EXPECT_TRUE(buildingStory.setRenderingColor(renderingColor));

    EXPECT_EQ(1u, m.getModelObjects<RenderingColor>().size());

    // Delete BuildingStory, since the color is referenced, it should be deleted too
    EXPECT_EQ(2u, buildingStory.remove().size());
    EXPECT_EQ(0u, m.getModelObjects<BuildingStory>().size());
    EXPECT_EQ(0u, m.getModelObjects<RenderingColor>().size());
  }

  {
    BuildingStory buildingStory(m);
    RenderingColor renderingColor(m);
    EXPECT_TRUE(buildingStory.setRenderingColor(renderingColor));

    // Test clone
    ModelObject mo = buildingStory.clone(m);
    EXPECT_EQ(2u, m.getModelObjects<BuildingStory>().size());
    EXPECT_EQ(2u, m.getModelObjects<RenderingColor>().size());

    BuildingStory buildingStoryClone = mo.cast<BuildingStory>();
    EXPECT_NE(buildingStory.handle(), buildingStoryClone.handle());
    boost::optional<RenderingColor> _r = buildingStoryClone.renderingColor();
    ASSERT_TRUE(_r);
    RenderingColor renderingColor2 = _r.get();
    EXPECT_NE(renderingColor.handle(), renderingColor2.handle());

    // Enforce 0 or 1 parent only
    // This should return false, and nothing done
    EXPECT_FALSE(buildingStory.setRenderingColor(renderingColor2));
    // That means buildingStory still has its original rendering color
    ASSERT_TRUE(buildingStory.renderingColor());
    EXPECT_EQ(renderingColor.handle(), buildingStory.renderingColor()->handle());

    // Test remove, clone should get remove with its rendering color
    EXPECT_EQ(2u, buildingStoryClone.remove().size());
    EXPECT_EQ(1u, m.getModelObjects<BuildingStory>().size());
    EXPECT_EQ(1u, m.getModelObjects<RenderingColor>().size());
  }
}
