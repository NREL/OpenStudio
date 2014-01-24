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

#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail {

Splitter_Impl::Splitter_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

Splitter_Impl::Splitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{ 
}

Splitter_Impl::Splitter_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other, 
  Model_Impl* model, 
  bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

Splitter_Impl::Splitter_Impl(const Splitter_Impl& other, 
  Model_Impl* model, 
  bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

boost::optional<ModelObject> Splitter_Impl::inletModelObject()
{
  return connectedObject( inletPort() );
}

boost::optional<ModelObject> Splitter_Impl::outletModelObject(unsigned branchIndex)
{
  return connectedObject( outletPort( branchIndex ) );
}

boost::optional<ModelObject> Splitter_Impl::lastOutletModelObject()
{
  std::vector<ModelObject> objects = outletModelObjects();
  if( objects.size() > 0 )
  {
    return OptionalModelObject(objects.back());
  }
  else
  {
    return OptionalModelObject();
  }
}

unsigned Splitter_Impl::newOutletPortAfterBranch(unsigned branchIndex)
{
  //std::vector<ModelObject> modelObjects = outletModelObjects();
  int stop = nextBranchIndex() - 1;
  for(int i = branchIndex; i < stop; i++ )
  {
    ModelObject mo = outletModelObject(i).get();
    unsigned port = connectedObjectPort( outletPort(i) ).get();
    model().connect(getObject<ModelObject>(),outletPort(i+2),mo,port);
  }

  Model _model = model();
  Node node( _model );

  _model.connect( getObject<ModelObject>(),outletPort(branchIndex+1),node,node.inletPort() );

  return outletPort(branchIndex++);
}

unsigned Splitter_Impl::branchIndexForOutletModelObject( ModelObject modelObject )
{
  int stop = nextBranchIndex();
  for(int i = 0; i < stop; i++)
  {
    if( outletModelObject(i) == modelObject )
    {
      return i;
    }
  }
  return 0;
}

unsigned Splitter_Impl::nextBranchIndex()
{
  unsigned i = 0;
  OptionalModelObject modelObject;
  modelObject = connectedObject( this->outletPort(i) );
  while( modelObject )
  {
    i++;
    modelObject = connectedObject( this->outletPort(i) );
  }
  return i;
}

void Splitter_Impl::removePortForBranch(unsigned branchIndex)
{
  int _nextBranchIndex = nextBranchIndex();
  model().disconnect(getObject<ModelObject>(),outletPort(branchIndex));
  //std::vector<ModelObject> modelObjects = outletModelObjects();
  for(int i = branchIndex + 1; i < _nextBranchIndex; i++ )
  {
    ModelObject mo = outletModelObject(i).get();
    unsigned port = connectedObjectPort( outletPort(i) ).get();
    model().disconnect(getObject<ModelObject>(),outletPort(i));
    model().connect(getObject<ModelObject>(),outletPort(i-1),mo,port);
  }
}

std::vector<ModelObject> Splitter_Impl::outletModelObjects()
{
  std::vector<ModelObject> result;
  int stop = nextBranchIndex();
  for( int i = 0; i < stop; i++ )
  {
    result.push_back( this->outletModelObject( i ).get() );
  }
  return result;
}

bool Splitter_Impl::isRemovable() const
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

//bool Splitter_Impl::addToNode(Node & node)
//{
//  return false;
//}

//ModelObject Splitter_Impl::clone(Model model) const
//{
//  return HVACComponent_Impl::clone( model );
//}

} // detail

Splitter::Splitter(boost::shared_ptr<detail::Splitter_Impl> p)
  : HVACComponent(p)
{}

Splitter::Splitter(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::Splitter_Impl>());
}     

unsigned Splitter::inletPort()
{
  return getImpl<detail::Splitter_Impl>()->inletPort();
}

unsigned Splitter::outletPort(unsigned branchIndex)
{
  return getImpl<detail::Splitter_Impl>()->outletPort(branchIndex);
}

unsigned Splitter::nextOutletPort()
{
  return getImpl<detail::Splitter_Impl>()->nextOutletPort();
}

boost::optional<ModelObject> Splitter::inletModelObject()
{
  return getImpl<detail::Splitter_Impl>()->inletModelObject();
}

boost::optional<ModelObject> Splitter::outletModelObject(unsigned branchIndex)
{
  return getImpl<detail::Splitter_Impl>()->outletModelObject(branchIndex);
}

boost::optional<ModelObject> Splitter::lastOutletModelObject()
{
  return getImpl<detail::Splitter_Impl>()->lastOutletModelObject();
}

std::vector<ModelObject> Splitter::outletModelObjects()
{
  return getImpl<detail::Splitter_Impl>()->outletModelObjects();
}

unsigned Splitter::newOutletPortAfterBranch(unsigned branchIndex)
{
  return getImpl<detail::Splitter_Impl>()->newOutletPortAfterBranch(branchIndex);
}

unsigned Splitter::branchIndexForOutletModelObject( ModelObject modelObject )
{
  return getImpl<detail::Splitter_Impl>()->branchIndexForOutletModelObject(modelObject);
}

unsigned Splitter::nextBranchIndex()
{
  return getImpl<detail::Splitter_Impl>()->nextBranchIndex();
}

void Splitter::removePortForBranch(unsigned branchIndex)
{
  return getImpl<detail::Splitter_Impl>()->removePortForBranch(branchIndex);
}

bool Splitter::isRemovable() const
{
  return getImpl<detail::Splitter_Impl>()->isRemovable();
}

} // model

} // openstudio

