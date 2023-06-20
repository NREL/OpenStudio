/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
