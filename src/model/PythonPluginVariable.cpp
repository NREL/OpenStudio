/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
