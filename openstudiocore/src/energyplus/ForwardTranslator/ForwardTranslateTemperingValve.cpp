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
#include "../../model/TemperingValve.hpp"
#include "../../model/TemperingValve_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/TemperingValve_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateTemperingValve( 
    TemperingValve & modelObject)
{
  IdfObject idfObject(IddObjectType::TemperingValve);
  m_idfObjects.push_back(idfObject);

  modelObject.getImpl<model::detail::TemperingValve_Impl>()->setControlNodes();

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // InletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(TemperingValveFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(TemperingValveFields::OutletNodeName,node->name().get());
    }
  }

  // Stream2SourceNodeName
  if( auto node = modelObject.stream2SourceNode() ) {
    idfObject.setString(TemperingValveFields::Stream2SourceNodeName,node->name().get()); 
  }

  // TemperatureSetpointNodeName
  if( auto node = modelObject.temperatureSetpointNode() ) {
    idfObject.setString(TemperingValveFields::TemperatureSetpointNodeName,node->name().get()); 
  }

  // PumpOutletNodeName
  if( auto node = modelObject.pumpOutletNode() ) {
    idfObject.setString(TemperingValveFields::PumpOutletNodeName,node->name().get()); 
  }

  return idfObject;
}

} // energyplus
} // openstudio
