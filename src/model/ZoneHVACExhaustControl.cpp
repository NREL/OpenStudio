/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ZoneHVACExhaustControl.hpp"
#include "ZoneHVACExhaustControl_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneHVAC_ExhaustControl_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACExhaustControl_Impl::ZoneHVACExhaustControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACExhaustControl::iddObjectType());
    }

    ZoneHVACExhaustControl_Impl::ZoneHVACExhaustControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACExhaustControl::iddObjectType());
    }

    ZoneHVACExhaustControl_Impl::ZoneHVACExhaustControl_Impl(const ZoneHVACExhaustControl_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACExhaustControl_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ZoneHVACExhaustControl_Impl::iddObjectType() const {
      return ZoneHVACExhaustControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACExhaustControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_ExhaustControlFields::AvailabilityScheduleName) != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACExhaustControl", "Availability");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACExhaustControl", "Exhaust Flow Fraction");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACExhaustControl", "Minimum Zone Temperature Limit");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACExhaustControl", "Minimum Exhaust Flow Fraction");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACExhaustControl", "Balanced Exhaust Fraction");
      }
      return result;
    }

    unsigned ZoneHVACExhaustControl_Impl::inletPort() const {
      return OS_ZoneHVAC_ExhaustControlFields::InletNodeName;
    }

    unsigned ZoneHVACExhaustControl_Impl::outletPort() const {
      return OS_ZoneHVAC_ExhaustControlFields::OutletNodeName;
    }

    ComponentType ZoneHVACExhaustControl_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> ZoneHVACExhaustControl_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ZoneHVACExhaustControl_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ZoneHVACExhaustControl_Impl::appGHeatingFuelTypes() const {
      return {};
    }

    Schedule ZoneHVACExhaustControl_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<ThermalZone> ZoneHVACExhaustControl_Impl::thermalZone() const {
      auto thisObject = this->getObject<ModelObject>();
      std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
      for (const auto& thermalZone : thermalZones) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          return thermalZone;
        }
      }
      return boost::none;
    }

    boost::optional<double> ZoneHVACExhaustControl_Impl::designExhaustFlowRate() const {
      return getDouble(OS_ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, true);
    }

    bool ZoneHVACExhaustControl_Impl::isDesignExhaustFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACExhaustControl_Impl::autosizedDesignExhaustFlowRate() {
      return getAutosizedValue("TODO_CHECK_SQL Design Exhaust Flow Rate", "m3/s");
    }

    std::string ZoneHVACExhaustControl_Impl::flowControlType() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_ExhaustControlFields::FlowControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneHVACExhaustControl_Impl::exhaustFlowFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName);
    }

    boost::optional<Schedule> ZoneHVACExhaustControl_Impl::minimumZoneTemperatureLimitSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName);
    }

    boost::optional<Schedule> ZoneHVACExhaustControl_Impl::minimumExhaustFlowFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName);
    }

    boost::optional<Schedule> ZoneHVACExhaustControl_Impl::balancedExhaustFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName);
    }

    bool ZoneHVACExhaustControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_ZoneHVAC_ExhaustControlFields::AvailabilityScheduleName, "ZoneHVACExhaustControl", "Availability", schedule);
      return result;
    }

    bool ZoneHVACExhaustControl_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m) {
        return false;
      }

      if (thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneHVACExhaustControl_Impl::removeFromThermalZone() {
      if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    bool ZoneHVACExhaustControl_Impl::setDesignExhaustFlowRate(double designExhaustFlowRate) {
      const bool result = setDouble(OS_ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, designExhaustFlowRate);
      return result;
    }

    void ZoneHVACExhaustControl_Impl::autosizeDesignExhaustFlowRate() {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACExhaustControl_Impl::setFlowControlType(const std::string& flowControlType) {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::FlowControlType, flowControlType);
      return result;
    }

    bool ZoneHVACExhaustControl_Impl::setExhaustFlowFractionSchedule(Schedule& schedule) {
      const bool result =
        setSchedule(OS_ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName, "ZoneHVACExhaustControl", "Exhaust Flow Fraction", schedule);
      return result;
    }

    void ZoneHVACExhaustControl_Impl::resetExhaustFlowFractionSchedule() {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACExhaustControl_Impl::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName, "ZoneHVACExhaustControl",
                                      "Minimum Zone Temperature Limit", schedule);
      return result;
    }

    void ZoneHVACExhaustControl_Impl::resetMinimumZoneTemperatureLimitSchedule() {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACExhaustControl_Impl::setMinimumExhaustFlowFractionSchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName, "ZoneHVACExhaustControl",
                                      "Minimum Exhaust Flow Fraction", schedule);
      return result;
    }

    void ZoneHVACExhaustControl_Impl::resetMinimumExhaustFlowFractionSchedule() {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACExhaustControl_Impl::setBalancedExhaustFractionSchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName, "ZoneHVACExhaustControl",
                                      "Balanced Exhaust Fraction", schedule);
      return result;
    }

    void ZoneHVACExhaustControl_Impl::resetBalancedExhaustFractionSchedule() {
      const bool result = setString(OS_ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName, "");
      OS_ASSERT(result);
    }

    void ZoneHVACExhaustControl_Impl::autosize() {
      autosizeDesignExhaustFlowRate();
    }

    void ZoneHVACExhaustControl_Impl::applySizingValues() {
      if (boost::optional<double> val_ = autosizedDesignExhaustFlowRate()) {
        setDesignExhaustFlowRate(*val_);
      }
    }

    boost::optional<Schedule> ZoneHVACExhaustControl_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_ExhaustControlFields::AvailabilityScheduleName);
    }

  }  // namespace detail

  ZoneHVACExhaustControl::ZoneHVACExhaustControl(const Model& model) : ZoneHVACComponent(ZoneHVACExhaustControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACExhaustControl_Impl>());

    bool ok = true;

    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);

    autosizeDesignExhaustFlowRate();

    ok = setFlowControlType("Scheduled");
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACExhaustControl::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_ExhaustControl};
  }

  std::vector<std::string> ZoneHVACExhaustControl::flowControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneHVAC_ExhaustControlFields::FlowControlType);
  }

  Schedule ZoneHVACExhaustControl::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->availabilitySchedule();
  }

  boost::optional<ThermalZone> ZoneHVACExhaustControl::thermalZone() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->thermalZone();
  }

  boost::optional<double> ZoneHVACExhaustControl::designExhaustFlowRate() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->designExhaustFlowRate();
  }

  bool ZoneHVACExhaustControl::isDesignExhaustFlowRateAutosized() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->isDesignExhaustFlowRateAutosized();
  }

  boost::optional<double> ZoneHVACExhaustControl::autosizedDesignExhaustFlowRate() {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->autosizedDesignExhaustFlowRate();
  }

  std::string ZoneHVACExhaustControl::flowControlType() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->flowControlType();
  }

  boost::optional<Schedule> ZoneHVACExhaustControl::exhaustFlowFractionSchedule() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->exhaustFlowFractionSchedule();
  }

  boost::optional<Schedule> ZoneHVACExhaustControl::minimumZoneTemperatureLimitSchedule() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->minimumZoneTemperatureLimitSchedule();
  }

  boost::optional<Schedule> ZoneHVACExhaustControl::minimumExhaustFlowFractionSchedule() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->minimumExhaustFlowFractionSchedule();
  }

  boost::optional<Schedule> ZoneHVACExhaustControl::balancedExhaustFractionSchedule() const {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->balancedExhaustFractionSchedule();
  }

  bool ZoneHVACExhaustControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACExhaustControl::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACExhaustControl::removeFromThermalZone() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->removeFromThermalZone();
  }

  bool ZoneHVACExhaustControl::setDesignExhaustFlowRate(double designExhaustFlowRate) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setDesignExhaustFlowRate(designExhaustFlowRate);
  }

  void ZoneHVACExhaustControl::autosizeDesignExhaustFlowRate() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->autosizeDesignExhaustFlowRate();
  }

  bool ZoneHVACExhaustControl::setFlowControlType(const std::string& flowControlType) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setFlowControlType(flowControlType);
  }

  bool ZoneHVACExhaustControl::setExhaustFlowFractionSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setExhaustFlowFractionSchedule(schedule);
  }

  void ZoneHVACExhaustControl::resetExhaustFlowFractionSchedule() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->resetExhaustFlowFractionSchedule();
  }

  bool ZoneHVACExhaustControl::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setMinimumZoneTemperatureLimitSchedule(schedule);
  }

  void ZoneHVACExhaustControl::resetMinimumZoneTemperatureLimitSchedule() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->resetMinimumZoneTemperatureLimitSchedule();
  }

  bool ZoneHVACExhaustControl::setMinimumExhaustFlowFractionSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setMinimumExhaustFlowFractionSchedule(schedule);
  }

  void ZoneHVACExhaustControl::resetMinimumExhaustFlowFractionSchedule() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->resetMinimumExhaustFlowFractionSchedule();
  }

  bool ZoneHVACExhaustControl::setBalancedExhaustFractionSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACExhaustControl_Impl>()->setBalancedExhaustFractionSchedule(schedule);
  }

  void ZoneHVACExhaustControl::resetBalancedExhaustFractionSchedule() {
    getImpl<detail::ZoneHVACExhaustControl_Impl>()->resetBalancedExhaustFractionSchedule();
  }

  /// @cond
  ZoneHVACExhaustControl::ZoneHVACExhaustControl(std::shared_ptr<detail::ZoneHVACExhaustControl_Impl> impl) : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
