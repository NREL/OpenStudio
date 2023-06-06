/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneVentilationWindandStackOpenArea.hpp"
#include "../../model/ZoneVentilationWindandStackOpenArea_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneVentilation_WindandStackOpenArea_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneVentilationWindandStackOpenArea(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneVentilation_WindandStackOpenArea) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneVentilation:WindandStackOpenArea");
      return boost::none;
    }

    // Start by checking if the WorkspaceObject has a ThermalZone before you even initialize a ModelObject
    boost::optional<ThermalZone> _zone;
    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::ZoneorSpaceName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Space> _space = modelObject->optionalCast<Space>()) {
          _zone = _space->thermalZone();
        }
      }
    }

    if (!_zone.has_value()) {
      LOG(Error, "Zone not found for workspace object " << workspaceObject.briefDescription() << ", it will not be Reverse Translated.");
      return boost::none;
    }

    openstudio::model::ZoneVentilationWindandStackOpenArea ventilation(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      ventilation.setName(*s);
    }

    ventilation.addToThermalZone(_zone.get());

    OptionalDouble _d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::OpeningArea);
    if (_d) {
      ventilation.setOpeningArea(_d.get());
    } else {
      // This one has a default of zero, but we should care about it (zero means it's useless too)
      LOG(Error, "Opening Area value not found for workspace object " << workspaceObject.briefDescription());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setOpeningAreaFractionSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness))) {
      ventilation.setOpeningEffectiveness(_d.get());
    } else {
      ventilation.autocalculateOpeningEffectiveness();
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle))) {
      ventilation.setEffectiveAngle(_d.get());
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::HeightDifference))) {
      ventilation.setHeightDifference(_d.get());
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening))) {
      ventilation.setDischargeCoefficientforOpening(_d.get());
    } else {
      ventilation.autocalculateDischargeCoefficientforOpening();
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature))) {
      ventilation.setMinimumIndoorTemperature(_d.get());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setMinimumIndoorTemperatureSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature))) {
      ventilation.setMaximumIndoorTemperature(_d.get());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setMaximumIndoorTemperatureSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature))) {
      ventilation.setDeltaTemperature(_d.get());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setDeltaTemperatureSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature))) {
      ventilation.setMinimumOutdoorTemperature(_d.get());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setMinimumOutdoorTemperatureSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature))) {
      ventilation.setMaximumOutdoorTemperature(_d.get());
    }

    target = workspaceObject.getTarget(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (boost::optional<Schedule> _sch = modelObject->optionalCast<Schedule>()) {
          ventilation.setMaximumOutdoorTemperatureSchedule(_sch.get());
        }
      }
    }

    if ((_d = workspaceObject.getDouble(openstudio::ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed))) {
      ventilation.setMaximumWindSpeed(_d.get());
    }

    return ventilation;
  }

}  // namespace energyplus

}  // namespace openstudio
