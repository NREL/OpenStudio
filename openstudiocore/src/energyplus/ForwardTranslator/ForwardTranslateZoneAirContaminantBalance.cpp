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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneAirContaminantBalance.hpp"
#include "../../model/ZoneAirContaminantBalance_Impl.hpp"
#include <utilities/idd/ZoneAirContaminantBalance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneAirContaminantBalance( ZoneAirContaminantBalance& modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ZoneAirContaminantBalance);

  m_idfObjects.push_back(idfObject);

  {
    if( modelObject.carbonDioxideConcentration() ) { 
      idfObject.setString(ZoneAirContaminantBalanceFields::CarbonDioxideConcentration,"Yes");
    } else {
      idfObject.setString(ZoneAirContaminantBalanceFields::CarbonDioxideConcentration,"No");
    }
  }

  {
    if( auto schedule = modelObject.outdoorCarbonDioxideSchedule() ) {
      idfObject.setString(ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName,schedule->nameString());
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

