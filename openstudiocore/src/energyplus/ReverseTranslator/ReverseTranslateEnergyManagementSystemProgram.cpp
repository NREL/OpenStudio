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

#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_Program_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemProgram(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Program) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Program");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_ProgramFields::Name);
  if(!s){
    LOG(Error, "WorkspaceObject EnergyManagementSystem_Program has no Name");
    return boost::none;
  }

  //make sure all other objects are translated first
  for (const WorkspaceObject& workspaceObject : m_workspace.objects()) {
    if ((workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Program) 
      && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine)
      && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ProgramCallingManager)
      && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_MeteredOutputVariable)
      && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_OutputVariable)) {
      translateAndMapWorkspaceObject(workspaceObject);
    }
  }

  openstudio::model::EnergyManagementSystemProgram emsProgram(m_model);
  emsProgram.setName(*s);

  unsigned n = workspaceObject.numExtensibleGroups();
  OptionalString line;
  for (unsigned i = 0; i < n; ++i) {
    line = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getString(EnergyManagementSystem_ProgramExtensibleFields::ProgramLine);
    if (line) {
      emsProgram.addLine(line.get());
    }
  }
  return emsProgram;
}

} // energyplus

} // openstudio

