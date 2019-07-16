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

#include "ExternalInterfaceActuator.hpp"
#include "ExternalInterfaceActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Actuator_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceActuator::iddObjectType());
  }

  ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceActuator::iddObjectType());
  }

  ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const ExternalInterfaceActuator_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceActuator_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceActuator_Impl::iddObjectType() const {
    return ExternalInterfaceActuator::iddObjectType();
  }

  ModelObject ExternalInterfaceActuator_Impl::actuatedComponentUnique() const {

    return this->getTarget(OS_ExternalInterface_ActuatorFields::ActuatedComponentUniqueName)->cast<ModelObject>();
  }

  std::string ExternalInterfaceActuator_Impl::actuatedComponentType() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ActuatedComponentType,true);
    if (value) {
      return value.get();
    } else {
      return "";
    }
  }

  std::string ExternalInterfaceActuator_Impl::actuatedComponentControlType() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ActuatedComponentControlType,true);
    if (value) {
      return value.get();
    } else {
      return "";
    }
  }

  boost::optional<double> ExternalInterfaceActuator_Impl::optionalInitialValue() const {
    return getDouble(OS_ExternalInterface_ActuatorFields::OptionalInitialValue,true);
  }

  bool ExternalInterfaceActuator_Impl::setActuatedComponentUnique(const ModelObject& modelObject) {
    bool result = setPointer(OS_ExternalInterface_ActuatorFields::ActuatedComponentUniqueName, modelObject.handle());
    return result;
  }

  bool ExternalInterfaceActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
    bool result = setString(OS_ExternalInterface_ActuatorFields::ActuatedComponentType, actuatedComponentType);
    return result;
  }

  bool ExternalInterfaceActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    bool result = setString(OS_ExternalInterface_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
    return result;
  }

  bool ExternalInterfaceActuator_Impl::setOptionalInitialValue(double optionalInitialValue) {
    bool result = setDouble(OS_ExternalInterface_ActuatorFields::OptionalInitialValue, optionalInitialValue);
    OS_ASSERT(result);
    return result;
  }

  void ExternalInterfaceActuator_Impl::resetOptionalInitialValue() {
    bool result = setString(OS_ExternalInterface_ActuatorFields::OptionalInitialValue, "");
    OS_ASSERT(result);
  }

  bool ExternalInterfaceActuator_Impl::exportToBCVTB() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool ExternalInterfaceActuator_Impl::isExportToBCVTBDefaulted() const {
    return isEmpty(OS_ExternalInterface_ActuatorFields::ExportToBCVTB);
  }

  bool ExternalInterfaceActuator_Impl::setExportToBCVTB(bool exportToBCVTB) {
    bool result = false;
    if (exportToBCVTB) {
      result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "True");
    } else {
      result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "False");
    }
    OS_ASSERT(result);
    return result;
  }

  void ExternalInterfaceActuator_Impl::resetExportToBCVTB() {
    bool result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "");
    OS_ASSERT(result);
  }

} // detail

ExternalInterfaceActuator::ExternalInterfaceActuator(const ModelObject& modelObject, const std::string actuatedComponentType, const std::string actuatedComponentControlType)
  : ModelObject(ExternalInterfaceActuator::iddObjectType(), modelObject.model()) {
  OS_ASSERT(getImpl<detail::ExternalInterfaceActuator_Impl>());

  bool ok = setActuatedComponentUnique(modelObject);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponentUnique to "
      << modelObject.briefDescription() << ".");
  }
  ok = setActuatedComponentType(actuatedComponentType);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to "
      << actuatedComponentType << ".");
  }
  ok = setActuatedComponentControlType(actuatedComponentControlType);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to "
      << actuatedComponentControlType << ".");
  }
}

ExternalInterfaceActuator::ExternalInterfaceActuator(const ModelObject& modelObject)
  : ModelObject(ExternalInterfaceActuator::iddObjectType(), modelObject.model())
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceActuator_Impl>());

  bool ok = setActuatedComponentUnique(modelObject);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponentUnique to "
      << modelObject.briefDescription() << ".");
  }
  //TODO set below to some appropriate default that depends on the modelObject::IddType
  // setActuatedComponentType();
  // setActuatedComponentControlType();
}

IddObjectType ExternalInterfaceActuator::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_Actuator);
}

ModelObject ExternalInterfaceActuator::actuatedComponentUnique() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentUnique();
}

std::string ExternalInterfaceActuator::actuatedComponentType() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentType();
}

std::string ExternalInterfaceActuator::actuatedComponentControlType() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentControlType();
}

boost::optional<double> ExternalInterfaceActuator::optionalInitialValue() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->optionalInitialValue();
}

bool ExternalInterfaceActuator::setActuatedComponentUnique(const ModelObject& modelObject) {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentUnique(modelObject);
}

bool ExternalInterfaceActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
}

bool ExternalInterfaceActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentControlType(actuatedComponentControlType);
}

bool ExternalInterfaceActuator::setOptionalInitialValue(double optionalInitialValue) {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->setOptionalInitialValue(optionalInitialValue);
}

void ExternalInterfaceActuator::resetOptionalInitialValue() {
  getImpl<detail::ExternalInterfaceActuator_Impl>()->resetOptionalInitialValue();
}

bool ExternalInterfaceActuator::exportToBCVTB() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->exportToBCVTB();
}

bool ExternalInterfaceActuator::isExportToBCVTBDefaulted() const {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->isExportToBCVTBDefaulted();
}

bool ExternalInterfaceActuator::setExportToBCVTB(bool exportToBCVTB) {
  return getImpl<detail::ExternalInterfaceActuator_Impl>()->setExportToBCVTB(exportToBCVTB);
}

void ExternalInterfaceActuator::resetExportToBCVTB() {
  getImpl<detail::ExternalInterfaceActuator_Impl>()->resetExportToBCVTB();
}

/// @cond
ExternalInterfaceActuator::ExternalInterfaceActuator(std::shared_ptr<detail::ExternalInterfaceActuator_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
