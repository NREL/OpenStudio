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

#include "ExternalInterfaceFunctionalMockupUnitExportFromVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const IdfObject& idfObject,
                                                                                                                             Model_Impl* model,
                                                                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                             Model_Impl* model,
                                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl& other,
                                                                                                                             Model_Impl* model,
                                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableIndexKeyName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::fMUVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, outputVariableIndexKeyName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setOutputVariableName(const std::string& outputVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, outputVariableName);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
    return result;
  }

} // detail

ExternalInterfaceFunctionalMockupUnitExportFromVariable::ExternalInterfaceFunctionalMockupUnitExportFromVariable(const Model& model,
                                                                                                                 const std::string& outputVariableIndexKeyName,
                                                                                                                 const std::string& outputVariableName,
                                                                                                                 const std::string& fMUVariableName)
  : ModelObject(ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>());

  setOutputVariableIndexKeyName(outputVariableIndexKeyName);
  bool ok = setOutputVariableName(outputVariableName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableName to "
      << outputVariableName << ".");
  }
  setFMUVariableName(fMUVariableName);
}

IddObjectType ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable);
}

std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::outputVariableIndexKeyName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->outputVariableIndexKeyName();
}

std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::outputVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->outputVariableName();
}

std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->fMUVariableName();
}

bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setOutputVariableIndexKeyName(outputVariableIndexKeyName);
}

bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setOutputVariableName(const std::string& outputVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setOutputVariableName(outputVariableName);
}

bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setFMUVariableName(const std::string& fMUVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setFMUVariableName(fMUVariableName);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitExportFromVariable::ExternalInterfaceFunctionalMockupUnitExportFromVariable(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
