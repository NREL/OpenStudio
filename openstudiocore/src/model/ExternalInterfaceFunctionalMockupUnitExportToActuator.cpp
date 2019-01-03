/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportToActuator.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(const ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType();
  }

  ModelObject ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentUnique() const {
    return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName)->cast<ModelObject>();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentType() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentControlType() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::fMUVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  double ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue,true);
    if (value) {
      return value.get();
    }
    return -9999;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentUnique(const ModelObject& modelObject) {
    bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName, modelObject.handle());
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType, actuatedComponentType);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue, initialValue);
    OS_ASSERT(result);
    return result;
  }

} // detail

ExternalInterfaceFunctionalMockupUnitExportToActuator::ExternalInterfaceFunctionalMockupUnitExportToActuator(const ModelObject& modelObject,
                                                                                                             const std::string& actuatedComponentType,
                                                                                                             const std::string& actuatedComponentControlType,
                                                                                                             const std::string& fMUVariableName,
                                                                                                             double initialValue)
  : ModelObject(ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType(), modelObject.model())
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>());

  bool ok = setActuatedComponentUnique(modelObject);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s ActuatedComponentUnique to " << modelObject.nameString() << ".");
  }
  setActuatedComponentType(actuatedComponentType);
  setActuatedComponentControlType(actuatedComponentControlType);
  setFMUVariableName(fMUVariableName);
  setInitialValue(initialValue);
}

IddObjectType ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator);
}

ModelObject ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentUnique() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentUnique();
}

std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentType() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentType();
}

std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentControlType() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentControlType();
}

std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->fMUVariableName();
}

double ExternalInterfaceFunctionalMockupUnitExportToActuator::initialValue() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->initialValue();
}

bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentUnique(const ModelObject& modelObject) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentUnique(modelObject);
}

bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
}

bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentControlType(actuatedComponentControlType);
}

bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setFMUVariableName(const std::string& fMUVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setFMUVariableName(fMUVariableName);
}

bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setInitialValue(double initialValue) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitExportToActuator::ExternalInterfaceFunctionalMockupUnitExportToActuator(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
