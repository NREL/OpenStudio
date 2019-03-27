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

#include "AirflowNetworkDistributionLinkage.hpp"
#include "AirflowNetworkDistributionLinkage_Impl.hpp"
#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "AirflowNetworkNode.hpp"
#include "AirflowNetworkNode_Impl.hpp"
#include "AirflowNetworkComponent.hpp"
#include "AirflowNetworkComponent_Impl.hpp"
#include "AirflowNetworkFan.hpp"
#include "AirflowNetworkFan_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkDistributionLinkage_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkDistributionLinkage_Impl::AirflowNetworkDistributionLinkage_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : AirflowNetworkLinkage_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDistributionLinkage::iddObjectType());
  }

  AirflowNetworkDistributionLinkage_Impl::AirflowNetworkDistributionLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : AirflowNetworkLinkage_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkDistributionLinkage::iddObjectType());
  }

  AirflowNetworkDistributionLinkage_Impl::AirflowNetworkDistributionLinkage_Impl(const AirflowNetworkDistributionLinkage_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : AirflowNetworkLinkage_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkDistributionLinkage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkDistributionLinkage_Impl::iddObjectType() const
  {
    return AirflowNetworkDistributionLinkage::iddObjectType();
  }

  AirflowNetworkNode AirflowNetworkDistributionLinkage_Impl::node1() const
  {
    boost::optional<AirflowNetworkNode> value = optionalNode1();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Node1 attached.");
    }
    return value.get();
  }

  AirflowNetworkNode AirflowNetworkDistributionLinkage_Impl::node2() const
  {
    boost::optional<AirflowNetworkNode> value = optionalNode2();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Node2 attached.");
    }
    return value.get();
  }

  AirflowNetworkComponent AirflowNetworkDistributionLinkage_Impl::component() const
  {
    boost::optional<AirflowNetworkComponent> value = optionalComponent();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Component attached.");
    }
    return value.get();
  }

  boost::optional<ThermalZone> AirflowNetworkDistributionLinkage_Impl::thermalZone() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirflowNetworkDistributionLinkageFields::ThermalZoneName);
  }

  bool AirflowNetworkDistributionLinkage_Impl::setThermalZone(const ThermalZone& zone)
  {
    bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::ThermalZoneName, zone.handle());
    return result;
  }

  void AirflowNetworkDistributionLinkage_Impl::resetThermalZone()
  {
    bool result = setString(OS_AirflowNetworkDistributionLinkageFields::ThermalZoneName, "");
    OS_ASSERT(result);
  }

  boost::optional<AirflowNetworkNode> AirflowNetworkDistributionLinkage_Impl::optionalNode1() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkNode>(OS_AirflowNetworkDistributionLinkageFields::Node1Name);
  }

  boost::optional<AirflowNetworkNode> AirflowNetworkDistributionLinkage_Impl::optionalNode2() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkNode>(OS_AirflowNetworkDistributionLinkageFields::Node2Name);
  }

  boost::optional<AirflowNetworkComponent> AirflowNetworkDistributionLinkage_Impl::optionalComponent() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkComponent>(OS_AirflowNetworkDistributionLinkageFields::ComponentName);
  }

  bool AirflowNetworkDistributionLinkage_Impl::setNode1(const AirflowNetworkNode& airflowNetworkNode)
  {
    bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node1Name, airflowNetworkNode.handle());
    return result;
  }

  bool AirflowNetworkDistributionLinkage_Impl::setNode2(const AirflowNetworkNode& airflowNetworkNode)
  {
    bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node2Name, airflowNetworkNode.handle());
    return result;
  }

  bool AirflowNetworkDistributionLinkage_Impl::setComponent(const AirflowNetworkComponent& airflowNetworkComponent)
  {
    bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::ComponentName, airflowNetworkComponent.handle());
    return result;
  }

} // detail

AirflowNetworkDistributionLinkage::AirflowNetworkDistributionLinkage(const Model& model, const AirflowNetworkNode &node1,
  const AirflowNetworkNode &node2, const AirflowNetworkComponent &component) : AirflowNetworkLinkage(AirflowNetworkDistributionLinkage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkDistributionLinkage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirflowNetworkDistributionLinkageFields::Node1Name
  //     OS_AirflowNetworkDistributionLinkageFields::Node2Name
  //     OS_AirflowNetworkDistributionLinkageFields::ComponentName
  bool ok = true;
  ok = setNode1(node1);
  OS_ASSERT(ok);
  ok = setNode2(node2);
  OS_ASSERT(ok);
  ok = setComponent(component);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkDistributionLinkage::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkDistributionLinkage);
}

AirflowNetworkNode AirflowNetworkDistributionLinkage::node1() const
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->node1();
}

AirflowNetworkNode AirflowNetworkDistributionLinkage::node2() const
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->node2();
}

AirflowNetworkComponent AirflowNetworkDistributionLinkage::component() const
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->component();
}

boost::optional<ThermalZone> AirflowNetworkDistributionLinkage::thermalZone() const
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->thermalZone();
}

bool AirflowNetworkDistributionLinkage::setNode1(const AirflowNetworkNode& airflowNetworkNode)
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->setNode1(airflowNetworkNode);
}

bool AirflowNetworkDistributionLinkage::setNode2(const AirflowNetworkNode& airflowNetworkNode)
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->setNode2(airflowNetworkNode);
}

bool AirflowNetworkDistributionLinkage::setComponent(const AirflowNetworkComponent& airflowNetworkComponent)
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->setComponent(airflowNetworkComponent);
}

bool AirflowNetworkDistributionLinkage::setThermalZone(const ThermalZone& zone)
{
  return getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->setThermalZone(zone);
}

void AirflowNetworkDistributionLinkage::resetThermalZone()
{
  getImpl<detail::AirflowNetworkDistributionLinkage_Impl>()->resetThermalZone();
}

/// @cond
AirflowNetworkDistributionLinkage::AirflowNetworkDistributionLinkage(std::shared_ptr<detail::AirflowNetworkDistributionLinkage_Impl> impl)
  : AirflowNetworkLinkage(impl)
{}
/// @endcond

} // model
} // openstudio

