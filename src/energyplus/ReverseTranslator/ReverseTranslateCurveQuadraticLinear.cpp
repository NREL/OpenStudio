/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveQuadraticLinear.hpp"
#include "../../model/CurveQuadraticLinear_Impl.hpp"

#include <utilities/idd/Curve_QuadraticLinear_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveQuadraticLinear(const WorkspaceObject& workspaceObject) {
    CurveQuadraticLinear curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient1Constant))) {
      curve.setCoefficient1Constant(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient2x))) {
      curve.setCoefficient2x(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient3x_POW_2))) {
      curve.setCoefficient3xPOW2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient4y))) {
      curve.setCoefficient4y(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient5x_TIMES_y))) {
      curve.setCoefficient5xTIMESY(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y))) {
      curve.setCoefficient6xPOW2TIMESY(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumValueofy))) {
      curve.setMinimumValueofy(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumValueofy))) {
      curve.setMaximumValueofy(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::InputUnitTypeforY, false, true))) {
      curve.setInputUnitTypeforY(*s);
    }
    if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
