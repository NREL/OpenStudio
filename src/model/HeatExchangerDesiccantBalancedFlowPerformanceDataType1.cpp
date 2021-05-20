/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HeatExchangerDesiccantBalancedFlow.hpp"
#include "HeatExchangerDesiccantBalancedFlow_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::iddObjectType() const {
      return HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType();
    }

    ModelObject HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::clone(Model model) const {
      // clone the operating modes is already handle in ModelObject_Impl::clone since they are ResourceObjects
      // We don't do ParentObject_Impl::clone since it'll also CLONE the children...
      return ModelObject_Impl::clone(model);
    }

    std::vector<ModelObject> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::children() const {
      std::vector<ModelObject> result;



      return result;
    }

    std::vector<IdfObject> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::remove() {
      if (!heatExchangerDesiccantBalancedFlows().empty()) {
        LOG(Warn, "Cannot remove object because it is used by at least one heatExchangerDesiccantBalancedFlow as a required field");
        return std::vector<IdfObject>();
      }
      return ResourceObject_Impl::remove();
    }

    std::vector<HeatExchangerDesiccantBalancedFlow> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::heatExchangerDesiccantBalancedFlows() const {
      return getObject<ModelObject>().getModelObjectSources<HeatExchangerDesiccantBalancedFlow>(HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

  }  // namespace detail

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1::HeatExchangerDesiccantBalancedFlowPerformanceDataType1(const Model& model)
    : ResourceObject(HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>());


  }

  IddObjectType HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1);
  }

  std::vector<HeatExchangerDesiccantBalancedFlow> HeatExchangerDesiccantBalancedFlowPerformanceDataType1::heatExchangerDesiccantBalancedFlows() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->heatExchangerDesiccantBalancedFlows();
  }

  /// @cond
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1::HeatExchangerDesiccantBalancedFlowPerformanceDataType1(std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl> impl)
    : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
