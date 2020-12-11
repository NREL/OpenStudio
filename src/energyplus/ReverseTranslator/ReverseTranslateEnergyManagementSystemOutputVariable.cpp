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

#include "../../model/EnergyManagementSystemOutputVariable.hpp"
#include "../../model/EnergyManagementSystemOutputVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemOutputVariable(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_OutputVariable) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_OutputVariable");
      return boost::none;
    }

    //make sure all other objects are translated first except below
    for (const WorkspaceObject& workspaceObject : m_workspace.objects()) {
      if ((workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Program)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ProgramCallingManager)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_MeteredOutputVariable)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_OutputVariable)) {
        translateAndMapWorkspaceObject(workspaceObject);
      }
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::Name);
    if (!s) {
      LOG(Error, "EnergyManagementSystem_OutputVariable has no Name");
      return boost::none;
    }
    openstudio::model::EnergyManagementSystemOutputVariable emsOutputVariable(m_model);
    emsOutputVariable.setName(*s);

    // mark this object as translated so we don't recursively translate it below
    m_workspaceToModelMap.insert(std::make_pair(workspaceObject.handle(), emsOutputVariable));

    s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": EMSVariableName not set");
      return boost::none;
    } else {
      Workspace workspace = workspaceObject.workspace();
      //look for GlobalVariables, translate and check if there is a name match since GV's dont have name field.
      boost::optional<WorkspaceObject> wsObject = workspace.getObjectByTypeAndName(IddObjectType::EnergyManagementSystem_GlobalVariable, *s);
      //for (WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)) {
      if (wsObject) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject.get());
        if (modelObject) {
          if (modelObject.get().cast<EnergyManagementSystemGlobalVariable>().name() == s) {
            emsOutputVariable.setEMSVariableName(*s);
          }
        }
      }
      //look for name match on other (EMS) objects.
      // DLM: I am not really sure what this is supposed to do, it was recursively calling translateAndMapWorkspaceObject on the current object.
      // DLM: Also, it is continually setting the name on emsOutputVariable and not the new modelObject
      for (WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
        if (modelObject) {
          emsOutputVariable.setEMSVariableName(*s);
          break;
        }
      }
    }

    s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::UpdateFrequency);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": UpdateFrequency not set");
      return boost::none;
    } else {
      emsOutputVariable.setUpdateFrequency(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": TypeofDatainVariable not set");
      return boost::none;
    } else {
      emsOutputVariable.setTypeOfDataInVariable(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::Units);
    if (s) {
      emsOutputVariable.setUnits(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName);
    if (s) {
      Workspace workspace = workspaceObject.workspace();
      for (WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
        if (modelObject) {
          if (modelObject.get().iddObjectType() == IddObjectType::OS_EnergyManagementSystem_Program) {
            emsOutputVariable.setEMSProgramOrSubroutineName(modelObject.get().cast<EnergyManagementSystemProgram>());
          } else if (modelObject.get().iddObjectType() == IddObjectType::OS_EnergyManagementSystem_Subroutine) {
            emsOutputVariable.setEMSProgramOrSubroutineName(modelObject.get().cast<EnergyManagementSystemSubroutine>());
          }
          return emsOutputVariable;
        }
      }
    }

    return emsOutputVariable;
  }

}  // namespace energyplus

}  // namespace openstudio
