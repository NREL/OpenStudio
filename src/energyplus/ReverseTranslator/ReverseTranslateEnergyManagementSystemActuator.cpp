/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
      if (wsObjects.size() == 0) {
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
