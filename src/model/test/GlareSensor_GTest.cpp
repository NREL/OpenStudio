/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../GlareSensor.hpp"
#include "../GlareSensor_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GlareSensor_Clone) {
  Model model;

  GlareSensor control(model);
  Space space(model);

  EXPECT_FALSE(control.space());
  EXPECT_EQ(0u, space.glareSensors().size());

  control.setSpace(space);

  ASSERT_TRUE(control.space());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(1u, space.glareSensors().size());

  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<GlareSensor>().size());
  ModelObject object = control.clone(model);
  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<GlareSensor>().size());

  ASSERT_TRUE(object.optionalCast<GlareSensor>());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(space.handle(), object.cast<GlareSensor>().space()->handle());
  EXPECT_EQ(2u, space.glareSensors().size());
}
