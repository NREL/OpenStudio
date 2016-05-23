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

  //%rename(ModelYearDescription) openstudio::model::YearDescription;
  %ignore openstudio::model::YearDescription;

  // site is being imported from weather file
  // schedule day is being imported from design day
  // might want to move both of those to geometry so they can import resources and site

#endif

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
UNIQUEMODELOBJECT_TEMPLATES(OutputControlReportingTolerances);
UNIQUEMODELOBJECT_TEMPLATES(OutsideSurfaceConvectionAlgorithm);
MODELOBJECT_TEMPLATES(SurfacePropertyConvectionCoefficientsMultipleSurface);
UNIQUEMODELOBJECT_TEMPLATES(RunPeriod);
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
UNIQUEMODELOBJECT_TEMPLATES(YearDescription);

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
SWIG_UNIQUEMODELOBJECT(OutputControlReportingTolerances);
SWIG_UNIQUEMODELOBJECT(OutsideSurfaceConvectionAlgorithm);
SWIG_MODELOBJECT(SurfacePropertyConvectionCoefficientsMultipleSurface, 1);
SWIG_UNIQUEMODELOBJECT(RunPeriod);
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
SWIG_UNIQUEMODELOBJECT(YearDescription);

#endif
