/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveLinear.hpp"
#include "../../model/CurveLinear_Impl.hpp"

#include <utilities/idd/Curve_Linear_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveLinear(const WorkspaceObject& workspaceObject) {
    CurveLinear curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_LinearFields::Coefficient1Constant))) {
      curve.setCoefficient1Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_LinearFields::Coefficient2x))) {
      curve.setCoefficient2x(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_LinearFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_LinearFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_LinearFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_LinearFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_LinearFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_LinearFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
