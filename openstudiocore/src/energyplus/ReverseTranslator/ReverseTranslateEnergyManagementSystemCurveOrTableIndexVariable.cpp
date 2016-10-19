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

#include "../../model/EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "../../model/EnergyManagementSystemCurveOrTableIndexVariable_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_CurveOrTableIndexVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemCurveOrTableIndexVariable(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_CurveOrTableIndexVariable");
    return boost::none;
  }

  OptionalString s1 = workspaceObject.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::Name);
  if(!s1){
    LOG(Error, "WorkspaceObject EnergyManagementSystem_CurveOrTableIndexVariable has no name");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName);
  if (!s) {
    LOG(Error, workspaceObject.nameString() + ": has no CurveorTableObjectName");
    return boost::none;
  }

  Workspace workspace = workspaceObject.workspace();

  if (s) {
    //for ( WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
    std::vector<WorkspaceObject> wsObjects = workspace.getObjectsByName(*s);
    if (wsObjects.size() > 1) {
      LOG(Error, workspaceObject.nameString() + ": CurveorTableObject is not unique.  More than 1 object with that name.");
      return boost::none;
    }
    if (wsObjects.size() == 0) {
      LOG(Error, workspaceObject.nameString() + ": CurveorTableObject not found.");
      return boost::none;
    } else {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObjects[0]);
      if (modelObject) {
        openstudio::model::EnergyManagementSystemCurveOrTableIndexVariable emsCurveOrTableIndexVariable(m_model);
        emsCurveOrTableIndexVariable.setName(*s1);
        boost::optional<model::Curve> curve = modelObject.get().optionalCast<model::Curve>();
        if (curve) {
          emsCurveOrTableIndexVariable.setCurveOrTableObject(curve.get());
          return emsCurveOrTableIndexVariable;
        }
        return boost::none;
      }
    }
  }
  return boost::none;
}

} // energyplus

} // openstudio

