/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SurfacePropertyConvectionCoefficientsMultipleSurface.hpp"
#include "../SurfacePropertyConvectionCoefficientsMultipleSurface_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyConvectionCoefficientsMultipleSurface) {
  Model model;

  SurfacePropertyConvectionCoefficientsMultipleSurface convectionCoefficients(model);
}
