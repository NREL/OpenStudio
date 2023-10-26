/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const IdfObject& idfObject,
                                                                                                                           Model_Impl* model,
                                                                                                                           bool keepHandle)
      : Schedule_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(
      const ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(
        OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName);
    }

    void ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ensureNoLeapDays() {
      LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
    }

    std::vector<double> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::values() const {
      DoubleVector result;
      result.push_back(initialValue());
      return result;
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    double ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::resetScheduleTypeLimits() {
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(const Model& model,
                                                                                                               const std::string& fMUVariableName,
                                                                                                               double initialValue)
    : Schedule(ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>());

    setFMUVariableName(fMUVariableName);
    setInitialValue(initialValue);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule};
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToSchedule::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->fMUVariableName();
  }

  double ExternalInterfaceFunctionalMockupUnitExportToSchedule::initialValue() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->initialValue();
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setFMUVariableName(fMUVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setInitialValue(initialValue);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl> impl)
    : Schedule(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
