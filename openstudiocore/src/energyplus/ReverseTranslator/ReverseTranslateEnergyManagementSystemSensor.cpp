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

#include "../../model/EnergyManagementSystemSensor.hpp"
#include "../../model/EnergyManagementSystemSensor_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemSensor(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Sensor) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Sensor");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_SensorFields::Name);
  if(!s){
    return boost::none;
  }

  openstudio::model::EnergyManagementSystemSensor emsSensor(m_model);

  if (s) {
    emsSensor.setName(*s);
  }

  Workspace workspace = workspaceObject.workspace();

  s = workspaceObject.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName);
  if (s) {
    for (const WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::Output_Variable)) {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
      if (modelObject) {
        boost::optional<OutputVariable> outvar = modelObject->optionalCast<OutputVariable>();
        if (outvar) {
          if (outvar.get().variableName() == s) {
            emsSensor.setOutputVariable(outvar.get());
            break;
          }
        }
      }
    }
    for (const WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::Output_Meter)) {
      boost::optional<openstudio::model::OutputMeter> outvar = wsObject.optionalCast<openstudio::model::OutputMeter>();
      if (outvar) {
        if (outvar.get().name() == s) {
          emsSensor.setOutputMeter(wsObject.cast<openstudio::model::OutputMeter>());
          break;
        }
      }
    }
  }

  s = workspaceObject.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName);
  if(s){
    emsSensor.setKeyName(*s);
  }

  return emsSensor;
}

} // energyplus

} // openstudio

