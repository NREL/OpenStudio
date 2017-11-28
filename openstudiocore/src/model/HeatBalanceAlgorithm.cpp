/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "HeatBalanceAlgorithm.hpp"
#include "HeatBalanceAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatBalanceAlgorithm::iddObjectType());
  }

  HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatBalanceAlgorithm::iddObjectType());
  }

  HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const HeatBalanceAlgorithm_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> HeatBalanceAlgorithm_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool HeatBalanceAlgorithm_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& HeatBalanceAlgorithm_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatBalanceAlgorithm_Impl::iddObjectType() const {
    return HeatBalanceAlgorithm::iddObjectType();
  }

  std::string HeatBalanceAlgorithm_Impl::algorithm() const {
    boost::optional<std::string> value = getString(OS_HeatBalanceAlgorithmFields::Algorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatBalanceAlgorithm_Impl::isAlgorithmDefaulted() const {
    return isEmpty(OS_HeatBalanceAlgorithmFields::Algorithm);
  }

  double HeatBalanceAlgorithm_Impl::surfaceTemperatureUpperLimit() const {
    boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatBalanceAlgorithm_Impl::getSurfaceTemperatureUpperLimit(bool returnIP) const {
    OptionalDouble value = surfaceTemperatureUpperLimit();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool HeatBalanceAlgorithm_Impl::isSurfaceTemperatureUpperLimitDefaulted() const {
    return isEmpty(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit);
  }

  double HeatBalanceAlgorithm_Impl::minimumSurfaceConvectionHeatTransferCoefficientValue() const {
    boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatBalanceAlgorithm_Impl::getMinimumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP) const {
    OptionalDouble value = minimumSurfaceConvectionHeatTransferCoefficientValue();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool HeatBalanceAlgorithm_Impl::isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
    return isEmpty(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue);
  }

  double HeatBalanceAlgorithm_Impl::maximumSurfaceConvectionHeatTransferCoefficientValue() const {
    boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatBalanceAlgorithm_Impl::getMaximumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP) const {
    OptionalDouble value = maximumSurfaceConvectionHeatTransferCoefficientValue();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool HeatBalanceAlgorithm_Impl::isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
    return isEmpty(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue);
  }

  bool HeatBalanceAlgorithm_Impl::setAlgorithm(std::string algorithm) {
    bool result = setString(OS_HeatBalanceAlgorithmFields::Algorithm, algorithm);
    return result;
  }

  void HeatBalanceAlgorithm_Impl::resetAlgorithm() {
    bool result = setString(OS_HeatBalanceAlgorithmFields::Algorithm, "");
    OS_ASSERT(result);
  }

  bool HeatBalanceAlgorithm_Impl::setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit) {
    bool result = setDouble(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, surfaceTemperatureUpperLimit);
    return result;
  }

  bool HeatBalanceAlgorithm_Impl::setSurfaceTemperatureUpperLimit(const Quantity& surfaceTemperatureUpperLimit) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit,surfaceTemperatureUpperLimit);
    if (!value) {
      return false;
    }
    return setSurfaceTemperatureUpperLimit(value.get());
  }

  void HeatBalanceAlgorithm_Impl::resetSurfaceTemperatureUpperLimit() {
    bool result = setString(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, "");
    OS_ASSERT(result);
  }

  bool HeatBalanceAlgorithm_Impl::setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue) {
    bool result = setDouble(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue, minimumSurfaceConvectionHeatTransferCoefficientValue);
    return result;
  }

  bool HeatBalanceAlgorithm_Impl::setMinimumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& minimumSurfaceConvectionHeatTransferCoefficientValue) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue,minimumSurfaceConvectionHeatTransferCoefficientValue);
    if (!value) {
      return false;
    }
    return setMinimumSurfaceConvectionHeatTransferCoefficientValue(value.get());
  }

  void HeatBalanceAlgorithm_Impl::resetMinimumSurfaceConvectionHeatTransferCoefficientValue() {
    bool result = setString(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue, "");
    OS_ASSERT(result);
  }

  bool HeatBalanceAlgorithm_Impl::setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue) {
    bool result = setDouble(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue, maximumSurfaceConvectionHeatTransferCoefficientValue);
    return result;
  }

  bool HeatBalanceAlgorithm_Impl::setMaximumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& maximumSurfaceConvectionHeatTransferCoefficientValue) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue,maximumSurfaceConvectionHeatTransferCoefficientValue);
    if (!value) {
      return false;
    }
    return setMaximumSurfaceConvectionHeatTransferCoefficientValue(value.get());
  }

  void HeatBalanceAlgorithm_Impl::resetMaximumSurfaceConvectionHeatTransferCoefficientValue() {
    bool result = setString(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> HeatBalanceAlgorithm_Impl::algorithmValues() const {
    return HeatBalanceAlgorithm::algorithmValues();
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::surfaceTemperatureUpperLimit_SI() const {
    return getSurfaceTemperatureUpperLimit(false);
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::surfaceTemperatureUpperLimit_IP() const {
    return getSurfaceTemperatureUpperLimit(true);
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::minimumSurfaceConvectionHeatTransferCoefficientValue_SI() const {
    return getMinimumSurfaceConvectionHeatTransferCoefficientValue(false);
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::minimumSurfaceConvectionHeatTransferCoefficientValue_IP() const {
    return getMinimumSurfaceConvectionHeatTransferCoefficientValue(true);
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::maximumSurfaceConvectionHeatTransferCoefficientValue_SI() const {
    return getMaximumSurfaceConvectionHeatTransferCoefficientValue(false);
  }

  openstudio::Quantity HeatBalanceAlgorithm_Impl::maximumSurfaceConvectionHeatTransferCoefficientValue_IP() const {
    return getMaximumSurfaceConvectionHeatTransferCoefficientValue(true);
  }

} // detail

IddObjectType HeatBalanceAlgorithm::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatBalanceAlgorithm);
}

std::vector<std::string> HeatBalanceAlgorithm::algorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatBalanceAlgorithmFields::Algorithm);
}

std::string HeatBalanceAlgorithm::algorithm() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->algorithm();
}

bool HeatBalanceAlgorithm::isAlgorithmDefaulted() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isAlgorithmDefaulted();
}

double HeatBalanceAlgorithm::surfaceTemperatureUpperLimit() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->surfaceTemperatureUpperLimit();
}

Quantity HeatBalanceAlgorithm::getSurfaceTemperatureUpperLimit(bool returnIP) const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->getSurfaceTemperatureUpperLimit(returnIP);
}

bool HeatBalanceAlgorithm::isSurfaceTemperatureUpperLimitDefaulted() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isSurfaceTemperatureUpperLimitDefaulted();
}

double HeatBalanceAlgorithm::minimumSurfaceConvectionHeatTransferCoefficientValue() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->minimumSurfaceConvectionHeatTransferCoefficientValue();
}

Quantity HeatBalanceAlgorithm::getMinimumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP) const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->getMinimumSurfaceConvectionHeatTransferCoefficientValue(returnIP);
}

bool HeatBalanceAlgorithm::isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted();
}

double HeatBalanceAlgorithm::maximumSurfaceConvectionHeatTransferCoefficientValue() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->maximumSurfaceConvectionHeatTransferCoefficientValue();
}

Quantity HeatBalanceAlgorithm::getMaximumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP) const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->getMaximumSurfaceConvectionHeatTransferCoefficientValue(returnIP);
}

bool HeatBalanceAlgorithm::isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted();
}

bool HeatBalanceAlgorithm::setAlgorithm(std::string algorithm) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setAlgorithm(algorithm);
}

void HeatBalanceAlgorithm::resetAlgorithm() {
  getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetAlgorithm();
}

bool HeatBalanceAlgorithm::setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setSurfaceTemperatureUpperLimit(surfaceTemperatureUpperLimit);
}

bool HeatBalanceAlgorithm::setSurfaceTemperatureUpperLimit(const Quantity& surfaceTemperatureUpperLimit) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setSurfaceTemperatureUpperLimit(surfaceTemperatureUpperLimit);
}

void HeatBalanceAlgorithm::resetSurfaceTemperatureUpperLimit() {
  getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetSurfaceTemperatureUpperLimit();
}

bool HeatBalanceAlgorithm::setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMinimumSurfaceConvectionHeatTransferCoefficientValue(minimumSurfaceConvectionHeatTransferCoefficientValue);
}

bool HeatBalanceAlgorithm::setMinimumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& minimumSurfaceConvectionHeatTransferCoefficientValue) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMinimumSurfaceConvectionHeatTransferCoefficientValue(minimumSurfaceConvectionHeatTransferCoefficientValue);
}

void HeatBalanceAlgorithm::resetMinimumSurfaceConvectionHeatTransferCoefficientValue() {
  getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetMinimumSurfaceConvectionHeatTransferCoefficientValue();
}

bool HeatBalanceAlgorithm::setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMaximumSurfaceConvectionHeatTransferCoefficientValue(maximumSurfaceConvectionHeatTransferCoefficientValue);
}

bool HeatBalanceAlgorithm::setMaximumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& maximumSurfaceConvectionHeatTransferCoefficientValue) {
  return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMaximumSurfaceConvectionHeatTransferCoefficientValue(maximumSurfaceConvectionHeatTransferCoefficientValue);
}

void HeatBalanceAlgorithm::resetMaximumSurfaceConvectionHeatTransferCoefficientValue() {
  getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetMaximumSurfaceConvectionHeatTransferCoefficientValue();
}

/// @cond
/// constructor
HeatBalanceAlgorithm::HeatBalanceAlgorithm(std::shared_ptr<detail::HeatBalanceAlgorithm_Impl> impl)
  : ModelObject(std::move(impl))
{
}

/// constructor
HeatBalanceAlgorithm::HeatBalanceAlgorithm(Model& model)
  : ModelObject(HeatBalanceAlgorithm::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatBalanceAlgorithm_Impl>());

  this->setAlgorithm("ConductionTransferFunction");

  this->setSurfaceTemperatureUpperLimit(200.0);
}

/// @endcond

} // model
} // openstudio

