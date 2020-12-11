/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ExternalInterfaceFunctionalMockupUnitImportFromVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableIndexKeyName() const {
      boost::optional<std::string> value =
        getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUFile() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName)
        ->cast<ExternalInterfaceFunctionalMockupUnitImport>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUInstanceName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
      bool result =
        setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, outputVariableIndexKeyName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setOutputVariableName(const std::string& outputVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, outputVariableName);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName, fMUFile.handle());
      //TODO in forward translator set the field to the .fMUFileName()
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, fMUInstanceName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitImportFromVariable::ExternalInterfaceFunctionalMockupUnitImportFromVariable(
    const Model& model, const std::string& outputVariableIndexKeyName, const std::string& outputVariableName,
    const ExternalInterfaceFunctionalMockupUnitImport& fMUFile, const std::string& fMUInstanceName, const std::string& fMUVariableName)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>());

    setOutputVariableIndexKeyName(outputVariableIndexKeyName);
    bool ok = setOutputVariableName(outputVariableName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableName to " << outputVariableName << ".");
    }
    ok = setFMUFile(fMUFile);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to " << fMUFile.fMUFileName() << ".");
    }
    setFMUInstanceName(fMUInstanceName);
    setFMUVariableName(fMUVariableName);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable);
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::outputVariableIndexKeyName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->outputVariableIndexKeyName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::outputVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->outputVariableName();
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUFile() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUFile();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUInstanceName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUInstanceName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUVariableName();
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setOutputVariableIndexKeyName(outputVariableIndexKeyName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setOutputVariableName(const std::string& outputVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setOutputVariableName(outputVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUFile(fMUFile);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUInstanceName(const std::string& fMUInstanceName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUInstanceName(fMUInstanceName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUVariableName(fMUVariableName);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitImportFromVariable::ExternalInterfaceFunctionalMockupUnitImportFromVariable(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
