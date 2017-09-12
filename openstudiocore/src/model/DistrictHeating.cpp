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

#include "DistrictHeating.hpp"
#include "DistrictHeating_Impl.hpp"

#include <utilities/idd/OS_DistrictHeating_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/core/Assert.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DistrictHeating_Impl::DistrictHeating_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DistrictHeating::iddObjectType());
  }

  DistrictHeating_Impl::DistrictHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DistrictHeating::iddObjectType());
  }

  DistrictHeating_Impl::DistrictHeating_Impl(const DistrictHeating_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DistrictHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DistrictHeating_Impl::iddObjectType() const {
    return DistrictHeating::iddObjectType();
  }

  boost::optional<double> DistrictHeating_Impl::nominalCapacity() const {
    return getDouble(OS_DistrictHeatingFields::NominalCapacity,true);
  }

  bool DistrictHeating_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_DistrictHeatingFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool DistrictHeating_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_DistrictHeatingFields::NominalCapacity, nominalCapacity.get());
    }
    return result;
  }

  void DistrictHeating_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_DistrictHeatingFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  unsigned DistrictHeating_Impl::inletPort()
  {
    return OS_DistrictHeatingFields::HotWaterInletNodeName;
  }

  unsigned DistrictHeating_Impl::outletPort()
  {
    return OS_DistrictHeatingFields::HotWaterOutletNodeName;
  }

  bool DistrictHeating_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

DistrictHeating::DistrictHeating(const Model& model)
  : StraightComponent(DistrictHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DistrictHeating_Impl>());
  autosizeNominalCapacity();
}

IddObjectType DistrictHeating::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DistrictHeating);
  return result;
}

boost::optional<double> DistrictHeating::nominalCapacity() const {
  return getImpl<detail::DistrictHeating_Impl>()->nominalCapacity();
}

bool DistrictHeating::isNominalCapacityAutosized() const {
  return getImpl<detail::DistrictHeating_Impl>()->isNominalCapacityAutosized();
}

bool DistrictHeating::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::DistrictHeating_Impl>()->setNominalCapacity(nominalCapacity);
}

void DistrictHeating::autosizeNominalCapacity() {
  getImpl<detail::DistrictHeating_Impl>()->autosizeNominalCapacity();
}

/// @cond

DistrictHeating::DistrictHeating(std::shared_ptr<detail::DistrictHeating_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond


} // model

} // openstudio



