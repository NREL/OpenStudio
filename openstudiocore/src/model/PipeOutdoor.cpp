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

#include "PipeOutdoor.hpp"
#include "PipeOutdoor_Impl.hpp"

#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Pipe_Outdoor_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Unit.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PipeOutdoor_Impl::PipeOutdoor_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const PipeOutdoor_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PipeOutdoor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PipeOutdoor_Impl::iddObjectType() const {
    return PipeOutdoor::iddObjectType();
  }

  unsigned PipeOutdoor_Impl::inletPort()
  {
    return OS_Pipe_OutdoorFields::FluidInletNode;
  }

  unsigned PipeOutdoor_Impl::outletPort()
  {
    return OS_Pipe_OutdoorFields::FluidOutletNode;
  }

  boost::optional<Construction> PipeOutdoor_Impl::construction() const {
    return getObject<ModelObject>().getModelObjectTarget<Construction>(OS_Pipe_OutdoorFields::Construction);
  }

  boost::optional<Node> PipeOutdoor_Impl::ambientTemperatureOutdoorAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode);
  }

  double PipeOutdoor_Impl::pipeInsideDiameter() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PipeOutdoor_Impl::pipeLength() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PipeOutdoor_Impl::setConstruction(const boost::optional<Construction>& construction) {
    bool result(false);
    if (construction) {
      result = setPointer(OS_Pipe_OutdoorFields::Construction, construction.get().handle());
    }
    else {
      resetConstruction();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetConstruction() {
    bool result = setString(OS_Pipe_OutdoorFields::Construction, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setAmbientTemperatureOutdoorAirNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, node.get().handle());
    }
    else {
      resetAmbientTemperatureOutdoorAirNode();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetAmbientTemperatureOutdoorAirNode() {
    bool result = setString(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setPipeInsideDiameter(double pipeInsideDiameter) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter, pipeInsideDiameter);
    return result;
  }

  bool PipeOutdoor_Impl::setPipeLength(double pipeLength) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeLength, pipeLength);
    return result;
  }

  bool PipeOutdoor_Impl::addToNode(Node & node)
  {
    if(node.plantLoop()) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

PipeOutdoor::PipeOutdoor(const Model& model)
  : StraightComponent(PipeOutdoor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PipeOutdoor_Impl>());

  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setPipeInsideDiameter(0.05);
  OS_ASSERT(ok);
   ok = setPipeLength(100.0);
  OS_ASSERT(ok);
}

IddObjectType PipeOutdoor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Pipe_Outdoor);
}

boost::optional<Construction> PipeOutdoor::construction() const {
  return getImpl<detail::PipeOutdoor_Impl>()->construction();
}

boost::optional<Node> PipeOutdoor::ambientTemperatureOutdoorAirNode() const {
  return getImpl<detail::PipeOutdoor_Impl>()->ambientTemperatureOutdoorAirNode();
}

double PipeOutdoor::pipeInsideDiameter() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeInsideDiameter();
}

double PipeOutdoor::pipeLength() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeLength();
}

bool PipeOutdoor::setConstruction(const Construction& construction) {
  return getImpl<detail::PipeOutdoor_Impl>()->setConstruction(construction);
}

void PipeOutdoor::resetConstruction() {
  getImpl<detail::PipeOutdoor_Impl>()->resetConstruction();
}

bool PipeOutdoor::setAmbientTemperatureOutdoorAirNode(const Node& node) {
  return getImpl<detail::PipeOutdoor_Impl>()->setAmbientTemperatureOutdoorAirNode(node);
}

void PipeOutdoor::resetAmbientTemperatureOutdoorAirNode() {
  getImpl<detail::PipeOutdoor_Impl>()->resetAmbientTemperatureOutdoorAirNode();
}

bool PipeOutdoor::setPipeInsideDiameter(double pipeInsideDiameter) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeInsideDiameter(pipeInsideDiameter);
}

bool PipeOutdoor::setPipeLength(double pipeLength) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeLength(pipeLength);
}

/// @cond
PipeOutdoor::PipeOutdoor(std::shared_ptr<detail::PipeOutdoor_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

