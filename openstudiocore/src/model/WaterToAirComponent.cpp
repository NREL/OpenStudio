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

#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "Model.hpp"

#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

WaterToAirComponent_Impl::WaterToAirComponent_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

WaterToAirComponent_Impl::WaterToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{ 
}

WaterToAirComponent_Impl::WaterToAirComponent_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

WaterToAirComponent_Impl::WaterToAirComponent_Impl(const WaterToAirComponent_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

boost::optional<Loop> WaterToAirComponent_Impl::loop() const
{
  return boost::none;
}

boost::optional<ModelObject> WaterToAirComponent_Impl::airInletModelObject()
{
  return connectedObject( airInletPort() );
}

boost::optional<ModelObject> WaterToAirComponent_Impl::airOutletModelObject()
{
  return connectedObject( airOutletPort() );
}

boost::optional<ModelObject> WaterToAirComponent_Impl::waterInletModelObject()
{
  return connectedObject( waterInletPort() );
}

boost::optional<ModelObject> WaterToAirComponent_Impl::waterOutletModelObject()
{
  return connectedObject( waterOutletPort() );
}

std::vector<HVACComponent> WaterToAirComponent_Impl::edges(bool isDemandComponent)
{
  std::vector<HVACComponent> edges;
  if( isDemandComponent ) {
    if( boost::optional<ModelObject> edgeModelObject = this->waterOutletModelObject() ) {
      if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
        edges.push_back(*edgeObject);
      }
    }
  } else {
    if( boost::optional<ModelObject> edgeModelObject = this->airOutletModelObject() ) {
      if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
        edges.push_back(*edgeObject);
      }
    }
  }
  return edges;
}

boost::optional<AirLoopHVAC> WaterToAirComponent_Impl::airLoopHVAC() const
{
  return HVACComponent_Impl::airLoopHVAC();
}

boost::optional<PlantLoop> WaterToAirComponent_Impl::plantLoop() const
{
  return HVACComponent_Impl::plantLoop();
}

bool WaterToAirComponent_Impl::addToNode(Node & node)
{
  Model _model = node.model(); 
  ModelObject thisModelObject = getObject<ModelObject>();
  boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = node.airLoopHVACOutdoorAirSystem();

  if( node.airLoopHVAC() && ! oaSystem )
  {
    AirLoopHVAC airLoop = node.airLoopHVAC().get();
    if( airLoop.demandComponent( node.handle() ) )
    {
      return false;
    }
    else
    {
      if( airLoopHVAC() )
      {
        removeFromAirLoopHVAC();
      }

      if( node == airLoop.supplyOutletNode() &&
          node.inletModelObject().get() == airLoop.supplyInletNode() )
      {
        unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
        ModelObject oldTargetModelObject = node;

        _model.connect( oldSourceModelObject, oldOutletPort,
                        thisModelObject, airInletPort() );
        _model.connect( thisModelObject, airOutletPort(),
                        oldTargetModelObject, oldInletPort );
        return true;
      }
      if( node == airLoop.supplyInletNode() &&
          node.outletModelObject().get() == airLoop.supplyOutletNode() )
      {
        unsigned oldOutletPort = node.outletPort();
        unsigned oldInletPort = node.connectedObjectPort( node.outletPort() ).get();
        ModelObject oldSourceModelObject = node;
        ModelObject oldTargetModelObject = node.outletModelObject().get();

        _model.connect( oldSourceModelObject, oldOutletPort,
                        thisModelObject, airInletPort() );
        _model.connect( thisModelObject, airOutletPort(),
                        oldTargetModelObject, oldInletPort );
        return true;
      }
      else if( node == airLoop.supplyOutletNode() )
      {
        unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
        ModelObject oldTargetModelObject = node;

        Node newNode( _model );
        _model.connect( oldSourceModelObject, oldOutletPort,
                        newNode, newNode.inletPort() );
        _model.connect( newNode, newNode.outletPort(),
                        thisModelObject, airInletPort() );                        
        _model.connect( thisModelObject, airOutletPort(),
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
                        thisModelObject, airInletPort() );
        _model.connect( thisModelObject, airOutletPort(),
                        newNode, newNode.inletPort() );
        _model.connect( newNode, newNode.outletPort(),
                        oldTargetModelObject, oldInletPort );
        return true;
      }
    }
  }
  else if( node.plantLoop() )
  {
    PlantLoop _plantLoop = node.plantLoop().get();
    if( ! _plantLoop.demandComponent( node.handle() ) )
    {
      return false;
    }
    else
    {
      if( plantLoop() )
      {
        removeFromPlantLoop();
      }

      if( node == _plantLoop.demandOutletNode() &&
          node.inletModelObject().get() == _plantLoop.demandInletNode() )
      {
        unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
        ModelObject oldTargetModelObject = node;

        _model.connect( oldSourceModelObject, oldOutletPort,
                        thisModelObject, waterInletPort() );
        _model.connect( thisModelObject, waterOutletPort(),
                        oldTargetModelObject, oldInletPort );
        return true;
      }
      else if( node == _plantLoop.demandOutletNode() )
      {
        unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
        ModelObject oldTargetModelObject = node;

        Node newNode( _model );
        _model.connect( oldSourceModelObject, oldOutletPort,
                        newNode, newNode.inletPort() );
        _model.connect( newNode, newNode.outletPort(),
                        thisModelObject, waterInletPort() );                        
        _model.connect( thisModelObject, waterOutletPort(),
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
                        thisModelObject, waterInletPort() );
        _model.connect( thisModelObject, waterOutletPort(),
                        newNode, newNode.inletPort() );
        _model.connect( newNode, newNode.outletPort(),
                        oldTargetModelObject, oldInletPort );
        return true;
      }
    }
  }
  else
  {
    return false;
  }
}

std::vector<openstudio::IdfObject> WaterToAirComponent_Impl::remove()
{
  if( isRemovable() )
  {
    removeFromAirLoopHVAC();

    removeFromPlantLoop();

    return HVACComponent_Impl::remove();
  }
  
  return std::vector<IdfObject>();
}

ModelObject WaterToAirComponent_Impl::clone(Model model) const
{
  WaterToAirComponent mo = HVACComponent_Impl::clone( model ).cast<WaterToAirComponent>();

  mo.setString(mo.airInletPort(),""); 
  mo.setString(mo.airOutletPort(),""); 

  mo.setString(mo.waterInletPort(),""); 
  mo.setString(mo.waterOutletPort(),""); 

  return mo;
}

bool WaterToAirComponent_Impl::addToSplitter(Splitter & splitter)
{
  //Model _model = splitter.model();
  //WaterToAirComponent thisModelObject = getObject<ModelObject>().optionalCast<WaterToAirComponent>().get();
  //if( splitter.plantLoop() )
  //{
  //  PlantLoop plantLoop = splitter.plantLoop().get();
  //  if( plantLoop.demandComponent( splitter.handle() ) )
  //  {
  //    Mixer mixer = plantLoop.demandMixer().get();
  //    unsigned splitterPort = splitter.newOutletPortAfterBranch( splitter.nextBranchIndex() );
  //    unsigned mixerPort = mixer.newInletPortAfterBranch( mixer.nextBranchIndex() );
  //    Node inletNode(_model);
  //    Node outletNode(_model);

  //    _model.connect( splitter, splitterPort,
  //                    inletNode, inletNode.inletPort() );
  //    _model.connect( inletNode, inletNode.outletPort(),
  //                    thisModelObject, thisModelObject.waterInletPort() );
  //    _model.connect( thisModelObject, thisModelObject.waterOutletPort(),
  //                    outletNode, outletNode.inletPort() );
  //    _model.connect( outletNode, outletNode.outletPort(),
  //                    mixer, mixerPort );

  //    return true;
  //  }
  //  else
  //  {
  //    return false;
  //  }
  //}
  //else if( splitter.airLoopHVAC() )
  //{
  //  return false;
  //}
  //else
  //{
  //  return false;
  //}
  return false;
}

void WaterToAirComponent_Impl::disconnectWaterSide()
{
  Model _model = this->model();

  ModelObject mo = this->getObject<ModelObject>();
  
  _model.disconnect(mo,this->waterInletPort());

  _model.disconnect(mo,this->waterOutletPort());
}

void WaterToAirComponent_Impl::disconnectAirSide()
{
  Model _model = this->model();

  ModelObject mo = this->getObject<ModelObject>();
  
  _model.disconnect(mo,this->airInletPort());

  _model.disconnect(mo,this->airOutletPort());
}

void WaterToAirComponent_Impl::disconnect()
{
  this->disconnectWaterSide();
  
  this->disconnectAirSide();
}

bool WaterToAirComponent_Impl::removeFromAirLoopHVAC()
{
  Model _model = model(); 
  ModelObject thisObject = getObject<ModelObject>();

  if( boost::optional<AirLoopHVAC> airLoop = airLoopHVAC() )
  {
    if( airOutletModelObject().get() == airLoop->supplyOutletNodes().front() &&
        airInletModelObject().get() == airLoop->supplyInletNode() )
    {
      ModelObject sourceModelObject = airInletModelObject().get(); 
      unsigned sourcePort = connectedObjectPort( airInletPort() ).get();
      ModelObject targetModelObject = airOutletModelObject().get();
      unsigned targetPort = connectedObjectPort( airOutletPort() ).get();

      model().disconnect(thisObject,airOutletPort()); 
      model().disconnect(thisObject,airInletPort()); 

      _model.connect( sourceModelObject, sourcePort,
                      targetModelObject, targetPort );
      return true;
    }
    else if( airOutletModelObject().get() == airLoop->supplyOutletNodes().front() )
    {
      Node sourceModelObject = airInletModelObject()->optionalCast<Node>().get(); 
      ModelObject targetModelObject = airOutletModelObject().get();
      unsigned targetPort = connectedObjectPort( airOutletPort() ).get();
      ModelObject source2ModelObject = sourceModelObject.inletModelObject().get(); 
      unsigned source2Port = sourceModelObject.connectedObjectPort( sourceModelObject.inletPort() ).get(); 

      model().disconnect(thisObject,airOutletPort()); 
      model().disconnect(thisObject,airInletPort()); 

      _model.connect( source2ModelObject, source2Port,
                      targetModelObject, targetPort );

      sourceModelObject.remove();

      return true;
    }
    else
    {
      ModelObject sourceModelObject = airInletModelObject().get();
      unsigned sourcePort = connectedObjectPort( airInletPort() ).get();
      Node targetModelObject = airOutletModelObject()->optionalCast<Node>().get();
      ModelObject target2ModelObject = targetModelObject.outletModelObject().get();
      unsigned target2Port = targetModelObject.connectedObjectPort( targetModelObject.outletPort() ).get();

      model().disconnect(thisObject,airOutletPort()); 
      model().disconnect(thisObject,airInletPort()); 


      _model.connect( sourceModelObject, sourcePort,
                      target2ModelObject, target2Port );

      targetModelObject.remove();

      return true;
    }
  }

  return false;
}

bool WaterToAirComponent_Impl::removeFromPlantLoop()
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

  if( _loop && _loop->demandComponent(handle()) )
  {
    targetModelObject = waterOutletModelObject()->optionalCast<Node>(); 
    sourceModelObject = waterInletModelObject()->optionalCast<Node>();
    targetPort = connectedObjectPort(waterOutletPort());
    sourcePort = connectedObjectPort(waterInletPort());
    
    target2ModelObject = targetModelObject->outletModelObject();
    source2ModelObject = sourceModelObject->inletModelObject();
    target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
    source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

    boost::optional<Node> demandInletNode = _loop->demandInletNode();
    boost::optional<Node> demandOutletNode = _loop->demandOutletNode();

    Mixer mixer = _loop->demandMixer();
    Splitter splitter = _loop->demandSplitter();
    
    // If the component is stuck directly between the inlet and outlet node.
    if( demandInletNode->handle() == waterInletModelObject()->handle() 
        &&
        demandOutletNode->handle() == waterOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,waterOutletPort()); 
      model().disconnect(thisObject,waterInletPort()); 

      model().connect( demandInletNode.get(), demandInletNode->outletPort(), 
                       demandOutletNode.get(), demandOutletNode->inletPort() );
      
      return true;
    }
    // If the component is at the very end of the supply path, but there is another component preceding this one.
    else if( demandOutletNode->handle() == waterOutletModelObject()->handle() )
    {
      model().disconnect(thisObject,waterOutletPort()); 
      model().disconnect(thisObject,waterInletPort()); 

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

      _model.disconnect(thisObject,waterOutletPort()); 
      _model.disconnect(thisObject,waterInletPort()); 

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
      model().disconnect(thisObject,waterOutletPort()); 
      model().disconnect(thisObject,waterInletPort()); 

      model().connect( sourceModelObject.get(), sourcePort.get(), 
                       target2ModelObject.get(), target2Port.get() );
      targetModelObject->remove();
      return true;
    }
  }

  return false;
}

} // detail

WaterToAirComponent::WaterToAirComponent(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::WaterToAirComponent_Impl>());
}     

WaterToAirComponent::WaterToAirComponent(std::shared_ptr<detail::WaterToAirComponent_Impl> p)
  : HVACComponent(p)
{}

boost::optional<ModelObject> WaterToAirComponent::airInletModelObject()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->airInletModelObject();
}

boost::optional<ModelObject> WaterToAirComponent::airOutletModelObject()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->airOutletModelObject();
}

boost::optional<ModelObject> WaterToAirComponent::waterInletModelObject()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->waterInletModelObject();
}

boost::optional<ModelObject> WaterToAirComponent::waterOutletModelObject()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->waterOutletModelObject();
}

boost::optional<AirLoopHVAC> WaterToAirComponent::airLoopHVAC() const
{
  return getImpl<detail::WaterToAirComponent_Impl>()->airLoopHVAC();
}

boost::optional<PlantLoop> WaterToAirComponent::plantLoop() const
{
  return getImpl<detail::WaterToAirComponent_Impl>()->plantLoop();
}

bool WaterToAirComponent::addToNode(Node & node)
{
  return getImpl<detail::WaterToAirComponent_Impl>()->addToNode( node );
}

std::vector<openstudio::IdfObject> WaterToAirComponent::remove()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->remove();
}

ModelObject WaterToAirComponent::clone(Model model) const
{
  return getImpl<detail::WaterToAirComponent_Impl>()->clone( model );
}

unsigned WaterToAirComponent::airInletPort()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->airInletPort();
}

unsigned WaterToAirComponent::airOutletPort()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->airOutletPort();
}

unsigned WaterToAirComponent::waterInletPort()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->waterInletPort();
}

unsigned WaterToAirComponent::waterOutletPort()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->waterOutletPort();
}

bool WaterToAirComponent::addToSplitter(Splitter & splitter)
{
  return getImpl<detail::WaterToAirComponent_Impl>()->addToSplitter( splitter );
}

void WaterToAirComponent::disconnectWaterSide()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->disconnectWaterSide();
}

void WaterToAirComponent::disconnectAirSide()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->disconnectAirSide();
}

bool WaterToAirComponent::removeFromAirLoopHVAC()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->removeFromAirLoopHVAC();
}

bool WaterToAirComponent::removeFromPlantLoop()
{
  return getImpl<detail::WaterToAirComponent_Impl>()->removeFromPlantLoop();
}

} // model

} // openstudio

