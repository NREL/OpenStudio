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

#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ScheduleTypeLimits_LowerLimitValue_Quantity) {
  Model model;
  // TODO: Check constructor.
  ScheduleTypeLimits scheduleTypeLimits(model);

  Unit units = scheduleTypeLimits.getLowerLimitValue(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(scheduleTypeLimits.setLowerLimitValue(testQ));
  ASSERT_TRUE(scheduleTypeLimits.lowerLimitValue());
  EXPECT_EQ(1.0,scheduleTypeLimits.lowerLimitValue().get());
  OSOptionalQuantity q = scheduleTypeLimits.getLowerLimitValue(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ScheduleTypeLimits_UpperLimitValue_Quantity) {
  Model model;
  // TODO: Check constructor.
  ScheduleTypeLimits scheduleTypeLimits(model);

  Unit units = scheduleTypeLimits.getUpperLimitValue(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(scheduleTypeLimits.setUpperLimitValue(testQ));
  OSOptionalQuantity q = scheduleTypeLimits.getUpperLimitValue(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

