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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/PlantEquipmentOperationOutdoorRelativeHumidity.hpp"
#include "../../model/PlantEquipmentOperationOutdoorRelativeHumidity_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/PlantEquipmentOperation_OutdoorRelativeHumidity_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationOutdoorRelativeHumidity( PlantEquipmentOperationOutdoorRelativeHumidity & modelObject )
{
  IdfObject idfObject(IddObjectType::PlantEquipmentOperation_OutdoorRelativeHumidity);
  m_idfObjects.push_back(idfObject);

  // Name
  auto name = modelObject.name().get();
  idfObject.setName(name);

  double lowerLimit = modelObject.minimumLowerLimit();
  int i = 1;
  for( auto upperLimit : modelObject.loadRangeUpperLimits() ) {
    auto equipment = modelObject.equipment(upperLimit);
    if( ! equipment.empty() ) {
      auto eg = idfObject.pushExtensibleGroup();
      eg.setDouble(PlantEquipmentOperation_OutdoorRelativeHumidityExtensibleFields::RelativeHumidityRangeLowerLimit,lowerLimit);
      eg.setDouble(PlantEquipmentOperation_OutdoorRelativeHumidityExtensibleFields::RelativeHumidityRangeUpperLimit,upperLimit);

      IdfObject equipmentList(IddObjectType::PlantEquipmentList);
      m_idfObjects.push_back(equipmentList);
      auto equipmentListName = name + " equipment list " + std::to_string(i);
      equipmentList.setName(equipmentListName);
      eg.setString(PlantEquipmentOperation_OutdoorRelativeHumidityExtensibleFields::RangeEquipmentListName,equipmentListName);

      for( auto component : equipment ) {
        auto eg2 = equipmentList.pushExtensibleGroup();
        auto idf_component = translateAndMapModelObject(component);
        OS_ASSERT(idf_component);
        eg2.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType,idf_component->iddObject().name());
        eg2.setString(PlantEquipmentListExtensibleFields::EquipmentName,idf_component->name().get());
      }
    }

    lowerLimit = upperLimit;
    ++i;
  }

  return idfObject;
}

} // energyplus

} // openstudio

