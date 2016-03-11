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
#include "../../model/ZoneAirMassFlowConservation.hpp"
#include "../../model/ZoneAirMassFlowConservation_Impl.hpp"

#include <utilities/idd/ZoneAirMassFlowConservation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneAirMassFlowConservation(
    ZoneAirMassFlowConservation & modelObject)
{
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in 
  // the final file. Also set's the idfObject's name.
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneAirMassFlowConservation, modelObject);

  if (modelObject.adjustZoneMixingForZoneAirMassFlowBalance()){
    idfObject.setString(ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, "Yes");
  }else{
    idfObject.setString(ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, "No");
  }

  idfObject.setString(ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, modelObject.infiltrationBalancingMethod());
  
  idfObject.setString(ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, modelObject.infiltrationBalancingZones());

  return idfObject;
}

} // energyplus

} // openstudio

