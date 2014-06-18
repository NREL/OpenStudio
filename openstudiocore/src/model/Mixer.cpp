/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

boost::optional<ModelObject> Mixer_Impl::outletModelObject()
{
  return connectedObject( outletPort() );
}

boost::optional<ModelObject> Mixer_Impl::inletModelObject(unsigned branchIndex)
{
  return connectedObject( inletPort( branchIndex ) );
}

boost::optional<ModelObject> Mixer_Impl::lastInletModelObject()
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

unsigned Mixer_Impl::branchIndexForInletModelObject( ModelObject modelObject )
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

unsigned Mixer_Impl::nextBranchIndex()
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

std::vector<ModelObject> Mixer_Impl::inletModelObjects()
{
  std::vector<ModelObject> result;
  unsigned stop = nextBranchIndex();
  for( unsigned i = 0; i < stop; i++ )
  {
    result.push_back( this->inletModelObject( i ).get() );
  }
  return result;
}

std::vector<HVACComponent> Mixer_Impl::edges(bool isDemandComponent)
{
  std::vector<HVACComponent> edges;
  if( boost::optional<ModelObject> edgeModelObject = this->outletModelObject() ) {
    if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
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
  : HVACComponent(p)
{}

Mixer::Mixer(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::Mixer_Impl>());
}     

unsigned Mixer::outletPort()
{
  return getImpl<detail::Mixer_Impl>()->outletPort();
}     

unsigned Mixer::inletPort(unsigned branchIndex)
{
  return getImpl<detail::Mixer_Impl>()->inletPort(branchIndex);
}     

unsigned Mixer::nextInletPort()
{
  return getImpl<detail::Mixer_Impl>()->nextInletPort();
}     

boost::optional<ModelObject> Mixer::outletModelObject()
{
  return getImpl<detail::Mixer_Impl>()->outletModelObject();
}     

boost::optional<ModelObject> Mixer::inletModelObject(unsigned branchIndex)
{
  return getImpl<detail::Mixer_Impl>()->inletModelObject(branchIndex);
}     

boost::optional<ModelObject> Mixer::lastInletModelObject()
{
  return getImpl<detail::Mixer_Impl>()->lastInletModelObject();
}     

std::vector<ModelObject> Mixer::inletModelObjects()
{
  return getImpl<detail::Mixer_Impl>()->inletModelObjects();
}     

unsigned Mixer::newInletPortAfterBranch(unsigned branchIndex)
{
  return getImpl<detail::Mixer_Impl>()->newInletPortAfterBranch(branchIndex);
}     

unsigned Mixer::branchIndexForInletModelObject( ModelObject modelObject )
{
  return getImpl<detail::Mixer_Impl>()->branchIndexForInletModelObject(modelObject);
}     

unsigned Mixer::nextBranchIndex()
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

