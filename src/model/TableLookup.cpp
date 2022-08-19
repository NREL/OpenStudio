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

#include "TableLookup.hpp"
#include "TableLookup_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

#include <algorithm>
#include <iomanip>

namespace openstudio {
namespace model {

  namespace detail {

    TableLookup_Impl::TableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableLookup::iddObjectType());
    }

    TableLookup_Impl::TableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableLookup::iddObjectType());
    }

    TableLookup_Impl::TableLookup_Impl(const TableLookup_Impl& other, Model_Impl* model, bool keepHandle) : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableLookup_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType TableLookup_Impl::iddObjectType() const {
      return TableLookup::iddObjectType();
    }

    int TableLookup_Impl::numVariables() const {
      return 1;
    }

    double TableLookup_Impl::evaluate(const std::vector<double>& independentVariables) const {
      LOG(Warn, "Curve evaluation isn't implemented for TableLookup");
      return -9999.0;
    }

  }  // namespace detail

  TableLookup::TableLookup(const Model& model) : Curve(TableLookup::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableLookup_Impl>());
  }

  IddObjectType TableLookup::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Table_Lookup);
  }

  /// @cond
  TableLookup::TableLookup(std::shared_ptr<detail::TableLookup_Impl> impl) : Curve(std::dynamic_pointer_cast<detail::Curve_Impl>(std::move(impl))) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
