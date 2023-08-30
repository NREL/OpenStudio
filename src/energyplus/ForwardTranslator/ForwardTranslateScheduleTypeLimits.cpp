/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ScheduleTypeLimits.hpp"
#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>

#include <utilities/idd/ScheduleTypeLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleTypeLimits(ScheduleTypeLimits& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ScheduleTypeLimits);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ScheduleTypeLimitsFields::Name, modelObject.name().get());

    OptionalDouble d = modelObject.getDouble(OS_ScheduleTypeLimitsFields::LowerLimitValue, false);
    if (d) {
      idfObject.setDouble(ScheduleTypeLimitsFields::LowerLimitValue, *d);
    }

    d = modelObject.getDouble(OS_ScheduleTypeLimitsFields::UpperLimitValue, false);
    if (d) {
      idfObject.setDouble(ScheduleTypeLimitsFields::UpperLimitValue, *d);
    }

    OptionalString s = modelObject.getString(OS_ScheduleTypeLimitsFields::NumericType, false, true);
    if (s) {
      idfObject.setString(ScheduleTypeLimitsFields::NumericType, *s);
    }

    s = modelObject.getString(OS_ScheduleTypeLimitsFields::UnitType, false, true);
    if (s) {
      std::string unitType = *s;
      boost::to_lower(unitType);
      if ((unitType == "clothinginsulation") || (unitType == "controlmode") || (unitType == "linearpowerdensity") || (unitType == "massflowrate")
          || (unitType == "pressure") || (unitType == "rotationsperminute") || (unitType == "solarenergy") || (unitType == "volumetricflowrate")) {
        // unit type key is unsupported in EnergyPlus--fall back on 'Any Number'
        m_idfObjects.pop_back();
        if (!m_anyNumberScheduleTypeLimits) {
          IdfObject anyNumberLimits(IddObjectType::ScheduleTypeLimits);
          m_idfObjects.push_back(anyNumberLimits);
          anyNumberLimits.setName("Any Number");
          m_anyNumberScheduleTypeLimits = anyNumberLimits;
        }
        return m_anyNumberScheduleTypeLimits;
      }
      idfObject.setString(ScheduleTypeLimitsFields::UnitType, unitType);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
