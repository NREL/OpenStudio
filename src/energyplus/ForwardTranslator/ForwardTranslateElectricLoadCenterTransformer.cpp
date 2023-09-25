/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterTransformer.hpp"
#include "../../model/ElectricLoadCenterTransformer_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterTransformer(model::ElectricLoadCenterTransformer& modelObject) {
    boost::optional<double> optD;
    std::string s;
    boost::optional<std::string> optS;
    boost::optional<Schedule> schedule;
    boost::optional<ThermalZone> zone;

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Transformer, modelObject);

    schedule = modelObject.availabilitySchedule();
    if (schedule) {
      boost::optional<IdfObject> scheduleIdf = translateAndMapModelObject(*schedule);
      if (scheduleIdf) {
        idfObject.setString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, scheduleIdf->name().get());
      }
    }

    idfObject.setString(ElectricLoadCenter_TransformerFields::TransformerUsage, modelObject.transformerUsage());

    zone = modelObject.zone();
    if (zone) {
      idfObject.setString(ElectricLoadCenter_TransformerFields::ZoneName, zone->name().get());
    }

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::RadiativeFraction, modelObject.radiativeFraction());

    optD = modelObject.ratedCapacity();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedCapacity, optD.get());
    }

    idfObject.setString(ElectricLoadCenter_TransformerFields::Phase, modelObject.phase());

    idfObject.setString(ElectricLoadCenter_TransformerFields::ConductorMaterial, modelObject.conductorMaterial());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, modelObject.fullLoadTemperatureRise());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, modelObject.fractionofEddyCurrentLosses());

    idfObject.setString(ElectricLoadCenter_TransformerFields::PerformanceInputMethod, modelObject.performanceInputMethod());

    optD = modelObject.ratedNoLoadLoss();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, optD.get());
    }

    optD = modelObject.ratedLoadLoss();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedLoadLoss, optD.get());
    }

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::NameplateEfficiency, modelObject.nameplateEfficiency());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, modelObject.perUnitLoadforNameplateEfficiency());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency,
                        modelObject.referenceTemperatureforNameplateEfficiency());

    optD = modelObject.perUnitLoadforMaximumEfficiency();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, optD.get());
    }
    if (modelObject.considerTransformerLossforUtilityCost()) {
      s = "Yes";
    } else {
      s = "No";
    }
    idfObject.setString(ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, s);

    //meters
    std::vector<std::string> meters = modelObject.meters();
    if (!meters.empty()) {
      for (const auto& meter : meters) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ElectricLoadCenter_TransformerExtensibleFields::MeterName, meter);
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
