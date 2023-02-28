/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HumidifierSteamGas.hpp"
#include "HumidifierSteamGas_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include <utilities/idd/OS_Humidifier_Steam_Gas_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HumidifierSteamGas_Impl::HumidifierSteamGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HumidifierSteamGas::iddObjectType());
    }

    HumidifierSteamGas_Impl::HumidifierSteamGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HumidifierSteamGas::iddObjectType());
    }

    HumidifierSteamGas_Impl::HumidifierSteamGas_Impl(const HumidifierSteamGas_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HumidifierSteamGas_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Humidifier Water Volume Flow Rate",
        "Humidifier Water Volume",
        "Humidifier NaturalGas Use Thermal Efficiency",
        "Humidifier NaturalGas Rate",
        "Humidifier NaturalGas Energy",
        "Humidifier Auxiliary Electricity Rate",
        "Humidifier Auxiliary Electricity Energy",
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

    IddObjectType HumidifierSteamGas_Impl::iddObjectType() const {
      return HumidifierSteamGas::iddObjectType();
    }

    std::vector<ScheduleTypeKey> HumidifierSteamGas_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Humidifier_Steam_GasFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("HumidifierSteamGas", "Availability"));
      }
      return result;
    }

    unsigned HumidifierSteamGas_Impl::inletPort() const {
      return OS_Humidifier_Steam_GasFields::AirInletNodeName;
    }

    unsigned HumidifierSteamGas_Impl::outletPort() const {
      return OS_Humidifier_Steam_GasFields::AirOutletNodeName;
    }

    boost::optional<Schedule> HumidifierSteamGas_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Humidifier_Steam_GasFields::AvailabilityScheduleName);
    }

    boost::optional<double> HumidifierSteamGas_Impl::ratedCapacity() const {
      return getDouble(OS_Humidifier_Steam_GasFields::RatedCapacity, true);
    }

    bool HumidifierSteamGas_Impl::isRatedCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Humidifier_Steam_GasFields::RatedCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HumidifierSteamGas_Impl::ratedGasUseRate() const {
      return getDouble(OS_Humidifier_Steam_GasFields::RatedGasUseRate, true);
    }

    bool HumidifierSteamGas_Impl::isRatedGasUseRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Humidifier_Steam_GasFields::RatedGasUseRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double HumidifierSteamGas_Impl::thermalEfficiency() const {
      boost::optional<double> value = getDouble(OS_Humidifier_Steam_GasFields::ThermalEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HumidifierSteamGas_Impl::isThermalEfficiencyDefaulted() const {
      return isEmpty(OS_Humidifier_Steam_GasFields::ThermalEfficiency);
    }

    boost::optional<Curve> HumidifierSteamGas_Impl::thermalEfficiencyModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Humidifier_Steam_GasFields::ThermalEfficiencyModifierCurveName);
    }

    boost::optional<double> HumidifierSteamGas_Impl::ratedFanPower() const {
      return getDouble(OS_Humidifier_Steam_GasFields::RatedFanPower, true);
    }

    double HumidifierSteamGas_Impl::auxiliaryElectricPower() const {
      boost::optional<double> value = getDouble(OS_Humidifier_Steam_GasFields::AuxiliaryElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HumidifierSteamGas_Impl::isAuxiliaryElectricPowerDefaulted() const {
      return isEmpty(OS_Humidifier_Steam_GasFields::AuxiliaryElectricPower);
    }

    // boost::optional<WaterStorageTank> HumidifierSteamGas_Impl::waterStorageTank() const {
    //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Humidifier_Steam_GasFields::WaterStorageTankName);
    // }

    std::string HumidifierSteamGas_Impl::inletWaterTemperatureOption() const {
      boost::optional<std::string> value = getString(OS_Humidifier_Steam_GasFields::InletWaterTemperatureOption, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HumidifierSteamGas_Impl::isInletWaterTemperatureOptionDefaulted() const {
      return isEmpty(OS_Humidifier_Steam_GasFields::InletWaterTemperatureOption);
    }

    bool HumidifierSteamGas_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Humidifier_Steam_GasFields::AvailabilityScheduleName, "HumidifierSteamGas", "Availability", schedule);
      return result;
    }

    void HumidifierSteamGas_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_Humidifier_Steam_GasFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setRatedCapacity(boost::optional<double> ratedCapacity) {
      bool result(false);
      if (ratedCapacity) {
        result = setDouble(OS_Humidifier_Steam_GasFields::RatedCapacity, ratedCapacity.get());
      }
      return result;
    }

    void HumidifierSteamGas_Impl::autosizeRatedCapacity() {
      bool result = setString(OS_Humidifier_Steam_GasFields::RatedCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setRatedGasUseRate(boost::optional<double> ratedGasUseRate) {
      bool result(false);
      if (ratedGasUseRate) {
        result = setDouble(OS_Humidifier_Steam_GasFields::RatedGasUseRate, ratedGasUseRate.get());
      } else {
        resetRatedGasUseRate();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetRatedGasUseRate() {
      bool result = setString(OS_Humidifier_Steam_GasFields::RatedGasUseRate, "");
      OS_ASSERT(result);
    }

    void HumidifierSteamGas_Impl::autosizeRatedGasUseRate() {
      bool result = setString(OS_Humidifier_Steam_GasFields::RatedGasUseRate, "autosize");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setThermalEfficiency(boost::optional<double> thermalEfficiency) {
      bool result(false);
      if (thermalEfficiency) {
        result = setDouble(OS_Humidifier_Steam_GasFields::ThermalEfficiency, thermalEfficiency.get());
      } else {
        resetThermalEfficiency();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetThermalEfficiency() {
      bool result = setString(OS_Humidifier_Steam_GasFields::ThermalEfficiency, "");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setThermalEfficiencyModifierCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_Humidifier_Steam_GasFields::ThermalEfficiencyModifierCurveName, curve.get().handle());
      } else {
        resetThermalEfficiencyModifierCurve();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetThermalEfficiencyModifierCurve() {
      bool result = setString(OS_Humidifier_Steam_GasFields::ThermalEfficiencyModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setRatedFanPower(boost::optional<double> ratedFanPower) {
      bool result(false);
      if (ratedFanPower) {
        result = setDouble(OS_Humidifier_Steam_GasFields::RatedFanPower, ratedFanPower.get());
      } else {
        resetRatedFanPower();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetRatedFanPower() {
      bool result = setString(OS_Humidifier_Steam_GasFields::RatedFanPower, "");
      OS_ASSERT(result);
    }

    bool HumidifierSteamGas_Impl::setAuxiliaryElectricPower(boost::optional<double> auxiliaryElectricPower) {
      bool result(false);
      if (auxiliaryElectricPower) {
        result = setDouble(OS_Humidifier_Steam_GasFields::AuxiliaryElectricPower, auxiliaryElectricPower.get());
      } else {
        resetAuxiliaryElectricPower();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetAuxiliaryElectricPower() {
      bool result = setString(OS_Humidifier_Steam_GasFields::AuxiliaryElectricPower, "");
      OS_ASSERT(result);
    }

    // bool HumidifierSteamGas_Impl::setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
    //   bool result(false);
    //   if (waterStorageTank) {
    //     result = setPointer(OS_Humidifier_Steam_GasFields::WaterStorageTankName, waterStorageTank.get().handle());
    //   }
    //   else {
    //     resetWaterStorageTank();
    //     result = true;
    //   }
    //   return result;
    // }

    // void HumidifierSteamGas_Impl::resetWaterStorageTank() {
    //   bool result = setString(OS_Humidifier_Steam_GasFields::WaterStorageTankName, "");
    //   OS_ASSERT(result);
    // }

    bool HumidifierSteamGas_Impl::setInletWaterTemperatureOption(boost::optional<std::string> inletWaterTemperatureOption) {
      bool result(false);
      if (inletWaterTemperatureOption) {
        result = setString(OS_Humidifier_Steam_GasFields::InletWaterTemperatureOption, inletWaterTemperatureOption.get());
      } else {
        resetInletWaterTemperatureOption();
        result = true;
      }
      return result;
    }

    void HumidifierSteamGas_Impl::resetInletWaterTemperatureOption() {
      bool result = setString(OS_Humidifier_Steam_GasFields::InletWaterTemperatureOption, "");
      OS_ASSERT(result);
    }

    boost::optional<double> HumidifierSteamGas_Impl::autosizedRatedCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity Volume", "m3/s");
    }

    boost::optional<double> HumidifierSteamGas_Impl::autosizedRatedGasUseRate() const {
      return getAutosizedValue("Design Size Rated Power", "W");
    }

    void HumidifierSteamGas_Impl::autosize() {
      autosizeRatedCapacity();
      autosizeRatedGasUseRate();
    }

    void HumidifierSteamGas_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedCapacity();
      if (val) {
        setRatedCapacity(val.get());
      }

      val = autosizedRatedGasUseRate();
      if (val) {
        setRatedGasUseRate(val.get());
      }
    }

  }  // namespace detail

  HumidifierSteamGas::HumidifierSteamGas(const Model& model) : StraightComponent(HumidifierSteamGas::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HumidifierSteamGas_Impl>());
    autosizeRatedCapacity();
    setRatedGasUseRate(104000);  // JJR: per idf example in docs
    setString(OS_Humidifier_Steam_GasFields::WaterStorageTankName, "");
  }

  IddObjectType HumidifierSteamGas::iddObjectType() {
    return {IddObjectType::OS_Humidifier_Steam_Gas};
  }

  boost::optional<Schedule> HumidifierSteamGas::availabilitySchedule() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->availabilitySchedule();
  }

  boost::optional<double> HumidifierSteamGas::ratedCapacity() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->ratedCapacity();
  }

  bool HumidifierSteamGas::isRatedCapacityAutosized() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->isRatedCapacityAutosized();
  }

  boost::optional<double> HumidifierSteamGas::ratedGasUseRate() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->ratedGasUseRate();
  }

  bool HumidifierSteamGas::isRatedGasUseRateAutosized() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->isRatedGasUseRateAutosized();
  }

  double HumidifierSteamGas::thermalEfficiency() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->thermalEfficiency();
  }

  bool HumidifierSteamGas::isThermalEfficiencyDefaulted() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->isThermalEfficiencyDefaulted();
  }

  boost::optional<Curve> HumidifierSteamGas::thermalEfficiencyModifierCurve() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->thermalEfficiencyModifierCurve();
  }

  boost::optional<double> HumidifierSteamGas::ratedFanPower() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->ratedFanPower();
  }

  double HumidifierSteamGas::auxiliaryElectricPower() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->auxiliaryElectricPower();
  }

  bool HumidifierSteamGas::isAuxiliaryElectricPowerDefaulted() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->isAuxiliaryElectricPowerDefaulted();
  }

  // boost::optional<WaterStorageTank> HumidifierSteamGas::waterStorageTank() const {
  //   return getImpl<detail::HumidifierSteamGas_Impl>()->waterStorageTank();
  // }

  std::string HumidifierSteamGas::inletWaterTemperatureOption() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->inletWaterTemperatureOption();
  }

  bool HumidifierSteamGas::isInletWaterTemperatureOptionDefaulted() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->isInletWaterTemperatureOptionDefaulted();
  }

  bool HumidifierSteamGas::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setAvailabilitySchedule(schedule);
  }

  void HumidifierSteamGas::resetAvailabilitySchedule() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetAvailabilitySchedule();
  }

  bool HumidifierSteamGas::setRatedCapacity(double ratedCapacity) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setRatedCapacity(ratedCapacity);
  }

  void HumidifierSteamGas::autosizeRatedCapacity() {
    getImpl<detail::HumidifierSteamGas_Impl>()->autosizeRatedCapacity();
  }

  bool HumidifierSteamGas::setRatedGasUseRate(double ratedGasUseRate) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setRatedGasUseRate(ratedGasUseRate);
  }

  void HumidifierSteamGas::resetRatedGasUseRate() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetRatedGasUseRate();
  }

  void HumidifierSteamGas::autosizeRatedGasUseRate() {
    getImpl<detail::HumidifierSteamGas_Impl>()->autosizeRatedGasUseRate();
  }

  bool HumidifierSteamGas::setThermalEfficiency(double thermalEfficiency) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setThermalEfficiency(thermalEfficiency);
  }

  void HumidifierSteamGas::resetThermalEfficiency() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetThermalEfficiency();
  }

  bool HumidifierSteamGas::setThermalEfficiencyModifierCurve(const Curve& curve) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setThermalEfficiencyModifierCurve(curve);
  }

  void HumidifierSteamGas::resetThermalEfficiencyModifierCurve() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetThermalEfficiencyModifierCurve();
  }

  bool HumidifierSteamGas::setRatedFanPower(double ratedFanPower) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setRatedFanPower(ratedFanPower);
  }

  void HumidifierSteamGas::resetRatedFanPower() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetRatedFanPower();
  }

  bool HumidifierSteamGas::setAuxiliaryElectricPower(double auxiliaryElectricPower) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setAuxiliaryElectricPower(auxiliaryElectricPower);
  }

  void HumidifierSteamGas::resetAuxiliaryElectricPower() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetAuxiliaryElectricPower();
  }

  // bool HumidifierSteamGas::setWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  //   return getImpl<detail::HumidifierSteamGas_Impl>()->setWaterStorageTank(waterStorageTank);
  // }

  // void HumidifierSteamGas::resetWaterStorageTank() {
  //   getImpl<detail::HumidifierSteamGas_Impl>()->resetWaterStorageTank();
  // }

  bool HumidifierSteamGas::setInletWaterTemperatureOption(const std::string& inletWaterTemperatureOption) {
    return getImpl<detail::HumidifierSteamGas_Impl>()->setInletWaterTemperatureOption(inletWaterTemperatureOption);
  }

  void HumidifierSteamGas::resetInletWaterTemperatureOption() {
    getImpl<detail::HumidifierSteamGas_Impl>()->resetInletWaterTemperatureOption();
  }

  /// @cond
  HumidifierSteamGas::HumidifierSteamGas(std::shared_ptr<detail::HumidifierSteamGas_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> HumidifierSteamGas::autosizedRatedCapacity() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->autosizedRatedCapacity();
  }

  boost::optional<double> HumidifierSteamGas::autosizedRatedGasUseRate() const {
    return getImpl<detail::HumidifierSteamGas_Impl>()->autosizedRatedGasUseRate();
  }

}  // namespace model
}  // namespace openstudio
