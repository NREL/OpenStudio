/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

  OptionalString s;
  OptionalDouble d;
  OptionalModelObject mo;

  if (modelObject.inletModelObject()) {
    translateModelObject(modelObject.inletModelObject().get());
  }

  if (modelObject.outletModelObject()) {
    translateModelObject(modelObject.outletModelObject().get());
  }

  ///////////////////////////////////////////////////////////////////////////
  s = modelObject.name();
  if (s)
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.construction();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::Pipe_IndoorFields::ConstructionName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::Pipe_IndoorFields::EnvironmentType, modelObject.environmentType());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.ambientTemperatureZone();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureZoneName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.ambientTemperatureSchedule();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureScheduleName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.ambientAirVelocitySchedule();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientAirVelocityScheduleName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());
  ///////////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeLength, modelObject.pipeLength());
  ///////////////////////////////////////////////////////////////////////////

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

