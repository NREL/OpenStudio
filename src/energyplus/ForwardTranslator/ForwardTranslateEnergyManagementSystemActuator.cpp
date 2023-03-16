/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemActuator(EnergyManagementSystemActuator& modelObject) {

    const boost::optional<ModelObject> actuatedComponent_ = modelObject.actuatedComponent();
    if (!actuatedComponent_) {
      LOG(Error, "Actuated Component for Actuator " << modelObject.nameString() << " does not exist, it will not be translated.");
      return boost::none;
    }

    // Start by dealing with "OS:Site" and "OS:WeatherFile" that are special ones (will not pass the test on `m.name()` below
    if (actuatedComponent_->iddObjectType() == openstudio::IddObjectType::OS_Site
        || actuatedComponent_->iddObjectType() == openstudio::IddObjectType::OS_WeatherFile) {
      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, "Environment");
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, "Weather Data");
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    // As this point, it MUST have a name, as we dealt with the special ones already
    if (!actuatedComponent_->name()) {
      LOG(Error, "Actuated Component Name for Actuator '" << modelObject.nameString() << "' does not exist, it will not be translated.");
      return boost::none;
    }

    // check if actuatedComponent is a SpaceLoad
    auto load_ = actuatedComponent_->optionalCast<model::SpaceLoadInstance>();
    if (!load_) {
      // Classic case, we just write it
      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, actuatedComponent_->nameString());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    // SpaceLoadInstance: check if first if it's assigned to a single space
    auto space_ = load_->space();
    if (space_) {
      // if load is assigned to a single space instead of a space type then the name will not be changed in EnergyPlus
      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, actuatedComponent_->nameString());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    // At this point the SpaceLoadInstance is not assigned to a space, so it's assigned to a SpaceType or nothing

    auto spaceType_ = load_->spaceType();
    if (!spaceType_) {
      LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                              << "' which is not associated with a ThermalZone, it will not be translated.");
      return boost::none;
    }

    // Deal with the SpaceType case...

    // Case 1: the user correctly assigned a Space or ThermalZone to it
    if (const boost::optional<Space> specificSpace_ = modelObject.space()) {
      // Assert the space has a thermalZone attached to it.
      auto tz_ = specificSpace_->thermalZone();
      if (!tz_) {
        LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                << "' and specified that it wanted to be attached to the Space '" << specificSpace_->nameString()
                                << "' but the Space has no Thermal Zone, so it will not be translated.");
        return boost::none;
      }

      std::string zoneOrSpaceName = specificSpace_->nameString();

      // Try to be helpful in case the user assigned a Space, but the FT doesn't translate spaces
      if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        LOG(Warn, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                               << "' and specified that it wanted to be attached to the Space '" << specificSpace_->nameString()
                               << "' but you have turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone '"
                               << tz_->nameString() << "'.");
        zoneOrSpaceName = tz_->nameString();
      }

      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      const std::string spaceLoadNameForEMS = zoneOrSpaceName + " " + actuatedComponent_->nameString();
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, spaceLoadNameForEMS);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    if (const boost::optional<ThermalZone> specificZone_ = modelObject.thermalZone()) {
      auto spaces = specificZone_->spaces();
      if (spaces.empty()) {
        LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                << "' and specified that it wanted to be attached to the ThermalZone '" << specificZone_->nameString()
                                << "' but the Thermal Zone has no Spaces, so it will not be translated.");
        return boost::none;
      }

      std::string zoneOrSpaceName = specificZone_->nameString();

      if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        if (spaces.size() == 1) {
          zoneOrSpaceName = spaces.front().nameString();
          LOG(Warn, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                 << "' and specified that it wanted to be attached to the Thermal Zone '" << specificZone_->nameString()
                                 << "' but you have turned on ForwardTranslation's Space Feature. Falling back to using the attached Space '"
                                 << zoneOrSpaceName << "' since there is only 1.");
        } else {  // if (spaces.size() > 1) {
          std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());
          zoneOrSpaceName = spaces.front().nameString();
          // TODO: do we grab the first space or just don't translate it?
          LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                  << "' and specified that it wanted to be attached to the Thermal Zone '" << specificZone_->nameString()
                                  << "' but you have turned on ForwardTranslation's Space Feature. Your Thermal Zone has multiple spaces "
                                     "attached. Falling back to using the first Space '"
                                  << zoneOrSpaceName << "'.");
        }
      }

      // ZoneName object exists so set name to be the concatenation of it plus the spaceloadobject
      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      const std::string spaceLoadNameForEMS = zoneOrSpaceName + " " + actuatedComponent_->nameString();
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, spaceLoadNameForEMS);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    // ZoneName object does not exist so try and translate anyway

    // the load references a space type, this means it will reference a ZoneList (if excludeSpaceTranslation = true) or a SpaceList in EnergyPlus
    // in EnergyPlus, each load in a ZoneList is duplicated into each zone with a new name based on the zone, same for SpaceList
    {
      auto spaces = spaceType_->spaces();
      if (spaces.empty()) {
        LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                << "' and specified that it wanted to be attached to the SpaceType '" << spaceType_->nameString()
                                << "' but the Space Type has no Spaces, so it will not be translated.");
        return boost::none;
      }

      std::string zoneOrSpaceName;

      if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        if (spaces.size() == 1) {
          zoneOrSpaceName = spaces.front().nameString();
          LOG(Warn, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString() << "' attached to the SpaceType '"
                                 << spaceType_->nameString()
                                 << "' but you have turned on ForwardTranslation's Space Feature. Falling back to using the attached Space '"
                                 << zoneOrSpaceName << "' since there is only 1.");
        } else {  // if (spaces.size() > 1) {
          // TODO: do we grab the first space or just don't translate it?
          std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());
          zoneOrSpaceName = spaces.front().nameString();
          LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                  << "' attached to the SpaceType '" << spaceType_->nameString()
                                  << "' but you have turned on ForwardTranslation's Space Feature. The Space Type has multiple spaces "
                                     "attached. Falling back to using the first Space '"
                                  << zoneOrSpaceName << "'.");
        }
      } else {
        // Not translating Spaces
        std::set<std::string> zoneNames;
        for (const auto& space : spaceType_->spaces()) {
          if (auto tz_ = space.thermalZone()) {
            zoneNames.insert(tz_->nameString());
          }
        }
        if (zoneNames.empty()) {
          LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                  << "' attached to the SpaceType '" << spaceType_->nameString()
                                  << "' but the Space Type has zero spaces attached to any Thermal Zones, so it will not be translated.");
          return boost::none;
        }
        zoneOrSpaceName = *(zoneNames.begin());
        if (zoneNames.size() == 1) {
          LOG(Warn, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString() << "' attached to the SpaceType '"
                                 << spaceType_->nameString()
                                 << "' but you have turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone '"
                                 << zoneOrSpaceName << "' since there is only 1.");
        } else {  // if (zoneNames.size() > 1) {
          // TODO: do we grab the first space or just don't translate it?
          LOG(Error, "Actuator '" << modelObject.nameString() << "' references a SpaceLoad '" << load_->nameString()
                                  << "' attached to the SpaceType '" << spaceType_->nameString()
                                  << "' but you have turned off ForwardTranslation's Space Feature. "
                                     "Your SpaceType ends up linked to multiples Thermal Zones. Falling back to using the first Thermal Zone '"
                                  << zoneOrSpaceName << "'.");
        }
      }

      // ZoneName object exists so set name to be the concatenation of it plus the spaceloadobject
      IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Actuator, modelObject);
      const std::string spaceLoadNameForEMS = zoneOrSpaceName + " " + actuatedComponent_->nameString();
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, spaceLoadNameForEMS);
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, modelObject.actuatedComponentType());
      idfObject.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, modelObject.actuatedComponentControlType());
      return idfObject;
    }

    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
