/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Sensor_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/UUID.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemSensor::iddObjectType());
    }

    EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemSensor::iddObjectType());
    }

    EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const EnergyManagementSystemSensor_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemSensor_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemSensor_Impl::iddObjectType() const {
      return EnergyManagementSystemSensor::iddObjectType();
    }

    std::string EnergyManagementSystemSensor_Impl::keyName() const {
      OptionalString results = getString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterIndexKeyName, true);
      if (results) {
        return results.get();
      }
      return "";
    }

    boost::optional<OutputVariable> EnergyManagementSystemSensor_Impl::outputVariable() const {
      boost::optional<OutputVariable> value = optionalOutputVariable();
      if (!value.is_initialized()) {
        LOG(Info, " does not have an Output Variable attached.");
        return value;
      }
      return value.get();
    }

    boost::optional<OutputMeter> EnergyManagementSystemSensor_Impl::outputMeter() const {
      boost::optional<OutputMeter> value = optionalOutputMeter();
      if (!value.is_initialized()) {
        LOG(Info, " does not have an Output Meter attached.");
        return value;
      }
      return value.get();
    }

    std::string EnergyManagementSystemSensor_Impl::outputVariableOrMeterName() const {
      OptionalString results = getString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, true);
      if (results) {
        return results.get();
      }
      return "";
    }

    bool EnergyManagementSystemSensor_Impl::setKeyName(const std::string& keyName) {
      bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterIndexKeyName, keyName);
      return result;
    }

    bool EnergyManagementSystemSensor_Impl::setOutputVariable(const OutputVariable& outputVariable) {
      //bool result = setPointer(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputVariable.handle());
      bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, toString(outputVariable.handle()));
      return result;
    }

    bool EnergyManagementSystemSensor_Impl::setOutputMeter(const OutputMeter& outputMeter) {
      //bool result = setPointer(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputMeter.handle());
      bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, toString(outputMeter.handle()));
      return result;
    }

    bool EnergyManagementSystemSensor_Impl::setOutputVariableOrMeterName(const std::string& outputVariableOrMeterName) {
      //bool result = setPointer(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputMeter.handle());
      bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputVariableOrMeterName);
      return result;
    }

    boost::optional<OutputVariable> EnergyManagementSystemSensor_Impl::optionalOutputVariable() const {
      //Note: Cant do /object-list implementation since Auto Naming of Objects causes issues.
      //      Instead, doing an /alpha getString implementation
      //return getObject<ModelObject>().getModelObjectTarget<OutputVariable>(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
      boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
      const Model m = this->model();
      if (handle) {
        UUID uid = toUUID(handle.get());
        boost::optional<ModelObject> object = m.getModelObject<model::ModelObject>(uid);
        if (object) {
          if (object->optionalCast<OutputVariable>()) {
            return object->cast<OutputVariable>();
          }
        }
      }
      return boost::none;
    }

    boost::optional<OutputMeter> EnergyManagementSystemSensor_Impl::optionalOutputMeter() const {
      //Note: Cant do /object-list implementation since Auto Naming of Objects causes issues.
      //      Instead, doing an /alpha getString implementation
      //return getObject<ModelObject>().getModelObjectTarget<OutputMeter>(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
      boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
      const Model m = this->model();
      if (handle) {
        UUID uid = toUUID(handle.get());
        boost::optional<ModelObject> object = m.getModelObject<model::ModelObject>(uid);
        if (object) {
          if (object->optionalCast<OutputMeter>()) {
            return object->cast<OutputMeter>();
          }
        }
      }
      return boost::none;
    }

  }  // namespace detail

  EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const OutputVariable& outvar)
    : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
    bool ok = setOutputVariable(outvar);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariable to " << outvar.briefDescription() << ".");
    }
    if (!outvar.isKeyValueDefaulted()) {
      std::string key = outvar.keyValue();
      setKeyName(key);
    }
  }

  EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const OutputMeter& outvar)
    : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
    bool ok = setOutputMeter(outvar);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariable to " << outvar.briefDescription() << ".");
    }
  }

  EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const std::string& outputVariableOrMeterName)
    : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
    bool ok = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputVariableOrMeterName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableOrMeterName to " << outputVariableOrMeterName << ".");
    }
  }

  EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model) : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
    //TODO setOutputVariable() on a default OutputVariable
  }

  IddObjectType EnergyManagementSystemSensor::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_Sensor};
  }

  std::string EnergyManagementSystemSensor::keyName() const {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->keyName();
  }

  boost::optional<OutputVariable> EnergyManagementSystemSensor::outputVariable() const {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputVariable();
  }

  boost::optional<OutputMeter> EnergyManagementSystemSensor::outputMeter() const {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputMeter();
  }

  std::string EnergyManagementSystemSensor::outputVariableOrMeterName() const {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputVariableOrMeterName();
  }

  bool EnergyManagementSystemSensor::setKeyName(const std::string& keyName) {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setKeyName(keyName);
  }

  bool EnergyManagementSystemSensor::setOutputVariable(const OutputVariable& outputVariable) {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputVariable(outputVariable);
  }

  bool EnergyManagementSystemSensor::setOutputMeter(const OutputMeter& outputMeter) {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputMeter(outputMeter);
  }

  bool EnergyManagementSystemSensor::setOutputVariableOrMeterName(const std::string& outputVariableOrMeterName) {
    return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputVariableOrMeterName(outputVariableOrMeterName);
  }

  /// @cond
  EnergyManagementSystemSensor::EnergyManagementSystemSensor(std::shared_ptr<detail::EnergyManagementSystemSensor_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
