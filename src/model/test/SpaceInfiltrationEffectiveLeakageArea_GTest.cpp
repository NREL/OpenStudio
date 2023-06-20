/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceInfiltrationEffectiveLeakageArea) {
  Model model;

  SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea(model);
  EXPECT_EQ(1u, model.numObjects());

  spaceInfiltrationEffectiveLeakageArea.setEffectiveAirLeakageArea(0.1);
  spaceInfiltrationEffectiveLeakageArea.setStackCoefficient(0.2);
  spaceInfiltrationEffectiveLeakageArea.setWindCoefficient(0.3);

  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.effectiveAirLeakageArea(), 0.1);
  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.stackCoefficient(), 0.2);
  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.windCoefficient(), 0.3);

  boost::optional<model::ModelObject> clone1 = spaceInfiltrationEffectiveLeakageArea.clone(spaceInfiltrationEffectiveLeakageArea.model());
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<model::ModelObject> clone2 = clone1->cast<model::SpaceInfiltrationEffectiveLeakageArea>().clone(clone1->model());
  EXPECT_EQ(3u, model.numObjects());

  auto spaceType1 = SpaceType(model);
  auto success = clone1->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  auto spaceType2 = SpaceType(model);
  success = clone2->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpaceType(spaceType2);
  ASSERT_TRUE(success);

  success = clone2->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  EXPECT_EQ(2, model.getObjectsByType(IddObjectType::OS_SpaceType).size());

  EXPECT_EQ(3, model.getObjectsByType(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea).size());
}
