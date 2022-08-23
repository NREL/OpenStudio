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
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const TableIndependentVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableIndependentVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType TableIndependentVariable_Impl::iddObjectType() const {
      return TableIndependentVariable::iddObjectType();
    }

    std::string interpolationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::InterpolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string extrapolationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::ExtrapolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string unitType() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::UnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<TableLookup> tableLookups() const {
      std::vector<TableLookup> result;
      return result;
    }

    bool setInterpolationMethod(std::string interpolationMethod) {
      bool result = setString(OS_Table_IndependentVariableFields::InterpolationMethod, interpolationMethod);
      return result;
    }

    bool setExtrapolationMethod(std::string extrapolationMethod) {
      bool result = setString(OS_Table_IndependentVariableFields::ExtrapolationMethod, extrapolationMethod);
      return result;
    }

    bool setUnitType(std::string unitType) {
      bool result = setString(OS_Table_IndependentVariableFields::UnitType, unitType);
      return result;
    }

  }  // namespace detail

  TableIndependentVariable::TableIndependentVariable(const Model& model) : ResourceObject(TableIndependentVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableIndependentVariable_Impl>());

    bool ok = true;
    ok = setInterpolationMethod("Linear");
    OS_ASSERT(ok);
    ok = setExtrapolationMethod("Constant");
    OS_ASSERT(ok);
    ok = setUnitType("Dimensionless");
    OS_ASSERT(ok);
  }

  IddObjectType TableIndependentVariable::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Table_IndependentVariable);
  }

  std::string interpolationMethod() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->interpolationMethod();
  }

  std::string extrapolationMethod() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->extrapolationMethod();
  }

  std::string unitType() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->unitType();
  }

  std::vector<TableLookup> tableLookups() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->tableLookups();
  }

  bool setInterpolationMethod(std::string interpolationMethod) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setInterpolationMethod(interpolationMethod);
  }

  bool setExtrapolationMethod(std::string extrapolationMethod) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setExtrapolationMethod(extrapolationMethod);
  }

  bool setUnitType(std::string unitType) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setUnitType(unitType);
  }

  /// @cond
  TableIndependentVariable::TableIndependentVariable(std::shared_ptr<detail::TableIndependentVariable_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
