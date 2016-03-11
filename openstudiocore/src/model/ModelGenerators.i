#ifndef MODEL_GENERATORS_I
#define MODEL_GENERATORS_I

#ifdef SWIGPYTHON
  %module openstudiomodelgenerators
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>

%{
  #include <model/PhotovoltaicPerformance.hpp>
  #include <model/PhotovoltaicPerformance_Impl.hpp>
  #include <model/Generator.hpp>
  #include <model/Generator_Impl.hpp>
  #include <model/Inverter.hpp>
  #include <model/Inverter_Impl.hpp>
%}


#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelGenerators
  
#endif

namespace openstudio {
  namespace model {

    // forward declarations
    class Generator;
  }
}

MODELOBJECT_TEMPLATES(PhotovoltaicPerformance);
MODELOBJECT_TEMPLATES(Generator);
MODELOBJECT_TEMPLATES(Inverter);
MODELOBJECT_TEMPLATES(GeneratorPhotovoltaic);
MODELOBJECT_TEMPLATES(ElectricLoadCenterDistribution);
MODELOBJECT_TEMPLATES(ElectricLoadCenterInverterLookUpTable);
MODELOBJECT_TEMPLATES(ElectricLoadCenterInverterSimple);
MODELOBJECT_TEMPLATES(PhotovoltaicPerformanceEquivalentOneDiode);
MODELOBJECT_TEMPLATES(PhotovoltaicPerformanceSimple);

SWIG_MODELOBJECT(PhotovoltaicPerformance, 0);
SWIG_MODELOBJECT(Generator, 0);
SWIG_MODELOBJECT(Inverter, 0);
SWIG_MODELOBJECT(GeneratorPhotovoltaic, 1);
SWIG_MODELOBJECT(ElectricLoadCenterDistribution, 1);
SWIG_MODELOBJECT(ElectricLoadCenterInverterLookUpTable, 1);
SWIG_MODELOBJECT(ElectricLoadCenterInverterSimple, 1);
SWIG_MODELOBJECT(PhotovoltaicPerformanceEquivalentOneDiode, 1);
SWIG_MODELOBJECT(PhotovoltaicPerformanceSimple, 1);

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
