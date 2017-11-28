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
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
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
#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
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
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "SetpointManagerSingleZoneCooling.hpp"
#include "SetpointManagerSingleZoneCooling_Impl.hpp"
#include "SetpointManagerSingleZoneHeating.hpp"
#include "SetpointManagerSingleZoneHeating_Impl.hpp"
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

  std::vector<ScheduleTypeKey> AirLoopHVAC_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    // UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    // UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    // if (std::find(b,e,OS_AirLoopHVACFields::ReturnAirBypassFlowTemperatureSetpointScheduleName) != e)
    // {
    //   result.push_back(ScheduleTypeKey("AirLoopHVAC","Return Air Bypass Flow Temperature Setpoint"));
    // }
    return result;
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
    if( auto mo = this->connectedObject(openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeA) ) {
      auto node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      nodeVector.push_back(node.get());
    }
    if( auto mo = this->connectedObject(openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeB) ) {
      auto node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      nodeVector.push_back(node.get());
    }
    return nodeVector;
  }

  std::vector<Node> AirLoopHVAC_Impl::demandInletNodes() const
  {
    std::vector<Node> nodeVector;
    if( auto mo = this->connectedObject(openstudio::OS_AirLoopHVACFields::DemandSideInletNodeA) ) {
      auto node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      nodeVector.push_back(node.get());
    }
    if( auto mo = this->connectedObject(openstudio::OS_AirLoopHVACFields::DemandSideInletNodeB) ) {
      auto node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      nodeVector.push_back(node.get());
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

  std::vector<openstudio::IdfObject> AirLoopHVAC_Impl::remove()
  {
    ModelObjectVector modelObjects;
    ModelObjectVector::iterator it;

    sizingSystem().remove();

    if( auto t_availabilityManager = availabilityManager() ) {
      t_availabilityManager->remove();
    }

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
    modelObjects = supplyComponents( openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem );
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

  boost::optional<HVACComponent> AirLoopHVAC_Impl::terminalForLastBranch(Mixer & mixer)
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

        if( ! mo->optionalCast<Splitter>() && ! mo->optionalCast<Mixer>() && ! mo->optionalCast<Node>() )
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

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone,
                                              AirLoopHVAC & airLoopHVAC,
                                              Splitter & splitter,
                                              Mixer & mixer,
                                              OptionalHVACComponent & optAirTerminal)
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

    auto demandInletNodes = t_airLoopHVAC->demandInletNodes();
    for( unsigned i = 0; i < demandInletNodes.size(); ++i ) {
      thermalZone.removeSupplyPlenum(i);
    }
    thermalZone.removeReturnPlenum();

    std::vector<ModelObject> modelObjects;
    AirLoopHVACZoneSplitter zoneSplitter = t_airLoopHVAC->zoneSplitter();
    AirLoopHVACZoneMixer zoneMixer = t_airLoopHVAC->zoneMixer();
    boost::optional<ModelObject> splitterOutletObject;
    boost::optional<ModelObject> mixerInletObject;
    std::vector<ModelObject>::iterator findit;

    // Before we go wrecking the loop, cleanly remove anything that is not a node or zone
    // (ie terminals).  This is important because dual duct terminals especially have to worry about
    // the second duct on their removal.
    modelObjects = t_airLoopHVAC->demandComponents(zoneSplitter,thermalZone);
    for( auto & modelObject : modelObjects ) {
      if( (! modelObject.optionalCast<Node>()) && (! modelObject.optionalCast<ThermalZone>()) ) {
        modelObject.remove();
      }
    }

    modelObjects = t_airLoopHVAC->demandComponents(thermalZone,zoneMixer);
    findit = std::find(modelObjects.begin(),modelObjects.end(),zoneMixer);
    mixerInletObject = *(findit - 1);

    modelObjects = t_airLoopHVAC->demandComponents(zoneSplitter,thermalZone);
    findit = std::find(modelObjects.begin(),modelObjects.end(),zoneSplitter);
    splitterOutletObject = *(findit + 1);

    OS_ASSERT(splitterOutletObject);
    OS_ASSERT(mixerInletObject);

    modelObjects = t_airLoopHVAC->demandComponents(splitterOutletObject->cast<HVACComponent>(),mixerInletObject->cast<HVACComponent>());

    // Do this first because, the zone needs to be connected to airloop
    // in order for disconnect to figure out which port is the airloop port
    thermalZone.disconnect();

    zoneSplitter.removePortForBranch(zoneSplitter.branchIndexForOutletModelObject(splitterOutletObject.get()));
    zoneMixer.removePortForBranch(zoneMixer.branchIndexForInletModelObject(mixerInletObject.get()));

    for( const auto & modelObject : modelObjects ) {
      if( ! modelObject.optionalCast<ThermalZone>() ) {
        modelObject.cast<HVACComponent>().disconnect();
      }
    }

    for( auto & modelObject : modelObjects ) {
      if( ! modelObject.optionalCast<ThermalZone>() ) {
        modelObject.remove();
      }
    }

    {
      std::vector<SetpointManagerSingleZoneReheat> setpointManagers = subsetCastVector<SetpointManagerSingleZoneReheat>(t_airLoopHVAC->supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneReheat spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
      }
    }

    {
      std::vector<SetpointManagerSingleZoneCooling> setpointManagers = subsetCastVector<SetpointManagerSingleZoneCooling>(t_airLoopHVAC->supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneCooling spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
      }
    }

    {
      std::vector<SetpointManagerSingleZoneHeating> setpointManagers = subsetCastVector<SetpointManagerSingleZoneHeating>(t_airLoopHVAC->supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneHeating spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
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

  AirLoopHVACZoneMixer AirLoopHVAC_Impl::zoneMixer() const
  {
    auto result = getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneMixer>(OS_AirLoopHVACFields::DemandMixerName);
    if (result) return result.get();

    std::vector<AirLoopHVACZoneMixer> mixers = subsetCastVector<AirLoopHVACZoneMixer>(demandComponents( IddObjectType::OS_AirLoopHVAC_ZoneMixer ));
    OS_ASSERT(! mixers.empty());
    auto mixer = mixers.front();
    return mixer;
  }

  bool AirLoopHVAC_Impl::setZoneMixer(Mixer const & mixer)
  {
    auto result = setPointer(OS_AirLoopHVACFields::DemandMixerName,mixer.handle());
    return result;
  }

  AirLoopHVACZoneSplitter AirLoopHVAC_Impl::zoneSplitter() const
  {
    auto result = getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneSplitter>(OS_AirLoopHVACFields::DemandSplitterAName);
    if (result) return result.get();

    auto splitters = subsetCastVector<AirLoopHVACZoneSplitter>(demandComponents( demandInletNode(),demandOutletNode(),IddObjectType::OS_AirLoopHVAC_ZoneSplitter ));
    OS_ASSERT(splitters.size() == 1u);
    auto splitter = splitters.front();
    return splitter;
  }

  bool AirLoopHVAC_Impl::setZoneSplitter(Splitter const & splitter, int path)
  {
    bool result = false;
    if (path == 0) {
      result = setPointer(OS_AirLoopHVACFields::DemandSplitterAName,splitter.handle());
    } else {
      result = setPointer(OS_AirLoopHVACFields::DemandSplitterBName,splitter.handle());
    }
    return result;
  }

  void AirLoopHVAC_Impl::resetZoneSplitter(int path)
  {
    bool result = false;
    if (path == 0) {
      result = setString(OS_AirLoopHVACFields::DemandSplitterAName, "");
    } else {
      result = setString(OS_AirLoopHVACFields::DemandSplitterBName, "");
    }
    OS_ASSERT(result);
  }

  std::vector<AirLoopHVACZoneSplitter> AirLoopHVAC_Impl::zoneSplitters() const
  {
    std::vector<AirLoopHVACZoneSplitter> splitters { zoneSplitter() };
    auto t_demandInletNodes = demandInletNodes();
    if( t_demandInletNodes.size() == 2u ) {
      auto result = getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneSplitter>(OS_AirLoopHVACFields::DemandSplitterBName);
      if (result) {
        splitters.push_back(result.get());
        return splitters;
      }

      auto inletNode = t_demandInletNodes[1];
      auto splitters2 = subsetCastVector<AirLoopHVACZoneSplitter>(demandComponents( inletNode,demandOutletNode(),IddObjectType::OS_AirLoopHVAC_ZoneSplitter ));
      OS_ASSERT(splitters2.size() == 1u);
      auto splitter = splitters2.front();
      splitters.push_back(splitter);
    }
    return splitters;
  }


  ModelObject AirLoopHVAC_Impl::clone(Model model) const
  {
    AirLoopHVAC airLoopClone = Loop_Impl::clone(model).cast<AirLoopHVAC>();

    {
      auto clone = availabilitySchedule().clone(model).cast<Schedule>();
      airLoopClone.setPointer(OS_AirLoopHVACFields::AvailabilitySchedule,clone.handle());
    }

    if( auto mo = availabilityManager() ) {
      auto clone = mo->clone(model).cast<AvailabilityManager>();
      airLoopClone.setAvailabilityManager(clone);
    } else {
      airLoopClone.setString(OS_AirLoopHVACFields::AvailabilityManager,"");
    }

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
    boost::optional<HVACComponent> comp;

    return addBranchForZoneImpl(thermalZone,comp);
  }

  bool AirLoopHVAC_Impl::addBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal)
  {
    boost::optional<HVACComponent> comp = airTerminal;

    return addBranchForZoneImpl(thermalZone, comp);
  }

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, OptionalStraightComponent & airTerminal)
  {
    boost::optional<HVACComponent> comp;

    if( airTerminal ) {
      comp = airTerminal->cast<HVACComponent>();
    }

    return addBranchForZoneImpl(thermalZone, comp);
  }

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, OptionalHVACComponent & airTerminal)
  {
    bool result = true;
    bool complete = false;

    Splitter splitter = zoneSplitter();
    Mixer mixer = zoneMixer();

    if( auto currentSystem = thermalZone.airLoopHVAC() ) {
      if( currentSystem->handle() == handle() ) return false;
    }

    if( ! airTerminal )
    {
      std::vector<ModelObject> modelObjects = demandComponents(splitter,mixer);
      // Only clone last air terminal if there is are no plenums
      if( subsetCastVector<AirLoopHVACSupplyPlenum>(modelObjects).empty() &&
          subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects).empty() )
      {
        auto lastAirTerminal = terminalForLastBranch(mixer);
        auto lastThermalZone = zoneForLastBranch(mixer);

        if(lastAirTerminal && lastThermalZone)
        {
          boost::optional<PlantLoop> plantLoop = plantForAirTerminal(lastAirTerminal.get());
          Model t_model = model();
          airTerminal = lastAirTerminal->clone(t_model).cast<HVACComponent>();
          if( plantLoop )
          {
            setPlantForAirTerminal(airTerminal.get(),plantLoop.get());
          }
        }
        else if(lastAirTerminal)
        {
          auto t_comps = demandComponents(lastAirTerminal.get(),demandOutletNode());
          OS_ASSERT(t_comps.size() > 1);
          auto t_comp = t_comps[1];
          boost::optional<Node> node = t_comp.optionalCast<Node>();
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
    auto result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVACFields::AvailabilitySchedule);
    if( ! result ) {
      result = model().alwaysOnDiscreteSchedule();
    }
    OS_ASSERT(result);
    return result.get();
  }

  void AirLoopHVAC_Impl::setAvailabilitySchedule(Schedule & schedule)
  {
    auto result = setPointer(OS_AirLoopHVACFields::AvailabilitySchedule,schedule.handle());
    OS_ASSERT(result);

    auto seriesPIUs = subsetCastVector<AirTerminalSingleDuctSeriesPIUReheat>(demandComponents(AirTerminalSingleDuctSeriesPIUReheat::iddObjectType()));
    for( auto & piu : seriesPIUs ) {
      piu.getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }

    auto parallelPIUs = subsetCastVector<AirTerminalSingleDuctParallelPIUReheat>(demandComponents(AirTerminalSingleDuctParallelPIUReheat::iddObjectType()));
    for( auto & piu : parallelPIUs ) {
      piu.getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }
  }

  bool AirLoopHVAC_Impl::setNightCycleControlType(std::string const & controlType)
  {
    auto createNightCycleManager = [&]() {
      boost::optional<AvailabilityManagerNightCycle> result;
      auto t_model = model();
      AvailabilityManagerNightCycle nightCycle(t_model);
      if( nightCycle.setControlType(controlType) ) {
        result = nightCycle;
      } else {
        nightCycle.remove();
      }
      return result;
    };

    if( auto t_availabilityManager = availabilityManager() ) {
      if( auto nightCycle = t_availabilityManager->optionalCast<AvailabilityManagerNightCycle>() ) {
        return nightCycle->setControlType(controlType);
      } else {
        if( auto nightCycle = createNightCycleManager() ) {
          t_availabilityManager->remove();
          auto result = setAvailabilityManager(nightCycle.get());
          OS_ASSERT(result);
          return true;
        }
      }
    } else {
      if( auto nightCycle = createNightCycleManager() ) {
        auto result = setAvailabilityManager(nightCycle.get());
        OS_ASSERT(result);
        return true;
      }
    }
    return false;
  }

  // boost::optional<Schedule>  AirLoopHVAC_Impl::returnAirBypassFlowTemperatureSetpointSchedule() const
  // {
  //   return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVACFields::ReturnAirBypassFlowTemperatureSetpointScheduleName);
  // }

  // bool  AirLoopHVAC_Impl::setReturnAirBypassFlowTemperatureSetpointSchedule(Schedule & temperatureSetpointSchedule)
  // {
  //   bool result = setSchedule(OS_AirLoopHVACFields::ReturnAirBypassFlowTemperatureSetpointScheduleName,
  //                             "AirLoopHVAC",
  //                             "Return Air Bypass Flow Temperature Setpoint",
  //                             temperatureSetpointSchedule);
  //   return result;
  // }

  // void  AirLoopHVAC_Impl::resetReturnAirBypassFlowTemperatureSetpointSchedule()
  // {
  //   bool result = setString(OS_AirLoopHVACFields::ReturnAirBypassFlowTemperatureSetpointScheduleName,"");
  //   OS_ASSERT(result);
  // }

  std::string AirLoopHVAC_Impl::nightCycleControlType() const
  {
    if( auto t_availabilityManager = availabilityManager() ) {
      if( auto nightCycle = t_availabilityManager->optionalCast<AvailabilityManagerNightCycle>() ) {
        return nightCycle->controlType();
      }
    }
    return "StayOff";
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

  boost::optional<AvailabilityManager> AirLoopHVAC_Impl::availabilityManager() const {
    return getObject<ModelObject>().getModelObjectTarget<AvailabilityManager>(OS_AirLoopHVACFields::AvailabilityManager);
  }

  bool AirLoopHVAC_Impl::setAvailabilityManager(const AvailabilityManager & availabilityManager) {
    auto type = availabilityManager.iddObjectType();
    if( type == IddObjectType::OS_AvailabilityManager_NightCycle ||
        type == IddObjectType::OS_AvailabilityManager_HybridVentilation ||
        type == IddObjectType::OS_AvailabilityManager_NightVentilation ||
        type == IddObjectType::OS_AvailabilityManager_OptimumStart ) {
      return setPointer(OS_AirLoopHVACFields::AvailabilityManager, availabilityManager.handle());
    }
    return false;
  }

  void AirLoopHVAC_Impl::resetAvailabilityManager() {
    bool result = setString(OS_AirLoopHVACFields::AvailabilityManager, "");
    OS_ASSERT(result);
  }

  unsigned AirLoopHVAC_Impl::supplyOutletPortA() const {
    return OS_AirLoopHVACFields::SupplySideOutletNodeA;
  }

  unsigned AirLoopHVAC_Impl::supplyOutletPortB() const {
    return OS_AirLoopHVACFields::SupplySideOutletNodeB;
  }

  unsigned AirLoopHVAC_Impl::supplyInletPort() const {
    return OS_AirLoopHVACFields::SupplySideInletNodeName;
  }

  unsigned AirLoopHVAC_Impl::demandInletPortA() const {
    return OS_AirLoopHVACFields::DemandSideInletNodeA;
  }

  unsigned AirLoopHVAC_Impl::demandInletPortB() const {
    return OS_AirLoopHVACFields::DemandSideInletNodeB;
  }

  unsigned AirLoopHVAC_Impl::demandOutletPort() const {
    return OS_AirLoopHVACFields::DemandSideOutletNodeName;
  }

  boost::optional<Splitter> AirLoopHVAC_Impl::supplySplitter() const {
    auto result = getObject<ModelObject>().getModelObjectTarget<Splitter>(OS_AirLoopHVACFields::SupplySplitterName);
    if (result) return result.get();

    auto splitters = subsetCastVector<Splitter>(supplyComponents());
    if( ! splitters.empty() ) {
      auto splitter = splitters.front();
      return splitter;
    } else {
      return boost::none;
    }
  }

  bool AirLoopHVAC_Impl::setSupplySplitter(Splitter const & splitter)
  {
    auto result = setPointer(OS_AirLoopHVACFields::SupplySplitterName,splitter.handle());
    return result;
  }

  void AirLoopHVAC_Impl::resetSupplySplitter()
  {
    bool result = setString(OS_AirLoopHVACFields::SupplySplitterName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVAC_Impl::removeSupplySplitter()
  {
    auto t_supplySplitter = supplySplitter();

    if( ! t_supplySplitter ) return false;

    auto _model = model();

    auto inletModelObject = t_supplySplitter->inletModelObject();
    OS_ASSERT(inletModelObject);

    auto inletNode = inletModelObject->optionalCast<Node>();
    OS_ASSERT(inletNode);

    auto t_supplyOutletNodes = supplyOutletNodes();
    OS_ASSERT(t_supplyOutletNodes.size() == 2u);

    auto splitterOutletModelObjects = t_supplySplitter->outletModelObjects();
    OS_ASSERT(splitterOutletModelObjects.size() == 2u);

    auto splitterOutletNodes = subsetCastVector<Node>(splitterOutletModelObjects);
    OS_ASSERT(splitterOutletNodes.size() == 2u);

    auto comps0 = supplyComponents(splitterOutletNodes[0],t_supplyOutletNodes[0]);
    OS_ASSERT(comps0.size() >= 1u);
    auto comps1 = supplyComponents(splitterOutletNodes[1],t_supplyOutletNodes[1]);
    OS_ASSERT(comps1.size() >= 1u);

    for( auto it = comps0.begin(); it != comps0.end(); ++it ) {
      it->getImpl<detail::HVACComponent_Impl>()->disconnect();
    }

    for( auto it = comps1.begin(); it != comps1.end(); ++it ) {
      it->getImpl<detail::HVACComponent_Impl>()->disconnect();
    }

    t_supplySplitter->disconnect();

    auto airLoopHVAC = getObject<AirLoopHVAC>();
    if( inletNode.get() == supplyInletNode() ) {
      Node supplyOutletNode(_model);
      _model.connect(inletNode.get(),inletNode->outletPort(),supplyOutletNode,supplyOutletNode.inletPort());
      _model.connect(supplyOutletNode,supplyOutletNode.outletPort(),airLoopHVAC,supplyOutletPortA());
    } else {
      _model.connect(inletNode.get(),inletNode->outletPort(),airLoopHVAC,supplyOutletPortA());
    }

    for( auto it = comps0.begin(); it != comps0.end(); ++it ) {
      if( ! it->handle().isNull() ) {
        it->remove();
      }
    }

    for( auto it = comps1.begin(); it != comps1.end(); ++it ) {
      if( ! it->handle().isNull() ) {
        it->remove();
      }
    }

    t_supplySplitter->remove();

    // reset supply splitter in AirLoopHVAC since it is removed
    resetSupplySplitter();

    return true;
  }

  bool AirLoopHVAC_Impl::removeSupplySplitter(HVACComponent & hvacComponent)
  {
    auto t_supplySplitter = supplySplitter();

    if( ! t_supplySplitter ) return false;

    auto t_supplyOutletNodes = supplyOutletNodes();
    OS_ASSERT(t_supplyOutletNodes.size() == 2u);

    auto splitterOutletModelObjects = t_supplySplitter->outletModelObjects();
    OS_ASSERT(splitterOutletModelObjects.size() == 2u);

    auto splitterOutletNodes = subsetCastVector<Node>(splitterOutletModelObjects);
    OS_ASSERT(splitterOutletNodes.size() == 2u);

    auto systemStartComponent = supplyInletNode();
    boost::optional<HVACComponent> systemEndComponent;
    auto componentInletPort = t_supplySplitter->getImpl<detail::Splitter_Impl>()->inletPort();
    boost::optional<unsigned> componentOutletPort;

    std::vector<ModelObject> removeComps;

    // Remove branch "0" if it contains hvacComponent
    {
      auto comps = supplyComponents(splitterOutletNodes[0],t_supplyOutletNodes[0]);
      OS_ASSERT(! comps.empty());
      if( std::find(comps.begin(),comps.end(),hvacComponent) != comps.end() ) {
        removeComps = comps;
        for( auto it = comps.begin(); it != comps.end(); ++it ) {
          it->getImpl<detail::HVACComponent_Impl>()->disconnect();
        }
        systemEndComponent = t_supplyOutletNodes[1];
        componentOutletPort = t_supplySplitter->getImpl<detail::Splitter_Impl>()->outletPort(1);
      }
    }

    // Remove branch "1" if it contains hvacComponent
    if( (! systemEndComponent) || (! componentOutletPort) ) {
      auto comps = supplyComponents(splitterOutletNodes[1],t_supplyOutletNodes[1]);
      OS_ASSERT(! comps.empty());
      if( std::find(comps.begin(),comps.end(),hvacComponent) != comps.end() ) {
        removeComps = comps;
        for( auto it = comps.begin() + 1; it != comps.end(); ++it ) {
          it->getImpl<detail::HVACComponent_Impl>()->disconnect();
        }
        systemEndComponent = t_supplyOutletNodes[0];
        componentOutletPort = t_supplySplitter->getImpl<detail::Splitter_Impl>()->outletPort(0);
      }
    }

    if( (! systemEndComponent) || (! componentOutletPort) ) return false;

    auto result = t_supplySplitter->getImpl<detail::Splitter_Impl>()->removeFromLoop(systemStartComponent,systemEndComponent.get(),componentInletPort,componentOutletPort.get());

    for( auto & comp : removeComps ) {
      comp.remove();
    }

    // reset supply splitter in AirLoopHVAC since it is removed
    resetSupplySplitter();

    return result;
  }

  boost::optional<Node> AirLoopHVAC_Impl::supplySplitterInletNode() const {
    if( auto splitter = supplySplitter() ) {
      if( auto mo = splitter->inletModelObject() ) {
        return mo->optionalCast<Node>();
      }
    }

    return boost::none;
  }

  std::vector<Node> AirLoopHVAC_Impl::supplySplitterOutletNodes() const {
    if( auto splitter = supplySplitter() ) {
      return subsetCastVector<Node>(splitter->outletModelObjects());
    }

    return std::vector<Node>();
  }

  bool AirLoopHVAC_Impl::removeDualDuctTerminalFromAirLoopHVAC(HVACComponent & terminal, const unsigned inletPortA, const unsigned inletPortB, const unsigned outletPort) {
    bool result = true;

    auto _model = terminal.model();

    auto t_airLoopHVAC = terminal.airLoopHVAC();
    if( ! t_airLoopHVAC ) result = false;

    if( result ) {
      auto demandInletNodes = t_airLoopHVAC->demandInletNodes();
      OS_ASSERT(demandInletNodes.size() == 2u);

      auto zoneSplitters = t_airLoopHVAC->zoneSplitters();
      OS_ASSERT(zoneSplitters.size() == 2u);

      // Assuming two paths, demandInletNodes[0] (path 0) and demandInletNodes[1] (path 1)
      // Disconnect from path 1 first.
      // If there are no other terminals / zones on the path 1, then remove the entire path
      // including any plenum and the path 1 zone splitter.
      // This effectively turns it back into a single duct system on the demand side.
      {
        auto zoneSplitter = zoneSplitters[1];
        auto demandInletNode = demandInletNodes[1];

        auto terminalInletModelObject = terminal.connectedObject(inletPortB);
        OS_ASSERT(terminalInletModelObject);
        auto terminalInletNode = terminalInletModelObject->optionalCast<Node>();
        OS_ASSERT(terminalInletNode);

        // Get comps upstream and downstream of zone splitter for this leg
        // Exclude the zone splitter itself
        auto upstreamComps = t_airLoopHVAC->demandComponents(demandInletNode,zoneSplitter);
        upstreamComps.pop_back();
        auto downstreamComps = t_airLoopHVAC->demandComponents(zoneSplitter,terminalInletNode.get());
        downstreamComps.erase(downstreamComps.begin());
        OS_ASSERT(! downstreamComps.empty());

        boost::optional<AirLoopHVACSupplyPlenum> supplyPlenum;
        auto supplyPlenums = subsetCastVector<AirLoopHVACSupplyPlenum>(downstreamComps);
        if( ! supplyPlenums.empty() ) {
          supplyPlenum = supplyPlenums.front();
        }

        bool removePlenum = false;
        if( supplyPlenum ) {
          if( supplyPlenum->outletModelObjects().size() == 1u ) {
            removePlenum = true;
          }
        }
        bool removeSplitter = false;
        if( (! supplyPlenum) || removePlenum ) {
          if( zoneSplitter.outletModelObjects().size() == 1u ) {
            removeSplitter = true;
          }
        }

        if( supplyPlenum ) {
          auto branchIndex = supplyPlenum->branchIndexForOutletModelObject(terminalInletNode.get());
          supplyPlenum->removePortForBranch(branchIndex);
          if( removePlenum ) {
            supplyPlenum->getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->disconnect();
            supplyPlenum->remove();
          }
        }

        zoneSplitter.removePortForBranch(zoneSplitter.branchIndexForOutletModelObject(downstreamComps.front()));

        terminalInletNode->getImpl<detail::Node_Impl>()->disconnect();
        terminalInletNode->remove();

        if( removeSplitter ) {
          for( auto & comp : upstreamComps ) {
            comp.getImpl<detail::Node_Impl>()->disconnect();
            comp.remove();
          }
          zoneSplitter.remove();

          // reset demand splitter B in AirLoopHVAC since it is removed
          t_airLoopHVAC->getImpl<detail::AirLoopHVAC_Impl>()->resetZoneSplitter(1);
        }
      }

      // Now disconnect path 0
      {
        auto terminalInletModelObject = terminal.connectedObject(inletPortA);
        OS_ASSERT(terminalInletModelObject);
        auto terminalInletNode = terminalInletModelObject->optionalCast<Node>();
        OS_ASSERT(terminalInletNode);

        auto targetModelObject = terminal.connectedObject(outletPort);
        OS_ASSERT(targetModelObject);
        auto targetPort = terminal.connectedObjectPort(outletPort);
        OS_ASSERT(targetPort);


        auto sourceModelObject = terminalInletNode->inletModelObject();
        OS_ASSERT(sourceModelObject);
        auto sourcePort = terminalInletNode->connectedObjectPort(terminalInletNode->getImpl<detail::Node_Impl>()->inletPort());
        OS_ASSERT(sourcePort);

        _model.connect( sourceModelObject.get(),
                        sourcePort.get(),
                        targetModelObject.get(),
                        targetPort.get() );

        terminalInletNode->getImpl<detail::Node_Impl>()->disconnect();
        terminalInletNode->remove();
      }
    }

    for( auto & thermalZone : _model.getConcreteModelObjects<ThermalZone>() )
    {
      auto equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),terminal) != equipment.end() ) {
        thermalZone.removeEquipment(terminal);
        break;
      }
    }

    return result;
  }

  bool AirLoopHVAC_Impl::addDualDuctTerminalToNode(HVACComponent & terminal, const unsigned inletPortA, const unsigned inletPortB, const unsigned outletPort, Node & node) {
    // Initialize result to true,
    // then do a series of checks to make sure we can proceed.
    // If any check does not pass, set result = false, and use that to halt progress.
    bool result = true;

    Model _model = node.model();
    if( _model != terminal.model() ) { result = false; }

    auto t_airLoopHVAC = node.airLoopHVAC();
    if( result && ! t_airLoopHVAC ) { result = false; }

    auto outlet = node.outletModelObject();
    if( result && ! outlet ) {
      OS_ASSERT(outlet);
      result = false;
    }

    auto inlet = node.inletModelObject();
    if( result && ! inlet ) {
      OS_ASSERT(inlet);
      result = false;
    }

    auto sourcePort = node.connectedObjectPort(node.inletPort());
    if( result && ! sourcePort ) {
      OS_ASSERT(sourcePort);
      result = false;
    }

    if( result && ! inlet->optionalCast<Splitter>() ) { result = false; }

    boost::optional<ThermalZone> thermalZone;
    if( result ) {
      if( auto portList = outlet->optionalCast<PortList>()  ) {
        thermalZone = portList->thermalZone();
      }
    }

    if( result && ! (thermalZone || outlet->optionalCast<Mixer>()) ) { result = false; }

    if( result ) {
      Node inletNode(_model);

      _model.connect( inlet.get(),
                      sourcePort.get(),
                      inletNode,
                      inletNode.inletPort() );

      _model.connect( inletNode,
                      inletNode.outletPort(),
                      terminal,
                      inletPortA );

      _model.connect( terminal,
                      outletPort,
                      node,
                      node.inletPort() );

      // Hook up other side of terminal
      auto inletNodes = t_airLoopHVAC->demandInletNodes();
      boost::optional<AirLoopHVACZoneSplitter> zoneSplitter2;

      if( inletNodes.size() == 2 ) {
        auto t_zoneSplitters = t_airLoopHVAC->zoneSplitters();
        OS_ASSERT(t_zoneSplitters.size() == 2u);
        zoneSplitter2 = t_zoneSplitters[1];
      } else {
        // Make a second demand inlet node and zone splitter if required
        Node demandInletNode2(_model);

        _model.connect(t_airLoopHVAC.get(),
          t_airLoopHVAC->getImpl<detail::AirLoopHVAC_Impl>()->demandInletPortB(),
          demandInletNode2,
          demandInletNode2.inletPort());

        zoneSplitter2 = AirLoopHVACZoneSplitter(_model);

        // add demand splitter B in AirLoopHVAC and save to field
        t_airLoopHVAC->getImpl<detail::AirLoopHVAC_Impl>()->setZoneSplitter(zoneSplitter2.get(), 1);

        _model.connect(demandInletNode2,
          demandInletNode2.outletPort(),
          zoneSplitter2.get(),
          zoneSplitter2->inletPort());
      }

      OS_ASSERT(zoneSplitter2);
      Node splitter2OutletNode(_model);

      _model.connect(zoneSplitter2.get(),
        zoneSplitter2->nextOutletPort(),
        splitter2OutletNode,
        splitter2OutletNode.inletPort());

      _model.connect(splitter2OutletNode,
        splitter2OutletNode.outletPort(),
        terminal,
        inletPortB);

      // Add to zone equipment list
      if( thermalZone ) {
        thermalZone->addEquipment(terminal);
      }
    }

    return result;
  }

  bool AirLoopHVAC_Impl::isDualDuct() const {
    return supplySplitter() ? true : false;
  }

} // detail

AirLoopHVAC::AirLoopHVAC(Model& model, bool dualDuct)
  : Loop(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVAC_Impl>());

  setString(openstudio::OS_AirLoopHVACFields::DesignSupplyAirFlowRate,"AutoSize");

  // supply side

  openstudio::model::Node supplyInletNode(model);
  openstudio::model::Node supplyOutletNode(model);

  model.connect( *this,openstudio::OS_AirLoopHVACFields::SupplySideInletNodeName,
                 supplyInletNode,openstudio::OS_NodeFields::InletPort );
  model.connect( supplyInletNode,openstudio::OS_NodeFields::OutletPort,
                 supplyOutletNode,openstudio::OS_NodeFields::InletPort );
  model.connect( supplyOutletNode,openstudio::OS_NodeFields::OutletPort,*this,
                 openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeA );

  // demand side

  Node demandInletNode(model);
  Node demandOutletNode(model);
  Node branchNode(model);

  model.connect( *this,openstudio::OS_AirLoopHVACFields::DemandSideInletNodeA,
                 demandInletNode,demandInletNode.inletPort() );
  model.connect( demandOutletNode,demandOutletNode.outletPort(),
                 *this,openstudio::OS_AirLoopHVACFields::DemandSideOutletNodeName );


  openstudio::model::AirLoopHVACZoneSplitter airLoopHVACZoneSplitter(model);
  getImpl<detail::AirLoopHVAC_Impl>()->setZoneSplitter(airLoopHVACZoneSplitter, 0);
  openstudio::model::AirLoopHVACZoneMixer airLoopHVACZoneMixer(model);
  getImpl<detail::AirLoopHVAC_Impl>()->setZoneMixer(airLoopHVACZoneMixer);

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

  // AvailabilityManagerScheduled
  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
  }

  if( dualDuct ) {
    ConnectorSplitter splitter(model);
    splitter.addToNode(supplyOutletNode);
    getImpl<detail::AirLoopHVAC_Impl>()->setSupplySplitter(splitter);
  }
}

AirLoopHVAC::AirLoopHVAC(std::shared_ptr<detail::AirLoopHVAC_Impl> impl)
  : Loop(std::move(impl))
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

std::vector<IdfObject> AirLoopHVAC::remove()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->remove();
}

OptionalAirLoopHVACOutdoorAirSystem AirLoopHVAC::airLoopHVACOutdoorAirSystem() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->airLoopHVACOutdoorAirSystem();
}

AirLoopHVACZoneMixer AirLoopHVAC::zoneMixer() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->zoneMixer();
}

AirLoopHVACZoneSplitter AirLoopHVAC::zoneSplitter() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->zoneSplitter();
}

std::vector<AirLoopHVACZoneSplitter> AirLoopHVAC::zoneSplitters() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->zoneSplitters();
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

boost::optional<Splitter> AirLoopHVAC::supplySplitter() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->supplySplitter();
}

boost::optional<Node> AirLoopHVAC::supplySplitterInletNode() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->supplySplitterInletNode();
}

std::vector<Node> AirLoopHVAC::supplySplitterOutletNodes() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->supplySplitterOutletNodes();
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

bool AirLoopHVAC::addBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal)
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

bool AirLoopHVAC::setNightCycleControlType(std::string const & controlType)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setNightCycleControlType(controlType);
}

// boost::optional<Schedule> AirLoopHVAC::returnAirBypassFlowTemperatureSetpointSchedule() const
// {
//   return getImpl<detail::AirLoopHVAC_Impl>()->returnAirBypassFlowTemperatureSetpointSchedule();
// }

// bool AirLoopHVAC::setReturnAirBypassFlowTemperatureSetpointSchedule(Schedule & temperatureSetpointSchedule)
// {
//   return getImpl<detail::AirLoopHVAC_Impl>()->setReturnAirBypassFlowTemperatureSetpointSchedule(temperatureSetpointSchedule);
// }

// void AirLoopHVAC::resetReturnAirBypassFlowTemperatureSetpointSchedule()
// {
//   getImpl<detail::AirLoopHVAC_Impl>()->resetReturnAirBypassFlowTemperatureSetpointSchedule();
// }

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

boost::optional<AvailabilityManager> AirLoopHVAC::availabilityManager() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->availabilityManager();
}

bool AirLoopHVAC::setAvailabilityManager(const AvailabilityManager& availabilityManager)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setAvailabilityManager(availabilityManager);
}

void AirLoopHVAC::resetAvailabilityManager()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->resetAvailabilityManager();
}

bool AirLoopHVAC::removeSupplySplitter()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->removeSupplySplitter();
}

bool AirLoopHVAC::removeSupplySplitter(HVACComponent & hvacComponent)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->removeSupplySplitter(hvacComponent);
}

bool AirLoopHVAC::isDualDuct() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->isDualDuct();
}

} // model

} // openstudio
