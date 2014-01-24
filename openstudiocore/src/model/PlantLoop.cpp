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

#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/SizingPlant.hpp>
#include <model/SizingPlant_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/WaterToWaterComponent.hpp>
#include <model/WaterToWaterComponent_Impl.hpp>
#include <model/CoilCoolingWater.hpp>
#include <model/CoilCoolingWater_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <utilities/idd/OS_PlantLoop_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail {

PlantLoop_Impl::PlantLoop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : Loop_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == PlantLoop::iddObjectType());
}

PlantLoop_Impl::PlantLoop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
  : Loop_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == PlantLoop::iddObjectType());
}

PlantLoop_Impl::PlantLoop_Impl(const PlantLoop_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
  : Loop_Impl(other,model,keepHandle)
{
}

std::vector<ModelObject> PlantLoop_Impl::demandComponents( HVACComponent inletComp,
                                                           HVACComponent outletComp,
                                                           openstudio::IddObjectType type )
{
  return Loop_Impl::demandComponents(inletComp,outletComp,type);
}

std::vector<openstudio::IdfObject> PlantLoop_Impl::remove()
{
  sizingPlant().remove();

  ModelObjectVector modelObjects;
  ModelObjectVector::iterator it;
  modelObjects = this->components();

  for(it = modelObjects.begin();
      it != modelObjects.end();
      ++it)
  {
    if( boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>() )
    {
      comp->disconnectWaterSide();
    }
    else if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
    {
      comp->disconnect();
    }
  }

  std::vector<openstudio::IdfObject> idfObjects =  ModelObject_Impl::remove();

  for(it = modelObjects.begin();
      it != modelObjects.end();
      ++it)
  {
    if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
    {
      if( ! it->optionalCast<WaterToAirComponent>() )
      {
        it->cast<HVACComponent>().remove();
      }
    }
  }

  return idfObjects;
}

IddObjectType PlantLoop_Impl::iddObjectType() const {
  return PlantLoop::iddObjectType();
}

std::vector<ModelObject> PlantLoop_Impl::demandComponents(openstudio::IddObjectType type)
{
  return demandComponents( demandInletNode(), demandOutletNode(), type );
}

OptionalModelObject PlantLoop_Impl::component(openstudio::Handle handle)
{
  return Loop_Impl::component( handle );
}

ModelObject PlantLoop_Impl::clone(Model model) const
{
  return Loop_Impl::clone(model);
}

std::vector<ModelObject> PlantLoop_Impl::demandComponents(
    std::vector<HVACComponent> inletComps,
    std::vector<HVACComponent> outletComps,
    openstudio::IddObjectType type
  )
{
  return Loop_Impl::demandComponents(inletComps, outletComps, type);
}

unsigned PlantLoop_Impl::supplyInletPort() const
{
  return openstudio::OS_PlantLoopFields::PlantSideInletNodeName;
}

unsigned PlantLoop_Impl::supplyOutletPort() const
{
  return openstudio::OS_PlantLoopFields::PlantSideOutletNodeName;
}

unsigned PlantLoop_Impl::demandInletPort() const
{
  return openstudio::OS_PlantLoopFields::DemandSideInletNodeName;
}

unsigned PlantLoop_Impl::demandOutletPort() const
{
  return openstudio::OS_PlantLoopFields::DemandSideOutletNodeName;
}

Node PlantLoop_Impl::supplyInletNode() const
{
  return connectedObject(supplyInletPort())->optionalCast<Node>().get();
}

Node PlantLoop_Impl::supplyOutletNode() const
{
  return connectedObject(supplyOutletPort())->optionalCast<Node>().get();
}

Node PlantLoop_Impl::demandInletNode() const
{
  return connectedObject(demandInletPort())->optionalCast<Node>().get();
}

Node PlantLoop_Impl::demandOutletNode() const
{
  return connectedObject(demandOutletPort())->optionalCast<Node>().get();
}

bool PlantLoop_Impl::addSupplyBranchForComponent( HVACComponent component )
{
  Model _model = this->model();

  if( component.model() != _model )
  {
    return false;
  }

  Splitter splitter = this->supplySplitter();
  Mixer mixer = this->supplyMixer();

  if( splitter.outletModelObjects().size() == 1u )
  {
    if( boost::optional<ModelObject> mo = splitter.lastOutletModelObject() )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        if ( (node->outletModelObject().get() == mixer) &&       
              (node->inletModelObject().get() == splitter) )       
        {
          if( component.addToNode(node.get()) )
          {
            return true;
          }
        }
      }
    }
  }

  int nextOutletPort;
  int nextInletPort;

  nextOutletPort = splitter.nextOutletPort();
  nextInletPort = mixer.nextInletPort();

  Node node(_model);

  _model.connect(splitter,nextOutletPort,node,node.inletPort());
  _model.connect(node,node.outletPort(),mixer,nextInletPort);

  if( component.addToNode(node) )
  {
    return true;
  }
  else
  {
    removeSupplyBranchWithComponent(node);
  
    return false;
  }

  return false;
}

bool PlantLoop_Impl::removeSupplyBranchWithComponent( HVACComponent component )
{
  if( ! supplyComponent(component.handle()) )
  {
    return false;
  }
  
  return removeBranchWithComponent(component,supplySplitter(),supplyMixer(),true);
}

bool PlantLoop_Impl::addDemandBranchForComponent( HVACComponent component )
{
  Model _model = this->model();

  if( component.model() != _model )
  {
    return false;
  }

  Splitter splitter = this->demandSplitter();
  Mixer mixer = this->demandMixer();

  if( splitter.outletModelObjects().size() == 1u )
  {
    if( boost::optional<ModelObject> mo = splitter.lastOutletModelObject() )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        if ( (node->outletModelObject().get() == mixer) &&       
              (node->inletModelObject().get() == splitter) )       
        {
          return component.addToNode(node.get());
        }
      }
    }
  }

  int nextOutletPort;
  int nextInletPort;

  nextOutletPort = splitter.nextOutletPort();
  nextInletPort = mixer.nextInletPort();

  Node node(_model);

  _model.connect(splitter,nextOutletPort,node,node.inletPort());
  _model.connect(node,node.outletPort(),mixer,nextInletPort);

  if( component.addToNode(node) )
  {
    return true;
  }
  else
  {
    _model.disconnect(node,node.outletPort());
    _model.disconnect(node,node.inletPort());
    node.remove();
  
    return false;
  }

  return false;
}

bool PlantLoop_Impl::removeBranchWithComponent( HVACComponent component, Splitter splitter, Mixer mixer, bool isSupplyComponent )
{
  Model _model = model();

  std::vector<ModelObject> backwardComponents = components(splitter,component);
  std::vector<ModelObject> forwardComponents = components(component,mixer);

  // Validate that the component is between the demand splitter and mixer
  if( backwardComponents.front().handle() != splitter.handle() )
  {
    return false;
  }

  if( backwardComponents.back().handle() != component.handle() )
  {
    return false;
  }

  if( forwardComponents.front().handle() != component.handle() )
  {
    return false;
  }

  if( forwardComponents.back().handle() != mixer.handle() )
  {
    return false;
  }

  // Remove the backwardComponents end because we don't want component twice
  backwardComponents.erase(backwardComponents.end() - 1);

  // Combine forwardComponents and backwardComponents for a complete vector of all components on the branch
  std::vector<ModelObject> allComponents(backwardComponents.begin(),backwardComponents.end());
  allComponents.insert(allComponents.end(),forwardComponents.begin(),forwardComponents.end());

  // Remove the splitter and mixer from the vector
  allComponents.erase(allComponents.begin());
  allComponents.erase(allComponents.end() - 1);

  for( std::vector<ModelObject>::iterator it = allComponents.begin();
       it < allComponents.end();
       ++it )
  {
    if( ! it->optionalCast<Node>() )
    {
      if( boost::optional<StraightComponent> comp = it->optionalCast<StraightComponent>() )
      {
        it->remove();
      }
      else if( boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>() )
      {
        comp->removeFromPlantLoop();

        if( ! comp->airLoopHVAC() )
        {
          comp->remove();
        }
      }
      else if( boost::optional<WaterToWaterComponent> comp = it->optionalCast<WaterToWaterComponent>() )
      {
        if( isSupplyComponent )
        {
          comp->removeFromPlantLoop();
        }
        else
        {
          comp->removeFromSecondaryPlantLoop();
        }

        if( (! comp->plantLoop()) && (! comp->secondaryPlantLoop()) )
        {
          comp->remove();
        }
      }
    }
  }

  return true;
}

bool PlantLoop_Impl::removeDemandBranchWithComponent( HVACComponent component )
{
  if( ! demandComponent(component.handle()) )
  {
    return false;
  }
  
  return removeBranchWithComponent(component,demandSplitter(),demandMixer(),false);
}

bool PlantLoop_Impl::isDemandBranchEmpty()
{
  Splitter splitter = this->demandSplitter();

  if( splitter.lastOutletModelObject()->optionalCast<Mixer>() )
  {
    return true;
  }
  else
  {
    return false;
  }
}

Mixer PlantLoop_Impl::supplyMixer()
{
  return supplyComponents( IddObjectType::OS_Connector_Mixer ).front().cast<Mixer>();
}

Splitter PlantLoop_Impl::supplySplitter()
{
  return supplyComponents( IddObjectType::OS_Connector_Splitter ).front().cast<Splitter>();
}

Mixer PlantLoop_Impl::demandMixer()
{
  return demandComponents( IddObjectType::OS_Connector_Mixer ).front().cast<ConnectorMixer>();
}

Splitter PlantLoop_Impl::demandSplitter()
{
  return demandComponents( IddObjectType::OS_Connector_Splitter ).front().cast<ConnectorSplitter>();
}

double PlantLoop_Impl::maximumLoopTemperature()
{
  return getDouble(OS_PlantLoopFields::MaximumLoopTemperature,true).get();
}

void PlantLoop_Impl::setMaximumLoopTemperature( double value )
{
  setDouble(OS_PlantLoopFields::MaximumLoopTemperature,value);
}

double PlantLoop_Impl::minimumLoopTemperature()
{
  return getDouble(OS_PlantLoopFields::MinimumLoopTemperature,true).get();
}

void PlantLoop_Impl::setMinimumLoopTemperature( double value )
{
  setDouble(OS_PlantLoopFields::MinimumLoopTemperature,value);
}

boost::optional<double> PlantLoop_Impl::maximumLoopFlowRate()
{
  return getDouble(OS_PlantLoopFields::MaximumLoopFlowRate,true);
}

void PlantLoop_Impl::setMaximumLoopFlowRate( double value )
{
  setDouble(OS_PlantLoopFields::MaximumLoopFlowRate,value);
}

bool PlantLoop_Impl::isMaximumLoopFlowRateAutosized()
{
  bool result = false;
  boost::optional<std::string> value = getString(OS_PlantLoopFields::MaximumLoopFlowRate, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

void PlantLoop_Impl::autosizeMaximumLoopFlowRate()
{
  setString(OS_PlantLoopFields::MaximumLoopFlowRate,"Autosize");
}

boost::optional<double> PlantLoop_Impl::minimumLoopFlowRate()
{
  return getDouble(OS_PlantLoopFields::MinimumLoopFlowRate,true);
}

void PlantLoop_Impl::setMinimumLoopFlowRate( double value )
{
  setDouble(OS_PlantLoopFields::MinimumLoopFlowRate,value);
}

bool PlantLoop_Impl::isMinimumLoopFlowRateAutosized()
{
  bool result = false;
  boost::optional<std::string> value = getString(OS_PlantLoopFields::MinimumLoopFlowRate, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

void PlantLoop_Impl::autosizeMinimumLoopFlowRate()
{
  setString(OS_PlantLoopFields::MinimumLoopFlowRate,"Autosize");
}

boost::optional<double> PlantLoop_Impl::plantLoopVolume()
{
  return getDouble(OS_PlantLoopFields::PlantLoopVolume,true);
}

void PlantLoop_Impl::setPlantLoopVolume( double value )
{
  setDouble(OS_PlantLoopFields::PlantLoopVolume,value);
}

bool PlantLoop_Impl::isPlantLoopVolumeAutocalculated()
{
  bool result = false;
  boost::optional<std::string> value = getString(OS_PlantLoopFields::PlantLoopVolume, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autocalculate");
  }
  return result;
}

void PlantLoop_Impl::autocalculatePlantLoopVolume()
{
  setString(OS_PlantLoopFields::PlantLoopVolume,"Autocalculate");
}

std::string PlantLoop_Impl::fluidType()
{
  return getString(OS_PlantLoopFields::FluidType,true).get();
}

void PlantLoop_Impl::setFluidType( const std::string & value )
{
  setString(OS_PlantLoopFields::FluidType,value);
}

Node PlantLoop_Impl::loopTemperatureSetpointNode()
{
  boost::optional<Node> node;

  node = getObject<ModelObject>().getModelObjectTarget<Node>(OS_PlantLoopFields::LoopTemperatureSetpointNodeName);

  OS_ASSERT(node);

  return node.get();
}

void PlantLoop_Impl::setLoopTemperatureSetpointNode( Node & node )
{
  if( node.model() == this->model() )
  {
    setPointer(OS_PlantLoopFields::LoopTemperatureSetpointNodeName,node.handle());
  }
}

std::vector<ModelObject> PlantLoop_Impl::children() const
{
  std::vector<ModelObject> result;

  result.push_back(sizingPlant());

  return result;
}

SizingPlant PlantLoop_Impl::sizingPlant() const
{
  boost::optional<SizingPlant> sizingPlant;

  std::vector<SizingPlant> sizingObjects;
  
  sizingObjects = model().getModelObjects<SizingPlant>();

  for( std::vector<SizingPlant>::iterator it = sizingObjects.begin();
       it < sizingObjects.end();
       ++it )
  {
    if( it->plantLoop().handle() == this->handle() )
    {
      sizingPlant = *it;
    }
  }

  if( sizingPlant )
  {
    return sizingPlant.get();
  }
  else
  {
    LOG_AND_THROW("PlantLoop missing SizingPlant object"); 
  }
}

boost::optional<std::string> PlantLoop_Impl::commonPipeSimulation() const
{
  return getString(OS_PlantLoopFields::CommonPipeSimulation);
}

bool PlantLoop_Impl::setCommonPipeSimulation(const std::string & value)
{
  return setString(OS_PlantLoopFields::CommonPipeSimulation,value);
}

void PlantLoop_Impl::resetCommonPipeSimulation()
{
  setString(OS_PlantLoopFields::CommonPipeSimulation,"");
}

} // detail

PlantLoop::PlantLoop(Model& model)
  : Loop(PlantLoop::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantLoop_Impl>());

  SizingPlant sizingPlant(model,*this);

  autocalculatePlantLoopVolume();

  // supply side

  Node supplyInletNode(model);
  Node supplyOutletNode(model);
  Node connectorNode(model);

  ConnectorMixer supplyMixer(model);
  ConnectorSplitter supplySplitter(model);

  model.connect( *this, this->supplyInletPort(),
                 supplyInletNode, supplyInletNode.inletPort() );

  model.connect( supplyInletNode,supplyInletNode.outletPort(),
                 supplySplitter,supplySplitter.inletPort() );

  model.connect( supplySplitter,supplySplitter.nextOutletPort(),
                 connectorNode,connectorNode.inletPort() );

  model.connect( connectorNode,connectorNode.outletPort(),
                 supplyMixer,supplyMixer.nextInletPort() );

  model.connect( supplyMixer,supplyMixer.outletPort(),
                 supplyOutletNode,supplyOutletNode.inletPort() );
  
  model.connect( supplyOutletNode, supplyOutletNode.outletPort(),
                 *this, this->supplyOutletPort() );

  // demand side

  Node demandInletNode(model);
  Node demandOutletNode(model);
  Node branchNode(model);
  ConnectorMixer mixer(model);
  ConnectorSplitter splitter(model);

  model.connect( *this, demandInletPort(),
                 demandInletNode, demandInletNode.inletPort() );

  model.connect( demandInletNode, demandInletNode.outletPort(),
                 splitter, splitter.inletPort() );

  model.connect( splitter, splitter.nextOutletPort(),
                 branchNode, branchNode.inletPort() );

  model.connect( branchNode, branchNode.outletPort(),
                 mixer, mixer.nextInletPort() );

  model.connect( mixer, mixer.outletPort(),
                 demandOutletNode, demandOutletNode.inletPort() );

  model.connect( demandOutletNode, demandOutletNode.outletPort(),
                 *this, demandOutletPort() );

  setLoopTemperatureSetpointNode(supplyOutletNode);

  setString(OS_PlantLoopFields::DemandSideConnectorListName,"");
  setString(OS_PlantLoopFields::LoadDistributionScheme,"");
  setString(OS_PlantLoopFields::AvailabilityManagerListName,"");
  setString(OS_PlantLoopFields::PlantLoopDemandCalculationScheme,"");
  setString(OS_PlantLoopFields::CommonPipeSimulation,"");
  setString(OS_PlantLoopFields::PressureSimulationType,"");
}

PlantLoop::PlantLoop(boost::shared_ptr<detail::PlantLoop_Impl> impl)
  : Loop(impl)
{}

//std::vector<ModelObject> PlantLoop::supplyComponents(HVACComponent inletComp,
//                                                     HVACComponent outletComp,
//                                                     openstudio::IddObjectType type)
//{
//  return getImpl<detail::PlantLoop_Impl>()->supplyComponents(inletComp, outletComp, type);
//}

std::vector<ModelObject> PlantLoop::demandComponents(HVACComponent inletComp,
                                                     HVACComponent outletComp,
                                                     openstudio::IddObjectType type)
{
  return getImpl<detail::PlantLoop_Impl>()->demandComponents(inletComp, outletComp, type);
}

std::vector<IdfObject> PlantLoop::remove()
{
  return getImpl<detail::PlantLoop_Impl>()->remove();
}

//std::vector<ModelObject> PlantLoop::components(openstudio::IddObjectType type)
//{
//  return getImpl<detail::PlantLoop_Impl>()->components( type );
//}

//std::vector<ModelObject> PlantLoop::supplyComponents(openstudio::IddObjectType type)
//{
//  return getImpl<detail::PlantLoop_Impl>()->supplyComponents( type );
//}

std::vector<ModelObject> PlantLoop::demandComponents(openstudio::IddObjectType type)
{
  return getImpl<detail::PlantLoop_Impl>()->demandComponents( type );
}

boost::optional<ModelObject> PlantLoop::component(openstudio::Handle handle)
{
  return getImpl<detail::PlantLoop_Impl>()->component( handle );
}

ModelObject PlantLoop::clone(Model model) const
{
  return getImpl<detail::PlantLoop_Impl>()->clone( model );
}

//std::vector<ModelObject> PlantLoop::supplyComponents(std::vector<HVACComponent> inletComps,
//    std::vector<HVACComponent> outletComps,
//    openstudio::IddObjectType type
//  )
//{
//  return getImpl<detail::PlantLoop_Impl>()->supplyComponents( inletComps, outletComps, type);
//}

std::vector<ModelObject> PlantLoop::demandComponents(std::vector<HVACComponent> inletComps,
    std::vector<HVACComponent> outletComps,
    openstudio::IddObjectType type
  )
{
  return getImpl<detail::PlantLoop_Impl>()->demandComponents( inletComps, outletComps, type);
}

unsigned PlantLoop::supplyInletPort() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyInletPort();
}

unsigned PlantLoop::supplyOutletPort() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyOutletPort();
}

unsigned PlantLoop::demandInletPort() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandInletPort();
}

unsigned PlantLoop::demandOutletPort() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandOutletPort();
}

Node PlantLoop::supplyInletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyInletNode();
}

Node PlantLoop::supplyOutletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyOutletNode();
}

Node PlantLoop::demandInletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandInletNode();
}

Node PlantLoop::demandOutletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandOutletNode();
}

bool PlantLoop::addDemandBranchForComponent( HVACComponent component )
{
  return getImpl<detail::PlantLoop_Impl>()->addDemandBranchForComponent( component );
}

bool PlantLoop::addSupplyBranchForComponent( HVACComponent component )
{
  return getImpl<detail::PlantLoop_Impl>()->addSupplyBranchForComponent( component );
}

bool PlantLoop::removeSupplyBranchWithComponent( HVACComponent component )
{
  return getImpl<detail::PlantLoop_Impl>()->removeSupplyBranchWithComponent( component );
}

bool PlantLoop::removeDemandBranchWithComponent( HVACComponent component )
{
  return getImpl<detail::PlantLoop_Impl>()->removeDemandBranchWithComponent( component );
}

Mixer PlantLoop::supplyMixer()
{
  return getImpl<detail::PlantLoop_Impl>()->supplyMixer();
}

Splitter PlantLoop::supplySplitter()
{
  return getImpl<detail::PlantLoop_Impl>()->supplySplitter();
}

Mixer PlantLoop::demandMixer()
{
  return getImpl<detail::PlantLoop_Impl>()->demandMixer();
}

Splitter PlantLoop::demandSplitter()
{
  return getImpl<detail::PlantLoop_Impl>()->demandSplitter();
}

IddObjectType PlantLoop::iddObjectType() {
  IddObjectType result(IddObjectType::OS_PlantLoop);
  return result;
}

double PlantLoop::maximumLoopTemperature()
{
  return getImpl<detail::PlantLoop_Impl>()->maximumLoopTemperature();
}

void PlantLoop::setMaximumLoopTemperature( double value )
{
  getImpl<detail::PlantLoop_Impl>()->setMaximumLoopTemperature( value );
}

double PlantLoop::minimumLoopTemperature()
{
  return getImpl<detail::PlantLoop_Impl>()->minimumLoopTemperature();
}

void PlantLoop::setMinimumLoopTemperature( double value )
{
  getImpl<detail::PlantLoop_Impl>()->setMinimumLoopTemperature( value );
}

boost::optional<double> PlantLoop::maximumLoopFlowRate()
{
  return getImpl<detail::PlantLoop_Impl>()->maximumLoopFlowRate();
}

void PlantLoop::setMaximumLoopFlowRate( double value )
{
  getImpl<detail::PlantLoop_Impl>()->setMaximumLoopFlowRate( value );
}

bool PlantLoop::isMaximumLoopFlowRateAutosized()
{
  return getImpl<detail::PlantLoop_Impl>()->isMaximumLoopFlowRateAutosized();
}

void PlantLoop::autosizeMaximumLoopFlowRate()
{
  getImpl<detail::PlantLoop_Impl>()->autosizeMaximumLoopFlowRate();
}

boost::optional<double> PlantLoop::minimumLoopFlowRate()
{
  return getImpl<detail::PlantLoop_Impl>()->minimumLoopFlowRate();
}

void PlantLoop::setMinimumLoopFlowRate( double value )
{
  getImpl<detail::PlantLoop_Impl>()->setMinimumLoopFlowRate( value );
}

bool PlantLoop::isMinimumLoopFlowRateAutosized()
{
  return getImpl<detail::PlantLoop_Impl>()->isMinimumLoopFlowRateAutosized();
}

void PlantLoop::autosizeMinimumLoopFlowRate()
{
  getImpl<detail::PlantLoop_Impl>()->autosizeMinimumLoopFlowRate();
}

boost::optional<double> PlantLoop::plantLoopVolume()
{
  return getImpl<detail::PlantLoop_Impl>()->plantLoopVolume();
}

void PlantLoop::setPlantLoopVolume( double value )
{
  getImpl<detail::PlantLoop_Impl>()->setPlantLoopVolume( value );
}

bool PlantLoop::isPlantLoopVolumeAutocalculated()
{
  return getImpl<detail::PlantLoop_Impl>()->isPlantLoopVolumeAutocalculated();
}

void PlantLoop::autocalculatePlantLoopVolume()
{
  getImpl<detail::PlantLoop_Impl>()->autocalculatePlantLoopVolume();
}

std::string PlantLoop::fluidType()
{
  return getImpl<detail::PlantLoop_Impl>()->fluidType();
}

void PlantLoop::setFluidType( const std::string & value )
{
  getImpl<detail::PlantLoop_Impl>()->setFluidType( value );
}

Node PlantLoop::loopTemperatureSetpointNode()
{
  return getImpl<detail::PlantLoop_Impl>()->loopTemperatureSetpointNode();
}

void PlantLoop::setLoopTemperatureSetpointNode( Node & node )
{
  getImpl<detail::PlantLoop_Impl>()->setLoopTemperatureSetpointNode( node );
}

SizingPlant PlantLoop::sizingPlant() const
{
  return getImpl<detail::PlantLoop_Impl>()->sizingPlant();
}

boost::optional<std::string> PlantLoop::commonPipeSimulation() const
{
  return getImpl<detail::PlantLoop_Impl>()->commonPipeSimulation();
}

bool PlantLoop::setCommonPipeSimulation(const std::string & value)
{
  return getImpl<detail::PlantLoop_Impl>()->setCommonPipeSimulation(value);
}

void PlantLoop::resetCommonPipeSimulation()
{
  getImpl<detail::PlantLoop_Impl>()->resetCommonPipeSimulation();
}


} // model
} // openstudio

