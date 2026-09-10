/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ComfortViewFactorAngles.hpp"
#include "../../model/ComfortViewFactorAngles_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/math/FloatCompare.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ComfortViewFactorAngles_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateComfortViewFactorAngles(ComfortViewFactorAngles& modelObject) {
    std::vector<AngleFactor> angleFactors = modelObject.angleFactors();
    if (angleFactors.empty()) {
      LOG(Warn, modelObject.briefDescription() << " does not have any angle factors and will not be translated.");
      return boost::none;
    }

    std::vector<std::pair<std::string, double>> translatedAngleFactors;
    double sum = 0.0;
    constexpr double tolerance = 0.000001;
    boost::optional<ThermalZone> thermalZone;

    for (const AngleFactor& angleFactor : angleFactors) {
      ModelObject surface = angleFactor.surface();
      boost::optional<Space> space;
      if (auto planarSurface = surface.optionalCast<PlanarSurface>()) {
        space = planarSurface->space();
      } else if (auto internalMass = surface.optionalCast<InternalMass>()) {
        space = internalMass->space();
      }
      const auto surfaceThermalZone = space ? space->thermalZone() : boost::none;
      if (!surfaceThermalZone || (thermalZone && (surfaceThermalZone->handle() != thermalZone->handle()))) {
        LOG(Error, modelObject.briefDescription()
                     << " has surfaces that are not all assigned to the same ThermalZone and will not be translated.");
        return boost::none;
      }
      thermalZone = surfaceThermalZone;
      if (auto idfSurface = translateAndMapModelObject(surface)) {
        const double value = angleFactor.angleFactor();
        sum += value;
        translatedAngleFactors.emplace_back(idfSurface->nameString(), value);
      } else {
        LOG(Error, "Could not translate an AngleFactor group for " << modelObject.briefDescription() << ". Continuing with the rest.");
      }
    }

    if (translatedAngleFactors.empty()) {
      LOG(Error, modelObject.briefDescription() << " does not have any valid angle factors and will not be translated.");
      return boost::none;
    }

    if (!equal(sum, 1.0, tolerance)) {
      LOG(Error, modelObject.briefDescription() << " has valid angle factors that sum to " << sum << " instead of 1.0 and will not be translated.");
      return boost::none;
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
