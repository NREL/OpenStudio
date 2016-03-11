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
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/PipeIndoor.hpp"
#include "../../model/PipeIndoor_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Pipe_Indoor_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePipeIndoor(PipeIndoor & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::Pipe_Indoor);

  m_idfObjects.push_back(idfObject);

  if(auto value = modelObject.name()) {
    idfObject.setName(value.get());
  }

  if(auto node = modelObject.inletModelObject()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::FluidInletNodeName, node->name().get());
  }

  if(auto node = modelObject.outletModelObject()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::FluidOutletNodeName, node->name().get());
  }

  if(auto construction = modelObject.construction()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::ConstructionName,construction->name().get());
  }

  idfObject.setString(openstudio::Pipe_IndoorFields::EnvironmentType, modelObject.environmentType());

  if(auto zone = modelObject.ambientTemperatureZone()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureZoneName,zone->name().get());
  }

  if(auto schedule = modelObject.ambientTemperatureSchedule()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureScheduleName,schedule->name().get());
  }

  if(auto schedule = modelObject.ambientAirVelocitySchedule()) {
    idfObject.setString(openstudio::Pipe_IndoorFields::AmbientAirVelocityScheduleName,schedule->name().get());
  }

  idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());

  idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeLength, modelObject.pipeLength());

  return idfObject;
}

} // energyplus

} // openstudio

