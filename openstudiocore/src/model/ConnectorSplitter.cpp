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

#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_Connector_Splitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail{

  ConnectorSplitter_Impl::ConnectorSplitter_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
                                                   : Splitter_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConnectorSplitter::iddObjectType());
  }

  ConnectorSplitter_Impl::ConnectorSplitter_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
        : Splitter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConnectorSplitter::iddObjectType());
  }

  ConnectorSplitter_Impl::ConnectorSplitter_Impl(
      const ConnectorSplitter_Impl& other, Model_Impl* model, bool keepHandle)
        : Splitter_Impl(other,model,keepHandle)
  {}

  ConnectorSplitter_Impl::~ConnectorSplitter_Impl()
  {
  }

  const std::vector<std::string> & ConnectorSplitter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConnectorSplitter_Impl::iddObjectType() const {
    return ConnectorSplitter::iddObjectType();
  }

  std::vector<openstudio::IdfObject> ConnectorSplitter_Impl::remove()
  {
    OptionalConnectorSplitter self = model().getModelObject<ConnectorSplitter>(handle());
    model().disconnect(*self,inletPort());
    for( int i = 0; i < int(nextBranchIndex()) - 1; i++ )
    {
      model().disconnect(*self,outletPort(i));
    }
    return ModelObject_Impl::remove();
  }

  unsigned ConnectorSplitter_Impl::inletPort()
  {
    return OS_Connector_SplitterFields::InletBranchName;
  }

  unsigned ConnectorSplitter_Impl::outletPort(unsigned branchIndex)
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned ConnectorSplitter_Impl::nextOutletPort()
  {
    return outletPort( this->nextBranchIndex() );
  }

  bool ConnectorSplitter_Impl::addToNode(Node & node)
  {
    auto t_model = model();

    if( node.model() != t_model ) {
      return false;
    }

    if( loop() ) {
      return false;
    }

    if( ! outletModelObjects().empty() ) {
      LOG(Warn, briefDescription() << " must not have existing outlet connections to use addToNode method.");
      return false;
    }

    auto t_nodeAirLoop = node.airLoopHVAC();

    if( ! t_nodeAirLoop ) {
      LOG(Warn, briefDescription() << " must be added to an AirLoopHVAC supply node use addToNode method.");
      return false;
    }

    if( ! t_nodeAirLoop->supplyComponent(node.handle()) ) {
      LOG(Warn, briefDescription() << " must be added to an AirLoopHVAC supply node use addToNode method.");
      return false;
    }

    if( ! t_nodeAirLoop->supplyComponents(iddObjectType()).empty() ) {
      LOG(Warn, briefDescription() << " already has a splitter.");
      return false;
    }

    auto supplyInletNode = t_nodeAirLoop->supplyInletNode();
    auto supplyOutletNode = t_nodeAirLoop->supplyOutletNode();

    // Hook Up Duct "A"
    auto t_inletPort = inletPort();
    auto portA = nextOutletPort();
    HVACComponent_Impl::addToNode(node, supplyInletNode, supplyOutletNode, t_inletPort, portA);

    // Hook Up Duct "B"
    Node supplyOutletNodeB(t_model);
    auto thisObject = getObject<model::HVACComponent>();
    auto portB = nextOutletPort();
    t_model.connect(thisObject,portB,supplyOutletNodeB,supplyOutletNodeB.inletPort());
    t_model.connect(supplyOutletNodeB,supplyOutletNodeB.outletPort(),t_nodeAirLoop.get(),t_nodeAirLoop->getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletPortB());

    return true;
  }

  ModelObject ConnectorSplitter_Impl::clone(Model model) const
  {
    return HVACComponent_Impl::clone( model );
  }

} // detail

ConnectorSplitter::ConnectorSplitter(const Model& model)
  : Splitter(ConnectorSplitter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ConnectorSplitter_Impl>());
}

ConnectorSplitter::ConnectorSplitter(std::shared_ptr<detail::ConnectorSplitter_Impl> p)
  : Splitter(std::move(p))
{}

std::vector<openstudio::IdfObject> ConnectorSplitter::remove()
{
  return getImpl<detail::ConnectorSplitter_Impl>()->remove();
}

unsigned ConnectorSplitter::inletPort()
{
  return getImpl<detail::ConnectorSplitter_Impl>()->inletPort();
}

unsigned ConnectorSplitter::outletPort(unsigned branchIndex)
{
  return getImpl<detail::ConnectorSplitter_Impl>()->outletPort(branchIndex);
}

unsigned ConnectorSplitter::nextOutletPort()
{
  return getImpl<detail::ConnectorSplitter_Impl>()->nextOutletPort();
}

bool ConnectorSplitter::addToNode(Node & node)
{
  return getImpl<detail::ConnectorSplitter_Impl>()->addToNode( node );
}

ModelObject ConnectorSplitter::clone(Model model) const
{
  return getImpl<detail::ConnectorSplitter_Impl>()->clone( model );
}

IddObjectType ConnectorSplitter::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Connector_Splitter);
  return result;
}

} // model
} // openstudio

