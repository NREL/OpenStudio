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

#include "../../model/Model.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateScheduleConstant( ScheduleConstant & modelObject )
{
  IdfObject scheduleConstant = createAndRegisterIdfObject(openstudio::IddObjectType::Schedule_Constant,
                                                          modelObject);

  scheduleConstant.setName(modelObject.name().get());
  scheduleConstant.setString(Schedule_ConstantFields::ScheduleTypeLimitsName,
                             modelObject.getString(OS_Schedule_ConstantFields::ScheduleTypeLimitsName,true).get());
  scheduleConstant.setDouble(Schedule_ConstantFields::HourlyValue, modelObject.value());

  return boost::optional<IdfObject>(scheduleConstant);
}

} // energyplus

} // openstudio

