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

#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"

#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkEquivalentDuct_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkEquivalentDuct_Impl::AirflowNetworkEquivalentDuct_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkEquivalentDuct::iddObjectType());
  }

  AirflowNetworkEquivalentDuct_Impl::AirflowNetworkEquivalentDuct_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkEquivalentDuct::iddObjectType());
  }

  AirflowNetworkEquivalentDuct_Impl::AirflowNetworkEquivalentDuct_Impl(const AirflowNetworkEquivalentDuct_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkEquivalentDuct_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkEquivalentDuct_Impl::iddObjectType() const
  {
    return AirflowNetworkEquivalentDuct::iddObjectType();
  }

  boost::optional<StraightComponent> AirflowNetworkEquivalentDuct_Impl::component() const
  {
    return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_AirflowNetworkEquivalentDuctFields::ComponentName);
  }

  double AirflowNetworkEquivalentDuct_Impl::airPathLength() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkEquivalentDuctFields::AirPathLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkEquivalentDuct_Impl::airPathHydraulicDiameter() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkEquivalentDuctFields::AirPathHydraulicDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkEquivalentDuct_Impl::setAirPathLength(double airPathLength)
  {
    bool result = setDouble(OS_AirflowNetworkEquivalentDuctFields::AirPathLength, airPathLength);
    return result;
  }

  bool AirflowNetworkEquivalentDuct_Impl::setAirPathHydraulicDiameter(double airPathHydraulicDiameter)
  {
    bool result = setDouble(OS_AirflowNetworkEquivalentDuctFields::AirPathHydraulicDiameter, airPathHydraulicDiameter);
    return result;
  }

  bool AirflowNetworkEquivalentDuct_Impl::setComponent(const StraightComponent &component)
  {
    return setPointer(OS_AirflowNetworkEquivalentDuctFields::ComponentName, component.handle());
  }

  void AirflowNetworkEquivalentDuct_Impl::resetComponent()
  {
    bool result = setString(OS_AirflowNetworkEquivalentDuctFields::ComponentName, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkEquivalentDuct::AirflowNetworkEquivalentDuct(const Model& model, const Handle &handle)
  : ModelObject(AirflowNetworkEquivalentDuct::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkEquivalentDuct_Impl>());
  bool ok = getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->setPointer(OS_AirflowNetworkEquivalentDuctFields::ComponentName, handle);
  OS_ASSERT(ok);
  ok = setAirPathLength(1.0);
  OS_ASSERT(ok);
  ok = setAirPathHydraulicDiameter(1.0);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkEquivalentDuct::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkEquivalentDuct);
}

boost::optional<StraightComponent> AirflowNetworkEquivalentDuct::component() const
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->component();
}

double AirflowNetworkEquivalentDuct::airPathLength() const
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->airPathLength();
}

double AirflowNetworkEquivalentDuct::airPathHydraulicDiameter() const
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->airPathHydraulicDiameter();
}

bool AirflowNetworkEquivalentDuct::setAirPathLength(double airPathLength)
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->setAirPathLength(airPathLength);
}

bool AirflowNetworkEquivalentDuct::setAirPathHydraulicDiameter(double airPathHydraulicDiameter)
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->setAirPathHydraulicDiameter(airPathHydraulicDiameter);
}

bool AirflowNetworkEquivalentDuct::setComponent(const StraightComponent &component)
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->setComponent(component);
}

void AirflowNetworkEquivalentDuct::resetComponent()
{
  return getImpl<detail::AirflowNetworkEquivalentDuct_Impl>()->resetComponent();
}

/// @cond
AirflowNetworkEquivalentDuct::AirflowNetworkEquivalentDuct(std::shared_ptr<detail::AirflowNetworkEquivalentDuct_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

