#ifndef MODEL_PLANTEQUIPMENTOPERATIONSCHEME_I
#define MODEL_PLANTEQUIPMENTOPERATIONSCHEME_I


#ifdef SWIGPYTHON
  %module openstudiomodelplantequipmentoperationscheme
#endif

%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>

#if defined SWIGCSHARP
  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelPlantEquipmentOperationScheme
#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

MODELOBJECT_TEMPLATES(PlantEquipmentOperationCoolingLoad);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationHeatingLoad);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorDryBulb);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorWetBulb);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorDewpoint);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorRelativeHumidity);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorDryBulbDifference);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorWetBulbDifference);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationOutdoorDewpointDifference);

SWIG_MODELOBJECT(PlantEquipmentOperationCoolingLoad,1);
SWIG_MODELOBJECT(PlantEquipmentOperationHeatingLoad,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorDryBulb,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorWetBulb,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorDewpoint,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorRelativeHumidity,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorDryBulbDifference,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorWetBulbDifference,1);
SWIG_MODELOBJECT(PlantEquipmentOperationOutdoorDewpointDifference,1);


#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        boost::optional<PlantEquipmentOperationHeatingLoad> plantEquipmentOperationHeatingLoad(const openstudio::model::PlantLoop& plantLoop){
          return plantLoop.plantEquipmentOperationHeatingLoad();
        }

        bool setPlantEquipmentOperationHeatingLoad(openstudio::model::PlantLoop plantLoop,
                                                   openstudio::model::PlantEquipmentOperationHeatingLoad eq)
        {
          return plantLoop.setPlantEquipmentOperationHeatingLoad(eq);
        }

        boost::optional<PlantEquipmentOperationCoolingLoad> plantEquipmentOperationCoolingLoad(const openstudio::model::PlantLoop& plantLoop){
          return plantLoop.plantEquipmentOperationCoolingLoad();
        }

        bool setPlantEquipmentOperationCoolingLoad(openstudio::model::PlantLoop plantLoop,
                                                   openstudio::model::PlantEquipmentOperationCoolingLoad eq)
        {
          return plantLoop.setPlantEquipmentOperationCoolingLoad(eq);
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

    public partial class PlantLoop : Loop
    {
      public OptionalPlantEquipmentOperationHeatingLoad plantEquipmentOperationHeatingLoad() {
        return OpenStudio.OpenStudioModelPlantEquipmentOperationScheme.plantEquipmentOperationHeatingLoad(this);
      }

      public bool setPlantEquipmentOperationHeatingLoad(OpenStudio.PlantEquipmentOperationHeatingLoad eq) {
        return OpenStudio.OpenStudioModelPlantEquipmentOperationScheme.setPlantEquipmentOperationHeatingLoad(this, eq);
      }

      public OptionalPlantEquipmentOperationCoolingLoad plantEquipmentOperationCoolingLoad() {
        return OpenStudio.OpenStudioModelPlantEquipmentOperationScheme.plantEquipmentOperationCoolingLoad(this);
      }

      public bool setPlantEquipmentOperationCoolingLoad(OpenStudio.PlantEquipmentOperationCoolingLoad eq) {
        return OpenStudio.OpenStudioModelPlantEquipmentOperationScheme.setPlantEquipmentOperationCoolingLoad(this, eq);
      }
    }

  %}
#endif

#endif
