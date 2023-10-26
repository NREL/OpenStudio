/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Surface.hpp"
#include "../RenderingColor.hpp"
#include "../ScheduleConstant.hpp"
#include "../ConstructionAirBoundary.hpp"
#include "../ConstructionAirBoundary_Impl.hpp"
#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ConstructionAirBoundary) {
  Model model;

  ConstructionAirBoundary construction(model);
  EXPECT_EQ(0, construction.getNetArea());
  EXPECT_FALSE(construction.setUFactor(0.1));
  EXPECT_FALSE(construction.setConductance(10));
  EXPECT_FALSE(construction.isOpaque());
  EXPECT_FALSE(construction.isFenestration());
  EXPECT_FALSE(construction.isSolarDiffusing());
  EXPECT_TRUE(construction.isModelPartition());
  EXPECT_FALSE(construction.isGreenRoof());
  EXPECT_FALSE(construction.uFactor());
  EXPECT_FALSE(construction.thermalConductance());
  EXPECT_FALSE(construction.heatCapacity());
  EXPECT_FALSE(construction.heatCapacity());
  EXPECT_FALSE(construction.interiorVisibleAbsorptance());
  EXPECT_FALSE(construction.exteriorVisibleAbsorptance());
  EXPECT_FALSE(construction.visibleTransmittance());
  EXPECT_FALSE(construction.renderingColor());

  EXPECT_EQ("None", construction.airExchangeMethod());
  EXPECT_EQ(0.0, construction.simpleMixingAirChangesPerHour());
  EXPECT_FALSE(construction.renderingColor());
  EXPECT_FALSE(construction.simpleMixingSchedule());

  EXPECT_TRUE(construction.setAirExchangeMethod("SimpleMixing"));
  EXPECT_TRUE(construction.setSimpleMixingAirChangesPerHour(1.0));

  RenderingColor color(model);
  construction.setRenderingColor(color);

  ScheduleConstant schedule(model);
  schedule.setValue(0.25);
  construction.setSimpleMixingSchedule(schedule);

  EXPECT_EQ("SimpleMixing", construction.airExchangeMethod());
  EXPECT_EQ(1.0, construction.simpleMixingAirChangesPerHour());
  ASSERT_TRUE(construction.renderingColor());
  EXPECT_EQ(color.handle(), construction.renderingColor()->handle());
  ASSERT_TRUE(construction.simpleMixingSchedule());
  EXPECT_EQ(schedule.handle(), construction.simpleMixingSchedule()->handle());

  // square with unit area
  std::vector<Point3d> points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  EXPECT_TRUE(surface.setConstruction(construction));
  ASSERT_TRUE(surface.construction());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());
  EXPECT_TRUE(surface.isAirWall());
  EXPECT_EQ(1.0, construction.getNetArea());

  // DEPRECATED
  EXPECT_EQ("GroupedZones", construction.solarAndDaylightingMethod());
  EXPECT_EQ("GroupedZones", construction.radiantExchangeMethod());
  EXPECT_FALSE(construction.setSolarAndDaylightingMethod("GroupedZones"));
  EXPECT_FALSE(construction.setRadiantExchangeMethod("GroupedZones"));
  EXPECT_EQ("GroupedZones", construction.solarAndDaylightingMethod());
  EXPECT_EQ("GroupedZones", construction.radiantExchangeMethod());
}
