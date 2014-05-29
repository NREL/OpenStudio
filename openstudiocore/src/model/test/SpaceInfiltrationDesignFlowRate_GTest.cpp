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
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../SpaceInfiltrationDesignFlowRate_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, SpaceInfiltrationDesignFlowRate)
{
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
}
