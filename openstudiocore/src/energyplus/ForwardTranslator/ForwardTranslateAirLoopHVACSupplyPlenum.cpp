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
#include "../../model/AirLoopHVACSupplyPlenum.hpp"
#include "../../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_SupplyPlenum_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACSupplyPlenum( AirLoopHVACSupplyPlenum & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_SupplyPlenum);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setName(s);

  // Inlet Node Name
  if( boost::optional<model::ModelObject> node = modelObject.inletModelObject() )
  {
    idfObject.setString(openstudio::AirLoopHVAC_SupplyPlenumFields::InletNodeName,node->name().get());
  }

  // ZoneName
  // and
  // ZoneNodeName

  if( boost::optional<model::ThermalZone> zone = modelObject.thermalZone() )
  {
    if( boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get()) )
    {
      idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneName,_zone->name().get());

      model::Node node = zone->zoneAirNode();
      idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneNodeName,node.name().get());
    }
  }


  // OutletNodeName
  
  std::vector<ModelObject> outletModelObjects = modelObject.outletModelObjects();
  for( const auto & outletModelObject : outletModelObjects )
  {
    if( OptionalAirTerminalSingleDuctUncontrolled airTerminal =
        outletModelObject.optionalCast<AirTerminalSingleDuctUncontrolled>() )
    {
      if( boost::optional<model::ModelObject> node = airTerminal->outletModelObject() )
      {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
        eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName,node->name().get());
      }
    }
    else
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName,outletModelObject.name().get());
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

