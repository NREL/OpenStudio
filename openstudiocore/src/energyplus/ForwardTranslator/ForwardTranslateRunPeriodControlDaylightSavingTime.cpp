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
#include "../../model/RunPeriodControlDaylightSavingTime.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRunPeriodControlDaylightSavingTime( RunPeriodControlDaylightSavingTime & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::RunPeriodControl_DaylightSavingTime);
  
  boost::optional<std::string> s;

  s = modelObject.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate);
  if( s )
  {
    idfObject.setString(RunPeriodControl_DaylightSavingTimeFields::StartDate, s.get());
  }

  s = modelObject.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate);
  if( s )
  {
    idfObject.setString(RunPeriodControl_DaylightSavingTimeFields::EndDate, s.get());
  }
  
  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

