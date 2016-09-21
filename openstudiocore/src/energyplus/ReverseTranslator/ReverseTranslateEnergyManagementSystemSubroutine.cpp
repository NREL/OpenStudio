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

#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemSubroutine(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Subroutine");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_SubroutineFields::Name);
  if (!s) {
    return boost::none;
  }

  openstudio::model::EnergyManagementSystemSubroutine emsProgram(m_model);

  if (s) {
    emsProgram.setName(*s);
  }

  //for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
  //emsProgram.pushExtensibleGroup(eg.fields());
  //}
  unsigned n = workspaceObject.numExtensibleGroups();
  //OptionalWorkspaceObject line;
  OptionalString line;
  for (unsigned i = 0; i < n; ++i) {
    line = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getString(EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
    if (line) {
      emsProgram.addLine(line.get());
    }
  }
  return emsProgram;
}

} // energyplus

} // openstudio

