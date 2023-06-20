/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveFunctionalPressureDrop.hpp"
#include "../../model/CurveFunctionalPressureDrop_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Functional_PressureDrop_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveFunctionalPressureDrop(CurveFunctionalPressureDrop& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Functional_PressureDrop);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    idfObject.setDouble(Curve_Functional_PressureDropFields::Diameter, modelObject.diameter());
    if ((d = modelObject.minorLossCoefficient())) {
      idfObject.setDouble(Curve_Functional_PressureDropFields::MinorLossCoefficient, *d);
    }
    if ((d = modelObject.length())) {
      idfObject.setDouble(Curve_Functional_PressureDropFields::Length, *d);
    }
    if ((d = modelObject.roughness())) {
      idfObject.setDouble(Curve_Functional_PressureDropFields::Roughness, *d);
    }
    if ((d = modelObject.fixedFrictionFactor())) {
      idfObject.setDouble(Curve_Functional_PressureDropFields::FixedFrictionFactor, *d);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
