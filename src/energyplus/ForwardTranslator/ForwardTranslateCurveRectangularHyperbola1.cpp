/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveRectangularHyperbola1.hpp"
#include "../../model/CurveRectangularHyperbola1_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_RectangularHyperbola1_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveRectangularHyperbola1(CurveRectangularHyperbola1& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_RectangularHyperbola1);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_RectangularHyperbola1Fields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_RectangularHyperbola1Fields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforxDefaulted()) {
      idfObject.setString(Curve_RectangularHyperbola1Fields::InputUnitTypeforx, modelObject.inputUnitTypeforx());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_RectangularHyperbola1Fields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
