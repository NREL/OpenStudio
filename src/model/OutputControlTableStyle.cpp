/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputControlTableStyle.hpp"
#include "OutputControlTableStyle_Impl.hpp"

#include <utilities/idd/OS_OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputControlTableStyle_Impl::OutputControlTableStyle_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputControlTableStyle::iddObjectType());
    }

    OutputControlTableStyle_Impl::OutputControlTableStyle_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputControlTableStyle::iddObjectType());
    }

    OutputControlTableStyle_Impl::OutputControlTableStyle_Impl(const OutputControlTableStyle_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputControlTableStyle_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputControlTableStyle_Impl::iddObjectType() const {
      return OutputControlTableStyle::iddObjectType();
    }

    std::string OutputControlTableStyle_Impl::columnSeparator() const {
      boost::optional<std::string> value = getString(OS_OutputControl_Table_StyleFields::ColumnSeparator, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string OutputControlTableStyle_Impl::unitConversion() const {
      boost::optional<std::string> value = getString(OS_OutputControl_Table_StyleFields::UnitConversion, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputControlTableStyle_Impl::setColumnSeparator(const std::string& columnSeparator) {
      bool result = setString(OS_OutputControl_Table_StyleFields::ColumnSeparator, columnSeparator);
      return result;
    }

    bool OutputControlTableStyle_Impl::setUnitConversion(const std::string& unitConversion) {
      bool result = setString(OS_OutputControl_Table_StyleFields::UnitConversion, unitConversion);
      return result;
    }

  }  // namespace detail

  IddObjectType OutputControlTableStyle::iddObjectType() {
    return IddObjectType(IddObjectType::OS_OutputControl_Table_Style);
  }

  std::vector<std::string> OutputControlTableStyle::columnSeparatorValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_OutputControl_Table_StyleFields::ColumnSeparator);
  }

  std::vector<std::string> OutputControlTableStyle::unitConversionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_OutputControl_Table_StyleFields::UnitConversion);
  }

  std::string OutputControlTableStyle::columnSeparator() const {
    return getImpl<detail::OutputControlTableStyle_Impl>()->columnSeparator();
  }

  std::string OutputControlTableStyle::unitConversion() const {
    return getImpl<detail::OutputControlTableStyle_Impl>()->unitConversion();
  }

  bool OutputControlTableStyle::setColumnSeparator(const std::string& columnSeparator) {
    return getImpl<detail::OutputControlTableStyle_Impl>()->setColumnSeparator(columnSeparator);
  }

  bool OutputControlTableStyle::setUnitConversion(const std::string& unitConversion) {
    return getImpl<detail::OutputControlTableStyle_Impl>()->setUnitConversion(unitConversion);
  }

  /// @cond
  OutputControlTableStyle::OutputControlTableStyle(std::shared_ptr<detail::OutputControlTableStyle_Impl> impl) : ModelObject(std::move(impl)) {}
  OutputControlTableStyle::OutputControlTableStyle(Model& model) : ModelObject(OutputControlTableStyle::iddObjectType(), model) {
    bool ok = setColumnSeparator("HTML");
    OS_ASSERT(ok);
    ok = setUnitConversion("None");
    OS_ASSERT(ok);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
