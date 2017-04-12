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

#include "AirflowNetworkLinkage.hpp"
#include "AirflowNetworkLinkage_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "AirflowNetworkNode.hpp"
#include "AirflowNetworkNode_Impl.hpp"
#include "AirflowNetworkComponent.hpp"
#include "AirflowNetworkComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkLinkage_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkLinkage::iddObjectType());
  }

  AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkLinkage::iddObjectType());
  }

  AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const AirflowNetworkLinkage_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkLinkage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkLinkage_Impl::iddObjectType() const
  {
    return AirflowNetworkLinkage::iddObjectType();
  }

  AirflowNetworkNode AirflowNetworkLinkage_Impl::node1() const
  {
    boost::optional<AirflowNetworkNode> value = optionalNode1();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Node1 attached.");
    }
    return value.get();
  }

  AirflowNetworkNode AirflowNetworkLinkage_Impl::node2() const
  {
    boost::optional<AirflowNetworkNode> value = optionalNode2();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Node2 attached.");
    }
    return value.get();
  }

  AirflowNetworkComponent AirflowNetworkLinkage_Impl::component() const
  {
    boost::optional<AirflowNetworkComponent> value = optionalComponent();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Component attached.");
    }
    return value.get();
  }

  boost::optional<ThermalZone> AirflowNetworkLinkage_Impl::thermalZone() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirflowNetworkLinkageFields::ThermalZoneName);
  }

  bool AirflowNetworkLinkage_Impl::setNode1(const AirflowNetworkNode& airflowNetworkNode)
  {
    bool result = setPointer(OS_AirflowNetworkLinkageFields::Node1Name, airflowNetworkNode.handle());
    return result;
  }

  bool AirflowNetworkLinkage_Impl::setNode2(const AirflowNetworkNode& airflowNetworkNode)
  {
    bool result = setPointer(OS_AirflowNetworkLinkageFields::Node2Name, airflowNetworkNode.handle());
    return result;
  }

  bool AirflowNetworkLinkage_Impl::setComponent(const AirflowNetworkComponent& airflowNetworkComponent)
  {
    bool result = setPointer(OS_AirflowNetworkLinkageFields::ComponentName, airflowNetworkComponent.handle());
    return result;
  }

  bool AirflowNetworkLinkage_Impl::setThermalZone(const ThermalZone& zone)
  {
    bool result = setPointer(OS_AirflowNetworkLinkageFields::ThermalZoneName, zone.handle());
    return result;
  }

  void AirflowNetworkLinkage_Impl::resetThermalZone()
  {
    bool result = setString(OS_AirflowNetworkLinkageFields::ThermalZoneName, "");
    OS_ASSERT(result);
  }

  boost::optional<AirflowNetworkNode> AirflowNetworkLinkage_Impl::optionalNode1() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkNode>(OS_AirflowNetworkLinkageFields::Node1Name);
  }

  boost::optional<AirflowNetworkNode> AirflowNetworkLinkage_Impl::optionalNode2() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkNode>(OS_AirflowNetworkLinkageFields::Node2Name);
  }

  boost::optional<AirflowNetworkComponent> AirflowNetworkLinkage_Impl::optionalComponent() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkComponent>(OS_AirflowNetworkLinkageFields::ComponentName);
  }

} // detail

AirflowNetworkLinkage::AirflowNetworkLinkage(const Model& model)
  : ModelObject(AirflowNetworkLinkage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkLinkage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirflowNetworkLinkageFields::Node1Name
  //     OS_AirflowNetworkLinkageFields::Node2Name
  //     OS_AirflowNetworkLinkageFields::ComponentName
  bool ok = true;
  // ok = setNode1();
  OS_ASSERT(ok);
  // ok = setNode2();
  OS_ASSERT(ok);
  // ok = setComponent();
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkLinkage::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkLinkage);
}

AirflowNetworkNode AirflowNetworkLinkage::node1() const
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->node1();
}

AirflowNetworkNode AirflowNetworkLinkage::node2() const
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->node2();
}

AirflowNetworkComponent AirflowNetworkLinkage::component() const
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->component();
}

boost::optional<ThermalZone> AirflowNetworkLinkage::thermalZone() const
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->thermalZone();
}

bool AirflowNetworkLinkage::setNode1(const AirflowNetworkNode& airflowNetworkNode)
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->setNode1(airflowNetworkNode);
}

bool AirflowNetworkLinkage::setNode2(const AirflowNetworkNode& airflowNetworkNode)
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->setNode2(airflowNetworkNode);
}

bool AirflowNetworkLinkage::setComponent(const AirflowNetworkComponent& airflowNetworkComponent)
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->setComponent(airflowNetworkComponent);
}

bool AirflowNetworkLinkage::setThermalZone(const ThermalZone& zone)
{
  return getImpl<detail::AirflowNetworkLinkage_Impl>()->setThermalZone(zone);
}

void AirflowNetworkLinkage::resetThermalZone()
{
  getImpl<detail::AirflowNetworkLinkage_Impl>()->resetThermalZone();
}

/// @cond
AirflowNetworkLinkage::AirflowNetworkLinkage(std::shared_ptr<detail::AirflowNetworkLinkage_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

