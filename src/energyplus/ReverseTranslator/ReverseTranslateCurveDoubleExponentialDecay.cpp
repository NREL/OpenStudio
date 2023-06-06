/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveDoubleExponentialDecay.hpp"
#include "../../model/CurveDoubleExponentialDecay_Impl.hpp"

#include <utilities/idd/Curve_DoubleExponentialDecay_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveDoubleExponentialDecay(const WorkspaceObject& workspaceObject) {
    CurveDoubleExponentialDecay curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::Coefficient1C1))) {
      curve.setCoefficient1C1(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::Coefficient2C2))) {
      curve.setCoefficient2C2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::Coefficient3C3))) {
      curve.setCoefficient3C3(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::Coefficient4C4))) {
      curve.setCoefficient4C4(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::Coefficient5C5))) {
      curve.setCoefficient5C5(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_DoubleExponentialDecayFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_DoubleExponentialDecayFields::InputUnitTypeforx, false, true))) {
      curve.setInputUnitTypeforx(*s);
    }
    if ((s = workspaceObject.getString(Curve_DoubleExponentialDecayFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
