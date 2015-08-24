#ifndef MODEL_HVAC_I
#define MODEL_HVAC_I

#ifdef SWIGPYTHON
  %module openstudiomodelhvac
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>


#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelGenerators
  
#endif

MODELOBJECT_TEMPLATES(Generator);
MODELOBJECT_TEMPLATES(Inverter);

SWIG_MODELOBJECT(Generator, 0);
SWIG_MODELOBJECT(Inverter, 0);


#endif 
