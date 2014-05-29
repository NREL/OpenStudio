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

#include "../OutputControlReportingTolerances.hpp"
#include "../OutputControlReportingTolerances_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,OutputControlReportingTolerances_ToleranceforTimeHeatingSetpointNotMet_Quantity) {
  Model model;
  // TODO: Check constructor.
  OutputControlReportingTolerances outputControlReportingTolerances = model.getUniqueModelObject<OutputControlReportingTolerances>();

  Unit units = outputControlReportingTolerances.getToleranceforTimeHeatingSetpointNotMet(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeHeatingSetpointNotMet(testQ));
  Quantity q = outputControlReportingTolerances.getToleranceforTimeHeatingSetpointNotMet(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,OutputControlReportingTolerances_ToleranceforTimeCoolingSetpointNotMet_Quantity) {
  Model model;
  // TODO: Check constructor.
  OutputControlReportingTolerances outputControlReportingTolerances = model.getUniqueModelObject<OutputControlReportingTolerances>();

  Unit units = outputControlReportingTolerances.getToleranceforTimeCoolingSetpointNotMet(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeCoolingSetpointNotMet(testQ));
  Quantity q = outputControlReportingTolerances.getToleranceforTimeCoolingSetpointNotMet(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

