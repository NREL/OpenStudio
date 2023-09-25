/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveQuadraticLinear.hpp"
#include "../../model/CurveQuadraticLinear_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_QuadraticLinear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveQuadraticLinear(CurveQuadraticLinear& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_QuadraticLinear);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient2x, modelObject.coefficient2x());
    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient3x_POW_2, modelObject.coefficient3xPOW2());
    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient4y, modelObject.coefficient4y());
    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient5x_TIMES_y, modelObject.coefficient5xTIMESY());
    idfObject.setDouble(Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y, modelObject.coefficient6xPOW2TIMESY());
    idfObject.setDouble(Curve_QuadraticLinearFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_QuadraticLinearFields::MaximumValueofx, modelObject.maximumValueofx());
    idfObject.setDouble(Curve_QuadraticLinearFields::MinimumValueofy, modelObject.minimumValueofy());
    idfObject.setDouble(Curve_QuadraticLinearFields::MaximumValueofy, modelObject.maximumValueofy());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_QuadraticLinearFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_QuadraticLinearFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_QuadraticLinearFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isInputUnitTypeforYDefaulted()) {
      idfObject.setString(Curve_QuadraticLinearFields::InputUnitTypeforY, modelObject.inputUnitTypeforY());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_QuadraticLinearFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
