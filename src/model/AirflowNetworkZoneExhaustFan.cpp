/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirflowNetworkZoneExhaustFan.hpp"
#include "AirflowNetworkZoneExhaustFan_Impl.hpp"

#include "FanZoneExhaust.hpp"
#include "FanZoneExhaust_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkZoneExhaustFan_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
  }

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
  }

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const AirflowNetworkZoneExhaustFan_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkZoneExhaustFan_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkZoneExhaustFan_Impl::iddObjectType() const {
    return AirflowNetworkZoneExhaustFan::iddObjectType();
  }

  boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan_Impl::fanZoneExhaust() const
  {
    boost::optional<FanZoneExhaust> value = getObject<ModelObject>().getModelObjectTarget<FanZoneExhaust>(OS_AirflowNetworkZoneExhaustFanFields::Name);
    return value;
  }

  boost::optional<AirflowNetworkCrack> AirflowNetworkZoneExhaustFan_Impl::crack() const
  {
    boost::optional<AirflowNetworkCrack> value = getObject<ModelObject>().getModelObjectTarget<AirflowNetworkCrack>(OS_AirflowNetworkZoneExhaustFanFields::CrackName);
    return value;
  }

  bool AirflowNetworkZoneExhaustFan_Impl::setCrack(const AirflowNetworkCrack& crack)
  {
    return setPointer(OS_AirflowNetworkZoneExhaustFanFields::CrackName, crack.handle());
  }

  void AirflowNetworkZoneExhaustFan_Impl::resetCrack()
  {
    bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::CrackName, "");
    OS_ASSERT(result);
  }

  void AirflowNetworkZoneExhaustFan_Impl::resetFanZoneExhaust()
  {
    bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::Name, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(const Model& model, const AirflowNetworkCrack& crack, const Handle &handle)
  : ModelObject(AirflowNetworkZoneExhaustFan::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>());
  bool ok = setCrack(crack);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setPointer(OS_AirflowNetworkZoneExhaustFanFields::Name, handle);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkZoneExhaustFan::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirflowNetworkZoneExhaustFan);
}

boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan::fanZoneExhaust() const {
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->fanZoneExhaust();
}

boost::optional<AirflowNetworkCrack> AirflowNetworkZoneExhaustFan::crack() const {
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->crack();
}

bool AirflowNetworkZoneExhaustFan::setCrack(const AirflowNetworkCrack& crack) {
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setCrack(crack);
}

void AirflowNetworkZoneExhaustFan::resetCrack()
{
  getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetCrack();
}

void AirflowNetworkZoneExhaustFan::resetFanZoneExhaust()
{
  getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetFanZoneExhaust();
}

/// @cond
AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(std::shared_ptr<detail::AirflowNetworkZoneExhaustFan_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

