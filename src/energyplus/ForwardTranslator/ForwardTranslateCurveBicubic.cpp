/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Bicubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveBicubic(CurveBicubic& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Bicubic);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_BicubicFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_BicubicFields::Coefficient2x, modelObject.coefficient2x());
    idfObject.setDouble(Curve_BicubicFields::Coefficient3x_POW_2, modelObject.coefficient3xPOW2());
    idfObject.setDouble(Curve_BicubicFields::Coefficient4y, modelObject.coefficient4y());
    idfObject.setDouble(Curve_BicubicFields::Coefficient5y_POW_2, modelObject.coefficient5yPOW2());
    idfObject.setDouble(Curve_BicubicFields::Coefficient6x_TIMES_y, modelObject.coefficient6xTIMESY());
    idfObject.setDouble(Curve_BicubicFields::Coefficient7x_POW_3, modelObject.coefficient7xPOW3());
    idfObject.setDouble(Curve_BicubicFields::Coefficient8y_POW_3, modelObject.coefficient8yPOW3());
    idfObject.setDouble(Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y, modelObject.coefficient9xPOW2TIMESY());
    idfObject.setDouble(Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2, modelObject.coefficient10xTIMESYPOW2());
    idfObject.setDouble(Curve_BicubicFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_BicubicFields::MaximumValueofx, modelObject.maximumValueofx());
    idfObject.setDouble(Curve_BicubicFields::MinimumValueofy, modelObject.minimumValueofy());
    idfObject.setDouble(Curve_BicubicFields::MaximumValueofy, modelObject.maximumValueofy());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_BicubicFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_BicubicFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_BicubicFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isInputUnitTypeforYDefaulted()) {
      idfObject.setString(Curve_BicubicFields::InputUnitTypeforY, modelObject.inputUnitTypeforY());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_BicubicFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
