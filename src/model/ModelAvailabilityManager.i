#ifndef MODEL_AVAILABILITYMANAGER_I
#define MODEL_AVAILABILITYMANAGER_I


#ifdef SWIGPYTHON
  %module openstudiomodelavailabilitymanager
#endif

%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>

#if defined SWIGCSHARP
  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelAvailabilityManager
#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

MODELOBJECT_TEMPLATES(AvailabilityManagerNightCycle);
MODELOBJECT_TEMPLATES(AvailabilityManagerHybridVentilation);
MODELOBJECT_TEMPLATES(AvailabilityManagerOptimumStart);
MODELOBJECT_TEMPLATES(AvailabilityManagerDifferentialThermostat);
MODELOBJECT_TEMPLATES(AvailabilityManagerNightVentilation);
MODELOBJECT_TEMPLATES(AvailabilityManagerHighTemperatureTurnOn);
MODELOBJECT_TEMPLATES(AvailabilityManagerHighTemperatureTurnOff);
MODELOBJECT_TEMPLATES(AvailabilityManagerLowTemperatureTurnOn);
MODELOBJECT_TEMPLATES(AvailabilityManagerLowTemperatureTurnOff);
MODELOBJECT_TEMPLATES(AvailabilityManagerScheduled);
MODELOBJECT_TEMPLATES(AvailabilityManagerScheduledOn);
MODELOBJECT_TEMPLATES(AvailabilityManagerScheduledOff);

SWIG_MODELOBJECT(AvailabilityManagerNightCycle,1);
SWIG_MODELOBJECT(AvailabilityManagerHybridVentilation,1);
SWIG_MODELOBJECT(AvailabilityManagerOptimumStart,1);
SWIG_MODELOBJECT(AvailabilityManagerDifferentialThermostat,1);
SWIG_MODELOBJECT(AvailabilityManagerNightVentilation,1);
SWIG_MODELOBJECT(AvailabilityManagerHighTemperatureTurnOn,1);
SWIG_MODELOBJECT(AvailabilityManagerHighTemperatureTurnOff,1);
SWIG_MODELOBJECT(AvailabilityManagerLowTemperatureTurnOn,1);
SWIG_MODELOBJECT(AvailabilityManagerLowTemperatureTurnOff,1);
SWIG_MODELOBJECT(AvailabilityManagerScheduled,1);
SWIG_MODELOBJECT(AvailabilityManagerScheduledOn,1);
SWIG_MODELOBJECT(AvailabilityManagerScheduledOff,1);

#endif
