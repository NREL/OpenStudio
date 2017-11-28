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

#include "PlantEquipmentOperationOutdoorWetBulbDifference.hpp"
#include "PlantEquipmentOperationOutdoorWetBulbDifference_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorWetBulbDifference_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(const PlantEquipmentOperationOutdoorWetBulbDifference_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationOutdoorWetBulbDifference_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationOutdoorWetBulbDifference_Impl::iddObjectType() const {
    return PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType();
  }

  boost::optional<Node> PlantEquipmentOperationOutdoorWetBulbDifference_Impl::referenceTemperatureNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName);
  }

  bool PlantEquipmentOperationOutdoorWetBulbDifference_Impl::setReferenceTemperatureNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName, node.get().handle());
    }
    else {
      resetReferenceTemperatureNode();
      result = true;
    }
    return result;
  }

  void PlantEquipmentOperationOutdoorWetBulbDifference_Impl::resetReferenceTemperatureNode() {
    bool result = setString(OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName, "");
    OS_ASSERT(result);
  }

  double PlantEquipmentOperationOutdoorWetBulbDifference_Impl::maximumUpperLimit() const
  {
    return 100.0;
  }

  double PlantEquipmentOperationOutdoorWetBulbDifference_Impl::minimumLowerLimit() const
  {
    return -50.0;
  }

} // detail

PlantEquipmentOperationOutdoorWetBulbDifference::PlantEquipmentOperationOutdoorWetBulbDifference(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>());
}

IddObjectType PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulbDifference);
}

boost::optional<Node> PlantEquipmentOperationOutdoorWetBulbDifference::referenceTemperatureNode() const {
  return getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->referenceTemperatureNode();
}

bool PlantEquipmentOperationOutdoorWetBulbDifference::setReferenceTemperatureNode(const Node& node) {
  return getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->setReferenceTemperatureNode(node);
}

void PlantEquipmentOperationOutdoorWetBulbDifference::resetReferenceTemperatureNode() {
  getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->resetReferenceTemperatureNode();
}

/// @cond
PlantEquipmentOperationOutdoorWetBulbDifference::PlantEquipmentOperationOutdoorWetBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

