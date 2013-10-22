/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeLimits_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/foreach.hpp>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateScheduleCompact( ScheduleCompact & modelObject )
{
  IdfObject scheduleCompact = createAndRegisterIdfObject(openstudio::IddObjectType::Schedule_Compact,
                                                         modelObject);

  OptionalString os;
  if ((os = modelObject.name())) {
    scheduleCompact.setName(*os);
  }

  boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
  if (scheduleTypeLimits){
    boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
    if (idfScheduleTypeLimits){
      scheduleCompact.setString(Schedule_CompactFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
    }
  }

  BOOST_FOREACH(const IdfExtensibleGroup& eg,modelObject.extensibleGroups()) {
    scheduleCompact.pushExtensibleGroup(eg.fields());
  }

  return boost::optional<IdfObject>(scheduleCompact);
}

} // energyplus

} // openstudio

