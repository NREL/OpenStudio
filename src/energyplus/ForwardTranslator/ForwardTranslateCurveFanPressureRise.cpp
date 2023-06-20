/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveFanPressureRise.hpp"
#include "../../model/CurveFanPressureRise_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_FanPressureRise_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveFanPressureRise(CurveFanPressureRise& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_FanPressureRise);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_FanPressureRiseFields::Coefficient1C1, modelObject.coefficient1C1());
    idfObject.setDouble(Curve_FanPressureRiseFields::Coefficient2C2, modelObject.coefficient2C2());
    idfObject.setDouble(Curve_FanPressureRiseFields::Coefficient3C3, modelObject.coefficient3C3());
    idfObject.setDouble(Curve_FanPressureRiseFields::Coefficient4C4, modelObject.coefficient4C4());
    idfObject.setDouble(Curve_FanPressureRiseFields::MinimumValueofQfan, modelObject.minimumValueofQfan());
    idfObject.setDouble(Curve_FanPressureRiseFields::MaximumValueofQfan, modelObject.maximumValueofQfan());
    idfObject.setDouble(Curve_FanPressureRiseFields::MinimumValueofPsm, modelObject.minimumValueofPsm());
    idfObject.setDouble(Curve_FanPressureRiseFields::MaximumValueofPsm, modelObject.maximumValueofPsm());
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_FanPressureRiseFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_FanPressureRiseFields::MaximumCurveOutput, *d);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
