/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveExponent.hpp"
#include "../../model/CurveExponent_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Exponent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveExponent(CurveExponent& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Exponent);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_ExponentFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_ExponentFields::Coefficient2Constant, modelObject.coefficient2Constant());
    idfObject.setDouble(Curve_ExponentFields::Coefficient3Constant, modelObject.coefficient3Constant());
    idfObject.setDouble(Curve_ExponentFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_ExponentFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_ExponentFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_ExponentFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_ExponentFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_ExponentFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
