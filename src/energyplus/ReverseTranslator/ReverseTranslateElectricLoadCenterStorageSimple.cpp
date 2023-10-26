/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ElectricLoadCenterStorageSimple.hpp"
#include "../../model/ElectricLoadCenterStorageSimple_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_Simple_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageSimple(const WorkspaceObject& workspaceObject) {

    OptionalDouble d;
    boost::optional<WorkspaceObject> owo;
    OptionalString optS;

    openstudio::model::ElectricLoadCenterStorageSimple elcStorSimple(m_model);

    // Name
    optS = workspaceObject.name();
    if (optS) {
      elcStorSimple.setName(*optS);
    }

    // AvailabilityScheduleName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName))) {
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get())) {
        if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
          elcStorSimple.setAvailabilitySchedule(schedule.get());
        }
      }
    }

    // ZoneName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_SimpleFields::ZoneName))) {
      if (boost::optional<ModelObject> mo_ = translateAndMapWorkspaceObject(owo.get())) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = mo_->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            elcStorSimple.setThermalZone(z_.get());
          }
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Zone Name'");
        }
      }
    }

    // Radiative Fraction, defaults (double)
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains);
    if (d) {
      elcStorSimple.setRadiativeFractionforZoneHeatGains(*d);
    }

    // nominalEnergeticEfficiencyforCharging, defaults
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging);
    if (d) {
      elcStorSimple.setNominalEnergeticEfficiencyforCharging(*d);
    }

    // nominalEnergeticEfficiencyforDischarging, defaults
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency);
    if (d) {
      elcStorSimple.setNominalDischargingEnergeticEfficiency(*d);
    }

    // maximumStorageCapacity, required, assigned in ctor
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity);
    if (d) {
      elcStorSimple.setMaximumStorageCapacity(*d);
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum Storage Capacity");
    }

    // maximumPowerforDischarging, required, assigned in ctor
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging);
    if (d) {
      elcStorSimple.setMaximumPowerforDischarging(*d);
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum power for discharging");
    }

    // maximumPowerforCharging, required, assigned in ctor
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging);
    if (d) {
      elcStorSimple.setMaximumPowerforCharging(*d);
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum power for charging");
    }

    // initialStateofCharge
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge);
    if (d) {
      elcStorSimple.setInitialStateofCharge(*d);
    }

    return elcStorSimple;
  }

}  // namespace energyplus

}  // namespace openstudio
