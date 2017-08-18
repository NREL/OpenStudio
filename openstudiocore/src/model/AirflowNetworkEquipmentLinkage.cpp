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

#include "AirflowNetworkEquipmentLinkage.hpp"
#include "AirflowNetworkEquipmentLinkage_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "FanZoneExhaust.hpp"
#include "FanZoneExhaust_Impl.hpp"
#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkEquipmentLinkage_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkEquipmentLinkage_Impl::AirflowNetworkEquipmentLinkage_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkEquipmentLinkage::iddObjectType());
  }

  AirflowNetworkEquipmentLinkage_Impl::AirflowNetworkEquipmentLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkEquipmentLinkage::iddObjectType());
  }

  AirflowNetworkEquipmentLinkage_Impl::AirflowNetworkEquipmentLinkage_Impl(const AirflowNetworkEquipmentLinkage_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkEquipmentLinkage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkEquipmentLinkage_Impl::iddObjectType() const
  {
    return AirflowNetworkEquipmentLinkage::iddObjectType();
  }

  boost::optional<FanZoneExhaust> AirflowNetworkEquipmentLinkage_Impl::fanZoneExhaust() const
  {
    boost::optional<FanZoneExhaust> value = getObject<ModelObject>().getModelObjectTarget<FanZoneExhaust>(OS_AirflowNetworkEquipmentLinkageFields::Name);
    return value;
  }

  boost::optional<ControllerOutdoorAir> AirflowNetworkEquipmentLinkage_Impl::controllerOutdoorAir() const
  {
    boost::optional<ControllerOutdoorAir> value = getObject<ModelObject>().getModelObjectTarget<ControllerOutdoorAir>(OS_AirflowNetworkEquipmentLinkageFields::Name);
    return value;
  }

  boost::optional<AirflowNetworkCrack> AirflowNetworkEquipmentLinkage_Impl::crack() const
  {
    boost::optional<AirflowNetworkCrack> value = getObject<ModelObject>().getModelObjectTarget<AirflowNetworkCrack>(OS_AirflowNetworkEquipmentLinkageFields::CrackName);
    return value;
  }

  bool AirflowNetworkEquipmentLinkage_Impl::setCrack(const AirflowNetworkCrack& crack)
  {
    return setPointer(OS_AirflowNetworkEquipmentLinkageFields::CrackName, crack.handle());
  }

  void AirflowNetworkEquipmentLinkage_Impl::resetCrack()
  {
    bool result = setString(OS_AirflowNetworkEquipmentLinkageFields::CrackName, "");
    OS_ASSERT(result);
  }

  void AirflowNetworkEquipmentLinkage_Impl::resetEquipment()
  {
    bool result = setString(OS_AirflowNetworkEquipmentLinkageFields::Name, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkEquipmentLinkage::AirflowNetworkEquipmentLinkage(const Model& model, const AirflowNetworkCrack& crack, const Handle &handle)
  : ModelObject(AirflowNetworkEquipmentLinkage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>());
  bool ok = setCrack(crack);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->setPointer(OS_AirflowNetworkEquipmentLinkageFields::Name, handle);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkEquipmentLinkage::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkEquipmentLinkage);
}

boost::optional<FanZoneExhaust> AirflowNetworkEquipmentLinkage::fanZoneExhaust() const
{
  return getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->fanZoneExhaust();
}

boost::optional<ControllerOutdoorAir> AirflowNetworkEquipmentLinkage::controllerOutdoorAir() const
{
  return getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->controllerOutdoorAir();
}

boost::optional<AirflowNetworkCrack> AirflowNetworkEquipmentLinkage::crack() const
{
  return getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->crack();
}

bool AirflowNetworkEquipmentLinkage::setCrack(const AirflowNetworkCrack& crack)
{
  return getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->setCrack(crack);
}

void AirflowNetworkEquipmentLinkage::resetCrack()
{
  getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->resetCrack();
}

void AirflowNetworkEquipmentLinkage::resetEquipment()
{
  getImpl<detail::AirflowNetworkEquipmentLinkage_Impl>()->resetEquipment();
}

/// @cond
AirflowNetworkEquipmentLinkage::AirflowNetworkEquipmentLinkage(std::shared_ptr<detail::AirflowNetworkEquipmentLinkage_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

