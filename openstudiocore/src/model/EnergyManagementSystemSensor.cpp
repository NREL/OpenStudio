/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Sensor_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

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

  boost::optional<std::string> EnergyManagementSystemSensor_Impl::outputVariableorOutputMeterIndexKeyName() const {
    return getString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterIndexKeyName,true);
  }

  OutputVariable EnergyManagementSystemSensor_Impl::outputVariable() const {
    boost::optional<OutputVariable> value = optionalOutputVariable();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Output Variable or Output Meter attached.");
    }
    return value.get();
  }

  OutputMeter EnergyManagementSystemSensor_Impl::outputMeter() const {
    boost::optional<OutputMeter> value = optionalOutputMeter();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Output Variable or Output Meter attached.");
    }
    return value.get();
  }

  void EnergyManagementSystemSensor_Impl::setOutputVariableorOutputMeterIndexKeyName(const std::string& outputVariableorOutputMeterIndexKeyName) {
    bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterIndexKeyName, outputVariableorOutputMeterIndexKeyName);
    OS_ASSERT(result);
  }

  void EnergyManagementSystemSensor_Impl::resetOutputVariableorOutputMeterIndexKeyName() {
    bool result = setString(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterIndexKeyName, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemSensor_Impl::setOutputVariable(const OutputVariable& outputVariable) {
    bool result = setPointer(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputVariable.handle());
    return result;
  }

  bool EnergyManagementSystemSensor_Impl::setOutputMeter(const OutputMeter& outputMeter) {
    bool result = setPointer(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName, outputMeter.handle());
    return result;
  }

  boost::optional<OutputVariable> EnergyManagementSystemSensor_Impl::optionalOutputVariable() const {
    return getObject<ModelObject>().getModelObjectTarget<OutputVariable>(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
  }

  boost::optional<OutputMeter> EnergyManagementSystemSensor_Impl::optionalOutputMeter() const {
    return getObject<ModelObject>().getModelObjectTarget<OutputMeter>(OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName);
  }

} // detail

EnergyManagementSystemSensor::EnergyManagementSystemSensor(const Model& model)
  : ModelObject(EnergyManagementSystemSensor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSensor_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_EnergyManagementSystem_SensorFields::OutputVariableorOutputMeterName
  bool ok = true;
  // ok = setOutputVariableorOutputMeter();
  OS_ASSERT(ok);
}

IddObjectType EnergyManagementSystemSensor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Sensor);
}

boost::optional<std::string> EnergyManagementSystemSensor::outputVariableorOutputMeterIndexKeyName() const {
  return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputVariableorOutputMeterIndexKeyName();
}

boost::optional<OutputVariable> EnergyManagementSystemSensor::outputVariable() const {
  return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputVariable();
}

boost::optional<OutputMeter> EnergyManagementSystemSensor::outputMeter() const {
  return getImpl<detail::EnergyManagementSystemSensor_Impl>()->outputMeter();
}

void EnergyManagementSystemSensor::setOutputVariableorOutputMeterIndexKeyName(const std::string& outputVariableorOutputMeterIndexKeyName) {
  getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputVariableorOutputMeterIndexKeyName(outputVariableorOutputMeterIndexKeyName);
}

void EnergyManagementSystemSensor::resetOutputVariableorOutputMeterIndexKeyName() {
  getImpl<detail::EnergyManagementSystemSensor_Impl>()->resetOutputVariableorOutputMeterIndexKeyName();
}

bool EnergyManagementSystemSensor::setOutputVariable(const OutputVariable& outputVariable) {
  return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputVariable(outputVariable);
}

bool EnergyManagementSystemSensor::setOutputMeter(const OutputMeter& outputMeter) {
  return getImpl<detail::EnergyManagementSystemSensor_Impl>()->setOutputMeter(outputMeter);
}

/// @cond
EnergyManagementSystemSensor::EnergyManagementSystemSensor(std::shared_ptr<detail::EnergyManagementSystemSensor_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

