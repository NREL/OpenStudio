/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../InternalMass.hpp"
#include "../InternalMass_Impl.hpp"
#include "../InternalMassDefinition.hpp"
#include "../InternalMassDefinition_Impl.hpp"
#include "../SurfacePropertyConvectionCoefficients.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, InternalMass) {
  Model model;

  InternalMassDefinition definition(model);
  InternalMass internalMass(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, InternalMass_Clone) {
  Model model;

  InternalMassDefinition definition(model);
  InternalMass internalMass1(definition);
  SurfacePropertyConvectionCoefficients cc(internalMass1);

  auto internalMass2 = internalMass1.clone(model).cast<InternalMass>();
  EXPECT_TRUE(internalMass2.surfacePropertyConvectionCoefficients());
}
