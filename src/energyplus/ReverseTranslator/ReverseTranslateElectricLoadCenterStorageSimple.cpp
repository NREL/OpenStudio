/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

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

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageSimple(const WorkspaceObject& workspaceObject) {

    OptionalModelObject result, temp;
    OptionalDouble d;
    boost::optional<WorkspaceObject> owo;
    OptionalString optS;

    // TODO: The availability schedule is in the ElectricLoadCenter:Generators (list) in E+, here it's carried by the generator itself
    // Should also get the Rated Thermal To Electrical Power Ratio in the list

    //Generator:MicroTurbine,
    //    Capstone C65,            !- Name

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
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get())) {
        if (boost::optional<ThermalZone> thermalZone = mo->optionalCast<ThermalZone>()) {
          elcStorSimple.setThermalZone(thermalZone.get());
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

    result = elcStorSimple;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
