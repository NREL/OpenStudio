/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "ElectricLoadCenterInverterLookUpTable.hpp"
#include "ElectricLoadCenterInverterLookUpTable_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
//#include "Zone.hpp"
//#include "Zone_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

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

  //boost::optional<Zone> ElectricLoadCenterInverterLookUpTable_Impl::zone() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Zone>(OS_ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName);
  //}

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

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat10PowerandNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat20PowerandNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat30PowerandNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat50PowerandNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat75PowerandNominalVoltage() const {
    return getDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage,true);
  }

  boost::optional<double> ElectricLoadCenterInverterLookUpTable_Impl::efficiencyat100PowerandNominalVoltage() const {
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

  //bool ElectricLoadCenterInverterLookUpTable_Impl::setZone(const boost::optional<Zone>& zone) {
  //  bool result(false);
  //  if (zone) {
  //    result = setPointer(OS_ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName, zone.get().handle());
  //  }
  //  else {
  //    resetZone();
  //    result = true;
  //  }
  //  return result;
  //}

  void ElectricLoadCenterInverterLookUpTable_Impl::resetZone() {
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

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat10PowerandNominalVoltage(boost::optional<double> efficiencyat10PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat10PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage, efficiencyat10PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat10PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat10PowerandNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat20PowerandNominalVoltage(boost::optional<double> efficiencyat20PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat20PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage, efficiencyat20PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat20PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat20PowerandNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat30PowerandNominalVoltage(boost::optional<double> efficiencyat30PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat30PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage, efficiencyat30PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat30PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat30PowerandNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat50PowerandNominalVoltage(boost::optional<double> efficiencyat50PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat50PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage, efficiencyat50PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat50PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat50PowerandNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat75PowerandNominalVoltage(boost::optional<double> efficiencyat75PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat75PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage, efficiencyat75PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat75PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat75PowerandNominalVoltage() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterLookUpTable_Impl::setEfficiencyat100PowerandNominalVoltage(boost::optional<double> efficiencyat100PowerandNominalVoltage) {
    bool result(false);
    if (efficiencyat100PowerandNominalVoltage) {
      result = setDouble(OS_ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage, efficiencyat100PowerandNominalVoltage.get());
    }
    else {
      resetEfficiencyat100PowerandNominalVoltage();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterInverterLookUpTable_Impl::resetEfficiencyat100PowerandNominalVoltage() {
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

//boost::optional<Zone> ElectricLoadCenterInverterLookUpTable::zone() const {
//  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->zone();
//}

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

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat10PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat10PowerandNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat20PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat20PowerandNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat30PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat30PowerandNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat50PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat50PowerandNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat75PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat75PowerandNominalVoltage();
}

boost::optional<double> ElectricLoadCenterInverterLookUpTable::efficiencyat100PowerandNominalVoltage() const {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->efficiencyat100PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setAvailabilitySchedule(schedule);
}

void ElectricLoadCenterInverterLookUpTable::resetAvailabilitySchedule() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetAvailabilitySchedule();
}

//bool ElectricLoadCenterInverterLookUpTable::setZone(const Zone& zone) {
//  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setZone(zone);
//}

void ElectricLoadCenterInverterLookUpTable::resetZone() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetZone();
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

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat10PowerandNominalVoltage(double efficiencyat10PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat10PowerandNominalVoltage(efficiencyat10PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat10PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat10PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat20PowerandNominalVoltage(double efficiencyat20PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat20PowerandNominalVoltage(efficiencyat20PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat20PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat20PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat30PowerandNominalVoltage(double efficiencyat30PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat30PowerandNominalVoltage(efficiencyat30PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat30PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat30PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat50PowerandNominalVoltage(double efficiencyat50PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat50PowerandNominalVoltage(efficiencyat50PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat50PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat50PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat75PowerandNominalVoltage(double efficiencyat75PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat75PowerandNominalVoltage(efficiencyat75PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat75PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat75PowerandNominalVoltage();
}

bool ElectricLoadCenterInverterLookUpTable::setEfficiencyat100PowerandNominalVoltage(double efficiencyat100PowerandNominalVoltage) {
  return getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->setEfficiencyat100PowerandNominalVoltage(efficiencyat100PowerandNominalVoltage);
}

void ElectricLoadCenterInverterLookUpTable::resetEfficiencyat100PowerandNominalVoltage() {
  getImpl<detail::ElectricLoadCenterInverterLookUpTable_Impl>()->resetEfficiencyat100PowerandNominalVoltage();
}

/// @cond
ElectricLoadCenterInverterLookUpTable::ElectricLoadCenterInverterLookUpTable(std::shared_ptr<detail::ElectricLoadCenterInverterLookUpTable_Impl> impl)
  : Inverter(impl)
{}
/// @endcond

} // model
} // openstudio

