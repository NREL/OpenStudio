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
#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Exterior_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateExteriorLights(
    model::ExteriorLights& modelObject)
{
  IdfObject idfObject(IddObjectType::Exterior_Lights);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(Exterior_LightsFields::Name, modelObject.name().get());

  OptionalIdfObject relatedIdfObject;

  boost::optional<model::Schedule> schedule;
  if( auto t_schedule = modelObject.schedule() ) {
    schedule = t_schedule.get();
  } else {
    schedule = modelObject.model().alwaysOnDiscreteSchedule();
  }
  OS_ASSERT(schedule);
  relatedIdfObject = translateAndMapModelObject(schedule.get());
  OS_ASSERT(relatedIdfObject);
  idfObject.setString(Exterior_LightsFields::ScheduleName,relatedIdfObject->name().get());

  model::ExteriorLightsDefinition definition = modelObject.exteriorLightsDefinition();

  double designLevel = definition.designLevel()*modelObject.multiplier();
  idfObject.setDouble(Exterior_LightsFields::DesignLevel,designLevel);

  if (!modelObject.isControlOptionDefaulted()) {
    idfObject.setString(Exterior_LightsFields::ControlOption,modelObject.controlOption());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()) {
    idfObject.setString(Exterior_LightsFields::EndUseSubcategory,modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus
} // openstudio
