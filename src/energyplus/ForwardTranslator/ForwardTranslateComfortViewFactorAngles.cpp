/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ComfortViewFactorAngles.hpp"

#include "../../utilities/math/FloatCompare.hpp"

#include <utilities/idd/ComfortViewFactorAngles_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateComfortViewFactorAngles(ComfortViewFactorAngles& modelObject) {
    const auto comfortViewFactorAngles = modelObject.comfortViewFactorAngles();
    if (comfortViewFactorAngles.empty()) {
      LOG(Error, modelObject.briefDescription() << " does not have any angle factors and will not be translated.");
      return boost::none;
    }

    double sum = 0.0;
    for (const auto& comfortViewFactorAngle : comfortViewFactorAngles) {
      sum += comfortViewFactorAngle.angleFactor();
    }
    constexpr double tolerance = 0.000001;
    if (!equal(sum, 1.0, tolerance)) {
      LOG(Error, modelObject.briefDescription() << " has angle factors that sum to " << sum << " instead of 1.0 and will not be translated.");
      return boost::none;
    }

    std::vector<std::pair<std::string, double>> translatedAngleFactors;
    for (const auto& comfortViewFactorAngle : comfortViewFactorAngles) {
      if (auto surface = translateAndMapModelObject(comfortViewFactorAngle.surface())) {
        translatedAngleFactors.emplace_back(surface->nameString(), comfortViewFactorAngle.angleFactor());
      } else {
        LOG(Error, "Could not translate a Surface for " << modelObject.briefDescription() << ".");
        return boost::none;
      }
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ComfortViewFactorAngles, modelObject);
    for (const auto& [surfaceName, angleFactor] : translatedAngleFactors) {
      auto group = idfObject.pushExtensibleGroup();
      group.setString(ComfortViewFactorAnglesExtensibleFields::SurfaceName, surfaceName);
      group.setDouble(ComfortViewFactorAnglesExtensibleFields::AngleFactor, angleFactor);
    }
    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio