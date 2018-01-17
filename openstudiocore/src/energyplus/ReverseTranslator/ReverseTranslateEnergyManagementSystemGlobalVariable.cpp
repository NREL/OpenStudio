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

#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemGlobalVariable(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_GlobalVariable) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_GlobalVariable");
    return boost::none;
  }

  if (workspaceObject.numExtensibleGroups() > 1) {
    LOG(Warn, "EnergyManagementSystem GlobalVariable has more than 1 value.");
  }

  OptionalModelObject result;
  for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
    boost::optional<std::string> value = eg.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName);
    if (value) {
      openstudio::model::EnergyManagementSystemGlobalVariable emsGlobalVariable(m_model, value.get());
      if (!result) { 
        result = emsGlobalVariable;
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio

