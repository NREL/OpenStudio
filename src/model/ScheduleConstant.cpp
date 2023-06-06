/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleConstant.hpp"
#include "ScheduleConstant_Impl.hpp"
#include "Model.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleConstant_Impl::ScheduleConstant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleConstant::iddObjectType());
    }

    ScheduleConstant_Impl::ScheduleConstant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleConstant::iddObjectType());
    }

    ScheduleConstant_Impl::ScheduleConstant_Impl(const ScheduleConstant_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ScheduleConstant_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ScheduleConstant_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ScheduleConstant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Schedule Value"};
      return result;
    }

    IddObjectType ScheduleConstant_Impl::iddObjectType() const {
      return ScheduleConstant::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ScheduleConstant_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_ConstantFields::ScheduleTypeLimitsName);
    }

    std::vector<double> ScheduleConstant_Impl::values() const {
      return DoubleVector(1u, value());
    }

    double ScheduleConstant_Impl::value() const {
      boost::optional<double> result = getDouble(OS_Schedule_ConstantFields::Value, true);
      if (!result) {
        return 0.0;
      }
      return *result;
    }

    bool ScheduleConstant_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_Schedule_ConstantFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ScheduleConstant_Impl::resetScheduleTypeLimits() {
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_Schedule_ConstantFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ScheduleConstant_Impl::setValue(double value) {
      bool ok = setDouble(OS_Schedule_ConstantFields::Value, value);
      OS_ASSERT(ok);
      return ok;
    }

    void ScheduleConstant_Impl::ensureNoLeapDays() {
      // nothing to do
    }

    std::vector<EMSActuatorNames> ScheduleConstant_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Schedule:Constant", "Schedule Value"}};
      return actuators;
    }

    std::vector<std::string> ScheduleConstant_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  // create a new ScheduleConstant object in the model's workspace
  ScheduleConstant::ScheduleConstant(const Model& model) : Schedule(ScheduleConstant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleConstant_Impl>());
    setValue(0.0);
  }

  // constructor
  ScheduleConstant::ScheduleConstant(std::shared_ptr<detail::ScheduleConstant_Impl> impl) : Schedule(std::move(impl)) {}

  IddObjectType ScheduleConstant::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Schedule_Constant);
    return result;
  }

  double ScheduleConstant::value() const {
    return getImpl<detail::ScheduleConstant_Impl>()->value();
  }

  bool ScheduleConstant::setValue(double value) {
    return getImpl<detail::ScheduleConstant_Impl>()->setValue(value);
  }

}  // namespace model
}  // namespace openstudio
