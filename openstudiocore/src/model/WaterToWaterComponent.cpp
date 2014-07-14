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

#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{ 
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(const WaterToWaterComponent_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

OptionalModelObject WaterToWaterComponent_Impl::supplyInletModelObject()
{
  return connectedObject(supplyInletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::supplyOutletModelObject()
{
  return connectedObject(supplyOutletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::demandInletModelObject()
{
  return connectedObject(demandInletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::demandOutletModelObject()
{
  return connectedObject(demandOutletPort());
}

std::vector<HVACComponent> WaterToWaterComponent_Impl::edges(bool isDemandComponent)
{
  std::vector<HVACComponent> edges;
  if( isDemandComponent ) {
    if( boost::optional<ModelObject> edgeModelObject = this->demandOutletModelObject() ) {
      if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
        edges.push_back(*edgeObject);
      }
    }
  } else {
    if( boost::optional<ModelObject> edgeModelObject = this->supplyOutletModelObject() ) {
      if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
        edges.push_back(*edgeObject);
      }
    }
  }
  return edges;
}

std::vector<openstudio::IdfObject> WaterToWaterComponent_Impl::remove()
{
  removeFromPlantLoop();

  removeFromSecondaryPlantLoop();

  return HVACComponent_Impl::remove();
}

boost::optional<ParentObject> WaterToWaterComponent_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

std::vector<ModelObject> WaterToWaterComponent_Impl::children() const
{
  std::vector<ModelObject> result;

  return result;
}

void WaterToWaterComponent_Impl::disconnect()
{
  ModelObject mo = this->getObject<ModelObject>();
  this->model().disconnect(mo, this->supplyInletPort());
  this->model().disconnect(mo, this->supplyOutletPort());
  this->model().disconnect(mo, this->demandInletPort());
  this->model().disconnect(mo, this->demandOutletPort());
}

bool WaterToWaterComponent_Impl::addToNode(Node & node)
{
  Model _model = model();
  ModelObject thisModelObject = getObject<ModelObject>();

  if( node.model() != _model )
  {
    return false;
  }

  if( boost::optional<PlantLoop> _plantLoop = node.plantLoop() )
  {
    int inletport;
    int outletport;

    boost::optional<Node> loopInletNode;
    boost::optional<Node> loopOutletNode;

    if( _plantLoop->supplyComponent(node.handle()) )
    {
      loopOutletNode = _plantLoop->supplyOutletNode();
      loopInletNode = _plantLoop->supplyInletNode();

      inletport = supplyInletPort();
      outletport = supplyOutletPort();

      if( plantLoop() )
      {
        removeFromPlantLoop();
      }
    }
    else if( _plantLoop->demandComponent(node.handle()) )
    {
      loopOutletNode = _plantLoop->demandOutletNode();
      loopInletNode = _plantLoop->demandInletNode();

      inletport = demandInletPort();
      outletport = demandOutletPort();

      if( secondaryPlantLoop() )
      {
        removeFromSecondaryPlantLoop();
      }
    }
    else
    {
      return false;
    }

    if( node == loopOutletNode.get() &&
        node.inletModelObject().get() == loopInletNode.get() )
    {
      unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
      unsigned oldInletPort = node.inletPort();
      ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
      ModelObject oldTargetModelObject = node;

      _model.connect( oldSourceModelObject, oldOutletPort,
                      thisModelObject, inletport );
      _model.connect( thisModelObject, outletport,
                      oldTargetModelObject, oldInletPort );
      return true;
    }
    else if( node == loopOutletNode.get() )
    {
      unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
      unsigned oldInletPort = node.inletPort();
      ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
      ModelObject oldTargetModelObject = node;

      Node newNode( _model );
      _model.connect( oldSourceModelObject, oldOutletPort,
                      newNode, newNode.inletPort() );
      _model.connect( newNode, newNode.outletPort(),
                      thisModelObject, inletport );                        
      _model.connect( thisModelObject, outletport,
                      oldTargetModelObject, oldInletPort );
      return true;
    }
    else
    {
      unsigned oldOutletPort = node.outletPort();
      unsigned oldInletPort = node.connectedObjectPort( node.outletPort() ).get();
      ModelObject oldSourceModelObject = node;
      ModelObject oldTargetModelObject = node.connectedObject( node.outletPort() ).get();
  
      Node newNode( _model );
      _model.connect( oldSourceModelObject, oldOutletPort,
                      thisModelObject, inletport );
      _model.connect( thisModelObject, outletport,
                      newNode, newNode.inletPort() );
      _model.connect( newNode, newNode.outletPort(),
                      oldTargetModelObject, oldInletPort );
      return true;
    }
  }

  return false;
}

ModelObject WaterToWaterComponent_Impl::clone(Model model) const
{
  WaterToWaterComponent mo =  HVACComponent_Impl::clone( model ).cast<WaterToWaterComponent>();

  mo.setString(mo.supplyInletPort(),""); 
  mo.setString(mo.supplyOutletPort(),""); 
  mo.setString(mo.demandInletPort(),""); 
  mo.setString(mo.demandOutletPort(),""); 

  return mo;
}

boost::optional<PlantLoop> WaterToWaterComponent_Impl::plantLoop() const
{
  if( m_plantLoop )
  {
    return m_plantLoop;
  }
  else
  {
    std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

    for(const auto & elem : plantLoops)
    {
      OptionalPlantLoop plantLoop = elem.optionalCast<PlantLoop>();
      if(plantLoop)
      {
        if( plantLoop->supplyComponent(this->handle()) )
        {
          m_plantLoop = plantLoop;

          return plantLoop;
        }
      }
    }
  }

  return boost::none;
}

boost::optional<PlantLoop> WaterToWaterComponent_Impl::secondaryPlantLoop() const
{
  if( m_secondaryPlantLoop )
  {
    return m_secondaryPlantLoop;
  }
  else
  {
    std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

    for(const auto & elem : plantLoops)
    {
      OptionalPlantLoop plantLoop = elem.optionalCast<PlantLoop>();
      if(plantLoop)
      {
        if( plantLoop->demandComponent(this->handle()) )
        {
          m_secondaryPlantLoop = plantLoop;

          return plantLoop;
        }
      }
    }
  }

  return boost::none;
}

bool WaterToWaterComponent_Impl::removeFromPlantLoop()
{
  boost::optional<PlantLoop> _loop = plantLoop();

  OptionalNode targetModelObject;
  OptionalNode sourceModelObject;
  OptionalUnsigned targetPort;
  OptionalUnsigned sourcePort;

  OptionalModelObject target2ModelObject; 
  OptionalModelObject source2ModelObject;
  OptionalUnsigned target2Port;
  OptionalUnsigned source2Port;

  HVACComponent thisObject = getObject<HVACComponent>();

  if( _loop && _loop->supplyComponent(handle()) )
  {
    targetModelObject = supplyOutletModelObject()->optionalCast<Node>(); 
    sourceModelObject = supplyInletModelObject()->optionalCast<Node>();
    targetPort = connectedObjectPort(supplyOutletPort());
    sourcePort = connectedObjectPort(supplyInletPort());
    
    target2ModelObject = targetModelObject->outletModelObject();
    source2ModelObject = sourceModelObject->inletModelObject();
    target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
    source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

    boost::optional<Node> supplyInletNode = _loop->supplyInletNode();
    boost::optional<Node> supplyOutletNode = _loop->supplyOutletNode();

    Splitter splitter = _loop->supplySplitter();
    Mixer mixer = _loop->supplyMixer();
    
    // If the component is stuck directly between the inlet and outlet node.
    if( supplyInletNode->handle() == supplyInletModelObject()->handle() 
        &&
        supplyOutletNode->handle() == supplyOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,supplyOutletPort()); 
      model().disconnect(thisObject,supplyInletPort()); 

      model().connect( supplyInletNode.get(), supplyInletNode->outletPort(), 
                       supplyOutletNode.get(), supplyOutletNode->inletPort() );

      return true;
    }
    // If the component is at the very end of the supply path, but there is another component preceding this one.
    else if( supplyOutletNode->handle() == supplyOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,supplyOutletPort()); 
      model().disconnect(thisObject,supplyInletPort()); 

      model().connect( source2ModelObject.get(), source2Port.get(), 
                       supplyOutletNode.get(), supplyOutletNode->inletPort() );

      sourceModelObject->remove();

      return true;
    }
    // If the component is the only component (except nodes) between a splitter mixer pair
    else if( (target2ModelObject.get() == mixer) && 
             (source2ModelObject.get() == splitter) )
    {
      Model _model = model();

      int i = splitter.branchIndexForOutletModelObject(sourceModelObject.get());
      int j = mixer.branchIndexForInletModelObject(targetModelObject.get());

      OS_ASSERT(i == j);

      splitter.removePortForBranch(i);
      mixer.removePortForBranch(i);

      _model.disconnect(thisObject,supplyOutletPort()); 
      _model.disconnect(thisObject,supplyInletPort()); 

      targetModelObject->remove();
      sourceModelObject->remove();

      if( ! splitter.lastOutletModelObject() )
      {
        Node newNode(_model);
        _model.connect(splitter,splitter.nextOutletPort(),newNode,newNode.inletPort());
        _model.connect(newNode,newNode.outletPort(),mixer,mixer.nextInletPort());
      }

      return true;
    }
    // Else remove the component and the outlet node
    else
    {
      model().disconnect(thisObject,supplyOutletPort()); 
      model().disconnect(thisObject,supplyInletPort()); 

      model().connect( sourceModelObject.get(), sourcePort.get(), 
                       target2ModelObject.get(), target2Port.get() );
      targetModelObject->remove();
      return true;
    }
  }

  return false;
}

bool WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop()
{
  boost::optional<PlantLoop> _secondaryLoop = secondaryPlantLoop();

  OptionalNode targetModelObject;
  OptionalNode sourceModelObject;
  OptionalUnsigned targetPort;
  OptionalUnsigned sourcePort;

  OptionalModelObject target2ModelObject; 
  OptionalModelObject source2ModelObject;
  OptionalUnsigned target2Port;
  OptionalUnsigned source2Port;

  HVACComponent thisObject = getObject<HVACComponent>();

  if( _secondaryLoop && _secondaryLoop->demandComponent(handle()) )
  {
    targetModelObject = demandOutletModelObject()->optionalCast<Node>(); 
    sourceModelObject = demandInletModelObject()->optionalCast<Node>();
    targetPort = connectedObjectPort(demandOutletPort());
    sourcePort = connectedObjectPort(demandInletPort());
    
    target2ModelObject = targetModelObject->outletModelObject();
    source2ModelObject = sourceModelObject->inletModelObject();
    target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
    source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

    boost::optional<Node> demandInletNode = _secondaryLoop->demandInletNode();
    boost::optional<Node> demandOutletNode = _secondaryLoop->demandOutletNode();

    Mixer mixer = _secondaryLoop->demandMixer();
    Splitter splitter = _secondaryLoop->demandSplitter();
    
    // If the component is stuck directly between the inlet and outlet node.
    if( demandInletNode->handle() == demandInletModelObject()->handle() 
        &&
        demandOutletNode->handle() == demandOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,demandOutletPort()); 
      model().disconnect(thisObject,demandInletPort()); 

      model().connect( demandInletNode.get(), demandInletNode->outletPort(), 
                       demandOutletNode.get(), demandOutletNode->inletPort() );
      
      return true;
    }
    // If the component is at the very end of the supply path, but there is another component preceding this one.
    else if( demandOutletNode->handle() == demandOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,demandOutletPort()); 
      model().disconnect(thisObject,demandInletPort()); 

      model().connect( source2ModelObject.get(), source2Port.get(), 
                       demandOutletNode.get(), demandOutletNode->inletPort() );
      sourceModelObject->remove();
      return true;
    }
    // If the component is the only component (except nodes) between a splitter mixer pair
    else if( (target2ModelObject.get() == mixer) && 
             (source2ModelObject.get() == splitter) )
    {
      Model _model = model();

      int i = splitter.branchIndexForOutletModelObject(sourceModelObject.get());
      int j = mixer.branchIndexForInletModelObject(targetModelObject.get());

      OS_ASSERT(i == j);

      splitter.removePortForBranch(i);
      mixer.removePortForBranch(i);

      model().disconnect(thisObject,demandOutletPort()); 
      model().disconnect(thisObject,demandInletPort()); 

      targetModelObject->remove();
      sourceModelObject->remove();

      if( ! splitter.lastOutletModelObject() )
      {
        Node newNode(_model);
        _model.connect(splitter,splitter.nextOutletPort(),newNode,newNode.inletPort());
        _model.connect(newNode,newNode.outletPort(),mixer,mixer.nextInletPort());
      }

      return true;
    }
    // Else remove the component and the outlet node
    else
    {
      model().disconnect(thisObject,demandOutletPort()); 
      model().disconnect(thisObject,demandInletPort()); 

      model().connect( sourceModelObject.get(), sourcePort.get(), 
                       target2ModelObject.get(), target2Port.get() );
      targetModelObject->remove();
      return true;
    }
  }

  return false;
}

} // detail

WaterToWaterComponent::WaterToWaterComponent(IddObjectType type,const Model& model) 
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::WaterToWaterComponent_Impl>());
}     

WaterToWaterComponent::WaterToWaterComponent(std::shared_ptr<detail::WaterToWaterComponent_Impl> p)
  : HVACComponent(p)
{}

unsigned WaterToWaterComponent::supplyInletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyInletPort();
}

unsigned WaterToWaterComponent::supplyOutletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyOutletPort();
}

OptionalModelObject WaterToWaterComponent::supplyInletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyInletModelObject();
}

OptionalModelObject WaterToWaterComponent::supplyOutletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyOutletModelObject();
}

unsigned WaterToWaterComponent::demandInletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandInletPort();
}

unsigned WaterToWaterComponent::demandOutletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandOutletPort();
}

OptionalModelObject WaterToWaterComponent::demandInletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandInletModelObject();
}

OptionalModelObject WaterToWaterComponent::demandOutletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandOutletModelObject();
}

std::vector<IdfObject> WaterToWaterComponent::remove()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->remove();
}

bool WaterToWaterComponent::addToNode(Node & node)
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->addToNode( node );
}

ModelObject WaterToWaterComponent::clone(Model model) const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->clone( model );
}

void WaterToWaterComponent::disconnect()
{
  getImpl<detail::WaterToWaterComponent_Impl>()->disconnect();
}

boost::optional<PlantLoop> WaterToWaterComponent::plantLoop() const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->plantLoop();
}

boost::optional<PlantLoop> WaterToWaterComponent::secondaryPlantLoop() const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->secondaryPlantLoop();
}

bool WaterToWaterComponent::removeFromPlantLoop()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->removeFromPlantLoop();
}

bool WaterToWaterComponent::removeFromSecondaryPlantLoop()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->removeFromSecondaryPlantLoop();
}

} // model

} // openstudio

