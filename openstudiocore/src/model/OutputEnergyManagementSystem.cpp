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

#include "OutputEnergyManagementSystem.hpp"
#include "OutputEnergyManagementSystem_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_EnergyManagementSystem_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  OutputEnergyManagementSystem_Impl::OutputEnergyManagementSystem_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutputEnergyManagementSystem::iddObjectType());
  }

  OutputEnergyManagementSystem_Impl::OutputEnergyManagementSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutputEnergyManagementSystem::iddObjectType());
  }

  OutputEnergyManagementSystem_Impl::OutputEnergyManagementSystem_Impl(const OutputEnergyManagementSystem_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& OutputEnergyManagementSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutputEnergyManagementSystem_Impl::iddObjectType() const {
    return OutputEnergyManagementSystem::iddObjectType();
  }

  std::string OutputEnergyManagementSystem_Impl::actuatorAvailabilityDictionaryReporting() const {
    boost::optional<std::string> value = getString(OS_Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutputEnergyManagementSystem_Impl::isActuatorAvailabilityDictionaryReportingDefaulted() const {
    return isEmpty(OS_Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting);
  }

  std::string OutputEnergyManagementSystem_Impl::internalVariableAvailabilityDictionaryReporting() const {
    boost::optional<std::string> value = getString(OS_Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutputEnergyManagementSystem_Impl::isInternalVariableAvailabilityDictionaryReportingDefaulted() const {
    return isEmpty(OS_Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting);
  }

  std::string OutputEnergyManagementSystem_Impl::eMSRuntimeLanguageDebugOutputLevel() const {
    boost::optional<std::string> value = getString(OS_Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutputEnergyManagementSystem_Impl::isEMSRuntimeLanguageDebugOutputLevelDefaulted() const {
    return isEmpty(OS_Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel);
  }

  bool OutputEnergyManagementSystem_Impl::setActuatorAvailabilityDictionaryReporting(const std::string& actuatorAvailabilityDictionaryReporting) {
    bool result = setString(OS_Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting, actuatorAvailabilityDictionaryReporting);
    return result;
  }

  void OutputEnergyManagementSystem_Impl::resetActuatorAvailabilityDictionaryReporting() {
    bool result = setString(OS_Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting, "");
    OS_ASSERT(result);
  }

  bool OutputEnergyManagementSystem_Impl::setInternalVariableAvailabilityDictionaryReporting(const std::string& internalVariableAvailabilityDictionaryReporting) {
    bool result = setString(OS_Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting, internalVariableAvailabilityDictionaryReporting);
    return result;
  }

  void OutputEnergyManagementSystem_Impl::resetInternalVariableAvailabilityDictionaryReporting() {
    bool result = setString(OS_Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting, "");
    OS_ASSERT(result);
  }

  bool OutputEnergyManagementSystem_Impl::setEMSRuntimeLanguageDebugOutputLevel(const std::string& eMSRuntimeLanguageDebugOutputLevel) {
    bool result = setString(OS_Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel, eMSRuntimeLanguageDebugOutputLevel);
    return result;
  }

  void OutputEnergyManagementSystem_Impl::resetEMSRuntimeLanguageDebugOutputLevel() {
    bool result = setString(OS_Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType OutputEnergyManagementSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Output_EnergyManagementSystem);
}

std::vector<std::string> OutputEnergyManagementSystem::actuatorAvailabilityDictionaryReportingValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting);
}

std::vector<std::string> OutputEnergyManagementSystem::internalVariableAvailabilityDictionaryReportingValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting);
}

std::vector<std::string> OutputEnergyManagementSystem::eMSRuntimeLanguageDebugOutputLevelValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel);
}

std::string OutputEnergyManagementSystem::actuatorAvailabilityDictionaryReporting() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->actuatorAvailabilityDictionaryReporting();
}

bool OutputEnergyManagementSystem::isActuatorAvailabilityDictionaryReportingDefaulted() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->isActuatorAvailabilityDictionaryReportingDefaulted();
}

std::string OutputEnergyManagementSystem::internalVariableAvailabilityDictionaryReporting() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->internalVariableAvailabilityDictionaryReporting();
}

bool OutputEnergyManagementSystem::isInternalVariableAvailabilityDictionaryReportingDefaulted() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->isInternalVariableAvailabilityDictionaryReportingDefaulted();
}

std::string OutputEnergyManagementSystem::eMSRuntimeLanguageDebugOutputLevel() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->eMSRuntimeLanguageDebugOutputLevel();
}

bool OutputEnergyManagementSystem::isEMSRuntimeLanguageDebugOutputLevelDefaulted() const {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->isEMSRuntimeLanguageDebugOutputLevelDefaulted();
}

bool OutputEnergyManagementSystem::setActuatorAvailabilityDictionaryReporting(const std::string& actuatorAvailabilityDictionaryReporting) {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->setActuatorAvailabilityDictionaryReporting(actuatorAvailabilityDictionaryReporting);
}

void OutputEnergyManagementSystem::resetActuatorAvailabilityDictionaryReporting() {
  getImpl<detail::OutputEnergyManagementSystem_Impl>()->resetActuatorAvailabilityDictionaryReporting();
}

bool OutputEnergyManagementSystem::setInternalVariableAvailabilityDictionaryReporting(const std::string& internalVariableAvailabilityDictionaryReporting) {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->setInternalVariableAvailabilityDictionaryReporting(internalVariableAvailabilityDictionaryReporting);
}

void OutputEnergyManagementSystem::resetInternalVariableAvailabilityDictionaryReporting() {
  getImpl<detail::OutputEnergyManagementSystem_Impl>()->resetInternalVariableAvailabilityDictionaryReporting();
}

bool OutputEnergyManagementSystem::setEMSRuntimeLanguageDebugOutputLevel(const std::string& eMSRuntimeLanguageDebugOutputLevel) {
  return getImpl<detail::OutputEnergyManagementSystem_Impl>()->setEMSRuntimeLanguageDebugOutputLevel(eMSRuntimeLanguageDebugOutputLevel);
}

void OutputEnergyManagementSystem::resetEMSRuntimeLanguageDebugOutputLevel() {
  getImpl<detail::OutputEnergyManagementSystem_Impl>()->resetEMSRuntimeLanguageDebugOutputLevel();
}

/// @cond
OutputEnergyManagementSystem::OutputEnergyManagementSystem(std::shared_ptr<detail::OutputEnergyManagementSystem_Impl> impl)
  : ModelObject(impl)
{}
OutputEnergyManagementSystem::OutputEnergyManagementSystem(Model& model)
  : ModelObject(OutputEnergyManagementSystem::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

