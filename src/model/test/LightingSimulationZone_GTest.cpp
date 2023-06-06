/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../LightingSimulationZone.hpp"
#include "../RenderingColor.hpp"
#include "../Space.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LightingSimulationZone) {

  Model model;
  RenderingColor color(model);
  Space space1(model);
  Space space2(model);

  LightingSimulationZone lightingSimulationZone(model);
  EXPECT_FALSE(lightingSimulationZone.renderingColor());
  EXPECT_EQ(0u, lightingSimulationZone.spaces().size());

  EXPECT_TRUE(lightingSimulationZone.setRenderingColor(color));
  ASSERT_TRUE(lightingSimulationZone.renderingColor());
  EXPECT_EQ(color.handle(), lightingSimulationZone.renderingColor()->handle());

  lightingSimulationZone.resetRenderingColor();
  EXPECT_FALSE(lightingSimulationZone.renderingColor());

  EXPECT_TRUE(lightingSimulationZone.addSpace(space1));
  ASSERT_EQ(1u, lightingSimulationZone.spaces().size());
  EXPECT_EQ(space1.handle(), lightingSimulationZone.spaces()[0].handle());

  EXPECT_TRUE(lightingSimulationZone.addSpace(space2));
  ASSERT_EQ(2u, lightingSimulationZone.spaces().size());
  EXPECT_EQ(space1.handle(), lightingSimulationZone.spaces()[0].handle());
  EXPECT_EQ(space2.handle(), lightingSimulationZone.spaces()[1].handle());

  lightingSimulationZone.resetSpaces();
  EXPECT_EQ(0u, lightingSimulationZone.spaces().size());
}
