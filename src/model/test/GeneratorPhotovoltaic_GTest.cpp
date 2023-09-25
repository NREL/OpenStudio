/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../GeneratorPhotovoltaic.hpp"
#include "../ShadingSurface.hpp"
#include "../Schedule.hpp"
#include "../PhotovoltaicPerformanceSimple.hpp"
#include "../PhotovoltaicPerformanceSimple_Impl.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"
#include "../PhotovoltaicPerformanceSandia.hpp"
#include "../PhotovoltaicPerformanceSandia_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"

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
  EXPECT_FALSE(panel.ratedThermaltoElectricalPowerRatio());
  // Should be false now that ELCD is not in ctor
  EXPECT_FALSE(panel.electricLoadCenterDistribution());

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

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
  EXPECT_FALSE(panel.ratedThermaltoElectricalPowerRatio());
  // Should be false now that ELCD is not in ctor
  EXPECT_FALSE(panel.electricLoadCenterDistribution());

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

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

TEST_F(ModelFixture, GeneratorPhotovoltaic_Sandia) {
  Model model;

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::sandia(model);
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
  EXPECT_FALSE(panel.ratedThermaltoElectricalPowerRatio());
  // Should be false now that ELCD is not in ctor
  EXPECT_FALSE(panel.electricLoadCenterDistribution());

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

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
  ASSERT_TRUE(performance.optionalCast<PhotovoltaicPerformanceSandia>());
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
  {
    GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::sandia(model);
    PhotovoltaicPerformance performance = panel.photovoltaicPerformance();

    std::vector<IdfObject> removed = performance.remove();
    EXPECT_EQ(0, removed.size());
    EXPECT_NO_THROW(panel.photovoltaicPerformance());

    removed = panel.remove();
    ASSERT_EQ(2, removed.size());
    EXPECT_EQ(GeneratorPhotovoltaic::iddObjectType(), removed[0].iddObject().type());
    EXPECT_EQ(PhotovoltaicPerformanceSandia::iddObjectType(), removed[1].iddObject().type());
  }
}
TEST_F(ModelFixture, GeneratorPhotovoltaic_ElectricLoadCenterDistribution) {
  Model model;

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);

  //should be 0 default ELCD attached to panel
  std::vector<ElectricLoadCenterDistribution> elcd = model.getConcreteModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(0u, elcd.size());
  EXPECT_FALSE(panel.electricLoadCenterDistribution());
  //Add a ELCD
  ElectricLoadCenterDistribution elcd1(model);
  EXPECT_TRUE(elcd1.addGenerator(panel));
  EXPECT_EQ(elcd1.handle(), panel.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getConcreteModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the panel to it which should remove the existing one attached to panel
  EXPECT_TRUE(elcd2.addGenerator(panel));
  EXPECT_EQ(0, elcd1.generators().size());
  EXPECT_EQ(elcd2.handle(), panel.electricLoadCenterDistribution().get().handle());
}
