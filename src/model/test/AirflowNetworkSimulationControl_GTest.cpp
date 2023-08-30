/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkSimulationControl.hpp"
#include "../AirflowNetworkSimulationControl_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_SimulationControl) {
  Model model;

  auto simcon = model.getUniqueModelObject<AirflowNetworkSimulationControl>();
  EXPECT_TRUE(simcon.isAirflowNetworkControlDefaulted());
  EXPECT_TRUE(simcon.isWindPressureCoefficientTypeDefaulted());
  EXPECT_TRUE(simcon.isHeightSelectionforLocalWindPressureCalculationDefaulted());
  EXPECT_TRUE(simcon.isBuildingTypeDefaulted());
  EXPECT_TRUE(simcon.isMaximumNumberofIterationsDefaulted());
  EXPECT_TRUE(simcon.isInitializationTypeDefaulted());
  EXPECT_TRUE(simcon.isRelativeAirflowConvergenceToleranceDefaulted());
  EXPECT_TRUE(simcon.isAbsoluteAirflowConvergenceToleranceDefaulted());
  EXPECT_TRUE(simcon.isConvergenceAccelerationLimitDefaulted());
  EXPECT_TRUE(simcon.isAzimuthAngleofLongAxisofBuildingDefaulted());
  EXPECT_TRUE(simcon.isBuildingAspectRatioDefaulted());
  EXPECT_TRUE(simcon.isHeightDependenceofExternalNodeTemperatureDefaulted());

  EXPECT_TRUE(simcon.isSolverDefaulted());
  EXPECT_EQ("SkylineLU", simcon.solver());

  EXPECT_TRUE(simcon.setSolver("ConjugateGradient"));
  EXPECT_FALSE(simcon.isSolverDefaulted());
  EXPECT_EQ("ConjugateGradient", simcon.solver());

  simcon.resetSolver();
  EXPECT_TRUE(simcon.isSolverDefaulted());
  EXPECT_EQ("SkylineLU", simcon.solver());

  EXPECT_TRUE(simcon.isAllowUnsupportedZoneEquipmentDefaulted());
  EXPECT_FALSE(simcon.allowUnsupportedZoneEquipment());

  EXPECT_TRUE(simcon.setAllowUnsupportedZoneEquipment(true));
  EXPECT_FALSE(simcon.isAllowUnsupportedZoneEquipmentDefaulted());
  EXPECT_TRUE(simcon.allowUnsupportedZoneEquipment());

  simcon.resetAllowUnsupportedZoneEquipment();
  EXPECT_TRUE(simcon.isAllowUnsupportedZoneEquipmentDefaulted());
  EXPECT_FALSE(simcon.allowUnsupportedZoneEquipment());
}
