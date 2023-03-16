/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
