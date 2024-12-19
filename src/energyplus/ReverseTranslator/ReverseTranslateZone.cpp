/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZone(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Zone) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Zone");
      return boost::none;
    }

    // this function creates a space and a thermal zone, it returns the space.  If you want the
    // thermal zone you can reliably dereference the result of space.thermalZone().

    openstudio::model::ThermalZone thermalZone(m_model);

    openstudio::model::Space space(m_model);
    space.setThermalZone(thermalZone);

    boost::optional<std::string> _idfZoneName;

    OptionalString s = workspaceObject.name();
    if (s) {
      thermalZone.setName(*s);
      space.setName(*s + " Space");
      _idfZoneName = *s;
    }

    OptionalDouble d = workspaceObject.getDouble(ZoneFields::DirectionofRelativeNorth);
    if (d) {
      space.setDirectionofRelativeNorth(*d);
    }

    d = workspaceObject.getDouble(ZoneFields::XOrigin);
    if (d) {
      space.setXOrigin(*d);
    }

    d = workspaceObject.getDouble(ZoneFields::YOrigin);
    if (d) {
      space.setYOrigin(*d);
    }

    d = workspaceObject.getDouble(ZoneFields::ZOrigin);
    if (d) {
      space.setZOrigin(*d);
    }

    OptionalInt i = workspaceObject.getInt(ZoneFields::Type);
    if (i) {
      // no-op
    }

    i = workspaceObject.getInt(ZoneFields::Multiplier);
    if (i) {
      thermalZone.setMultiplier(*i);
    }

    d = workspaceObject.getDouble(ZoneFields::CeilingHeight);
    if (d) {
      thermalZone.setCeilingHeight(*d);
    }

    d = workspaceObject.getDouble(ZoneFields::Volume);
    if (d) {
      thermalZone.setVolume(*d);
    }

    s = workspaceObject.getString(ZoneFields::ZoneInsideConvectionAlgorithm);
    if (s) {
      thermalZone.setZoneInsideConvectionAlgorithm(*s);
    }

    s = workspaceObject.getString(ZoneFields::ZoneOutsideConvectionAlgorithm);
    if (s) {
      thermalZone.setZoneOutsideConvectionAlgorithm(*s);
    }

    s = workspaceObject.getString(ZoneFields::PartofTotalFloorArea);
    if (s) {
      if (istringEqual("Yes", *s)) {
        space.setPartofTotalFloorArea(true);
      } else {
        space.setPartofTotalFloorArea(false);
      }
    }

    // Thermostat

    // If the zone in the idf file does not have a name, there is no use in even trying to find a thermostat
    if (_idfZoneName) {
      Workspace workspace = workspaceObject.workspace();

      // Loop on all ZoneControl:Thermostat objects, trying to find the one
      std::vector<WorkspaceObject> _zoneControlThermostats;
      _zoneControlThermostats = workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat);
      for (const auto& _zoneControlThermostat : _zoneControlThermostats) {
        if (boost::optional<std::string> zoneName = _zoneControlThermostat.getString(ZoneControl_ThermostatFields::ZoneorZoneListName)) {
          bool zoneControlThermostatfound = false;

          // If it references our zone, then good to go!
          if (openstudio::istringEqual(zoneName.get(), _idfZoneName.get())) {
            zoneControlThermostatfound = true;
          }
          // Otherwise, it might be in a ZoneList
          else if (boost::optional<WorkspaceObject> _zoneList = workspace.getObjectByTypeAndName(IddObjectType::ZoneList, zoneName.get())) {
            // Loop on all entries (=Zone Names) in that ZoneList
            std::vector<IdfExtensibleGroup> zoneListGroup = _zoneList->extensibleGroups();
            for (const auto& zoneListElem : zoneListGroup) {
              boost::optional<std::string> zoneListZoneName = zoneListElem.getString(ZoneListExtensibleFields::ZoneName);
              if (zoneListZoneName) {
                if (openstudio::istringEqual(zoneListZoneName.get(), _idfZoneName.get())) {
                  // If that's the one, no need to keep processing other Zone Names
                  zoneControlThermostatfound = true;
                  break;
                }
              }
            }
          }
          if (zoneControlThermostatfound) {

            // We only support ThermostatSetpoint:DualSetpoint

            // TODO: JM 2019-09-04 switch back to an extensible object once/if https://github.com/NREL/EnergyPlus/issues/7484 is addressed and the
            // 'Temperature Difference Between Cutout And Setpoint' field is moved before the extensible fields
            // For now, we revert to a non extensible object, so we can still write that field

            /*
 *          std::vector<IdfExtensibleGroup> extensibleGroups = _zoneControlThermostat.extensibleGroups();
 *          for( const auto & extensibleGroup : extensibleGroups )
 *          {
 *            boost::optional<std::string> thermostatType = extensibleGroup.getString(ZoneControl_ThermostatExtensibleFields::ControlObjectType);
 *            boost::optional<std::string> thermostatName = extensibleGroup.getString(ZoneControl_ThermostatExtensibleFields::ControlName);
 *
 *            if( thermostatName && thermostatType )
 *            {
 *              boost::optional<WorkspaceObject> _thermostat
 *               = workspace.getObjectByTypeAndName(IddObjectType(thermostatType.get()),thermostatName.get());
 *
 *              if( _thermostat )
 *              {
 *                boost::optional<ModelObject> thermostat = translateAndMapWorkspaceObject(_thermostat.get());
 *                if( thermostat )
 *                {
 *                  if( boost::optional<ThermostatSetpointDualSetpoint> thermostatSetpointDualSetpoint
 *                      = thermostat->optionalCast<ThermostatSetpointDualSetpoint>() )
 *                  {
 *                    thermalZone.setThermostatSetpointDualSetpoint(thermostatSetpointDualSetpoint.get());
 *                  }
 *                }
 *              }
 *            }
 *          }
 */

            auto checkIfDualSetpointAndTranslate = [&](boost::optional<std::string>& _thermostatType, boost::optional<std::string>& _thermostatName) {
              if (_thermostatName && _thermostatType) {
                boost::optional<WorkspaceObject> _i_thermostat =
                  workspace.getObjectByTypeAndName(IddObjectType(_thermostatType.get()), _thermostatName.get());
                if (_i_thermostat) {
                  boost::optional<ModelObject> _m_thermostat = translateAndMapWorkspaceObject(_i_thermostat.get());
                  if (_m_thermostat) {
                    if (boost::optional<ThermostatSetpointDualSetpoint> _thermostatSetpointDualSetpoint =
                          _m_thermostat->optionalCast<ThermostatSetpointDualSetpoint>()) {
                      thermalZone.setThermostatSetpointDualSetpoint(_thermostatSetpointDualSetpoint.get());

                      // Reverse Translate 'Temperature Difference Between Cutout And Setpoint' from ZoneControl:Thermostat
                      // by placing it on the ThermostatSetpoint:DualSetpoint object
                      if (boost::optional<double> _cutOutTemp =
                            _zoneControlThermostat.getDouble(ZoneControl_ThermostatFields::TemperatureDifferenceBetweenCutoutAndSetpoint)) {
                        _thermostatSetpointDualSetpoint->setTemperatureDifferenceBetweenCutoutAndSetpoint(_cutOutTemp.get());
                      }
                    }
                  }
                }
              }
            };
            // Group 1
            {
              boost::optional<std::string> thermostatType =
                _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control1ObjectType, false, true);
              boost::optional<std::string> thermostatName = _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control1Name, false, true);
              checkIfDualSetpointAndTranslate(thermostatType, thermostatName);
            }

            // Group 2
            {
              boost::optional<std::string> thermostatType =
                _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control2ObjectType, false, true);
              boost::optional<std::string> thermostatName = _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control2Name, false, true);
              checkIfDualSetpointAndTranslate(thermostatType, thermostatName);
            }

            // Group 3
            {
              boost::optional<std::string> thermostatType =
                _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control3ObjectType, false, true);
              boost::optional<std::string> thermostatName = _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control3Name, false, true);
              checkIfDualSetpointAndTranslate(thermostatType, thermostatName);
            }

            // Group 4
            {
              boost::optional<std::string> thermostatType =
                _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control4ObjectType, false, true);
              boost::optional<std::string> thermostatName = _zoneControlThermostat.getString(ZoneControl_ThermostatFields::Control4Name, false, true);
              checkIfDualSetpointAndTranslate(thermostatType, thermostatName);
            }

            break;
          }  // End if zoneControlThermostatFound
        }  // End if ZoneorZoneListName exists on ZoneControl:Thermostat object
      }  // End Loop on each ZoneControlThermostats
    }  // End if _idfZoneName

    // Zone Equipment
    /*
  if( _idfZoneName )
  {
    std::vector<WorkspaceObject> zoneHVACEquipmentConnections;
    zoneHVACEquipmentConnections = workspaceObject.workspace().getObjectsByType(IddObjectType::ZoneHVAC_EquipmentConnections);

    for( std::vector<WorkspaceObject>::iterator it = zoneHVACEquipmentConnections.begin();
         it != zoneHVACEquipmentConnections.end();
         it++ )
    {
      s = it->getString(ZoneHVAC_EquipmentConnectionsFields::ZoneName);

      if( s && istringEqual(s.get(),_idfZoneName.get()) )
      {
        boost::optional<WorkspaceObject> _zoneEquipmentList = it->getTarget(ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName);

        if( _zoneEquipmentList )
        {
          translateAndMapWorkspaceObject(_zoneEquipmentList.get());
        }

        break;
      }
    }
  }
*/

    return space;
  }

}  // namespace energyplus

}  // namespace openstudio
