#ifndef MODEL_SIMULATION_I
#define MODEL_SIMULATION_I

#ifdef SWIGPYTHON
  %module openstudiomodelsimulation
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>

%{
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/time/DateTime.hpp>
%}

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelSimulation

  // Conflicts with class name in utilities
  // Note JM 2019-04-16: Will completely ignore it below if C# actually. Note that there are convenience functions in Model.hpp such as Model::setCalendarYear to act upon YearDescription
  // indirectly.
  //%rename(ModelYearDescription) openstudio::model::YearDescription;

  // Note JM 2019-04-16: Ignoring Site-related methods and reimplementing them in ModelGeometry.i using partial classes
  %ignore openstudio::model::WeatherFile::site;
  %ignore openstudio::model::ClimateZones::site;

  // Note JM 2020-03-11: Ignoring this, will reimplement later in ModelHVAC.i using partial classes
  %ignore openstudio::model::ShadowCalculation::addShadingZoneGroup;
  %ignore openstudio::model::ShadowCalculation::getShadingZoneGroup;

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
namespace model {

  // forward declarations
  class Surface;
  class SubSurface;
  class InternalMass;
  class Schedule;
  class ThermalZone; // For ShadowCalculation::addShadingZoneGroup & getShadingZoneGroup
}
}

// Note JM 2019-04-16: This is a special case, technically it should be UNIQUEMODELOBJET_TEMPLATES(RunPeriod) since it IS unique right now
// Nevertheless, there is a `std::vector<RunPeriod> SimulationControl::runPeriods()` (a reservation for when we allow multiple RunPeriods)
// UNIQUEMODELOBJECT_TEMPLATES(RunPeriod);
MODELOBJECT_TEMPLATES(RunPeriod);

UNIQUEMODELOBJECT_TEMPLATES(SimulationControl);
UNIQUEMODELOBJECT_TEMPLATES(LightingSimulationControl);
UNIQUEMODELOBJECT_TEMPLATES(SizingParameters);
MODELOBJECT_TEMPLATES(SizingPeriod);
UNIQUEMODELOBJECT_TEMPLATES(WeatherFile);
MODELOBJECT_TEMPLATES(WeatherFileConditionType);
MODELOBJECT_TEMPLATES(WeatherFileDays);
UNIQUEMODELOBJECT_TEMPLATES(ClimateZones);
MODELEXTENSIBLEGROUP_TEMPLATES(ClimateZone);
MODELOBJECT_TEMPLATES(DesignDay);
MODELOBJECT_TEMPLATES(LightingDesignDay);
UNIQUEMODELOBJECT_TEMPLATES(ConvergenceLimits);
UNIQUEMODELOBJECT_TEMPLATES(RunPeriodControlDaylightSavingTime);
UNIQUEMODELOBJECT_TEMPLATES(HeatBalanceAlgorithm);
UNIQUEMODELOBJECT_TEMPLATES(InsideSurfaceConvectionAlgorithm);
UNIQUEMODELOBJECT_TEMPLATES(OutputControlFiles);
UNIQUEMODELOBJECT_TEMPLATES(OutputControlReportingTolerances);
UNIQUEMODELOBJECT_TEMPLATES(OutputControlTableStyle);
UNIQUEMODELOBJECT_TEMPLATES(OutputControlTimestamp);
UNIQUEMODELOBJECT_TEMPLATES(OutputDebuggingData);
UNIQUEMODELOBJECT_TEMPLATES(OutputDiagnostics);
UNIQUEMODELOBJECT_TEMPLATES(OutputJSON);
UNIQUEMODELOBJECT_TEMPLATES(OutputSQLite);
UNIQUEMODELOBJECT_TEMPLATES(OutputSchedules);
UNIQUEMODELOBJECT_TEMPLATES(OutputConstructions);
MODELOBJECT_TEMPLATES(OutputEnvironmentalImpactFactors);
UNIQUEMODELOBJECT_TEMPLATES(EnvironmentalImpactFactors);
MODELOBJECT_TEMPLATES(FuelFactors);
UNIQUEMODELOBJECT_TEMPLATES(OutsideSurfaceConvectionAlgorithm);
MODELOBJECT_TEMPLATES(SurfacePropertyConvectionCoefficientsMultipleSurface);
UNIQUEMODELOBJECT_TEMPLATES(ShadowCalculation);
MODELOBJECT_TEMPLATES(RunPeriodControlSpecialDays);
UNIQUEMODELOBJECT_TEMPLATES(Timestep);
UNIQUEMODELOBJECT_TEMPLATES(ZoneAirContaminantBalance);
UNIQUEMODELOBJECT_TEMPLATES(ZoneAirHeatBalanceAlgorithm);
UNIQUEMODELOBJECT_TEMPLATES(ZoneAirMassFlowConservation);
UNIQUEMODELOBJECT_TEMPLATES(ZoneCapacitanceMultiplierResearchSpecial);
MODELOBJECT_TEMPLATES(SkyTemperature);
UNIQUEMODELOBJECT_TEMPLATES(SiteGroundReflectance);
UNIQUEMODELOBJECT_TEMPLATES(SiteGroundTemperatureBuildingSurface);
UNIQUEMODELOBJECT_TEMPLATES(SiteGroundTemperatureDeep);
UNIQUEMODELOBJECT_TEMPLATES(SiteGroundTemperatureShallow);
UNIQUEMODELOBJECT_TEMPLATES(SiteGroundTemperatureFCfactorMethod);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureUndisturbedKusudaAchenbach);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureUndisturbedXing);
UNIQUEMODELOBJECT_TEMPLATES(SiteWaterMainsTemperature);
#ifndef SWIGCSHARP
  // Ignored for Csharp, use Model::setCalendarYear etc
  UNIQUEMODELOBJECT_TEMPLATES(YearDescription);
#endif
UNIQUEMODELOBJECT_TEMPLATES(FoundationKivaSettings);
UNIQUEMODELOBJECT_TEMPLATES(OutputTableSummaryReports);
UNIQUEMODELOBJECT_TEMPLATES(PerformancePrecisionTradeoffs);

SWIG_UNIQUEMODELOBJECT(RunPeriod);
SWIG_UNIQUEMODELOBJECT(SimulationControl);
SWIG_UNIQUEMODELOBJECT(LightingSimulationControl);
SWIG_UNIQUEMODELOBJECT(SizingParameters);
SWIG_MODELOBJECT(SizingPeriod, 0);
SWIG_UNIQUEMODELOBJECT(WeatherFile);
SWIG_MODELOBJECT(WeatherFileConditionType, 1);
SWIG_MODELOBJECT(WeatherFileDays, 1);
SWIG_UNIQUEMODELOBJECT(ClimateZones);
SWIG_MODELEXTENSIBLEGROUP(ClimateZone);
SWIG_MODELOBJECT(DesignDay, 1);
SWIG_MODELOBJECT(LightingDesignDay, 1);
SWIG_UNIQUEMODELOBJECT(ConvergenceLimits);
SWIG_UNIQUEMODELOBJECT(RunPeriodControlDaylightSavingTime);
SWIG_UNIQUEMODELOBJECT(HeatBalanceAlgorithm);
SWIG_UNIQUEMODELOBJECT(InsideSurfaceConvectionAlgorithm);
SWIG_UNIQUEMODELOBJECT(OutputControlFiles);
SWIG_UNIQUEMODELOBJECT(OutputControlReportingTolerances);
SWIG_UNIQUEMODELOBJECT(OutputControlTableStyle);
SWIG_UNIQUEMODELOBJECT(OutputControlTimestamp);
SWIG_UNIQUEMODELOBJECT(OutputDebuggingData);
SWIG_UNIQUEMODELOBJECT(OutputDiagnostics);
SWIG_UNIQUEMODELOBJECT(OutputJSON);
SWIG_UNIQUEMODELOBJECT(OutputSQLite);
SWIG_UNIQUEMODELOBJECT(OutputSchedules);
SWIG_UNIQUEMODELOBJECT(OutputConstructions);
SWIG_MODELOBJECT(OutputEnvironmentalImpactFactors, 1);
SWIG_UNIQUEMODELOBJECT(EnvironmentalImpactFactors);
SWIG_MODELOBJECT(FuelFactors, 1);
SWIG_UNIQUEMODELOBJECT(OutsideSurfaceConvectionAlgorithm);
SWIG_MODELOBJECT(SurfacePropertyConvectionCoefficientsMultipleSurface, 1);
SWIG_UNIQUEMODELOBJECT(ShadowCalculation);
SWIG_MODELOBJECT(RunPeriodControlSpecialDays, 1);
SWIG_UNIQUEMODELOBJECT(Timestep);
SWIG_UNIQUEMODELOBJECT(ZoneAirContaminantBalance);
SWIG_UNIQUEMODELOBJECT(ZoneAirHeatBalanceAlgorithm);
SWIG_UNIQUEMODELOBJECT(ZoneAirMassFlowConservation);
SWIG_UNIQUEMODELOBJECT(ZoneCapacitanceMultiplierResearchSpecial);
SWIG_MODELOBJECT(SkyTemperature, 1);
SWIG_UNIQUEMODELOBJECT(SiteGroundReflectance);
SWIG_UNIQUEMODELOBJECT(SiteGroundTemperatureBuildingSurface);
SWIG_UNIQUEMODELOBJECT(SiteGroundTemperatureDeep);
SWIG_UNIQUEMODELOBJECT(SiteGroundTemperatureShallow);
SWIG_UNIQUEMODELOBJECT(SiteGroundTemperatureFCfactorMethod);
SWIG_MODELOBJECT(SiteGroundTemperatureUndisturbedKusudaAchenbach, 1);
SWIG_MODELOBJECT(SiteGroundTemperatureUndisturbedXing, 1);
SWIG_UNIQUEMODELOBJECT(SiteWaterMainsTemperature);
#ifndef SWIGCSHARP
  SWIG_UNIQUEMODELOBJECT(YearDescription);
#endif
SWIG_UNIQUEMODELOBJECT(FoundationKivaSettings);
SWIG_UNIQUEMODELOBJECT(OutputTableSummaryReports);
SWIG_UNIQUEMODELOBJECT(PerformancePrecisionTradeoffs);

#if defined SWIGCSHARP || defined(SWIGJAVA)

  %inline {
    namespace openstudio {
      namespace model {
        // Special case (it behaves like a UniqueModelObject while it actually isn't)
        boost::optional<RunPeriod> runPeriod(const openstudio::model::Model& model) {
          return model.runPeriod();
        }

        boost::optional<ClimateZones> climateZones(const openstudio::model::Model& model) {
          return model.climateZones();
        }

        boost::optional<ConvergenceLimits> convergenceLimits(const openstudio::model::Model& model) {
          return model.convergenceLimits();
        }

        boost::optional<EnvironmentalImpactFactors> environmentalImpactFactors(const openstudio::model::Model& model) {
          return model.environmentalImpactFactors();
        }

        boost::optional<FoundationKivaSettings> foundationKivaSettings(const openstudio::model::Model& model) {
          return model.foundationKivaSettings();
        }

        boost::optional<HeatBalanceAlgorithm> heatBalanceAlgorithm(const openstudio::model::Model& model) {
          return model.heatBalanceAlgorithm();
        }

        boost::optional<InsideSurfaceConvectionAlgorithm> insideSurfaceConvectionAlgorithm(const openstudio::model::Model& model) {
          return model.insideSurfaceConvectionAlgorithm();
        }

        boost::optional<LightingSimulationControl> lightingSimulationControl(const openstudio::model::Model& model) {
          return model.lightingSimulationControl();
        }

        boost::optional<OutputControlFiles> outputControlFiles(const openstudio::model::Model& model) {
          return model.outputControlFiles();
        }

        boost::optional<OutputControlReportingTolerances> outputControlReportingTolerances(const openstudio::model::Model& model) {
          return model.outputControlReportingTolerances();
        }

        boost::optional<OutputControlTableStyle> outputControlTableStyle(const openstudio::model::Model& model) {
          return model.outputControlTableStyle();
        }

        boost::optional<OutputControlTimestamp> outputControlTimestamp(const openstudio::model::Model& model) {
          return model.outputControlTimestamp();
        }

        boost::optional<OutputDebuggingData> outputDebuggingData(const openstudio::model::Model& model) {
          return model.outputDebuggingData();
        }

        boost::optional<OutputDiagnostics> outputDiagnostics(const openstudio::model::Model& model) {
          return model.outputDiagnostics();
        }

        boost::optional<OutputJSON> outputJSON(const openstudio::model::Model& model) {
          return model.outputJSON();
        }

        boost::optional<OutputSQLite> outputSQLite(const openstudio::model::Model& model) {
          return model.outputSQLite();
        }

        boost::optional<OutputTableSummaryReports> outputTableSummaryReports(const openstudio::model::Model& model) {
          return model.outputTableSummaryReports();
        }

        boost::optional<OutputSchedules> outputSchedules(const openstudio::model::Model& model) {
          return model.outputSchedules();
        }

        boost::optional<OutputConstructions> outputConstructions(const openstudio::model::Model& model) {
          return model.outputConstructions();
        }

        boost::optional<OutsideSurfaceConvectionAlgorithm> outsideSurfaceConvectionAlgorithm(const openstudio::model::Model& model) {
          return model.outsideSurfaceConvectionAlgorithm();
        }

        boost::optional<PerformancePrecisionTradeoffs> performancePrecisionTradeoffs(const openstudio::model::Model& model) {
          return model.performancePrecisionTradeoffs();
        }

        boost::optional<RunPeriodControlDaylightSavingTime> runPeriodControlDaylightSavingTime(const openstudio::model::Model& model) {
          return model.runPeriodControlDaylightSavingTime();
        }

        boost::optional<ShadowCalculation> shadowCalculation(const openstudio::model::Model& model) {
          return model.shadowCalculation();
        }

        boost::optional<SimulationControl> simulationControl(const openstudio::model::Model& model) {
          return model.simulationControl();
        }

        boost::optional<SiteGroundReflectance> siteGroundReflectance(const openstudio::model::Model& model) {
          return model.siteGroundReflectance();
        }

        boost::optional<SiteGroundTemperatureBuildingSurface> siteGroundTemperatureBuildingSurface(const openstudio::model::Model& model) {
          return model.siteGroundTemperatureBuildingSurface();
        }

        boost::optional<SiteGroundTemperatureDeep> siteGroundTemperatureDeep(const openstudio::model::Model& model) {
          return model.siteGroundTemperatureDeep();
        }

        boost::optional<SiteGroundTemperatureFCfactorMethod> siteGroundTemperatureFCfactorMethod(const openstudio::model::Model& model) {
          return model.siteGroundTemperatureFCfactorMethod();
        }

        boost::optional<SiteGroundTemperatureShallow> siteGroundTemperatureShallow(const openstudio::model::Model& model) {
          return model.siteGroundTemperatureShallow();
        }

        boost::optional<SiteWaterMainsTemperature> siteWaterMainsTemperature(const openstudio::model::Model& model) {
          return model.siteWaterMainsTemperature();
        }

        boost::optional<SizingParameters> sizingParameters(const openstudio::model::Model& model) {
          return model.sizingParameters();
        }

        boost::optional<Timestep> timestep(const openstudio::model::Model& model) {
          return model.timestep();
        }

        boost::optional<WeatherFile> weatherFile(const openstudio::model::Model& model) {
          return model.weatherFile();
        }

        // This is Ignored above
        // boost::optional<YearDescription> yearDescription(const openstudio::model::Model& model) {
        //  return model.yearDescription();
        // }

        boost::optional<ZoneAirContaminantBalance> zoneAirContaminantBalance(const openstudio::model::Model& model) {
          return model.zoneAirContaminantBalance();
        }

        boost::optional<ZoneAirHeatBalanceAlgorithm> zoneAirHeatBalanceAlgorithm(const openstudio::model::Model& model) {
          return model.zoneAirHeatBalanceAlgorithm();
        }

        boost::optional<ZoneAirMassFlowConservation> zoneAirMassFlowConservation(const openstudio::model::Model& model) {
          return model.zoneAirMassFlowConservation();
        }

        boost::optional<ZoneCapacitanceMultiplierResearchSpecial> zoneCapacitanceMultiplierResearchSpecial(const openstudio::model::Model& model) {
          return model.zoneCapacitanceMultiplierResearchSpecial();
        }

      }
    }
  } // %inline

#endif // defined SWIGCSHARP


#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class Model : Workspace {

      // Special case (it behaves like a UniqueModelObject while it actually isn't)
      public OptionalRunPeriod runPeriod() {
        return OpenStudio.OpenStudioModelSimulation.runPeriod(this);
      }


      public OptionalClimateZones climateZones() {
        return OpenStudio.OpenStudioModelSimulation.climateZones(this);
      }

      public OptionalConvergenceLimits convergenceLimits() {
        return OpenStudio.OpenStudioModelSimulation.convergenceLimits(this);
      }

      public OptionalEnvironmentalImpactFactors environmentalImpactFactors() {
        return OpenStudio.OpenStudioModelSimulation.environmentalImpactFactors(this);
      }

      public OptionalFoundationKivaSettings foundationKivaSettings() {
        return OpenStudio.OpenStudioModelSimulation.foundationKivaSettings(this);
      }

      public OptionalHeatBalanceAlgorithm heatBalanceAlgorithm() {
        return OpenStudio.OpenStudioModelSimulation.heatBalanceAlgorithm(this);
      }

      public OptionalInsideSurfaceConvectionAlgorithm insideSurfaceConvectionAlgorithm() {
        return OpenStudio.OpenStudioModelSimulation.insideSurfaceConvectionAlgorithm(this);
      }

      public OptionalLightingSimulationControl lightingSimulationControl() {
        return OpenStudio.OpenStudioModelSimulation.lightingSimulationControl(this);
      }

      public OptionalOutputControlFiles outputControlFiles() {
        return OpenStudio.OpenStudioModelSimulation.outputControlFiles(this);
      }

      public OptionalOutputControlReportingTolerances outputControlReportingTolerances() {
        return OpenStudio.OpenStudioModelSimulation.outputControlReportingTolerances(this);
      }

      public OptionalOutputControlTableStyle outputControlTableStyle() {
        return OpenStudio.OpenStudioModelSimulation.outputControlTableStyle(this);
      }

      public OptionalOutputControlTimestamp outputControlTimestamp() {
        return OpenStudio.OpenStudioModelSimulation.outputControlTimestamp(this);
      }

      public OptionalOutputDebuggingData outputDebuggingData() {
        return OpenStudio.OpenStudioModelSimulation.outputDebuggingData(this);
      }

      public OptionalOutputDiagnostics outputDiagnostics() {
        return OpenStudio.OpenStudioModelSimulation.outputDiagnostics(this);
      }

      public OptionalOutputJSON outputJSON() {
        return OpenStudio.OpenStudioModelSimulation.outputJSON(this);
      }

      public OptionalOutputSQLite outputSQLite() {
        return OpenStudio.OpenStudioModelSimulation.outputSQLite(this);
      }

      public OptionalOutputTableSummaryReports outputTableSummaryReports() {
        return OpenStudio.OpenStudioModelSimulation.outputTableSummaryReports(this);
      }

      public OptionalOutputSchedules outputSchedules() {
        return OpenStudio.OpenStudioModelSimulation.outputSchedules(this);
      }

      public OptionalOutputConstructions outputConstructions() {
        return OpenStudio.OpenStudioModelSimulation.outputConstructions(this);
      }

      public OptionalOutsideSurfaceConvectionAlgorithm outsideSurfaceConvectionAlgorithm() {
        return OpenStudio.OpenStudioModelSimulation.outsideSurfaceConvectionAlgorithm(this);
      }

      public OptionalPerformancePrecisionTradeoffs performancePrecisionTradeoffs() {
        return OpenStudio.OpenStudioModelSimulation.performancePrecisionTradeoffs(this);
      }

      public OptionalRunPeriodControlDaylightSavingTime runPeriodControlDaylightSavingTime() {
        return OpenStudio.OpenStudioModelSimulation.runPeriodControlDaylightSavingTime(this);
      }

      public OptionalShadowCalculation shadowCalculation() {
        return OpenStudio.OpenStudioModelSimulation.shadowCalculation(this);
      }

      public OptionalSimulationControl simulationControl() {
        return OpenStudio.OpenStudioModelSimulation.simulationControl(this);
      }

      public OptionalSiteGroundReflectance siteGroundReflectance() {
        return OpenStudio.OpenStudioModelSimulation.siteGroundReflectance(this);
      }

      public OptionalSiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurface() {
        return OpenStudio.OpenStudioModelSimulation.siteGroundTemperatureBuildingSurface(this);
      }

      public OptionalSiteGroundTemperatureDeep siteGroundTemperatureDeep() {
        return OpenStudio.OpenStudioModelSimulation.siteGroundTemperatureDeep(this);
      }

      public OptionalSiteGroundTemperatureFCfactorMethod siteGroundTemperatureFCfactorMethod() {
        return OpenStudio.OpenStudioModelSimulation.siteGroundTemperatureFCfactorMethod(this);
      }

      public OptionalSiteGroundTemperatureShallow siteGroundTemperatureShallow() {
        return OpenStudio.OpenStudioModelSimulation.siteGroundTemperatureShallow(this);
      }

      public OptionalSiteWaterMainsTemperature siteWaterMainsTemperature() {
        return OpenStudio.OpenStudioModelSimulation.siteWaterMainsTemperature(this);
      }

      public OptionalSizingParameters sizingParameters() {
        return OpenStudio.OpenStudioModelSimulation.sizingParameters(this);
      }

      public OptionalTimestep timestep() {
        return OpenStudio.OpenStudioModelSimulation.timestep(this);
      }

      public OptionalWeatherFile weatherFile() {
        return OpenStudio.OpenStudioModelSimulation.weatherFile(this);
      }

      // This is ignored above
      // public OptionalYearDescription yearDescription() {
      //   return OpenStudio.OpenStudioModelSimulation.yearDescription(this);
      // }

      public OptionalZoneAirContaminantBalance zoneAirContaminantBalance() {
        return OpenStudio.OpenStudioModelSimulation.zoneAirContaminantBalance(this);
      }

      public OptionalZoneAirHeatBalanceAlgorithm zoneAirHeatBalanceAlgorithm() {
        return OpenStudio.OpenStudioModelSimulation.zoneAirHeatBalanceAlgorithm(this);
      }

      public OptionalZoneAirMassFlowConservation zoneAirMassFlowConservation() {
        return OpenStudio.OpenStudioModelSimulation.zoneAirMassFlowConservation(this);
      }

      public OptionalZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecial() {
        return OpenStudio.OpenStudioModelSimulation.zoneCapacitanceMultiplierResearchSpecial(this);
      }

    } // partial class Model
  %} // pragma
#endif // defined(SWIGCSHARP)

#endif // MODEL_GEOMETRY_I
