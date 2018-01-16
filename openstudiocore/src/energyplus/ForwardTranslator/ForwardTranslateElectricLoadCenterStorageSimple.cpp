/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterStorageSimple(model::ElectricLoadCenterStorageSimple & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Storage_Simple, modelObject);

  if (modelObject.name()) {
    idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::Name, modelObject.name().get());
  }

  // Availability Schedule, defaults to model.alwaysOnDiscrete
  Schedule sched = modelObject.availabilitySchedule();
  boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
  if (_sched) {
    idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName,
      _sched->name().get());
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

} // energyplus

} // openstudio

