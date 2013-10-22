/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/SizingSystem.hpp>
#include <model/SizingSystem_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/AvailabilityManagerAssignmentList.hpp>
#include <model/AvailabilityManagerAssignmentList_Impl.hpp>
#include <model/AvailabilityManagerScheduled.hpp>
#include <model/AvailabilityManagerScheduled_Impl.hpp>
#include <model/AvailabilityManagerNightCycle.hpp>
#include <model/AvailabilityManagerNightCycle_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/ScheduleYear.hpp>
#include <model/ScheduleYear_Impl.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

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
    modelObjects = this->components();

    for(it = modelObjects.begin();
        it != modelObjects.end();
        it++)
    {
      if( boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>() )
      {
        comp->disconnectAirSide();
      }
      else if( boost::optional<ThermalZone> tz = it->optionalCast<ThermalZone>() )
      {
      }
      else if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
      {
        comp->disconnect();
      }
    }

    sizingSystem().remove();

    availabilityManagerAssignmentList().remove();

    std::vector<openstudio::IdfObject> idfObjects =  ModelObject_Impl::remove();

    for(it = modelObjects.begin();
        it != modelObjects.end();
        it++)
    {
      if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
      {
        if( ! it->optionalCast<ThermalZone>() )
        {
          if( ! it->handle().isNull() )
          {
            it->cast<HVACComponent>().remove();
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

  OptionalAirLoopHVACOutdoorAirSystem AirLoopHVAC_Impl::airLoopHVACOutdoorAirSystem()
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

  bool AirLoopHVAC_Impl::addBranchForZone(ThermalZone & thermalZone, OptionalStraightComponent optAirTerminal)
  {
    Model _model = model();

    if( _model != thermalZone.model() )
    {
      LOG(Error,"Cannot add zone: " << thermalZone.briefDescription() << " as child of :" << briefDescription()
            << ", because they are not in the same Model.");

      return false;
    }

    if( optAirTerminal && _model != optAirTerminal->model() )
    {
      LOG(Error,"Cannot add branch for terminal: " << optAirTerminal->briefDescription()
            << ", because they are not in the same Model.");

      return false;
    }

    if( optAirTerminal && optAirTerminal->airLoopHVAC() )
    {
      LOG(Error,"Cannot add branch for terminal: " << optAirTerminal->briefDescription()
            << ", because it is already attached to an AirLoopHVAC object.");

      return false;
    }

    thermalZone.setUseIdealAirLoads(false);

    if( OptionalAirLoopHVAC airLoopHVAC = thermalZone.airLoopHVAC() )
    {
      airLoopHVAC->removeBranchForZone(thermalZone);
    }

    AirLoopHVACZoneSplitter _zoneSplitter = zoneSplitter();
    AirLoopHVACZoneMixer _zoneMixer = zoneMixer();

    boost::optional<ThermalZone> lastThermalZone = zoneForLastBranch();
    boost::optional<HVACComponent> lastAirTerminal = terminalForLastBranch();

    boost::optional<Node> node;
    boost::optional<HVACComponent> terminal;
    boost::optional<PlantLoop> plant;

    if( ((! lastThermalZone) && (! lastAirTerminal)) )
    {
      boost::optional<ModelObject> mo = _zoneSplitter.outletModelObject(0);
      if( mo )
      {
        node = mo->optionalCast<Node>();
      }
    }
    else if( (! lastThermalZone) && lastAirTerminal )
    {
      terminal = lastAirTerminal->clone(_model).cast<HVACComponent>();
      plant = plantForAirTerminal(lastAirTerminal.get());
      //  setPlantForAirTerminal(lastAirTerminalClone,plantLoop.get());
      lastAirTerminal->remove();

      boost::optional<ModelObject> mo = _zoneSplitter.outletModelObject(0);
      if( mo )
      {
        node = mo->optionalCast<Node>();
      }
    }
    else if( lastThermalZone )
    {
      int nextOutletPort = _zoneSplitter.nextOutletPort();
      int nextInletPort = _zoneMixer.nextInletPort();

      Node newNode(_model);

      _model.connect( _zoneSplitter,
                      nextOutletPort,
                      newNode,
                      newNode.inletPort() );
      _model.connect( newNode,
                      newNode.outletPort(),
                      _zoneMixer,
                      nextInletPort );

      node = newNode;

      if( lastAirTerminal )
      {
        terminal = lastAirTerminal->clone(_model).cast<HVACComponent>();
        plant = plantForAirTerminal(lastAirTerminal.get());
      }
    }

    OS_ASSERT(node);

    if( optAirTerminal )
    {
      if( terminal )
      {
        terminal->remove();
      }

      terminal = optAirTerminal;
    }

    thermalZone.addToNode(node.get());

    if( terminal )
    {
      Node airInletNode = thermalZone.inletPortList().airLoopHVACModelObject()->cast<Node>();

      terminal->addToNode(airInletNode);

      if( plant )
      {
        setPlantForAirTerminal(terminal.get(),plant.get());
      }
    }

    return true;
  }

  boost::optional<PlantLoop> AirLoopHVAC_Impl::plantForAirTerminal( HVACComponent & airTerminal )
  {
    std::vector<WaterToAirComponent> comps = airTerminal.model().getModelObjects<WaterToAirComponent>();

    for( std::vector<WaterToAirComponent>::iterator it = comps.begin();
         it < comps.end();
         it++ )
    {
      if( boost::optional<HVACComponent> comp = it->containingHVACComponent() )
      {
        if( comp.get() == airTerminal )
        {
          if( boost::optional<PlantLoop> plantLoop = it->plantLoop() )
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

    for( std::vector<WaterToAirComponent>::iterator it = comps.begin();
         it < comps.end();
         it++ )
    {
      if( boost::optional<HVACComponent> comp = it->containingHVACComponent() )
      {
        if( comp.get() == airTerminal )
        {
          plantLoop.addDemandBranchForComponent(*it);

          return;
        }
      }
    }
  }

  boost::optional<ThermalZone> AirLoopHVAC_Impl::zoneForLastBranch()
  {
    AirLoopHVACZoneMixer mixer = zoneMixer();

    if( OptionalNode node = mixer.lastInletModelObject()->optionalCast<Node>() )
    {
      return node->inletModelObject()->optionalCast<ThermalZone>();
    }
    else
    {
      return boost::none;
    }
  }

  boost::optional<HVACComponent> AirLoopHVAC_Impl::terminalForLastBranch()
  {
    AirLoopHVACZoneMixer mixer = zoneMixer();

    if( OptionalNode node = mixer.lastInletModelObject()->optionalCast<Node>() )
    {
      if( boost::optional<ThermalZone> zone = node->inletModelObject()->optionalCast<ThermalZone>() )
      {
        boost::optional<Node> zoneInletNode = zone->inletPortList().airLoopHVACModelObject()->optionalCast<Node>();

        OS_ASSERT( zoneInletNode );

        boost::optional<ModelObject> mo = zoneInletNode->inletModelObject();

        OS_ASSERT( mo );

        if( ! mo->optionalCast<AirLoopHVACZoneSplitter>() )
        {
          if( boost::optional<HVACComponent> hvacComponent = mo->optionalCast<HVACComponent>() )
          {
            return hvacComponent;
          }
        }
      }
      else
      {
        boost::optional<ModelObject> mo = node->inletModelObject();

        OS_ASSERT( mo );

        if( ! mo->optionalCast<AirLoopHVACZoneSplitter>() )
        {
          if( boost::optional<HVACComponent> hvacComponent = mo->optionalCast<HVACComponent>() )
          {
            return hvacComponent;
          }
        }
      }
    }

    return boost::none;
  }

  bool AirLoopHVAC_Impl::removeBranchForZone(ThermalZone & thermalZone)
  {
    Model _model = model();

    AirLoopHVACZoneSplitter _zoneSplitter = zoneSplitter();
    AirLoopHVACZoneMixer _zoneMixer = zoneMixer();

    int _nextBranchIndex = _zoneMixer.nextBranchIndex();
    for( int i = 0; i < _nextBranchIndex; i++ )
    {
      HVACComponent outletComponent = _zoneSplitter.outletModelObject(i)->cast<HVACComponent>();
      HVACComponent inletComponent = _zoneMixer.inletModelObject(i)->cast<HVACComponent>();

      std::vector<ModelObject> modelObjects;
      modelObjects = demandComponents( outletComponent,
                                       inletComponent,
                                       IddObjectType::OS_ThermalZone );


      ThermalZone thisBranchThermalZone =
        modelObjects.front().cast<ThermalZone>();
      if( thisBranchThermalZone == thermalZone )
      {
        std::vector<ModelObject> allModelObjects;
        allModelObjects = demandComponents( outletComponent,
                                            inletComponent );
        _zoneSplitter.removePortForBranch(i);
        _zoneMixer.removePortForBranch(i);
        for( std::vector<ModelObject>::iterator it = allModelObjects.begin();
             it < allModelObjects.end();
             it++ )
        {
          it->cast<HVACComponent>().disconnect();
        }

        if( ! _zoneSplitter.lastOutletModelObject() )
        {
          Node newNode(_model);

          _model.connect(_zoneSplitter,_zoneSplitter.nextOutletPort(),newNode,newNode.inletPort());

          _model.connect(newNode,newNode.outletPort(),_zoneMixer,_zoneMixer.nextInletPort());
        }

        std::vector<ModelObject> zoneEquipment = thermalZone.equipment();

        for( std::vector<ModelObject>::iterator it = allModelObjects.begin();
             it < allModelObjects.end();
             it++ )
        {
          if( ! it->optionalCast<ThermalZone>() )
          {
            it->cast<HVACComponent>().remove();
          }
        }

        if( boost::optional<SetpointManagerSingleZoneReheat> spm = supplyOutletNode().getSetpointManagerSingleZoneReheat() )
        {
          if( spm->controlZone() == thermalZone )
          {
            spm->resetControlZone();
          }
        }

        return true;
      }
    }

    return false;
  }

  AirLoopHVACZoneMixer AirLoopHVAC_Impl::zoneMixer()
  {
    Node demandNode = demandOutletNode();
    OptionalModelObject mixer = demandNode.inletModelObject();
    if( !mixer )
    {
      Model _model = model();
      AirLoopHVACZoneMixer airLoopHVACZoneMixer(_model);
      _model.connect(airLoopHVACZoneMixer,
                    openstudio::OS_AirLoopHVAC_ZoneMixerFields::OutletNodeName,
                    demandNode,
                    openstudio::OS_NodeFields::InletPort);


      mixer = airLoopHVACZoneMixer;
    }

    return mixer->cast<AirLoopHVACZoneMixer>();
  }

  AirLoopHVACZoneSplitter AirLoopHVAC_Impl::zoneSplitter()
  {
    OptionalAirLoopHVACZoneSplitter split;
    std::vector<ModelObject> v = this->demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneSplitter );
    if( v.size() == 0)
    {
      Model _model = model();
      std::vector<Node> inletVec = demandInletNodes();
      OS_ASSERT(inletVec.size()==1);
      AirLoopHVACZoneSplitter airLoopHVACZoneSplitter(_model);
      _model.connect(inletVec[0],
                    openstudio::OS_NodeFields::OutletPort,
                    airLoopHVACZoneSplitter,
                    openstudio::OS_AirLoopHVAC_ZoneSplitterFields::InletNodeName);

      return airLoopHVACZoneSplitter;
    }
    else if( v.size() == 1 )
    {
      return v[0].cast<AirLoopHVACZoneSplitter>();
    }
    else
    {
      throw;
    }
    return *split;
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

  boost::optional<Node> AirLoopHVAC_Impl::reliefAirNode()
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

    return addBranchForZone(thermalZone,comp);
  }

  bool AirLoopHVAC_Impl::addBranchForHVACComponent(HVACComponent airTerminal)
  {
    Model _model = this->model();

    if( airTerminal.model() != _model )
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
            if( airTerminal.addToNode(node.get()) )
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

    if( airTerminal.addToNode(node) )
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

    for( std::vector<SizingSystem>::iterator it = sizingObjects.begin();
         it < sizingObjects.end();
         it++ )
    {
      try {
        if( it->airLoopHVAC().handle() == this->handle() )
        {
          sizingSystem = *it;
        }
      }
      catch (...) {
        LOG(Debug,it->briefDescription() << " is not attached to an AirLoopHVAC object.");
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

  std::vector<ThermalZone> AirLoopHVAC_Impl::thermalZones()
  {
    std::vector<ModelObject> objects = demandComponents(IddObjectType::OS_ThermalZone);
    //std::vector<ModelObject> v = this->demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneSplitter );

    std::vector<ThermalZone> result;

    for( std::vector<ModelObject>::iterator it = objects.begin();
         it != objects.end();
         it++ )
    {
      result.push_back(it->cast<ThermalZone>());
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

AirLoopHVAC::AirLoopHVAC(boost::shared_ptr<detail::AirLoopHVAC_Impl> impl)
  : Loop(impl)
{}

Node AirLoopHVAC::supplyInletNode()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyInletNode();
}

std::vector<Node> AirLoopHVAC::supplyOutletNodes()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletNodes();
}

std::vector<Node> AirLoopHVAC::demandInletNodes()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->demandInletNodes();
}

Node AirLoopHVAC::demandOutletNode()
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

OptionalAirLoopHVACOutdoorAirSystem AirLoopHVAC::airLoopHVACOutdoorAirSystem()
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
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZone( thermalZone, optAirTerminal );
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
  // ETH@20111101 Adding to get Ruby bindings building.
  LOG_AND_THROW("Not implemented.");
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

Node AirLoopHVAC::supplyOutletNode()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletNode();
}

Node AirLoopHVAC::demandInletNode()
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

bool AirLoopHVAC::addBranchForHVACComponent(HVACComponent airTerminal)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForHVACComponent(airTerminal);
}

SizingSystem AirLoopHVAC::sizingSystem() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->sizingSystem();
}

std::vector<ThermalZone> AirLoopHVAC::thermalZones()
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

} // model

} // openstudio

