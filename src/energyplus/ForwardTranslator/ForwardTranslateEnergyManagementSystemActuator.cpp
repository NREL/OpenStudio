/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
#include "../../model/SpaceLoad.hpp"
#include "../../model/SpaceLoad_Impl.hpp"
#include "../../model/SpaceLoadInstance.hpp"
#include "../../model/SpaceLoadInstance_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemActuator(EnergyManagementSystemActuator & modelObject) {
  boost::optional<std::string> s;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator);

  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
  const boost::optional<ModelObject> m_opt = modelObject.actuatedComponent();
  if (m_opt) {
    ModelObject m = m_opt.get();

    // Start by dealing with "OS:Site" and "OS:WeatherFile" that are special ones (will not pass the test on `m.name()` below
    if (m.iddObjectType() == openstudio::IddObjectType::OS_Site || m.iddObjectType() == openstudio::IddObjectType::OS_WeatherFile) {
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, "Environment");
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, "Weather Data");
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      m_idfObjects.push_back(idfObject);
      return idfObject;
    }

    // The component should have a name
    if (!m.name()) {
      LOG(Error, "Actuated Component Name for Actuator '" << modelObject.nameString() << "' does not exist, it will not be translated.");
      return boost::none;
    }

    // check if actuatedComponent is a SpaceLoad
    if (auto load = m.optionalCast<model::SpaceLoadInstance>()) {
      const boost::optional<ModelObject> m_zN = modelObject.zoneName();
      // if SpaceLoad, check if thermalzone names exist
      auto space = load->space();
      auto spaceType = load->spaceType();
      if (spaceType) {
        if (m_zN){
          //ZoneName object exists so set name to be the concatonation of it plus the spaceloadobject
          std::string tz_name = m_zN.get().nameString() + " " + m.nameString();
          idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, tz_name);
          idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
          idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());

          m_idfObjects.push_back(idfObject);
          return idfObject;
        }
        //ZoneName object does not exist so try and translate anyway

        // should not also have a space assigned
        OS_ASSERT(!space);

        // the load references a space type, this means it will reference a zonelist in EnergyPlus
        // in EnergyPlus, each load in a zonelist is duplicated into each zone with a new name based on the zone
        boost::optional<IdfObject> result;
        for (const auto& space : spaceType->spaces()){
          auto tz = space.thermalZone();
          if (tz) {
            std::string tz_name = tz.get().nameString() + " " + m.nameString();
            idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, tz_name);
            idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
            idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
            if (!result){
              result = idfObject;
            }
          } else {
            LOG(Error, "Actuator '" << modelObject.nameString()  << "' references a SpaceLoad '"
                        << load.get().name().get() << "' which is not associated with a ThermalZone, it will not be translated.");
          }
        }
        //Give WArning that spaceType has multiple spaces
        if (spaceType->spaces().size() > 1) {
          LOG(Warn, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load.get().name().get() << "' attached to SpaceType '" << spaceType.get().nameString() << "', with multiple spaces. Check your EMS programs to make sure your actuators are correct.");
        }
        // return the first idf object
        if (result) {
          m_idfObjects.push_back(result.get());
        }
        return result;

      } else if (space) {

        // if load is assigned to a single space instead of a space type then the name will not be changed in EnergyPlus
        idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, m.nameString());
        idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
        idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
        m_idfObjects.push_back(idfObject);

        return idfObject;
      } else {
        LOG(Error, "Actuator '" << modelObject.nameString()  << "' references a SpaceLoad '"
                   << load.get().name().get() << "' which is not associated with a ThermalZone, it will not be translated.");
        return boost::none;
      }

    // Classic case, we just write it
    } else {
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, m.nameString());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      m_idfObjects.push_back(idfObject);
      return idfObject;
    }
  } else {
    LOG(Error, "Actuated Component for Actuator " << modelObject.nameString() << " does not exist, it will not be translated.");
    return boost::none;
  }
}

} // energyplus

} // openstudio

