/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterInverterSimple.hpp"
#include "../../model/ElectricLoadCenterInverterSimple_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Inverter_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterInverterSimple(model::ElectricLoadCenterInverterSimple& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Inverter_Simple, modelObject);

    if (modelObject.name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_SimpleFields::Name, modelObject.name().get());
    }

    if (modelObject.availabilitySchedule() && modelObject.availabilitySchedule().get().name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_SimpleFields::AvailabilityScheduleName, modelObject.availabilitySchedule().get().name().get());
    }

    if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
      idfObject.setString(ElectricLoadCenter_Inverter_SimpleFields::ZoneName, modelObject.thermalZone().get().name().get());
    }

    if (modelObject.radiativeFraction()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_SimpleFields::RadiativeFraction, modelObject.radiativeFraction().get());
    }

    if (modelObject.inverterEfficiency()) {
      idfObject.setDouble(ElectricLoadCenter_Inverter_SimpleFields::InverterEfficiency, modelObject.inverterEfficiency().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
