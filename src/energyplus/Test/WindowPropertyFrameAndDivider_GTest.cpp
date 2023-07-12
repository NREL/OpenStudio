/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::energyplus;
using namespace openstudio::model;

TEST_F(EnergyPlusFixture, WindowPropertyFrameAndDivider) {

  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices{
    {0, 0, 10},
    {0, 0, 0},
    {10, 0, 0},
    {10, 0, 10},
  };

  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices = {
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };

  SubSurface subSurface1(vertices, model);
  subSurface1.setName("No Offset");
  subSurface1.setSurface(surface);

  SubSurface subSurface2(vertices, model);
  subSurface2.setName("Offset");
  subSurface2.setSurface(surface);

  Vector3d normal = subSurface1.outwardNormal();
  EXPECT_DOUBLE_EQ(0.0, normal.x());
  EXPECT_DOUBLE_EQ(-1.0, normal.y());
  EXPECT_DOUBLE_EQ(0.0, normal.z());

  WindowPropertyFrameAndDivider frameAndDivider(model);
  frameAndDivider.setOutsideRevealDepth(1.0);

  EXPECT_FALSE(subSurface2.allowWindowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface2.setSubSurfaceType("GlassDoor"));
  EXPECT_TRUE(subSurface2.allowWindowPropertyFrameAndDivider());

  EXPECT_TRUE(subSurface2.setWindowPropertyFrameAndDivider(frameAndDivider));
  ASSERT_TRUE(subSurface2.windowPropertyFrameAndDivider());

  ForwardTranslator forwardTranslator;
  OptionalWorkspace outWorkspace = forwardTranslator.translateModel(model);
  ASSERT_TRUE(outWorkspace);

  ReverseTranslator reverseTranslator;
  OptionalModel outModel = reverseTranslator.translateWorkspace(*outWorkspace);
  ASSERT_TRUE(outModel);

  EXPECT_EQ(1u, outModel->getConcreteModelObjects<WindowPropertyFrameAndDivider>().size());

  boost::optional<SubSurface> testSubSurface = outModel->getConcreteModelObjectByName<SubSurface>("Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_TRUE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());

  testSubSurface = outModel->getConcreteModelObjectByName<SubSurface>("No Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_FALSE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());
}
