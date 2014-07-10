/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
