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
MODELOBJECT_TEMPLATES(SizingParameters);
MODELOBJECT_TEMPLATES(SizingPeriod);
UNIQUEMODELOBJECT_TEMPLATES(TimeDependentValuation);
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
UNIQUEMODELOBJECT_TEMPLATES(OutsideSurfaceConvectionAlgorithm);
UNIQUEMODELOBJECT_TEMPLATES(RunPeriod);
UNIQUEMODELOBJECT_TEMPLATES(ShadowCalculation);
MODELOBJECT_TEMPLATES(RunPeriodControlSpecialDays);
UNIQUEMODELOBJECT_TEMPLATES(Timestep);
UNIQUEMODELOBJECT_TEMPLATES(ZoneAirContaminantBalance);
UNIQUEMODELOBJECT_TEMPLATES(ZoneAirHeatBalanceAlgorithm);
UNIQUEMODELOBJECT_TEMPLATES(ZoneCapacitanceMultiplierResearchSpecial);
MODELOBJECT_TEMPLATES(SkyTemperature);
MODELOBJECT_TEMPLATES(SiteGroundReflectance);
MODELOBJECT_TEMPLATES(SiteGroundTemperatureBuildingSurface);
MODELOBJECT_TEMPLATES(SiteWaterMainsTemperature);
UNIQUEMODELOBJECT_TEMPLATES(YearDescription);

SWIG_UNIQUEMODELOBJECT(SimulationControl);
SWIG_UNIQUEMODELOBJECT(LightingSimulationControl);
SWIG_MODELOBJECT(SizingParameters);
SWIG_MODELOBJECT(SizingPeriod);
SWIG_UNIQUEMODELOBJECT(TimeDependentValuation);
SWIG_UNIQUEMODELOBJECT(WeatherFile);
SWIG_MODELOBJECT(WeatherFileConditionType);
SWIG_MODELOBJECT(WeatherFileDays);
SWIG_UNIQUEMODELOBJECT(ClimateZones);
SWIG_MODELEXTENSIBLEGROUP(ClimateZone);
SWIG_MODELOBJECT(DesignDay);
SWIG_MODELOBJECT(LightingDesignDay);
SWIG_UNIQUEMODELOBJECT(ConvergenceLimits);
SWIG_UNIQUEMODELOBJECT(RunPeriodControlDaylightSavingTime);
SWIG_UNIQUEMODELOBJECT(HeatBalanceAlgorithm);
SWIG_UNIQUEMODELOBJECT(InsideSurfaceConvectionAlgorithm);
SWIG_UNIQUEMODELOBJECT(OutsideSurfaceConvectionAlgorithm);
SWIG_UNIQUEMODELOBJECT(RunPeriod);
SWIG_UNIQUEMODELOBJECT(ShadowCalculation);
SWIG_MODELOBJECT(RunPeriodControlSpecialDays);
SWIG_UNIQUEMODELOBJECT(Timestep);
SWIG_UNIQUEMODELOBJECT(ZoneAirContaminantBalance);
SWIG_UNIQUEMODELOBJECT(ZoneAirHeatBalanceAlgorithm);
SWIG_UNIQUEMODELOBJECT(ZoneCapacitanceMultiplierResearchSpecial);
SWIG_MODELOBJECT(SkyTemperature);
SWIG_UNIQUEMODELOBJECT(SiteGroundReflectance);
SWIG_UNIQUEMODELOBJECT(SiteGroundTemperatureBuildingSurface);
SWIG_UNIQUEMODELOBJECT(SiteWaterMainsTemperature);
SWIG_UNIQUEMODELOBJECT(YearDescription);

#endif 
