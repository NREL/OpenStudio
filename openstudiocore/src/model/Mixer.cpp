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

#include "Model.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

Mixer_Impl::Mixer_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

Mixer_Impl::Mixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{
}

Mixer_Impl::Mixer_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,
  Model_Impl* model,
  bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

Mixer_Impl::Mixer_Impl(const Mixer_Impl& other,
  Model_Impl* model,
  bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

boost::optional<ModelObject> Mixer_Impl::outletModelObject() const
{
  return connectedObject( outletPort() );
}

boost::optional<ModelObject> Mixer_Impl::inletModelObject(unsigned branchIndex) const
{
  return connectedObject( inletPort( branchIndex ) );
}

boost::optional<ModelObject> Mixer_Impl::lastInletModelObject() const
{
  std::vector<ModelObject> objects = inletModelObjects();
  if( objects.size() > 0 )
  {
    return OptionalModelObject(objects.back());
  }
  else
  {
    return OptionalModelObject();
  }
}

unsigned Mixer_Impl::newInletPortAfterBranch(unsigned branchIndex)
{
  //std::vector<ModelObject> modelObjects = inletModelObjects();
  unsigned stop = nextBranchIndex();
  for(unsigned i = branchIndex; i < stop; i++ )
  {
    ModelObject mo = inletModelObject(i).get();
    unsigned port = connectedObjectPort( inletPort(i) ).get();
    model().connect(getObject<ModelObject>(),inletPort(i+2),mo,port);
  }

  Model _model = model();
  Node node( _model );

  _model.connect( node,node.outletPort(),getObject<ModelObject>(),inletPort(branchIndex+1) );

  return inletPort(branchIndex++);
}

unsigned Mixer_Impl::branchIndexForInletModelObject( ModelObject modelObject ) const
{
  unsigned stop = nextBranchIndex();

  for(unsigned i = 0; i < stop; i++)
  {
    if( inletModelObject(i) == modelObject )
    {
      return i;
    }
  }
  return 0;
}

unsigned Mixer_Impl::nextBranchIndex() const
{
  unsigned i = 0;
  OptionalModelObject modelObject;
  modelObject = connectedObject( this->inletPort(i) );
  while( modelObject )
  {
    i++;
    modelObject = connectedObject( this->inletPort(i) );
  }
  return i;
}

void Mixer_Impl::removePortForBranch(unsigned branchIndex)
{
  int _nextBranchIndex = nextBranchIndex();
  model().disconnect(getObject<ModelObject>(),inletPort(branchIndex));
  for(int i = branchIndex + 1; i < _nextBranchIndex; i++ )
  {
    ModelObject mo = inletModelObject(i).get();
    unsigned port = connectedObjectPort( inletPort(i) ).get();
    model().disconnect(getObject<ModelObject>(),inletPort(i));
    model().connect(mo,port,getObject<ModelObject>(),inletPort(i-1));
  }
}

std::vector<ModelObject> Mixer_Impl::inletModelObjects() const
{
  std::vector<ModelObject> result;
  unsigned stop = nextBranchIndex();
  for( unsigned i = 0; i < stop; i++ )
  {
    result.push_back( this->inletModelObject( i ).get() );
  }
  return result;
}

std::vector<HVACComponent> Mixer_Impl::edges(const boost::optional<HVACComponent> & prev)
{
  std::vector<HVACComponent> edges;
  if( auto edgeModelObject = this->outletModelObject() ) {
    if( auto edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
      edges.push_back(*edgeObject);
    }
  }
  return edges;
}

bool Mixer_Impl::isRemovable() const
{
  if( airLoopHVAC() || plantLoop() )
  {
    return false;
  }
  else
  {
    return true;
  }
}

} // detail

Mixer::Mixer(std::shared_ptr<detail::Mixer_Impl> p)
  : HVACComponent(std::move(p))
{}

Mixer::Mixer(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::Mixer_Impl>());
}

unsigned Mixer::outletPort() const
{
  return getImpl<detail::Mixer_Impl>()->outletPort();
}

unsigned Mixer::inletPort(unsigned branchIndex) const
{
  return getImpl<detail::Mixer_Impl>()->inletPort(branchIndex);
}

unsigned Mixer::nextInletPort() const
{
  return getImpl<detail::Mixer_Impl>()->nextInletPort();
}

boost::optional<ModelObject> Mixer::outletModelObject() const
{
  return getImpl<detail::Mixer_Impl>()->outletModelObject();
}

boost::optional<ModelObject> Mixer::inletModelObject(unsigned branchIndex) const
{
  return getImpl<detail::Mixer_Impl>()->inletModelObject(branchIndex);
}

boost::optional<ModelObject> Mixer::lastInletModelObject() const
{
  return getImpl<detail::Mixer_Impl>()->lastInletModelObject();
}

std::vector<ModelObject> Mixer::inletModelObjects() const
{
  return getImpl<detail::Mixer_Impl>()->inletModelObjects();
}

unsigned Mixer::newInletPortAfterBranch(unsigned branchIndex)
{
  return getImpl<detail::Mixer_Impl>()->newInletPortAfterBranch(branchIndex);
}

unsigned Mixer::branchIndexForInletModelObject( ModelObject modelObject ) const
{
  return getImpl<detail::Mixer_Impl>()->branchIndexForInletModelObject(modelObject);
}

unsigned Mixer::nextBranchIndex() const
{
  return getImpl<detail::Mixer_Impl>()->nextBranchIndex();
}

void Mixer::removePortForBranch(unsigned branchIndex)
{
  return getImpl<detail::Mixer_Impl>()->removePortForBranch(branchIndex);
}

bool Mixer::isRemovable() const
{
  return getImpl<detail::Mixer_Impl>()->isRemovable();
}

} // model

} // openstudio

