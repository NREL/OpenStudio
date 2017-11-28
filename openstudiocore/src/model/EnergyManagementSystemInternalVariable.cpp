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

#include "EnergyManagementSystemInternalVariable.hpp"
#include "EnergyManagementSystemInternalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_InternalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
  }

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
  }

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const EnergyManagementSystemInternalVariable_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemInternalVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemInternalVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemInternalVariable::iddObjectType();
  }

  std::string EnergyManagementSystemInternalVariable_Impl::internalDataIndexKeyName() const {
    OptionalString result = getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName,true);
    if (result) {
      return result.get();
    }
    return "";
  }

  std::string EnergyManagementSystemInternalVariable_Impl::internalDataType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType,true);
    return value.get();
  }

  bool EnergyManagementSystemInternalVariable_Impl::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, internalDataIndexKeyName);
    return result;
  }

  void EnergyManagementSystemInternalVariable_Impl::resetInternalDataIndexKeyName() {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemInternalVariable_Impl::setInternalDataType(const std::string& internalDataType) {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType, internalDataType);
    return result;
  }

} // detail

EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(const Model& model, const std::string internalDataType)
: ModelObject(EnergyManagementSystemInternalVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemInternalVariable_Impl>());
  bool ok = setInternalDataType(internalDataType);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s internalDataType to " << internalDataType << ".");
  }
}

EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(const Model& model)
  : ModelObject(EnergyManagementSystemInternalVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemInternalVariable_Impl>());
  //TODO setInternalDataType to valid EIO default value
}

IddObjectType EnergyManagementSystemInternalVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_InternalVariable);
}

std::string EnergyManagementSystemInternalVariable::internalDataIndexKeyName() const {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataIndexKeyName();
}

std::string EnergyManagementSystemInternalVariable::internalDataType() const {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataType();
}

bool EnergyManagementSystemInternalVariable::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataIndexKeyName(internalDataIndexKeyName);
}

void EnergyManagementSystemInternalVariable::resetInternalDataIndexKeyName() {
  getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->resetInternalDataIndexKeyName();
}

bool EnergyManagementSystemInternalVariable::setInternalDataType(const std::string& internalDataType) {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataType(internalDataType);
}

/// @cond
EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

