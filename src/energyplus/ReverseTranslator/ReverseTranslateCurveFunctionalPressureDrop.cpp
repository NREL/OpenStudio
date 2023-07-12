/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveFunctionalPressureDrop.hpp"
#include "../../model/CurveFunctionalPressureDrop_Impl.hpp"

#include <utilities/idd/Curve_Functional_PressureDrop_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveFunctionalPressureDrop(const WorkspaceObject& workspaceObject) {
    CurveFunctionalPressureDrop curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_Functional_PressureDropFields::Diameter))) {
      curve.setDiameter(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_Functional_PressureDropFields::MinorLossCoefficient))) {
      curve.setMinorLossCoefficient(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_Functional_PressureDropFields::Length))) {
      curve.setLength(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_Functional_PressureDropFields::Roughness))) {
      curve.setRoughness(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_Functional_PressureDropFields::FixedFrictionFactor))) {
      curve.setFixedFrictionFactor(*d);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
