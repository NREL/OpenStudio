/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveExponentialDecay.hpp"
#include "../../model/CurveExponentialDecay_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_ExponentialDecay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveExponentialDecay(CurveExponentialDecay& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_ExponentialDecay);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_ExponentialDecayFields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_ExponentialDecayFields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_ExponentialDecayFields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_ExponentialDecayFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_ExponentialDecayFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_ExponentialDecayFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_ExponentialDecayFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforxDefaulted()) {
      idfObject.setString(Curve_ExponentialDecayFields::InputUnitTypeforx, modelObject.inputUnitTypeforx());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_ExponentialDecayFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
