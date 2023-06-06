/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveFanPressureRise.hpp"
#include "../../model/CurveFanPressureRise_Impl.hpp"

#include <utilities/idd/Curve_FanPressureRise_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveFanPressureRise(const WorkspaceObject& workspaceObject) {
    CurveFanPressureRise curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient1C1))) {
      curve.setCoefficient1C1(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient2C2))) {
      curve.setCoefficient2C2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient3C3))) {
      curve.setCoefficient3C3(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient4C4))) {
      curve.setCoefficient4C4(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumValueofQfan))) {
      curve.setMinimumValueofQfan(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumValueofQfan))) {
      curve.setMaximumValueofQfan(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumValueofPsm))) {
      curve.setMinimumValueofPsm(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumValueofPsm))) {
      curve.setMaximumValueofPsm(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
