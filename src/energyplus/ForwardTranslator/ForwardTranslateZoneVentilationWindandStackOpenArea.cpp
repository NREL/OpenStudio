/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ZoneVentilationWindandStackOpenArea.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneVentilation_WindandStackOpenArea_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneVentilationWindandStackOpenArea(ZoneVentilationWindandStackOpenArea& modelObject) {
    auto idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneVentilation_WindandStackOpenArea, modelObject);

    // Note: no need to really handle the case where it doesn't exist since ThermalZone is the one calling this translation to begin with
    if (boost::optional<ThermalZone> value = modelObject.thermalZone()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::ZoneorSpaceName, value->name().get());
    }

    {
      double value = modelObject.openingArea();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningArea, value);
    }

    {
      Schedule schedule = modelObject.openingAreaFractionSchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName, _schedule->name().get());
      }
    }

    if (modelObject.isOpeningEffectivenessAutocalculated()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, "Autocalculate");
    } else {
      if (boost::optional<double> value = modelObject.openingEffectiveness()) {
        idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, value.get());
      }
    }

    {
      double value = modelObject.effectiveAngle();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle, value);
    }

    {
      double value = modelObject.heightDifference();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::HeightDifference, value);
    }

    if (modelObject.isDischargeCoefficientforOpeningAutocalculated()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, "Autocalculate");
    } else {
      if (boost::optional<double> value = modelObject.dischargeCoefficientforOpening()) {
        idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, value.get());
      }
    }

    {
      auto value = modelObject.minimumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature, value);
    }

    if (auto value = modelObject.minimumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature, value);
    }

    if (auto value = modelObject.maximumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.deltaTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature, value);
    }

    if (auto value = modelObject.deltaTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.minimumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature, value);
    }

    if (auto value = modelObject.minimumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature, value);
    }

    if (auto value = modelObject.maximumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumWindSpeed();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
