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
#include "../../model/RefrigerationSystem.hpp"
#include "../../model/RefrigerationSubcoolerLiquidSuction.hpp"

#include <utilities/idd/Refrigeration_Subcooler_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSubcoolerLiquidSuction( RefrigerationSubcoolerLiquidSuction & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject subcoolerLiquidSuction = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Subcooler, modelObject);

  // Subcooler Type
  subcoolerLiquidSuction.setString(Refrigeration_SubcoolerFields::SubcoolerType, "LiquidSuction");

  // Liquid Suction Design Subcooling Temperature Difference
  d = modelObject.liquidSuctionDesignSubcoolingTemperatureDifference();
  if (d) {
    subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::LiquidSuctionDesignSubcoolingTemperatureDifference,d.get());
  }

  // Design Liquid Inlet Temperature
  d = modelObject.designLiquidInletTemperature();
  if (d) {
    subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::DesignLiquidInletTemperature,d.get());
  }

  // Design Vapor Inlet Temperature
  d = modelObject.designVaporInletTemperature();
  if (d) {
    subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::DesignVaporInletTemperature,d.get());
  }

  return subcoolerLiquidSuction;
}
}
}
