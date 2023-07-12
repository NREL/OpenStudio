/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PythonPluginVariable.hpp"
#include "PythonPluginVariable_Impl.hpp"

#include "PythonPluginTrendVariable.hpp"
#include "PythonPluginTrendVariable_Impl.hpp"
#include "PythonPluginOutputVariable.hpp"
#include "PythonPluginOutputVariable_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_Variable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginVariable_Impl::PythonPluginVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginVariable::iddObjectType());
    }

    PythonPluginVariable_Impl::PythonPluginVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginVariable::iddObjectType());
    }

    PythonPluginVariable_Impl::PythonPluginVariable_Impl(const PythonPluginVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginVariable_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType PythonPluginVariable_Impl::iddObjectType() const {
      return PythonPluginVariable::iddObjectType();
    }

    std::vector<PythonPluginTrendVariable> PythonPluginVariable_Impl::pythonPluginTrendVariables() const {
      return getObject<ModelObject>().getModelObjectSources<PythonPluginTrendVariable>(PythonPluginTrendVariable::iddObjectType());
    }

    std::vector<PythonPluginOutputVariable> PythonPluginVariable_Impl::pythonPluginOutputVariables() const {
      return getObject<ModelObject>().getModelObjectSources<PythonPluginOutputVariable>(PythonPluginOutputVariable::iddObjectType());
    }

    ModelObject PythonPluginVariable_Impl::clone(Model model) const {
      auto newVar = ModelObject_Impl::clone(model).cast<PythonPluginVariable>();

      for (const auto& child : pythonPluginTrendVariables()) {
        child.clone(model).cast<PythonPluginTrendVariable>().setPythonPluginVariable(newVar);
      }

      for (const auto& child : pythonPluginOutputVariables()) {
        child.clone(model).cast<PythonPluginOutputVariable>().setPythonPluginVariable(newVar);
      }

      return std::move(newVar);
    }

    std::vector<IdfObject> PythonPluginVariable_Impl::remove() {
      std::vector<IdfObject> result;
      auto trends = pythonPluginTrendVariables();
      auto outputs = pythonPluginOutputVariables();
      result.reserve(trends.size() + outputs.size() + 1);

      for (auto& child : trends) {
        result.emplace_back(child.remove().front());
      }

      for (auto& child : outputs) {
        result.emplace_back(child.remove().front());
      }

      result.emplace_back(ModelObject_Impl::remove().front());
      return result;
    }

  }  // namespace detail

  PythonPluginVariable::PythonPluginVariable(const Model& model) : ModelObject(PythonPluginVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PythonPluginVariable_Impl>());
  }

  IddObjectType PythonPluginVariable::iddObjectType() {
    return {IddObjectType::OS_PythonPlugin_Variable};
  }

  std::vector<PythonPluginTrendVariable> PythonPluginVariable::pythonPluginTrendVariables() const {
    return getImpl<detail::PythonPluginVariable_Impl>()->pythonPluginTrendVariables();
  }
  std::vector<PythonPluginOutputVariable> PythonPluginVariable::pythonPluginOutputVariables() const {
    return getImpl<detail::PythonPluginVariable_Impl>()->pythonPluginOutputVariables();
  }

  /// @cond
  PythonPluginVariable::PythonPluginVariable(std::shared_ptr<detail::PythonPluginVariable_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
