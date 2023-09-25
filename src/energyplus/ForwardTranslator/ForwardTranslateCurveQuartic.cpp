/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveQuartic.hpp"
#include "../../model/CurveQuartic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Quartic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveQuartic(CurveQuartic& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Quartic);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_QuarticFields::Coefficient1Constant, modelObject.coefficient1Constant());
    idfObject.setDouble(Curve_QuarticFields::Coefficient2x, modelObject.coefficient2x());
    idfObject.setDouble(Curve_QuarticFields::Coefficient3x_POW_2, modelObject.coefficient3xPOW2());
    idfObject.setDouble(Curve_QuarticFields::Coefficient4x_POW_3, modelObject.coefficient4xPOW3());
    idfObject.setDouble(Curve_QuarticFields::Coefficient5x_POW_4, modelObject.coefficient5xPOW4());
    idfObject.setDouble(Curve_QuarticFields::MinimumValueofx, modelObject.minimumValueofx());
    idfObject.setDouble(Curve_QuarticFields::MaximumValueofx, modelObject.maximumValueofx());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_QuarticFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_QuarticFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_QuarticFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_QuarticFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
