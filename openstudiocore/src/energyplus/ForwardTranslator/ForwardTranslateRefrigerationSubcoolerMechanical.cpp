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
#include "../../model/RefrigerationSubcoolerMechanical.hpp"

#include <utilities/idd/Refrigeration_Subcooler_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSubcoolerMechanical( RefrigerationSubcoolerMechanical & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject subcoolerMechanical = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Subcooler, modelObject);

  subcoolerMechanical.setString(Refrigeration_SubcoolerFields::SubcoolerType, "Mechanical");
  
  // Capacity-Providing System
  boost::optional<RefrigerationSystem> capacityProvidingSystem = modelObject.capacityProvidingSystem();

  if( capacityProvidingSystem )
  {
    boost::optional<IdfObject> _capacityProvidingSystem = translateAndMapModelObject(capacityProvidingSystem.get());

    if( _capacityProvidingSystem && _capacityProvidingSystem->name() )
    {
      subcoolerMechanical.setString(Refrigeration_SubcoolerFields::CapacityProvidingSystem,_capacityProvidingSystem->name().get());
    }
  }
   
  // Outlet Control Temperature
  d = modelObject.outletControlTemperature();
  if (d) {
    subcoolerMechanical.setDouble(Refrigeration_SubcoolerFields::OutletControlTemperature,d.get());
  }
  
  return subcoolerMechanical;
}
}
}
