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

#include "ExternalInterfaceVariable.hpp"
#include "ExternalInterfaceVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Variable_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceVariable::iddObjectType());
  }

  ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceVariable::iddObjectType());
  }

  ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const ExternalInterfaceVariable_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceVariable_Impl::iddObjectType() const {
    return ExternalInterfaceVariable::iddObjectType();
  }

  double ExternalInterfaceVariable_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_VariableFields::InitialValue,true);
    if (value) {
      return value.get();
    }
    return -9999;
  }

  bool ExternalInterfaceVariable_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_VariableFields::InitialValue, initialValue);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceVariable_Impl::exportToBCVTB() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_VariableFields::ExportToBCVTB, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool ExternalInterfaceVariable_Impl::isExportToBCVTBDefaulted() const {
    return isEmpty(OS_ExternalInterface_VariableFields::ExportToBCVTB);
  }

  bool ExternalInterfaceVariable_Impl::setExportToBCVTB(bool exportToBCVTB) {
    bool result = false;
    if (exportToBCVTB) {
      result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "True");
    } else {
      result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "False");
    }
    OS_ASSERT(result);
    return result;
  }

  void ExternalInterfaceVariable_Impl::resetExportToBCVTB() {
    bool result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "");
    OS_ASSERT(result);
  }

} // detail

ExternalInterfaceVariable::ExternalInterfaceVariable(const Model& model, const std::string& variableName, double initialValue)
  : ModelObject(ExternalInterfaceVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceVariable_Impl>());
  bool ok = getImpl<detail::ExternalInterfaceVariable_Impl>()->setName(variableName);
  if ( (!ok) || (variableName != this->nameString() )) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << variableName << ".");
  }
  setInitialValue(initialValue);
}

IddObjectType ExternalInterfaceVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_Variable);
}

double ExternalInterfaceVariable::initialValue() const {
  return getImpl<detail::ExternalInterfaceVariable_Impl>()->initialValue();
}

bool ExternalInterfaceVariable::setInitialValue(double initialValue) {
  return getImpl<detail::ExternalInterfaceVariable_Impl>()->setInitialValue(initialValue);
}

bool ExternalInterfaceVariable::exportToBCVTB() const {
  return getImpl<detail::ExternalInterfaceVariable_Impl>()->exportToBCVTB();
}

bool ExternalInterfaceVariable::isExportToBCVTBDefaulted() const {
  return getImpl<detail::ExternalInterfaceVariable_Impl>()->isExportToBCVTBDefaulted();
}

bool ExternalInterfaceVariable::setExportToBCVTB(bool exportToBCVTB) {
  return getImpl<detail::ExternalInterfaceVariable_Impl>()->setExportToBCVTB(exportToBCVTB);
}

void ExternalInterfaceVariable::resetExportToBCVTB() {
  getImpl<detail::ExternalInterfaceVariable_Impl>()->resetExportToBCVTB();
}

/// @cond
ExternalInterfaceVariable::ExternalInterfaceVariable(std::shared_ptr<detail::ExternalInterfaceVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
