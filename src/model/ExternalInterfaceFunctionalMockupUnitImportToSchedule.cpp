/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitImportToSchedule.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(const IdfObject& idfObject,
                                                                                                                           Model_Impl* model,
                                                                                                                           bool keepHandle)
      : Schedule_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(
      const ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(
        OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName);
    }

    void ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ensureNoLeapDays() {
      LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
    }

    std::vector<double> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::values() const {
      DoubleVector result;
      result.push_back(initialValue());
      return result;
    }

    ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUFile() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName)
        ->cast<ExternalInterfaceFunctionalMockupUnitImport>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUInstanceName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    double ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::resetScheduleTypeLimits() {
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName, fMUFile.handle());
      //TODO in FT use .fMUFile();
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, fMUInstanceName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitImportToSchedule::ExternalInterfaceFunctionalMockupUnitImportToSchedule(
    const Model& model, const ExternalInterfaceFunctionalMockupUnitImport& fMUFile, const std::string& fMUInstanceName,
    const std::string& fMUVariableName, double initialValue)
    : Schedule(ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>());

    bool ok = setFMUFile(fMUFile);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to " << fMUFile.fMUFileName() << ".");
    }
    setFMUInstanceName(fMUInstanceName);
    setFMUVariableName(fMUVariableName);
    setInitialValue(initialValue);
    //TODO move the Forward Translator
    //if (schedule.scheduleTypeLimits()) {
    //  ok = setScheduleTypeLimits(schedule.scheduleTypeLimits().get());
    //}
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule};
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUFile() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUFile();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUInstanceName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUInstanceName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUVariableName();
  }

  double ExternalInterfaceFunctionalMockupUnitImportToSchedule::initialValue() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->initialValue();
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUFile(fMUFile);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUInstanceName(const std::string& fMUInstanceName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUInstanceName(fMUInstanceName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUVariableName(fMUVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setInitialValue(initialValue);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitImportToSchedule::ExternalInterfaceFunctionalMockupUnitImportToSchedule(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl> impl)
    : Schedule(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
