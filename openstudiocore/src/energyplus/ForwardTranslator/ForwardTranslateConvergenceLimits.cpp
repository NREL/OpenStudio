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
#include "../../model/ConvergenceLimits.hpp"
#include <utilities/idd/ConvergenceLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateConvergenceLimits( ConvergenceLimits & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::ConvergenceLimits);

  if (OptionalInt i = modelObject.minimumSystemTimestep()) {
    idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep,*i);
  }else{
    // EnergyPlus IDD has a note saying 1 is the default but does not actually list it as the default
    // A severe error message is issued if this field is blank
    idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep,1);
  }

  if (!modelObject.isMaximumHVACIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MaximumHVACIterations,
                     modelObject.maximumHVACIterations());
  }

  if (!modelObject.isMinimumPlantIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MinimumPlantIterations,
                     modelObject.minimumPlantIterations());
  }

  if (!modelObject.isMaximumPlantIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MaximumPlantIterations,
                     modelObject.maximumPlantIterations());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus
} // openstudio
