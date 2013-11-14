/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ReverseTranslator.hpp>

#include <model/ConvergenceLimits.hpp>
#include <model/ConvergenceLimits_Impl.hpp>

#include <utilities/idd/ConvergenceLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Optional.hpp>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<model::ModelObject> ReverseTranslator::translateConvergenceLimits(
    const WorkspaceObject& workspaceObject)
{
  OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::ConvergenceLimits);

  OptionalInt i;

  ConvergenceLimits convergenceLimits = m_model.getUniqueModelObject<ConvergenceLimits>();

  if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MinimumSystemTimestep))) {
    convergenceLimits.setMinimumSystemTimestep(*i);
  }

  if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MaximumHVACIterations))) {
    convergenceLimits.setMaximumHVACIterations(*i);
  }
  
  if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MinimumPlantIterations))) {
    convergenceLimits.setMinimumPlantIterations(*i);
  }

  if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MaximumPlantIterations))) {
    convergenceLimits.setMaximumPlantIterations(*i);
  }

  return convergenceLimits;
}

} // energyplus
} // openstudio
