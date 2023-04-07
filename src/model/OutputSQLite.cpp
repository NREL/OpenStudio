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

#include "OutputSQLite.hpp"
#include "OutputSQLite_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_SQLite_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputSQLite_Impl::OutputSQLite_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputSQLite::iddObjectType());
    }

    OutputSQLite_Impl::OutputSQLite_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputSQLite::iddObjectType());
    }

    OutputSQLite_Impl::OutputSQLite_Impl(const OutputSQLite_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputSQLite_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputSQLite_Impl::iddObjectType() const {
      return OutputSQLite::iddObjectType();
    }

    std::string OutputSQLite_Impl::optionType() const {
      boost::optional<std::string> value = getString(OS_Output_SQLiteFields::OptionType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string OutputSQLite_Impl::unitConversionforTabularData() const {
      boost::optional<std::string> value = getString(OS_Output_SQLiteFields::UnitConversionforTabularData, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputSQLite_Impl::setOptionType(const std::string& optionType) {
      bool result = setString(OS_Output_SQLiteFields::OptionType, optionType);
      return result;
    }

    bool OutputSQLite_Impl::setUnitConversionforTabularData(const std::string& unitConversionforTabularData) {
      bool result = setString(OS_Output_SQLiteFields::UnitConversionforTabularData, unitConversionforTabularData);
      return result;
    }

  }  // namespace detail

  IddObjectType OutputSQLite::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Output_SQLite);
  }

  std::vector<std::string> OutputSQLite::optionTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_SQLiteFields::OptionType);
  }

  std::vector<std::string> OutputSQLite::unitConversionforTabularDataValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_SQLiteFields::UnitConversionforTabularData);
  }

  std::string OutputSQLite::optionType() const {
    return getImpl<detail::OutputSQLite_Impl>()->optionType();
  }

  std::string OutputSQLite::unitConversionforTabularData() const {
    return getImpl<detail::OutputSQLite_Impl>()->unitConversionforTabularData();
  }

  bool OutputSQLite::setOptionType(const std::string& optionType) {
    return getImpl<detail::OutputSQLite_Impl>()->setOptionType(optionType);
  }

  bool OutputSQLite::setUnitConversionforTabularData(const std::string& unitConversionforTabularData) {
    return getImpl<detail::OutputSQLite_Impl>()->setUnitConversionforTabularData(unitConversionforTabularData);
  }

  /// @cond
  OutputSQLite::OutputSQLite(std::shared_ptr<detail::OutputSQLite_Impl> impl) : ModelObject(impl) {}
  OutputSQLite::OutputSQLite(Model& model) : ModelObject(OutputSQLite::iddObjectType(), model) {
    bool ok = setOptionType("SimpleAndTabular");
    OS_ASSERT(ok);
    ok = setUnitConversionforTabularData("UseOutputControlTableStyle");
    OS_ASSERT(ok);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
