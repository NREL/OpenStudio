/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../GeneratorPhotovoltaic.hpp"
#include "../ShadingSurface.hpp"
#include "../Schedule.hpp"
#include "../PhotovoltaicPerformanceSimple.hpp"
#include "../PhotovoltaicPerformanceSimple_Impl.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "../PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"
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
  //should be true now that ELDC is in ctor
  EXPECT_TRUE(panel.electricLoadCenterDistribution());

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
  EXPECT_FALSE(panel.ratedThermaltoElectricalPowerRatio());
  //should be true now that ELDC is in ctor
  EXPECT_TRUE(panel.electricLoadCenterDistribution());

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
TEST_F(ModelFixture, GeneratorPhotovoltaic_ElectricLoadCenterDistribution) {
  Model model;

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);

  //should be 1 default ELCD attached to panel
  std::vector<ElectricLoadCenterDistribution> elcd = model.getModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(1u, elcd.size());
  EXPECT_EQ(1u, elcd[0].generators().size());
  std::vector<Generator> generators = elcd[0].generators();
  EXPECT_EQ(generators[0].handle(), panel.handle());
  EXPECT_TRUE(panel.electricLoadCenterDistribution());
  EXPECT_EQ(elcd[0].handle(), panel.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the panel to it which should remove the existing one attached to panel
  EXPECT_TRUE(elcd2.addGenerator(panel));
  EXPECT_EQ(0, elcd[0].generators().size());
  EXPECT_EQ(elcd2.handle(), panel.electricLoadCenterDistribution().get().handle());
}
