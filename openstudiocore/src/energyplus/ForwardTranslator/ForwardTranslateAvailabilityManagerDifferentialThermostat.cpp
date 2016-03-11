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
#include "../../model/AvailabilityManagerDifferentialThermostat.hpp"
#include "../../model/AvailabilityManagerDifferentialThermostat_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_DifferentialThermostat_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerDifferentialThermostat( 
    AvailabilityManagerDifferentialThermostat & modelObject)
{
  IdfObject idfObject(IddObjectType::AvailabilityManager_DifferentialThermostat);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // HotNodeName
  if( auto node = modelObject.hotNode() ) {
    idfObject.setString(AvailabilityManager_DifferentialThermostatFields::HotNodeName,node->name().get());
  }

  // ColdNodeName
  if( auto node = modelObject.coldNode() ) {
    idfObject.setString(AvailabilityManager_DifferentialThermostatFields::ColdNodeName,node->name().get());
  }

  // TemperatureDifferenceOnLimit
  {
    auto value = modelObject.temperatureDifferenceOnLimit();
    idfObject.setDouble(AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit,value);
  }

  // TemperatureDifferenceOffLimit
  {
    auto value = modelObject.temperatureDifferenceOffLimit();
    idfObject.setDouble(AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit,value);
  }

  return idfObject;
}

} // energyplus
} // openstudio
