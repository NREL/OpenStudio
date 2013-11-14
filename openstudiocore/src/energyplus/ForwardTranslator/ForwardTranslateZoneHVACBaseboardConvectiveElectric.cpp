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
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ZoneHVACBaseboardConvectiveElectric.hpp>
#include <model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <utilities/idd/ZoneHVAC_Baseboard_Convective_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACBaseboardConvectiveElectric(
    ZoneHVACBaseboardConvectiveElectric & modelObject )
{
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in 
  // the final file. Also set's the idfObject's name.
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneHVAC_Baseboard_Convective_Electric,modelObject);

  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;
  
  // AvailabilityScheduleName
  Schedule availabilitySchedule = modelObject.availabilitySchedule();
  translateAndMapModelObject(availabilitySchedule);
  idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilityScheduleName,
                      availabilitySchedule.name().get() );

  // NominalCapacity

  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity,value.get());
  }

  // Efficiency

  if( (value = modelObject.efficiency()) )
  {
    idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

