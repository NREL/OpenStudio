/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveRectangularHyperbola1.hpp"
#include "../../model/CurveRectangularHyperbola1_Impl.hpp"

#include <utilities/idd/Curve_RectangularHyperbola1_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveRectangularHyperbola1(const WorkspaceObject& workspaceObject) {
    CurveRectangularHyperbola1 curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      curve.setName(*s);
    }

    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::Coefficient1C1))) {
      curve.setCoefficient1C1(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::Coefficient2C2))) {
      curve.setCoefficient2C2(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::Coefficient3C3))) {
      curve.setCoefficient3C3(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::MinimumValueofx))) {
      curve.setMinimumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::MaximumValueofx))) {
      curve.setMaximumValueofx(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::MinimumCurveOutput))) {
      curve.setMinimumCurveOutput(*d);
    }
    if ((d = workspaceObject.getDouble(Curve_RectangularHyperbola1Fields::MaximumCurveOutput))) {
      curve.setMaximumCurveOutput(*d);
    }
    if ((s = workspaceObject.getString(Curve_RectangularHyperbola1Fields::InputUnitTypeforx, false, true))) {
      curve.setInputUnitTypeforx(*s);
    }
    if ((s = workspaceObject.getString(Curve_RectangularHyperbola1Fields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
