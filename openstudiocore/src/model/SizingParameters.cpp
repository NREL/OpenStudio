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

#include "SizingParameters.hpp"
#include "SizingParameters_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include <utilities/idd/OS_Sizing_Parameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SizingParameters_Impl::SizingParameters_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SizingParameters::iddObjectType());
  }

  SizingParameters_Impl::SizingParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SizingParameters::iddObjectType());
  }

  SizingParameters_Impl::SizingParameters_Impl(const SizingParameters_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SizingParameters_Impl::parent() const
  {
    boost::optional<SimulationControl> result = this->model().getOptionalUniqueModelObject<SimulationControl>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SizingParameters_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SizingParameters_Impl::iddObjectType() const {
    return SizingParameters::iddObjectType();
  }

  double SizingParameters_Impl::heatingSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Sizing_ParametersFields::HeatingSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingParameters_Impl::getHeatingSizingFactor(bool returnIP) const {
    OptionalDouble value = heatingSizingFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ParametersFields::HeatingSizingFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingParameters_Impl::isHeatingSizingFactorDefaulted() const {
    return isEmpty(OS_Sizing_ParametersFields::HeatingSizingFactor);
  }

  double SizingParameters_Impl::coolingSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Sizing_ParametersFields::CoolingSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingParameters_Impl::getCoolingSizingFactor(bool returnIP) const {
    OptionalDouble value = coolingSizingFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ParametersFields::CoolingSizingFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingParameters_Impl::isCoolingSizingFactorDefaulted() const {
    return isEmpty(OS_Sizing_ParametersFields::CoolingSizingFactor);
  }

  boost::optional<int> SizingParameters_Impl::timestepsinAveragingWindow() const {
    return getInt(OS_Sizing_ParametersFields::TimestepsinAveragingWindow,true);
  }

  bool SizingParameters_Impl::setHeatingSizingFactor(double heatingSizingFactor) {
    bool result = setDouble(OS_Sizing_ParametersFields::HeatingSizingFactor, heatingSizingFactor);
    return result;
  }

  bool SizingParameters_Impl::setHeatingSizingFactor(const Quantity& heatingSizingFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ParametersFields::HeatingSizingFactor,heatingSizingFactor);
    if (!value) {
      return false;
    }
    return setHeatingSizingFactor(value.get());
  }

  void SizingParameters_Impl::resetHeatingSizingFactor() {
    bool result = setString(OS_Sizing_ParametersFields::HeatingSizingFactor, "");
    OS_ASSERT(result);
  }

  bool SizingParameters_Impl::setCoolingSizingFactor(double coolingSizingFactor) {
    bool result = setDouble(OS_Sizing_ParametersFields::CoolingSizingFactor, coolingSizingFactor);
    return result;
  }

  bool SizingParameters_Impl::setCoolingSizingFactor(const Quantity& coolingSizingFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ParametersFields::CoolingSizingFactor,coolingSizingFactor);
    if (!value) {
      return false;
    }
    return setCoolingSizingFactor(value.get());
  }

  void SizingParameters_Impl::resetCoolingSizingFactor() {
    bool result = setString(OS_Sizing_ParametersFields::CoolingSizingFactor, "");
    OS_ASSERT(result);
  }

  bool SizingParameters_Impl::setTimestepsinAveragingWindow(boost::optional<int> timestepsinAveragingWindow) {
    bool result(false);
    if (timestepsinAveragingWindow) {
      result = setInt(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, timestepsinAveragingWindow.get());
    }
    else {
      result = setString(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, "");
    }
    return result;
  }

  void SizingParameters_Impl::resetTimestepsinAveragingWindow() {
    bool result = setString(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity SizingParameters_Impl::heatingSizingFactor_SI() const {
    return getHeatingSizingFactor(false);
  }

  openstudio::Quantity SizingParameters_Impl::heatingSizingFactor_IP() const {
    return getHeatingSizingFactor(true);
  }

  openstudio::Quantity SizingParameters_Impl::coolingSizingFactor_SI() const {
    return getCoolingSizingFactor(false);
  }

  openstudio::Quantity SizingParameters_Impl::coolingSizingFactor_IP() const {
    return getCoolingSizingFactor(true);
  }

} // detail

IddObjectType SizingParameters::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Sizing_Parameters);
}

double SizingParameters::heatingSizingFactor() const {
  return getImpl<detail::SizingParameters_Impl>()->heatingSizingFactor();
}

Quantity SizingParameters::getHeatingSizingFactor(bool returnIP) const {
  return getImpl<detail::SizingParameters_Impl>()->getHeatingSizingFactor(returnIP);
}

bool SizingParameters::isHeatingSizingFactorDefaulted() const {
  return getImpl<detail::SizingParameters_Impl>()->isHeatingSizingFactorDefaulted();
}

double SizingParameters::coolingSizingFactor() const {
  return getImpl<detail::SizingParameters_Impl>()->coolingSizingFactor();
}

Quantity SizingParameters::getCoolingSizingFactor(bool returnIP) const {
  return getImpl<detail::SizingParameters_Impl>()->getCoolingSizingFactor(returnIP);
}

bool SizingParameters::isCoolingSizingFactorDefaulted() const {
  return getImpl<detail::SizingParameters_Impl>()->isCoolingSizingFactorDefaulted();
}

boost::optional<int> SizingParameters::timestepsinAveragingWindow() const {
  return getImpl<detail::SizingParameters_Impl>()->timestepsinAveragingWindow();
}

bool SizingParameters::setHeatingSizingFactor(double heatingSizingFactor) {
  return getImpl<detail::SizingParameters_Impl>()->setHeatingSizingFactor(heatingSizingFactor);
}

bool SizingParameters::setHeatingSizingFactor(const Quantity& heatingSizingFactor) {
  return getImpl<detail::SizingParameters_Impl>()->setHeatingSizingFactor(heatingSizingFactor);
}

void SizingParameters::resetHeatingSizingFactor() {
  getImpl<detail::SizingParameters_Impl>()->resetHeatingSizingFactor();
}

bool SizingParameters::setCoolingSizingFactor(double coolingSizingFactor) {
  return getImpl<detail::SizingParameters_Impl>()->setCoolingSizingFactor(coolingSizingFactor);
}

bool SizingParameters::setCoolingSizingFactor(const Quantity& coolingSizingFactor) {
  return getImpl<detail::SizingParameters_Impl>()->setCoolingSizingFactor(coolingSizingFactor);
}

void SizingParameters::resetCoolingSizingFactor() {
  getImpl<detail::SizingParameters_Impl>()->resetCoolingSizingFactor();
}

bool SizingParameters::setTimestepsinAveragingWindow(int timestepsinAveragingWindow) {
  return getImpl<detail::SizingParameters_Impl>()->setTimestepsinAveragingWindow(timestepsinAveragingWindow);
}

void SizingParameters::resetTimestepsinAveragingWindow() {
  getImpl<detail::SizingParameters_Impl>()->resetTimestepsinAveragingWindow();
}

/// @cond
SizingParameters::SizingParameters(std::shared_ptr<detail::SizingParameters_Impl> impl)
  : ModelObject(std::move(impl))
{}

SizingParameters::SizingParameters(Model& model)
  : ModelObject(SizingParameters::iddObjectType(),model)
{
  setHeatingSizingFactor(1.25);
  setCoolingSizingFactor(1.15);
}

/// @endcond


} // model
} // openstudio

