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
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentList_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEquipmentList( ZoneHVACEquipmentList & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  std::vector<ModelObject> objects = modelObject.equipment();

  if (objects.empty()){
    // do not write out this object
    return boost::none;
  }

  IdfObject idfObject(IddObjectType::ZoneHVAC_EquipmentList);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  for( std::vector<ModelObject>::iterator it = objects.begin();
       it != objects.end();
       it++ )
  {
    unsigned coolingPriority = modelObject.coolingPriority(*it);
    unsigned heatingPriority = modelObject.heatingPriority(*it);

    boost::optional<IdfObject> _equipment = translateAndMapModelObject(*it);

    if( _equipment )
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_equipment->iddObject().name()); 
      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_equipment->name().get()); 
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,coolingPriority); 
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,heatingPriority); 
    }
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

