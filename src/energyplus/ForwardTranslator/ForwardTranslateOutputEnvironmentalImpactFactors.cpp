/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputEnvironmentalImpactFactors.hpp"

#include "../../model/EnvironmentalImpactFactors.hpp"
#include "../../model/EnvironmentalImpactFactors_Impl.hpp"
#include "../../model/FuelFactors.hpp"
#include "../../model/FuelFactors_Impl.hpp"

#include <utilities/idd/Output_EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputEnvironmentalImpactFactors(model::OutputEnvironmentalImpactFactors& modelObject) {

    // This is the object that triggers the translation of the two others. Also, at **least** on FuelFactor should be present (for Electricity...)
    // or you KNOW E+ is going to crash

    auto fuelFactors = modelObject.model().getConcreteModelObjects<FuelFactors>();
    if (fuelFactors.empty()) {
      LOG(Error, "Your object of type 'OutputEnvironmentalImpactFactors' will not be translated since you have zero FuelFactors, "
                 "which are actually required for each fuel you use");
      return boost::none;
    }

    // It doesn't have a name
    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_EnvironmentalImpactFactors, modelObject);

    idfObject.setString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency, modelObject.reportingFrequency());

    for (auto& fuelFactor : fuelFactors) {
      translateAndMapModelObject(fuelFactor);
    }

    for (auto& environmentalImpactFactor : modelObject.model().getConcreteModelObjects<EnvironmentalImpactFactors>()) {
      translateAndMapModelObject(environmentalImpactFactor);
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
