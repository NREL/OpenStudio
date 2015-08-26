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
MODELOBJECT_TEMPLATES(GeneratorPhotovoltaic);
MODELOBJECT_TEMPLATES(ElectricLoadCenterDistribution);

SWIG_MODELOBJECT(Generator, 0);
SWIG_MODELOBJECT(Inverter, 0);
SWIG_MODELOBJECT(GeneratorPhotovoltaic, 1);
SWIG_MODELOBJECT(ElectricLoadCenterDistribution, 1);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        std::vector<openstudio::model::GeneratorPhotovoltaic> getGeneratorPhotovoltaics(const openstudio::model::PlanarSurface& surface){
          return surface.generatorPhotovoltaics();
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
        
    public partial class PlanarSurface : ParentObject {
      public GeneratorPhotovoltaicVector generatorPhotovoltaics()
      {
        return OpenStudio.OpenStudioModelGenerators.getGeneratorPhotovoltaics(this);
      }
    }  
    
  %}
#endif

#endif 
