/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirLoopHVACSupplyPlenum.hpp"
#include "AirLoopHVACSupplyPlenum_Impl.hpp"
#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "SizingSystem.hpp"
#include "SizingSystem_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"
#include "AvailabilityManagerScheduled.hpp"
#include "AvailabilityManagerScheduled_Impl.hpp"
#include "AvailabilityManagerNightCycle.hpp"
#include "AvailabilityManagerNightCycle_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "ScheduleYear.hpp"
#include "ScheduleYear_Impl.hpp"
#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  AirLoopHVAC_Impl::AirLoopHVAC_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Loop_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVAC::iddObjectType());
  }

  AirLoopHVAC_Impl::AirLoopHVAC_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : Loop_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVAC::iddObjectType());
  }

  AirLoopHVAC_Impl::AirLoopHVAC_Impl(const AirLoopHVAC_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : Loop_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVAC_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType AirLoopHVAC_Impl::iddObjectType() const {
    return AirLoopHVAC::iddObjectType();
  }

  Node AirLoopHVAC_Impl::supplyInletNode() const
  {
    OptionalModelObject optionalModelObject;
    OptionalNode optionalNode;
    optionalModelObject = this->connectedObject(openstudio::OS_AirLoopHVACFields::SupplySideInletNodeName);
    if(optionalModelObject)
    {
      optionalNode = optionalModelObject->optionalCast<Node>();
    }
    return optionalNode.get();
  }

  std::vector<Node> AirLoopHVAC_Impl::supplyOutletNodes() const
  {
    std::vector<Node> nodeVector;
    OptionalModelObject optionalModelObject;
    OptionalNode optionalNode;
    optionalModelObject = this->connectedObject(openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeNames);
    if(optionalModelObject)
    {
      optionalNode = optionalModelObject->optionalCast<Node>();
      if(optionalNode)
      {
        nodeVector.push_back(*optionalNode);
      }
    }
    return nodeVector;
  }


  std::vector<Node> AirLoopHVAC_Impl::demandInletNodes() const
  {
    std::vector<Node> nodeVector;
    OptionalModelObject optionalModelObject;
    OptionalNode optionalNode;
    optionalModelObject = this->connectedObject(openstudio::OS_AirLoopHVACFields::DemandSideInletNodeNames);
    if(optionalModelObject)
    {
      optionalNode = optionalModelObject->optionalCast<Node>();
      if(optionalNode)
      {
        nodeVector.push_back(*optionalNode);
      }
    }
    return nodeVector;
  }


  Node AirLoopHVAC_Impl::demandOutletNode() const
  {
    OptionalModelObject optionalModelObject;
    OptionalNode optionalNode;
    optionalModelObject = this->connectedObject(openstudio::OS_AirLoopHVACFields::DemandSideOutletNodeName);
    if(optionalModelObject)
    {
      optionalNode = optionalModelObject->optionalCast<Node>();
    }
    return optionalNode.get();
  }

  void AirLoopHVAC_Impl::addAirLoopComp(ModelObject targetObj, ModelObject newComp)
  {
    if( OptionalNode node = targetObj.optionalCast<Node>() )
    {
      if( OptionalHVACComponent comp = newComp.optionalCast<HVACComponent>() )
      {
        comp->addToNode( node.get() );
      }
    }
  }

  void AirLoopHVAC_Impl::removeAirLoopComp(ModelObject targetObj)
  {
    Model model = targetObj.model();

    OptionalStraightComponent straightComponent = targetObj.optionalCast<StraightComponent>();
    // if we are deleting a straight component that is not a node
    if( straightComponent && ! targetObj.optionalCast<Node>() )
    {
      straightComponent->remove();
    }
    // if we are deleting an AirLoopHVACOutdoorAirSystem
    OptionalAirLoopHVACOutdoorAirSystem mixer = targetObj.optionalCast<AirLoopHVACOutdoorAirSystem>();
    if(mixer)
    {
      mixer->remove();
    }
  }

  std::vector<openstudio::IdfObject> AirLoopHVAC_Impl::remove()
  {
    ModelObjectVector modelObjects;
    ModelObjectVector::iterator it;

    sizingSystem().remove();

    availabilityManagerAssignmentList().remove();

    modelObjects = supplyComponents();
    for(it = modelObjects.begin();
        it != modelObjects.end();
        ++it)
    {
      if( boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>() )
      {
        comp->removeFromAirLoopHVAC();
        if( ! comp->plantLoop() )
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
      if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
      {
        if( ! it->handle().isNull() )
        {
          comp->disconnect();
          if( ! comp->optionalCast<ThermalZone>() )
          {
            comp->remove();
          }
        }
      }
    }

    return idfObjects;
  }

  std::vector<ModelObject> AirLoopHVAC_Impl::components(openstudio::IddObjectType type)
  {
    std::vector<ModelObject> result;

    result = Loop_Impl::components( type );

    std::vector<ModelObject> oaComponents = this->oaComponents( type );

    result.insert(result.end(),oaComponents.begin(),oaComponents.end());

    return result;
  }

  OptionalAirLoopHVACOutdoorAirSystem AirLoopHVAC_Impl::airLoopHVACOutdoorAirSystem() const
  {
    OptionalAirLoopHVACOutdoorAirSystem result;
    ModelObjectVector modelObjects;
    modelObjects = supplyComponents( this->supplyInletNode(),
                                     this->supplyOutletNodes().front(),
                                     openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem );
    if( modelObjects.size() == 1 )
    {
      if( OptionalAirLoopHVACOutdoorAirSystem oaSystem
          = modelObjects.front().optionalCast<AirLoopHVACOutdoorAirSystem>() )
      {
        result = oaSystem;
      }
    }

    return result;
  }

  boost::optional<ThermalZone> AirLoopHVAC_Impl::zoneForLastBranch(Mixer & mixer)
  {
    if( OptionalNode node = mixer.lastInletModelObject()->optionalCast<Node>() )
    {
      return node->inletModelObject()->optionalCast<ThermalZone>();
    }
    else
    {
      return boost::none;
    }
  }

  boost::optional<StraightComponent> AirLoopHVAC_Impl::terminalForLastBranch(Mixer & mixer)
  {
    if( OptionalNode node = mixer.lastInletModelObject()->optionalCast<Node>() )
    {
      if( boost::optional<ThermalZone> zone = node->inletModelObject()->optionalCast<ThermalZone>() )
      {
        boost::optional<Node> zoneInletNode = zone->inletPortList().airLoopHVACModelObject()->optionalCast<Node>();

        OS_ASSERT( zoneInletNode );

        boost::optional<ModelObject> mo = zoneInletNode->inletModelObject();

        OS_ASSERT( mo );

        if( ! mo->optionalCast<Splitter>() )
        {
          if( boost::optional<StraightComponent> hvacComponent = mo->optionalCast<StraightComponent>() )
          {
            return hvacComponent;
          }
        }
      }
      else
      {
        boost::optional<ModelObject> mo = node->inletModelObject();

        OS_ASSERT( mo );

        if( ! mo->optionalCast<Splitter>() && ! mo->optionalCast<Mixer>() && ! mo->optionalCast<Node>() )
        {
          if( boost::optional<StraightComponent> hvacComponent = mo->optionalCast<StraightComponent>() )
          {
            return hvacComponent;
          }
        }
      }
    }

    return boost::none;
  }

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, 
                                              AirLoopHVAC & airLoopHVAC,
                                              Splitter & splitter,
                                              Mixer & mixer,
                                              OptionalStraightComponent & optAirTerminal)
  {
    Model _model = thermalZone.model();

    if( (optAirTerminal && _model != optAirTerminal->model()) ||
         _model != splitter.model() || 
         _model != mixer.model() )
    {
      return false;
    }

    if( optAirTerminal && optAirTerminal->airLoopHVAC() )
    {
      return false;
    }

    if( thermalZone.isPlenum() )
    {
      return false;
    }

    if( ! airLoopHVAC.demandComponent(splitter.handle()) )
    {
      return false;
    }

    if( ! airLoopHVAC.demandComponent(mixer.handle()) )
    {
      return false;
    }

    thermalZone.setUseIdealAirLoads(false);

    if( OptionalAirLoopHVAC airLoopHVAC = thermalZone.airLoopHVAC() )
    {
      airLoopHVAC->removeBranchForZone(thermalZone);
    }

    boost::optional<Node> node;

    std::vector<ModelObject> objects = airLoopHVAC.demandComponents(splitter,mixer);
    if( objects.size() == 3u )
    {
      node = objects[1].optionalCast<Node>();
    }

    if( ! node )
    {
      int nextOutletPort = splitter.nextOutletPort();
      int nextInletPort = mixer.nextInletPort();

      Node newNode(_model);

      _model.connect( splitter,
                      nextOutletPort,
                      newNode,
                      newNode.inletPort() );
      _model.connect( newNode,
                      newNode.outletPort(),
                      mixer,
                      nextInletPort );

      node = newNode;
    }

    OS_ASSERT(node);

    thermalZone.addToNode(node.get());

    if( optAirTerminal )
    {
      Node airInletNode = thermalZone.inletPortList().airLoopHVACModelObject()->cast<Node>();

      optAirTerminal->addToNode(airInletNode);
    }

    return true;
  }

  boost::optional<PlantLoop> AirLoopHVAC_Impl::plantForAirTerminal( HVACComponent & airTerminal )
  {
    std::vector<WaterToAirComponent> comps = airTerminal.model().getModelObjects<WaterToAirComponent>();

    for( const auto & elem : comps )
    {
      if( boost::optional<HVACComponent> comp = elem.containingHVACComponent() )
      {
        if( comp.get() == airTerminal )
        {
          if( boost::optional<PlantLoop> plantLoop = elem.plantLoop() )
          {
            return plantLoop;
          }
          else
          {
            return boost::none;
          }
        }
      }
    }

    return boost::none;
  }

  void AirLoopHVAC_Impl::setPlantForAirTerminal( HVACComponent & airTerminal, PlantLoop & plantLoop )
  {
    std::vector<WaterToAirComponent> comps = airTerminal.model().getModelObjects<WaterToAirComponent>();

    for( const auto & elem : comps )
    {
      if( boost::optional<HVACComponent> comp = elem.containingHVACComponent() )
      {
        if( comp.get() == airTerminal )
        {
          plantLoop.addDemandBranchForComponent(elem);

          return;
        }
      }
    }
  }

  bool AirLoopHVAC_Impl::removeBranchForZone(ThermalZone & thermalZone)
  {
    Model _model = model();
    boost::optional<AirLoopHVAC> t_airLoopHVAC = thermalZone.airLoopHVAC();

    if( ! t_airLoopHVAC )
    {
      return false;
    }

    if( t_airLoopHVAC->handle() != handle() )
    {
      return false;
    }

    thermalZone.removeSupplyPlenum();
    thermalZone.removeReturnPlenum();

    std::vector<ModelObject> modelObjects;
    AirLoopHVACZoneSplitter zoneSplitter = t_airLoopHVAC->zoneSplitter();
    AirLoopHVACZoneMixer zoneMixer = t_airLoopHVAC->zoneMixer();
    boost::optional<ModelObject> splitterOutletObject;
    boost::optional<ModelObject> mixerInletObject;
    std::vector<ModelObject>::iterator findit;

    modelObjects = t_airLoopHVAC->demandComponents(thermalZone,zoneMixer);
    findit = std::find(modelObjects.begin(),modelObjects.end(),zoneMixer);
    mixerInletObject = *(findit - 1);

    modelObjects = t_airLoopHVAC->demandComponents(zoneSplitter,thermalZone);
    findit = std::find(modelObjects.begin(),modelObjects.end(),zoneSplitter);
    splitterOutletObject = *(findit + 1);

    OS_ASSERT(splitterOutletObject);
    OS_ASSERT(mixerInletObject);

    modelObjects = t_airLoopHVAC->demandComponents(splitterOutletObject->cast<HVACComponent>(),mixerInletObject->cast<HVACComponent>());
    
    zoneSplitter.removePortForBranch(zoneSplitter.branchIndexForOutletModelObject(splitterOutletObject.get()));
    zoneMixer.removePortForBranch(zoneMixer.branchIndexForInletModelObject(mixerInletObject.get()));

    for( const auto & modelObject : modelObjects )
    {
      modelObject.cast<HVACComponent>().disconnect();
    }

    for( auto & modelObject : modelObjects )
    {
      if( ! modelObject.optionalCast<ThermalZone>() )
      {
        modelObject.remove();
      }
    }

    std::vector<SetpointManagerSingleZoneReheat> setpointManagers = subsetCastVector<SetpointManagerSingleZoneReheat>(t_airLoopHVAC->supplyOutletNode().setpointManagers());
    if( ! setpointManagers.empty() ) {
      SetpointManagerSingleZoneReheat spm = setpointManagers.front();
      if( spm.controlZone() == thermalZone )
      {
        spm.resetControlZone();
      }
    }

    if( (! zoneSplitter.lastOutletModelObject()) || (! zoneMixer.lastInletModelObject()) )
    {
      Node newNode(_model);

      _model.connect(zoneSplitter,zoneSplitter.nextOutletPort(),newNode,newNode.inletPort());

      _model.connect(newNode,newNode.outletPort(),zoneMixer,zoneMixer.nextInletPort());
    }

    return true;
  }

  AirLoopHVACZoneMixer AirLoopHVAC_Impl::zoneMixer()
  {
    std::vector<AirLoopHVACZoneMixer> mixers = subsetCastVector<AirLoopHVACZoneMixer>(demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneMixer ));
    OS_ASSERT(! mixers.empty());
    return mixers.front();
  }

  AirLoopHVACZoneSplitter AirLoopHVAC_Impl::zoneSplitter()
  {
    std::vector<AirLoopHVACZoneSplitter> splitters = subsetCastVector<AirLoopHVACZoneSplitter>(demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneSplitter ));
    OS_ASSERT(! splitters.empty());
    return splitters.front();
  }

  ModelObject AirLoopHVAC_Impl::clone(Model model) const
  {
    AirLoopHVAC airLoopClone = Loop_Impl::clone(model).cast<AirLoopHVAC>();

    AvailabilityManagerAssignmentList amalClone = availabilityManagerAssignmentList().clone(model).cast<AvailabilityManagerAssignmentList>();

    airLoopClone.setPointer(OS_AirLoopHVACFields::AvailabilityManagerListName,amalClone.handle());

    return airLoopClone;
  }

  std::vector<ModelObject> AirLoopHVAC_Impl::oaComponents(openstudio::IddObjectType type)
  {
    if( airLoopHVACOutdoorAirSystem() )
    {
      return airLoopHVACOutdoorAirSystem()->components();
    }
    else
    {
      return std::vector<ModelObject>();
    }
  }

  boost::optional<Node> AirLoopHVAC_Impl::reliefAirNode() const
  {
    if( airLoopHVACOutdoorAirSystem() )
    {
      return airLoopHVACOutdoorAirSystem()->outboardReliefNode();
    }
    else
    {
      return boost::optional<Node>();
    }
  }

  Node AirLoopHVAC_Impl::supplyOutletNode() const
  {
    return supplyOutletNodes().front();
  }

  Node AirLoopHVAC_Impl::demandInletNode() const
  {
    return demandInletNodes().front();
  }

  std::vector<ModelObject> AirLoopHVAC_Impl::supplyOutletNodesAsModelObjects() {
    ModelObjectVector result = castVector<ModelObject>(supplyOutletNodes());
    return result;
  }

  std::vector<ModelObject> AirLoopHVAC_Impl::demandInletNodesAsModelObjects() {
    ModelObjectVector result = castVector<ModelObject>(demandInletNodes());
    return result;
  }

  boost::optional<ModelObject> AirLoopHVAC_Impl::demandOutletNodeAsModelObject() {
    OptionalModelObject result = demandOutletNode();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVAC_Impl::reliefAirNodeAsModelObject() {
    OptionalModelObject result;
    OptionalNode intermediate = reliefAirNode();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVAC_Impl::zoneMixerAsModelObject() {
    OptionalModelObject result = zoneMixer();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVAC_Impl::zoneSplitterAsModelObject() {
    OptionalModelObject result = zoneSplitter();
    return result;
  }

  Splitter AirLoopHVAC_Impl::demandSplitter()
  {
    return this->zoneSplitter();
  }

  Mixer AirLoopHVAC_Impl::demandMixer()
  {
    return this->zoneMixer();
  }

  bool AirLoopHVAC_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
  {
    boost::optional<StraightComponent> comp;

    return addBranchForZoneImpl(thermalZone,comp);
  }

  bool AirLoopHVAC_Impl::addBranchForZone(ThermalZone & thermalZone, StraightComponent & airTerminal)
  {
    boost::optional<StraightComponent> comp = airTerminal;

    return addBranchForZoneImpl(thermalZone, comp);
  }

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, OptionalStraightComponent airTerminal)
  {
    bool result = true;
    bool complete = false;

    Splitter splitter = zoneSplitter();
    Mixer mixer = zoneMixer();

    if( ! airTerminal )
    {
      std::vector<ModelObject> modelObjects = demandComponents(splitter,mixer);
      // Only clone last air terminal if there is are no plenums
      if( subsetCastVector<AirLoopHVACSupplyPlenum>(modelObjects).empty() &&
          subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects).empty() )
      { 
        boost::optional<StraightComponent> lastAirTerminal = terminalForLastBranch(mixer);
        boost::optional<ThermalZone> lastThermalZone = zoneForLastBranch(mixer);

        if(lastAirTerminal && lastThermalZone)
        {
          boost::optional<PlantLoop> plantLoop = plantForAirTerminal(lastAirTerminal.get());
          Model t_model = model();
          airTerminal = lastAirTerminal->clone(t_model).cast<StraightComponent>();
          if( plantLoop )
          {
            setPlantForAirTerminal(airTerminal.get(),plantLoop.get());
          }
        }
        else if(lastAirTerminal)
        {
          boost::optional<ModelObject> mo = lastAirTerminal->outletModelObject();
          OS_ASSERT(mo);
          boost::optional<Node> node = mo->optionalCast<Node>();
          OS_ASSERT(node);
          result = thermalZone.addToNode(node.get());
          complete = true;
        }
      }
    }

    if( ! complete )
    {
      AirLoopHVAC airLoopHVAC = getObject<AirLoopHVAC>();
      return addBranchForZoneImpl(thermalZone,airLoopHVAC,splitter,mixer,airTerminal);
    }
    else
    {
      return result;
    }
  }

  bool AirLoopHVAC_Impl::addBranchForHVACComponent(HVACComponent hvacComponent)
  {
    Model _model = this->model();

    if( hvacComponent.model() != _model )
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
            if( hvacComponent.addToNode(node.get()) )
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

    if( hvacComponent.addToNode(node) )
    {
      return true;
    }
    else
    {
      node.remove();

      return false;
    }
  }

  SizingSystem AirLoopHVAC_Impl::sizingSystem() const
  {
    boost::optional<SizingSystem> sizingSystem;

    std::vector<SizingSystem> sizingObjects;

    sizingObjects = getObject<AirLoopHVAC>().getModelObjectSources<SizingSystem>();

    for( const auto & sizingObject : sizingObjects )
    {
      try {
        if( sizingObject.airLoopHVAC().handle() == this->handle() )
        {
          sizingSystem = sizingObject;
        }
      }
      catch (...) {
        LOG(Debug,sizingObject.briefDescription() << " is not attached to an AirLoopHVAC object.");
      }
    }

    if( sizingSystem )
    {
      return sizingSystem.get();
    }
    else
    {
      LOG_AND_THROW(briefDescription() << " missing Sizing:System object.");
    }
  }

  std::vector<ThermalZone> AirLoopHVAC_Impl::thermalZones() const
  {
    std::vector<ModelObject> objects = demandComponents(IddObjectType::OS_ThermalZone);
    //std::vector<ModelObject> v = this->demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneSplitter );

    std::vector<ThermalZone> result;

    for( const auto & elem : objects )
    {
      result.push_back(elem.cast<ThermalZone>());
    }

    return result;
  }

  std::vector<ModelObject> AirLoopHVAC_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(sizingSystem());

    return result;
  }

  boost::optional<double> AirLoopHVAC_Impl::designSupplyAirFlowRate() const {
    return getDouble(OS_AirLoopHVACFields::DesignSupplyAirFlowRate,true);
  }

  OSOptionalQuantity AirLoopHVAC_Impl::getDesignSupplyAirFlowRate(bool returnIP) const {
    OptionalDouble value = designSupplyAirFlowRate();
    return getQuantityFromDouble(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, value, returnIP);
  }

  bool AirLoopHVAC_Impl::isDesignSupplyAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void AirLoopHVAC_Impl::setDesignSupplyAirFlowRate(boost::optional<double> designSupplyAirFlowRate) {
    bool result(false);
    if (designSupplyAirFlowRate) {
      result = setDouble(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, designSupplyAirFlowRate.get());
    }
    else {
      resetDesignSupplyAirFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool AirLoopHVAC_Impl::setDesignSupplyAirFlowRate(const OSOptionalQuantity& designSupplyAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (designSupplyAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_AirLoopHVACFields::DesignSupplyAirFlowRate,designSupplyAirFlowRate.get());
      if (value) {
        setDesignSupplyAirFlowRate(value);
        result = true;
      }
    }
    else {
      setDesignSupplyAirFlowRate(value);
      result = true;
    }
    return result;
  }

  void AirLoopHVAC_Impl::resetDesignSupplyAirFlowRate() {
    bool result = setString(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, "");
    OS_ASSERT(result);
  }

  void AirLoopHVAC_Impl::autosizeDesignSupplyAirFlowRate() {
    bool result = setString(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  Schedule AirLoopHVAC_Impl::availabilitySchedule() const
  {
    return availabilityManagerAssignmentList().availabilityManagerScheduled().schedule();
  }
  
  void AirLoopHVAC_Impl::setAvailabilitySchedule(Schedule & schedule)
  {
    availabilityManagerAssignmentList().availabilityManagerScheduled().setSchedule(schedule);

    auto seriesPIUs = subsetCastVector<AirTerminalSingleDuctSeriesPIUReheat>(demandComponents(AirTerminalSingleDuctSeriesPIUReheat::iddObjectType()));
    for( auto & piu : seriesPIUs ) {
      piu.getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }

    auto parallelPIUs = subsetCastVector<AirTerminalSingleDuctParallelPIUReheat>(demandComponents(AirTerminalSingleDuctParallelPIUReheat::iddObjectType()));
    for( auto & piu : parallelPIUs ) {
      piu.getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }
  }
  
  bool AirLoopHVAC_Impl::setNightCycleControlType(std::string controlType)
  {
    return availabilityManagerAssignmentList().availabilityManagerNightCycle().setControlType(controlType);
  }
  
  std::string AirLoopHVAC_Impl::nightCycleControlType() const
  {
    return availabilityManagerAssignmentList().availabilityManagerNightCycle().controlType();
  }

  AvailabilityManagerAssignmentList AirLoopHVAC_Impl::availabilityManagerAssignmentList() const
  {
    boost::optional<WorkspaceObject> wo = getTarget(OS_AirLoopHVACFields::AvailabilityManagerListName);

    OS_ASSERT(wo);

    return wo->cast<AvailabilityManagerAssignmentList>();
  }

  boost::optional<Node> AirLoopHVAC_Impl::mixedAirNode() const
  {
    boost::optional<Node> result;

    if( boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVACOutdoorAirSystem() )
    {
      if( boost::optional<ModelObject> mo = oaSystem->mixedAirModelObject() )
      {
        result = mo->optionalCast<Node>();
      }
    }

    return result;
  }

  boost::optional<HVACComponent> lastFan(const std::vector<ModelObject> & comps)
  {
    boost::optional<HVACComponent> result;

    auto constantVolumeFans = subsetCastVector<FanConstantVolume>(comps);
    if( ! constantVolumeFans.empty() ) {
      result = constantVolumeFans.back();      
    } else {
      auto variableVolumeFans = subsetCastVector<FanVariableVolume>(comps);
      if( ! variableVolumeFans.empty() ) {
        result = variableVolumeFans.back();
      }
    }

    return result;
  }

  boost::optional<HVACComponent> AirLoopHVAC_Impl::supplyFan() const
  {
    boost::optional<HVACComponent> result;

    boost::optional<HVACComponent> start = supplyInletNode();
    if( auto oaSystem = airLoopHVACOutdoorAirSystem() ) {
      start = oaSystem.get();
    }
    OS_ASSERT(start);
    auto end = supplyOutletNode();

    auto t_supplyComponents = supplyComponents(start.get(),end);
    result = lastFan(t_supplyComponents);

    return result;
  }

  boost::optional<HVACComponent> AirLoopHVAC_Impl::returnFan() const
  {
    boost::optional<HVACComponent> result;

    auto start = supplyInletNode();
    auto end = airLoopHVACOutdoorAirSystem();

    if( end ) {
      auto t_supplyComponents = supplyComponents(start,end.get());
      result = lastFan(t_supplyComponents);
    }

    return result;
  }

  boost::optional<HVACComponent> AirLoopHVAC_Impl::reliefFan() const
  {
    boost::optional<HVACComponent> result;

    if( auto oaSystem = airLoopHVACOutdoorAirSystem() )
    {
      auto t_reliefComponents = oaSystem->reliefComponents();
      result = lastFan(t_reliefComponents);
    }

    return result;
  }

} // detail

AirLoopHVAC::AirLoopHVAC(Model& model)
  : Loop(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVAC_Impl>());

  setString(openstudio::OS_AirLoopHVACFields::DesignSupplyAirFlowRate,"AutoSize");

  AvailabilityManagerAssignmentList list(model);

  setPointer(OS_AirLoopHVACFields::AvailabilityManagerListName,list.handle());

  // supply side

  openstudio::model::Node supplyInletNode(model);
  openstudio::model::Node supplyOutletNode(model);

  model.connect( *this,openstudio::OS_AirLoopHVACFields::SupplySideInletNodeName,
                 supplyInletNode,openstudio::OS_NodeFields::InletPort );
  model.connect( supplyInletNode,openstudio::OS_NodeFields::OutletPort,
                 supplyOutletNode,openstudio::OS_NodeFields::InletPort );
  model.connect( supplyOutletNode,openstudio::OS_NodeFields::OutletPort,*this,
                 openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeNames );

  // demand side

  Node demandInletNode(model);
  Node demandOutletNode(model);
  Node branchNode(model);

  model.connect( *this,openstudio::OS_AirLoopHVACFields::DemandSideInletNodeNames,
                 demandInletNode,demandInletNode.inletPort() );
  model.connect( demandOutletNode,demandOutletNode.outletPort(),
                 *this,openstudio::OS_AirLoopHVACFields::DemandSideOutletNodeName );


  openstudio::model::AirLoopHVACZoneSplitter airLoopHVACZoneSplitter(model);
  openstudio::model::AirLoopHVACZoneMixer airLoopHVACZoneMixer(model);

  model.connect( demandInletNode,demandInletNode.outletPort(),
                 airLoopHVACZoneSplitter, airLoopHVACZoneSplitter.inletPort() );

  model.connect( airLoopHVACZoneSplitter, airLoopHVACZoneSplitter.nextOutletPort(),
                 branchNode, branchNode.inletPort() );

  model.connect( branchNode, branchNode.outletPort(),
                 airLoopHVACZoneMixer, airLoopHVACZoneMixer.nextInletPort() );

  model.connect( airLoopHVACZoneMixer,airLoopHVACZoneMixer.outletPort(),
                 demandOutletNode,demandOutletNode.inletPort() );

  // Sizing:System

  SizingSystem sizingSystem(model,*this);
}

AirLoopHVAC::AirLoopHVAC(std::shared_ptr<detail::AirLoopHVAC_Impl> impl)
  : Loop(impl)
{}

Node AirLoopHVAC::supplyInletNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyInletNode();
}

std::vector<Node> AirLoopHVAC::supplyOutletNodes() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletNodes();
}

std::vector<Node> AirLoopHVAC::demandInletNodes() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->demandInletNodes();
}

Node AirLoopHVAC::demandOutletNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->demandOutletNode();
}

void AirLoopHVAC::addAirLoopComp(openstudio::model::ModelObject targetObj, openstudio::model::ModelObject newObj)
{
  getImpl<detail::AirLoopHVAC_Impl>()->addAirLoopComp(targetObj, newObj);
}

std::vector<IdfObject> AirLoopHVAC::remove()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->remove();
}

OptionalAirLoopHVACOutdoorAirSystem AirLoopHVAC::airLoopHVACOutdoorAirSystem() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->airLoopHVACOutdoorAirSystem();
}

AirLoopHVACZoneMixer AirLoopHVAC::zoneMixer()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->zoneMixer();
}

std::vector<Node> AirLoopHVAC::zoneMixerInletNodes()
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

boost::optional<Node> AirLoopHVAC::zoneMixerOutletNode()
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

AirLoopHVACZoneSplitter AirLoopHVAC::zoneSplitter()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->zoneSplitter();
}

boost::optional<Node> AirLoopHVAC::zoneSplitterInletNode(int zoneSplitterIndex)
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

std::vector<Node> AirLoopHVAC::zoneSplitterOutletNodes(int zoneSplitterIndex)
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

bool AirLoopHVAC::addBranchForZone(openstudio::model::ThermalZone & thermalZone,
                                   boost::optional<StraightComponent> optAirTerminal)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZoneImpl( thermalZone, optAirTerminal );
}

ModelObject AirLoopHVAC::clone(Model model) const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->clone( model );
}

std::vector<ModelObject> AirLoopHVAC::oaComponents(openstudio::IddObjectType type)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->oaComponents( type );
}

boost::optional<Node> AirLoopHVAC::outdoorAirNode()
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

boost::optional<Node> AirLoopHVAC::reliefAirNode()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->reliefAirNode();
}

boost::optional<Node> AirLoopHVAC::mixedAirNode()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->mixedAirNode();
}

boost::optional<Node> AirLoopHVAC::returnAirNode()
{
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

boost::optional<Node> AirLoopHVAC::supplySplitterInletNode() {
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

std::vector<Node> AirLoopHVAC::supplySplitterOutletNodes() {
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

std::vector<Node> AirLoopHVAC::supplyMixerInletNodes() {
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

boost::optional<Node> AirLoopHVAC::supplyMixerOutletNode() {
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
}

Node AirLoopHVAC::supplyOutletNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletNode();
}

Node AirLoopHVAC::demandInletNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->demandInletNode();
}


bool AirLoopHVAC::removeBranchForZone(openstudio::model::ThermalZone & thermalZone)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->removeBranchForZone(thermalZone);
}

IddObjectType AirLoopHVAC::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC);
}

bool AirLoopHVAC::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZone(thermalZone);
}

bool AirLoopHVAC::addBranchForZone(ThermalZone & thermalZone, StraightComponent & airTerminal)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZone(thermalZone, airTerminal);
}

bool AirLoopHVAC::addBranchForHVACComponent(HVACComponent component)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForHVACComponent(component);
}

SizingSystem AirLoopHVAC::sizingSystem() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->sizingSystem();
}

std::vector<ThermalZone> AirLoopHVAC::thermalZones() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->thermalZones();
}

boost::optional<double> AirLoopHVAC::designSupplyAirFlowRate() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->designSupplyAirFlowRate();
}

OSOptionalQuantity AirLoopHVAC::getDesignSupplyAirFlowRate(bool returnIP) const {
  return getImpl<detail::AirLoopHVAC_Impl>()->getDesignSupplyAirFlowRate(returnIP);
}

bool AirLoopHVAC::isDesignSupplyAirFlowRateAutosized() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->isDesignSupplyAirFlowRateAutosized();
}

void AirLoopHVAC::setDesignSupplyAirFlowRate(double designSupplyAirFlowRate) {
  getImpl<detail::AirLoopHVAC_Impl>()->setDesignSupplyAirFlowRate(designSupplyAirFlowRate);
}

bool AirLoopHVAC::setDesignSupplyAirFlowRate(const Quantity& designSupplyAirFlowRate) {
  return getImpl<detail::AirLoopHVAC_Impl>()->setDesignSupplyAirFlowRate(designSupplyAirFlowRate);
}

void AirLoopHVAC::resetDesignSupplyAirFlowRate() {
  getImpl<detail::AirLoopHVAC_Impl>()->resetDesignSupplyAirFlowRate();
}

void AirLoopHVAC::autosizeDesignSupplyAirFlowRate() {
  getImpl<detail::AirLoopHVAC_Impl>()->autosizeDesignSupplyAirFlowRate();
}

Schedule AirLoopHVAC::availabilitySchedule() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->availabilitySchedule();
}

void AirLoopHVAC::setAvailabilitySchedule(Schedule & schedule)
{
  getImpl<detail::AirLoopHVAC_Impl>()->setAvailabilitySchedule(schedule);
}

bool AirLoopHVAC::setNightCycleControlType(std::string controlType)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setNightCycleControlType(controlType);
}

std::string AirLoopHVAC::nightCycleControlType() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->nightCycleControlType();
}

boost::optional<HVACComponent> AirLoopHVAC::supplyFan() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyFan();
}

boost::optional<HVACComponent> AirLoopHVAC::returnFan() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->returnFan();
}

boost::optional<HVACComponent> AirLoopHVAC::reliefFan() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->reliefFan();
}

} // model

} // openstudio
