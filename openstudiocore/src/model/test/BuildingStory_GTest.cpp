/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"

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

