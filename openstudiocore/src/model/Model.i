#ifndef MODEL_I
#define MODEL_I

#ifdef SWIGPYTHON
  %module openstudiomodel

  %pythoncode %{
    from openstudiomodelcore import *
    from openstudiomodelsimulation import *
    from openstudiomodelresources import *
    from openstudiomodelgeometry import *
    from openstudiomodelhvac import *
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
