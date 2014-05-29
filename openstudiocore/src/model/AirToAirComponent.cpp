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

#include "AirToAirComponent.hpp"
#include "AirToAirComponent_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

AirToAirComponent_Impl::AirToAirComponent_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

AirToAirComponent_Impl::AirToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{ 
}

AirToAirComponent_Impl::AirToAirComponent_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

AirToAirComponent_Impl::AirToAirComponent_Impl(const AirToAirComponent_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

OptionalModelObject AirToAirComponent_Impl::primaryAirInletModelObject()
{
  return connectedObject(primaryAirInletPort());
}

OptionalModelObject AirToAirComponent_Impl::primaryAirOutletModelObject()
{
  return connectedObject(primaryAirOutletPort());
}

OptionalModelObject AirToAirComponent_Impl::secondaryAirInletModelObject()
{
  return connectedObject(secondaryAirInletPort());
}

OptionalModelObject AirToAirComponent_Impl::secondaryAirOutletModelObject()
{
  return connectedObject(secondaryAirOutletPort());
}

std::vector<openstudio::IdfObject> AirToAirComponent_Impl::remove()
{
  Model _model = model();

  boost::optional<Node> oaOutletNode;
  boost::optional<ModelObject> oaOutletModelObject;
  boost::optional<unsigned> oaOutletModelObjectPort;

  boost::optional<Node> oaInletNode;

  boost::optional<Node> reliefInletNode;
  boost::optional<ModelObject> reliefInletModelObject;
  boost::optional<unsigned> reliefInletModelObjectPort;

  boost::optional<Node> reliefOutletNode;

  if( primaryAirOutletModelObject() && primaryAirInletModelObject() )
  {
    boost::optional<ModelObject> mo = primaryAirOutletModelObject();

    boost::optional<Node> node = mo->optionalCast<Node>();

    OS_ASSERT(node);

    oaOutletNode = node;

    boost::optional<ModelObject> mo2 = oaOutletNode->outletModelObject();

    OS_ASSERT(mo2);

    oaOutletModelObject = mo2;

    oaOutletModelObjectPort = oaOutletNode->connectedObjectPort(oaOutletNode->outletPort());

    OS_ASSERT(oaOutletModelObjectPort);

    mo2 = primaryAirInletModelObject();

    OS_ASSERT(mo2);

    node = mo2->optionalCast<Node>();

    OS_ASSERT(node);

    oaInletNode = node;
  }

  if( secondaryAirInletModelObject() && secondaryAirOutletModelObject() )
  {
    boost::optional<ModelObject> mo = secondaryAirInletModelObject();

    boost::optional<Node> node = mo->optionalCast<Node>();

    OS_ASSERT(node);

    reliefInletNode = node;

    boost::optional<ModelObject> mo2 = reliefInletNode->inletModelObject();

    OS_ASSERT(mo2);

    reliefInletModelObject = mo2;

    reliefInletModelObjectPort = reliefInletNode->connectedObjectPort(reliefInletNode->inletPort());

    OS_ASSERT(reliefInletModelObjectPort);

    mo2 = secondaryAirOutletModelObject();

    OS_ASSERT(mo2);

    node = mo2->optionalCast<Node>();

    OS_ASSERT(node);

    reliefOutletNode = node;
  }

  if( oaOutletModelObject && oaInletNode && oaOutletNode )
  {
    _model.connect(oaInletNode.get(),oaInletNode->outletPort(),oaOutletModelObject.get(),oaOutletModelObjectPort.get());

    oaOutletNode->remove();
  }

  if( reliefInletModelObject && reliefOutletNode && reliefInletNode )
  {
    _model.connect(reliefInletModelObject.get(),reliefInletModelObjectPort.get(),reliefOutletNode.get(),reliefOutletNode->inletPort());

    reliefInletNode->remove();
  }

  return HVACComponent_Impl::remove();
}

bool AirToAirComponent_Impl::addToNode(Node & node)
{
  Model _model = node.model(); 
  boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = node.airLoopHVACOutdoorAirSystem();
  
  if( oaSystem )
  {
    // The OA node that we will connect to
    boost::optional<Node> oaNode;

    // The node on the relief side that we should connect to
    boost::optional<Node> reliefNode;

    Node outboardOANode = oaSystem->outboardOANode().get();
    Node outboardReliefNode = oaSystem->outboardReliefNode().get();
    HVACComponent thisObject = getObject<HVACComponent>();

    // next / prev components are going to be assigned either the next downstream AirToAirComponent or the end of the air stream
    // The end of the air stream is either the OA systems itself or the outboard relief / supply nodes.
    // Downstream is from the perspective of the particular side of the OA System the component is on.
    // Downstream on the supply side is toward the OA Mixer itself.
    // Downstream on the relief side is toward the most outboard relief node
    boost::optional<HVACComponent> nextSupplyComponent;
    boost::optional<HVACComponent> prevSupplyComponent;

    boost::optional<HVACComponent> nextReliefComponent;
    boost::optional<HVACComponent> prevReliefComponent;

    std::vector<ModelObject> oaComponents = oaSystem->oaComponents();
    std::vector<ModelObject> reliefComponents = oaSystem->reliefComponents();

    if( oaSystem->oaComponent(node.handle()) )
    {
      oaNode = node;

      // n will be the number of HVACComponent objects from the drop node to "nextSupplyComponent"
      int n = -1;

      // bigNPrime will be the number of HVACComponent objects from prevReliefComponent to nextReliefComponent
      int bigNPrime = -1;

      // Find nextSupplyComponent

      auto dropNodeLocation = std::find(oaComponents.begin(),oaComponents.end(),node);

      OS_ASSERT( dropNodeLocation != oaComponents.end() );

      for( auto it = dropNodeLocation;
           it != oaComponents.end();
           ++it )
      {
        n++;

        if( (nextSupplyComponent = it->optionalCast<AirToAirComponent>()) )
        {
          break;
        }
      }

      if( ! nextSupplyComponent )
      {
        nextSupplyComponent = oaSystem;
      }

      // Find prevSupplyComponent

      auto rDropNodeLocation = 
        std::find(oaComponents.rbegin(),oaComponents.rend(),node);

      OS_ASSERT( rDropNodeLocation != oaComponents.rend() );

      for( auto it = rDropNodeLocation;
           it != oaComponents.rend();
           ++it )
      {
        if( (prevSupplyComponent = it->optionalCast<AirToAirComponent>()) )
        {
          break;
        }
      }

      if( ! prevSupplyComponent )
      {
        prevSupplyComponent = outboardOANode;
      }

      // Find prevReliefComponent
      
      prevReliefComponent = nextSupplyComponent;

      // Find nextReliefComponent

      OS_ASSERT(prevReliefComponent);

      std::vector<ModelObject>::iterator prevReliefComponentLocation;

      if( prevReliefComponent.get() != oaSystem.get() )
      {
        prevReliefComponentLocation = std::find(reliefComponents.begin(),reliefComponents.end(),prevReliefComponent.get());
      }
      else
      {
        prevReliefComponentLocation = reliefComponents.begin();
      }

      for( auto it = prevReliefComponentLocation;
           it != reliefComponents.end();
           ++it )
      {
        bigNPrime++;

        if( (nextReliefComponent = it->optionalCast<AirToAirComponent>()) )
        {
          if( *it != prevReliefComponent.get() )
          {
            break;
          }
        }
      }

      if( ! nextReliefComponent )
      {
        nextReliefComponent = outboardReliefNode;
      }

      // Find the relief node

      if( n < bigNPrime )
      {
        boost::optional<ModelObject> mo = *(prevReliefComponentLocation + n);

        OS_ASSERT(mo);

        reliefNode = mo->optionalCast<Node>();

        OS_ASSERT(reliefNode);
      }
      else
      {
        if( nextReliefComponent.get() == outboardReliefNode )
        {
          reliefNode = outboardReliefNode;
        }
        else
        {
          boost::optional<AirToAirComponent> comp = nextReliefComponent->optionalCast<AirToAirComponent>();

          OS_ASSERT(comp);

          boost::optional<ModelObject> comp2 = comp->secondaryAirInletModelObject();

          reliefNode = comp2->optionalCast<Node>();

          OS_ASSERT(reliefNode);
        }
      }

      OS_ASSERT(reliefNode);
      OS_ASSERT(oaNode);
    }
    else if( oaSystem->reliefComponent(node.handle()) )
    {
      reliefNode = node;

      // n will be the number of HVACComponent objects from the drop node to "nextReliefComponent"
      int n = -1;

      // bigNPrime will be the number of HVACComponent objects from prevSupplyComponent to nextSupplyComponent
      int bigNPrime = -1;

      // Find nextReliefComponent

      auto dropNodeLocation = std::find(reliefComponents.begin(),reliefComponents.end(),node);

      OS_ASSERT( dropNodeLocation != reliefComponents.end() );

      for( auto it = dropNodeLocation;
           it != reliefComponents.end();
           ++it )
      {
        n++;

        if( (nextReliefComponent = it->optionalCast<AirToAirComponent>()) )
        {
          break;
        }
      }

      if( ! nextReliefComponent )
      {
        nextReliefComponent = outboardReliefNode;
      }

      // Find prevReliefComponent

      auto rDropNodeLocation = 
        std::find(reliefComponents.rbegin(),reliefComponents.rend(),node);

      OS_ASSERT( rDropNodeLocation != reliefComponents.rend() );

      for( auto it = rDropNodeLocation;
           it != reliefComponents.rend();
           ++it )
      {
        if( (prevReliefComponent = it->optionalCast<AirToAirComponent>()) )
        {
          break;
        }
      }

      if( ! prevReliefComponent )
      {
        prevReliefComponent = oaSystem;
      }

      // Find prevSupplyComponent
      
      if( nextReliefComponent.get() != outboardReliefNode )
      {
        prevSupplyComponent = nextReliefComponent;
      }
      else
      {
        prevSupplyComponent = outboardOANode;
      }

      // Find nextSupplyComponent

      OS_ASSERT(prevSupplyComponent);

      std::vector<ModelObject>::iterator prevSupplyComponentLocation;

      prevSupplyComponentLocation = std::find(oaComponents.begin(),oaComponents.end(),prevSupplyComponent.get());

      for( auto it = prevSupplyComponentLocation;
           it != oaComponents.end();
           ++it )
      {
        bigNPrime++;

        if( (nextSupplyComponent = it->optionalCast<AirToAirComponent>()) )
        {
          break;
        }
      }

      if( ! nextSupplyComponent )
      {
        nextSupplyComponent = oaSystem;
      }

      // Find the oaNode

      if( n < bigNPrime )
      {
        boost::optional<ModelObject> mo = *(prevSupplyComponentLocation + n);

        OS_ASSERT(mo);

        oaNode = mo->optionalCast<Node>();

        OS_ASSERT(oaNode);
      }
      else
      {
        if( nextSupplyComponent.get() == oaSystem.get() )
        {
          oaNode = oaSystem->outdoorAirModelObject()->cast<Node>();
        }
        else
        {
          boost::optional<AirToAirComponent> comp = nextSupplyComponent->optionalCast<AirToAirComponent>();

          OS_ASSERT(comp);

          boost::optional<ModelObject> comp2 = comp->primaryAirInletModelObject();

          oaNode = comp2->optionalCast<Node>();

          OS_ASSERT(oaNode);
        }
      }

      OS_ASSERT(reliefNode);
      OS_ASSERT(oaNode);
    }

    OS_ASSERT(reliefNode);
    OS_ASSERT(oaNode);

    // Make new connections on the oa side

    ModelObject oldOutletModelObject = oaNode->outletModelObject().get();
    unsigned oldInletPort = oaNode->connectedObjectPort(oaNode->outletPort()).get();

    Node newNode(_model);
    newNode.createName();

    _model.connect(oaNode.get(),oaNode->outletPort(),thisObject,primaryAirInletPort());

    _model.connect(thisObject,primaryAirOutletPort(),newNode,newNode.inletPort());

    _model.connect(newNode,newNode.outletPort(),oldOutletModelObject,oldInletPort);

    // Make new connections on the relief side

    Node newNode2(_model);
    newNode2.createName();

    if( reliefNode.get() == outboardReliefNode )
    {
      ModelObject oldMO = outboardReliefNode.inletModelObject().get();
      unsigned oldPort = outboardReliefNode.connectedObjectPort(outboardReliefNode.inletPort()).get();

      _model.connect(oldMO,oldPort,newNode2,newNode2.inletPort());

      _model.connect(newNode2,newNode2.outletPort(),thisObject,secondaryAirInletPort());

      _model.connect(thisObject,secondaryAirOutletPort(),outboardReliefNode,outboardReliefNode.inletPort());
    }
    else
    {
      ModelObject oldReliefOutletModelObject = reliefNode->outletModelObject().get();
      unsigned oldReliefInletPort = reliefNode->connectedObjectPort(reliefNode->outletPort()).get();

      _model.connect(reliefNode.get(),reliefNode->outletPort(),thisObject,secondaryAirInletPort());

      _model.connect(thisObject,secondaryAirOutletPort(),newNode2,newNode2.inletPort());

      _model.connect(newNode2,newNode2.outletPort(),oldReliefOutletModelObject,oldReliefInletPort);
    }

    return true;
  }

  return false;
}

ModelObject AirToAirComponent_Impl::clone(Model model) const
{
  AirToAirComponent mo =  HVACComponent_Impl::clone( model ).cast<AirToAirComponent>();

  mo.setString(mo.primaryAirInletPort(),""); 
  mo.setString(mo.primaryAirOutletPort(),""); 
  mo.setString(mo.secondaryAirInletPort(),""); 
  mo.setString(mo.secondaryAirOutletPort(),""); 

  return mo;
}

} // detail

AirToAirComponent::AirToAirComponent(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::AirToAirComponent_Impl>());
}     

AirToAirComponent::AirToAirComponent(std::shared_ptr<detail::AirToAirComponent_Impl> p)
  : HVACComponent(p)
{}

unsigned AirToAirComponent::primaryAirInletPort()
{
  return getImpl<detail::AirToAirComponent_Impl>()->primaryAirInletPort();
}

unsigned AirToAirComponent::primaryAirOutletPort()
{
  return getImpl<detail::AirToAirComponent_Impl>()->primaryAirOutletPort();
}

unsigned AirToAirComponent::secondaryAirInletPort()
{
  return getImpl<detail::AirToAirComponent_Impl>()->secondaryAirInletPort();
}

unsigned AirToAirComponent::secondaryAirOutletPort()
{
  return getImpl<detail::AirToAirComponent_Impl>()->secondaryAirOutletPort();
}

OptionalModelObject AirToAirComponent::primaryAirInletModelObject()
{
  return getImpl<detail::AirToAirComponent_Impl>()->primaryAirInletModelObject();
}

OptionalModelObject AirToAirComponent::primaryAirOutletModelObject()
{
  return getImpl<detail::AirToAirComponent_Impl>()->primaryAirOutletModelObject();
}

OptionalModelObject AirToAirComponent::secondaryAirInletModelObject()
{
  return getImpl<detail::AirToAirComponent_Impl>()->secondaryAirInletModelObject();
}

OptionalModelObject AirToAirComponent::secondaryAirOutletModelObject()
{
  return getImpl<detail::AirToAirComponent_Impl>()->secondaryAirOutletModelObject();
}

std::vector<IdfObject> AirToAirComponent::remove()
{
  return getImpl<detail::AirToAirComponent_Impl>()->remove();
}

bool AirToAirComponent::addToNode(Node & node)
{
  return getImpl<detail::AirToAirComponent_Impl>()->addToNode( node );
}

ModelObject AirToAirComponent::clone(Model model) const
{
  return getImpl<detail::AirToAirComponent_Impl>()->clone( model );
}

} // model

} // openstudio

