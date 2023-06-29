/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveSigmoid.hpp"
#include "../../model/CurveSigmoid_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Sigmoid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveSigmoid(CurveSigmoid& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Sigmoid);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_SigmoidFields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_SigmoidFields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_SigmoidFields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_SigmoidFields::Coefficient4C4, modelObject.coefficient4C4());
    idfObject.setDouble(Curve_SigmoidFields::Coefficient5C5, modelObject.coefficient5C5());
    idfObject.setDouble(Curve_SigmoidFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_SigmoidFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_SigmoidFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_SigmoidFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforxDefaulted()) {
      idfObject.setString(Curve_SigmoidFields::InputUnitTypeforx, modelObject.inputUnitTypeforx());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_SigmoidFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
