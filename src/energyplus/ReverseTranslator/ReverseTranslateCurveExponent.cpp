/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveExponent.hpp"
#include "../../model/CurveExponent_Impl.hpp"

#include <utilities/idd/Curve_Exponent_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveExponent(const WorkspaceObject& workspaceObject) {
    CurveExponent curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_ExponentFields::Coefficient1Constant))) {
      curve.setCoefficient1Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::Coefficient2Constant))) {
      curve.setCoefficient2Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::Coefficient3Constant))) {
      curve.setCoefficient3Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_ExponentFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_ExponentFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
