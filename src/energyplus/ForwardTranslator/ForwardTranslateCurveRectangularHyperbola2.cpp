/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveRectangularHyperbola2.hpp"
#include "../../model/CurveRectangularHyperbola2_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_RectangularHyperbola2_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveRectangularHyperbola2(CurveRectangularHyperbola2& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_RectangularHyperbola2);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_RectangularHyperbola2Fields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_RectangularHyperbola2Fields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_RectangularHyperbola2Fields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_RectangularHyperbola2Fields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_RectangularHyperbola2Fields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_RectangularHyperbola2Fields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_RectangularHyperbola2Fields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforxDefaulted()) {
      idfObject.setString(Curve_RectangularHyperbola2Fields::InputUnitTypeforx, modelObject.inputUnitTypeforx());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_RectangularHyperbola2Fields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
