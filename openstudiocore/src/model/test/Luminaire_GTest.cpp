/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Model.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../Luminaire.hpp"
#include "../Luminaire_Impl.hpp"
#include "../LuminaireDefinition.hpp"
#include "../LuminaireDefinition_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/geometry/EulerAngles.hpp"
#include "../../utilities/geometry/Transformation.hpp"

#include <boost/math/constants/constants.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Luminaire)
{
  Model model;
  LuminaireDefinition definition(model);
  Luminaire luminaire(definition);
  Luminaire luminaire2(definition);

  Transformation translation = Transformation::translation(Vector3d(1,0,0));
  EXPECT_TRUE(luminaire.setTransformation(translation));
  Point3d position = luminaire.position();
  EXPECT_EQ(1.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(translation.matrix() == luminaire.transformation().matrix());

  Transformation rotation = Transformation::rotation(EulerAngles(0,0,boost::math::constants::pi<double>()/2.0));
  EXPECT_TRUE(luminaire.setTransformation(rotation));
  position = luminaire.position();
  EXPECT_EQ(0.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(rotation.matrix() == luminaire.transformation().matrix());

  Transformation transformation = translation*rotation;
  EXPECT_TRUE(luminaire.setTransformation(transformation));
  position = luminaire.position();
  EXPECT_EQ(1.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(transformation.matrix() == luminaire.transformation().matrix());

}

TEST_F(ModelFixture, Luminaire_Cost)
{
  Model model;
  LuminaireDefinition definition(model);
  Luminaire luminaire(definition);
  Luminaire luminaire2(definition);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("LED Bulbs", definition, 10.0, "CostPerArea", "Construction");
  EXPECT_FALSE(cost);

  cost = LifeCycleCost::createLifeCycleCost("LED Bulbs", definition, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  Space space(model);
  luminaire.setSpace(space);

  EXPECT_DOUBLE_EQ(10.0, cost->totalCost());

  luminaire.setMultiplier(4);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  SpaceType spaceType(model);
  space.setSpaceType(spaceType);
  luminaire2.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(50.0, cost->totalCost());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);
  thermalZone.setMultiplier(2);

  EXPECT_DOUBLE_EQ(100.0, cost->totalCost());
}
