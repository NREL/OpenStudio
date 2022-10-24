#ifndef MODEL_I
#define MODEL_I

#ifdef SWIGPYTHON
  %module(directors="1") openstudiomodel

  // Inject every submodule into the openstudio.model namespace, so we can do like in Ruby
  // `fan = openstudio.model.FanVariableVolume(m)` (and not `openstudio.model.openstudiomodelstraightcomponent.FanVariableVolume(m)`)
  %pythoncode %{
    from openstudiomodelcore import *
    from openstudiomodelsimulation import *
    from openstudiomodelresources import *
    from openstudiomodelgeometry import *
    from openstudiomodelhvac import *
    from openstudiomodelzonehvac import *
    from openstudiomodelavailabilitymanager import *
    from openstudiomodelplantequipmentoperationscheme import *
    from openstudiomodelstraightcomponent import *
    from openstudiomodelairflow import *
    from openstudiomodelrefrigeration import *
    from openstudiomodelgenerators import *
  %}

#endif


%include <model/Model_Common_Include.i>

%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>
%import <model/ModelZoneHVAC.i>
%import <model/ModelAvailabilityManager.i>
%import <model/ModelPlantEquipmentOperationScheme.i>
%import <model/ModelStraightComponent.i>
%import <model/ModelAirflow.i>
%import <model/ModelRefrigeration.i>
%import <model/ModelGenerators.i>

#endif //MODEL_I
