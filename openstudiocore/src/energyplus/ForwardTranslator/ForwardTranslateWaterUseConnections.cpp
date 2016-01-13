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
#include "../../model/WaterUseConnections.hpp"
#include "../../model/WaterUseConnections_Impl.hpp"
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWaterUseConnections( 
    WaterUseConnections& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  OptionalSchedule schedule;

  IdfObject idfObject(IddObjectType::WaterUse_Connections);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // InletNodeName

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(WaterUse_ConnectionsFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(WaterUse_ConnectionsFields::OutletNodeName,node->name().get());
    }
  }

  // Hot Water Supply Temperature Schedule Name
  
  if( boost::optional<Schedule> s = modelObject.hotWaterSupplyTemperatureSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName,s->name().get());
  }

  // Cold Water Supply Temperature Schedule Name

  if( boost::optional<Schedule> s = modelObject.coldWaterSupplyTemperatureSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName,s->name().get());
  }

  //  Water Use Equipment 1 Name

  std::vector<WaterUseEquipment> equipment = modelObject.waterUseEquipment();

  for( auto & elem : equipment )
  {
    boost::optional<IdfObject> _equipment = translateAndMapModelObject(elem); 

    if( _equipment )
    {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

      group.setString(WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName,_equipment->name().get());
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio

