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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../AirflowNetworkSimulationControl.hpp"
#include "../AirflowNetworkSimulationControl_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"
#include "../ComponentCostAdjustments.hpp"
#include "../ComponentCostAdjustments_Impl.hpp"
#include "../ConvergenceLimits.hpp"
#include "../ConvergenceLimits_Impl.hpp"
#include "../CurrencyType.hpp"
#include "../CurrencyType_Impl.hpp"
#include "../ExternalInterface.hpp"
#include "../ExternalInterface_Impl.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"
#include "../HeatBalanceAlgorithm.hpp"
#include "../HeatBalanceAlgorithm_Impl.hpp"
#include "../LifeCycleCostParameters.hpp"
#include "../LifeCycleCostParameters_Impl.hpp"
#include "../LightingSimulationControl.hpp"
#include "../LightingSimulationControl_Impl.hpp"
#include "../OutputControlFiles.hpp"
#include "../OutputControlFiles_Impl.hpp"
#include "../OutputControlReportingTolerances.hpp"
#include "../OutputControlReportingTolerances_Impl.hpp"
#include "../OutputDebuggingData.hpp"
#include "../OutputDebuggingData_Impl.hpp"
#include "../OutputDiagnostics.hpp"
#include "../OutputDiagnostics_Impl.hpp"
#include "../OutputEnergyManagementSystem.hpp"
#include "../OutputEnergyManagementSystem_Impl.hpp"
#include "../OutputJSON.hpp"
#include "../OutputJSON_Impl.hpp"
#include "../PerformancePrecisionTradeoffs.hpp"
#include "../PerformancePrecisionTradeoffs_Impl.hpp"
#include "../RadianceParameters.hpp"
#include "../RadianceParameters_Impl.hpp"
#include "../RunPeriod.hpp"
#include "../RunPeriod_Impl.hpp"
#include "../RunPeriodControlDaylightSavingTime.hpp"
#include "../RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "../ShadowCalculation.hpp"
#include "../ShadowCalculation_Impl.hpp"
#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../SiteGroundReflectance.hpp"
#include "../SiteGroundReflectance_Impl.hpp"
#include "../SiteGroundTemperatureBuildingSurface.hpp"
#include "../SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include "../SiteGroundTemperatureDeep.hpp"
#include "../SiteGroundTemperatureDeep_Impl.hpp"
#include "../SiteGroundTemperatureFCfactorMethod.hpp"
#include "../SiteGroundTemperatureFCfactorMethod_Impl.hpp"
#include "../SiteGroundTemperatureShallow.hpp"
#include "../SiteGroundTemperatureShallow_Impl.hpp"
#include "../SiteWaterMainsTemperature.hpp"
#include "../SiteWaterMainsTemperature_Impl.hpp"
#include "../SizingParameters.hpp"
#include "../SizingParameters_Impl.hpp"
#include "../Timestep.hpp"
#include "../Timestep_Impl.hpp"
#include "../Version.hpp"
#include "../Version_Impl.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../ZoneAirContaminantBalance.hpp"
#include "../ZoneAirContaminantBalance_Impl.hpp"
#include "../ZoneAirHeatBalanceAlgorithm.hpp"
#include "../ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "../ZoneAirMassFlowConservation.hpp"
#include "../ZoneAirMassFlowConservation_Impl.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetworkSimulationControl_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  AirflowNetworkSimulationControl airflowNetworkSimulationControl = model.getUniqueModelObject<AirflowNetworkSimulationControl>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  // We use a comment to see if cloning to another model works
  airflowNetworkSimulationControl.setComment("Custom Object");

  // clone it into the same model
  AirflowNetworkSimulationControl airflowNetworkSimulationControlClone =
    airflowNetworkSimulationControl.clone(model).cast<AirflowNetworkSimulationControl>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(airflowNetworkSimulationControl, airflowNetworkSimulationControlClone);
  EXPECT_EQ("! Custom Object", airflowNetworkSimulationControlClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  AirflowNetworkSimulationControl airflowNetworkSimulationControlClone2 =
    airflowNetworkSimulationControl.clone(model2).cast<AirflowNetworkSimulationControl>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  EXPECT_EQ("! Custom Object", airflowNetworkSimulationControlClone2.comment());
}

TEST_F(ModelFixture, Building_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Building>());
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Building>());
  // We use a comment to see if cloning to another model works
  building.setComment("Custom Object");

  // clone it into the same model
  Building buildingClone = building.clone(model).cast<Building>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(building, buildingClone);
  EXPECT_EQ("! Custom Object", buildingClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<Building>());
  Building buildingClone2 = building.clone(model2).cast<Building>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<Building>());
  EXPECT_EQ("! Custom Object", buildingClone2.comment());
}

TEST_F(ModelFixture, ClimateZones_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ClimateZones>());
  ClimateZones climateZones = model.getUniqueModelObject<ClimateZones>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ClimateZones>());
  // We use a comment to see if cloning to another model works
  climateZones.setComment("Custom Object");

  // clone it into the same model
  ClimateZones climateZonesClone = climateZones.clone(model).cast<ClimateZones>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(climateZones, climateZonesClone);
  EXPECT_EQ("! Custom Object", climateZonesClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ClimateZones>());
  ClimateZones climateZonesClone2 = climateZones.clone(model2).cast<ClimateZones>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ClimateZones>());
  EXPECT_EQ("! Custom Object", climateZonesClone2.comment());
}

TEST_F(ModelFixture, ConvergenceLimits_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ConvergenceLimits>());
  ConvergenceLimits convergenceLimits = model.getUniqueModelObject<ConvergenceLimits>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ConvergenceLimits>());
  // We use a comment to see if cloning to another model works
  convergenceLimits.setComment("Custom Object");

  // clone it into the same model
  ConvergenceLimits convergenceLimitsClone = convergenceLimits.clone(model).cast<ConvergenceLimits>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(convergenceLimits, convergenceLimitsClone);
  EXPECT_EQ("! Custom Object", convergenceLimitsClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ConvergenceLimits>());
  ConvergenceLimits convergenceLimitsClone2 = convergenceLimits.clone(model2).cast<ConvergenceLimits>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ConvergenceLimits>());
  EXPECT_EQ("! Custom Object", convergenceLimitsClone2.comment());
}

TEST_F(ModelFixture, ExternalInterface_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ExternalInterface>());
  ExternalInterface externalInterface = model.getUniqueModelObject<ExternalInterface>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ExternalInterface>());
  // We use a comment to see if cloning to another model works
  externalInterface.setComment("Custom Object");

  // clone it into the same model
  ExternalInterface externalInterfaceClone = externalInterface.clone(model).cast<ExternalInterface>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(externalInterface, externalInterfaceClone);
  EXPECT_EQ("! Custom Object", externalInterfaceClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ExternalInterface>());
  ExternalInterface externalInterfaceClone2 = externalInterface.clone(model2).cast<ExternalInterface>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ExternalInterface>());
  EXPECT_EQ("! Custom Object", externalInterfaceClone2.comment());
}

TEST_F(ModelFixture, Facility_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Facility>());
  Facility facility = model.getUniqueModelObject<Facility>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Facility>());
  // We use a comment to see if cloning to another model works
  facility.setComment("Custom Object");

  // clone it into the same model
  Facility facilityClone = facility.clone(model).cast<Facility>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(facility, facilityClone);
  EXPECT_EQ("! Custom Object", facilityClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<Facility>());
  Facility facilityClone2 = facility.clone(model2).cast<Facility>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<Facility>());
  EXPECT_EQ("! Custom Object", facilityClone2.comment());
}

TEST_F(ModelFixture, HeatBalanceAlgorithm_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  HeatBalanceAlgorithm heatBalanceAlgorithm = model.getUniqueModelObject<HeatBalanceAlgorithm>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  // We use a comment to see if cloning to another model works
  heatBalanceAlgorithm.setComment("Custom Object");

  // clone it into the same model
  HeatBalanceAlgorithm heatBalanceAlgorithmClone = heatBalanceAlgorithm.clone(model).cast<HeatBalanceAlgorithm>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(heatBalanceAlgorithm, heatBalanceAlgorithmClone);
  EXPECT_EQ("! Custom Object", heatBalanceAlgorithmClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  HeatBalanceAlgorithm heatBalanceAlgorithmClone2 = heatBalanceAlgorithm.clone(model2).cast<HeatBalanceAlgorithm>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  EXPECT_EQ("! Custom Object", heatBalanceAlgorithmClone2.comment());
}

TEST_F(ModelFixture, LifeCycleCostParameters_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  LifeCycleCostParameters lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  // We use a comment to see if cloning to another model works
  lifeCycleCostParameters.setComment("Custom Object");

  // clone it into the same model
  LifeCycleCostParameters lifeCycleCostParametersClone = lifeCycleCostParameters.clone(model).cast<LifeCycleCostParameters>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(lifeCycleCostParameters, lifeCycleCostParametersClone);
  EXPECT_EQ("! Custom Object", lifeCycleCostParametersClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  LifeCycleCostParameters lifeCycleCostParametersClone2 = lifeCycleCostParameters.clone(model2).cast<LifeCycleCostParameters>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  EXPECT_EQ("! Custom Object", lifeCycleCostParametersClone2.comment());
}

TEST_F(ModelFixture, LightingSimulationControl_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<LightingSimulationControl>());
  LightingSimulationControl lightingSimulationControl = model.getUniqueModelObject<LightingSimulationControl>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<LightingSimulationControl>());
  // We use a comment to see if cloning to another model works
  lightingSimulationControl.setComment("Custom Object");

  // clone it into the same model
  LightingSimulationControl lightingSimulationControlClone = lightingSimulationControl.clone(model).cast<LightingSimulationControl>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(lightingSimulationControl, lightingSimulationControlClone);
  EXPECT_EQ("! Custom Object", lightingSimulationControlClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<LightingSimulationControl>());
  LightingSimulationControl lightingSimulationControlClone2 = lightingSimulationControl.clone(model2).cast<LightingSimulationControl>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<LightingSimulationControl>());
  EXPECT_EQ("! Custom Object", lightingSimulationControlClone2.comment());
}

TEST_F(ModelFixture, OutputControlFiles_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputControlFiles>());
  OutputControlFiles outputControlFiles = model.getUniqueModelObject<OutputControlFiles>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputControlFiles>());
  // We use a comment to see if cloning to another model works
  outputControlFiles.setComment("Custom Object");

  // clone it into same model
  OutputControlFiles outputControlFilesClone = outputControlFiles.clone(model).cast<OutputControlFiles>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputControlFiles, outputControlFilesClone);
  EXPECT_EQ("! Custom Object", outputControlFilesClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputControlFiles>());
  OutputControlFiles outputControlFilesClone2 = outputControlFiles.clone(model2).cast<OutputControlFiles>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputControlFiles>());
  EXPECT_EQ("! Custom Object", outputControlFilesClone2.comment());
}

TEST_F(ModelFixture, OutputControlReportingTolerances_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  OutputControlReportingTolerances outputControlReportingTolerances = model.getUniqueModelObject<OutputControlReportingTolerances>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  // We use a comment to see if cloning to another model works
  outputControlReportingTolerances.setComment("Custom Object");

  // clone it into the same model
  OutputControlReportingTolerances outputControlReportingTolerancesClone =
    outputControlReportingTolerances.clone(model).cast<OutputControlReportingTolerances>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputControlReportingTolerances, outputControlReportingTolerancesClone);
  EXPECT_EQ("! Custom Object", outputControlReportingTolerancesClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  OutputControlReportingTolerances outputControlReportingTolerancesClone2 =
    outputControlReportingTolerances.clone(model2).cast<OutputControlReportingTolerances>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  EXPECT_EQ("! Custom Object", outputControlReportingTolerancesClone2.comment());
}

TEST_F(ModelFixture, OutputDebuggingData_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputDebuggingData>());
  OutputDebuggingData outputDebuggingData = model.getUniqueModelObject<OutputDebuggingData>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputDebuggingData>());
  // We use a comment to see if cloning to another model works
  outputDebuggingData.setComment("Custom Object");

  // clone it into the same model
  OutputDebuggingData outputDebuggingDataClone = outputDebuggingData.clone(model).cast<OutputDebuggingData>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputDebuggingData, outputDebuggingDataClone);
  EXPECT_EQ("! Custom Object", outputDebuggingDataClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputDebuggingData>());
  OutputDebuggingData outputDebuggingDataClone2 = outputDebuggingData.clone(model2).cast<OutputDebuggingData>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputDebuggingData>());
  EXPECT_EQ("! Custom Object", outputDebuggingDataClone2.comment());
}

TEST_F(ModelFixture, OutputDiagnostics_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputDiagnostics>());
  OutputDiagnostics outputDiagnostics = model.getUniqueModelObject<OutputDiagnostics>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputDiagnostics>());
  // We use a comment to see if cloning to another model works
  outputDiagnostics.setComment("Custom Object");

  // clone it into the same model
  OutputDiagnostics outputDiagnosticsClone = outputDiagnostics.clone(model).cast<OutputDiagnostics>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputDiagnostics, outputDiagnosticsClone);
  EXPECT_EQ("! Custom Object", outputDiagnosticsClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputDiagnostics>());
  OutputDiagnostics outputDiagnosticsClone2 = outputDiagnostics.clone(model2).cast<OutputDiagnostics>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputDiagnostics>());
  EXPECT_EQ("! Custom Object", outputDiagnosticsClone2.comment());
}

TEST_F(ModelFixture, OutputEnergyManagementSystem_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  OutputEnergyManagementSystem outputEnergyManagementSystem = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  // We use a comment to see if cloning to another model works
  outputEnergyManagementSystem.setComment("Custom Object");

  // clone it into the same model
  OutputEnergyManagementSystem outputEnergyManagementSystemClone = outputEnergyManagementSystem.clone(model).cast<OutputEnergyManagementSystem>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputEnergyManagementSystem, outputEnergyManagementSystemClone);
  EXPECT_EQ("! Custom Object", outputEnergyManagementSystemClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  OutputEnergyManagementSystem outputEnergyManagementSystemClone2 = outputEnergyManagementSystem.clone(model2).cast<OutputEnergyManagementSystem>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  EXPECT_EQ("! Custom Object", outputEnergyManagementSystemClone2.comment());
}

TEST_F(ModelFixture, OutputJSON_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<OutputJSON>());
  OutputJSON outputJSON = model.getUniqueModelObject<OutputJSON>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<OutputJSON>());
  // We use a comment to see if cloning to another model works
  outputJSON.setComment("Custom Object");

  // clone it into the same model
  OutputJSON outputJSONClone = outputJSON.clone(model).cast<OutputJSON>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputJSON, outputJSONClone);
  EXPECT_EQ("! Custom Object", outputJSONClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<OutputJSON>());
  OutputJSON outputJSONClone2 = outputJSON.clone(model2).cast<OutputJSON>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<OutputJSON>());
  EXPECT_EQ("! Custom Object", outputJSONClone2.comment());
}

TEST_F(ModelFixture, PerformancePrecisionTradeoffs_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  // We use a comment to see if cloning to another model works
  performancePrecisionTradeoffs.setComment("Custom Object");

  // clone it into the same model
  PerformancePrecisionTradeoffs performancePrecisionTradeoffsClone = performancePrecisionTradeoffs.clone(model).cast<PerformancePrecisionTradeoffs>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(performancePrecisionTradeoffs, performancePrecisionTradeoffsClone);
  EXPECT_EQ("! Custom Object", performancePrecisionTradeoffsClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  PerformancePrecisionTradeoffs performancePrecisionTradeoffsClone2 =
    performancePrecisionTradeoffs.clone(model2).cast<PerformancePrecisionTradeoffs>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  EXPECT_EQ("! Custom Object", performancePrecisionTradeoffsClone2.comment());
}

TEST_F(ModelFixture, RadianceParameters_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<RadianceParameters>());
  RadianceParameters radianceParameters = model.getUniqueModelObject<RadianceParameters>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<RadianceParameters>());
  // We use a comment to see if cloning to another model works
  radianceParameters.setComment("Custom Object");

  // clone it into the same model
  RadianceParameters radianceParametersClone = radianceParameters.clone(model).cast<RadianceParameters>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(radianceParameters, radianceParametersClone);
  EXPECT_EQ("! Custom Object", radianceParametersClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<RadianceParameters>());
  RadianceParameters radianceParametersClone2 = radianceParameters.clone(model2).cast<RadianceParameters>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<RadianceParameters>());
  EXPECT_EQ("! Custom Object", radianceParametersClone2.comment());
}

TEST_F(ModelFixture, RunPeriod_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<RunPeriod>());
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<RunPeriod>());
  // We use a comment to see if cloning to another model works
  runPeriod.setComment("Custom Object");

  // clone it into the same model
  RunPeriod runPeriodClone = runPeriod.clone(model).cast<RunPeriod>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(runPeriod, runPeriodClone);
  EXPECT_EQ("! Custom Object", runPeriodClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<RunPeriod>());
  RunPeriod runPeriodClone2 = runPeriod.clone(model2).cast<RunPeriod>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<RunPeriod>());
  EXPECT_EQ("! Custom Object", runPeriodClone2.comment());
}

TEST_F(ModelFixture, RunPeriodControlDaylightSavingTime_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  RunPeriodControlDaylightSavingTime runPeriodControlDaylightSavingTime = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  // We use a comment to see if cloning to another model works
  runPeriodControlDaylightSavingTime.setComment("Custom Object");

  // clone it into the same model
  RunPeriodControlDaylightSavingTime runPeriodControlDaylightSavingTimeClone =
    runPeriodControlDaylightSavingTime.clone(model).cast<RunPeriodControlDaylightSavingTime>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(runPeriodControlDaylightSavingTime, runPeriodControlDaylightSavingTimeClone);
  EXPECT_EQ("! Custom Object", runPeriodControlDaylightSavingTimeClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  RunPeriodControlDaylightSavingTime runPeriodControlDaylightSavingTimeClone2 =
    runPeriodControlDaylightSavingTime.clone(model2).cast<RunPeriodControlDaylightSavingTime>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  EXPECT_EQ("! Custom Object", runPeriodControlDaylightSavingTimeClone2.comment());
}

TEST_F(ModelFixture, ShadowCalculation_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ShadowCalculation>());
  ShadowCalculation shadowCalculation = model.getUniqueModelObject<ShadowCalculation>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ShadowCalculation>());
  // We use a comment to see if cloning to another model works
  shadowCalculation.setComment("Custom Object");

  // clone it into the same model
  ShadowCalculation shadowCalculationClone = shadowCalculation.clone(model).cast<ShadowCalculation>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(shadowCalculation, shadowCalculationClone);
  EXPECT_EQ("! Custom Object", shadowCalculationClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ShadowCalculation>());
  ShadowCalculation shadowCalculationClone2 = shadowCalculation.clone(model2).cast<ShadowCalculation>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ShadowCalculation>());
  EXPECT_EQ("! Custom Object", shadowCalculationClone2.comment());
}

TEST_F(ModelFixture, SimulationControl_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SimulationControl>());
  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SimulationControl>());
  // We use a comment to see if cloning to another model works
  simulationControl.setComment("Custom Object");

  // clone it into the same model
  SimulationControl simulationControlClone = simulationControl.clone(model).cast<SimulationControl>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(simulationControl, simulationControlClone);
  EXPECT_EQ("! Custom Object", simulationControlClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SimulationControl>());
  SimulationControl simulationControlClone2 = simulationControl.clone(model2).cast<SimulationControl>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SimulationControl>());
  EXPECT_EQ("! Custom Object", simulationControlClone2.comment());
}

TEST_F(ModelFixture, Site_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Site>());
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Site>());
  // We use a comment to see if cloning to another model works
  site.setComment("Custom Object");

  // clone it into the same model
  Site siteClone = site.clone(model).cast<Site>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(site, siteClone);
  EXPECT_EQ("! Custom Object", siteClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<Site>());
  Site siteClone2 = site.clone(model2).cast<Site>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<Site>());
  EXPECT_EQ("! Custom Object", siteClone2.comment());
}

TEST_F(ModelFixture, SiteGroundReflectance_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteGroundReflectance>());
  SiteGroundReflectance siteGroundReflectance = model.getUniqueModelObject<SiteGroundReflectance>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteGroundReflectance>());
  // We use a comment to see if cloning to another model works
  siteGroundReflectance.setComment("Custom Object");

  // clone it into the same model
  SiteGroundReflectance siteGroundReflectanceClone = siteGroundReflectance.clone(model).cast<SiteGroundReflectance>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteGroundReflectance, siteGroundReflectanceClone);
  EXPECT_EQ("! Custom Object", siteGroundReflectanceClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteGroundReflectance>());
  SiteGroundReflectance siteGroundReflectanceClone2 = siteGroundReflectance.clone(model2).cast<SiteGroundReflectance>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteGroundReflectance>());
  EXPECT_EQ("! Custom Object", siteGroundReflectanceClone2.comment());
}

TEST_F(ModelFixture, SiteGroundTemperatureBuildingSurface_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  SiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurface = model.getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  // We use a comment to see if cloning to another model works
  siteGroundTemperatureBuildingSurface.setComment("Custom Object");

  // clone it into the same model
  SiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurfaceClone =
    siteGroundTemperatureBuildingSurface.clone(model).cast<SiteGroundTemperatureBuildingSurface>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteGroundTemperatureBuildingSurface, siteGroundTemperatureBuildingSurfaceClone);
  EXPECT_EQ("! Custom Object", siteGroundTemperatureBuildingSurfaceClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  SiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurfaceClone2 =
    siteGroundTemperatureBuildingSurface.clone(model2).cast<SiteGroundTemperatureBuildingSurface>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  EXPECT_EQ("! Custom Object", siteGroundTemperatureBuildingSurfaceClone2.comment());
}

TEST_F(ModelFixture, SiteGroundTemperatureDeep_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  SiteGroundTemperatureDeep siteGroundTemperatureDeep = model.getUniqueModelObject<SiteGroundTemperatureDeep>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  // We use a comment to see if cloning to another model works
  siteGroundTemperatureDeep.setComment("Custom Object");

  // clone it into the same model
  SiteGroundTemperatureDeep siteGroundTemperatureDeepClone = siteGroundTemperatureDeep.clone(model).cast<SiteGroundTemperatureDeep>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteGroundTemperatureDeep, siteGroundTemperatureDeepClone);
  EXPECT_EQ("! Custom Object", siteGroundTemperatureDeepClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  SiteGroundTemperatureDeep siteGroundTemperatureDeepClone2 = siteGroundTemperatureDeep.clone(model2).cast<SiteGroundTemperatureDeep>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  EXPECT_EQ("! Custom Object", siteGroundTemperatureDeepClone2.comment());
}

TEST_F(ModelFixture, SiteGroundTemperatureFCfactorMethod_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  SiteGroundTemperatureFCfactorMethod siteGroundTemperatureFCfactorMethod = model.getUniqueModelObject<SiteGroundTemperatureFCfactorMethod>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  // We use a comment to see if cloning to another model works
  siteGroundTemperatureFCfactorMethod.setComment("Custom Object");

  // clone it into the same model
  SiteGroundTemperatureFCfactorMethod siteGroundTemperatureFCfactorMethodClone =
    siteGroundTemperatureFCfactorMethod.clone(model).cast<SiteGroundTemperatureFCfactorMethod>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteGroundTemperatureFCfactorMethod, siteGroundTemperatureFCfactorMethodClone);
  EXPECT_EQ("! Custom Object", siteGroundTemperatureFCfactorMethodClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  SiteGroundTemperatureFCfactorMethod siteGroundTemperatureFCfactorMethodClone2 =
    siteGroundTemperatureFCfactorMethod.clone(model2).cast<SiteGroundTemperatureFCfactorMethod>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  EXPECT_EQ("! Custom Object", siteGroundTemperatureFCfactorMethodClone2.comment());
}

TEST_F(ModelFixture, SiteGroundTemperatureShallow_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  SiteGroundTemperatureShallow siteGroundTemperatureShallow = model.getUniqueModelObject<SiteGroundTemperatureShallow>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  // We use a comment to see if cloning to another model works
  siteGroundTemperatureShallow.setComment("Custom Object");

  // clone it into the same model
  SiteGroundTemperatureShallow siteGroundTemperatureShallowClone = siteGroundTemperatureShallow.clone(model).cast<SiteGroundTemperatureShallow>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteGroundTemperatureShallow, siteGroundTemperatureShallowClone);
  EXPECT_EQ("! Custom Object", siteGroundTemperatureShallowClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  SiteGroundTemperatureShallow siteGroundTemperatureShallowClone2 = siteGroundTemperatureShallow.clone(model2).cast<SiteGroundTemperatureShallow>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  EXPECT_EQ("! Custom Object", siteGroundTemperatureShallowClone2.comment());
}

TEST_F(ModelFixture, SiteWaterMainsTemperature_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  SiteWaterMainsTemperature siteWaterMainsTemperature = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  // We use a comment to see if cloning to another model works
  siteWaterMainsTemperature.setComment("Custom Object");

  // clone it into the same model
  SiteWaterMainsTemperature siteWaterMainsTemperatureClone = siteWaterMainsTemperature.clone(model).cast<SiteWaterMainsTemperature>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteWaterMainsTemperature, siteWaterMainsTemperatureClone);
  EXPECT_EQ("! Custom Object", siteWaterMainsTemperatureClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  SiteWaterMainsTemperature siteWaterMainsTemperatureClone2 = siteWaterMainsTemperature.clone(model2).cast<SiteWaterMainsTemperature>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ("! Custom Object", siteWaterMainsTemperatureClone2.comment());
}

TEST_F(ModelFixture, SizingParameters_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<SizingParameters>());
  SizingParameters sizingParameters = model.getUniqueModelObject<SizingParameters>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<SizingParameters>());
  // We use a comment to see if cloning to another model works
  sizingParameters.setComment("Custom Object");

  // clone it into the same model
  SizingParameters sizingParametersClone = sizingParameters.clone(model).cast<SizingParameters>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(sizingParameters, sizingParametersClone);
  EXPECT_EQ("! Custom Object", sizingParametersClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<SizingParameters>());
  SizingParameters sizingParametersClone2 = sizingParameters.clone(model2).cast<SizingParameters>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<SizingParameters>());
  EXPECT_EQ("! Custom Object", sizingParametersClone2.comment());
}

TEST_F(ModelFixture, Timestep_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Timestep>());
  Timestep timestep = model.getUniqueModelObject<Timestep>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Timestep>());
  // We use a comment to see if cloning to another model works
  timestep.setComment("Custom Object");

  // clone it into the same model
  Timestep timestepClone = timestep.clone(model).cast<Timestep>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(timestep, timestepClone);
  EXPECT_EQ("! Custom Object", timestepClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<Timestep>());
  Timestep timestepClone2 = timestep.clone(model2).cast<Timestep>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<Timestep>());
  EXPECT_EQ("! Custom Object", timestepClone2.comment());
}

TEST_F(ModelFixture, WeatherFile_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<WeatherFile>());
  WeatherFile weatherFile = model.getUniqueModelObject<WeatherFile>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<WeatherFile>());
  // We use a comment to see if cloning to another model works
  weatherFile.setComment("Custom Object");

  // clone it into the same model
  WeatherFile weatherFileClone = weatherFile.clone(model).cast<WeatherFile>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(weatherFile, weatherFileClone);
  EXPECT_EQ("! Custom Object", weatherFileClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<WeatherFile>());
  WeatherFile weatherFileClone2 = weatherFile.clone(model2).cast<WeatherFile>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<WeatherFile>());
  EXPECT_EQ("! Custom Object", weatherFileClone2.comment());
}

// TODO: Doesn't work, apparently 'YearDescription' is ambiguous.
// candidates are:
// * Correct one: model/YearDescription.hpp   class openstudio::model::YearDescription
// * Incorrect:   utilities/time/Date.hpp     struct openstudio::YearDescription
//
// TEST_F(ModelFixture, YearDescription_UniqueModelObject_Clone)
// {
//   // create a model to use
//   Model model;
//
//   // Get the Unique ModelObject
//   EXPECT_FALSE(model.getOptionalUniqueModelObject<YearDescription>());
//   YearDescription yearDescription = model.getUniqueModelObject<YearDescription>();
//   EXPECT_TRUE(model.getOptionalUniqueModelObject<YearDescription>());
//   // We use a comment to see if cloning to another model works
//   yearDescription.setComment("Custom Object");
//
//   // clone it into the same model
//   YearDescription yearDescriptionClone = yearDescription.clone(model).cast<YearDescription>();
//   // UniqueModelObject: should be the same as the original
//   EXPECT_EQ(yearDescription, yearDescriptionClone);
//   EXPECT_EQ("! Custom Object", yearDescriptionClone.comment());
//
//   // clone it into a different model
//   Model model2;
//   EXPECT_FALSE(model2.getOptionalUniqueModelObject<YearDescription>());
//   YearDescription yearDescriptionClone2 = yearDescription.clone(model2).cast<YearDescription>();
//   EXPECT_TRUE(model2.getOptionalUniqueModelObject<YearDescription>());
//   EXPECT_EQ("! Custom Object", yearDescriptionClone2.comment());
// }

TEST_F(ModelFixture, ZoneAirContaminantBalance_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirContaminantBalance>());
  ZoneAirContaminantBalance zoneAirContaminantBalance = model.getUniqueModelObject<ZoneAirContaminantBalance>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneAirContaminantBalance>());
  // We use a comment to see if cloning to another model works
  zoneAirContaminantBalance.setComment("Custom Object");

  // clone it into the same model
  ZoneAirContaminantBalance zoneAirContaminantBalanceClone = zoneAirContaminantBalance.clone(model).cast<ZoneAirContaminantBalance>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(zoneAirContaminantBalance, zoneAirContaminantBalanceClone);
  EXPECT_EQ("! Custom Object", zoneAirContaminantBalanceClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ZoneAirContaminantBalance>());
  ZoneAirContaminantBalance zoneAirContaminantBalanceClone2 = zoneAirContaminantBalance.clone(model2).cast<ZoneAirContaminantBalance>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ZoneAirContaminantBalance>());
  EXPECT_EQ("! Custom Object", zoneAirContaminantBalanceClone2.comment());
}

TEST_F(ModelFixture, ZoneAirHeatBalanceAlgorithm_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  ZoneAirHeatBalanceAlgorithm zoneAirHeatBalanceAlgorithm = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  // We use a comment to see if cloning to another model works
  zoneAirHeatBalanceAlgorithm.setComment("Custom Object");

  // clone it into the same model
  ZoneAirHeatBalanceAlgorithm zoneAirHeatBalanceAlgorithmClone = zoneAirHeatBalanceAlgorithm.clone(model).cast<ZoneAirHeatBalanceAlgorithm>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(zoneAirHeatBalanceAlgorithm, zoneAirHeatBalanceAlgorithmClone);
  EXPECT_EQ("! Custom Object", zoneAirHeatBalanceAlgorithmClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  ZoneAirHeatBalanceAlgorithm zoneAirHeatBalanceAlgorithmClone2 = zoneAirHeatBalanceAlgorithm.clone(model2).cast<ZoneAirHeatBalanceAlgorithm>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  EXPECT_EQ("! Custom Object", zoneAirHeatBalanceAlgorithmClone2.comment());
}

TEST_F(ModelFixture, ZoneAirMassFlowConservation_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  ZoneAirMassFlowConservation zoneAirMassFlowConservation = model.getUniqueModelObject<ZoneAirMassFlowConservation>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  // We use a comment to see if cloning to another model works
  zoneAirMassFlowConservation.setComment("Custom Object");

  // clone it into the same model
  ZoneAirMassFlowConservation zoneAirMassFlowConservationClone = zoneAirMassFlowConservation.clone(model).cast<ZoneAirMassFlowConservation>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(zoneAirMassFlowConservation, zoneAirMassFlowConservationClone);
  EXPECT_EQ("! Custom Object", zoneAirMassFlowConservationClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  ZoneAirMassFlowConservation zoneAirMassFlowConservationClone2 = zoneAirMassFlowConservation.clone(model2).cast<ZoneAirMassFlowConservation>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  EXPECT_EQ("! Custom Object", zoneAirMassFlowConservationClone2.comment());
}

TEST_F(ModelFixture, ZoneCapacitanceMultiplierResearchSpecial_UniqueModelObject_Clone) {
  // create a model to use
  Model model;

  // Get the Unique ModelObject
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecial =
    model.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  // We use a comment to see if cloning to another model works
  zoneCapacitanceMultiplierResearchSpecial.setComment("Custom Object");

  // clone it into the same model
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecialClone =
    zoneCapacitanceMultiplierResearchSpecial.clone(model).cast<ZoneCapacitanceMultiplierResearchSpecial>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(zoneCapacitanceMultiplierResearchSpecial, zoneCapacitanceMultiplierResearchSpecialClone);
  EXPECT_EQ("! Custom Object", zoneCapacitanceMultiplierResearchSpecialClone.comment());

  // clone it into a different model
  Model model2;
  EXPECT_FALSE(model2.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecialClone2 =
    zoneCapacitanceMultiplierResearchSpecial.clone(model2).cast<ZoneCapacitanceMultiplierResearchSpecial>();
  EXPECT_TRUE(model2.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  EXPECT_EQ("! Custom Object", zoneCapacitanceMultiplierResearchSpecialClone2.comment());
}

// TODO: Not working for ComponentCostAdjustments
// TEST_F(ModelFixture, ComponentCostAdjustments_UniqueModelObject_Clone)
// {
//   // create a model to use
//   Model model;
//
//   // Get the Unique ModelObject
//   EXPECT_FALSE(model.getOptionalUniqueModelObject<ComponentCostAdjustments>());
//   ComponentCostAdjustments componentCostAdjustments = model.getUniqueModelObject<ComponentCostAdjustments>();
//   EXPECT_TRUE(model.getOptionalUniqueModelObject<ComponentCostAdjustments>());
//   // We use a comment to see if cloning to another model works
//   componentCostAdjustments.setComment("Custom Object");
//
//   // clone it into the same model
//   ComponentCostAdjustments componentCostAdjustmentsClone = componentCostAdjustments.clone(model).cast<ComponentCostAdjustments>();
//   // UniqueModelObject: should be the same as the original
//   EXPECT_EQ(componentCostAdjustments, componentCostAdjustmentsClone);
//   EXPECT_EQ("! Custom Object", componentCostAdjustmentsClone.comment());
//
//   // clone it into a different model
//   Model model2;
//   EXPECT_FALSE(model2.getOptionalUniqueModelObject<ComponentCostAdjustments>());
//   ComponentCostAdjustments componentCostAdjustmentsClone2 = componentCostAdjustments.clone(model2).cast<ComponentCostAdjustments>();
//   EXPECT_TRUE(model2.getOptionalUniqueModelObject<ComponentCostAdjustments>());
//   EXPECT_EQ("! Custom Object", componentCostAdjustmentsClone2.comment());
// }

// TODO: Not working for CurrencyType
// TEST_F(ModelFixture, CurrencyType_UniqueModelObject_Clone)
// {
//   // create a model to use
//   Model model;
//
//   // Get the Unique ModelObject
//   EXPECT_FALSE(model.getOptionalUniqueModelObject<CurrencyType>());
//   CurrencyType currencyType = model.getUniqueModelObject<CurrencyType>();
//   EXPECT_TRUE(model.getOptionalUniqueModelObject<CurrencyType>());
//   // We use a comment to see if cloning to another model works
//   currencyType.setComment("Custom Object");
//
//   // clone it into the same model
//   CurrencyType currencyTypeClone = currencyType.clone(model).cast<CurrencyType>();
//   // UniqueModelObject: should be the same as the original
//   EXPECT_EQ(currencyType, currencyTypeClone);
//   EXPECT_EQ("! Custom Object", currencyTypeClone.comment());
//
//   // clone it into a different model
//   Model model2;
//   EXPECT_FALSE(model2.getOptionalUniqueModelObject<CurrencyType>());
//   CurrencyType currencyTypeClone2 = currencyType.clone(model2).cast<CurrencyType>();
//   EXPECT_TRUE(model2.getOptionalUniqueModelObject<CurrencyType>());
//   EXPECT_EQ("! Custom Object", currencyTypeClone2.comment());
// }

// TODO: Not working for Version
// TEST_F(ModelFixture, Version_UniqueModelObject_Clone)
// {
//   // create a model to use
//   Model model;
//
//   // Get the Unique ModelObject
//   EXPECT_FALSE(model.getOptionalUniqueModelObject<Version>());
//   Version version = model.getUniqueModelObject<Version>();
//   EXPECT_TRUE(model.getOptionalUniqueModelObject<Version>());
//   // We use a comment to see if cloning to another model works
//   version.setComment("Custom Object");
//
//   // clone it into the same model
//   Version versionClone = version.clone(model).cast<Version>();
//   // UniqueModelObject: should be the same as the original
//   EXPECT_EQ(version, versionClone);
//   EXPECT_EQ("! Custom Object", versionClone.comment());
//
//   // clone it into a different model
//   Model model2;
//   EXPECT_FALSE(model2.getOptionalUniqueModelObject<Version>());
//   Version versionClone2 = version.clone(model2).cast<Version>();
//   EXPECT_TRUE(model2.getOptionalUniqueModelObject<Version>());
//   EXPECT_EQ("! Custom Object", versionClone2.comment());
// }
