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

#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceVentilationDesignFlowRate.hpp"
#include "../SpaceVentilationDesignFlowRate_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceVentilationDesignFlowRate) {
  Model model;

  SpaceVentilationDesignFlowRate spaceVentilationDesignFlowRate(model);
  EXPECT_EQ(1u, model.numObjects());

  ASSERT_TRUE(spaceVentilationDesignFlowRate.designFlowRate());
  EXPECT_EQ(0.0, spaceVentilationDesignFlowRate.designFlowRate().get());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceVentilationDesignFlowRate.setDesignFlowRate(1.0));
  ASSERT_TRUE(spaceVentilationDesignFlowRate.designFlowRate());
  EXPECT_EQ(1.0, spaceVentilationDesignFlowRate.designFlowRate().get());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceVentilationDesignFlowRate.setFlowperSpaceFloorArea(2.0));
  EXPECT_FALSE(spaceVentilationDesignFlowRate.designFlowRate());
  ASSERT_TRUE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_EQ(2.0, spaceVentilationDesignFlowRate.flowperSpaceFloorArea().get());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceVentilationDesignFlowRate.setFlowperExteriorSurfaceArea(3.0));
  EXPECT_FALSE(spaceVentilationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  ASSERT_TRUE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_EQ(3.0, spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea().get());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceVentilationDesignFlowRate.setFlowperExteriorWallArea(4.0));
  EXPECT_FALSE(spaceVentilationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  ASSERT_TRUE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_EQ(4.0, spaceVentilationDesignFlowRate.flowperExteriorWallArea().get());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceVentilationDesignFlowRate.setAirChangesperHour(5.0));
  EXPECT_FALSE(spaceVentilationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceVentilationDesignFlowRate.flowperExteriorWallArea());
  ASSERT_TRUE(spaceVentilationDesignFlowRate.airChangesperHour());
  EXPECT_EQ(5.0, spaceVentilationDesignFlowRate.airChangesperHour().get());

  boost::optional<model::ModelObject> clone1 = spaceVentilationDesignFlowRate.clone(spaceVentilationDesignFlowRate.model());
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<model::ModelObject> clone2 = clone1->cast<model::SpaceVentilationDesignFlowRate>().clone(clone1->model());
  EXPECT_EQ(3u, model.numObjects());

  auto spaceType1 = SpaceType(model);
  auto success = clone1->cast<model::SpaceVentilationDesignFlowRate>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  auto spaceType2 = SpaceType(model);
  success = clone2->cast<model::SpaceVentilationDesignFlowRate>().setSpaceType(spaceType2);
  ASSERT_TRUE(success);

  success = clone2->cast<model::SpaceVentilationDesignFlowRate>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  EXPECT_EQ(2, model.getObjectsByType(IddObjectType::OS_SpaceType).size());

  EXPECT_EQ(3, model.getObjectsByType(IddObjectType::OS_SpaceVentilation_DesignFlowRate).size());
}
