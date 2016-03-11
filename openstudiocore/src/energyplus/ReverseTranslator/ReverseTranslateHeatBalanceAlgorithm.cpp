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

#include "../../model/HeatBalanceAlgorithm.hpp"
#include "../../model/HeatBalanceAlgorithm_Impl.hpp"

#include <utilities/idd/HeatBalanceAlgorithm_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<model::ModelObject> ReverseTranslator::translateHeatBalanceAlgorithm(
    const WorkspaceObject& workspaceObject)
{
  OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::HeatBalanceAlgorithm);

  HeatBalanceAlgorithm heatBalanceAlgorithm = m_model.getUniqueModelObject<HeatBalanceAlgorithm>();

  OptionalString s = workspaceObject.getString(HeatBalanceAlgorithmFields::Algorithm,false,true);
  if (s) {
    heatBalanceAlgorithm.setAlgorithm(*s);
  }

  OptionalDouble d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit);
  if (d) {
    heatBalanceAlgorithm.setSurfaceTemperatureUpperLimit(*d);
  }

  d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue);
  if (d) {
    heatBalanceAlgorithm.setMinimumSurfaceConvectionHeatTransferCoefficientValue(*d);
  }

  d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue);
  if (d) {
    heatBalanceAlgorithm.setMaximumSurfaceConvectionHeatTransferCoefficientValue(*d);
  }

  return heatBalanceAlgorithm.cast<ModelObject>();
}

} // energyplus
} // openstudio
