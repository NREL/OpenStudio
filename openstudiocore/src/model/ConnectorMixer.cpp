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

#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_Connector_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail{

  ConnectorMixer_Impl::ConnectorMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Mixer_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConnectorMixer::iddObjectType());
  }

  ConnectorMixer_Impl::ConnectorMixer_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConnectorMixer::iddObjectType());
  }

  ConnectorMixer_Impl::ConnectorMixer_Impl(
      const ConnectorMixer_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
  }

  ConnectorMixer_Impl::~ConnectorMixer_Impl(){}

  const std::vector<std::string> & ConnectorMixer_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConnectorMixer_Impl::iddObjectType() const {
    return ConnectorMixer::iddObjectType();
  }

  std::vector<openstudio::IdfObject> ConnectorMixer_Impl::remove()
  {
    OptionalMixer self = model().getModelObject<Mixer>(handle());
    model().disconnect(*self,outletPort());
    for( int i = 0; i < int(nextBranchIndex()) - 1; i++ )
    {
      model().disconnect(*self,inletPort(i));
    }
    return ModelObject_Impl::remove();
  }

  unsigned ConnectorMixer_Impl::outletPort() const
  {
    return OS_Connector_MixerFields::OutletBranchName;
  }

  unsigned ConnectorMixer_Impl::inletPort(unsigned branchIndex) const
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned ConnectorMixer_Impl::nextInletPort() const
  {
    return inletPort( this->nextBranchIndex() );
  }

  bool ConnectorMixer_Impl::addToNode(Node & node)
  {
    return HVACComponent_Impl::addToNode( node );
  }

  ModelObject ConnectorMixer_Impl::clone(Model model) const
  {
    return HVACComponent_Impl::clone( model );
  }

}// detail

// create a new Mixer object in the model's workspace
ConnectorMixer::ConnectorMixer(const Model& model)
  : Mixer(ConnectorMixer::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ConnectorMixer_Impl>());
}

ConnectorMixer::ConnectorMixer(
    std::shared_ptr<detail::ConnectorMixer_Impl> p)
  : Mixer(std::move(p))
{}

std::vector<openstudio::IdfObject> ConnectorMixer::remove()
{
  return getImpl<detail::ConnectorMixer_Impl>()->remove();
}

unsigned ConnectorMixer::outletPort() const
{
  return getImpl<detail::ConnectorMixer_Impl>()->outletPort();
}

unsigned ConnectorMixer::inletPort(unsigned branchIndex) const
{
  return getImpl<detail::ConnectorMixer_Impl>()->inletPort(branchIndex);
}

unsigned ConnectorMixer::nextInletPort() const
{
  return getImpl<detail::ConnectorMixer_Impl>()->nextInletPort();
}

bool ConnectorMixer::addToNode(Node & node)
{
  return getImpl<detail::ConnectorMixer_Impl>()->addToNode( node );
}

ModelObject ConnectorMixer::clone(Model model) const
{
  return getImpl<detail::ConnectorMixer_Impl>()->clone( model );
}

IddObjectType ConnectorMixer::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Connector_Mixer);
  return result;
}

} // model
} // openstudio

