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
#include "../../utilities/core/StringHelpers.hpp"

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
    LOG(Error, "WorkspaceObject EnergyManagementSystem_Subroutine has no Name");
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

  openstudio::model::EnergyManagementSystemSubroutine emsProgram(m_model);
  emsProgram.setName(*s);

  //get all model objects so we can do name / uid substitution
  const std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<model::ModelObject>();
  size_t pos, len;
  std::string newline, uid;

  unsigned n = workspaceObject.numExtensibleGroups();
  OptionalString line;
  for (unsigned i = 0; i < n; ++i) {
    line = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getString(EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
    if (line) {
      newline = line.get();
      //split line on whitespaces to get look for modelobject names
      std::vector<std::string> results = splitString(line.get(), ' ');
      for (size_t j = 0; j < results.size(); j++) {
        for (size_t k = 0; k < modelObjects.size(); k++) {
          if (modelObjects.at(k).name()) {
            //check if program item is the name of a model object
            if (modelObjects.at(k).name().get() == results.at(j)) {
              pos = newline.find(results.at(j));
              len = results.at(j).length();
              uid = toString(modelObjects.at(k).handle());
              //replace modelobject name with handle
              newline.replace(pos, len, uid);
            }
          }
        }
      }
      emsProgram.addLine(newline);
    }
  }
  return emsProgram;
}

} // energyplus

} // openstudio

