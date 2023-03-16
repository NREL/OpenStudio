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

#include "ZoneAirHeatBalanceAlgorithm.hpp"
#include "ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneAirHeatBalanceAlgorithm::iddObjectType());
    }

    ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneAirHeatBalanceAlgorithm::iddObjectType());
    }

    ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const ZoneAirHeatBalanceAlgorithm_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ZoneAirHeatBalanceAlgorithm_Impl::parent() const {
      boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool ZoneAirHeatBalanceAlgorithm_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ZoneAirHeatBalanceAlgorithm_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate
      return result;
    }

    IddObjectType ZoneAirHeatBalanceAlgorithm_Impl::iddObjectType() const {
      return ZoneAirHeatBalanceAlgorithm::iddObjectType();
    }

    std::string ZoneAirHeatBalanceAlgorithm_Impl::algorithm() const {
      boost::optional<std::string> value = getString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::isAlgorithmDefaulted() const {
      return isEmpty(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::doSpaceHeatBalanceforSizing() const {
      return getBooleanFieldValue(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::isDoSpaceHeatBalanceforSizingDefaulted() const {
      return isEmpty(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::doSpaceHeatBalanceforSimulation() const {
      return getBooleanFieldValue(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::isDoSpaceHeatBalanceforSimulationDefaulted() const {
      return isEmpty(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::setAlgorithm(const std::string& algorithm) {
      bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm, algorithm);
      return result;
    }

    void ZoneAirHeatBalanceAlgorithm_Impl::resetAlgorithm() {
      bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm, "");
      OS_ASSERT(result);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::setDoSpaceHeatBalanceforSizing(bool doSpaceHeatBalanceforSizing) {
      this->getBooleanFieldValue(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing);
      bool result = false;
      if (doSpaceHeatBalanceforSizing) {
        result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "Yes");
      } else {
        result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void ZoneAirHeatBalanceAlgorithm_Impl::resetDoSpaceHeatBalanceforSizing() {
      bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "");
      OS_ASSERT(result);
    }

    bool ZoneAirHeatBalanceAlgorithm_Impl::setDoSpaceHeatBalanceforSimulation(bool doSpaceHeatBalanceforSimulation) {
      bool result = false;
      if (doSpaceHeatBalanceforSimulation) {
        result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "Yes");
      } else {
        result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void ZoneAirHeatBalanceAlgorithm_Impl::resetDoSpaceHeatBalanceforSimulation() {
      bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType ZoneAirHeatBalanceAlgorithm::iddObjectType() {
    return {IddObjectType::OS_ZoneAirHeatBalanceAlgorithm};
  }

  std::vector<std::string> ZoneAirHeatBalanceAlgorithm::validAlgorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm);
  }

  std::string ZoneAirHeatBalanceAlgorithm::algorithm() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->algorithm();
  }

  bool ZoneAirHeatBalanceAlgorithm::isAlgorithmDefaulted() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->isAlgorithmDefaulted();
  }

  bool ZoneAirHeatBalanceAlgorithm::doSpaceHeatBalanceforSizing() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->doSpaceHeatBalanceforSizing();
  }

  bool ZoneAirHeatBalanceAlgorithm::isDoSpaceHeatBalanceforSizingDefaulted() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->isDoSpaceHeatBalanceforSizingDefaulted();
  }

  bool ZoneAirHeatBalanceAlgorithm::doSpaceHeatBalanceforSimulation() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->doSpaceHeatBalanceforSimulation();
  }

  bool ZoneAirHeatBalanceAlgorithm::isDoSpaceHeatBalanceforSimulationDefaulted() const {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->isDoSpaceHeatBalanceforSimulationDefaulted();
  }

  bool ZoneAirHeatBalanceAlgorithm::setAlgorithm(const std::string& algorithm) {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->setAlgorithm(algorithm);
  }

  void ZoneAirHeatBalanceAlgorithm::resetAlgorithm() {
    getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->resetAlgorithm();
  }

  bool ZoneAirHeatBalanceAlgorithm::setDoSpaceHeatBalanceforSizing(bool doSpaceHeatBalanceforSizing) {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->setDoSpaceHeatBalanceforSizing(doSpaceHeatBalanceforSizing);
  }

  void ZoneAirHeatBalanceAlgorithm::resetDoSpaceHeatBalanceforSizing() {
    getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->resetDoSpaceHeatBalanceforSizing();
  }

  bool ZoneAirHeatBalanceAlgorithm::setDoSpaceHeatBalanceforSimulation(bool doSpaceHeatBalanceforSimulation) {
    return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->setDoSpaceHeatBalanceforSimulation(doSpaceHeatBalanceforSimulation);
  }

  void ZoneAirHeatBalanceAlgorithm::resetDoSpaceHeatBalanceforSimulation() {
    getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->resetDoSpaceHeatBalanceforSimulation();
  }

  /// @cond
  ZoneAirHeatBalanceAlgorithm::ZoneAirHeatBalanceAlgorithm(std::shared_ptr<detail::ZoneAirHeatBalanceAlgorithm_Impl> impl)
    : ModelObject(std::move(impl)) {}

  ZoneAirHeatBalanceAlgorithm::ZoneAirHeatBalanceAlgorithm(Model& model) : ModelObject(ZoneAirHeatBalanceAlgorithm::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>());

    // Leaving the IDD defaults
    // Algorithm: ThirdOrderBackwardDifference
    // DoSpaceHeatBalanceforSizing: false
    // DoSpaceHeatBalanceforSimulation: false
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
