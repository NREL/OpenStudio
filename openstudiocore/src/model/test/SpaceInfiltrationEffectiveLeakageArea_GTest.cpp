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

#include "../SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SpaceInfiltrationEffectiveLeakageArea_Quantity) {
  Model model;
  
  SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea(model);
  EXPECT_EQ(1u, model.numObjects());

  spaceInfiltrationEffectiveLeakageArea.setEffectiveAirLeakageArea(0.1);
  spaceInfiltrationEffectiveLeakageArea.setStackCoefficient(0.2);
  spaceInfiltrationEffectiveLeakageArea.setWindCoefficient(0.3);

  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.effectiveAirLeakageArea(), 0.1);
  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.stackCoefficient(), 0.2);
  EXPECT_EQ(spaceInfiltrationEffectiveLeakageArea.windCoefficient(), 0.3);
}

