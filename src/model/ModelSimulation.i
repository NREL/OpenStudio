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
UNIQUEMODELOBJECT_TEMPLATES(OutputDebuggingData);
UNIQUEMODELOBJECT_TEMPLATES(OutputDiagnostics);
UNIQUEMODELOBJECT_TEMPLATES(OutputJSON);
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
MODELOBJECT_TEMPLATES(SiteGroundReflectance);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureBuildingSurface);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureDeep);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureShallow);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureFCfactorMethod);
MODELOBJECT_TEMPLATES(SiteWaterMainsTemperature);
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
SWIG_UNIQUEMODELOBJECT(OutputDebuggingData);
SWIG_UNIQUEMODELOBJECT(OutputDiagnostics);
SWIG_UNIQUEMODELOBJECT(OutputJSON);
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
        boost::optional<OutputControlFiles> outputControlFiles(const openstudio::model::Model& model){
          return model.outputControlFiles();
        }
      }
    }
  }

#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class Model : Workspace {
      public OptionalOutputControlFiles outputControlFiles()
      {
        return OpenStudio.OpenStudioModelSimulation.outputControlFiles(this);
      }
    }
  %}
#endif

#endif // MODEL_GEOMETRY_I
