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

#include "../../model/EnergyManagementSystemInternalVariable.hpp"
#include "../../model/EnergyManagementSystemInternalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_InternalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemInternalVariable(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_InternalVariable) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_InternalVariable");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::Name);
  if(!s){
    LOG(Error, "WorkspaceObject EnergyManagementSystem_InternalVariable has no Name.");
    return boost::none;
  }

  openstudio::model::EnergyManagementSystemInternalVariable emsInternalVariable(m_model);
  emsInternalVariable.setName(*s);

  s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType);
  if (s) {
    emsInternalVariable.setInternalDataType(*s);
  }
  s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName);
  if (s) {
    emsInternalVariable.setInternalDataIndexKeyName(*s);
  }

  return emsInternalVariable;
}

} // energyplus

} // openstudio

