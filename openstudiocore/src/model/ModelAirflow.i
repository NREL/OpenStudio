#ifndef MODEL_AIRFLOW_I
#define MODEL_AIRFLOW_I

#ifdef SWIGPYTHON
  %module openstudiomodelairflow
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelAirflow

#endif

// May be able to remove these includes once they are included by concrete classes
%{
  #include <model/AirflowNetworkComponent.hpp>
  #include <model/AirflowNetworkComponent_Impl.hpp>
  #include <model/AirflowNetworkNode.hpp>
  #include <model/AirflowNetworkNode_Impl.hpp>
  #include <model/AirflowNetworkLinkage.hpp>
  #include <model/AirflowNetworkLinkage_Impl.hpp>
%}


// Base classes
MODELOBJECT_TEMPLATES(AirflowNetworkComponent);
MODELOBJECT_TEMPLATES(AirflowNetworkNode);
MODELOBJECT_TEMPLATES(AirflowNetworkLinkage);

SWIG_MODELOBJECT(AirflowNetworkComponent, 0);
SWIG_MODELOBJECT(AirflowNetworkNode, 0);
SWIG_MODELOBJECT(AirflowNetworkLinkage, 0);

// Concrete classes
MODELOBJECT_TEMPLATES(AirflowNetworkConstantPressureDrop);
MODELOBJECT_TEMPLATES(AirflowNetworkCrack);
MODELOBJECT_TEMPLATES(AirflowNetworkDetailedOpening);
MODELOBJECT_TEMPLATES(AirflowNetworkDistributionLinkage);
MODELOBJECT_TEMPLATES(AirflowNetworkDistributionNode);
MODELOBJECT_TEMPLATES(AirflowNetworkDuct);
MODELOBJECT_TEMPLATES(AirflowNetworkDuctViewFactors);
MODELOBJECT_TEMPLATES(AirflowNetworkEffectiveLeakageArea);
MODELOBJECT_TEMPLATES(AirflowNetworkEquivalentDuct);
MODELOBJECT_TEMPLATES(AirflowNetworkExternalNode);
MODELOBJECT_TEMPLATES(AirflowNetworkFan);
MODELOBJECT_TEMPLATES(AirflowNetworkHorizontalOpening);
MODELOBJECT_TEMPLATES(AirflowNetworkLeakageRatio);
MODELOBJECT_TEMPLATES(AirflowNetworkOccupantVentilationControl);
MODELOBJECT_TEMPLATES(AirflowNetworkOutdoorAirflow);
MODELOBJECT_TEMPLATES(AirflowNetworkReferenceCrackConditions);
MODELOBJECT_TEMPLATES(AirflowNetworkSimpleOpening);
UNIQUEMODELOBJECT_TEMPLATES(AirflowNetworkSimulationControl);
MODELOBJECT_TEMPLATES(AirflowNetworkSurface);
MODELOBJECT_TEMPLATES(AirflowNetworkZone);
MODELOBJECT_TEMPLATES(AirflowNetworkZoneExhaustFan);

SWIG_MODELOBJECT(AirflowNetworkConstantPressureDrop, 1);
SWIG_MODELOBJECT(AirflowNetworkCrack, 1);
SWIG_MODELOBJECT(AirflowNetworkDetailedOpening, 1);
SWIG_MODELOBJECT(AirflowNetworkDistributionLinkage, 1);
SWIG_MODELOBJECT(AirflowNetworkDistributionNode, 1);
SWIG_MODELOBJECT(AirflowNetworkDuct, 1);
SWIG_MODELOBJECT(AirflowNetworkDuctViewFactors, 1);
SWIG_MODELOBJECT(AirflowNetworkEffectiveLeakageArea, 1);
SWIG_MODELOBJECT(AirflowNetworkEquivalentDuct, 1);
SWIG_MODELOBJECT(AirflowNetworkExternalNode, 1);
SWIG_MODELOBJECT(AirflowNetworkFan, 1);
SWIG_MODELOBJECT(AirflowNetworkHorizontalOpening, 1);
SWIG_MODELOBJECT(AirflowNetworkLeakageRatio, 1);
SWIG_MODELOBJECT(AirflowNetworkOccupantVentilationControl, 1);
SWIG_MODELOBJECT(AirflowNetworkOutdoorAirflow, 1);
SWIG_MODELOBJECT(AirflowNetworkReferenceCrackConditions, 1);
SWIG_MODELOBJECT(AirflowNetworkSimpleOpening, 1);
SWIG_UNIQUEMODELOBJECT(AirflowNetworkSimulationControl);
SWIG_MODELOBJECT(AirflowNetworkSurface, 1);
SWIG_MODELOBJECT(AirflowNetworkZone, 1);
SWIG_MODELOBJECT(AirflowNetworkZoneExhaustFan, 1);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        openstudio::model::AirflowNetworkZone getAirflowNetworkZone(openstudio::model::ThermalZone& zone){
          return zone.getAirflowNetworkZone();
        }
        boost::optional<openstudio::model::AirflowNetworkZone> getOptionalAirflowNetworkZone(const openstudio::model::ThermalZone& zone){
          return zone.airflowNetworkZone();
        }
        //void removeAirflowNetworkZone(openstudio::model::AirflowNetworkZone zone){
        //  boost::optional<openstudio::model::AirflowNetworkZone> afz = zone.optionalAirflowNetworkZone();
        //  if (afz){
        //    afz->remove();
        //  }
        //}
      }
    }
  }
#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class ThermalZone : HVACComponent {

      // DLM: these can be re-enabled after they are hidden in ModelHVAC.i
      //public AirflowNetworkZone getAirflowNetworkZone()
      //{
      //  return OpenStudio.OpenStudioModelAirflow.getAirflowNetworkZone(this);
      //}
      //public OptionalAirflowNetworkZone airflowNetworkZone()
      //{
      //  return OpenStudio.OpenStudioModelAirflow.getOptionalAirflowNetworkZone(this);
      //}
    }
  %}
#endif

#endif
