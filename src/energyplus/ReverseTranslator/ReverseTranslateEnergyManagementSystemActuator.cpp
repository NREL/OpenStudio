/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/EnergyManagementSystemActuator_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemActuator(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Actuator) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Actuator");
      return boost::none;
    }

    OptionalString s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::Name);
    if (!s1) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_Actuator has no name");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName);
    if (!s) {
      LOG(Error, workspaceObject.nameString() + ": has no ActuatedComponentUniqueName");
      return boost::none;
    }

    Workspace workspace = workspaceObject.workspace();

    if (s) {
      std::vector<WorkspaceObject> wsObjects = workspace.getObjectsByName(*s);
      if (wsObjects.size() > 1) {
        LOG(Error, workspaceObject.nameString() + ": ActuatedComponentUniqueName is not unique.  More than 1 object with that name.");
        return boost::none;
      }
      if (wsObjects.empty()) {
        LOG(Error, workspaceObject.nameString() + ": ActuatedComponentUniqueName not found.");
        return boost::none;
      } else {
        //ActuatedComponent's name is unique so we can continue and try to translate that object.
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObjects[0]);
        if (modelObject) {
          openstudio::model::EnergyManagementSystemActuator emsActuator(modelObject.get());
          emsActuator.setName(*s1);
          emsActuator.setActuatedComponent(modelObject.get());
          s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
          if (s1) {
            emsActuator.setActuatedComponentType(*s1);
          }

          s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
          if (s1) {
            emsActuator.setActuatedComponentControlType(*s1);
          }
          return emsActuator;
        } else {
          LOG(Warn, "ATTENTION: Actuated Object " + wsObjects[0].nameString() + " does not reverse translate. ActuatedComponent is NOT set.");
          openstudio::model::EnergyManagementSystemActuator emsActuator(m_model);
          OptionalString s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::Name);
          emsActuator.setName(*s1);
          s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName);
          //Set actuated component as a comment since it cannot be reverse translated.
          emsActuator.setComment("Actuated Component Name = " + *s1);
          s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
          if (s1) {
            emsActuator.setActuatedComponentType(*s1);
          }

          s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
          if (s1) {
            emsActuator.setActuatedComponentControlType(*s1);
          }
          return emsActuator;
        }
      }
    }
    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
