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
