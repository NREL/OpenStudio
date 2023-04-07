/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "PythonPluginOutputVariable.hpp"
#include "PythonPluginOutputVariable_Impl.hpp"

#include "PythonPluginVariable.hpp"
#include "PythonPluginVariable_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_OutputVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginOutputVariable_Impl::PythonPluginOutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginOutputVariable::iddObjectType());
    }

    PythonPluginOutputVariable_Impl::PythonPluginOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginOutputVariable::iddObjectType());
    }

    PythonPluginOutputVariable_Impl::PythonPluginOutputVariable_Impl(const PythonPluginOutputVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginOutputVariable_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType PythonPluginOutputVariable_Impl::iddObjectType() const {
      return PythonPluginOutputVariable::iddObjectType();
    }

    PythonPluginVariable PythonPluginOutputVariable_Impl::pythonPluginVariable() const {
      boost::optional<PythonPluginVariable> value = optionalPythonPluginVariable();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Python Plugin Variable attached.");
      }
      return value.get();
    }

    std::string PythonPluginOutputVariable_Impl::typeofDatainVariable() const {
      boost::optional<std::string> value = getString(OS_PythonPlugin_OutputVariableFields::TypeofDatainVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string PythonPluginOutputVariable_Impl::updateFrequency() const {
      boost::optional<std::string> value = getString(OS_PythonPlugin_OutputVariableFields::UpdateFrequency, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<std::string> PythonPluginOutputVariable_Impl::units() const {
      return getString(OS_PythonPlugin_OutputVariableFields::Units, true);
    }

    boost::optional<std::string> PythonPluginOutputVariable_Impl::resourceType() const {
      return getString(OS_PythonPlugin_OutputVariableFields::ResourceType, true);
    }

    boost::optional<std::string> PythonPluginOutputVariable_Impl::groupType() const {
      return getString(OS_PythonPlugin_OutputVariableFields::GroupType, true);
    }

    boost::optional<std::string> PythonPluginOutputVariable_Impl::endUseCategory() const {
      return getString(OS_PythonPlugin_OutputVariableFields::EndUseCategory, true);
    }

    boost::optional<std::string> PythonPluginOutputVariable_Impl::endUseSubcategory() const {
      return getString(OS_PythonPlugin_OutputVariableFields::EndUseSubcategory, true);
    }

    bool PythonPluginOutputVariable_Impl::setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable) {
      bool result = setPointer(OS_PythonPlugin_OutputVariableFields::PythonPluginVariableName, pythonPluginVariable.handle());
      return result;
    }

    bool PythonPluginOutputVariable_Impl::setTypeofDatainVariable(const std::string& typeofDatainVariable) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::TypeofDatainVariable, typeofDatainVariable);
      return result;
    }

    bool PythonPluginOutputVariable_Impl::setUpdateFrequency(const std::string& updateFrequency) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::UpdateFrequency, updateFrequency);
      return result;
    }

    bool PythonPluginOutputVariable_Impl::setUnits(const std::string& units) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::Units, units);
      OS_ASSERT(result);
      return result;
    }

    void PythonPluginOutputVariable_Impl::resetUnits() {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::Units, "");
      OS_ASSERT(result);
    }

    bool PythonPluginOutputVariable_Impl::setResourceType(const std::string& resourceType) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::ResourceType, resourceType);
      return result;
    }

    void PythonPluginOutputVariable_Impl::resetResourceType() {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::ResourceType, "");
      OS_ASSERT(result);
    }

    bool PythonPluginOutputVariable_Impl::setGroupType(const std::string& groupType) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::GroupType, groupType);
      return result;
    }

    void PythonPluginOutputVariable_Impl::resetGroupType() {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::GroupType, "");
      OS_ASSERT(result);
    }

    bool PythonPluginOutputVariable_Impl::setEndUseCategory(const std::string& endUseCategory) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::EndUseCategory, endUseCategory);
      return result;
    }

    void PythonPluginOutputVariable_Impl::resetEndUseCategory() {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::EndUseCategory, "");
      OS_ASSERT(result);
    }

    bool PythonPluginOutputVariable_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void PythonPluginOutputVariable_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_PythonPlugin_OutputVariableFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    boost::optional<PythonPluginVariable> PythonPluginOutputVariable_Impl::optionalPythonPluginVariable() const {
      return getObject<ModelObject>().getModelObjectTarget<PythonPluginVariable>(OS_PythonPlugin_OutputVariableFields::PythonPluginVariableName);
    }

  }  // namespace detail

  PythonPluginOutputVariable::PythonPluginOutputVariable(const PythonPluginVariable& pythonPluginVariable)
    : ModelObject(PythonPluginOutputVariable::iddObjectType(), pythonPluginVariable.model()) {
    OS_ASSERT(getImpl<detail::PythonPluginOutputVariable_Impl>());

    bool ok = true;
    ok = setPythonPluginVariable(pythonPluginVariable);
    OS_ASSERT(ok);
    ok = setTypeofDatainVariable("Averaged");
    OS_ASSERT(ok);
    ok = setUpdateFrequency("ZoneTimestep");
    OS_ASSERT(ok);
  }

  IddObjectType PythonPluginOutputVariable::iddObjectType() {
    return {IddObjectType::OS_PythonPlugin_OutputVariable};
  }

  std::vector<std::string> PythonPluginOutputVariable::typeofDatainVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PythonPlugin_OutputVariableFields::TypeofDatainVariable);
  }

  std::vector<std::string> PythonPluginOutputVariable::updateFrequencyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PythonPlugin_OutputVariableFields::UpdateFrequency);
  }

  std::vector<std::string> PythonPluginOutputVariable::resourceTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PythonPlugin_OutputVariableFields::ResourceType);
  }

  std::vector<std::string> PythonPluginOutputVariable::groupTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PythonPlugin_OutputVariableFields::GroupType);
  }

  std::vector<std::string> PythonPluginOutputVariable::endUseCategoryValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PythonPlugin_OutputVariableFields::EndUseCategory);
  }

  PythonPluginVariable PythonPluginOutputVariable::pythonPluginVariable() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->pythonPluginVariable();
  }

  std::string PythonPluginOutputVariable::typeofDatainVariable() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->typeofDatainVariable();
  }

  std::string PythonPluginOutputVariable::updateFrequency() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->updateFrequency();
  }

  boost::optional<std::string> PythonPluginOutputVariable::units() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->units();
  }

  boost::optional<std::string> PythonPluginOutputVariable::resourceType() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->resourceType();
  }

  boost::optional<std::string> PythonPluginOutputVariable::groupType() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->groupType();
  }

  boost::optional<std::string> PythonPluginOutputVariable::endUseCategory() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->endUseCategory();
  }

  boost::optional<std::string> PythonPluginOutputVariable::endUseSubcategory() const {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->endUseSubcategory();
  }

  bool PythonPluginOutputVariable::setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setPythonPluginVariable(pythonPluginVariable);
  }

  bool PythonPluginOutputVariable::setTypeofDatainVariable(const std::string& typeofDatainVariable) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setTypeofDatainVariable(typeofDatainVariable);
  }

  bool PythonPluginOutputVariable::setUpdateFrequency(const std::string& updateFrequency) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setUpdateFrequency(updateFrequency);
  }

  bool PythonPluginOutputVariable::setUnits(const std::string& units) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setUnits(units);
  }

  void PythonPluginOutputVariable::resetUnits() {
    getImpl<detail::PythonPluginOutputVariable_Impl>()->resetUnits();
  }

  bool PythonPluginOutputVariable::setResourceType(const std::string& resourceType) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setResourceType(resourceType);
  }

  void PythonPluginOutputVariable::resetResourceType() {
    getImpl<detail::PythonPluginOutputVariable_Impl>()->resetResourceType();
  }

  bool PythonPluginOutputVariable::setGroupType(const std::string& groupType) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setGroupType(groupType);
  }

  void PythonPluginOutputVariable::resetGroupType() {
    getImpl<detail::PythonPluginOutputVariable_Impl>()->resetGroupType();
  }

  bool PythonPluginOutputVariable::setEndUseCategory(const std::string& endUseCategory) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setEndUseCategory(endUseCategory);
  }

  void PythonPluginOutputVariable::resetEndUseCategory() {
    getImpl<detail::PythonPluginOutputVariable_Impl>()->resetEndUseCategory();
  }

  bool PythonPluginOutputVariable::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::PythonPluginOutputVariable_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void PythonPluginOutputVariable::resetEndUseSubcategory() {
    getImpl<detail::PythonPluginOutputVariable_Impl>()->resetEndUseSubcategory();
  }

  /// @cond
  PythonPluginOutputVariable::PythonPluginOutputVariable(std::shared_ptr<detail::PythonPluginOutputVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
