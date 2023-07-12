/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputConstructions.hpp"
#include "OutputConstructions_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Constructions_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputConstructions_Impl::OutputConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputConstructions::iddObjectType());
    }

    OutputConstructions_Impl::OutputConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputConstructions::iddObjectType());
    }

    OutputConstructions_Impl::OutputConstructions_Impl(const OutputConstructions_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputConstructions_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputConstructions_Impl::iddObjectType() const {
      return OutputConstructions::iddObjectType();
    }

    bool OutputConstructions_Impl::reportConstructions() const {
      return getBooleanFieldValue(OS_Output_ConstructionsFields::ReportConstructions);
    }

    bool OutputConstructions_Impl::reportMaterials() const {
      return getBooleanFieldValue(OS_Output_ConstructionsFields::ReportMaterials);
    }

    bool OutputConstructions_Impl::setReportConstructions(bool reportConstructions) {
      return setBooleanFieldValue(OS_Output_ConstructionsFields::ReportConstructions, reportConstructions);
    }

    bool OutputConstructions_Impl::setReportMaterials(bool reportMaterials) {
      return setBooleanFieldValue(OS_Output_ConstructionsFields::ReportMaterials, reportMaterials);
    }

  }  // namespace detail

  IddObjectType OutputConstructions::iddObjectType() {
    return {IddObjectType::OS_Output_Constructions};
  }

  bool OutputConstructions::reportConstructions() const {
    return getImpl<detail::OutputConstructions_Impl>()->reportConstructions();
  }

  bool OutputConstructions::reportMaterials() const {
    return getImpl<detail::OutputConstructions_Impl>()->reportMaterials();
  }

  bool OutputConstructions::setReportConstructions(bool reportConstructions) {
    return getImpl<detail::OutputConstructions_Impl>()->setReportConstructions(reportConstructions);
  }

  bool OutputConstructions::setReportMaterials(bool reportMaterials) {
    return getImpl<detail::OutputConstructions_Impl>()->setReportMaterials(reportMaterials);
  }

  /// @cond
  OutputConstructions::OutputConstructions(std::shared_ptr<detail::OutputConstructions_Impl> impl) : ModelObject(std::move(impl)) {}
  OutputConstructions::OutputConstructions(Model& model) : ModelObject(OutputConstructions::iddObjectType(), model) {
    bool ok = setReportConstructions(true);
    OS_ASSERT(ok);
    ok = setReportMaterials(false);
    OS_ASSERT(ok);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
