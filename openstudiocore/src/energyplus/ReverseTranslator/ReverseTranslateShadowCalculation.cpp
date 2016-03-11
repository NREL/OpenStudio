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

#include "../../model/ShadowCalculation.hpp"
#include "../../model/ShadowCalculation_Impl.hpp"

#include <utilities/idd/ShadowCalculation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<model::ModelObject> ReverseTranslator::translateShadowCalculation(
    const WorkspaceObject& workspaceObject)
{
  OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::ShadowCalculation);

  ShadowCalculation shadowCalculation = m_model.getUniqueModelObject<ShadowCalculation>();

  OptionalInt i = workspaceObject.getInt(ShadowCalculationFields::CalculationFrequency);
  if (i) {
    shadowCalculation.setCalculationFrequency(*i);
  }

  i = workspaceObject.getInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations);
  if (i) {
    shadowCalculation.setMaximumFiguresInShadowOverlapCalculations(*i);
  }

  OptionalString s = workspaceObject.getString(ShadowCalculationFields::PolygonClippingAlgorithm);
  if (s && !s->empty()) {
    shadowCalculation.setPolygonClippingAlgorithm(*s);
  }

  s = workspaceObject.getString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm);
  if (s && !s->empty()) {
    shadowCalculation.setSkyDiffuseModelingAlgorithm(*s);
  }

  return shadowCalculation.cast<ModelObject>();
}

} // energyplus
} // openstuio
