/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
