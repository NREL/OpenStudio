/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/EnergyManagementSystemActuator_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemActuator(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Actuator) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Actuator");
    return boost::none;
  }

  OptionalString s1 = workspaceObject.getString(EnergyManagementSystem_ActuatorFields::Name);
  if(!s1){
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
    for ( WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
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
        LOG(Error, "ATTENTION: Actuated Object " + wsObject.nameString() + " does not reverse translate. ActuatedComponent is NOT set.");
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

} // energyplus

} // openstudio

