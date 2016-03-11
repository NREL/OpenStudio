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
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include "../../model/RunPeriodControlSpecialDays_Impl.hpp"
#include <utilities/idd/RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateRunPeriodControlSpecialDays( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::RunPeriodControl_SpecialDays )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: RunPeriodControl_SpecialDays");
     return boost::none;
  }

  boost::optional<std::string> s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::StartDate);
  if( !s )
  {
    LOG(Error, "No start date specified for RunPeriodControl:SpecialDays");
    return boost::none;
  }

  boost::optional<RunPeriodControlSpecialDays> mo;
  try{
    mo = RunPeriodControlSpecialDays(*s, m_model);
  }catch(...){
    LOG(Error, "'" << *s << "'  is not a correct date specification");
    return boost::none;
  }

  s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::Name);
  if( s )
  {
    mo->setName(s.get());
  }

  boost::optional<int> i = workspaceObject.getInt(RunPeriodControl_SpecialDaysFields::Duration);
  if( i )
  {
    mo->setDuration(i.get());
  }

  s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType);
  if( s )
  {
    mo->setString(OS_RunPeriodControl_SpecialDaysFields::SpecialDayType,s.get());
  }

  return *mo;
}

} // energyplus

} // openstudio

