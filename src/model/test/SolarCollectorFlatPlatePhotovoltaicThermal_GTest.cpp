/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
#include "../SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "../SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl.hpp"
#include "../SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "../SolarCollectorPerformancePhotovoltaicThermalSimple_Impl.hpp"
#include "../GeneratorPhotovoltaic.hpp"
#include "../PhotovoltaicPerformanceSimple.hpp"
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

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_SolarCollectorFlatPlatePhotovoltaicThermal_Ctors) {

  {
    Model m;
    SolarCollectorFlatPlatePhotovoltaicThermal testObject(m);
    auto performance = testObject.solarCollectorPerformance();
    EXPECT_TRUE(performance.optionalCast<SolarCollectorPerformancePhotovoltaicThermalSimple>());
  }
  {
    Model m;
    SolarCollectorPerformancePhotovoltaicThermalSimple performance(m);
    SolarCollectorFlatPlatePhotovoltaicThermal testObject(performance);
    EXPECT_TRUE(performance.optionalCast<SolarCollectorPerformancePhotovoltaicThermalSimple>());
    EXPECT_EQ(testObject.solarCollectorPerformance(), performance);
  }
  {
    Model m;
    SolarCollectorPerformancePhotovoltaicThermalBIPVT performance(m);
    SolarCollectorFlatPlatePhotovoltaicThermal testObject(performance);
    EXPECT_TRUE(performance.optionalCast<SolarCollectorPerformancePhotovoltaicThermalBIPVT>());
    EXPECT_EQ(testObject.solarCollectorPerformance(), performance);
  }
}

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_addToNode) {
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
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());

  SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());

  auto performance = collector.solarCollectorPerformance();

  EXPECT_TRUE(collector.addToNode(node));

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());

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

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
}

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_SetPerformance) {
  {
    Model model;

    SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
    EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_NO_THROW(collector.solarCollectorPerformance());

    EXPECT_TRUE(
      collector.setSolarCollectorPerformance(collector.solarCollectorPerformance().cast<SolarCollectorPerformancePhotovoltaicThermalSimple>()));
    EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(1u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_NO_THROW(collector.solarCollectorPerformance());
  }
  {
    Model model;

    SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
    SolarCollectorFlatPlatePhotovoltaicThermal collector2(model);
    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_NO_THROW(collector.solarCollectorPerformance());
    EXPECT_NO_THROW(collector2.solarCollectorPerformance());

    EXPECT_TRUE(
      collector.setSolarCollectorPerformance(collector2.solarCollectorPerformance().cast<SolarCollectorPerformancePhotovoltaicThermalSimple>()));
    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_NO_THROW(collector.solarCollectorPerformance());
    EXPECT_NO_THROW(collector2.solarCollectorPerformance());
    EXPECT_NE(collector.solarCollectorPerformance().handle(), collector2.solarCollectorPerformance().handle());
  }
}

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_PV) {
  Model model;

  SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
  auto performance = collector.solarCollectorPerformance();

  EXPECT_FALSE(collector.generatorPhotovoltaic());

  GeneratorPhotovoltaic pv = GeneratorPhotovoltaic::simple(model);
  EXPECT_TRUE(collector.setGeneratorPhotovoltaic(pv));
  ASSERT_TRUE(collector.generatorPhotovoltaic());

  collector.remove();

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(GeneratorPhotovoltaic::iddObjectType()).size());
  EXPECT_EQ(1u, model.getObjectsByType(PhotovoltaicPerformanceSimple::iddObjectType()).size());
}

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_Clone) {
  {
    Model model;
    SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
    auto performance = collector.solarCollectorPerformance();

    GeneratorPhotovoltaic pv = GeneratorPhotovoltaic::simple(model);
    EXPECT_TRUE(collector.setGeneratorPhotovoltaic(pv));

    Point3dVector points{
      {0, 1, 0},
      {0, 0, 0},
      {1, 0, 0},
      {1, 1, 0},
    };

    ShadingSurface shadingSurface(points, model);

    EXPECT_TRUE(collector.setSurface(shadingSurface));
    EXPECT_TRUE(pv.setSurface(shadingSurface));

    ModelObject clone = collector.clone(model);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlatePhotovoltaicThermal>());
    auto collector2 = clone.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    auto performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
    EXPECT_FALSE(collector2.surface());

    boost::optional<GeneratorPhotovoltaic> pv2 = collector2.generatorPhotovoltaic();
    ASSERT_TRUE(pv2);
    EXPECT_NE(pv.handle(), pv2->handle());
    EXPECT_NE(pv.photovoltaicPerformance().handle(), pv2->photovoltaicPerformance().handle());
    EXPECT_FALSE(pv2->surface());

    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(2u, model.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_EQ(2u, model.getObjectsByType(GeneratorPhotovoltaic::iddObjectType()).size());
    EXPECT_EQ(2u, model.getObjectsByType(PhotovoltaicPerformanceSimple::iddObjectType()).size());
    EXPECT_EQ(1u, model.getObjectsByType(ShadingSurface::iddObjectType()).size());
  }
  {
    Model model;
    SolarCollectorFlatPlatePhotovoltaicThermal collector(model);
    auto performance = collector.solarCollectorPerformance();

    GeneratorPhotovoltaic pv = GeneratorPhotovoltaic::simple(model);
    EXPECT_TRUE(collector.setGeneratorPhotovoltaic(pv));

    Point3dVector points{
      {0, 1, 0},
      {0, 0, 0},
      {1, 0, 0},
      {1, 1, 0},
    };

    ShadingSurface shadingSurface(points, model);

    EXPECT_TRUE(collector.setSurface(shadingSurface));
    EXPECT_TRUE(pv.setSurface(shadingSurface));

    Model model2;
    ModelObject clone = collector.clone(model2);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlatePhotovoltaicThermal>());
    auto collector2 = clone.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    auto performance2 = collector2.solarCollectorPerformance();

    EXPECT_NE(collector.handle(), collector2.handle());
    EXPECT_NE(performance.handle(), performance2.handle());
    EXPECT_FALSE(collector2.surface());

    boost::optional<GeneratorPhotovoltaic> pv2 = collector2.generatorPhotovoltaic();
    ASSERT_TRUE(pv2);
    EXPECT_NE(pv.handle(), pv2->handle());
    EXPECT_NE(pv.photovoltaicPerformance().handle(), pv2->photovoltaicPerformance().handle());
    EXPECT_FALSE(pv2->surface());

    EXPECT_EQ(1u, model2.getObjectsByType(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());
    EXPECT_EQ(1u, model2.getObjectsByType(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType()).size());
    EXPECT_EQ(1u, model2.getObjectsByType(GeneratorPhotovoltaic::iddObjectType()).size());
    EXPECT_EQ(1u, model2.getObjectsByType(PhotovoltaicPerformanceSimple::iddObjectType()).size());
    EXPECT_EQ(0u, model2.getObjectsByType(ShadingSurface::iddObjectType()).size());
  }
}
