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

#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "SizingPlant.hpp"
#include "SizingPlant_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "PlantEquipmentOperationScheme.hpp"
#include "PlantEquipmentOperationScheme_Impl.hpp"
#include "PlantEquipmentOperationHeatingLoad.hpp"
#include "PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "PlantEquipmentOperationCoolingLoad.hpp"
#include "PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_PlantLoop_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>

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

std::vector<openstudio::IdfObject> PlantLoop_Impl::remove()
{
  sizingPlant().remove();

  ModelObjectVector modelObjects;
  ModelObjectVector::iterator it;

  if( auto t_availabilityManager = availabilityManager() ) {
    t_availabilityManager->remove();
  }

  modelObjects = supplyComponents();
  for(it = modelObjects.begin();
      it != modelObjects.end();
      ++it)
  {
    if( boost::optional<WaterToWaterComponent> comp = it->optionalCast<WaterToWaterComponent>() )
    {
      comp->removeFromPlantLoop();
      if( ! comp->secondaryPlantLoop() )
      {
        comp->remove();
      }
    }
  }

  modelObjects = demandComponents();
  for(it = modelObjects.begin();
      it != modelObjects.end();
      ++it)
  {
    if( boost::optional<WaterToWaterComponent> comp = it->optionalCast<WaterToWaterComponent>() )
    {
      comp->removeFromSecondaryPlantLoop();
      if( ! comp->plantLoop() )
      {
        comp->remove();
      }
    }
    else if( boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>() )
    {
      comp->removeFromPlantLoop();
      if( ! comp->airLoopHVAC() )
      {
        comp->remove();
      }
    }
  }

  modelObjects = components();

  std::vector<openstudio::IdfObject> idfObjects =  ModelObject_Impl::remove();

  for(it = modelObjects.begin();
      it != modelObjects.end();
      ++it)
  {
    if( boost::optional<HVACComponent> comp = it->optionalCast<HVACComponent>() )
    {
      comp->disconnect();
    }
    it->remove();
  }

  return idfObjects;
}

IddObjectType PlantLoop_Impl::iddObjectType() const {
  return PlantLoop::iddObjectType();
}

ModelObject PlantLoop_Impl::clone(Model model) const
{
  PlantLoop plantLoopClone = Loop_Impl::clone(model).cast<PlantLoop>();
  if( auto mo = availabilityManager() ) {
    auto clone = mo->clone(model).cast<AvailabilityManager>();
    plantLoopClone.setAvailabilityManager(clone);
  } else {
    plantLoopClone.setString(OS_PlantLoopFields::AvailabilityManagerName,"");
  }

  return plantLoopClone;
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

std::vector<Node> PlantLoop_Impl::supplyOutletNodes() const
{
  return std::vector<Node> { supplyOutletNode() };
}

Node PlantLoop_Impl::demandInletNode() const
{
  return connectedObject(demandInletPort())->optionalCast<Node>().get();
}

std::vector<Node> PlantLoop_Impl::demandInletNodes() const
{
  return std::vector<Node> { demandInletNode() };
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

bool PlantLoop_Impl::addDemandBranchForComponent( HVACComponent component, bool tertiary )
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
          if( auto waterToWater = component.optionalCast<WaterToWaterComponent>() ) {
            if( tertiary ) {
              return waterToWater->addToTertiaryNode(node.get());
            } else {
              return waterToWater->addToNode(node.get());
            }
          }
          else {
            return component.addToNode(node.get());
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

   bool result = false;

   if( auto waterToWater = component.optionalCast<WaterToWaterComponent>() ) {
     if( tertiary ) {
       result = waterToWater->addToTertiaryNode(node);
     } else {
       result = waterToWater->addToNode(node);
     }
   }
   else {
     result = component.addToNode(node);
   }

  if( result )
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

  for( auto & elem : allComponents )
  {
    if( ! elem.optionalCast<Node>() )
    {
      if( boost::optional<StraightComponent> comp = elem.optionalCast<StraightComponent>() )
      {
        elem.remove();
      }
      else if( boost::optional<WaterToAirComponent> comp = elem.optionalCast<WaterToAirComponent>() )
      {
        comp->removeFromPlantLoop();

        if( ! comp->airLoopHVAC() )
        {
          comp->remove();
        }
      }
      else if( boost::optional<WaterToWaterComponent> comp = elem.optionalCast<WaterToWaterComponent>() )
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

Mixer PlantLoop_Impl::supplyMixer() const
{
  auto result = getObject<ModelObject>().getModelObjectTarget<Mixer>(OS_PlantLoopFields::SupplyMixerName);
  if (result) return result.get();
  return supplyComponents( IddObjectType::OS_Connector_Mixer ).front().cast<Mixer>();
}

void PlantLoop_Impl::setSupplyMixer(Mixer const & mixer)
{
  auto result = setPointer(OS_PlantLoopFields::SupplyMixerName,mixer.handle());
  OS_ASSERT(result);
}

Splitter PlantLoop_Impl::supplySplitter() const
{
  auto result = getObject<ModelObject>().getModelObjectTarget<Splitter>(OS_PlantLoopFields::SupplySplitterName);
  if (result) return result.get();
  return supplyComponents( IddObjectType::OS_Connector_Splitter ).front().cast<Splitter>();
}

void PlantLoop_Impl::setSupplySplitter(Splitter const & splitter)
{
  auto result = setPointer(OS_PlantLoopFields::SupplySplitterName,splitter.handle());
  OS_ASSERT(result);
}

Mixer PlantLoop_Impl::demandMixer()
{
  auto result = getObject<ModelObject>().getModelObjectTarget<Mixer>(OS_PlantLoopFields::DemandMixerName);
  if (result) return result.get();
  return demandComponents( IddObjectType::OS_Connector_Mixer ).front().cast<Mixer>();
}

void PlantLoop_Impl::setDemandMixer(Mixer const & mixer)
{
  auto result = setPointer(OS_PlantLoopFields::DemandMixerName,mixer.handle());
  OS_ASSERT(result);
}

Splitter PlantLoop_Impl::demandSplitter()
{
  auto result = getObject<ModelObject>().getModelObjectTarget<Splitter>(OS_PlantLoopFields::DemandSplitterName);
  if (result) return result.get();
  return demandComponents( IddObjectType::OS_Connector_Splitter ).front().cast<Splitter>();
}

void PlantLoop_Impl::setDemandSplitter(Splitter const & splitter)
{
  auto result = setPointer(OS_PlantLoopFields::DemandSplitterName,splitter.handle());
  OS_ASSERT(result);
}

std::string PlantLoop_Impl::loadDistributionScheme()
{
  auto value = getString(OS_PlantLoopFields::LoadDistributionScheme,true);
  OS_ASSERT(value);
  return value.get();
}

bool PlantLoop_Impl::setLoadDistributionScheme(std::string scheme)
{
  if( istringEqual(scheme,"Sequential") ) {
    scheme = "SequentialLoad";
  } else if( istringEqual(scheme,"Uniform") ) {
    scheme = "UniformLoad";
  }
  return setString(OS_PlantLoopFields::LoadDistributionScheme,scheme);
}

boost::optional<AvailabilityManager> PlantLoop_Impl::availabilityManager() const {
  return getObject<ModelObject>().getModelObjectTarget<AvailabilityManager>(OS_PlantLoopFields::AvailabilityManagerName);
}

bool PlantLoop_Impl::setAvailabilityManager(const AvailabilityManager & availabilityManager) {
  auto type = availabilityManager.iddObjectType();
  if( type == IddObjectType::OS_AvailabilityManager_NightCycle ||
      type == IddObjectType::OS_AvailabilityManager_HybridVentilation ||
      type == IddObjectType::OS_AvailabilityManager_NightVentilation ||
      type == IddObjectType::OS_AvailabilityManager_OptimumStart ||
      type == IddObjectType::OS_AvailabilityManager_DifferentialThermostat) {
    return setPointer(OS_PlantLoopFields::AvailabilityManagerName, availabilityManager.handle());
  }
  return false;
}

void PlantLoop_Impl::resetAvailabilityManager() {
  bool result = setString(OS_PlantLoopFields::AvailabilityManagerName, "");
  OS_ASSERT(result);
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

bool PlantLoop_Impl::setFluidType( const std::string & value )
{
  return setString(OS_PlantLoopFields::FluidType,value);
}

int PlantLoop_Impl::glycolConcentration() const {
  return getInt(OS_PlantLoopFields::GlycolConcentration,true).get();
}

void PlantLoop_Impl::setGlycolConcentration(int glycolConcentration) {
  setInt(OS_PlantLoopFields::GlycolConcentration, glycolConcentration);
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

  sizingObjects = model().getConcreteModelObjects<SizingPlant>();

  for( const auto & sizingObject : sizingObjects )
  {
    if( sizingObject.plantLoop().handle() == this->handle() )
    {
      sizingPlant = sizingObject;
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

  boost::optional<PlantEquipmentOperationHeatingLoad> PlantLoop_Impl::plantEquipmentOperationHeatingLoad() const {
    return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationHeatingLoad>(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad);
  }

  bool PlantLoop_Impl::setPlantEquipmentOperationHeatingLoad(const boost::optional<PlantEquipmentOperationHeatingLoad>& plantEquipmentOperationHeatingLoad) {
    bool result(false);
    if (plantEquipmentOperationHeatingLoad) {
      result = setPointer(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad, plantEquipmentOperationHeatingLoad.get().handle());
    }
    else {
      resetPlantEquipmentOperationHeatingLoad();
      result = true;
    }
    return result;
  }

  void PlantLoop_Impl::resetPlantEquipmentOperationHeatingLoad() {
    bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad, "");
    OS_ASSERT(result);
  }

  boost::optional<PlantEquipmentOperationCoolingLoad> PlantLoop_Impl::plantEquipmentOperationCoolingLoad() const {
    return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationCoolingLoad>(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad);
  }

  bool PlantLoop_Impl::setPlantEquipmentOperationCoolingLoad(const boost::optional<PlantEquipmentOperationCoolingLoad>& plantEquipmentOperationCoolingLoad) {
    bool result(false);
    if (plantEquipmentOperationCoolingLoad) {
      result = setPointer(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad, plantEquipmentOperationCoolingLoad.get().handle());
    }
    else {
      resetPlantEquipmentOperationCoolingLoad();
      result = true;
    }
    return result;
  }

  void PlantLoop_Impl::resetPlantEquipmentOperationCoolingLoad() {
    bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad, "");
    OS_ASSERT(result);
  }

  boost::optional<PlantEquipmentOperationScheme> PlantLoop_Impl::primaryPlantEquipmentOperationScheme() const {
    return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationScheme>(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme);
  }

  bool PlantLoop_Impl::setPrimaryPlantEquipmentOperationScheme(const boost::optional<PlantEquipmentOperationScheme>& plantEquipmentOperationScheme) {
    bool result(false);
    if (plantEquipmentOperationScheme) {
      result = setPointer(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme, plantEquipmentOperationScheme.get().handle());
    }
    else {
      resetPrimaryPlantEquipmentOperationScheme();
      result = true;
    }
    return result;
  }

  void PlantLoop_Impl::resetPrimaryPlantEquipmentOperationScheme() {
    bool result = setString(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme, "");
    OS_ASSERT(result);
  }

  bool PlantLoop_Impl::setPlantEquipmentOperationHeatingLoadSchedule(Schedule & schedule) {
    bool result = setSchedule(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule,
                              "PlantLoop",
                              "Plant Equipment Operation Heating Load Schedule",
                              schedule);
    return result;
  }

  void PlantLoop_Impl::resetPlantEquipmentOperationHeatingLoadSchedule() {
    setString(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule, "");
  }

  boost::optional<Schedule> PlantLoop_Impl::plantEquipmentOperationHeatingLoadSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule);
  }

  bool PlantLoop_Impl::setPlantEquipmentOperationCoolingLoadSchedule(Schedule & schedule) {
    bool result = setSchedule(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule,
                              "PlantLoop",
                              "Plant Equipment Operation Cooling Load Schedule",
                              schedule);
    return result;
  }

  boost::optional<Schedule> PlantLoop_Impl::plantEquipmentOperationCoolingLoadSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule);
  }

  void PlantLoop_Impl::resetPlantEquipmentOperationCoolingLoadSchedule() {
    setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule, "");
  }

  bool PlantLoop_Impl::setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule & schedule) {
    bool result = setSchedule(OS_PlantLoopFields::PrimaryPlantEquipmentOperationSchemeSchedule,
                              "PlantLoop",
                              "Primary Plant Equipment Operation Scheme Schedule",
                              schedule);
    return result;
  }

  void PlantLoop_Impl::resetPrimaryPlantEquipmentOperationSchemeSchedule() {
    setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule, "");
  }

  boost::optional<Schedule> PlantLoop_Impl::primaryPlantEquipmentOperationSchemeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PrimaryPlantEquipmentOperationSchemeSchedule);
  }

  bool PlantLoop_Impl::setComponentSetpointOperationSchemeSchedule(Schedule & schedule) {
    bool result = setSchedule(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule,
                              "PlantLoop",
                              "Component Setpoint Operation Scheme Schedule",
                              schedule);
    return result;
  }

  boost::optional<Schedule> PlantLoop_Impl::componentSetpointOperationSchemeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule);
  }

  void PlantLoop_Impl::resetComponentSetpointOperationSchemeSchedule() {
    setString(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule, "");
  }

} // detail

PlantLoop::PlantLoop(Model& model)
  : Loop(PlantLoop::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantLoop_Impl>());

  SizingPlant sizingPlant(model,*this);

  autocalculatePlantLoopVolume();
  setLoadDistributionScheme("Optimal");

  // supply side

  Node supplyInletNode(model);
  Node supplyOutletNode(model);
  Node connectorNode(model);

  ConnectorMixer supplyMixer(model);
  getImpl<detail::PlantLoop_Impl>()->setSupplyMixer(supplyMixer);
  ConnectorSplitter supplySplitter(model);
  getImpl<detail::PlantLoop_Impl>()->setSupplySplitter(supplySplitter);

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
  getImpl<detail::PlantLoop_Impl>()->setDemandMixer(mixer);
  ConnectorSplitter splitter(model);
  getImpl<detail::PlantLoop_Impl>()->setDemandSplitter(splitter);

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

  setGlycolConcentration(0);
  setString(OS_PlantLoopFields::DemandSideConnectorListName,"");
  setString(OS_PlantLoopFields::PlantLoopDemandCalculationScheme,"");
  setString(OS_PlantLoopFields::CommonPipeSimulation,"");
  setString(OS_PlantLoopFields::PressureSimulationType,"");
}

PlantLoop::PlantLoop(std::shared_ptr<detail::PlantLoop_Impl> impl)
  : Loop(std::move(impl))
{}

boost::optional<AvailabilityManager> PlantLoop::availabilityManager() const
{
  return getImpl<detail::PlantLoop_Impl>()->availabilityManager();
}

bool PlantLoop::setAvailabilityManager(const AvailabilityManager& availabilityManager)
{
  return getImpl<detail::PlantLoop_Impl>()->setAvailabilityManager(availabilityManager);
}

void PlantLoop::resetAvailabilityManager()
{
  return getImpl<detail::PlantLoop_Impl>()->resetAvailabilityManager();
}

std::vector<IdfObject> PlantLoop::remove()
{
  return getImpl<detail::PlantLoop_Impl>()->remove();
}

ModelObject PlantLoop::clone(Model model) const
{
  return getImpl<detail::PlantLoop_Impl>()->clone( model );
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

std::vector<Node> PlantLoop::supplyOutletNodes() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyOutletNodes();
}

Node PlantLoop::demandInletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandInletNode();
}

std::vector<Node> PlantLoop::demandInletNodes() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandInletNodes();
}

Node PlantLoop::demandOutletNode() const
{
  return getImpl<detail::PlantLoop_Impl>()->demandOutletNode();
}

bool PlantLoop::addDemandBranchForComponent( HVACComponent component, bool tertiary )
{
  return getImpl<detail::PlantLoop_Impl>()->addDemandBranchForComponent( component, tertiary );
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

Mixer PlantLoop::supplyMixer() const
{
  return getImpl<detail::PlantLoop_Impl>()->supplyMixer();
}

Splitter PlantLoop::supplySplitter() const
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

bool PlantLoop::setFluidType( const std::string & value )
{
  return getImpl<detail::PlantLoop_Impl>()->setFluidType( value );
}

std::vector<std::string> PlantLoop::fluidTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantLoopFields::FluidType);

}

int PlantLoop::glycolConcentration() const {
  return getImpl<detail::PlantLoop_Impl>()->glycolConcentration();
}

void PlantLoop::setGlycolConcentration(int glycolConcentration) {
  getImpl<detail::PlantLoop_Impl>()->setGlycolConcentration(glycolConcentration);
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

boost::optional<PlantEquipmentOperationHeatingLoad> PlantLoop::plantEquipmentOperationHeatingLoad() const {
  return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationHeatingLoad();
}

bool PlantLoop::setPlantEquipmentOperationHeatingLoad(const PlantEquipmentOperationHeatingLoad& plantOperation) {
  return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationHeatingLoad(plantOperation);
}

void PlantLoop::resetPlantEquipmentOperationHeatingLoad() {
  getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationHeatingLoad();
}

boost::optional<PlantEquipmentOperationCoolingLoad> PlantLoop::plantEquipmentOperationCoolingLoad() const {
  return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationCoolingLoad();
}

bool PlantLoop::setPlantEquipmentOperationCoolingLoad(const PlantEquipmentOperationCoolingLoad& plantOperation) {
  return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationCoolingLoad(plantOperation);
}

void PlantLoop::resetPlantEquipmentOperationCoolingLoad() {
  getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationCoolingLoad();
}

boost::optional<PlantEquipmentOperationScheme> PlantLoop::primaryPlantEquipmentOperationScheme() const {
  return getImpl<detail::PlantLoop_Impl>()->primaryPlantEquipmentOperationScheme();
}

bool PlantLoop::setPrimaryPlantEquipmentOperationScheme(const PlantEquipmentOperationScheme& plantOperation) {
  return getImpl<detail::PlantLoop_Impl>()->setPrimaryPlantEquipmentOperationScheme(plantOperation);
}

void PlantLoop::resetPrimaryPlantEquipmentOperationScheme() {
  getImpl<detail::PlantLoop_Impl>()->resetPrimaryPlantEquipmentOperationScheme();
}

std::string PlantLoop::loadDistributionScheme()
{
  return getImpl<detail::PlantLoop_Impl>()->loadDistributionScheme();
}

bool PlantLoop::setLoadDistributionScheme(std::string scheme)
{
  return getImpl<detail::PlantLoop_Impl>()->setLoadDistributionScheme(scheme);
}

std::vector<std::string> PlantLoop::loadDistributionSchemeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantLoopFields::LoadDistributionScheme);
}

bool PlantLoop::setPlantEquipmentOperationHeatingLoadSchedule(Schedule & schedule) {
  return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationHeatingLoadSchedule(schedule);
}

void PlantLoop::resetPlantEquipmentOperationHeatingLoadSchedule() {
  getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationHeatingLoadSchedule();
}

boost::optional<Schedule> PlantLoop::plantEquipmentOperationHeatingLoadSchedule() const {
  return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationHeatingLoadSchedule();
}

bool PlantLoop::setPlantEquipmentOperationCoolingLoadSchedule(Schedule & schedule) {
  return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationCoolingLoadSchedule(schedule);
}

boost::optional<Schedule> PlantLoop::plantEquipmentOperationCoolingLoadSchedule() const {
  return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationCoolingLoadSchedule();
}

void PlantLoop::resetPlantEquipmentOperationCoolingLoadSchedule() {
  getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationCoolingLoadSchedule();
}

bool PlantLoop::setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule & schedule) {
  return getImpl<detail::PlantLoop_Impl>()->setPrimaryPlantEquipmentOperationSchemeSchedule(schedule);
}

void PlantLoop::resetPrimaryPlantEquipmentOperationSchemeSchedule() {
  getImpl<detail::PlantLoop_Impl>()->resetPrimaryPlantEquipmentOperationSchemeSchedule();
}

boost::optional<Schedule> PlantLoop::primaryPlantEquipmentOperationSchemeSchedule() const {
  return getImpl<detail::PlantLoop_Impl>()->primaryPlantEquipmentOperationSchemeSchedule();
}

bool PlantLoop::setComponentSetpointOperationSchemeSchedule(Schedule & schedule) {
  return getImpl<detail::PlantLoop_Impl>()->setComponentSetpointOperationSchemeSchedule(schedule);
}

boost::optional<Schedule> PlantLoop::componentSetpointOperationSchemeSchedule() const {
  return getImpl<detail::PlantLoop_Impl>()->componentSetpointOperationSchemeSchedule();
}

void PlantLoop::resetComponentSetpointOperationSchemeSchedule() {
  getImpl<detail::PlantLoop_Impl>()->resetComponentSetpointOperationSchemeSchedule();
}

} // model
} // openstudio

