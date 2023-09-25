/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Biquadratic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveBiquadratic(CurveBiquadratic& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Biquadratic);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_BiquadraticFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_BiquadraticFields::Coefficient2x, modelObject.coefficient2x());
    idfObject.setDouble(Curve_BiquadraticFields::Coefficient3x_POW_2, modelObject.coefficient3xPOW2());
    idfObject.setDouble(Curve_BiquadraticFields::Coefficient4y, modelObject.coefficient4y());
    idfObject.setDouble(Curve_BiquadraticFields::Coefficient5y_POW_2, modelObject.coefficient5yPOW2());
    idfObject.setDouble(Curve_BiquadraticFields::Coefficient6x_TIMES_y, modelObject.coefficient6xTIMESY());
    idfObject.setDouble(Curve_BiquadraticFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_BiquadraticFields::MaximumValueofx, modelObject.maximumValueofx());
    idfObject.setDouble(Curve_BiquadraticFields::MinimumValueofy, modelObject.minimumValueofy());
    idfObject.setDouble(Curve_BiquadraticFields::MaximumValueofy, modelObject.maximumValueofy());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_BiquadraticFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_BiquadraticFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_BiquadraticFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isInputUnitTypeforYDefaulted()) {
      idfObject.setString(Curve_BiquadraticFields::InputUnitTypeforY, modelObject.inputUnitTypeforY());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_BiquadraticFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
