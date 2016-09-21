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

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::Name);
  if (!s) {
    LOG(Error, "EnergyManagementSystem_OutputVariable Name not set");
    return boost::none;
  }
  openstudio::model::EnergyManagementSystemOutputVariable emsOutputVariable(m_model);
  emsOutputVariable.setName(*s);

  s = workspaceObject.getString(EnergyManagementSystem_OutputVariableFields::EMSVariableName);
  if (!s) {
    LOG(Error, emsOutputVariable.nameString() + ": EMSVariableName not set");
    return boost::none;
  } else {
    emsOutputVariable.setEMSVariableName(*s);
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
    emsOutputVariable.setTypeofDatainVariable(*s);
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
        if (modelObject.get().iddObjectType() == IddObjectType::EnergyManagementSystem_Program) {
          emsOutputVariable.setEMSProgramorSubroutineName(modelObject.get().cast<EnergyManagementSystemProgram>());
        } else if (modelObject.get().iddObjectType() == IddObjectType::EnergyManagementSystem_Subroutine) {
          emsOutputVariable.setEMSProgramorSubroutineName(modelObject.get().cast<EnergyManagementSystemSubroutine>());
        }
        return emsOutputVariable;
      }
    }

  }

  return emsOutputVariable;
}

} // energyplus

} // openstudio

