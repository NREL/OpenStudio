/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ElectricLoadCenterInverterSimple.hpp"
#include "ElectricLoadCenterInverterSimple_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Inverter_Simple_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricLoadCenterInverterSimple_Impl::ElectricLoadCenterInverterSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Inverter_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterInverterSimple::iddObjectType());
    }

    ElectricLoadCenterInverterSimple_Impl::ElectricLoadCenterInverterSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : Inverter_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricLoadCenterInverterSimple::iddObjectType());
    }

    ElectricLoadCenterInverterSimple_Impl::ElectricLoadCenterInverterSimple_Impl(const ElectricLoadCenterInverterSimple_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : Inverter_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricLoadCenterInverterSimple_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Inverter DC to AC Efficiency",        "Inverter DC Input Electricity Rate",        "Inverter DC Input Electricity Energy",
        "Inverter AC Output Electricity Rate", "Inverter AC Output Electricity Energy",     "Inverter Conversion Loss Power",
        "Inverter Conversion Loss Energy",     "Inverter Conversion Loss Decrement Energy", "Inverter Thermal Loss Rate",
        "Inverter Thermal Loss Energy",        "Inverter Ancillary AC Electricity Rate",    "Inverter Ancillary AC Electricity Energy"};
      return result;
    }

    IddObjectType ElectricLoadCenterInverterSimple_Impl::iddObjectType() const {
      return ElectricLoadCenterInverterSimple::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricLoadCenterInverterSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ElectricLoadCenter_Inverter_SimpleFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ElectricLoadCenterInverterSimple", "Availability"));
      }
      return result;
    }

    boost::optional<Schedule> ElectricLoadCenterInverterSimple_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_Inverter_SimpleFields::AvailabilityScheduleName);
    }

    boost::optional<ThermalZone> ElectricLoadCenterInverterSimple_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_Inverter_SimpleFields::ZoneName);
    }

    boost::optional<double> ElectricLoadCenterInverterSimple_Impl::radiativeFraction() const {
      return getDouble(OS_ElectricLoadCenter_Inverter_SimpleFields::RadiativeFraction, true);
    }

    boost::optional<double> ElectricLoadCenterInverterSimple_Impl::inverterEfficiency() const {
      return getDouble(OS_ElectricLoadCenter_Inverter_SimpleFields::InverterEfficiency, true);
    }

    bool ElectricLoadCenterInverterSimple_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ElectricLoadCenter_Inverter_SimpleFields::AvailabilityScheduleName, "ElectricLoadCenterInverterSimple",
                                "Availability", schedule);
      return result;
    }

    void ElectricLoadCenterInverterSimple_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_SimpleFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterInverterSimple_Impl::setThermalZone(const ThermalZone& thermalZone) {
      return setPointer(OS_ElectricLoadCenter_Inverter_SimpleFields::ZoneName, thermalZone.handle());
    }

    void ElectricLoadCenterInverterSimple_Impl::resetThermalZone() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_SimpleFields::ZoneName, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterInverterSimple_Impl::setRadiativeFraction(boost::optional<double> radiativeFraction) {
      bool result(false);
      if (radiativeFraction) {
        result = setDouble(OS_ElectricLoadCenter_Inverter_SimpleFields::RadiativeFraction, radiativeFraction.get());
      } else {
        resetRadiativeFraction();
        result = true;
      }
      return result;
    }

    void ElectricLoadCenterInverterSimple_Impl::resetRadiativeFraction() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_SimpleFields::RadiativeFraction, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterInverterSimple_Impl::setInverterEfficiency(boost::optional<double> inverterEfficiency) {
      bool result(false);
      if (inverterEfficiency) {
        result = setDouble(OS_ElectricLoadCenter_Inverter_SimpleFields::InverterEfficiency, inverterEfficiency.get());
      } else {
        resetInverterEfficiency();
        result = true;
      }
      return result;
    }

    void ElectricLoadCenterInverterSimple_Impl::resetInverterEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_Inverter_SimpleFields::InverterEfficiency, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ElectricLoadCenterInverterSimple::ElectricLoadCenterInverterSimple(const Model& model)
    : Inverter(ElectricLoadCenterInverterSimple::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterInverterSimple_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType ElectricLoadCenterInverterSimple::iddObjectType() {
    return {IddObjectType::OS_ElectricLoadCenter_Inverter_Simple};
  }

  boost::optional<Schedule> ElectricLoadCenterInverterSimple::availabilitySchedule() const {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->availabilitySchedule();
  }

  boost::optional<double> ElectricLoadCenterInverterSimple::radiativeFraction() const {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->radiativeFraction();
  }

  boost::optional<double> ElectricLoadCenterInverterSimple::inverterEfficiency() const {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->inverterEfficiency();
  }

  bool ElectricLoadCenterInverterSimple::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->setAvailabilitySchedule(schedule);
  }

  void ElectricLoadCenterInverterSimple::resetAvailabilitySchedule() {
    getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->resetAvailabilitySchedule();
  }

  bool ElectricLoadCenterInverterSimple::setRadiativeFraction(double radiativeFraction) {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->setRadiativeFraction(radiativeFraction);
  }

  void ElectricLoadCenterInverterSimple::resetRadiativeFraction() {
    getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->resetRadiativeFraction();
  }

  bool ElectricLoadCenterInverterSimple::setInverterEfficiency(double inverterEfficiency) {
    return getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->setInverterEfficiency(inverterEfficiency);
  }

  void ElectricLoadCenterInverterSimple::resetInverterEfficiency() {
    getImpl<detail::ElectricLoadCenterInverterSimple_Impl>()->resetInverterEfficiency();
  }

  /// @cond
  ElectricLoadCenterInverterSimple::ElectricLoadCenterInverterSimple(std::shared_ptr<detail::ElectricLoadCenterInverterSimple_Impl> impl)
    : Inverter(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
