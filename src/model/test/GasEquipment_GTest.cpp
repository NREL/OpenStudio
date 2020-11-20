/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../GasEquipment.hpp"
#include "../GasEquipment_Impl.hpp"
#include "../GasEquipmentDefinition.hpp"
#include "../GasEquipmentDefinition_Impl.hpp"
#include "../LifeCycleCost.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GasEquipment) {
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, GasEquipment_Cost) {
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Gas Hookup", definition, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  Space space(model);
  gasEquipment.setSpace(space);

  EXPECT_DOUBLE_EQ(10.0, cost->totalCost());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  SpaceType spaceType(model);
  gasEquipment.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  space.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  GasEquipment gasEquipment2(definition);
  gasEquipment2.setSpace(space);

  EXPECT_DOUBLE_EQ(80.0, cost->totalCost());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, GasEquipment_FractionsLatentRadiantLost) {
  Model m;
  GasEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
