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

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelRefrigeration
  
#endif

MODELOBJECT_TEMPLATES(RefrigerationCase);
MODELOBJECT_TEMPLATES(RefrigerationCompressor);
MODELOBJECT_TEMPLATES(RefrigerationCondenserAirCooled);
MODELOBJECT_TEMPLATES(RefrigerationCondenserCascade);
MODELOBJECT_TEMPLATES(RefrigerationCondenserEvaporativeCooled);
MODELOBJECT_TEMPLATES(RefrigerationCondenserWaterCooled);
MODELOBJECT_TEMPLATES(RefrigerationSubcoolerLiquidSuction);
MODELOBJECT_TEMPLATES(RefrigerationSubcoolerMechanical);
MODELOBJECT_TEMPLATES(RefrigerationSecondarySystem);
MODELOBJECT_TEMPLATES(RefrigerationSystem);
MODELOBJECT_TEMPLATES(RefrigerationWalkIn);
MODELOBJECT_TEMPLATES(RefrigerationWalkInZoneBoundary);

SWIG_MODELOBJECT(RefrigerationCase);
SWIG_MODELOBJECT(RefrigerationCompressor);
SWIG_MODELOBJECT(RefrigerationCondenserAirCooled);
SWIG_MODELOBJECT(RefrigerationCondenserCascade);
SWIG_MODELOBJECT(RefrigerationCondenserEvaporativeCooled);
SWIG_MODELOBJECT(RefrigerationCondenserWaterCooled);
SWIG_MODELOBJECT(RefrigerationSubcoolerLiquidSuction);
SWIG_MODELOBJECT(RefrigerationSubcoolerMechanical);
SWIG_MODELOBJECT(RefrigerationSecondarySystem);
SWIG_MODELOBJECT(RefrigerationSystem);
SWIG_MODELOBJECT(RefrigerationWalkIn);
SWIG_MODELOBJECT(RefrigerationWalkInZoneBoundary);

#endif 
