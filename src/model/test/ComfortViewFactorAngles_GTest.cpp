/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ComfortViewFactorAngles.hpp"
#include "../Model.hpp"
#include "../PeopleDefinition.hpp"
#include "../Surface.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ComfortViewFactorAngles) {
  Model model;
  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface surface(points, model);
  ComfortViewFactorAngles comfortViewFactorAngles(model);

  EXPECT_THROW(AngleFactor(surface, -0.01), openstudio::Exception);
  EXPECT_THROW(AngleFactor(surface, 1.01), openstudio::Exception);

  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 0.25));
  EXPECT_EQ(1u, comfortViewFactorAngles.numberofComfortViewFactorAngles());

  auto comfortViewFactorAngle = comfortViewFactorAngles.getComfortViewFactorAngle(0);
  ASSERT_TRUE(comfortViewFactorAngle);
  EXPECT_EQ(surface.handle(), comfortViewFactorAngle->surface().handle());
  EXPECT_DOUBLE_EQ(0.25, comfortViewFactorAngle->angleFactor());
  EXPECT_EQ(AngleFactor(surface, 0.25), *comfortViewFactorAngle);
  EXPECT_NE(AngleFactor(surface, 0.5), *comfortViewFactorAngle);
  EXPECT_FALSE(comfortViewFactorAngles.getComfortViewFactorAngle(1));

  Model otherModel;
  Surface otherSurface(points, otherModel);
  EXPECT_FALSE(comfortViewFactorAngles.addAngleFactor(otherSurface, 0.75));
  EXPECT_EQ(1u, comfortViewFactorAngles.numberofComfortViewFactorAngles());

  PeopleDefinition peopleDefinition(model);
  EXPECT_FALSE(peopleDefinition.setSurfaceNameAngleFactorListName(otherSurface));
  EXPECT_EQ("EnclosureAveraged", peopleDefinition.meanRadiantTemperatureCalculationType());
  EXPECT_FALSE(peopleDefinition.surfaceNameAngleFactorListName());

  comfortViewFactorAngles.removeComfortViewFactorAngle(0);
  EXPECT_EQ(0u, comfortViewFactorAngles.numberofComfortViewFactorAngles());

  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 1.0));
  comfortViewFactorAngles.removeAllComfortViewFactorAngles();
  EXPECT_TRUE(comfortViewFactorAngles.comfortViewFactorAngles().empty());
}
