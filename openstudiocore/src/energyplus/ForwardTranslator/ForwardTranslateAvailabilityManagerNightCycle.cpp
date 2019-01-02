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
#include "../../model/AvailabilityManagerNightCycle.hpp"
#include "../../model/AvailabilityManagerNightCycle_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerNightCycle(
    AvailabilityManagerNightCycle & modelObject)
{
  IdfObject idfObject(IddObjectType::AvailabilityManager_NightCycle);
  m_idfObjects.push_back(idfObject);

  boost::optional<AirLoopHVAC> airLoopHVAC;
  if( auto loop = modelObject.loop() ) {
    airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
  }

  {
    auto schedule = modelObject.model().alwaysOnDiscreteSchedule();
    idfObject.setString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName,schedule.name().get());
  }

  if( airLoopHVAC ) {
    // Fan schedules are set to match the availabilitySchedule in the translator
    idfObject.setString(AvailabilityManager_NightCycleFields::FanScheduleName,airLoopHVAC->availabilitySchedule().name().get());
  }

  // TODO: @kbenne, we don't even translate the AVM:NightCycle if it's not on an airloop anyways
  // Translation is triggered from the AirLoopHVAC itself

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }


  {
    auto value = modelObject.thermostatTolerance();
    idfObject.setDouble(AvailabilityManager_NightCycleFields::ThermostatTolerance,value);
  }


  // Cycling Run Time and Cycling Run Time Control Type
  {
    double runtime = modelObject.cyclingRunTime();
    idfObject.setDouble(AvailabilityManager_NightCycleFields::CyclingRunTime, runtime);
    std::string cycRTCType = modelObject.cyclingRunTimeControlType();

    if (istringEqual(cycRTCType, "Thermostat")) {
      LOG(Info, "With a Cycling Run Time Control Type set to '" << cycRTCType
          << "', the entered Cycling Run Time of '" << runtime << "' seconds will be ignored for "
          << modelObject.briefDescription());
    }
    idfObject.setString(AvailabilityManager_NightCycleFields::CyclingRunTimeControlType, cycRTCType);
  }


  // We'll use that later to default the control zone lists
  std::string controlType = modelObject.controlType();
  idfObject.setString(AvailabilityManager_NightCycleFields::ControlType, controlType);

  // Zone Lists
  std::vector<ThermalZone> controlThermalZones = modelObject.controlThermalZones();
  std::vector<ThermalZone> coolingControlThermalZones = modelObject.coolingControlThermalZones();
  std::vector<ThermalZone> heatingControlThermalZones = modelObject.heatingControlThermalZones();
  std::vector<ThermalZone> heatingZoneFansOnlyThermalZones = modelObject.heatingZoneFansOnlyThermalZones();

  // Size of Zone Lists for convenience
  auto n_controlThermalZones = controlThermalZones.size();
  auto n_coolingControlThermalZones = coolingControlThermalZones.size();
  auto n_heatingControlThermalZones = heatingControlThermalZones.size();
  auto n_heatingZoneFansOnlyThermalZones = heatingZoneFansOnlyThermalZones.size();

  // Boolean to decide whether defaulting a given ZoneList to all zones served by system is needed or not
  bool default_controlThermalZones = false;
  bool default_coolingControlThermalZones = false;
  bool default_heatingControlThermalZones = false;
  bool default_heatingZoneFansOnlyThermalZones = false;

  // Main logic to warn user and decide if defaulting is needed based on the Control Type entered
  if ( istringEqual(controlType, "StayOff") ||
       istringEqual(controlType, "CycleOnAny") ||
       istringEqual(controlType, "CycleOnAnyZoneFansOnly") ) {

    // All Control Zone Lists are ignored
    if ((  n_controlThermalZones
         + n_coolingControlThermalZones
         + n_heatingControlThermalZones
         + n_heatingZoneFansOnlyThermalZones) > 0 ) {
      LOG(Info, "All Control Zone Lists will be ignored for " << modelObject.briefDescription()
            << " due to the Control Type of '" << controlType << "'.");
    }

  } else if ( istringEqual(controlType, "CycleOnControlZone") ) {

    // Only the controlThermalZones isn't ignored
    if ((  n_coolingControlThermalZones
         + n_heatingControlThermalZones
         + n_heatingZoneFansOnlyThermalZones) > 0 ) {
      LOG(Info, "All Control Zone Lists other than 'Control Zone List' will be ignored for " << modelObject.briefDescription()
            << " due to the Control Type of '" << controlType << "'.");
    }

    // Check if need to default the controlThermalZones
    // Another option might have been to switch the control Type to CycleOnAny
    if (n_controlThermalZones == 0) {
      default_controlThermalZones = true;
    }

  } else if ( istringEqual(controlType, "CycleOnAnyCoolingZone") ) {

    // Only the coolingControlThermalZones isn't ignored
    if ((  n_controlThermalZones
         + n_heatingControlThermalZones
         + n_heatingZoneFansOnlyThermalZones) > 0 ) {
      LOG(Info, "All Control Zone Lists  other than 'Cooling Control Zone List' will be ignored for " << modelObject.briefDescription()
            << " due to the Control Type of '" << controlType << "'.");
    }

    // Check if need to default
    if (n_coolingControlThermalZones == 0) {
      default_coolingControlThermalZones = true;
    }


  } else if ( istringEqual(controlType, "CycleOnAnyHeatingZone") ) {

    // Only the heatingControlThermalZones, and heatingZoneFansOnlyThermalZones aren't ignored
    if (( n_controlThermalZones
          + n_coolingControlThermalZones) > 0 ) {
      LOG(Info, "All Control Zone Lists  other than 'Heating Control Zone List' and optionally "
                "'Heating Zone Fans Only Zone List' will be ignored for " << modelObject.briefDescription()
                << " due to the Control Type of '" << controlType << "'.");
    }

    // Check if need to default
    if (n_heatingControlThermalZones == 0) {
      default_heatingControlThermalZones = true;
    }
    // TODO: @kbenne
    // here we should't default the heatingZoneFansOnlyThermalZones
    // I view that as an option to enable cycling on zone fans only or not,
    // while you only get central heating operation (not possible with CycleOnAny)

  } else if ( istringEqual(controlType, "CycleOnAnyHeatingZoneFansOnly") ) {

    // Only the heatingZoneFansOnlyThermalZones isn't ignored
    if ((  n_controlThermalZones
          + n_coolingControlThermalZones
          + n_heatingControlThermalZones) > 0 ) {
      LOG(Info, "All Control Zone Lists  other than 'Heating Zone Fans Only Zone List' will be ignored for "
                << modelObject.briefDescription()
                << " due to the Control Type of '" << controlType << "'.");
    }

    // Check if need to default
    if (n_heatingZoneFansOnlyThermalZones == 0) {
      default_heatingZoneFansOnlyThermalZones = true;
    }


  } else if ( istringEqual(controlType, "CycleOnAnyCoolingOrHeatingZone") ) {

    // Only the coolingControlThermalZones, coolingControlThermalZones, and optionally heatingZoneFansOnlyThermalZones aren't ignored
      if ( n_controlThermalZones > 0 ) {
      LOG(Info, "All Control Zone Lists  other than 'Heating Control Zone List', 'Cooling Control Zone List' and optionally "
                "'Heating Zone Fans Only Zone List' will be ignored for " << modelObject.briefDescription()
                << " due to the Control Type of '" << controlType << "'.");
    }

    // Check if need to default
    if (n_heatingControlThermalZones == 0) {
      default_heatingControlThermalZones = true;
    }
    if (n_coolingControlThermalZones == 0) {
      default_coolingControlThermalZones = true;
    }
    // TODO: @kbenne
    // Here we should default the heatingZoneFansOnlyThermalZones
    // Because otherwise the user could have just chosen "CycleOnAny"
    if (n_heatingZoneFansOnlyThermalZones == 0) {
      default_heatingZoneFansOnlyThermalZones = true;
    }


  } else {
    // should never get there, unless a new ControlType is added later...
    LOG_AND_THROW("Unknown Control Type of '" << controlType << "' for " << modelObject.briefDescription());
  } // End of Main logic to warn user and decide if defaulting is needed based on the Control Type entered


  // Whether a Zone List will actually be unused or not, we still translate it (perhaps the user is going to use the IDF later)

  // Control Thermal Zones
  {
    // Create a ZoneList, and populate it
    IdfObject zoneList(IddObjectType::ZoneList);
    std::string zoneListName = modelObject.name().get() + " Control Zones List";
    zoneList.setName(zoneListName);
    bool write_zonelist;

    if (default_controlThermalZones) {
      // Get all zones served by the AirLoop attached to it
      if( airLoopHVAC ) {
        for( const ThermalZone & tz : airLoopHVAC->thermalZones() ) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
        }
        LOG(Warn, "Defaulting the Control Zone List to all zones served by the AirLoopHVAC attached to "
               << modelObject.briefDescription());
        write_zonelist = true;
      } else {
        // Note: we never get here because the translation of the AVM:NightCyle isn't done if no AirLoopHVAC attached
        LOG(Error, "Control Zone List is expected for " << modelObject.briefDescription()
               << " but it cannot be defaulted because it isn't on an AirLoopHVAC");
        write_zonelist = false;
      }
    } else {
      if (n_controlThermalZones > 0) {
        if (n_controlThermalZones == 1) {
          // If only one, just write the thermalZone name directly
          write_zonelist = false;
          idfObject.setString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName,
                              controlThermalZones[0].name().get());
        } else {
          // More than one, we indeed create a zone list
          for (const ThermalZone& tz: controlThermalZones) {
            auto eg = zoneList.pushExtensibleGroup();
            eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
          }
          write_zonelist = true;
        }
      } else {
        // No zones = no zonelist
        write_zonelist = false;
      }
    }
    // Write ZoneList to the IDF and set the AVM ZoneList field to it only if something meaningful happened
    if (write_zonelist) {
      idfObject.setString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName, zoneListName);
      m_idfObjects.push_back(zoneList);
    }
  }


  // Cooling Control Thermal Zones
  {
    // Create a ZoneList, and populate it
    IdfObject zoneList(IddObjectType::ZoneList);
    std::string zoneListName = modelObject.name().get() + " Cooling Control Zones List";
    zoneList.setName(zoneListName);
    bool write_zonelist;

    if (default_coolingControlThermalZones) {
      // Get all zones served by the AirLoop attached to it
      if( airLoopHVAC ) {
        for( const ThermalZone & tz : airLoopHVAC->thermalZones() ) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
        }
        LOG(Warn, "Defaulting the Cooling Control Zone List to all zones served by the AirLoopHVAC attached to "
               << modelObject.briefDescription());
        write_zonelist = true;
      } else {
        LOG(Error, "Cooling Control Zone List is expected for " << modelObject.briefDescription()
               << " but it cannot be defaulted because it isn't on an AirLoopHVAC");
        write_zonelist = false;
      }
    } else {
      if (n_coolingControlThermalZones > 0) {
        if (n_coolingControlThermalZones == 1) {
          // If only one, just write the thermalZone name directly
          write_zonelist = false;
          idfObject.setString(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName,
                              coolingControlThermalZones[0].name().get());
        } else {
          // More than one, we indeed create a zone list
          for (const ThermalZone& tz: coolingControlThermalZones) {
            auto eg = zoneList.pushExtensibleGroup();
            eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
          }
          write_zonelist = true;
        }
      } else {
        // No zones = no zonelist
        write_zonelist = false;
      }
    }
    // Write ZoneList to the IDF and set the AVM ZoneList field to it only if something meaningful happened
    if (write_zonelist) {
      idfObject.setString(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName, zoneListName);
      m_idfObjects.push_back(zoneList);
    }
  }


  // Heating Control Thermal Zones
  {
    // Create a ZoneList, and populate it
    IdfObject zoneList(IddObjectType::ZoneList);
    std::string zoneListName = modelObject.name().get() + " Heating Control Zones List";
    zoneList.setName(zoneListName);
    bool write_zonelist;

    if (default_heatingControlThermalZones) {
      // Get all zones served by the AirLoop attached to it
      if( airLoopHVAC ) {
        for( const ThermalZone & tz : airLoopHVAC->thermalZones() ) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
        }
        LOG(Warn, "Defaulting the Heating Control Zone List to all zones served by the AirLoopHVAC attached to "
               << modelObject.briefDescription());
        write_zonelist = true;
      } else {
        LOG(Error, "Cooling Heating Zone List is expected for " << modelObject.briefDescription()
               << " but it cannot be defaulted because it isn't on an AirLoopHVAC");
        write_zonelist = false;
      }
    } else {
      if (n_heatingControlThermalZones > 0) {
        if (n_heatingControlThermalZones == 1) {
          // If only one, just write the thermalZone name directly
          write_zonelist = false;
          idfObject.setString(AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName,
                              heatingControlThermalZones[0].name().get());
        } else {
          // More than one, we indeed create a zone list

          for (const ThermalZone& tz: heatingControlThermalZones) {
            auto eg = zoneList.pushExtensibleGroup();
            eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
          }
          write_zonelist = true;
        }
      } else {
        // No zones = no zonelist
        write_zonelist = false;
      }
    }
    // Write ZoneList to the IDF and set the AVM ZoneList field to it only if something meaningful happened
    if (write_zonelist) {
      idfObject.setString(AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName, zoneListName);
      m_idfObjects.push_back(zoneList);
    }
  }

  // Heating Zone Fans Only Thermal Zones
  {
    // Create a ZoneList, and populate it
    IdfObject zoneList(IddObjectType::ZoneList);
    std::string zoneListName = modelObject.name().get() + " Heating Zone Fans Only Zones List";
    zoneList.setName(zoneListName);
    bool write_zonelist;

    if (default_heatingZoneFansOnlyThermalZones) {
      // Get all zones served by the AirLoop attached to it
      if( airLoopHVAC ) {
        for( const ThermalZone & tz : airLoopHVAC->thermalZones() ) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
        }
        LOG(Warn, "Defaulting the Heating Zone Fans Only Zones List to all zones served by the AirLoopHVAC attached to "
               << modelObject.briefDescription());
        write_zonelist = true;
      } else {
        LOG(Error, "Heating Zone Fans Only Zones List is expected for " << modelObject.briefDescription()
               << " but it cannot be defaulted because it isn't on an AirLoopHVAC");
        write_zonelist = false;
      }
    } else {
      if (n_heatingZoneFansOnlyThermalZones > 0) {
        if (n_heatingZoneFansOnlyThermalZones == 1) {
          // If only one, just write the thermalZone name directly
          write_zonelist = false;
          idfObject.setString(AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName,
                              heatingZoneFansOnlyThermalZones[0].name().get());
        } else {
          // More than one, we indeed create a zone list
          for (const ThermalZone& tz: heatingZoneFansOnlyThermalZones) {
            auto eg = zoneList.pushExtensibleGroup();
            eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
          }
          write_zonelist = true;
        }
      } else {
        // No zones = no zonelist
        write_zonelist = false;
      }
    }
    // Write ZoneList to the IDF and set the AVM ZoneList field to it only if something meaningful happened
    if (write_zonelist) {
      idfObject.setString(AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName, zoneListName);
      m_idfObjects.push_back(zoneList);
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
