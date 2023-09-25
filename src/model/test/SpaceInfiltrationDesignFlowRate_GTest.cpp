/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceInfiltrationDesignFlowRate) {
  Model model;

  SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate(model);
  EXPECT_EQ(1u, model.numObjects());

  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.designFlowRate());
  EXPECT_EQ(0.0, spaceInfiltrationDesignFlowRate.designFlowRate().get());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceInfiltrationDesignFlowRate.setDesignFlowRate(1.0));
  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.designFlowRate());
  EXPECT_EQ(1.0, spaceInfiltrationDesignFlowRate.designFlowRate().get());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceInfiltrationDesignFlowRate.setFlowperSpaceFloorArea(2.0));
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.designFlowRate());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_EQ(2.0, spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea().get());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceInfiltrationDesignFlowRate.setFlowperExteriorSurfaceArea(3.0));
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_EQ(3.0, spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea().get());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceInfiltrationDesignFlowRate.setFlowperExteriorWallArea(4.0));
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  EXPECT_EQ(4.0, spaceInfiltrationDesignFlowRate.flowperExteriorWallArea().get());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.airChangesperHour());

  EXPECT_TRUE(spaceInfiltrationDesignFlowRate.setAirChangesperHour(5.0));
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.designFlowRate());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperSpaceFloorArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorSurfaceArea());
  EXPECT_FALSE(spaceInfiltrationDesignFlowRate.flowperExteriorWallArea());
  ASSERT_TRUE(spaceInfiltrationDesignFlowRate.airChangesperHour());
  EXPECT_EQ(5.0, spaceInfiltrationDesignFlowRate.airChangesperHour().get());

  boost::optional<model::ModelObject> clone1 = spaceInfiltrationDesignFlowRate.clone(spaceInfiltrationDesignFlowRate.model());
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<model::ModelObject> clone2 = clone1->cast<model::SpaceInfiltrationDesignFlowRate>().clone(clone1->model());
  EXPECT_EQ(3u, model.numObjects());

  auto spaceType1 = SpaceType(model);
  auto success = clone1->cast<model::SpaceInfiltrationDesignFlowRate>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  auto spaceType2 = SpaceType(model);
  success = clone2->cast<model::SpaceInfiltrationDesignFlowRate>().setSpaceType(spaceType2);
  ASSERT_TRUE(success);

  success = clone2->cast<model::SpaceInfiltrationDesignFlowRate>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  EXPECT_EQ(2, model.getObjectsByType(IddObjectType::OS_SpaceType).size());

  EXPECT_EQ(3, model.getObjectsByType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate).size());
}
