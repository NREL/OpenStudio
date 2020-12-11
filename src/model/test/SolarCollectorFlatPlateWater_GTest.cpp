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
#include "../SolarCollectorPerformanceFlatPlate.hpp"
#include "../SolarCollectorPerformanceFlatPlate_Impl.hpp"
#include "../SolarCollectorFlatPlateWater.hpp"
#include "../SolarCollectorFlatPlateWater_Impl.hpp"
#include "../Surface.hpp"
#include "../ShadingSurface.hpp"
#include "../Schedule.hpp"
#include "../PlantLoop.hpp"
#include "../PumpConstantSpeed.hpp"
#include "../WaterHeaterMixed.hpp"
#include "../TemperingValve.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SolarCollectorFlatPlateWater_SolarCollectorFlatPlateWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SolarCollectorFlatPlateWater testObject(m);
      SolarCollectorPerformanceFlatPlate performance = testObject.solarCollectorPerformance();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SolarCollectorFlatPlateWater_addToNode) {
  Model model;

  PlantLoop plant(model);

  PumpConstantSpeed pump(model);
  {
    auto node = plant.supplyInletNode();
    pump.addToNode(node);
  }

  WaterHeaterMixed waterHeater(model);
  plant.addSupplyBranchForComponent(waterHeater);

  TemperingValve valve(model);
  plant.addSupplyBranchForComponent(valve);

  ASSERT_TRUE(waterHeater.supplyOutletModelObject());
  ASSERT_TRUE(waterHeater.supplyOutletModelObject()->optionalCast<Node>());
  Node node = waterHeater.supplyOutletModelObject()->cast<Node>();

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorFlatPlateWater::iddObjectType()).size());

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlateWater::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformanceFlatPlate::iddObjectType()).size());

  SolarCollectorFlatPlateWater collector(model);
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorFlatPlateWater::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorPerformanceFlatPlate::iddObjectType()).size());

  SolarCollectorPerformanceFlatPlate performance = collector.solarCollectorPerformance();
  EXPECT_NE(0.0, performance.grossArea());
  EXPECT_EQ("Water", performance.testFluid());
  EXPECT_NE(0.0, performance.testFlowRate());
  EXPECT_EQ("Inlet", performance.testCorrelationType());
  EXPECT_NE(0.0, performance.coefficient1ofEfficiencyEquation());
  EXPECT_NE(0.0, performance.coefficient2ofEfficiencyEquation());
  //EXPECT_FALSE(performance.coefficient3ofEfficiencyEquation());
  //EXPECT_TRUE(performance.coefficient2ofIncidentAngleModifier());
  //EXPECT_TRUE(performance.coefficient3ofIncidentAngleModifier());

  EXPECT_TRUE(collector.addToNode(node));

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(SolarCollectorFlatPlateWater::iddObjectType()).size());

  EXPECT_FALSE(collector.surface());

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  ShadingSurface shadingSurface(points, model);

  EXPECT_TRUE(collector.setSurface(shadingSurface));
  ASSERT_TRUE(collector.surface());
  EXPECT_EQ(shadingSurface.handle(), collector.surface()->handle());
  ASSERT_EQ(1u, shadingSurface.solarCollectors().size());
  EXPECT_EQ(collector.handle(), shadingSurface.solarCollectors()[0].handle());

  collector.remove();

  EXPECT_EQ(0, shadingSurface.solarCollectors().size());

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlateWater::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformanceFlatPlate::iddObjectType()).size());

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorFlatPlateWater::iddObjectType()).size());
}

TEST_F(ModelFixture, SolarCollectorFlatPlateWater_Clone) {
  {
    Model model;
    SolarCollectorFlatPlateWater collector(model);
    SolarCollectorPerformanceFlatPlate performance = collector.solarCollectorPerformance();

    ModelObject clone = collector.clone(model);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlateWater>());
    SolarCollectorFlatPlateWater collector2 = clone.cast<SolarCollectorFlatPlateWater>();
    SolarCollectorPerformanceFlatPlate performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
  }
  {
    Model model;
    SolarCollectorFlatPlateWater collector(model);
    SolarCollectorPerformanceFlatPlate performance = collector.solarCollectorPerformance();

    Model model2;
    ModelObject clone = collector.clone(model2);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlateWater>());
    SolarCollectorFlatPlateWater collector2 = clone.cast<SolarCollectorFlatPlateWater>();
    SolarCollectorPerformanceFlatPlate performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
  }
}
