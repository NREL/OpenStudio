/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../SolarCollectorPerformanceFlatPlate.hpp"
#include "../SolarCollectorFlatPlateWater.hpp"
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

TEST_F(ModelFixture, SolarCollectorFlatPlateWater_SolarCollectorFlatPlateWater)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SolarCollectorFlatPlateWater testObject(m);
    SolarCollectorPerformanceFlatPlate performance = testObject.solarCollectorPerformance();

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture, SolarCollectorFlatPlateWater_addToNode) {
  Model model;

  PlantLoop plant(model);

  PumpConstantSpeed pump(model);
  pump.addToNode(plant.supplyInletNode());

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
  EXPECT_TRUE(collector.surface());

  collector.remove();

  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorFlatPlateWater::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(SolarCollectorPerformanceFlatPlate::iddObjectType()).size());

  EXPECT_EQ(1u, plant.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, plant.supplyComponents(TemperingValve::iddObjectType()).size());
  EXPECT_EQ(0u, plant.supplyComponents(SolarCollectorFlatPlateWater::iddObjectType()).size());

}
