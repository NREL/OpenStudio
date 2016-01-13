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

#include "HumidifierSteamElectric.hpp"
#include "HumidifierSteamElectric_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_Humidifier_Steam_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HumidifierSteamElectric::iddObjectType());
  }

  HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HumidifierSteamElectric::iddObjectType());
  }

  HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const HumidifierSteamElectric_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HumidifierSteamElectric_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HumidifierSteamElectric_Impl::iddObjectType() const {
    return HumidifierSteamElectric::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HumidifierSteamElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("HumidifierSteamElectric","Availability"));
    }
    return result;
  }

  unsigned HumidifierSteamElectric_Impl::inletPort()
  {
    return OS_Humidifier_Steam_ElectricFields::AirInletNodeName;
  }

  unsigned HumidifierSteamElectric_Impl::outletPort()
  {
    return OS_Humidifier_Steam_ElectricFields::AirOutletNodeName;
  }

  boost::optional<Schedule> HumidifierSteamElectric_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName);
  }

  boost::optional<double> HumidifierSteamElectric_Impl::ratedPower() const {
    return getDouble(OS_Humidifier_Steam_ElectricFields::RatedPower,true);
  }

  bool HumidifierSteamElectric_Impl::isRatedPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Humidifier_Steam_ElectricFields::RatedPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> HumidifierSteamElectric_Impl::ratedFanPower() const {
    return getDouble(OS_Humidifier_Steam_ElectricFields::RatedFanPower,true);
  }

  boost::optional<double> HumidifierSteamElectric_Impl::standbyPower() const {
    return getDouble(OS_Humidifier_Steam_ElectricFields::StandbyPower,true);
  }

  // boost::optional<WaterStorageTank> HumidifierSteamElectric_Impl::waterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName);
  // }

  bool HumidifierSteamElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName,
                              "HumidifierSteamElectric",
                              "Availability",
                              schedule);
    return result;
  }

  void HumidifierSteamElectric_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool HumidifierSteamElectric_Impl::setRatedPower(boost::optional<double> ratedPower) {
    bool result(false);
    if (ratedPower) {
      result = setDouble(OS_Humidifier_Steam_ElectricFields::RatedPower, ratedPower.get());
    }
    else {
      resetRatedPower();
      result = true;
    }
    return result;
  }

  void HumidifierSteamElectric_Impl::resetRatedPower() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::RatedPower, "");
    OS_ASSERT(result);
  }

  void HumidifierSteamElectric_Impl::autosizeRatedPower() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::RatedPower, "autosize");
    OS_ASSERT(result);
  }

  bool HumidifierSteamElectric_Impl::setRatedFanPower(boost::optional<double> ratedFanPower) {
    bool result(false);
    if (ratedFanPower) {
      result = setDouble(OS_Humidifier_Steam_ElectricFields::RatedFanPower, ratedFanPower.get());
    }
    else {
      resetRatedFanPower();
      result = true;
    }
    return result;
  }

  void HumidifierSteamElectric_Impl::resetRatedFanPower() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::RatedFanPower, "");
    OS_ASSERT(result);
  }

  bool HumidifierSteamElectric_Impl::setStandbyPower(boost::optional<double> standbyPower) {
    bool result(false);
    if (standbyPower) {
      result = setDouble(OS_Humidifier_Steam_ElectricFields::StandbyPower, standbyPower.get());
    }
    else {
      resetStandbyPower();
      result = true;
    }
    return result;
  }

  void HumidifierSteamElectric_Impl::resetStandbyPower() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::StandbyPower, "");
    OS_ASSERT(result);
  }

  // bool HumidifierSteamElectric_Impl::setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void HumidifierSteamElectric_Impl::resetWaterStorageTank() {
  //   bool result = setString(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  boost::optional<double> HumidifierSteamElectric_Impl::ratedCapacity() const {
    return getDouble(OS_Humidifier_Steam_ElectricFields::RatedCapacity,true);
  }

  bool HumidifierSteamElectric_Impl::isRatedCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Humidifier_Steam_ElectricFields::RatedCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool HumidifierSteamElectric_Impl::setRatedCapacity(boost::optional<double> ratedCapacity) {
    bool result(false);
    if (ratedCapacity) {
      result = setDouble(OS_Humidifier_Steam_ElectricFields::RatedCapacity, ratedCapacity.get());
    }
    return result;
  }

  void HumidifierSteamElectric_Impl::autosizeRatedCapacity() {
    bool result = setString(OS_Humidifier_Steam_ElectricFields::RatedCapacity, "autosize");
    OS_ASSERT(result);
  }

} // detail

HumidifierSteamElectric::HumidifierSteamElectric(const Model& model)
  : StraightComponent(HumidifierSteamElectric::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HumidifierSteamElectric_Impl>());
  autosizeRatedCapacity();
  setRatedPower(104000);
  setString(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName,"");
}

IddObjectType HumidifierSteamElectric::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Humidifier_Steam_Electric);
}

boost::optional<Schedule> HumidifierSteamElectric::availabilitySchedule() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->availabilitySchedule();
}

boost::optional<double> HumidifierSteamElectric::ratedPower() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->ratedPower();
}

bool HumidifierSteamElectric::isRatedPowerAutosized() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->isRatedPowerAutosized();
}

boost::optional<double> HumidifierSteamElectric::ratedFanPower() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->ratedFanPower();
}

boost::optional<double> HumidifierSteamElectric::standbyPower() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->standbyPower();
}

// boost::optional<WaterStorageTank> HumidifierSteamElectric::waterStorageTank() const {
//   return getImpl<detail::HumidifierSteamElectric_Impl>()->waterStorageTank();
// }

bool HumidifierSteamElectric::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->setAvailabilitySchedule(schedule);
}

void HumidifierSteamElectric::resetAvailabilitySchedule() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->resetAvailabilitySchedule();
}

bool HumidifierSteamElectric::setRatedPower(double ratedPower) {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->setRatedPower(ratedPower);
}

void HumidifierSteamElectric::resetRatedPower() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->resetRatedPower();
}

void HumidifierSteamElectric::autosizeRatedPower() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->autosizeRatedPower();
}

bool HumidifierSteamElectric::setRatedFanPower(double ratedFanPower) {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->setRatedFanPower(ratedFanPower);
}

void HumidifierSteamElectric::resetRatedFanPower() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->resetRatedFanPower();
}

bool HumidifierSteamElectric::setStandbyPower(double standbyPower) {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->setStandbyPower(standbyPower);
}

void HumidifierSteamElectric::resetStandbyPower() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->resetStandbyPower();
}

boost::optional<double> HumidifierSteamElectric::ratedCapacity() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->ratedCapacity();
}

bool HumidifierSteamElectric::isRatedCapacityAutosized() const {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->isRatedCapacityAutosized();
}

bool HumidifierSteamElectric::setRatedCapacity(double ratedCapacity) {
  return getImpl<detail::HumidifierSteamElectric_Impl>()->setRatedCapacity(ratedCapacity);
}

void HumidifierSteamElectric::autosizeRatedCapacity() {
  getImpl<detail::HumidifierSteamElectric_Impl>()->autosizeRatedCapacity();
}

// bool HumidifierSteamElectric::setWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::HumidifierSteamElectric_Impl>()->setWaterStorageTank(waterStorageTank);
// }

// void HumidifierSteamElectric::resetWaterStorageTank() {
//   getImpl<detail::HumidifierSteamElectric_Impl>()->resetWaterStorageTank();
// }

/// @cond
HumidifierSteamElectric::HumidifierSteamElectric(std::shared_ptr<detail::HumidifierSteamElectric_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

