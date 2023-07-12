/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputSchedules.hpp"
#include "OutputSchedules_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Schedules_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputSchedules_Impl::OutputSchedules_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputSchedules::iddObjectType());
    }

    OutputSchedules_Impl::OutputSchedules_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputSchedules::iddObjectType());
    }

    OutputSchedules_Impl::OutputSchedules_Impl(const OutputSchedules_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputSchedules_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputSchedules_Impl::iddObjectType() const {
      return OutputSchedules::iddObjectType();
    }

    std::string OutputSchedules_Impl::keyField() const {
      boost::optional<std::string> value = getString(OS_Output_SchedulesFields::KeyField, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputSchedules_Impl::setKeyField(const std::string& keyField) {
      bool result = setString(OS_Output_SchedulesFields::KeyField, keyField);
      return result;
    }

  }  // namespace detail

  IddObjectType OutputSchedules::iddObjectType() {
    return {IddObjectType::OS_Output_Schedules};
  }

  std::vector<std::string> OutputSchedules::keyFieldValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_SchedulesFields::KeyField);
  }

  std::vector<std::string> OutputSchedules::validKeyFieldValues() {
    return keyFieldValues();
  }

  std::string OutputSchedules::keyField() const {
    return getImpl<detail::OutputSchedules_Impl>()->keyField();
  }

  bool OutputSchedules::setKeyField(const std::string& keyField) {
    return getImpl<detail::OutputSchedules_Impl>()->setKeyField(keyField);
  }

  /// @cond
  OutputSchedules::OutputSchedules(std::shared_ptr<detail::OutputSchedules_Impl> impl) : ModelObject(impl) {}
  OutputSchedules::OutputSchedules(Model& model) : ModelObject(OutputSchedules::iddObjectType(), model) {
    bool ok = setKeyField("Hourly");
    OS_ASSERT(ok);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
