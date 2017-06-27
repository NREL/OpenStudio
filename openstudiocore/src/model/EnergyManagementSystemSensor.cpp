/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemSensor::iddObjectType());
  }

  EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemSensor::iddObjectType());
  }

  EnergyManagementSystemSensor_Impl::EnergyManagementSystemSensor_Impl(const EnergyManagementSystemSensor_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemSensor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
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

} // detail

EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const OutputVariable& outvar)
  : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
  bool ok = setOutputVariable(outvar);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariable to "
      << outvar.briefDescription() << ".");
  }
}

EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const OutputMeter& outvar)
  : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
  bool ok = setOutputMeter(outvar);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariable to "
      << outvar.briefDescription() << ".");
  }
}

EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model, const std::string& outputVariableOrMeterName)
  : ModelObject(EnergyManagementSystemSensor::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
  bool ok = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputVariableOrMeterName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableOrMeterName to "
      << outputVariableOrMeterName << ".");
  }
}

EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model)
  : ModelObject(EnergyManagementSystemSensor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());
  //TODO setOutputVariable() on a default OutputVariable
}

IddObjectType EnergyManagementSystemSensor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Sensor);
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
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

