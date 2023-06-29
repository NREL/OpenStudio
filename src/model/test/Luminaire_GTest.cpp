/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

TEST_F(ModelFixture, Luminaire) {
  Model model;
  LuminaireDefinition definition(model);
  Luminaire luminaire(definition);
  Luminaire luminaire2(definition);

  Transformation translation = Transformation::translation(Vector3d(1, 0, 0));
  EXPECT_TRUE(luminaire.setTransformation(translation));
  Point3d position = luminaire.position();
  EXPECT_EQ(1.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(translation.matrix() == luminaire.transformation().matrix());

  Transformation rotation = Transformation::rotation(EulerAngles(0, 0, boost::math::constants::pi<double>() / 2.0));
  EXPECT_TRUE(luminaire.setTransformation(rotation));
  position = luminaire.position();
  EXPECT_EQ(0.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(rotation.matrix() == luminaire.transformation().matrix());

  Transformation transformation = translation * rotation;
  EXPECT_TRUE(luminaire.setTransformation(transformation));
  position = luminaire.position();
  EXPECT_EQ(1.0, position.x());
  EXPECT_EQ(0.0, position.y());
  EXPECT_EQ(0.0, position.z());
  EXPECT_TRUE(transformation.matrix() == luminaire.transformation().matrix());
}

TEST_F(ModelFixture, Luminaire_Cost) {
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
