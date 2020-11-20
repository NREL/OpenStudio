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

    HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HumidifierSteamElectric::iddObjectType());
    }

    HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HumidifierSteamElectric::iddObjectType());
    }

    HumidifierSteamElectric_Impl::HumidifierSteamElectric_Impl(const HumidifierSteamElectric_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HumidifierSteamElectric_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Humidifier Water Volume Flow Rate", "Humidifier Water Volume", "Humidifier Electricity Rate", "Humidifier Electricity Energy",
        "Humidifier Mains Water Volume"

        // Water Storage Tank Name isn't implemented
        // https://github.com/NREL/EnergyPlus/blob/a5b04d73ecfef706cda7d0543ccad31e7c27087a/src/EnergyPlus/Humidifiers.cc#L471
        //"Humidifier Storage Tank Water Volume Flow Rate",
        //"Humidifier Storage Tank Water Volume",
        //"Humidifier Starved Storage Tank Water Volume Flow Rate",
        //"Humidifier Starved Storage Tank Water Volume"
      };
      return result;
    }

    IddObjectType HumidifierSteamElectric_Impl::iddObjectType() const {
      return HumidifierSteamElectric::iddObjectType();
    }

    std::vector<ScheduleTypeKey> HumidifierSteamElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("HumidifierSteamElectric", "Availability"));
      }
      return result;
    }

    unsigned HumidifierSteamElectric_Impl::inletPort() const {
      return OS_Humidifier_Steam_ElectricFields::AirInletNodeName;
    }

    unsigned HumidifierSteamElectric_Impl::outletPort() const {
      return OS_Humidifier_Steam_ElectricFields::AirOutletNodeName;
    }

    boost::optional<Schedule> HumidifierSteamElectric_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName);
    }

    boost::optional<double> HumidifierSteamElectric_Impl::ratedPower() const {
      return getDouble(OS_Humidifier_Steam_ElectricFields::RatedPower, true);
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
      return getDouble(OS_Humidifier_Steam_ElectricFields::RatedFanPower, true);
    }

    boost::optional<double> HumidifierSteamElectric_Impl::standbyPower() const {
      return getDouble(OS_Humidifier_Steam_ElectricFields::StandbyPower, true);
    }

    // boost::optional<WaterStorageTank> HumidifierSteamElectric_Impl::waterStorageTank() const {
    //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName);
    // }

    bool HumidifierSteamElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Humidifier_Steam_ElectricFields::AvailabilityScheduleName, "HumidifierSteamElectric", "Availability", schedule);
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
      } else {
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
      } else {
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
      } else {
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
      return getDouble(OS_Humidifier_Steam_ElectricFields::RatedCapacity, true);
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

    boost::optional<double> HumidifierSteamElectric_Impl::autosizedRatedCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity Volume", "m3/s");
    }

    boost::optional<double> HumidifierSteamElectric_Impl::autosizedRatedPower() const {
      return getAutosizedValue("Design Size Rated Power", "W");
    }

    void HumidifierSteamElectric_Impl::autosize() {
      autosizeRatedCapacity();
      autosizeRatedPower();
    }

    void HumidifierSteamElectric_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedCapacity();
      if (val) {
        setRatedCapacity(val.get());
      }

      val = autosizedRatedPower();
      if (val) {
        setRatedPower(val.get());
      }
    }

  }  // namespace detail

  HumidifierSteamElectric::HumidifierSteamElectric(const Model& model) : StraightComponent(HumidifierSteamElectric::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HumidifierSteamElectric_Impl>());
    autosizeRatedCapacity();
    setRatedPower(104000);
    setString(OS_Humidifier_Steam_ElectricFields::WaterStorageTankName, "");
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
  HumidifierSteamElectric::HumidifierSteamElectric(std::shared_ptr<detail::HumidifierSteamElectric_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> HumidifierSteamElectric::autosizedRatedCapacity() const {
    return getImpl<detail::HumidifierSteamElectric_Impl>()->autosizedRatedCapacity();
  }

  boost::optional<double> HumidifierSteamElectric::autosizedRatedPower() const {
    return getImpl<detail::HumidifierSteamElectric_Impl>()->autosizedRatedPower();
  }

}  // namespace model
}  // namespace openstudio
