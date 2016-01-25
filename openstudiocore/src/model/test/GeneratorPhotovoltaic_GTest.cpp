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
#include "../GeneratorPhotovoltaic.hpp"
#include "../ShadingSurface.hpp"
#include "../Schedule.hpp"
#include "../PhotovoltaicPerformanceSimple.hpp"
#include "../PhotovoltaicPerformanceSimple_Impl.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, GeneratorPhotovoltaic_Simple) {
  Model model;

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);
  EXPECT_FALSE(panel.surface());
  EXPECT_EQ("Decoupled", panel.heatTransferIntegrationMode());
  EXPECT_TRUE(panel.isHeatTransferIntegrationModeDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInParallel());
  EXPECT_TRUE(panel.isNumberOfModulesInParallelDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInSeries());
  EXPECT_TRUE(panel.isNumberOfModulesInSeriesDefaulted());
  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());

  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());
  EXPECT_FALSE(panel.ratedThermalToElectricalPowerRatio());
  EXPECT_FALSE(panel.electricLoadCenterDistribution());

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  ShadingSurface shadingSurface(points, model);

  EXPECT_TRUE(panel.setSurface(shadingSurface));
  EXPECT_TRUE(panel.surface());
  EXPECT_EQ(1u, shadingSurface.generatorPhotovoltaics().size());
  panel.resetSurface();
  EXPECT_TRUE(panel.setHeatTransferIntegrationMode("IntegratedSurfaceOutsideFace"));
  EXPECT_EQ("IntegratedSurfaceOutsideFace", panel.heatTransferIntegrationMode());
  EXPECT_FALSE(panel.isHeatTransferIntegrationModeDefaulted());
  panel.resetHeatTransferIntegrationMode();
  EXPECT_TRUE(panel.setNumberOfModulesInParallel(2.0));
  EXPECT_EQ(2.0, panel.numberOfModulesInParallel());
  EXPECT_FALSE(panel.isNumberOfModulesInParallelDefaulted());
  panel.resetNumberOfModulesInParallel();
  EXPECT_TRUE(panel.setNumberOfModulesInSeries(3.0));
  EXPECT_EQ(3.0, panel.numberOfModulesInSeries());
  EXPECT_FALSE(panel.isNumberOfModulesInSeriesDefaulted());
  panel.resetNumberOfModulesInSeries();
  panel.setRatedElectricPowerOutput(10.0);
  ASSERT_TRUE(panel.ratedElectricPowerOutput());
  EXPECT_EQ(10.0, panel.ratedElectricPowerOutput().get());
  panel.resetRatedElectricPowerOutput();
  auto schedule = model.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(panel.setAvailabilitySchedule(schedule));
  ASSERT_TRUE(panel.availabilitySchedule());
  panel.resetAvailabilitySchedule();

  PhotovoltaicPerformance performance = panel.photovoltaicPerformance();
  ASSERT_TRUE(performance.optionalCast<PhotovoltaicPerformanceSimple>());
}

TEST_F(ModelFixture, GeneratorPhotovoltaic_OneDiode) {
  Model model;

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::equivalentOneDiode(model);
  EXPECT_FALSE(panel.surface());
  EXPECT_EQ("Decoupled", panel.heatTransferIntegrationMode());
  EXPECT_TRUE(panel.isHeatTransferIntegrationModeDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInParallel());
  EXPECT_TRUE(panel.isNumberOfModulesInParallelDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInSeries());
  EXPECT_TRUE(panel.isNumberOfModulesInSeriesDefaulted());
  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());

  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());
  EXPECT_FALSE(panel.ratedThermalToElectricalPowerRatio());
  EXPECT_FALSE(panel.electricLoadCenterDistribution());

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  ShadingSurface shadingSurface(points, model);

  EXPECT_TRUE(panel.setSurface(shadingSurface));
  EXPECT_TRUE(panel.surface());
  panel.resetSurface();
  EXPECT_TRUE(panel.setHeatTransferIntegrationMode("DecoupledUllebergDynamic"));
  EXPECT_EQ("DecoupledUllebergDynamic", panel.heatTransferIntegrationMode());
  EXPECT_FALSE(panel.isHeatTransferIntegrationModeDefaulted());
  panel.resetHeatTransferIntegrationMode();
  EXPECT_TRUE(panel.setNumberOfModulesInParallel(2.0));
  EXPECT_EQ(2.0, panel.numberOfModulesInParallel());
  EXPECT_FALSE(panel.isNumberOfModulesInParallelDefaulted());
  panel.resetNumberOfModulesInParallel();
  EXPECT_TRUE(panel.setNumberOfModulesInSeries(3.0));
  EXPECT_EQ(3.0, panel.numberOfModulesInSeries());
  EXPECT_FALSE(panel.isNumberOfModulesInSeriesDefaulted());
  panel.resetNumberOfModulesInSeries();
  panel.setRatedElectricPowerOutput(10.0);
  ASSERT_TRUE(panel.ratedElectricPowerOutput());
  EXPECT_EQ(10.0, panel.ratedElectricPowerOutput().get());
  panel.resetRatedElectricPowerOutput();
  auto schedule = model.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(panel.setAvailabilitySchedule(schedule));
  ASSERT_TRUE(panel.availabilitySchedule());
  panel.resetAvailabilitySchedule();

  PhotovoltaicPerformance performance = panel.photovoltaicPerformance();
  ASSERT_TRUE(performance.optionalCast<PhotovoltaicPerformanceEquivalentOneDiode>());
}

TEST_F(ModelFixture, GeneratorPhotovoltaic_Delete) {
  Model model;

  {
    GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);
    PhotovoltaicPerformance performance = panel.photovoltaicPerformance();

    std::vector<IdfObject> removed = performance.remove();
    EXPECT_EQ(0, removed.size());
    EXPECT_NO_THROW(panel.photovoltaicPerformance());

    removed = panel.remove();
    ASSERT_EQ(2, removed.size());
    EXPECT_EQ(GeneratorPhotovoltaic::iddObjectType(), removed[0].iddObject().type());
    EXPECT_EQ(PhotovoltaicPerformanceSimple::iddObjectType(), removed[1].iddObject().type());
  }

  {
    GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::equivalentOneDiode(model);
    PhotovoltaicPerformance performance = panel.photovoltaicPerformance();

    std::vector<IdfObject> removed = performance.remove();
    EXPECT_EQ(0, removed.size());
    EXPECT_NO_THROW(panel.photovoltaicPerformance());

    removed = panel.remove();
    ASSERT_EQ(2, removed.size());
    EXPECT_EQ(GeneratorPhotovoltaic::iddObjectType(), removed[0].iddObject().type());
    EXPECT_EQ(PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType(), removed[1].iddObject().type());
  }

}
