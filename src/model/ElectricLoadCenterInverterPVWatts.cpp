/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ElectricLoadCenterInverterPVWatts.hpp"
#include "ElectricLoadCenterInverterPVWatts_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Inverter_PVWatts_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Inverter_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterInverterPVWatts::iddObjectType());
    }

    ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : Inverter_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricLoadCenterInverterPVWatts::iddObjectType());
    }

    ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const ElectricLoadCenterInverterPVWatts_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : Inverter_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricLoadCenterInverterPVWatts_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Inverter DC to AC Efficiency",        "Inverter DC Input Electricity Rate",        "Inverter DC Input Electricity Energy",
        "Inverter AC Output Electricity Rate", "Inverter AC Output Electricity Energy",     "Inverter Conversion Loss Power",
        "Inverter Conversion Loss Energy",     "Inverter Conversion Loss Decrement Energy", "Inverter Thermal Loss Rate",
        "Inverter Thermal Loss Energy",        "Inverter Ancillary AC Electricity Rate",    "Inverter Ancillary AC Electricity Energy"};
      return result;
    }

    IddObjectType ElectricLoadCenterInverterPVWatts_Impl::iddObjectType() const {
      return ElectricLoadCenterInverterPVWatts::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricLoadCenterInverterPVWatts_Impl::getScheduleTypeKeys(const Schedule& /*schedule*/) const {
      std::vector<ScheduleTypeKey> result;
      return result;
    }

    boost::optional<ThermalZone> ElectricLoadCenterInverterPVWatts_Impl::thermalZone() const {
      return boost::none;
    }

    bool ElectricLoadCenterInverterPVWatts_Impl::setThermalZone(const ThermalZone& /*thermalZone*/) {
      return false;
    }

    void ElectricLoadCenterInverterPVWatts_Impl::resetThermalZone() {
      bool result = true;
      OS_ASSERT(result);
    }

    double ElectricLoadCenterInverterPVWatts_Impl::dcToACSizeRatio() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterInverterPVWatts_Impl::isDCToACSizeRatioDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio);
    }

    double ElectricLoadCenterInverterPVWatts_Impl::inverterEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterInverterPVWatts_Impl::isInverterEfficiencyDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency);
    }

    bool ElectricLoadCenterInverterPVWatts_Impl::setDCToACSizeRatio(double dcToACSizeRatio) {
      bool result = setDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, dcToACSizeRatio);
      OS_ASSERT(result);
      return result;
    }

    void ElectricLoadCenterInverterPVWatts_Impl::resetDCToACSizeRatio() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterInverterPVWatts_Impl::setInverterEfficiency(double inverterEfficiency) {
      bool result = setDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, inverterEfficiency);
      OS_ASSERT(result);
      return result;
    }

    void ElectricLoadCenterInverterPVWatts_Impl::resetInverterEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ElectricLoadCenterInverterPVWatts::ElectricLoadCenterInverterPVWatts(const Model& model)
    : Inverter(ElectricLoadCenterInverterPVWatts::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType ElectricLoadCenterInverterPVWatts::iddObjectType() {
    return {IddObjectType::OS_ElectricLoadCenter_Inverter_PVWatts};
  }

  double ElectricLoadCenterInverterPVWatts::dcToACSizeRatio() const {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->dcToACSizeRatio();
  }

  bool ElectricLoadCenterInverterPVWatts::isDCToACSizeRatioDefaulted() const {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->isDCToACSizeRatioDefaulted();
  }

  double ElectricLoadCenterInverterPVWatts::inverterEfficiency() const {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->inverterEfficiency();
  }

  bool ElectricLoadCenterInverterPVWatts::isInverterEfficiencyDefaulted() const {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->isInverterEfficiencyDefaulted();
  }

  bool ElectricLoadCenterInverterPVWatts::setDCToACSizeRatio(double dcToACSizeRatio) {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->setDCToACSizeRatio(dcToACSizeRatio);
  }

  void ElectricLoadCenterInverterPVWatts::resetDCToACSizeRatio() {
    getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->resetDCToACSizeRatio();
  }

  bool ElectricLoadCenterInverterPVWatts::setInverterEfficiency(double inverterEfficiency) {
    return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->setInverterEfficiency(inverterEfficiency);
  }

  void ElectricLoadCenterInverterPVWatts::resetInverterEfficiency() {
    getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->resetInverterEfficiency();
  }

  /// @cond
  ElectricLoadCenterInverterPVWatts::ElectricLoadCenterInverterPVWatts(std::shared_ptr<detail::ElectricLoadCenterInverterPVWatts_Impl> impl)
    : Inverter(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
