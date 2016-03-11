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

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/ScheduleTypeLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateScheduleTypeLimits( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ScheduleTypeLimits )
  {
    LOG(Error, "WorkspaceObject is not IddObjectType: ScheduleTypeLimits");
    return boost::none;
  }

  openstudio::model::ScheduleTypeLimits scheduleTypeLimits( m_model );
  OptionalString s = workspaceObject.name();
  if (s) {
    if ((workspaceObject.numFields() == 1u) &&
        (istringEqual(*s,"Any Number") || istringEqual(*s,"Number")))
    {
      // Do not translate ScheduleTypeLimits called "Any Number" or "Number" and
      // with no other fields specified. Instead, let ModelObjects assign more
      // meaningful limits.
      LOG(Info,"The energyplus::ReverseTranslator throws out all 'Any Number' ScheduleTypeLimits "
          << "to leave the OpenStudio model free to apply the appropriate units and limits.");
      return boost::none;
    }
    scheduleTypeLimits.setName(*s);
  }

  OptionalDouble d = workspaceObject.getDouble(ScheduleTypeLimitsFields::LowerLimitValue);
  if (d) {
    scheduleTypeLimits.setLowerLimitValue(*d);
  }

  d = workspaceObject.getDouble(ScheduleTypeLimitsFields::UpperLimitValue);
  if (d) {
    scheduleTypeLimits.setUpperLimitValue(*d);
  }

  s = workspaceObject.getString(ScheduleTypeLimitsFields::NumericType);
  if (s) {
    scheduleTypeLimits.setNumericType(*s);
  }

  s = workspaceObject.getString(ScheduleTypeLimitsFields::UnitType);
  if (s) {
    scheduleTypeLimits.setUnitType(*s);
  }
  else {
    bool test;
    // Attempt to default based on name (many EnergyPlus files do not have this field filled out).
    std::string name = scheduleTypeLimits.name().get();
    if (boost::regex_search(name,boost::regex("[Tt][Ee][Mm][Pp]"))) {
      if (boost::regex_search(name,boost::regex("[Dd][Ee][Ll][Tt][Aa]"))) {
        test = scheduleTypeLimits.setUnitType("DeltaTemperature");
        OS_ASSERT(test);
      }
      else {
        test = scheduleTypeLimits.setUnitType("Temperature");
        OS_ASSERT(test);
      }
    }
    else if (boost::regex_search(name,boost::regex("[Oo][Nn]")) &&
             boost::regex_search(name,boost::regex("[Oo][Ff][Ff]")))
    {
      test = scheduleTypeLimits.setUnitType("Availability");
      OS_ASSERT(test);
    }
    else if (boost::regex_search(name,boost::regex("[Cc][Oo][Nn][Tt][Rr][Oo][Ll]"))) {
      test = scheduleTypeLimits.setUnitType("ControlMode");
      OS_ASSERT(test);
    }
  }

  return scheduleTypeLimits;
}

} // energyplus

} // openstudio

