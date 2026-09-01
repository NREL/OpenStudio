/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneMRTCalculation.hpp"
#include "../../model/ZoneMRTCalculation_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneMRTCalculation_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneMRTCalculation(ZoneMRTCalculation& modelObject) {
    ThermalZone zone = modelObject.thermalZone();
    boost::optional<IdfObject> _zone = translateAndMapModelObject(zone);
    if (!_zone) {
      LOG(Error, "ZoneMRTCalculation references a zone '" << zone.nameString() << "' that was not translated, so it will not be translated either");
      return boost::none;
    }

    std::vector<MRTWeightingFactor> mrtWeightingFactors = modelObject.mrtWeightingFactors();
    if (mrtWeightingFactors.empty()) {
      LOG(Error,
          "ZoneMRTCalculation for zone '" << zone.nameString() << "' doesn't have at least one MRT weighting factor, it will not be translated.");
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::ZoneMRTCalculation);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ZoneMRTCalculationFields::ZoneName, _zone->name().get());

    for (const MRTWeightingFactor& mrtWeightingFactor : mrtWeightingFactors) {
      People people = mrtWeightingFactor.people();
      boost::optional<IdfObject> _people = translateAndMapModelObject(people);

      if (_people) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ZoneMRTCalculationExtensibleFields::PeopleName, _people->name().get());
        eg.setDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor, mrtWeightingFactor.mrtWeightingFactor());
      } else {
        LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription() << ". Continuing with the rest.");
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio