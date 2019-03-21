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

#include "AirflowNetworkOutdoorAirflow.hpp"
#include "AirflowNetworkOutdoorAirflow_Impl.hpp"

#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkOutdoorAirflow_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkOutdoorAirflow::iddObjectType());
  }

  AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkOutdoorAirflow::iddObjectType());
  }

  AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const AirflowNetworkOutdoorAirflow_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkOutdoorAirflow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkOutdoorAirflow_Impl::iddObjectType() const {
    return AirflowNetworkOutdoorAirflow::iddObjectType();
  }

  boost::optional<ControllerOutdoorAir> AirflowNetworkOutdoorAirflow_Impl::controllerOutdoorAir() const
  {
    boost::optional<ControllerOutdoorAir> value = getObject<ModelObject>().getModelObjectTarget<ControllerOutdoorAir>(OS_AirflowNetworkOutdoorAirflowFields::Name);
    return value;
  }

  boost::optional<AirflowNetworkCrack> AirflowNetworkOutdoorAirflow_Impl::crack() const
  {
    boost::optional<AirflowNetworkCrack> value = getObject<ModelObject>().getModelObjectTarget<AirflowNetworkCrack>(OS_AirflowNetworkOutdoorAirflowFields::CrackName);
    return value;
  }

  bool AirflowNetworkOutdoorAirflow_Impl::setCrack(const AirflowNetworkCrack& crack)
  {
    return setPointer(OS_AirflowNetworkOutdoorAirflowFields::CrackName, crack.handle());
  }

  void AirflowNetworkOutdoorAirflow_Impl::resetCrack()
  {
    bool result = setString(OS_AirflowNetworkOutdoorAirflowFields::CrackName, "");
    OS_ASSERT(result);
  }

  void AirflowNetworkOutdoorAirflow_Impl::resetControllerOutdoorAir()
  {
    bool result = setString(OS_AirflowNetworkOutdoorAirflowFields::Name, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkOutdoorAirflow::AirflowNetworkOutdoorAirflow(const Model& model, const AirflowNetworkCrack& crack, const Handle &handle)
  : ModelObject(AirflowNetworkOutdoorAirflow::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>());
  bool ok = setCrack(crack);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->setPointer(OS_AirflowNetworkOutdoorAirflowFields::Name, handle);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkOutdoorAirflow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirflowNetworkOutdoorAirflow);
}

boost::optional<ControllerOutdoorAir> AirflowNetworkOutdoorAirflow::controllerOutdoorAir() const {
  return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->controllerOutdoorAir();
}

boost::optional<AirflowNetworkCrack> AirflowNetworkOutdoorAirflow::crack() const {
  return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->crack();
}

bool AirflowNetworkOutdoorAirflow::setCrack(const AirflowNetworkCrack& crack) {
  return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->setCrack(crack);
}

void AirflowNetworkOutdoorAirflow::resetCrack()
{
  getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->resetCrack();
}

void AirflowNetworkOutdoorAirflow::resetControllerOutdoorAir()
{
  getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->resetControllerOutdoorAir();
}

/// @cond
AirflowNetworkOutdoorAirflow::AirflowNetworkOutdoorAirflow(std::shared_ptr<detail::AirflowNetworkOutdoorAirflow_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

