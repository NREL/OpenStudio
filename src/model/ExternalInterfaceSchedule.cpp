/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceSchedule.hpp"
#include "ExternalInterfaceSchedule_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceSchedule::iddObjectType());
    }

    ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceSchedule::iddObjectType());
    }

    ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const ExternalInterfaceSchedule_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ExternalInterfaceSchedule_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ExternalInterfaceSchedule_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    const std::vector<std::string>& ExternalInterfaceSchedule_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceSchedule_Impl::iddObjectType() const {
      return ExternalInterfaceSchedule::iddObjectType();
    }

    void ExternalInterfaceSchedule_Impl::ensureNoLeapDays() {
      LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
    }

    std::vector<double> ExternalInterfaceSchedule_Impl::values() const {
      DoubleVector result;
      result.push_back(initialValue());
      return result;
    }

    boost::optional<ScheduleTypeLimits> ExternalInterfaceSchedule_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName);
    }

    double ExternalInterfaceSchedule_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_ScheduleFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ExternalInterfaceSchedule_Impl::resetScheduleTypeLimits() {
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ExternalInterfaceSchedule_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_ScheduleFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceSchedule_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_ScheduleFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool ExternalInterfaceSchedule_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_ExternalInterface_ScheduleFields::ExportToBCVTB);
    }

    bool ExternalInterfaceSchedule_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_ExternalInterface_ScheduleFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_ExternalInterface_ScheduleFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceSchedule_Impl::resetExportToBCVTB() {
      bool result = setString(OS_ExternalInterface_ScheduleFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ExternalInterfaceSchedule::ExternalInterfaceSchedule(const Model& model, double initialValue)
    : Schedule(ExternalInterfaceSchedule::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceSchedule_Impl>());

    setInitialValue(initialValue);
    //TODO move the Forward Translator
    //if (schedule.scheduleTypeLimits()) {
    //  ok = setScheduleTypeLimits(schedule.scheduleTypeLimits().get());
    //}
  }

  ExternalInterfaceSchedule::ExternalInterfaceSchedule(const Model& model) : Schedule(ExternalInterfaceSchedule::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceSchedule_Impl>());
    setInitialValue(0.0);
  }

  IddObjectType ExternalInterfaceSchedule::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_Schedule};
  }

  double ExternalInterfaceSchedule::initialValue() const {
    return getImpl<detail::ExternalInterfaceSchedule_Impl>()->initialValue();
  }

  bool ExternalInterfaceSchedule::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceSchedule_Impl>()->setInitialValue(initialValue);
  }

  bool ExternalInterfaceSchedule::exportToBCVTB() const {
    return getImpl<detail::ExternalInterfaceSchedule_Impl>()->exportToBCVTB();
  }

  bool ExternalInterfaceSchedule::isExportToBCVTBDefaulted() const {
    return getImpl<detail::ExternalInterfaceSchedule_Impl>()->isExportToBCVTBDefaulted();
  }

  bool ExternalInterfaceSchedule::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::ExternalInterfaceSchedule_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void ExternalInterfaceSchedule::resetExportToBCVTB() {
    getImpl<detail::ExternalInterfaceSchedule_Impl>()->resetExportToBCVTB();
  }

  /// @cond
  ExternalInterfaceSchedule::ExternalInterfaceSchedule(std::shared_ptr<detail::ExternalInterfaceSchedule_Impl> impl) : Schedule(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
