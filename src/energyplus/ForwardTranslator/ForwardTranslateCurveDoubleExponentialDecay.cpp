/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveDoubleExponentialDecay.hpp"
#include "../../model/CurveDoubleExponentialDecay_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_DoubleExponentialDecay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveDoubleExponentialDecay(CurveDoubleExponentialDecay& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_DoubleExponentialDecay);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_DoubleExponentialDecayFields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::Coefficient4C4, modelObject.coefficient4C4());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::Coefficient5C5, modelObject.coefficient5C5());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_DoubleExponentialDecayFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_DoubleExponentialDecayFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_DoubleExponentialDecayFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforxDefaulted()) {
      idfObject.setString(Curve_DoubleExponentialDecayFields::InputUnitTypeforx, modelObject.inputUnitTypeforx());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_DoubleExponentialDecayFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
