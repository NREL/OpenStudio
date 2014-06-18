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

#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SimulationControl_LoadsConvergenceToleranceValue_Quantity) {
  Model model;
 
  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  Unit units = simulationControl.getLoadsConvergenceToleranceValue(true).units(); // Get IP units.

  double value(0.3);
  Quantity testQ(value,units);
  EXPECT_TRUE(simulationControl.setLoadsConvergenceToleranceValue(testQ));
  Quantity q = simulationControl.getLoadsConvergenceToleranceValue(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SimulationControl_TemperatureConvergenceToleranceValue_Quantity) {
  Model model;

  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  Unit units = simulationControl.getTemperatureConvergenceToleranceValue(true).units(); // Get IP units.

  double value(0.3);
  Quantity testQ(value,units);
  EXPECT_TRUE(simulationControl.setTemperatureConvergenceToleranceValue(testQ));
  Quantity q = simulationControl.getTemperatureConvergenceToleranceValue(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SimulationControl_LifeCycleCost) {
  Model model;

  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  // adding this cost (and many others) would be really weird, expect a throw
  EXPECT_THROW(LifeCycleCost cost(simulationControl), openstudio::Exception);

}
