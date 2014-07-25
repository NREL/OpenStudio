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

#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
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

StraightComponent_Impl::StraightComponent_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

StraightComponent_Impl::StraightComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{ 
}

StraightComponent_Impl::StraightComponent_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

StraightComponent_Impl::StraightComponent_Impl(const StraightComponent_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

OptionalModelObject StraightComponent_Impl::inletModelObject()
{
  return connectedObject(inletPort());
}

OptionalModelObject StraightComponent_Impl::outletModelObject()
{
  return connectedObject(outletPort());
}

std::vector<openstudio::IdfObject> StraightComponent_Impl::remove()
{
  boost::optional<Loop> loop = this->loop();
  boost::optional<AirLoopHVAC> airLoop = this->airLoopHVAC();

  if( loop )
  {
    boost::optional<Splitter> splitter;
    boost::optional<Mixer> mixer;

    OptionalAirLoopHVACOutdoorAirSystem oaSystem;
    if( airLoop )
    {
      oaSystem = airLoop->airLoopHVACOutdoorAirSystem();
    }

    OptionalNode targetModelObject;
    OptionalNode sourceModelObject;
    OptionalUnsigned targetPort;
    OptionalUnsigned sourcePort;

    OptionalModelObject target2ModelObject; 
    OptionalModelObject source2ModelObject;
    OptionalUnsigned target2Port;
    OptionalUnsigned source2Port;

    HVACComponent thisObject = getObject<HVACComponent>();

    if( loop && loop->demandComponent(handle()) )
    {
      targetModelObject = outletModelObject()->optionalCast<Node>(); 
      sourceModelObject = inletModelObject()->optionalCast<Node>();
      targetPort = connectedObjectPort(outletPort());
      sourcePort = connectedObjectPort(inletPort());
      
      target2ModelObject = targetModelObject->outletModelObject();
      source2ModelObject = sourceModelObject->inletModelObject();
      target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
      source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

      boost::optional<Node> demandInletNode = loop->demandInletNode();
      boost::optional<Node> demandOutletNode = loop->demandOutletNode();

      Mixer mixer = loop->demandMixer();
      Splitter splitter = loop->demandSplitter();
      
      // If the component is stuck directly between the inlet and outlet node.
      if( demandInletNode->handle() == inletModelObject()->handle() 
          &&
          demandOutletNode->handle() == outletModelObject()->handle() )
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( demandInletNode.get(), demandInletNode->outletPort(), 
                         demandOutletNode.get(), demandOutletNode->inletPort() );
      }
      // If the component is at the very end of the supply path, but there is another component preceding this one.
      else if( demandOutletNode->handle() == outletModelObject()->handle() )
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( source2ModelObject.get(), source2Port.get(), 
                         demandOutletNode.get(), demandOutletNode->inletPort() );
        sourceModelObject->remove();
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

        _model.disconnect(thisObject,outletPort()); 
        _model.disconnect(thisObject,inletPort()); 

        targetModelObject->remove();
        sourceModelObject->remove();

        if( ! splitter.lastOutletModelObject() )
        {
          Node newNode(_model);
          _model.connect(splitter,splitter.nextOutletPort(),newNode,newNode.inletPort());
          _model.connect(newNode,newNode.outletPort(),mixer,mixer.nextInletPort());
        }
      }
      // Else remove the component and the outlet node
      else
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( sourceModelObject.get(), sourcePort.get(), 
                         target2ModelObject.get(), target2Port.get() );
        targetModelObject->remove();
      }
    }
    else if( loop && loop->supplyComponent(handle()) )
    {
      targetModelObject = outletModelObject()->optionalCast<Node>(); 
      sourceModelObject = inletModelObject()->optionalCast<Node>();
      targetPort = connectedObjectPort(outletPort());
      sourcePort = connectedObjectPort(inletPort());
      
      target2ModelObject = targetModelObject->outletModelObject();
      source2ModelObject = sourceModelObject->inletModelObject();
      target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
      source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

      boost::optional<Node> supplyInletNode = loop->supplyInletNode();
      boost::optional<Node> supplyOutletNode = loop->supplyOutletNode();

      boost::optional<PlantLoop> plantLoop = loop->optionalCast<PlantLoop>();

      if( plantLoop )
      {
        mixer = plantLoop->supplyMixer();
        splitter = plantLoop->supplySplitter();
      }

      // If the component is stuck directly between the inlet and outlet node.
      if( supplyInletNode->handle() == inletModelObject()->handle() 
          &&
          supplyOutletNode->handle() == outletModelObject()->handle() )
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( supplyInletNode.get(), supplyInletNode->outletPort(), 
                         supplyOutletNode.get(), supplyOutletNode->inletPort() );
      }
      // If the component is at the very end of the supply path, but there is another component preceding this one.
      else if( supplyOutletNode->handle() == outletModelObject()->handle() )
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( source2ModelObject.get(), source2Port.get(), 
                         supplyOutletNode.get(), supplyOutletNode->inletPort() );
        sourceModelObject->remove();
      }
      // If the component is the only component (except nodes) between a splitter mixer pair
      else if( mixer && (target2ModelObject.get() == mixer.get()) && 
               splitter && (source2ModelObject.get() == splitter.get()) )
      {
        Model _model = model();

        int i = splitter->branchIndexForOutletModelObject(sourceModelObject.get());
        int j = mixer->branchIndexForInletModelObject(targetModelObject.get());

        OS_ASSERT(i == j);

        splitter->removePortForBranch(i);
        mixer->removePortForBranch(i);

        _model.disconnect(thisObject,outletPort()); 
        _model.disconnect(thisObject,inletPort()); 

        targetModelObject->remove();
        sourceModelObject->remove();

        if( ! splitter->lastOutletModelObject() )
        {
          Node newNode(_model);
          _model.connect(splitter.get(),splitter->nextOutletPort(),newNode,newNode.inletPort());
          _model.connect(newNode,newNode.outletPort(),mixer.get(),mixer->nextInletPort());
        }
      }
      // Else remove the component and the outlet node
      else
      {
        model().disconnect(thisObject,outletPort()); 
        model().disconnect(thisObject,inletPort()); 

        model().connect( sourceModelObject.get(), sourcePort.get(), 
                         target2ModelObject.get(), target2Port.get() );
        targetModelObject->remove();
      }
    }
    else if( oaSystem )
    {
      if( oaSystem->component(this->handle()) )
      {
        OptionalNode outboardOANode = oaSystem->outboardOANode()->optionalCast<Node>();
        OptionalNode outboardReliefNode = oaSystem->outboardReliefNode()->optionalCast<Node>();
        OptionalNode outdoorAirNode = oaSystem->outdoorAirModelObject()->optionalCast<Node>();
        OptionalNode reliefAirNode = oaSystem->reliefAirModelObject()->optionalCast<Node>();
        
        // If the component is the most outboard on the oa path
        if( this->inletModelObject()->handle() == outboardOANode->handle() )
        {
          OptionalNode targetNode = this->outletModelObject()->optionalCast<Node>(); 
          OptionalUnsigned targetPort = this->connectedObjectPort( this->outletPort() );
          OptionalModelObject target2ModelObject = targetNode->outletModelObject();
          OptionalUnsigned target2Port = targetNode->connectedObjectPort(targetNode->outletPort());
          targetNode->disconnect();
          this->disconnect();
          this->model().connect(outboardOANode.get(),outboardOANode->outletPort(),target2ModelObject.get(),target2Port.get());
          targetNode->remove();
        }
        // If the component is the most outboard on the relief air path
        else if( this->outletModelObject()->handle() == outboardReliefNode->handle() )
        {
          OptionalNode sourceNode = this->inletModelObject()->optionalCast<Node>(); 
          OptionalUnsigned sourcePort = this->connectedObjectPort( this->inletPort() );
          OptionalModelObject source2ModelObject = sourceNode->inletModelObject();
          OptionalUnsigned source2Port = sourceNode->connectedObjectPort(sourceNode->inletPort());
          sourceNode->disconnect();
          this->disconnect();
          this->model().connect(source2ModelObject.get(),source2Port.get(),outboardReliefNode.get(),outboardReliefNode->inletPort());
          sourceNode->remove();
        }
        else
        {
          OptionalNode sourceNode = this->inletModelObject()->optionalCast<Node>(); 
          OptionalUnsigned sourcePort = this->connectedObjectPort(this->inletPort());
          OptionalNode targetNode = this->outletModelObject()->optionalCast<Node>();
          OptionalUnsigned targetPort = this->connectedObjectPort( this->outletPort() );
          OptionalModelObject source2ModelObject = sourceNode->inletModelObject();
          OptionalUnsigned source2Port = sourceNode->connectedObjectPort(sourceNode->inletPort());
          OptionalModelObject target2ModelObject = targetNode->outletModelObject();
          OptionalUnsigned target2Port = targetNode->connectedObjectPort(targetNode->outletPort());
          
          this->disconnect();
          
          targetNode->disconnect();
          
          this->model().connect(sourceNode.get(),sourcePort.get(),target2ModelObject.get(),target2Port.get());

          targetNode->remove();
        }
      }
    }
  }
          
  this->disconnect();

  return HVACComponent_Impl::remove();
}

boost::optional<ParentObject> StraightComponent_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

std::vector<ModelObject> StraightComponent_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

OptionalAirLoopHVAC StraightComponent_Impl::airLoopHVAC() const
{
  return HVACComponent_Impl::airLoopHVAC();
}

void StraightComponent_Impl::disconnect()
{
  ModelObject mo = this->getObject<ModelObject>();
  this->model().disconnect(mo, this->inletPort());
  this->model().disconnect(mo, this->outletPort());
}

std::vector<HVACComponent> StraightComponent_Impl::edges(bool isDemandComponent)
{
  std::vector<HVACComponent> edges;
  if( boost::optional<ModelObject> edgeModelObject = this->outletModelObject() ) {
    if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
      edges.push_back(*edgeObject);
    }
  }
  return edges;
}

bool StraightComponent_Impl::addToNode(Node & node)
{
  Model _model = node.model(); 
  ModelObject thisModelObject = getObject<ModelObject>();
  boost::optional<Loop> loop = node.loop();
  boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = node.airLoopHVACOutdoorAirSystem();

  if( boost::optional<Loop> currentLoop = this->loop() )
  {
    return false;
  }

  if( loop && ! oaSystem )
  {
    int inletport = inletPort();
    int outletport = outletPort();

    boost::optional<Node> loopInletNode;
    boost::optional<Node> loopOutletNode;

    if( loop->supplyComponent(node.handle()) )
    {
      loopOutletNode = loop->supplyOutletNode();
      loopInletNode = loop->supplyInletNode();
    }
    else if( loop->demandComponent(node.handle()) )
    {
      loopOutletNode = loop->demandOutletNode();
      loopInletNode = loop->demandInletNode();
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
    else if( node == loopInletNode.get() &&
             node.outletModelObject().get() == loopOutletNode.get() )
    {
      unsigned oldInletPort = node.connectedObjectPort( node.outletPort() ).get();
      unsigned oldOutletPort = node.outletPort();
      ModelObject oldTargetModelObject = node.connectedObject( node.outletPort() ).get();
      ModelObject oldSourceModelObject = node;

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
  else if( oaSystem )
  {
    if(oaSystem->oaComponent(node.handle()))
    {
      unsigned oldOutletPort = node.outletPort();
      unsigned oldInletPort = node.connectedObjectPort( node.outletPort() ).get();
      ModelObject oldSourceModelObject = node;
      ModelObject oldTargetModelObject = node.connectedObject( node.outletPort() ).get();
    
      Node newNode( _model );
      newNode.createName();

      _model.connect( oldSourceModelObject, oldOutletPort,
                      thisModelObject, inletPort() );
      _model.connect( thisModelObject, outletPort(),
                      newNode, newNode.inletPort() );
      _model.connect( newNode, newNode.outletPort(),
                      oldTargetModelObject, oldInletPort );
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

ModelObject StraightComponent_Impl::clone(Model model) const
{
  StraightComponent mo =  HVACComponent_Impl::clone( model ).cast<StraightComponent>();

  mo.setString(mo.inletPort(),""); 
  mo.setString(mo.outletPort(),""); 

  return mo;
}

} // detail

StraightComponent::StraightComponent(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::StraightComponent_Impl>());
}     

StraightComponent::StraightComponent(std::shared_ptr<detail::StraightComponent_Impl> p)
  : HVACComponent(p)
{}

unsigned StraightComponent::inletPort()
{
  return getImpl<detail::StraightComponent_Impl>()->inletPort();
}

unsigned StraightComponent::outletPort()
{
  return getImpl<detail::StraightComponent_Impl>()->outletPort();
}

OptionalModelObject StraightComponent::inletModelObject()
{
  return getImpl<detail::StraightComponent_Impl>()->inletModelObject();
}

OptionalModelObject StraightComponent::outletModelObject()
{
  return getImpl<detail::StraightComponent_Impl>()->outletModelObject();
}

std::vector<IdfObject> StraightComponent::remove()
{
  return getImpl<detail::StraightComponent_Impl>()->remove();
}

OptionalAirLoopHVAC StraightComponent::airLoopHVAC() const
{
  return getImpl<detail::StraightComponent_Impl>()->airLoopHVAC();
}

bool StraightComponent::addToNode(Node & node)
{
  return getImpl<detail::StraightComponent_Impl>()->addToNode( node );
}

ModelObject StraightComponent::clone(Model model) const
{
  return getImpl<detail::StraightComponent_Impl>()->clone( model );
}

void StraightComponent::disconnect()
{
  getImpl<detail::StraightComponent_Impl>()->disconnect();
}

} // model

} // openstudio

