/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../DesignDay.hpp"
#include "../LifeCycleCost.hpp"
#include "../PlantLoop.hpp"
#include "../SizingPlant.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SimulationControl_GettersSetters) {
  Model model;

  auto simulationControl = model.getUniqueModelObject<SimulationControl>();

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

  auto simulationControl = model.getUniqueModelObject<SimulationControl>();

  // adding this cost (and many others) would be really weird, expect a throw
  EXPECT_THROW(LifeCycleCost cost(simulationControl), openstudio::Exception);
}

TEST_F(ModelFixture, SimulationControl_SmartDefaults) {

  Model m;

  auto sc = m.getUniqueModelObject<SimulationControl>();

  auto testDefaults = [&sc](bool doZone, bool doSystem, bool doPlant, bool doHVACForSizing) {
    EXPECT_EQ(doZone, sc.doZoneSizingCalculation());
    EXPECT_EQ(doSystem, sc.doSystemSizingCalculation());
    EXPECT_EQ(doPlant, sc.doPlantSizingCalculation());
    EXPECT_EQ(doHVACForSizing, sc.doHVACSizingSimulationforSizingPeriods());
  };

  sc.resetDoHVACSizingSimulationforSizingPeriods();
  sc.resetDoZoneSizingCalculation();
  sc.resetDoSystemSizingCalculation();
  sc.resetDoPlantSizingCalculation();

  // No Design Day, no Zonal / AirLoopHVAC / PlantLoop
  testDefaults(false, false, false, false);

  DesignDay dd(m);
  testDefaults(false, false, false, false);

  EXPECT_FALSE(sc.doZoneSizingCalculation());
  EXPECT_FALSE(sc.doSystemSizingCalculation());
  EXPECT_FALSE(sc.doPlantSizingCalculation());
  EXPECT_FALSE(sc.doHVACSizingSimulationforSizingPeriods());

  ThermalZone z(m);
  testDefaults(false, false, false, false);

  EXPECT_FALSE(sc.doZoneSizingCalculation());
  EXPECT_FALSE(sc.doSystemSizingCalculation());
  EXPECT_FALSE(sc.doPlantSizingCalculation());
  EXPECT_FALSE(sc.doHVACSizingSimulationforSizingPeriods());

  ZoneHVACBaseboardConvectiveElectric b1(m);
  testDefaults(false, false, false, false);

  // We have design day + a zone with some equipment => precondition for doZoneSizingCalculation is met
  EXPECT_TRUE(b1.addToThermalZone(z));
  testDefaults(true, false, false, false);

  // Design Day + AirLoopHVAC => doSystemSizingCalculation
  AirLoopHVAC a(m);
  testDefaults(true, true, false, false);

  // Design Day + PlantLoop => doPlantSizingCalculation
  PlantLoop p(m);
  EXPECT_TRUE(p.sizingPlant().setSizingOption("NonCoincident"));
  testDefaults(true, true, true, false);

  // Design Day + Coicident => doHVACSizingSimulationforSizingPeriods
  EXPECT_TRUE(p.sizingPlant().setSizingOption("Coincident"));
  testDefaults(true, true, true, true);

  // Remove design day
  dd.remove();
  testDefaults(false, false, false, false);
}
