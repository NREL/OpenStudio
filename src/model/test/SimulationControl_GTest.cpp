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

#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../LifeCycleCost.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SimulationControl_GettersSetters) {
  Model model;

  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  // Do Zone Sizing Calculation:  Boolean
  // Check Idd default: false
  EXPECT_TRUE(simulationControl.isDoZoneSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doZoneSizingCalculation());
  // Test true
  EXPECT_TRUE(simulationControl.setDoZoneSizingCalculation(true));
  EXPECT_TRUE(simulationControl.doZoneSizingCalculation());
  EXPECT_FALSE(simulationControl.isDoZoneSizingCalculationDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setDoZoneSizingCalculation(false));
  EXPECT_FALSE(simulationControl.doZoneSizingCalculation());
  // Test reset
  simulationControl.resetDoZoneSizingCalculation();
  EXPECT_TRUE(simulationControl.isDoZoneSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doZoneSizingCalculation());

  // Do System Sizing Calculation:  Boolean
  // Check Idd default: false
  EXPECT_TRUE(simulationControl.isDoSystemSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doSystemSizingCalculation());
  // Test true
  EXPECT_TRUE(simulationControl.setDoSystemSizingCalculation(true));
  EXPECT_TRUE(simulationControl.doSystemSizingCalculation());
  EXPECT_FALSE(simulationControl.isDoSystemSizingCalculationDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setDoSystemSizingCalculation(false));
  EXPECT_FALSE(simulationControl.doSystemSizingCalculation());
  // Test reset
  simulationControl.resetDoSystemSizingCalculation();
  EXPECT_TRUE(simulationControl.isDoSystemSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doSystemSizingCalculation());

  // Do Plant Sizing Calculation:  Boolean
  // Check Idd default: false
  EXPECT_TRUE(simulationControl.isDoPlantSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doPlantSizingCalculation());
  // Test true
  EXPECT_TRUE(simulationControl.setDoPlantSizingCalculation(true));
  EXPECT_TRUE(simulationControl.doPlantSizingCalculation());
  EXPECT_FALSE(simulationControl.isDoPlantSizingCalculationDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setDoPlantSizingCalculation(false));
  EXPECT_FALSE(simulationControl.doPlantSizingCalculation());
  // Test reset
  simulationControl.resetDoPlantSizingCalculation();
  EXPECT_TRUE(simulationControl.isDoPlantSizingCalculationDefaulted());
  EXPECT_FALSE(simulationControl.doPlantSizingCalculation());

  // Run Simulation for Sizing Periods:  Boolean
  // Check Idd default: true
  EXPECT_TRUE(simulationControl.isRunSimulationforSizingPeriodsDefaulted());
  EXPECT_TRUE(simulationControl.runSimulationforSizingPeriods());
  // Test true
  EXPECT_TRUE(simulationControl.setRunSimulationforSizingPeriods(true));
  EXPECT_TRUE(simulationControl.runSimulationforSizingPeriods());
  EXPECT_FALSE(simulationControl.isRunSimulationforSizingPeriodsDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setRunSimulationforSizingPeriods(false));
  EXPECT_FALSE(simulationControl.runSimulationforSizingPeriods());
  // Test reset
  simulationControl.resetRunSimulationforSizingPeriods();
  EXPECT_TRUE(simulationControl.isRunSimulationforSizingPeriodsDefaulted());
  EXPECT_TRUE(simulationControl.runSimulationforSizingPeriods());

  // Run Simulation for Weather File Run Periods:  Boolean
  // Check Idd default: true
  EXPECT_TRUE(simulationControl.isRunSimulationforWeatherFileRunPeriodsDefaulted());
  EXPECT_TRUE(simulationControl.runSimulationforWeatherFileRunPeriods());
  // Test true
  EXPECT_TRUE(simulationControl.setRunSimulationforWeatherFileRunPeriods(true));
  EXPECT_TRUE(simulationControl.runSimulationforWeatherFileRunPeriods());
  EXPECT_FALSE(simulationControl.isRunSimulationforWeatherFileRunPeriodsDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setRunSimulationforWeatherFileRunPeriods(false));
  EXPECT_FALSE(simulationControl.runSimulationforWeatherFileRunPeriods());
  // Test reset
  simulationControl.resetRunSimulationforWeatherFileRunPeriods();
  EXPECT_TRUE(simulationControl.isRunSimulationforWeatherFileRunPeriodsDefaulted());
  EXPECT_TRUE(simulationControl.runSimulationforWeatherFileRunPeriods());

  // Loads Convergence Tolerance Value:  Double
  // Check Idd default: .04
  EXPECT_TRUE(simulationControl.isLoadsConvergenceToleranceValueDefaulted());
  EXPECT_EQ(.04, simulationControl.loadsConvergenceToleranceValue());
  EXPECT_TRUE(simulationControl.setLoadsConvergenceToleranceValue(0.02));
  EXPECT_FALSE(simulationControl.isLoadsConvergenceToleranceValueDefaulted());
  EXPECT_EQ(0.02, simulationControl.loadsConvergenceToleranceValue());
  // Test reset
  simulationControl.resetLoadsConvergenceToleranceValue();
  EXPECT_EQ(.04, simulationControl.loadsConvergenceToleranceValue());
  EXPECT_TRUE(simulationControl.isLoadsConvergenceToleranceValueDefaulted());

  // Temperature Convergence Tolerance Value:  Double
  // Check Idd default: .4
  EXPECT_TRUE(simulationControl.isTemperatureConvergenceToleranceValueDefaulted());
  EXPECT_EQ(.4, simulationControl.temperatureConvergenceToleranceValue());
  EXPECT_TRUE(simulationControl.setTemperatureConvergenceToleranceValue(0.2));
  EXPECT_FALSE(simulationControl.isTemperatureConvergenceToleranceValueDefaulted());
  EXPECT_EQ(0.2, simulationControl.temperatureConvergenceToleranceValue());
  // Test reset
  simulationControl.resetTemperatureConvergenceToleranceValue();
  EXPECT_EQ(.4, simulationControl.temperatureConvergenceToleranceValue());
  EXPECT_TRUE(simulationControl.isTemperatureConvergenceToleranceValueDefaulted());

  // Solar Distribution:  String
  // Check Idd default: "FullExterior"
  EXPECT_TRUE(simulationControl.isSolarDistributionDefaulted());
  EXPECT_EQ("FullExterior", simulationControl.solarDistribution());
  // Test a valid choice
  EXPECT_TRUE(simulationControl.setSolarDistribution("MinimalShadowing"));
  EXPECT_FALSE(simulationControl.isSolarDistributionDefaulted());
  EXPECT_EQ("MinimalShadowing", simulationControl.solarDistribution());
  // Test an invalid choice
  EXPECT_FALSE(simulationControl.setSolarDistribution("BadChoice"));
  EXPECT_EQ("MinimalShadowing", simulationControl.solarDistribution());

  // Maximum Number of Warmup Days:  Integer
  // Check Idd default: 25
  EXPECT_TRUE(simulationControl.isMaximumNumberofWarmupDaysDefaulted());
  EXPECT_EQ(25, simulationControl.maximumNumberofWarmupDays());
  EXPECT_TRUE(simulationControl.setMaximumNumberofWarmupDays(12));
  EXPECT_FALSE(simulationControl.isMaximumNumberofWarmupDaysDefaulted());
  EXPECT_EQ(12, simulationControl.maximumNumberofWarmupDays());
  // Test reset
  simulationControl.resetMaximumNumberofWarmupDays();
  EXPECT_EQ(25, simulationControl.maximumNumberofWarmupDays());
  EXPECT_TRUE(simulationControl.isMaximumNumberofWarmupDaysDefaulted());

  // Minimum Number of Warmup Days:  Integer
  // Check Idd default: 1 (Used to be 6 before 9.3.0)
  EXPECT_TRUE(simulationControl.isMinimumNumberofWarmupDaysDefaulted());
  EXPECT_EQ(1, simulationControl.minimumNumberofWarmupDays());
  EXPECT_TRUE(simulationControl.setMinimumNumberofWarmupDays(3));
  EXPECT_FALSE(simulationControl.isMinimumNumberofWarmupDaysDefaulted());
  EXPECT_EQ(3, simulationControl.minimumNumberofWarmupDays());
  // Test reset
  simulationControl.resetMinimumNumberofWarmupDays();
  EXPECT_EQ(1, simulationControl.minimumNumberofWarmupDays());
  EXPECT_TRUE(simulationControl.isMinimumNumberofWarmupDaysDefaulted());

  // Do HVAC Sizing Simulation for Sizing Periods:  Boolean
  // Check Idd default: false
  EXPECT_TRUE(simulationControl.isDoHVACSizingSimulationforSizingPeriodsDefaulted());
  EXPECT_FALSE(simulationControl.doHVACSizingSimulationforSizingPeriods());
  // Test true
  EXPECT_TRUE(simulationControl.setDoHVACSizingSimulationforSizingPeriods(true));
  EXPECT_TRUE(simulationControl.doHVACSizingSimulationforSizingPeriods());
  EXPECT_FALSE(simulationControl.isDoHVACSizingSimulationforSizingPeriodsDefaulted());
  // Test false
  EXPECT_TRUE(simulationControl.setDoHVACSizingSimulationforSizingPeriods(false));
  EXPECT_FALSE(simulationControl.doHVACSizingSimulationforSizingPeriods());
  // Test reset
  simulationControl.resetDoHVACSizingSimulationforSizingPeriods();
  EXPECT_TRUE(simulationControl.isDoHVACSizingSimulationforSizingPeriodsDefaulted());
  EXPECT_FALSE(simulationControl.doHVACSizingSimulationforSizingPeriods());

  // Maximum Number of HVAC Sizing Simulation Passes:  Integer
  // Check Idd default: 1
  EXPECT_TRUE(simulationControl.isMaximumNumberofHVACSizingSimulationPassesDefaulted());
  EXPECT_EQ(1, simulationControl.maximumNumberofHVACSizingSimulationPasses());
  EXPECT_TRUE(simulationControl.setMaximumNumberofHVACSizingSimulationPasses(12));
  EXPECT_FALSE(simulationControl.isMaximumNumberofHVACSizingSimulationPassesDefaulted());
  EXPECT_EQ(12, simulationControl.maximumNumberofHVACSizingSimulationPasses());
  // Test reset
  simulationControl.resetMaximumNumberofHVACSizingSimulationPasses();
  EXPECT_EQ(1, simulationControl.maximumNumberofHVACSizingSimulationPasses());
  EXPECT_TRUE(simulationControl.isMaximumNumberofHVACSizingSimulationPassesDefaulted());
}

TEST_F(ModelFixture, SimulationControl_LifeCycleCost) {
  Model model;

  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  // adding this cost (and many others) would be really weird, expect a throw
  EXPECT_THROW(LifeCycleCost cost(simulationControl), openstudio::Exception);
}
