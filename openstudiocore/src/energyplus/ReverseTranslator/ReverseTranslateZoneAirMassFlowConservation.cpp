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

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneAirMassFlowConservation.hpp"
#include "../../model/ZoneAirMassFlowConservation_Impl.hpp"
#include "../../model/Model.hpp"

#include <utilities/idd/ZoneAirMassFlowConservation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneAirMassFlowConservation(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::ZoneAirMassFlowConservation){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneAirMassFlowConservation");
    return boost::none;
  }

  openstudio::model::ZoneAirMassFlowConservation result = m_model.getUniqueModelObject<ZoneAirMassFlowConservation>();

  OptionalString s = workspaceObject.getString(openstudio::ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, true);
  if (s){
    if (istringEqual("Yes", *s)){
      result.setAdjustZoneMixingForZoneAirMassFlowBalance(true);
    } else{
      result.setAdjustZoneMixingForZoneAirMassFlowBalance(false);
    }
  }

  s = workspaceObject.getString(openstudio::ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, true);
  if (s){
    result.setSourceZoneInfiltrationTreatment(*s);
  }

  return result;
}

} // energyplus

} // openstudio

