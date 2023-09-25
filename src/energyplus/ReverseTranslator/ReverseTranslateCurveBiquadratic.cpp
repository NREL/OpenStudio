/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"

#include <utilities/idd/Curve_Biquadratic_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveBiquadratic(const WorkspaceObject& workspaceObject) {
    CurveBiquadratic curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient1Constant))) {
      curve.setCoefficient1Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient2x))) {
      curve.setCoefficient2x(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient3x_POW_2))) {
      curve.setCoefficient3xPOW2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient4y))) {
      curve.setCoefficient4y(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient5y_POW_2))) {
      curve.setCoefficient5yPOW2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::Coefficient6x_TIMES_y))) {
      curve.setCoefficient6xTIMESY(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MinimumValueofy))) {
      curve.setMinimumValueofy(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MaximumValueofy))) {
      curve.setMaximumValueofy(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_BiquadraticFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_BiquadraticFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_BiquadraticFields::InputUnitTypeforY, false, true))) {
      curve.setInputUnitTypeforY(*s);
    }
    if ((s = workspaceObject.getString(Curve_BiquadraticFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
