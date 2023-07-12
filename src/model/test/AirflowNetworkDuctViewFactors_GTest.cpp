/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkDuctViewFactors.hpp"
#include "../AirflowNetworkDuctViewFactors_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_DuctViewFactors_Basic) {
  // openstudio::Logger::instance().standardOutLogger().enable();
  // openstudio::Logger::instance().standardOutLogger().setLogLevel(Info);
  Model model;
  Point3dVector points;

  // square with unit area
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  Surface surface(points, model);
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  AirflowNetworkDuctViewFactors dvf(model);

  dvf.setViewFactor(surface, 1.0);

  dvf.setViewFactor(surface, 1.0);
}
