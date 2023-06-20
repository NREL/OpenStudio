/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveSigmoid.hpp"
#include "../../model/CurveSigmoid_Impl.hpp"

#include <utilities/idd/Curve_Sigmoid_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveSigmoid(const WorkspaceObject& workspaceObject) {
    CurveSigmoid curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::Coefficient1C1))) {
      curve.setCoefficient1C1(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::Coefficient2C2))) {
      curve.setCoefficient2C2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::Coefficient3C3))) {
      curve.setCoefficient3C3(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::Coefficient4C4))) {
      curve.setCoefficient4C4(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::Coefficient5C5))) {
      curve.setCoefficient5C5(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_SigmoidFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_SigmoidFields::InputUnitTypeforx, false, true))) {
      curve.setInputUnitTypeforx(*s);
    }
    if ((s = workspaceObject.getString(Curve_SigmoidFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
