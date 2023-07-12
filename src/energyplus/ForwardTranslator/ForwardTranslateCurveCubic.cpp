/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Cubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveCubic(CurveCubic& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Cubic);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_CubicFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_CubicFields::Coefficient2x, modelObject.coefficient2x());
    idfObject.setDouble(Curve_CubicFields::Coefficient3x_POW_2, modelObject.coefficient3xPOW2());
    idfObject.setDouble(Curve_CubicFields::Coefficient4x_POW_3, modelObject.coefficient4xPOW3());
    idfObject.setDouble(Curve_CubicFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_CubicFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_CubicFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_CubicFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_CubicFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_CubicFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
