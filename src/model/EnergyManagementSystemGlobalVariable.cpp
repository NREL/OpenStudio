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

#include "EnergyManagementSystemGlobalVariable.hpp"
#include "EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                         bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
    }

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
    }

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const EnergyManagementSystemGlobalVariable_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemGlobalVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemGlobalVariable_Impl::iddObjectType() const {
      return EnergyManagementSystemGlobalVariable::iddObjectType();
    }

    bool EnergyManagementSystemGlobalVariable_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool EnergyManagementSystemGlobalVariable_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB);
    }

    bool EnergyManagementSystemGlobalVariable_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void EnergyManagementSystemGlobalVariable_Impl::resetExportToBCVTB() {
      bool result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(const Model& model, const std::string& variableName)
    : ModelObject(EnergyManagementSystemGlobalVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>());
    bool ok = getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->setName(variableName).has_value();
    if ((!ok) || (variableName != this->nameString())) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << variableName << ".");
    }
  }

  IddObjectType EnergyManagementSystemGlobalVariable::iddObjectType() {
    return IddObjectType(IddObjectType::OS_EnergyManagementSystem_GlobalVariable);
  }

  bool EnergyManagementSystemGlobalVariable::exportToBCVTB() const {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->exportToBCVTB();
  }

  bool EnergyManagementSystemGlobalVariable::isExportToBCVTBDefaulted() const {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->isExportToBCVTBDefaulted();
  }

  bool EnergyManagementSystemGlobalVariable::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void EnergyManagementSystemGlobalVariable::resetExportToBCVTB() {
    getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->resetExportToBCVTB();
  }

  /// @cond
  EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
