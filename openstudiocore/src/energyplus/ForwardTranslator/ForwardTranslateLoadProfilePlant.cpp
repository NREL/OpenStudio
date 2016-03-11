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
#include "../../model/LoadProfilePlant.hpp"
#include "../../model/LoadProfilePlant_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/LoadProfile_Plant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateLoadProfilePlant( LoadProfilePlant& modelObject )
{
  OptionalString s;
  OptionalDouble value;
  OptionalModelObject temp;

  //Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::LoadProfile_Plant, modelObject);

  // InletNodeName
  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(LoadProfile_PlantFields::InletNodeName,node->name().get());
  }

  // OutletNodeName
  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(LoadProfile_PlantFields::OutletNodeName,node->name().get());
  }

  // LoadScheduleName
  {
    auto schedule = modelObject.loadSchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(LoadProfile_PlantFields::LoadScheduleName,_schedule->name().get());
    }
  }
  
  // PeakFlowRate
  if( (value = modelObject.peakFlowRate()) ) {
    idfObject.setDouble(LoadProfile_PlantFields::PeakFlowRate,value.get()); 
  }

  // FlowRateFractionScheduleName
  {
    auto schedule = modelObject.flowRateFractionSchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(LoadProfile_PlantFields::FlowRateFractionScheduleName,_schedule->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

