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

#include "../../model/ScheduleTypeLimits.hpp"
#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>

#include <utilities/idd/ScheduleTypeLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateScheduleTypeLimits( ScheduleTypeLimits & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ScheduleTypeLimits);
  m_idfObjects.push_back(idfObject);

  idfObject.setString(ScheduleTypeLimitsFields::Name, modelObject.name().get());

  OptionalDouble d = modelObject.getDouble(OS_ScheduleTypeLimitsFields::LowerLimitValue, false);
  if (d){
    idfObject.setDouble(ScheduleTypeLimitsFields::LowerLimitValue, *d);
  }

  d = modelObject.getDouble(OS_ScheduleTypeLimitsFields::UpperLimitValue, false);
  if (d){
    idfObject.setDouble(ScheduleTypeLimitsFields::UpperLimitValue, *d);
  }

  OptionalString s = modelObject.getString(OS_ScheduleTypeLimitsFields::NumericType, false, true);
  if (s){
    idfObject.setString(ScheduleTypeLimitsFields::NumericType, *s);
  }

  s = modelObject.getString(OS_ScheduleTypeLimitsFields::UnitType, false, true);
  if (s){
    std::string unitType = *s;
    boost::to_lower(unitType);
    if ((unitType == "clothinginsulation") ||
        (unitType == "rotationsperminute") ||
        (unitType == "massflowrate") ||
        (unitType == "pressure") ||
        (unitType == "solarenergy"))
    {
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

} // energyplus

} // openstudio

