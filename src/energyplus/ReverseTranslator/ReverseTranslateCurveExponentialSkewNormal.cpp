/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveExponentialSkewNormal.hpp"
#include "../../model/CurveExponentialSkewNormal_Impl.hpp"

#include <utilities/idd/Curve_ExponentialSkewNormal_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveExponentialSkewNormal(const WorkspaceObject& workspaceObject) {
    CurveExponentialSkewNormal curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::Coefficient1C1))) {
      curve.setCoefficient1C1(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::Coefficient2C2))) {
      curve.setCoefficient2C2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::Coefficient3C3))) {
      curve.setCoefficient3C3(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::Coefficient4C4))) {
      curve.setCoefficient4C4(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_ExponentialSkewNormalFields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_ExponentialSkewNormalFields::InputUnitTypeforx, false, true))) {
      curve.setInputUnitTypeforx(*s);
    }
    if ((s = workspaceObject.getString(Curve_ExponentialSkewNormalFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
