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
