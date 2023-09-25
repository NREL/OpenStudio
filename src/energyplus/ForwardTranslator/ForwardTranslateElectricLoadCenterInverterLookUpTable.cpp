/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterInverterLookUpTable.hpp"
#include "../../model/ElectricLoadCenterInverterLookUpTable_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Inverter_LookUpTable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateElectricLoadCenterInverterLookUpTable(model::ElectricLoadCenterInverterLookUpTable& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Inverter_LookUpTable, modelObject);

    if (modelObject.name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::Name, modelObject.name().get());
    }

    if (modelObject.availabilitySchedule() && modelObject.availabilitySchedule().get().name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName,
                          modelObject.availabilitySchedule().get().name().get());
    }

    if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName, modelObject.thermalZone().get().name().get());
    }

    if (modelObject.radiativeFraction()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::RadiativeFraction, modelObject.radiativeFraction().get());
    }

    if (modelObject.ratedMaximumContinuousOutputPower()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::RatedMaximumContinuousOutputPower,
                          modelObject.ratedMaximumContinuousOutputPower().get());
    }

    if (modelObject.nightTareLossPower()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::NightTareLossPower, modelObject.nightTareLossPower().get());
    }

    if (modelObject.nominalVoltageInput()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::NominalVoltageInput, modelObject.nominalVoltageInput().get());
    }

    if (modelObject.efficiencyAt10PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage,
                          modelObject.efficiencyAt10PowerAndNominalVoltage().get());
    }

    if (modelObject.efficiencyAt20PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage,
                          modelObject.efficiencyAt20PowerAndNominalVoltage().get());
    }

    if (modelObject.efficiencyAt30PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage,
                          modelObject.efficiencyAt30PowerAndNominalVoltage().get());
    }

    if (modelObject.efficiencyAt50PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage,
                          modelObject.efficiencyAt50PowerAndNominalVoltage().get());
    }

    if (modelObject.efficiencyAt75PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage,
                          modelObject.efficiencyAt75PowerAndNominalVoltage().get());
    }

    if (modelObject.efficiencyAt100PowerAndNominalVoltage()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage,
                          modelObject.efficiencyAt100PowerAndNominalVoltage().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
