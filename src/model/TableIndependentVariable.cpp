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

#include "TableIndependentVariable.hpp"
#include "TableIndependentVariable_Impl.hpp"

#include "TableLookup.hpp"
#include "TableLookup_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Table_IndependentVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const TableIndependentVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableIndependentVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType TableIndependentVariable_Impl::iddObjectType() const {
      return TableIndependentVariable::iddObjectType();
    }

    ModelObject TableIndependentVariable_Impl::clone(Model model) const {
      auto t_clone = ParentObject_Impl::clone(model).cast<TableIndependentVariable>();

      return t_clone;
    }

    std::vector<ModelObject> TableIndependentVariable_Impl::children() const {
      std::vector<ModelObject> children;

      return children;
    }

  }  // namespace detail

  TableIndependentVariable::TableIndependentVariable(const Model& model) : ParentObject(TableIndependentVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableIndependentVariable_Impl>());

    bool ok = true;

    OS_ASSERT(ok);
  }

  IddObjectType TableIndependentVariable::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Table_IndependentVariable);
  }

  /// @cond
  TableIndependentVariable::TableIndependentVariable(std::shared_ptr<detail::TableIndependentVariable_Impl> impl) : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
