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
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipment_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipmentDefinition_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricEquipment) {
  Model model;

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, ElectricEquipment_Cost) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment1(definition);
  ElectricEquipment electricEquipment2(definition);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Dusting", definition, 0.5, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost);
  EXPECT_EQ(1, cost->repeatPeriodYears());

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  electricEquipment1.setSpace(*space1);

  EXPECT_DOUBLE_EQ(50, cost->totalCost());

  electricEquipment2.setSpace(*space2);

  EXPECT_DOUBLE_EQ(100, cost->totalCost());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(250.0, cost->totalCost());

  building.setSpaceType(spaceType);
  electricEquipment1.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(300.0, cost->totalCost());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, ElectricEquipment_FractionsLatentRadiantLost) {
  Model m;
  ElectricEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
