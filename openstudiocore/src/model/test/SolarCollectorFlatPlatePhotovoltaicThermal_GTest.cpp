/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
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

TEST_F(ModelFixture, SolarCollectorFlatPlatePhotovoltaicThermal_SolarCollectorFlatPlatePhotovoltaicThermal)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SolarCollectorFlatPlatePhotovoltaicThermal testObject(m);
    SolarCollectorPerformancePhotovoltaicThermalSimple performance = testObject.solarCollectorPerformance();

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
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

  SolarCollectorPerformancePhotovoltaicThermalSimple performance = collector.solarCollectorPerformance();

  EXPECT_TRUE(collector.addToNode(node));

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType()).size());

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

    collector.setSolarCollectorPerformance(collector.solarCollectorPerformance());
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

    collector.setSolarCollectorPerformance(collector2.solarCollectorPerformance());
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
  SolarCollectorPerformancePhotovoltaicThermalSimple performance = collector.solarCollectorPerformance();

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
    SolarCollectorPerformancePhotovoltaicThermalSimple performance = collector.solarCollectorPerformance();

    GeneratorPhotovoltaic pv = GeneratorPhotovoltaic::simple(model);
    EXPECT_TRUE(collector.setGeneratorPhotovoltaic(pv));

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));

    ShadingSurface shadingSurface(points, model);

    EXPECT_TRUE(collector.setSurface(shadingSurface));
    EXPECT_TRUE(pv.setSurface(shadingSurface));

    ModelObject clone = collector.clone(model);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlatePhotovoltaicThermal>());
    SolarCollectorFlatPlatePhotovoltaicThermal collector2 = clone.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    SolarCollectorPerformancePhotovoltaicThermalSimple performance2 = collector2.solarCollectorPerformance();

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
    SolarCollectorPerformancePhotovoltaicThermalSimple performance = collector.solarCollectorPerformance();

    GeneratorPhotovoltaic pv = GeneratorPhotovoltaic::simple(model);
    EXPECT_TRUE(collector.setGeneratorPhotovoltaic(pv));

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));

    ShadingSurface shadingSurface(points, model);

    EXPECT_TRUE(collector.setSurface(shadingSurface));
    EXPECT_TRUE(pv.setSurface(shadingSurface));

    Model model2;
    ModelObject clone = collector.clone(model2);
    ASSERT_TRUE(clone.optionalCast<SolarCollectorFlatPlatePhotovoltaicThermal>());
    SolarCollectorFlatPlatePhotovoltaicThermal collector2 = clone.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    SolarCollectorPerformancePhotovoltaicThermalSimple performance2 = collector2.solarCollectorPerformance();

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
