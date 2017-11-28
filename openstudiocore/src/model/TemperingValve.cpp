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

#include "TemperingValve.hpp"
#include "TemperingValve_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "PumpConstantSpeed.hpp"
#include "PumpConstantSpeed_Impl.hpp"
#include "PumpVariableSpeed.hpp"
#include "PumpVariableSpeed_Impl.hpp"
#include "HeaderedPumpsConstantSpeed.hpp"
#include "HeaderedPumpsConstantSpeed_Impl.hpp"
#include "HeaderedPumpsVariableSpeed.hpp"
#include "HeaderedPumpsVariableSpeed_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_TemperingValve_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  TemperingValve_Impl::TemperingValve_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == TemperingValve::iddObjectType());
  }

  TemperingValve_Impl::TemperingValve_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == TemperingValve::iddObjectType());
  }

  TemperingValve_Impl::TemperingValve_Impl(const TemperingValve_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& TemperingValve_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType TemperingValve_Impl::iddObjectType() const {
    return TemperingValve::iddObjectType();
  }

  boost::optional<Node> TemperingValve_Impl::stream2SourceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_TemperingValveFields::Stream2SourceNode);
  }

  boost::optional<Node> TemperingValve_Impl::temperatureSetpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_TemperingValveFields::TemperatureSetpointNode);
  }

  boost::optional<Node> TemperingValve_Impl::pumpOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_TemperingValveFields::PumpOutletNode);
  }

  bool TemperingValve_Impl::setStream2SourceNode(const boost::optional<Node> & stream2SourceNode) {
    bool result(false);
    if (stream2SourceNode) {
      result = setPointer(OS_TemperingValveFields::Stream2SourceNode, stream2SourceNode.get().handle());
    }
    else {
      resetStream2SourceNode();
      result = true;
    }
    return result;
  }

  void TemperingValve_Impl::resetStream2SourceNode() {
    bool result = setString(OS_TemperingValveFields::Stream2SourceNode, "");
    OS_ASSERT(result);
  }

  bool TemperingValve_Impl::setTemperatureSetpointNode(const boost::optional<Node> & temperatureSetpointNode) {
    bool result(false);
    if (temperatureSetpointNode) {
      result = setPointer(OS_TemperingValveFields::TemperatureSetpointNode, temperatureSetpointNode.get().handle());
    }
    else {
      resetTemperatureSetpointNode();
      result = true;
    }
    return result;
  }

  void TemperingValve_Impl::resetTemperatureSetpointNode() {
    bool result = setString(OS_TemperingValveFields::TemperatureSetpointNode, "");
    OS_ASSERT(result);
  }

  bool TemperingValve_Impl::setPumpOutletNode(const boost::optional<Node> & pumpOutletNode) {
    bool result(false);
    if (pumpOutletNode) {
      result = setPointer(OS_TemperingValveFields::PumpOutletNode, pumpOutletNode.get().handle());
    }
    else {
      resetPumpOutletNode();
      result = true;
    }
    return result;
  }

  void TemperingValve_Impl::resetPumpOutletNode() {
    bool result = setString(OS_TemperingValveFields::PumpOutletNode, "");
    OS_ASSERT(result);
  }

  unsigned TemperingValve_Impl::inletPort() {
    return OS_TemperingValveFields::InletNodeName;
  }

  unsigned TemperingValve_Impl::outletPort() {
    return OS_TemperingValveFields::OutletNodeName;
  }

  void TemperingValve_Impl::setControlNodes()
  {
    auto plant = plantLoop();

    if( ! plant ) return;

    if( ! pumpOutletNode() ) {
      std::vector<ModelObject> allpumps;
      auto pumps = plant->supplyComponents(PumpVariableSpeed::iddObjectType());
      allpumps.insert(allpumps.end(),pumps.begin(),pumps.end());
      pumps = plant->supplyComponents(PumpConstantSpeed::iddObjectType());
      allpumps.insert(allpumps.end(),pumps.begin(),pumps.end());
      pumps = plant->supplyComponents(HeaderedPumpsConstantSpeed::iddObjectType());
      allpumps.insert(allpumps.end(),pumps.begin(),pumps.end());
      pumps = plant->supplyComponents(HeaderedPumpsVariableSpeed::iddObjectType());
      allpumps.insert(allpumps.end(),pumps.begin(),pumps.end());

      if( ! allpumps.empty() ) {
        if( auto mo = allpumps.back().cast<StraightComponent>().outletModelObject() ) {
          if( auto node = mo->optionalCast<Node>() ) {
            setPumpOutletNode(node);
          }
        }
      }
    }

    if( ! temperatureSetpointNode() ) {
      auto node = plant->supplyOutletNode();
      setTemperatureSetpointNode(node);
    }

    auto mixer = plant->supplyMixer();

    if( ! stream2SourceNode() ) {
      TemperingValve thisObject = getObject<TemperingValve>();
      auto inletObjects = mixer.inletModelObjects();
      for( auto & inletObject : inletObjects ) {
        if( auto node = inletObject.optionalCast<Node>() ) {
          if( plant->supplyComponents(thisObject,node.get()).empty() ) {
            setStream2SourceNode(node.get());
          }
        }
      }
    }
  }

  bool TemperingValve_Impl::addToNode(Node & node)
  {
    auto plant = node.plantLoop();

    if( ! plant ) return false;

    if( ! plant->supplyComponent(node.handle()) ) return false;

    return StraightComponent_Impl::addToNode(node);
  }

} // detail

TemperingValve::TemperingValve(const Model& model)
  : StraightComponent(TemperingValve::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::TemperingValve_Impl>());
  setString(OS_TemperingValveFields::Stream2SourceNode," ");
  setString(OS_TemperingValveFields::TemperatureSetpointNode," ");
  setString(OS_TemperingValveFields::PumpOutletNode," ");
}

IddObjectType TemperingValve::iddObjectType() {
  return IddObjectType(IddObjectType::OS_TemperingValve);
}

boost::optional<Node> TemperingValve::stream2SourceNode() const {
  return getImpl<detail::TemperingValve_Impl>()->stream2SourceNode();
}

boost::optional<Node> TemperingValve::temperatureSetpointNode() const {
  return getImpl<detail::TemperingValve_Impl>()->temperatureSetpointNode();
}

boost::optional<Node> TemperingValve::pumpOutletNode() const {
  return getImpl<detail::TemperingValve_Impl>()->pumpOutletNode();
}

bool TemperingValve::setStream2SourceNode(const Node & stream2SourceNode) {
  return getImpl<detail::TemperingValve_Impl>()->setStream2SourceNode(stream2SourceNode);
}

void TemperingValve::resetStream2SourceNode() {
  getImpl<detail::TemperingValve_Impl>()->resetStream2SourceNode();
}

bool TemperingValve::setTemperatureSetpointNode(const Node & temperatureSetpointNode) {
  return getImpl<detail::TemperingValve_Impl>()->setTemperatureSetpointNode(temperatureSetpointNode);
}

void TemperingValve::resetTemperatureSetpointNode() {
  getImpl<detail::TemperingValve_Impl>()->resetTemperatureSetpointNode();
}

bool TemperingValve::setPumpOutletNode(const Node & pumpOutletNode) {
  return getImpl<detail::TemperingValve_Impl>()->setPumpOutletNode(pumpOutletNode);
}

void TemperingValve::resetPumpOutletNode() {
  getImpl<detail::TemperingValve_Impl>()->resetPumpOutletNode();
}

/// @cond
TemperingValve::TemperingValve(std::shared_ptr<detail::TemperingValve_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

