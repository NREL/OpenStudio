/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"

#include <utilities/idd/People_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePeopleDefinition(PeopleDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::People_Definition, modelObject);

    idfObject.setString(People_DefinitionFields::NumberofPeopleCalculationMethod, modelObject.numberofPeopleCalculationMethod());

    if (boost::optional<double> d = modelObject.numberofPeople()) {
      idfObject.setDouble(People_DefinitionFields::NumberofPeople, *d);
    }

    if (boost::optional<double> d = modelObject.peopleperSpaceFloorArea()) {
      idfObject.setDouble(People_DefinitionFields::PeopleperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.spaceFloorAreaperPerson()) {
      idfObject.setDouble(People_DefinitionFields::FloorAreaperPerson, *d);
    }

    idfObject.setDouble(People_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());

    if (boost::optional<double> d = modelObject.sensibleHeatFraction()) {
      idfObject.setDouble(People_DefinitionFields::SensibleHeatFraction, *d);
    }

    if (!modelObject.isCarbonDioxideGenerationRateDefaulted()) {
      idfObject.setDouble(People_DefinitionFields::CarbonDioxideGenerationRate, modelObject.carbonDioxideGenerationRate());
    }

    if (!modelObject.isEnableASHRAE55ComfortWarningsDefaulted()) {
      if (modelObject.enableASHRAE55ComfortWarnings()) {
        idfObject.setString(People_DefinitionFields::EnableASHRAE55ComfortWarnings, "Yes");
      } else {
        idfObject.setString(People_DefinitionFields::EnableASHRAE55ComfortWarnings, "No");
      }
    }

    if (!modelObject.isMeanRadiantTemperatureCalculationTypeDefaulted()) {
      idfObject.setString(People_DefinitionFields::MeanRadiantTemperatureCalculationType, modelObject.meanRadiantTemperatureCalculationType());
    }

    // As of 22.2.0, this is no longer possible to make this an extensible field
    // because E+ added 3 regular fields at the end (eg: Ankle Level Velocity Schedule Name) on the (legacy) People object.
    // People:Definition doesn't have that constraint, but we keep the same 7-model limit for consistency with the legacy translation.
    for (int i = 0, numComfortModelTypes = 0; i < modelObject.numThermalComfortModelTypes(); ++i) {
      boost::optional<std::string> s = modelObject.getThermalComfortModelType(i);
      if (s) {
        ++numComfortModelTypes;
        if (numComfortModelTypes > 7) {
          LOG(Warn, "For " << modelObject.briefDescription() << ", only 7 Thermal Confort Model Types are supported by EnergyPlus, number "
                            << numComfortModelTypes << " [=" << *s << "] will be ignored.");
        } else {
          idfObject.setString(People_DefinitionFields::ThermalComfortModel1Type + i, *s);
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
