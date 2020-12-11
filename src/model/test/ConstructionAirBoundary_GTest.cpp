/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  EXPECT_EQ("GroupedZones", construction.solarAndDaylightingMethod());
  EXPECT_EQ("GroupedZones", construction.radiantExchangeMethod());
  EXPECT_EQ("None", construction.airExchangeMethod());
  EXPECT_EQ(0.0, construction.simpleMixingAirChangesPerHour());
  EXPECT_FALSE(construction.renderingColor());
  EXPECT_FALSE(construction.simpleMixingSchedule());

  EXPECT_TRUE(construction.setSolarAndDaylightingMethod("GroupedZones"));
  EXPECT_TRUE(construction.setRadiantExchangeMethod("GroupedZones"));
  EXPECT_TRUE(construction.setAirExchangeMethod("SimpleMixing"));
  EXPECT_TRUE(construction.setSimpleMixingAirChangesPerHour(1.0));

  RenderingColor color(model);
  construction.setRenderingColor(color);

  ScheduleConstant schedule(model);
  schedule.setValue(0.25);
  construction.setSimpleMixingSchedule(schedule);

  EXPECT_EQ("GroupedZones", construction.solarAndDaylightingMethod());
  EXPECT_EQ("GroupedZones", construction.radiantExchangeMethod());
  EXPECT_EQ("SimpleMixing", construction.airExchangeMethod());
  EXPECT_EQ(1.0, construction.simpleMixingAirChangesPerHour());
  ASSERT_TRUE(construction.renderingColor());
  EXPECT_EQ(color.handle(), construction.renderingColor()->handle());
  ASSERT_TRUE(construction.simpleMixingSchedule());
  EXPECT_EQ(schedule.handle(), construction.simpleMixingSchedule()->handle());

  // square with unit area
  std::vector<Point3d> points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  Surface surface(points, model);
  EXPECT_TRUE(surface.setConstruction(construction));
  ASSERT_TRUE(surface.construction());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());
  EXPECT_TRUE(surface.isAirWall());
  EXPECT_EQ(1.0, construction.getNetArea());
}
