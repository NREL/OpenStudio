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

#include "ExternalInterfaceFunctionalMockupUnitImportToVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Variable_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToVariable::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToVariable::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitImportToVariable::iddObjectType();
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::fMUFile() const {
    return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUFileName)->cast<ExternalInterfaceFunctionalMockupUnitImport>();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::fMUInstanceName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUInstanceName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::fMUVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUVariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  double ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::InitialValue,true);
    if (value) {
      return value.get();
    }
    return -9999;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUFileName, fMUFile.handle());
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUInstanceName, fMUInstanceName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_VariableFields::InitialValue, initialValue);
    OS_ASSERT(result);
    return result;
  }

} // detail

ExternalInterfaceFunctionalMockupUnitImportToVariable::ExternalInterfaceFunctionalMockupUnitImportToVariable(const Model& model,
                                                                                                             const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                                                             const std::string& fMUInstanceName,
                                                                                                             const std::string& fMUVariableName,
                                                                                                             double initialValue)
  : ModelObject(ExternalInterfaceFunctionalMockupUnitImportToVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>());

  bool ok = setFMUFile(fMUFile);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to "
      << fMUFile.fMUFileName() << ".");
  }
  setFMUInstanceName(fMUInstanceName);
  setFMUVariableName(fMUVariableName);
  setInitialValue(initialValue);
}

IddObjectType ExternalInterfaceFunctionalMockupUnitImportToVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Variable);
}

ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToVariable::fMUFile() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->fMUFile();
}

std::string ExternalInterfaceFunctionalMockupUnitImportToVariable::fMUInstanceName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->fMUInstanceName();
}

std::string ExternalInterfaceFunctionalMockupUnitImportToVariable::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->fMUVariableName();
}

double ExternalInterfaceFunctionalMockupUnitImportToVariable::initialValue() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->initialValue();
}

bool ExternalInterfaceFunctionalMockupUnitImportToVariable::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->setFMUFile(fMUFile);
}

bool ExternalInterfaceFunctionalMockupUnitImportToVariable::setFMUInstanceName(const std::string& fMUInstanceName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->setFMUInstanceName(fMUInstanceName);
}

bool ExternalInterfaceFunctionalMockupUnitImportToVariable::setFMUVariableName(const std::string& fMUVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->setFMUVariableName(fMUVariableName);
}

bool ExternalInterfaceFunctionalMockupUnitImportToVariable::setInitialValue(double initialValue) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitImportToVariable::ExternalInterfaceFunctionalMockupUnitImportToVariable(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
