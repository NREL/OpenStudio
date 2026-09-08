/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include <sstream>

#include "ModelFixture.hpp"

#include "../ComfortViewFactorAngles.hpp"
#include "../Model.hpp"
#include "../PeopleDefinition.hpp"
#include "../Surface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ComfortViewFactorAngles) {
  Model model;
  Point3dVector points{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  Surface surface(points, model);
  ComfortViewFactorAngles comfortViewFactorAngles(model);

  // Individual angle factors must remain within the IDD's inclusive range.
  EXPECT_THROW(AngleFactor(surface, -0.01), openstudio::Exception);
  EXPECT_THROW(AngleFactor(surface, 1.01), openstudio::Exception);

  // A valid group is stored, indexed by its Surface, and returned as an AngleFactor.
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 0.25));
  EXPECT_EQ(1u, comfortViewFactorAngles.numberofAngleFactors());
  EXPECT_EQ(0u, comfortViewFactorAngles.angleFactorIndex(surface).get());
  EXPECT_FALSE(comfortViewFactorAngles.getAngleFactor(1));

  const auto angleFactors = comfortViewFactorAngles.angleFactors();
  ASSERT_EQ(1u, angleFactors.size());
  EXPECT_EQ(surface.handle(), angleFactors.front().surface().handle());
  EXPECT_DOUBLE_EQ(0.25, angleFactors.front().angleFactor());
  EXPECT_EQ(AngleFactor(surface, 0.25), angleFactors.front());
  EXPECT_NE(AngleFactor(surface, 0.5), angleFactors.front());
  std::stringstream stream;
  stream << angleFactors.front();
  EXPECT_EQ("(surface='Surface 1', angle factor=0.25)", stream.str());

  // Reusing a Surface updates its existing group instead of creating a duplicate.
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(surface, 0.5));
  EXPECT_EQ(1u, comfortViewFactorAngles.numberofAngleFactors());
  EXPECT_DOUBLE_EQ(0.5, comfortViewFactorAngles.getAngleFactor(0)->angleFactor());

  // New groups may bring the total to one, but cannot make it exceed one.
  Surface otherSurfaceInModel(points, model);
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactor(otherSurfaceInModel, 0.5));
  EXPECT_EQ(2u, comfortViewFactorAngles.numberofAngleFactors());
  Surface thirdSurfaceInModel(points, model);
  EXPECT_FALSE(comfortViewFactorAngles.addAngleFactor(thirdSurfaceInModel, 0.01));
  EXPECT_EQ(2u, comfortViewFactorAngles.numberofAngleFactors());

  // A Surface in another Model cannot be referenced.
  Model otherModel;
  Surface otherSurface(points, otherModel);
  EXPECT_FALSE(comfortViewFactorAngles.angleFactorIndex(otherSurface));
  EXPECT_FALSE(comfortViewFactorAngles.addAngleFactor(otherSurface, 0.75));
  EXPECT_EQ(2u, comfortViewFactorAngles.numberofAngleFactors());

  PeopleDefinition peopleDefinition(model);
  EXPECT_FALSE(peopleDefinition.setSurfaceNameAngleFactorListName(otherSurface));
  EXPECT_EQ("EnclosureAveraged", peopleDefinition.meanRadiantTemperatureCalculationType());
  EXPECT_FALSE(peopleDefinition.surfaceNameAngleFactorListName());

  // Indexed removal deletes one group.
  comfortViewFactorAngles.removeAngleFactor(0);
  EXPECT_EQ(1u, comfortViewFactorAngles.numberofAngleFactors());
  comfortViewFactorAngles.removeAngleFactor(0);
  EXPECT_EQ(0u, comfortViewFactorAngles.numberofAngleFactors());

  // Bulk addition supports distinct Surface groups, and clearing removes them all.
  EXPECT_TRUE(comfortViewFactorAngles.addAngleFactors({AngleFactor(surface, 0.25), AngleFactor(otherSurfaceInModel, 0.75)}));
  EXPECT_EQ(2u, comfortViewFactorAngles.numberofAngleFactors());
  EXPECT_DOUBLE_EQ(0.25, comfortViewFactorAngles.getAngleFactor(0)->angleFactor());
  EXPECT_DOUBLE_EQ(0.75, comfortViewFactorAngles.getAngleFactor(1)->angleFactor());
  comfortViewFactorAngles.removeAllAngleFactors();
  EXPECT_TRUE(comfortViewFactorAngles.angleFactors().empty());
}
