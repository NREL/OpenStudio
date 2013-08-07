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
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerOutdoorAirReset.hpp>
#include <model/SetpointManagerWarmest.hpp>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateNode( Node & modelObject )
{
  OptionalSetpointManagerMixedAir setpointManagerMixedAir = modelObject.getSetpointManagerMixedAir();
  if( setpointManagerMixedAir )
  {
    translateAndMapModelObject( setpointManagerMixedAir.get() );
  }

  OptionalSetpointManagerSingleZoneReheat setpointManagerSingleZoneReheat
    = modelObject.getSetpointManagerSingleZoneReheat();
  if( setpointManagerSingleZoneReheat )
  {
    translateAndMapModelObject( setpointManagerSingleZoneReheat.get());
  }

  boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature
    = modelObject.setpointManagerFollowOutdoorAirTemperature();
  if( setpointManagerFollowOutdoorAirTemperature )
  {
    translateAndMapModelObject( setpointManagerFollowOutdoorAirTemperature.get());
  }

  boost::optional<SetpointManagerScheduled> setpointManagerScheduled
    = modelObject.setpointManagerScheduled();
  if( setpointManagerScheduled )
  {
    translateAndMapModelObject( setpointManagerScheduled.get());
  }

  boost::optional<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirReset
    = modelObject.setpointManagerOutdoorAirReset();
  if( setpointManagerOutdoorAirReset )
  {
    translateAndMapModelObject( setpointManagerOutdoorAirReset.get());
  }

  boost::optional<SetpointManagerWarmest> setpointManagerWarmest
    = modelObject.setpointManagerWarmest();
  if( setpointManagerWarmest )
  {
    translateAndMapModelObject( setpointManagerWarmest.get());
  }

  return boost::optional<IdfObject>();
}

} // energyplus

} // openstudio

