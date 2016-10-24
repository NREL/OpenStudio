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

#include "../../model/EnergyManagementSystemTrendVariable.hpp"
#include "../../model/EnergyManagementSystemTrendVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_TrendVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemTrendVariable(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_TrendVariable) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_TrendVariable");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_TrendVariableFields::Name);
  if(!s){
    LOG(Error, "WorkspaceObject EnergyManagementSystem_TrendVariable has no Name");
    return boost::none;
  }

  openstudio::model::EnergyManagementSystemTrendVariable emsTrendVariable(m_model);
  emsTrendVariable.setName(*s);

  boost::optional<int> i = workspaceObject.getInt(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged);
  if (!i) {
    LOG(Error, emsTrendVariable.nameString() + ": NumberofTimestepstobeLogged not set");
    return boost::none;
  } else {
    emsTrendVariable.setNumberOfTimestepsToBeLogged(*i);
  }

  s = workspaceObject.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName);
  if (!s) {
    LOG(Error, emsTrendVariable.nameString() + ": EMSVariableName not set");
    return boost::none;
  } else {
    bool result = false;
    Workspace workspace = workspaceObject.workspace();
    //look for GlobalVariables, translate and check if there is a name match since GV's dont have name field.
    for (WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)) {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
      if (modelObject) {
        if (modelObject.get().cast<EnergyManagementSystemGlobalVariable>().name() == s) {
          result = emsTrendVariable.setEMSVariableName(*s);          
          if (result) {
            return emsTrendVariable;
          } else {
            return boost::none;
          }
        }
      }
    }
    //look for name match on other (EMS) objects.
    for (WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
      if (modelObject) {
        result = emsTrendVariable.setEMSVariableName(*s);
        if (result) {
          return emsTrendVariable;
        } else {
          return boost::none;
        }
      }
    }
  }

  return emsTrendVariable;
}

} // energyplus

} // openstudio

