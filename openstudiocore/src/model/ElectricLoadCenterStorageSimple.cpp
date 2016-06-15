/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ElectricLoadCenterStorageSimple.hpp"
#include "ElectricLoadCenterStorageSimple_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Zone.hpp"
#include "Zone_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Storage_Simple_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricLoadCenterStorageSimple_Impl::ElectricLoadCenterStorageSimple_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterStorageSimple::iddObjectType());
  }

  ElectricLoadCenterStorageSimple_Impl::ElectricLoadCenterStorageSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricLoadCenterStorageSimple::iddObjectType());
  }

  ElectricLoadCenterStorageSimple_Impl::ElectricLoadCenterStorageSimple_Impl(const ElectricLoadCenterStorageSimple_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricLoadCenterStorageSimple_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ElectricLoadCenterStorageSimple_Impl::iddObjectType() const {
    return ElectricLoadCenterStorageSimple::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricLoadCenterStorageSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricLoadCenterStorageSimple","Availability"));
    }
    return result;
  }

  boost::optional<Schedule> ElectricLoadCenterStorageSimple_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName);
  }

  boost::optional<Zone> ElectricLoadCenterStorageSimple_Impl::zone() const {
    return getObject<ModelObject>().getModelObjectTarget<Zone>(OS_ElectricLoadCenter_Storage_SimpleFields::ZoneName);
  }

  double ElectricLoadCenterStorageSimple_Impl::radiativeFractionforZoneHeatGains() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterStorageSimple_Impl::isRadiativeFractionforZoneHeatGainsDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains);
  }

  double ElectricLoadCenterStorageSimple_Impl::nominalEnergeticEfficiencyforCharging() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterStorageSimple_Impl::isNominalEnergeticEfficiencyforChargingDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging);
  }

  double ElectricLoadCenterStorageSimple_Impl::nominalDischargingEnergeticEfficiency() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterStorageSimple_Impl::isNominalDischargingEnergeticEfficiencyDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency);
  }

  double ElectricLoadCenterStorageSimple_Impl::maximumStorageCapacity() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ElectricLoadCenterStorageSimple_Impl::maximumPowerforDischarging() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ElectricLoadCenterStorageSimple_Impl::maximumPowerforCharging() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ElectricLoadCenterStorageSimple_Impl::initialStateofCharge() const {
    return getDouble(OS_ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge,true);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName,
                              "ElectricLoadCenterStorageSimple",
                              "Availability",
                              schedule);
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setZone(const boost::optional<Zone>& zone) {
    bool result(false);
    if (zone) {
      result = setPointer(OS_ElectricLoadCenter_Storage_SimpleFields::ZoneName, zone.get().handle());
    }
    else {
      resetZone();
      result = true;
    }
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::resetZone() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains, radiativeFractionforZoneHeatGains);
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::resetRadiativeFractionforZoneHeatGains() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging, nominalEnergeticEfficiencyforCharging);
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::resetNominalEnergeticEfficiencyforCharging() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency, nominalDischargingEnergeticEfficiency);
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::resetNominalDischargingEnergeticEfficiency() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency, "");
    OS_ASSERT(result);
  }

  void ElectricLoadCenterStorageSimple_Impl::setMaximumStorageCapacity(double maximumStorageCapacity) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity, maximumStorageCapacity);
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageSimple_Impl::setMaximumPowerforDischarging(double maximumPowerforDischarging) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging, maximumPowerforDischarging);
    return result;
  }

  bool ElectricLoadCenterStorageSimple_Impl::setMaximumPowerforCharging(double maximumPowerforCharging) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging, maximumPowerforCharging);
    return result;
  }

  void ElectricLoadCenterStorageSimple_Impl::setInitialStateofCharge(boost::optional<double> initialStateofCharge) {
    bool result(false);
    if (initialStateofCharge) {
      result = setDouble(OS_ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge, initialStateofCharge.get());
    }
    else {
      resetInitialStateofCharge();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ElectricLoadCenterStorageSimple_Impl::resetInitialStateofCharge() {
    bool result = setString(OS_ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge, "");
    OS_ASSERT(result);
  }

} // detail

ElectricLoadCenterStorageSimple::ElectricLoadCenterStorageSimple(const Model& model)
  : ParentObject(ElectricLoadCenterStorageSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricLoadCenterStorageSimple_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // setMaximumStorageCapacity();
  // ok = setMaximumPowerforDischarging();
  OS_ASSERT(ok);
  // ok = setMaximumPowerforCharging();
  OS_ASSERT(ok);
}

IddObjectType ElectricLoadCenterStorageSimple::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Storage_Simple);
}

boost::optional<Schedule> ElectricLoadCenterStorageSimple::availabilitySchedule() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->availabilitySchedule();
}

boost::optional<Zone> ElectricLoadCenterStorageSimple::zone() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->zone();
}

double ElectricLoadCenterStorageSimple::radiativeFractionforZoneHeatGains() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->radiativeFractionforZoneHeatGains();
}

bool ElectricLoadCenterStorageSimple::isRadiativeFractionforZoneHeatGainsDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->isRadiativeFractionforZoneHeatGainsDefaulted();
}

double ElectricLoadCenterStorageSimple::nominalEnergeticEfficiencyforCharging() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->nominalEnergeticEfficiencyforCharging();
}

bool ElectricLoadCenterStorageSimple::isNominalEnergeticEfficiencyforChargingDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->isNominalEnergeticEfficiencyforChargingDefaulted();
}

double ElectricLoadCenterStorageSimple::nominalDischargingEnergeticEfficiency() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->nominalDischargingEnergeticEfficiency();
}

bool ElectricLoadCenterStorageSimple::isNominalDischargingEnergeticEfficiencyDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->isNominalDischargingEnergeticEfficiencyDefaulted();
}

double ElectricLoadCenterStorageSimple::maximumStorageCapacity() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->maximumStorageCapacity();
}

double ElectricLoadCenterStorageSimple::maximumPowerforDischarging() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->maximumPowerforDischarging();
}

double ElectricLoadCenterStorageSimple::maximumPowerforCharging() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->maximumPowerforCharging();
}

boost::optional<double> ElectricLoadCenterStorageSimple::initialStateofCharge() const {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->initialStateofCharge();
}

bool ElectricLoadCenterStorageSimple::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setAvailabilitySchedule(schedule);
}

void ElectricLoadCenterStorageSimple::resetAvailabilitySchedule() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetAvailabilitySchedule();
}

bool ElectricLoadCenterStorageSimple::setZone(const Zone& zone) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setZone(zone);
}

void ElectricLoadCenterStorageSimple::resetZone() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetZone();
}

bool ElectricLoadCenterStorageSimple::setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setRadiativeFractionforZoneHeatGains(radiativeFractionforZoneHeatGains);
}

void ElectricLoadCenterStorageSimple::resetRadiativeFractionforZoneHeatGains() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetRadiativeFractionforZoneHeatGains();
}

bool ElectricLoadCenterStorageSimple::setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setNominalEnergeticEfficiencyforCharging(nominalEnergeticEfficiencyforCharging);
}

void ElectricLoadCenterStorageSimple::resetNominalEnergeticEfficiencyforCharging() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetNominalEnergeticEfficiencyforCharging();
}

bool ElectricLoadCenterStorageSimple::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setNominalDischargingEnergeticEfficiency(nominalDischargingEnergeticEfficiency);
}

void ElectricLoadCenterStorageSimple::resetNominalDischargingEnergeticEfficiency() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetNominalDischargingEnergeticEfficiency();
}

void ElectricLoadCenterStorageSimple::setMaximumStorageCapacity(double maximumStorageCapacity) {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setMaximumStorageCapacity(maximumStorageCapacity);
}

bool ElectricLoadCenterStorageSimple::setMaximumPowerforDischarging(double maximumPowerforDischarging) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setMaximumPowerforDischarging(maximumPowerforDischarging);
}

bool ElectricLoadCenterStorageSimple::setMaximumPowerforCharging(double maximumPowerforCharging) {
  return getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setMaximumPowerforCharging(maximumPowerforCharging);
}

void ElectricLoadCenterStorageSimple::setInitialStateofCharge(double initialStateofCharge) {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->setInitialStateofCharge(initialStateofCharge);
}

void ElectricLoadCenterStorageSimple::resetInitialStateofCharge() {
  getImpl<detail::ElectricLoadCenterStorageSimple_Impl>()->resetInitialStateofCharge();
}

/// @cond
ElectricLoadCenterStorageSimple::ElectricLoadCenterStorageSimple(std::shared_ptr<detail::ElectricLoadCenterStorageSimple_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

