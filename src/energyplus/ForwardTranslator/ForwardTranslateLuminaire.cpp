/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/LuminaireDefinition.hpp"
#include "../../model/LuminaireDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateLuminaire(Luminaire& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Lights);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    LuminaireDefinition definition = modelObject.luminaireDefinition();

    idfObject.setString(LightsFields::Name, modelObject.name().get());

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(LightsFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(LightsFields::ScheduleName, idf_schedule_->nameString());
    }

    idfObject.setString(LightsFields::DesignLevelCalculationMethod, "LightingLevel");

    double multiplier = modelObject.multiplier();

    idfObject.setDouble(LightsFields::LightingLevel, multiplier * definition.lightingPower());

    if (!definition.isReturnAirFractionDefaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFraction, definition.returnAirFraction());
    }

    if (!definition.isFractionRadiantDefaulted()) {
      idfObject.setDouble(LightsFields::FractionRadiant, definition.fractionRadiant());
    }

    if (!definition.isFractionVisibleDefaulted()) {
      idfObject.setDouble(LightsFields::FractionVisible, definition.fractionVisible());
    }

    if (!modelObject.isFractionReplaceableDefaulted()) {
      idfObject.setDouble(LightsFields::FractionReplaceable, modelObject.fractionReplaceable());
    }

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    if (!definition.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()) {
      if (definition.returnAirFractionCalculatedfromPlenumTemperature()) {
        idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature, "Yes");
      } else {
        idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature, "No");
      }
    }

    if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                          definition.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
    }

    if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                          definition.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
