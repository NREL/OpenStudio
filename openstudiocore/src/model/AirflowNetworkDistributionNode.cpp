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

#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkDistributionNode_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
  }

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
  }

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const AirflowNetworkDistributionNode_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkDistributionNode_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkDistributionNode_Impl::iddObjectType() const {
    return AirflowNetworkDistributionNode::iddObjectType();
  }

  boost::optional<std::string> AirflowNetworkDistributionNode_Impl::componentNameorNodeName() const {
    return getString(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName,true);
  }

  std::string AirflowNetworkDistributionNode_Impl::componentObjectTypeorNodeType() const {
    boost::optional<std::string> value = getString(OS_AirflowNetworkDistributionNodeFields::ComponentObjectTypeorNodeType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDistributionNode_Impl::isComponentObjectTypeorNodeTypeDefaulted() const {
    return isEmpty(OS_AirflowNetworkDistributionNodeFields::ComponentObjectTypeorNodeType);
  }

  double AirflowNetworkDistributionNode_Impl::nodeHeight() const {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDistributionNodeFields::NodeHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDistributionNode_Impl::isNodeHeightDefaulted() const {
    return isEmpty(OS_AirflowNetworkDistributionNodeFields::NodeHeight);
  }

  void AirflowNetworkDistributionNode_Impl::setComponentNameorNodeName(const std::string& componentNameorNodeName) {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName, componentNameorNodeName);
    OS_ASSERT(result);
  }

  void AirflowNetworkDistributionNode_Impl::resetComponentNameorNodeName() {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDistributionNode_Impl::setComponentObjectTypeorNodeType(const std::string& componentObjectTypeorNodeType) {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::ComponentObjectTypeorNodeType, componentObjectTypeorNodeType);
    return result;
  }

  void AirflowNetworkDistributionNode_Impl::resetComponentObjectTypeorNodeType() {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::ComponentObjectTypeorNodeType, "");
    OS_ASSERT(result);
  }

  void AirflowNetworkDistributionNode_Impl::setNodeHeight(double nodeHeight) {
    bool result = setDouble(OS_AirflowNetworkDistributionNodeFields::NodeHeight, nodeHeight);
    OS_ASSERT(result);
  }

  void AirflowNetworkDistributionNode_Impl::resetNodeHeight() {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::NodeHeight, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(const Model& model)
  : ModelObject(AirflowNetworkDistributionNode::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkDistributionNode_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType AirflowNetworkDistributionNode::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirflowNetworkDistributionNode);
}

std::vector<std::string> AirflowNetworkDistributionNode::componentObjectTypeorNodeTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirflowNetworkDistributionNodeFields::ComponentObjectTypeorNodeType);
}

boost::optional<std::string> AirflowNetworkDistributionNode::componentNameorNodeName() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->componentNameorNodeName();
}

std::string AirflowNetworkDistributionNode::componentObjectTypeorNodeType() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->componentObjectTypeorNodeType();
}

bool AirflowNetworkDistributionNode::isComponentObjectTypeorNodeTypeDefaulted() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->isComponentObjectTypeorNodeTypeDefaulted();
}

double AirflowNetworkDistributionNode::nodeHeight() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->nodeHeight();
}

bool AirflowNetworkDistributionNode::isNodeHeightDefaulted() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->isNodeHeightDefaulted();
}

void AirflowNetworkDistributionNode::setComponentNameorNodeName(const std::string& componentNameorNodeName) {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setComponentNameorNodeName(componentNameorNodeName);
}

void AirflowNetworkDistributionNode::resetComponentNameorNodeName() {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->resetComponentNameorNodeName();
}

bool AirflowNetworkDistributionNode::setComponentObjectTypeorNodeType(const std::string& componentObjectTypeorNodeType) {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setComponentObjectTypeorNodeType(componentObjectTypeorNodeType);
}

void AirflowNetworkDistributionNode::resetComponentObjectTypeorNodeType() {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->resetComponentObjectTypeorNodeType();
}

void AirflowNetworkDistributionNode::setNodeHeight(double nodeHeight) {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setNodeHeight(nodeHeight);
}

void AirflowNetworkDistributionNode::resetNodeHeight() {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->resetNodeHeight();
}

/// @cond
AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(std::shared_ptr<detail::AirflowNetworkDistributionNode_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

