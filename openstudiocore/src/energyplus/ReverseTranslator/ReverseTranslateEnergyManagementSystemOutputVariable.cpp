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

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemOutputVariable(const WorkspaceObject & workspaceObject)
{
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

} // energyplus

} // openstudio

