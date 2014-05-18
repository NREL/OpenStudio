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

#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/AvailabilityManagerScheduled.hpp>
#include <model/AvailabilityManagerScheduled_Impl.hpp>
#include <model/AirToAirComponent.hpp>
#include <model/AirToAirComponent_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/ControllerOutdoorAir_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ModelExtensibleGroup.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idd/OS_AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {

namespace model {

namespace detail {

  AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACOutdoorAirSystem::iddObjectType());
  }

  AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACOutdoorAirSystem::iddObjectType());
  }

  AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(
      const AirLoopHVACOutdoorAirSystem_Impl& other, 
      Model_Impl* model, 
      bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {}

  // virtual destructor
  AirLoopHVACOutdoorAirSystem_Impl::~AirLoopHVACOutdoorAirSystem_Impl(){}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> AirLoopHVACOutdoorAirSystem_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::children() const
  {
    std::vector<ModelObject> result;
    result.push_back(this->getControllerOutdoorAir());
    return result;
  }

  // Get all output variable names that could be associated with this object. 
  const std::vector<std::string>& AirLoopHVACOutdoorAirSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACOutdoorAirSystem_Impl::iddObjectType() const {\
    return AirLoopHVACOutdoorAirSystem::iddObjectType();
  }

  unsigned AirLoopHVACOutdoorAirSystem_Impl::returnAirPort() const
  {
    return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReturnAirStreamNodeName;
  }

  unsigned AirLoopHVACOutdoorAirSystem_Impl::outdoorAirPort() const
  {
    return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirStreamNodeName;
  }

  unsigned AirLoopHVACOutdoorAirSystem_Impl::reliefAirPort() const
  {
    return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReliefAirStreamNodeName;
  }

  unsigned AirLoopHVACOutdoorAirSystem_Impl::mixedAirPort() const
  {
    return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::MixedAirNodeName;
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::returnAirModelObject() const
  {
    return this->connectedObject(this->returnAirPort());
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::outdoorAirModelObject() const
  {
    return this->connectedObject(this->outdoorAirPort());
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::reliefAirModelObject() const
  {
    return this->connectedObject(this->reliefAirPort());
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::mixedAirModelObject() const
  {
    return this->connectedObject(this->mixedAirPort());
  }

  ControllerOutdoorAir AirLoopHVACOutdoorAirSystem_Impl::getControllerOutdoorAir() const
  {
    return this->getTarget(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ControllerName)->cast<ControllerOutdoorAir>();
  }

  void AirLoopHVACOutdoorAirSystem_Impl::setControllerOutdoorAir(const ControllerOutdoorAir & controllerOutdoorAir )
  {
    this->setPointer(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ControllerName,controllerOutdoorAir.handle());
  }

  ModelObject AirLoopHVACOutdoorAirSystem_Impl::clone(Model model) const
  {
    AirLoopHVACOutdoorAirSystem airLoopHVACOutdoorAirSystem = ModelObject_Impl::clone(model).cast<AirLoopHVACOutdoorAirSystem>();

    // Clone OA controller
    ControllerOutdoorAir controllerOutdoorAir = getControllerOutdoorAir().clone(model).cast<ControllerOutdoorAir>();
    airLoopHVACOutdoorAirSystem.setControllerOutdoorAir(controllerOutdoorAir);

    //// Clone connected objects

    //std::vector<ModelObject> _oaComponents = oaComponents();
    //unsigned lastPort = outdoorAirPort();
    //ModelObject lastModelObject = airLoopHVACOutdoorAirSystem;
    //for( std::vector<ModelObject>::iterator it = _oaComponents.begin(); it < _oaComponents.end(); it++ )
    //{
    //  ModelObject modelObject = it->clone(model);
    //  if( OptionalStraightComponent comp = modelObject.optionalCast<StraightComponent>() )
    //  {
    //    model.connect( comp.get(), comp->outletPort(),
    //                   lastModelObject, lastPort );
    //    lastModelObject = modelObject;
    //    lastPort = comp->inletPort(); 
    //  }
    //}

    //std::vector<ModelObject> _reliefComponents = reliefComponents();
    //lastPort = reliefAirPort();
    //lastModelObject = airLoopHVACOutdoorAirSystem;
    //for( std::vector<ModelObject>::iterator it = _reliefComponents.begin(); it < _reliefComponents.end(); it++ )
    //{
    //  ModelObject modelObject = it->clone(model);
    //  if( OptionalStraightComponent comp = modelObject.optionalCast<StraightComponent>() )
    //  {
    //    model.connect( lastModelObject, lastPort,
    //                   comp.get(), comp->inletPort() );
    //    lastModelObject = modelObject;
    //    lastPort = comp->outletPort();
    //  }
    //}

    //return airLoopHVACOutdoorAirSystem;
    //return ModelObject_Impl::clone(model);

    Node oaNode(model);
    model.connect(oaNode,oaNode.outletPort(),airLoopHVACOutdoorAirSystem,airLoopHVACOutdoorAirSystem.outdoorAirPort());

    Node reliefNode(model);
    model.connect(airLoopHVACOutdoorAirSystem,airLoopHVACOutdoorAirSystem.reliefAirPort(),reliefNode,reliefNode.inletPort());

    return airLoopHVACOutdoorAirSystem;
  }

  std::vector<IdfObject> AirLoopHVACOutdoorAirSystem_Impl::remove()
  { 
    // Handle h = this->handle();

    ModelObjectVector modelObjects;
    ModelObjectVector::iterator it;
    modelObjects = this->components();
    for(it = modelObjects.begin();
        it != modelObjects.end();
        ++it)
    {
      if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
      {
        if( ! it->handle().isNull() )
        {
          comp->disconnect();
          comp->remove();
        }
      }
    }

    //reliefAirModelObject()->remove();
    //outdoorAirModelObject()->remove();
    model().disconnect(getObject<ModelObject>(),outdoorAirPort());
    model().disconnect(getObject<ModelObject>(),reliefAirPort());

    if( this->airLoopHVAC() )
    {
      OptionalNode targetModelObject;
      OptionalNode sourceModelObject;
      targetModelObject = this->mixedAirModelObject()->optionalCast<Node>(); 
      sourceModelObject = this->returnAirModelObject()->optionalCast<Node>();
      OptionalUnsigned targetPort = this->connectedObjectPort(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::MixedAirNodeName);
      OptionalUnsigned sourcePort = this->connectedObjectPort(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReturnAirStreamNodeName);
      
      OptionalModelObject target2ModelObject = targetModelObject->outletModelObject();
      OptionalModelObject source2ModelObject = sourceModelObject->inletModelObject();
      OptionalUnsigned target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
      OptionalUnsigned source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());
      
      OptionalAirLoopHVAC airLoop = this->airLoop();
      OptionalNode supplyInletNode = airLoop->supplyInletNode();
      OptionalNode supplyOutletNode = OptionalNode(airLoop->supplyOutletNodes().front());
      
      model().disconnect(getObject<ModelObject>(),returnAirPort());
      model().disconnect(getObject<ModelObject>(),mixedAirPort());

      // If the component is stuck directly between the inlet and outlet node.
      if( supplyInletNode->handle() == sourceModelObject->handle() 
          &&
          supplyOutletNode->handle() == targetModelObject->handle() )
      {
        this->model().connect( supplyInletNode.get(), supplyInletNode->outletPort(), supplyOutletNode.get(), supplyOutletNode->inletPort() );
      }
      // If the component is at the very end of the supply path, but there is another component
      // preceding this one.
      else if( supplyOutletNode->handle() == targetModelObject->handle() )
      {
        sourceModelObject->remove();
        this->model().connect( source2ModelObject.get(), source2Port.get(), supplyOutletNode.get(), supplyOutletNode->inletPort() );
      }
      // else remove the component and the outlet node
      else
      {
        targetModelObject->remove();
        this->model().connect(sourceModelObject.get(), sourcePort.get(), target2ModelObject.get(), target2Port.get());
      }
    }

    getControllerOutdoorAir().remove();
    
    return ModelObject_Impl::remove();
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::oaComponents() const
  {
    std::vector<ModelObject> modelObjects;
    
    OptionalModelObject modelObject;

    modelObject = this->outdoorAirModelObject();
    
    while(modelObject)
    {
      if(OptionalStraightComponent comp = modelObject->optionalCast<StraightComponent>())
      {
        modelObjects.insert(modelObjects.begin(),*comp);
        modelObject = comp->inletModelObject();
      }
      else if( boost::optional<AirToAirComponent> comp = modelObject->optionalCast<AirToAirComponent>() )
      {
        modelObjects.insert(modelObjects.begin(),*comp);
        modelObject = comp->primaryAirInletModelObject();
      }
      else
      {
        break;
        // log unhandled component
      }
    }
    return modelObjects;
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::reliefComponents() const
  {
    std::vector<ModelObject> modelObjects;
    
    OptionalModelObject modelObject;

    modelObject = this->reliefAirModelObject();
    
    while(modelObject)
    {
      OptionalNode node = modelObject->optionalCast<Node>();
      OptionalStraightComponent comp = modelObject->optionalCast<StraightComponent>();
      if(node)
      {
        modelObjects.push_back(*node);
        modelObject = node->outletModelObject();
      }
      else if(comp)
      {
        modelObjects.push_back(*comp);
        modelObject = comp->outletModelObject();
      }
      else if( boost::optional<AirToAirComponent> comp = modelObject->optionalCast<AirToAirComponent>() )
      {
        modelObjects.push_back(*comp);
        modelObject = comp->secondaryAirOutletModelObject();
      }
      else
      {
        // log unhandled component
      }
    }
    return modelObjects;
  }
  boost::optional<Node> AirLoopHVACOutdoorAirSystem_Impl::outboardOANode() const
  {
    OptionalNode result;

    ModelObjectVector modelObjects;
    modelObjects = this->oaComponents();

    if( modelObjects.size() > 0 )
    {
      ModelObject modelObject = modelObjects.front();
      result = modelObject.optionalCast<Node>();
    }

    return result;
  }
  boost::optional<Node> AirLoopHVACOutdoorAirSystem_Impl::outboardReliefNode() const
  {
    OptionalNode result;

    ModelObjectVector modelObjects;
    modelObjects = this->reliefComponents();

    if( modelObjects.size() > 0 )
    {
      ModelObject modelObject = modelObjects.back();
      result = modelObject.optionalCast<Node>();
    }

    return result;
  }

  OptionalAirLoopHVAC AirLoopHVACOutdoorAirSystem_Impl::airLoop() const
  {
    OptionalAirLoopHVAC result;

    AirLoopHVACVector airLoops = this->model().getConcreteModelObjects<AirLoopHVAC>(); 
    AirLoopHVACVector::iterator it;
    
    for( it = airLoops.begin();
         it != airLoops.end();
         ++it )
    {
      OptionalAirLoopHVAC airLoop = it->optionalCast<AirLoopHVAC>();
      if( airLoop )
      {
        if( airLoop->component(this->handle()) )
        {
          return airLoop;
        }
      }
    }

    return result;
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::components() const
  {
    std::vector<ModelObject> result;
    result = this->oaComponents();
    
    std::vector<ModelObject> reliefComponents = this->reliefComponents();
    
    result.insert(result.end(),reliefComponents.begin(),reliefComponents.end());
    
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::component(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = components();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         ++it )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::oaComponent(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = oaComponents();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         ++it )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::reliefComponent(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = reliefComponents();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         ++it )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
  }

  bool AirLoopHVACOutdoorAirSystem_Impl::addToNode(Node & node)
  {
    Model _model = node.model(); 
    ModelObject thisModelObject = getObject<ModelObject>();

    if( OptionalAirLoopHVAC optionalAirLoop = node.airLoopHVAC() )
    {
      AirLoopHVAC airLoop = optionalAirLoop.get();

      if(airLoop.supplyComponents(this->iddObjectType()).size() > 0)
      {
        return false;
      }

      if( airLoop.supplyComponent( node.handle() ) )
      {
        if( node == airLoop.supplyOutletNodes().front() &&
            node.inletModelObject().get() == airLoop.supplyInletNode() )
        {
          unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
          unsigned oldInletPort = node.inletPort();
          ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
          ModelObject oldTargetModelObject = node;

          _model.connect( oldSourceModelObject, oldOutletPort,
                          thisModelObject, returnAirPort() );
          _model.connect( thisModelObject, mixedAirPort(),
                          oldTargetModelObject, oldInletPort );
          return true;
        }
        if( node == airLoop.supplyInletNode() &&
            node.outletModelObject().get() == airLoop.supplyOutletNodes().front() )
        {
          unsigned oldOutletPort = node.outletPort();
          unsigned oldInletPort = node.connectedObjectPort( node.outletPort() ).get();
          ModelObject oldSourceModelObject = node;
          ModelObject oldTargetModelObject = node.outletModelObject().get();

          _model.connect( oldSourceModelObject, oldOutletPort,
                          thisModelObject, returnAirPort() );
          _model.connect( thisModelObject, mixedAirPort(),
                          oldTargetModelObject, oldInletPort );
          return true;
        }
        else if( node == airLoop.supplyOutletNodes().front() )
        {
          unsigned oldOutletPort = node.connectedObjectPort( node.inletPort() ).get();
          unsigned oldInletPort = node.inletPort();
          ModelObject oldSourceModelObject = node.connectedObject( node.inletPort() ).get();
          ModelObject oldTargetModelObject = node;

          Node newNode( _model );
          _model.connect( oldSourceModelObject, oldOutletPort,
                          newNode, newNode.inletPort() );
          _model.connect( newNode, newNode.outletPort(),
                          thisModelObject, returnAirPort() );                        
          _model.connect( thisModelObject, mixedAirPort(),
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
                          thisModelObject, returnAirPort() );
          _model.connect( thisModelObject, mixedAirPort(),
                          newNode, newNode.inletPort() );
          _model.connect( newNode, newNode.outletPort(),
                          oldTargetModelObject, oldInletPort );
          return true;
        }
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

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::controllerOutdoorAirAsModelObject() const {
    OptionalModelObject result = getControllerOutdoorAir();
    return result;
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::oaComponentsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(oaComponents());
    return result;
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::reliefComponentsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(reliefComponents());
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::outboardOANodeAsModelObject() const {
    OptionalModelObject result;
    OptionalNode intermediate = outboardOANode();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::outboardReliefNodeAsModelObject() const {
    OptionalModelObject result;
    OptionalNode intermediate = outboardReliefNode();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::airLoopAsModelObject() const {
    OptionalModelObject result;
    OptionalAirLoopHVAC intermediate = airLoop();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::componentsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(components());
    return result;
  }

  bool AirLoopHVACOutdoorAirSystem_Impl::setControllerOutdoorAirAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalControllerOutdoorAir intermediate = modelObject->optionalCast<ControllerOutdoorAir>();
      if (intermediate) {
        setControllerOutdoorAir(*intermediate);
        return true;
      }
    }
    return false;
  }

} // detail

// create a new AirLoopHVACOutdoorAirSystem object in the model's workspace
AirLoopHVACOutdoorAirSystem::AirLoopHVACOutdoorAirSystem(Model & model, const ControllerOutdoorAir & controller)
  : HVACComponent(AirLoopHVACOutdoorAirSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>());

  AirLoopHVACOutdoorAirSystem outdoorAirSystem =
    AirLoopHVACOutdoorAirSystem(getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>());

  // Children.

  setControllerOutdoorAir(controller); 

  Node oaNode(model);
  model.connect(oaNode,oaNode.outletPort(),outdoorAirSystem,outdoorAirPort());

  Node reliefNode(model);
  model.connect(outdoorAirSystem,reliefAirPort(),reliefNode,reliefNode.inletPort());
}

AirLoopHVACOutdoorAirSystem::AirLoopHVACOutdoorAirSystem(
    boost::shared_ptr<detail::AirLoopHVACOutdoorAirSystem_Impl> impl)
  : HVACComponent(impl)
{
}

unsigned AirLoopHVACOutdoorAirSystem::returnAirPort() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->returnAirPort();
}

unsigned AirLoopHVACOutdoorAirSystem::outdoorAirPort() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outdoorAirPort();
}

unsigned AirLoopHVACOutdoorAirSystem::reliefAirPort() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefAirPort();
}

unsigned AirLoopHVACOutdoorAirSystem::mixedAirPort() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->mixedAirPort();
}

OptionalModelObject AirLoopHVACOutdoorAirSystem::returnAirModelObject()
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->returnAirModelObject();
}

OptionalModelObject AirLoopHVACOutdoorAirSystem::outdoorAirModelObject()
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outdoorAirModelObject();
}

OptionalModelObject AirLoopHVACOutdoorAirSystem::reliefAirModelObject()
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefAirModelObject();
}

OptionalModelObject AirLoopHVACOutdoorAirSystem::mixedAirModelObject()
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->mixedAirModelObject();
}

ControllerOutdoorAir AirLoopHVACOutdoorAirSystem::getControllerOutdoorAir() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->getControllerOutdoorAir();
}

void AirLoopHVACOutdoorAirSystem::setControllerOutdoorAir(const ControllerOutdoorAir & controllerOutdoorAir )
{
  getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->setControllerOutdoorAir(controllerOutdoorAir);
}

ModelObject AirLoopHVACOutdoorAirSystem::clone(Model model) const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->clone(model);
}

std::vector<IdfObject> AirLoopHVACOutdoorAirSystem::remove()
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->remove();
}

boost::optional<Node> AirLoopHVACOutdoorAirSystem::outboardOANode() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outboardOANode();
}

boost::optional<Node> AirLoopHVACOutdoorAirSystem::outboardReliefNode() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outboardReliefNode();
}

std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::oaComponents() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->oaComponents();
}

std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::reliefComponents() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefComponents();
}

std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::components() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->components();
}

boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::component(openstudio::Handle handle)
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->component(handle);
}

boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::oaComponent(openstudio::Handle handle)
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->oaComponent(handle);
}

boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::reliefComponent(openstudio::Handle handle)
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefComponent(handle);
}

boost::optional<AirLoopHVAC> AirLoopHVACOutdoorAirSystem::airLoop() const
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->airLoop();
}

bool AirLoopHVACOutdoorAirSystem::addToNode(Node & node)
{
  return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->addToNode( node );
}

IddObjectType AirLoopHVACOutdoorAirSystem::iddObjectType() {
  IddObjectType result(IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem);
  return result;
}

} // model
} // openstudio

