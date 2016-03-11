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

#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateScheduleCompact( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Schedule_Compact )
  {
    LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Compact");
    return boost::none;
  }

  ScheduleCompact scheduleCompact(m_model);

  OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_CompactFields::ScheduleTypeLimitsName);
  if (target){
    OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
    if (scheduleTypeLimits){
      scheduleCompact.setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
    }
  }

  if (OptionalString os = workspaceObject.name()) {
    scheduleCompact.setName(*os);
  }

  for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
    scheduleCompact.pushExtensibleGroup(eg.fields());
  }

  return scheduleCompact;
}

} // energyplus

} // openstudio

