#ifndef MODEL_REFRIGERATION_I
#define MODEL_REFRIGERATION_I

#ifdef SWIGPYTHON
  %module openstudiomodelrefrigeration
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>
%import <model/ModelZoneHVAC.i>

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelRefrigeration

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
  namespace model {

    // forward declarations
    class RefrigerationSystem;
  }
}

MODELOBJECT_TEMPLATES(RefrigerationAirChiller);
MODELOBJECT_TEMPLATES(RefrigerationCase);
MODELOBJECT_TEMPLATES(RefrigerationCompressor);
MODELOBJECT_TEMPLATES(RefrigerationCompressorRack);
MODELOBJECT_TEMPLATES(RefrigerationCondenserAirCooled);
MODELOBJECT_TEMPLATES(RefrigerationCondenserCascade);
MODELOBJECT_TEMPLATES(RefrigerationCondenserEvaporativeCooled);
MODELOBJECT_TEMPLATES(RefrigerationCondenserWaterCooled);
MODELOBJECT_TEMPLATES(RefrigerationDefrostCycleParameters);
MODELOBJECT_TEMPLATES(RefrigerationGasCoolerAirCooled);
MODELOBJECT_TEMPLATES(RefrigerationSubcoolerLiquidSuction);
MODELOBJECT_TEMPLATES(RefrigerationSubcoolerMechanical);
MODELOBJECT_TEMPLATES(RefrigerationSecondarySystem);
MODELOBJECT_TEMPLATES(RefrigerationSystem);
MODELOBJECT_TEMPLATES(RefrigerationTranscriticalSystem);
MODELOBJECT_TEMPLATES(RefrigerationWalkIn);
MODELOBJECT_TEMPLATES(RefrigerationWalkInZoneBoundary);

SWIG_MODELOBJECT(RefrigerationAirChiller, 1);
SWIG_MODELOBJECT(RefrigerationCase, 1);
SWIG_MODELOBJECT(RefrigerationCompressor, 1);
SWIG_MODELOBJECT(RefrigerationCompressorRack, 1);
SWIG_MODELOBJECT(RefrigerationCondenserAirCooled, 1);
SWIG_MODELOBJECT(RefrigerationCondenserCascade, 1);
SWIG_MODELOBJECT(RefrigerationCondenserEvaporativeCooled, 1);
SWIG_MODELOBJECT(RefrigerationCondenserWaterCooled, 1);
SWIG_MODELOBJECT(RefrigerationDefrostCycleParameters, 1);
SWIG_MODELOBJECT(RefrigerationGasCoolerAirCooled, 1);
SWIG_MODELOBJECT(RefrigerationSubcoolerLiquidSuction, 1);
SWIG_MODELOBJECT(RefrigerationSubcoolerMechanical, 1);
SWIG_MODELOBJECT(RefrigerationSecondarySystem, 1);
SWIG_MODELOBJECT(RefrigerationSystem, 1);
SWIG_MODELOBJECT(RefrigerationTranscriticalSystem, 1);
SWIG_MODELOBJECT(RefrigerationWalkIn, 1);
SWIG_MODELOBJECT(RefrigerationWalkInZoneBoundary, 1);

#endif
