/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterStorageSimple.hpp"
#include "../../model/ElectricLoadCenterStorageSimple_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterStorageSimple(model::ElectricLoadCenterStorageSimple& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Storage_Simple, modelObject);

    if (modelObject.name()) {
      idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::Name, modelObject.name().get());
    }

    // Availability Schedule, defaults to model.alwaysOnDiscrete
    Schedule sched = modelObject.availabilitySchedule();
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
    if (_sched) {
      idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName, _sched->name().get());
    }

    // ZoneName
    if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
      idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::ZoneName, modelObject.thermalZone().get().name().get());
    }

    // Radiative Fraction, defaults (double)
    {
      double value = modelObject.radiativeFractionforZoneHeatGains();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains, value);
    }

    // nominalEnergeticEfficiencyforCharging, defaults
    {
      double value = modelObject.nominalEnergeticEfficiencyforCharging();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging, value);
    }

    // nominalEnergeticEfficiencyforDischarging, defaults
    {
      double value = modelObject.nominalDischargingEnergeticEfficiency();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency, value);
    }

    // maximumStorageCapacity, required, assigned in ctor
    {
      double value = modelObject.maximumStorageCapacity();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity, value);
    }

    // maximumPowerforDischarging, required, assigned in ctor
    {
      double value = modelObject.maximumPowerforDischarging();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging, value);
    }

    // maximumPowerforCharging, required, assigned in ctor
    {
      double value = modelObject.maximumPowerforCharging();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging, value);
    }

    // initialStateofCharge
    {
      double value = modelObject.initialStateofCharge();
      idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
