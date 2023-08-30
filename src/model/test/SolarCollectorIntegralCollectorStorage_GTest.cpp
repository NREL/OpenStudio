/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SolarCollectorPerformanceIntegralCollectorStorage.hpp"
#include "../SolarCollectorPerformanceIntegralCollectorStorage_Impl.hpp"
#include "../SolarCollectorIntegralCollectorStorage.hpp"
#include "../SolarCollectorIntegralCollectorStorage_Impl.hpp"
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

TEST_F(ModelFixture, SolarCollectorIntegralCollectorStorage_SolarCollectorIntegralCollectorStorage) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SolarCollectorIntegralCollectorStorage testObject(m);
      SolarCollectorPerformanceIntegralCollectorStorage performance = testObject.solarCollectorPerformance();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SolarCollectorIntegralCollectorStorage_addToNode) {
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
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType()).size());

  SolarCollectorIntegralCollectorStorage collector(model);
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType()).size());

  SolarCollectorPerformanceIntegralCollectorStorage performance = collector.solarCollectorPerformance();

  EXPECT_TRUE(collector.addToNode(node));

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());

  EXPECT_FALSE(collector.surface());

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  ShadingSurface shadingSurface(points, model);

  EXPECT_TRUE(collector.setSurface(shadingSurface));
  ASSERT_TRUE(collector.surface());
  EXPECT_EQ(shadingSurface.handle(), collector.surface()->handle());
  ASSERT_EQ(1u, shadingSurface.solarCollectors().size());
  EXPECT_EQ(collector.handle(), shadingSurface.solarCollectors()[0].handle());

  collector.remove();

  EXPECT_EQ(0, shadingSurface.solarCollectors().size());

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType()).size());

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorIntegralCollectorStorage::iddObjectType()).size());
}

TEST_F(ModelFixture, SolarCollectorIntegralCollectorStorage_Clone) {
  {
    Model model;
    SolarCollectorIntegralCollectorStorage collector(model);
    SolarCollectorPerformanceIntegralCollectorStorage performance = collector.solarCollectorPerformance();

    ModelObject clone = collector.clone(model);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorIntegralCollectorStorage>());
    auto collector2 = clone.cast<SolarCollectorIntegralCollectorStorage>();
    SolarCollectorPerformanceIntegralCollectorStorage performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
  }
  {
    Model model;
    SolarCollectorIntegralCollectorStorage collector(model);
    SolarCollectorPerformanceIntegralCollectorStorage performance = collector.solarCollectorPerformance();

    Model model2;
    ModelObject clone = collector.clone(model2);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorIntegralCollectorStorage>());
    auto collector2 = clone.cast<SolarCollectorIntegralCollectorStorage>();
    SolarCollectorPerformanceIntegralCollectorStorage performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
  }
}
