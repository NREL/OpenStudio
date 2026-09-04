/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
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

    std::vector<MRTWeightingFactor> mrtWeightingFactors = modelObject.mrtWeightingFactors();
    if (mrtWeightingFactors.empty()) {
      LOG(Warn,
          "ZoneMRTCalculation for zone '" << zone.nameString() << "' doesn't have at least one MRT weighting factor, it will not be translated.");
      return boost::none;
    }

    std::vector<std::pair<std::string, double>> translatedMRTWeightingFactors;
    double sum = 0.0;

    for (const MRTWeightingFactor& mrtWeightingFactor : mrtWeightingFactors) {
      People people = mrtWeightingFactor.people();

      boost::optional<Space> space = people.space();

      if (!space) {
        LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription() << " because "
                                                                          << people.briefDescription()
                                                                          << " is not assigned to a Space.");
        continue;
      }

      boost::optional<ThermalZone> thermalZone = space->thermalZone();

      if (!thermalZone || (thermalZone->handle() != zone.handle())) {
        LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription() << " because "
                                                                          << people.briefDescription()
                                                                          << " is not assigned to the ZoneMRTCalculation ThermalZone.");
        continue;
      }

      if (people.peopleDefinition().numThermalComfortModelTypes() == 0) {
        LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription() << " because "
                                                                          << people.briefDescription()
                                                                          << " does not select at least one Thermal Comfort Model Type.");
        continue;
      }

      boost::optional<IdfObject> _people = translateAndMapModelObject(people);

      if (_people) {
        double value = mrtWeightingFactor.mrtWeightingFactor();
        sum += value;
        if (sum > 1.0) {
          LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription()
                                                                            << " because the MRT Weighting Factors would sum to " << sum
                                                                            << ", which is greater than 1.");
          sum -= value;
          continue;
        }

        translatedMRTWeightingFactors.emplace_back(_people->name().get(), value);
      } else {
        LOG(Error, "Could not translate an MRTWeightingFactor group for " << modelObject.briefDescription() << ". Continuing with the rest.");
      }
    }

    if (translatedMRTWeightingFactors.empty()) {
      LOG(Error, "ZoneMRTCalculation for zone '" << zone.nameString()
                                                 << "' doesn't have at least one valid MRT weighting factor, it will not be translated.");
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::ZoneMRTCalculation);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ZoneMRTCalculationFields::ZoneName, zone.nameString());

    for (const auto& translatedMRTWeightingFactor : translatedMRTWeightingFactors) {
      auto eg = idfObject.pushExtensibleGroup();
      eg.setString(ZoneMRTCalculationExtensibleFields::PeopleName, translatedMRTWeightingFactor.first);
      eg.setDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor, translatedMRTWeightingFactor.second);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio