/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ThermostatSetpointDualSetpoint.hpp"
#include "ThermostatSetpointDualSetpoint_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/OS_ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ThermostatSetpointDualSetpoint_Impl::ThermostatSetpointDualSetpoint_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Thermostat_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ThermostatSetpointDualSetpoint::iddObjectType());
    }

    ThermostatSetpointDualSetpoint_Impl::ThermostatSetpointDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : Thermostat_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ThermostatSetpointDualSetpoint::iddObjectType());
    }

    ThermostatSetpointDualSetpoint_Impl::ThermostatSetpointDualSetpoint_Impl(const ThermostatSetpointDualSetpoint_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : Thermostat_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ThermostatSetpointDualSetpoint_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ThermostatSetpointDualSetpoint_Impl::iddObjectType() const {
      return ThermostatSetpointDualSetpoint::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ThermostatSetpointDualSetpoint_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ThermostatSetpointDualSetpoint", "Heating Setpoint Temperature"));
      }
      if (std::find(b, e, OS_ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ThermostatSetpointDualSetpoint", "Cooling Setpoint Temperature"));
      }
      return result;
    }

    boost::optional<Schedule> ThermostatSetpointDualSetpoint_Impl::heatingSetpointTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName);
    }

    boost::optional<Schedule> ThermostatSetpointDualSetpoint_Impl::coolingSetpointTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName);
    }

    bool ThermostatSetpointDualSetpoint_Impl::setHeatingSetpointTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName, "ThermostatSetpointDualSetpoint",
                                "Heating Setpoint Temperature", schedule);
      return result;
    }

    void ThermostatSetpointDualSetpoint_Impl::resetHeatingSetpointTemperatureSchedule() {
      bool result = setString(OS_ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ThermostatSetpointDualSetpoint_Impl::setCoolingSetpointTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName, "ThermostatSetpointDualSetpoint",
                                "Cooling Setpoint Temperature", schedule);
      return result;
    }

    void ThermostatSetpointDualSetpoint_Impl::resetCoolingSetpointTemperatureSchedule() {
      bool result = setString(OS_ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> ThermostatSetpointDualSetpoint_Impl::heatingSetpointTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = heatingSetpointTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> ThermostatSetpointDualSetpoint_Impl::coolingSetpointTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = coolingSetpointTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool ThermostatSetpointDualSetpoint_Impl::setHeatingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setHeatingSetpointTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetHeatingSetpointTemperatureSchedule();
      }
      return true;
    }

    bool ThermostatSetpointDualSetpoint_Impl::setCoolingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setCoolingSetpointTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetCoolingSetpointTemperatureSchedule();
      }
      return true;
    }

    double ThermostatSetpointDualSetpoint_Impl::temperatureDifferenceBetweenCutoutAndSetpoint() const {
      boost::optional<double> value = getDouble(OS_ThermostatSetpoint_DualSetpointFields::TemperatureDifferenceBetweenCutoutAndSetpoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ThermostatSetpointDualSetpoint_Impl::setTemperatureDifferenceBetweenCutoutAndSetpoint(double deltaT) {
      bool result = setDouble(OS_ThermostatSetpoint_DualSetpointFields::TemperatureDifferenceBetweenCutoutAndSetpoint, deltaT);
      return result;
    }

    bool ThermostatSetpointDualSetpoint_Impl::isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted() const {
      return isEmpty(OS_ThermostatSetpoint_DualSetpointFields::TemperatureDifferenceBetweenCutoutAndSetpoint);
    }

    // Move to public side

  }  // namespace detail

  ThermostatSetpointDualSetpoint::ThermostatSetpointDualSetpoint(const Model& model)
    : Thermostat(ThermostatSetpointDualSetpoint::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ThermostatSetpointDualSetpoint_Impl>());
  }

  ThermostatSetpointDualSetpoint::ThermostatSetpointDualSetpoint(std::shared_ptr<detail::ThermostatSetpointDualSetpoint_Impl> impl)
    : Thermostat(impl) {}

  IddObjectType ThermostatSetpointDualSetpoint::iddObjectType() {
    IddObjectType result(IddObjectType::OS_ThermostatSetpoint_DualSetpoint);
    return result;
  }

  boost::optional<Schedule> ThermostatSetpointDualSetpoint::heatingSetpointTemperatureSchedule() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->heatingSetpointTemperatureSchedule();
  }

  boost::optional<Schedule> ThermostatSetpointDualSetpoint::coolingSetpointTemperatureSchedule() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->coolingSetpointTemperatureSchedule();
  }

  bool ThermostatSetpointDualSetpoint::setHeatingSetpointTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->setHeatingSetpointTemperatureSchedule(schedule);
  }

  void ThermostatSetpointDualSetpoint::resetHeatingSetpointTemperatureSchedule() {
    getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->resetHeatingSetpointTemperatureSchedule();
  }

  bool ThermostatSetpointDualSetpoint::setCoolingSetpointTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->setCoolingSetpointTemperatureSchedule(schedule);
  }

  void ThermostatSetpointDualSetpoint::resetCoolingSetpointTemperatureSchedule() {
    getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->resetCoolingSetpointTemperatureSchedule();
  }

  boost::optional<Schedule> ThermostatSetpointDualSetpoint::getHeatingSchedule() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->heatingSetpointTemperatureSchedule();
  }
  bool ThermostatSetpointDualSetpoint::setHeatingSchedule(Schedule& s) {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->setHeatingSetpointTemperatureSchedule(s);
  }

  boost::optional<Schedule> ThermostatSetpointDualSetpoint::getCoolingSchedule() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->coolingSetpointTemperatureSchedule();
  }

  bool ThermostatSetpointDualSetpoint::setCoolingSchedule(Schedule& s) {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->setCoolingSetpointTemperatureSchedule(s);
  }

  void ThermostatSetpointDualSetpoint::resetHeatingSchedule() {
    getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->resetHeatingSetpointTemperatureSchedule();
  }

  void ThermostatSetpointDualSetpoint::resetCoolingSchedule() {
    getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->resetCoolingSetpointTemperatureSchedule();
  }

  double ThermostatSetpointDualSetpoint::temperatureDifferenceBetweenCutoutAndSetpoint() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->temperatureDifferenceBetweenCutoutAndSetpoint();
  }

  bool ThermostatSetpointDualSetpoint::setTemperatureDifferenceBetweenCutoutAndSetpoint(double deltaT) {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->setTemperatureDifferenceBetweenCutoutAndSetpoint(deltaT);
  }

  bool ThermostatSetpointDualSetpoint::isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted() const {
    return getImpl<detail::ThermostatSetpointDualSetpoint_Impl>()->isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted();
  }

}  // namespace model
}  // namespace openstudio
