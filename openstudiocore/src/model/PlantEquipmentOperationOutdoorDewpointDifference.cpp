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

#include "PlantEquipmentOperationOutdoorDewpointDifference.hpp"
#include "PlantEquipmentOperationOutdoorDewpointDifference_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDewpointDifference_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(const PlantEquipmentOperationOutdoorDewpointDifference_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationOutdoorDewpointDifference_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationOutdoorDewpointDifference_Impl::iddObjectType() const {
    return PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType();
  }

  boost::optional<Node> PlantEquipmentOperationOutdoorDewpointDifference_Impl::referenceTemperatureNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName);
  }

  bool PlantEquipmentOperationOutdoorDewpointDifference_Impl::setReferenceTemperatureNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName, node.get().handle());
    }
    else {
      resetReferenceTemperatureNode();
      result = true;
    }
    return result;
  }

  void PlantEquipmentOperationOutdoorDewpointDifference_Impl::resetReferenceTemperatureNode() {
    bool result = setString(OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName, "");
    OS_ASSERT(result);
  }

  double PlantEquipmentOperationOutdoorDewpointDifference_Impl::maximumUpperLimit() const
  {
    return 100.0;
  }

  double PlantEquipmentOperationOutdoorDewpointDifference_Impl::minimumLowerLimit() const
  {
    return -50.0;
  }

} // detail

PlantEquipmentOperationOutdoorDewpointDifference::PlantEquipmentOperationOutdoorDewpointDifference(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>());
}

IddObjectType PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpointDifference);
}

boost::optional<Node> PlantEquipmentOperationOutdoorDewpointDifference::referenceTemperatureNode() const {
  return getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->referenceTemperatureNode();
}

bool PlantEquipmentOperationOutdoorDewpointDifference::setReferenceTemperatureNode(const Node& node) {
  return getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->setReferenceTemperatureNode(node);
}

void PlantEquipmentOperationOutdoorDewpointDifference::resetReferenceTemperatureNode() {
  getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->resetReferenceTemperatureNode();
}

/// @cond
PlantEquipmentOperationOutdoorDewpointDifference::PlantEquipmentOperationOutdoorDewpointDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

