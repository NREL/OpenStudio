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
#include "../Space_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../DesignSpecificationOutdoorAir.hpp"
#include "../DesignSpecificationOutdoorAir_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_Space) {
  Model model;

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);

  Space space(model);
  EXPECT_FALSE(space.designSpecificationOutdoorAir());
  EXPECT_TRUE(space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), space.designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_SpaceType) {
  Model model;

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);

  SpaceType spaceType(model);
  EXPECT_FALSE(spaceType.designSpecificationOutdoorAir());
  EXPECT_TRUE(spaceType.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));
  ASSERT_TRUE(spaceType.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), spaceType.designSpecificationOutdoorAir()->handle());

  Space space(model);
  EXPECT_TRUE(space.setSpaceType(spaceType));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), space.designSpecificationOutdoorAir()->handle());

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir2(model);
  EXPECT_TRUE(space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir2));
  ASSERT_TRUE(space.designSpecificationOutdoorAir());
  EXPECT_EQ(designSpecificationOutdoorAir2.handle(), space.designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_ThermalZone_2SameSpaces) {
  Model model;

  ThermalZone thermalZone(model);

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);

  Space space1(model);
  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  EXPECT_TRUE(space1.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));

  Space space2(model);
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  EXPECT_TRUE(space2.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir));

  EXPECT_EQ(1u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  boost::optional<Space> combinedSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(combinedSpace);
  EXPECT_FALSE(combinedSpace->isDesignSpecificationOutdoorAirDefaulted());
  ASSERT_TRUE(combinedSpace->designSpecificationOutdoorAir());
  EXPECT_EQ(1u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  EXPECT_EQ(designSpecificationOutdoorAir.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());
}

TEST_F(ModelFixture, DesignSpecificationOutdoorAir_ThermalZone_2DifferentSpaces) {
  Model model;

  // 1 m^2
  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  ThermalZone thermalZone(model);

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir1(model);
  EXPECT_TRUE(designSpecificationOutdoorAir1.setOutdoorAirFlowperPerson(0.0));
  EXPECT_TRUE(designSpecificationOutdoorAir1.setOutdoorAirFlowperFloorArea(2.0));

  DesignSpecificationOutdoorAir designSpecificationOutdoorAir2(model);
  EXPECT_TRUE(designSpecificationOutdoorAir2.setOutdoorAirFlowperPerson(3.0));

  Space space1(model);
  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  EXPECT_TRUE(space1.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir1));

  Surface surface1(points, model);
  surface1.setParent(space1);
  EXPECT_EQ(1.0, space1.floorArea());

  space1.setPeoplePerFloorArea(1.0);
  EXPECT_EQ(1.0, space1.numberOfPeople());

  Space space2(model);
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  EXPECT_TRUE(space2.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir2));

  Surface surface2(points, model);
  surface2.setParent(space2);
  EXPECT_EQ(1.0, space2.floorArea());

  space2.setPeoplePerFloorArea(2.0);
  EXPECT_EQ(2.0, space2.numberOfPeople());

  EXPECT_EQ(2u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  boost::optional<Space> combinedSpace = thermalZone.combineSpaces();
  ASSERT_TRUE(combinedSpace);
  EXPECT_FALSE(combinedSpace->isDesignSpecificationOutdoorAirDefaulted());
  ASSERT_TRUE(combinedSpace->designSpecificationOutdoorAir());
  EXPECT_EQ(3u, model.getModelObjects<DesignSpecificationOutdoorAir>().size());
  EXPECT_NE(designSpecificationOutdoorAir1.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());
  EXPECT_NE(designSpecificationOutdoorAir2.handle(), combinedSpace->designSpecificationOutdoorAir()->handle());

  EXPECT_EQ(1.0, combinedSpace->designSpecificationOutdoorAir()->outdoorAirFlowperFloorArea());  // (2*1 + 0*1)/2
  EXPECT_EQ(2.0, combinedSpace->designSpecificationOutdoorAir()->outdoorAirFlowperPerson());     // (0*1 + 3*2)/3
}
