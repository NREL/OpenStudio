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
#include "../../model/RefrigerationCondenserCascade.hpp"

#include <utilities/idd/Refrigeration_Condenser_Cascade_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCondenserCascade( RefrigerationCondenserCascade & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject cascade = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_Cascade, modelObject);

  // Rated Condensing Temperature
  d = modelObject.ratedCondensingTemperature();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::RatedCondensingTemperature,d.get());
  }

  // Rated Approach Temperature Difference
  d = modelObject.ratedApproachTemperatureDifference();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::RatedApproachTemperatureDifference,d.get());
  }

  // Rated Effective Total Heat Rejection Rate
  d = modelObject.ratedEffectiveTotalHeatRejectionRate();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::RatedEffectiveTotalHeatRejectionRate,d.get());
  }

  // Condensing Temperature Control Type
  s = modelObject.condensingTemperatureControlType();
  if (s) {
    cascade.setString(Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType,s.get());
  }

  // Condenser Refrigerant Operating Charge Inventory
  d = modelObject.condenserRefrigerantOperatingChargeInventory();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::CondenserRefrigerantOperatingChargeInventory,d.get());
  }

  // Condensate Receiver Refrigerant Inventory
  d = modelObject.condensateReceiverRefrigerantInventory();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::CondensateReceiverRefrigerantInventory,d.get());
  }

  // Condensate Piping Refrigerant Inventory
  d = modelObject.condensatePipingRefrigerantInventory();
  if (d) {
    cascade.setDouble(Refrigeration_Condenser_CascadeFields::CondensatePipingRefrigerantInventory,d.get());
  }
  
  return cascade;

}
}
}
