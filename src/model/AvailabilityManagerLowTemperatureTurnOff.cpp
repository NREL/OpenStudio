/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerLowTemperatureTurnOff.hpp"
#include "AvailabilityManagerLowTemperatureTurnOff_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_LowTemperatureTurnOff_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerLowTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerLowTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(
      const AvailabilityManagerLowTemperatureTurnOff_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerLowTemperatureTurnOff_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Low Temperature Turn Off Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerLowTemperatureTurnOff_Impl::iddObjectType() const {
      return AvailabilityManagerLowTemperatureTurnOff::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AvailabilityManagerLowTemperatureTurnOff_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AvailabilityManagerLowTemperatureTurnOff", "Applicability Schedule"));
      }
      return result;
    }

    boost::optional<Node> AvailabilityManagerLowTemperatureTurnOff_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName);
    }

    double AvailabilityManagerLowTemperatureTurnOff_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_LowTemperatureTurnOffFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerLowTemperatureTurnOff_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerLowTemperatureTurnOff_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerLowTemperatureTurnOff_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_LowTemperatureTurnOffFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

    Schedule AvailabilityManagerLowTemperatureTurnOff_Impl::applicabilitySchedule() const {
      boost::optional<Schedule> value = optionalApplicabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Applicability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<Schedule> AvailabilityManagerLowTemperatureTurnOff_Impl::optionalApplicabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName);
    }

    bool AvailabilityManagerLowTemperatureTurnOff_Impl::setApplicabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName,
                                "AvailabilityManagerLowTemperatureTurnOff", "Applicability Schedule", schedule);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerLowTemperatureTurnOff::AvailabilityManagerLowTemperatureTurnOff(const Model& model)
    : AvailabilityManager(AvailabilityManagerLowTemperatureTurnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>());

    // Set Applicability Schedule to alwaysOnDiscreteSchedule
    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setApplicabilitySchedule(schedule);
    }
    setTemperature(30);
  }

  IddObjectType AvailabilityManagerLowTemperatureTurnOff::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOff};
  }

  boost::optional<Node> AvailabilityManagerLowTemperatureTurnOff::sensorNode() const {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->sensorNode();
  }

  double AvailabilityManagerLowTemperatureTurnOff::temperature() const {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->temperature();
  }

  bool AvailabilityManagerLowTemperatureTurnOff::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerLowTemperatureTurnOff::resetSensorNode() {
    getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerLowTemperatureTurnOff::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setTemperature(temperature);
  }

  Schedule AvailabilityManagerLowTemperatureTurnOff::applicabilitySchedule() const {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->applicabilitySchedule();
  }

  bool AvailabilityManagerLowTemperatureTurnOff::setApplicabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setApplicabilitySchedule(schedule);
  }

  /// @cond
  AvailabilityManagerLowTemperatureTurnOff::AvailabilityManagerLowTemperatureTurnOff(
    std::shared_ptr<detail::AvailabilityManagerLowTemperatureTurnOff_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
