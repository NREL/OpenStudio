/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ElectricLoadCenterInverterLookUpTable.hpp"
#include "ElectricLoadCenterInverterLookUpTable_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Inverter_LookUpTable_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricLoadCenterInverterLookUpTable_Impl::ElectricLoadCenterInverterLookUpTable_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : Inverter_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterInverterLookUpTable::iddObjectType());
  }

  ElectricLoadCenterInverterLookUpTable_Impl::ElectricLoadCenterInverterLookUpTable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : Inverter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricLoadCenterInverterLookUpTable::iddObjectType());
  }

  ElectricLoadCenterInverterLookUpTable_Impl::ElectricLoadCenterInverterLookUpTable_Impl(const ElectricLoadCenterInverterLookUpTable_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : Inverter_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricLoadCenterInverterLookUpTable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ElectricLoadCenterInverterLookUpTable_Impl::iddObjectType() const {
    return ElectricLoadCenterInverterLookUpTable::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricLoadCenterInverterLookUpTable_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricLoadCenterInverterLookUpTable","Availability"));
    }
    return result;
  }

  boost::optional<Schedule> ElectricLoadCenterInverterLookUpTable_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName);
  }

  boost::optional<ThermalZone> ElectricLoadCenterInverterLookUpTable_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::radiativeFraction() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RadiativeFraction,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::ratedMaximumContinuousOutputPower() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RatedMaximumContinuousOutputPower,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::nightTareLossPower() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NightTareLossPower,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::nominalVoltageInput() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NominalVoltageInput,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt10PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt20PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt30PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt50PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt75PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyAt100PowerAndNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage,true);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName,
                              "ElectricLoadCenterInverterLookUpTable",
                              "Availability",
                              schedule);
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setThermalZone(const ThermalZone& thermalZone) {
    return setPointer(OS_ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName, thermalZone.handle());
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetThermalZone() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setRadiativeFraction(boost::optional<double> radiativeFraction) {
    bool result(false);
    if (radiativeFraction) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RadiativeFraction, radiativeFraction.get());
    }
    else {
      resetRadiativeFraction();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetRadiativeFraction() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RadiativeFraction, "");
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::setRatedMaximumContinuousOutputPower(boost::optional<double> ratedMaximumContinuousOutputPower) {
    bool result(false);
    if (ratedMaximumContinuousOutputPower) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RatedMaximumContinuousOutputPower, ratedMaximumContinuousOutputPower.get());
    }
    else {
      resetRatedMaximumContinuousOutputPower();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetRatedMaximumContinuousOutputPower() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::RatedMaximumContinuousOutputPower, "");
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::setNightTareLossPower(boost::optional<double> nightTareLossPower) {
    bool result(false);
    if (nightTareLossPower) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NightTareLossPower, nightTareLossPower.get());
    }
    else {
      resetNightTareLossPower();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetNightTareLossPower() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NightTareLossPower, "");
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::setNominalVoltageInput(boost::optional<double> nominalVoltageInput) {
    bool result(false);
    if (nominalVoltageInput) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NominalVoltageInput, nominalVoltageInput.get());
    }
    else {
      resetNominalVoltageInput();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetNominalVoltageInput() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::NominalVoltageInput, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt10PowerAndNominalVoltage(boost::optional<double> efficiencyAt10PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt10PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage, efficiencyAt10PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt10PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt10PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt20PowerAndNominalVoltage(boost::optional<double> efficiencyAt20PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt20PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage, efficiencyAt20PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt20PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt20PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt30PowerAndNominalVoltage(boost::optional<double> efficiencyAt30PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt30PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage, efficiencyAt30PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt30PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt30PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt50PowerAndNominalVoltage(boost::optional<double> efficiencyAt50PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt50PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage, efficiencyAt50PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt50PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt50PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt75PowerAndNominalVoltage(boost::optional<double> efficiencyAt75PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt75PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage, efficiencyAt75PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt75PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt75PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyAt100PowerAndNominalVoltage(boost::optional<double> efficiencyAt100PowerAndNominalVoltage) {
    bool result(false);
    if (efficiencyAt100PowerAndNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage, efficiencyAt100PowerAndNominalVoltage.get());
    }
    else {
      resetEfficiencyAt100PowerAndNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyAt100PowerAndNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

} // detail

ElectricLoadCenterInverterLookUpTable::ElectricLoadCenterInverterLookUpTable(const Model& model)
  : Inverter(ElectricLoadCenterInverterLookUpTable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType ElectricLoadCenterInverterLookUpTable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Inverter_LookUpTable);
}

boost::optional<Schedule> ElectricLoadCenterInverterLookUpTable::availabilitySchedule() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->availabilitySchedule();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::radiativeFraction() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->radiativeFraction();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::ratedMaximumContinuousOutputPower() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->ratedMaximumContinuousOutputPower();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::nightTareLossPower() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->nightTareLossPower();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::nominalVoltageInput() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->nominalVoltageInput();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt10PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt10PowerAndNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt20PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt20PowerAndNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt30PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt30PowerAndNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt50PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt50PowerAndNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt75PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt75PowerAndNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyAt100PowerAndNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyAt100PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setAvailabilitySchedule(schedule);
}

void ElectricLoadCenterInverterLookUpTable::resetAvailabilitySchedule() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetAvailabilitySchedule();
}

bool ElectricLoadCenterInverterLookUpTable::setRadiativeFraction(double radiativeFraction) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setRadiativeFraction(radiativeFraction);
}

void ElectricLoadCenterInverterLookUpTable::resetRadiativeFraction() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetRadiativeFraction();
}

void ElectricLoadCenterInverterLookUpTable::setRatedMaximumContinuousOutputPower(double ratedMaximumContinuousOutputPower) {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setRatedMaximumContinuousOutputPower(ratedMaximumContinuousOutputPower);
}

void ElectricLoadCenterInverterLookUpTable::resetRatedMaximumContinuousOutputPower() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetRatedMaximumContinuousOutputPower();
}

void ElectricLoadCenterInverterLookUpTable::setNightTareLossPower(double nightTareLossPower) {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setNightTareLossPower(nightTareLossPower);
}

void ElectricLoadCenterInverterLookUpTable::resetNightTareLossPower() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetNightTareLossPower();
}

void ElectricLoadCenterInverterLookUpTable::setNominalVoltageInput(double nominalVoltageInput) {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setNominalVoltageInput(nominalVoltageInput);
}

void ElectricLoadCenterInverterLookUpTable::resetNominalVoltageInput() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetNominalVoltageInput();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt10PowerAndNominalVoltage(double efficiencyAt10PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt10PowerAndNominalVoltage(efficiencyAt10PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt10PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt10PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt20PowerAndNominalVoltage(double efficiencyAt20PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt20PowerAndNominalVoltage(efficiencyAt20PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt20PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt20PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt30PowerAndNominalVoltage(double efficiencyAt30PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt30PowerAndNominalVoltage(efficiencyAt30PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt30PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt30PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt50PowerAndNominalVoltage(double efficiencyAt50PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt50PowerAndNominalVoltage(efficiencyAt50PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt50PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt50PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt75PowerAndNominalVoltage(double efficiencyAt75PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt75PowerAndNominalVoltage(efficiencyAt75PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt75PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt75PowerAndNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyAt100PowerAndNominalVoltage(double efficiencyAt100PowerAndNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyAt100PowerAndNominalVoltage(efficiencyAt100PowerAndNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyAt100PowerAndNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyAt100PowerAndNominalVoltage();
}

/// @cond
ElectricLoadCenterInverterLookUpTable::ElectricLoadCenterInverterLookUpTable(std::shared_ptr<detail::ElectricLoadCenterInverterLookUpTable_Impl> impl)
  : Inverter(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

