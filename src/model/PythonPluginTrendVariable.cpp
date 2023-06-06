/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PythonPluginTrendVariable.hpp"
#include "PythonPluginTrendVariable_Impl.hpp"

#include "PythonPluginVariable.hpp"
#include "PythonPluginVariable_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_TrendVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginTrendVariable_Impl::PythonPluginTrendVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginTrendVariable::iddObjectType());
    }

    PythonPluginTrendVariable_Impl::PythonPluginTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginTrendVariable::iddObjectType());
    }

    PythonPluginTrendVariable_Impl::PythonPluginTrendVariable_Impl(const PythonPluginTrendVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginTrendVariable_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType PythonPluginTrendVariable_Impl::iddObjectType() const {
      return PythonPluginTrendVariable::iddObjectType();
    }

    boost::optional<PythonPluginVariable> PythonPluginTrendVariable_Impl::optionalPythonPluginVariable() const {
      return getObject<ModelObject>().getModelObjectTarget<PythonPluginVariable>(OS_PythonPlugin_TrendVariableFields::NameofaPythonPluginVariable);
    }

    PythonPluginVariable PythonPluginTrendVariable_Impl::pythonPluginVariable() const {
      boost::optional<PythonPluginVariable> value = optionalPythonPluginVariable();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a PythonPluginVariable attached.");
      }
      return value.get();
    }

    int PythonPluginTrendVariable_Impl::numberofTimestepstobeLogged() const {
      boost::optional<int> value = getInt(OS_PythonPlugin_TrendVariableFields::NumberofTimestepstobeLogged, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PythonPluginTrendVariable_Impl::setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable) {
      bool result = setPointer(OS_PythonPlugin_TrendVariableFields::NameofaPythonPluginVariable, pythonPluginVariable.handle());
      return result;
    }

    bool PythonPluginTrendVariable_Impl::setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged) {
      bool result = setInt(OS_PythonPlugin_TrendVariableFields::NumberofTimestepstobeLogged, numberofTimestepstobeLogged);
      return result;
    }

  }  // namespace detail

  PythonPluginTrendVariable::PythonPluginTrendVariable(const PythonPluginVariable& pythonPluginVariable)
    : ModelObject(PythonPluginTrendVariable::iddObjectType(), pythonPluginVariable.model()) {
    OS_ASSERT(getImpl<detail::PythonPluginTrendVariable_Impl>());

    bool ok = true;
    ok = setPythonPluginVariable(pythonPluginVariable);
    OS_ASSERT(ok);
    ok = setNumberofTimestepstobeLogged(4);
    OS_ASSERT(ok);
  }

  IddObjectType PythonPluginTrendVariable::iddObjectType() {
    return {IddObjectType::OS_PythonPlugin_TrendVariable};
  }

  PythonPluginVariable PythonPluginTrendVariable::pythonPluginVariable() const {
    return getImpl<detail::PythonPluginTrendVariable_Impl>()->pythonPluginVariable();
  }

  int PythonPluginTrendVariable::numberofTimestepstobeLogged() const {
    return getImpl<detail::PythonPluginTrendVariable_Impl>()->numberofTimestepstobeLogged();
  }

  bool PythonPluginTrendVariable::setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable) {
    return getImpl<detail::PythonPluginTrendVariable_Impl>()->setPythonPluginVariable(pythonPluginVariable);
  }

  bool PythonPluginTrendVariable::setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged) {
    return getImpl<detail::PythonPluginTrendVariable_Impl>()->setNumberofTimestepstobeLogged(numberofTimestepstobeLogged);
  }

  /// @cond
  PythonPluginTrendVariable::PythonPluginTrendVariable(std::shared_ptr<detail::PythonPluginTrendVariable_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
