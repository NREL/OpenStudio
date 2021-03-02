/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "OutputControlReportingTolerances.hpp"
#include "OutputControlReportingTolerances_Impl.hpp"

#include <utilities/idd/OS_OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
    }

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
    }

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const OutputControlReportingTolerances_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputControlReportingTolerances_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputControlReportingTolerances_Impl::iddObjectType() const {
      return OutputControlReportingTolerances::iddObjectType();
    }

    double OutputControlReportingTolerances_Impl::toleranceforTimeHeatingSetpointNotMet() const {
      boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputControlReportingTolerances_Impl::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
      return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet);
    }

    double OutputControlReportingTolerances_Impl::toleranceforTimeCoolingSetpointNotMet() const {
      boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputControlReportingTolerances_Impl::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
      return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet);
    }

    bool OutputControlReportingTolerances_Impl::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
      bool result =
        setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, toleranceforTimeHeatingSetpointNotMet);
      return result;
    }

    void OutputControlReportingTolerances_Impl::resetToleranceforTimeHeatingSetpointNotMet() {
      bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, "");
      OS_ASSERT(result);
    }

    bool OutputControlReportingTolerances_Impl::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
      bool result =
        setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, toleranceforTimeCoolingSetpointNotMet);
      return result;
    }

    void OutputControlReportingTolerances_Impl::resetToleranceforTimeCoolingSetpointNotMet() {
      bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType OutputControlReportingTolerances::iddObjectType() {
    return IddObjectType(IddObjectType::OS_OutputControl_ReportingTolerances);
  }

  double OutputControlReportingTolerances::toleranceforTimeHeatingSetpointNotMet() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeHeatingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeHeatingSetpointNotMetDefaulted();
  }

  double OutputControlReportingTolerances::toleranceforTimeCoolingSetpointNotMet() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeCoolingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeCoolingSetpointNotMetDefaulted();
  }

  bool OutputControlReportingTolerances::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeHeatingSetpointNotMet(toleranceforTimeHeatingSetpointNotMet);
  }

  void OutputControlReportingTolerances::resetToleranceforTimeHeatingSetpointNotMet() {
    getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeHeatingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeCoolingSetpointNotMet(toleranceforTimeCoolingSetpointNotMet);
  }

  void OutputControlReportingTolerances::resetToleranceforTimeCoolingSetpointNotMet() {
    getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeCoolingSetpointNotMet();
  }

  /// @cond
  OutputControlReportingTolerances::OutputControlReportingTolerances(std::shared_ptr<detail::OutputControlReportingTolerances_Impl> impl)
    : ModelObject(std::move(impl)) {}
  OutputControlReportingTolerances::OutputControlReportingTolerances(Model& model)
    : ModelObject(OutputControlReportingTolerances::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
