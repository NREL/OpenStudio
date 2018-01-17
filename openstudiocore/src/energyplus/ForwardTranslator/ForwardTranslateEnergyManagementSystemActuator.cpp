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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/EnergyManagementSystemActuator_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/WeatherFile.hpp"

#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemActuator(EnergyManagementSystemActuator & modelObject)
{
  boost::optional<std::string> s;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator);
  m_idfObjects.push_back(idfObject);
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
  const ModelObject m = modelObject.actuatedComponent();
  if (m.iddObjectType() == openstudio::IddObjectType::OS_Site || m.iddObjectType() == openstudio::IddObjectType::OS_WeatherFile) {
    idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, "Environment");
    idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, "Weather Data");
  }
  else {
    idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, m.nameString());
    idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
    
  }
  
  idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
  
  return idfObject;
}

} // energyplus

} // openstudio

