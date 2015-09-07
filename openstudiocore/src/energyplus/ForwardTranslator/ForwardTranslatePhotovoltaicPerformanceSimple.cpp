/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../../model/PhotovoltaicPerformanceSimple.hpp"
#include "../../model/PhotovoltaicPerformanceSimple_Impl.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePhotovoltaicPerformanceSimple(model::PhotovoltaicPerformanceSimple & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PhotovoltaicPerformance_Simple, modelObject);
  
  // Evan, this is just hard coded for testing
  idfObject.setDouble(PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells, 1.0);
  idfObject.setString(PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode, "Fixed");
  idfObject.setDouble(PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, 0.2);

  return idfObject;
}

} // energyplus

} // openstudio

