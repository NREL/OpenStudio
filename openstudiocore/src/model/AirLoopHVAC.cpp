/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"
#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"
#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "AvailabilityManagerNightCycle.hpp"
#include "AvailabilityManagerNightCycle_Impl.hpp"

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
#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_NoReheat_FieldEnums.hxx>
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
    static std::vector<std::string> result{
      "Air System Simulation Cycle On Off Status",
      "HVAC System Solver Iteration Count",
      "Air System Solver Iteration Count",
      "Air System Simulation Maximum Iteration Count",
      "Air System Simulation Iteration Count",
      "Air System Component Model Simulation Calls"
    };
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

    auto zones = thermalZones();
    for ( auto & zone : zones ) {
      removeBranchForZone( zone );
    }

    modelObjects = components();

    std::vector<openstudio::IdfObject> idfObjects =  ModelObject_Impl::remove();

    for(it = modelObjects.begin();
        it != modelObjects.end();
        ++it)
    {
      if( OptionalHVACComponent comp = it->optionalCast<HVACComponent>() )
      {
        if( ! it->handle().isNull() ) {
          comp->disconnect();
          comp->remove();
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
    if( auto node = mixer.lastInletModelObject()->optionalCast<Node>() ) {
      auto nodeinlet = node->inletModelObject();
      OS_ASSERT(nodeinlet);
      auto pl = nodeinlet->optionalCast<PortList>();
      if ( pl ) {
        return pl->thermalZone();
      }
    }
    return boost::none;
  }

  boost::optional<HVACComponent> AirLoopHVAC_Impl::terminalForLastBranch(Mixer & mixer)
  {
    auto mixerInletNode = mixer.lastInletModelObject()->optionalCast<Node>();
    if ( mixerInletNode ) {
      auto upstreamComp = mixerInletNode->inletModelObject();
      OS_ASSERT(upstreamComp);
      auto pl = upstreamComp->optionalCast<PortList>();
      if ( pl ) {
        auto zone = pl->thermalZone();
        auto zoneInletNodes = subsetCastVector<Node>(zone.inletPortList().airLoopHVACModelObjects());
        auto airloop = mixer.airLoopHVAC();
        if ( ! airloop ) { return boost::none; }
        auto h = airloop->handle();
        for ( auto & zoneNode : zoneInletNodes ) {
          auto airloop = zoneNode.airLoopHVAC();
          OS_ASSERT(airloop);
          if ( h == airloop->handle() ) {
            auto mo = zoneNode.inletModelObject();
            OS_ASSERT(mo);
            if ( ! mo->optionalCast<Splitter>() ) {
              auto hvacComponent = mo->optionalCast<HVACComponent>();
              if ( hvacComponent ) {
                return hvacComponent;
              }
            }

          }
        }
      } else if ( ! upstreamComp->optionalCast<Splitter>() && ! upstreamComp->optionalCast<Mixer>() && ! upstreamComp->optionalCast<Node>() ) {
        if ( auto hvacComponent = upstreamComp->optionalCast<HVACComponent>() ) {
          return hvacComponent;
        }
      }
    }

    return boost::none;
  }

  bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone,
                                              AirLoopHVAC & airLoopHVAC,
                                              Splitter & splitter,
                                              Mixer & mixer,
                                              bool removeCurrentZones,
                                              OptionalHVACComponent & optAirTerminal)
  {
    Model _model = thermalZone.model();

    if( (optAirTerminal && _model != optAirTerminal->model()) ||
         _model != splitter.model() ||
         _model != mixer.model() )
    {
      return false;
    }

    if( optAirTerminal && optAirTerminal->airLoopHVAC() ) {
      return false;
    }

    if( thermalZone.isPlenum() ) {
      return false;
    }

    if( ! airLoopHVAC.demandComponent(splitter.handle()) ) {
      return false;
    }

    if( ! airLoopHVAC.demandComponent(mixer.handle()) ) {
      return false;
    }

    auto currentloops = thermalZone.airLoopHVACs();
    auto h = airLoopHVAC.handle();
    for ( const auto & loop : currentloops ) {
      if( h == loop.handle() ) {
        return false;
      }
    }

    auto lastThermalZone = zoneForLastBranch(mixer);
    auto lastAirTerminal = terminalForLastBranch(mixer);
    auto modelObjects = airLoopHVAC.demandComponents(splitter,mixer);
    auto hasSupplyPlenum = ! subsetCastVector<AirLoopHVACSupplyPlenum>(modelObjects).empty();
    auto hasReturnPlenum = ! subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects).empty();

    // If all of these conditions are met, then clone the "last" air terminal
    if ( ( ! optAirTerminal ) && ( ! hasSupplyPlenum ) && ( ! hasReturnPlenum ) && lastThermalZone && lastAirTerminal ) {
      // clone the air terminal
      Model t_model = airLoopHVAC.model();
      auto airTerminal = lastAirTerminal->clone(t_model).cast<HVACComponent>();
      optAirTerminal = airTerminal;

      // Reconnect the cloned terminal to the plant loop(s)

      // Special case for FourPipeInduction for now, which has both a cooling and heating plantLoop
      if ( lastAirTerminal->iddObjectType() == IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction ) {

        // Safe to directly cast
        AirTerminalSingleDuctConstantVolumeFourPipeInduction lastAtuFourPipe = lastAirTerminal->cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
        AirTerminalSingleDuctConstantVolumeFourPipeInduction newAtuFourPipe = airTerminal.cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();

        // If the original ATU's heating coil has a plant loop, reconnect it here
        if (boost::optional<PlantLoop> _heatingPl = lastAtuFourPipe.heatingCoil().plantLoop() ){
          _heatingPl->addDemandBranchForComponent(newAtuFourPipe.heatingCoil());
        }

        // If the original ATU has a cooling coil, if it's a CoilCoolingWater, and the cooling coil has a plantLoop, reconnect it here
        if (lastAtuFourPipe.coolingCoil()) {
          if (boost::optional<CoilCoolingWater> _lastCC = lastAtuFourPipe.coolingCoil()->optionalCast<CoilCoolingWater>() ) {
            if (boost::optional<PlantLoop> _coolingPl = _lastCC->plantLoop()) {
              _coolingPl->addDemandBranchForComponent(newAtuFourPipe.coolingCoil().get());
            }
          }
        }

      // Special case for FourPipeInduction, which has both optional cooling and heating coils, that can or cannot be linked to some PlantLoops
      } else if ( lastAirTerminal->iddObjectType() == IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam ) {

        // Safe to directly cast (checked iddObjectType already)
        AirTerminalSingleDuctConstantVolumeFourPipeBeam lastAtuFourPipeBeam = lastAirTerminal->cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();
        AirTerminalSingleDuctConstantVolumeFourPipeBeam newAtuFourPipeBeam = airTerminal.cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

        boost::optional<PlantLoop> _loop;

        // If the original ATU has a cooling coil, and the cooling coil has a plantLoop, reconnect it here
        boost::optional<HVACComponent> _hc = lastAtuFourPipeBeam.heatingCoil();
        if ( _hc && (_loop = _hc->plantLoop()) ) {
          boost::optional<HVACComponent> _hcClone = newAtuFourPipeBeam.heatingCoil();
          // FourPipeBeam::clone should clone and re-set the coils, so it should have worked
          OS_ASSERT(_hcClone);
          _loop->addDemandBranchForComponent(*_hcClone);
        }

        // If the original ATU has a cooling coil, and the cooling coil has a plantLoop, reconnect it here
        boost::optional<HVACComponent> _cc = lastAtuFourPipeBeam.coolingCoil();
        if ( _cc && (_loop = _cc->plantLoop()) ) {
          boost::optional<HVACComponent> _ccClone = newAtuFourPipeBeam.coolingCoil();
          // FourPipeBeam::clone should clone and re-set the coils, so it should have worked
          OS_ASSERT(_ccClone);
          _loop->addDemandBranchForComponent(*_ccClone);
        }

        // Special case for CooledBeam, which isn't a HVAComponent but a StraightComponent
      } else if (lastAirTerminal->iddObjectType() == IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam) {

        // Safe to directly cast
        AirTerminalSingleDuctConstantVolumeCooledBeam lastAtuCooledBeam = lastAirTerminal->cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
        AirTerminalSingleDuctConstantVolumeCooledBeam newAtuCooledBeam = airTerminal.cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();

        // If the original ATU's coilCoolingCooledBeam has a plant loop, reconnect it here
        if (boost::optional<PlantLoop> _coolingPl = lastAtuCooledBeam.coilCoolingCooledBeam().plantLoop() ){
          _coolingPl->addDemandBranchForComponent(newAtuCooledBeam.coilCoolingCooledBeam());
        }

      } else {

        // Default (old) behavior should work here, only a possible heating coil
        boost::optional<PlantLoop> plantLoop = plantForAirTerminal(lastAirTerminal.get());

        if( plantLoop ) {
          setPlantForAirTerminal(airTerminal,plantLoop.get());
        }
      }
    }

    thermalZone.setUseIdealAirLoads(false);

    boost::optional<Node> node;
    std::vector<ModelObject> objects = airLoopHVAC.demandComponents(splitter,mixer);

    if ( lastAirTerminal && (! lastThermalZone) ) {
      auto t_comps = airLoopHVAC.demandComponents(lastAirTerminal.get(), airLoopHVAC.demandOutletNode());
      OS_ASSERT(t_comps.size() > 1);
      auto t_comp = t_comps[1];
      node = t_comp.optionalCast<Node>();
      OS_ASSERT(node);
    } else if ( objects.size() == 3u ) {
      node = objects[1].optionalCast<Node>();
    }

    if ( ! node ) {
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

    if ( removeCurrentZones ) {
      thermalZone.addToNode(node.get());
    } else {
      thermalZone.multiAddToNode(node.get());
    }

    if ( optAirTerminal ) {
      optAirTerminal->addToNode(node.get());
    }

    return true;
  }

  boost::optional<PlantLoop> AirLoopHVAC_Impl::plantForAirTerminal( HVACComponent & airTerminal )
  {
    // TODO: two problems
    // One is that terminals such as AirTerminalSingleDuctConstantVolumeFourPipeInduction actually
    // have both a heating and a cooling coil...
    // The other is the CooledBeam terminal has a CoilCoolingCooledBeam that is a StraightComponent, no a WaterToAir one.
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

  bool AirLoopHVAC_Impl::setPlantForAirTerminal( HVACComponent & airTerminal, PlantLoop & plantLoop )
  {
    std::vector<WaterToAirComponent> comps = airTerminal.model().getModelObjects<WaterToAirComponent>();

    for( const auto & elem : comps )
    {
      if( boost::optional<HVACComponent> comp = elem.containingHVACComponent() )
      {
        if( comp.get() == airTerminal )
        {
          return plantLoop.addDemandBranchForComponent(elem);
        }
      }
    }
    return false;
  }

  bool AirLoopHVAC_Impl::removeBranchForZone(ThermalZone & thermalZone)
  {
    Model _model = model();

    auto splitter = zoneSplitter();
    auto mixer = zoneMixer();
    auto airloop = getObject<AirLoopHVAC>();

    // Before we go wrecking the loop, cleanly remove anything that is not a node or zone
    // (ie terminals).  This is important because dual duct terminals especially have to worry about
    // the second duct on their removal.
    auto modelObjects = subsetCastVector<HVACComponent>(demandComponents(splitter,thermalZone));
    if ( modelObjects.empty() ) {
      return false;
    }

    for( unsigned i = 0; i < demandInletNodes().size(); ++i ) {
      thermalZone.removeSupplyPlenum(airloop, i);
    }
    thermalZone.removeReturnPlenum(airloop);

    for( auto & modelObject : modelObjects ) {
      if( (! modelObject.optionalCast<Node>()) && (! modelObject.optionalCast<ThermalZone>()) ) {
        modelObject.remove();
      }
    }

    modelObjects = subsetCastVector<HVACComponent>(demandComponents(thermalZone,mixer));
    auto mixerInletObject = modelObjects[modelObjects.size() - 2].cast<HVACComponent>();
    auto zoneOutletObject = modelObjects[1].cast<HVACComponent>();

    modelObjects = subsetCastVector<HVACComponent>(demandComponents(splitter,thermalZone));
    auto splitterOutletObject = modelObjects[1].cast<HVACComponent>();
    auto zoneInletObject = modelObjects[modelObjects.size() - 2].cast<HVACComponent>();

    modelObjects = subsetCastVector<HVACComponent>(demandComponents(splitterOutletObject,mixerInletObject));

    auto inletPortList = thermalZone.inletPortList();
    auto returnPortList = thermalZone.returnPortList();

    auto inletportindex = inletPortList.portIndexForModelObject(zoneInletObject);
    inletPortList.getImpl<model::detail::PortList_Impl>()->removePort(inletPortList.port(inletportindex));

    auto returnportindex = returnPortList.portIndexForModelObject(zoneOutletObject);
    returnPortList.getImpl<model::detail::PortList_Impl>()->removePort(returnPortList.port(returnportindex));

    splitter.removePortForBranch(splitter.branchIndexForOutletModelObject(splitterOutletObject));
    mixer.removePortForBranch(mixer.branchIndexForInletModelObject(mixerInletObject));

    for ( auto & modelObject : modelObjects ) {
      if ( modelObject.optionalCast<ThermalZone>() ) {
        // This is to reset the cache
        modelObject.getImpl<model::detail::HVACComponent_Impl>()->m_airLoopHVAC = boost::none;
      } else {
        modelObject.remove();
      }
    }

    {
      std::vector<SetpointManagerSingleZoneReheat> setpointManagers = subsetCastVector<SetpointManagerSingleZoneReheat>(supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneReheat spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
      }
    }

    {
      std::vector<SetpointManagerSingleZoneCooling> setpointManagers = subsetCastVector<SetpointManagerSingleZoneCooling>(supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneCooling spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
      }
    }

    {
      std::vector<SetpointManagerSingleZoneHeating> setpointManagers = subsetCastVector<SetpointManagerSingleZoneHeating>(supplyOutletNode().setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerSingleZoneHeating spm = setpointManagers.front();
        if( spm.controlZone() == thermalZone )
        {
          spm.resetControlZone();
        }
      }
    }

    if ( (! splitter.lastOutletModelObject()) || (! mixer.lastInletModelObject()) ) {
      Node newNode(_model);
      _model.connect(splitter,splitter.nextOutletPort(),newNode,newNode.inletPort());
      _model.connect(newNode,newNode.outletPort(),mixer,mixer.nextInletPort());
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


  ModelObject AirLoopHVAC_Impl::clone(Model model) const {
    AirLoopHVAC airLoopClone = Loop_Impl::clone(model).cast<AirLoopHVAC>();

    airLoopClone.setString(supplyInletPort(),"");
    airLoopClone.setString(supplyOutletPortA(),"");
    airLoopClone.setString(supplyOutletPortB(),"");
    airLoopClone.setString(demandInletPortA(),"");
    airLoopClone.setString(demandInletPortB(),"");
    airLoopClone.setString(demandOutletPort(),"");

    // Sizing:System was already cloned because it is declared as a child
    // And because it has the setParent method overriden, no need to do anything

    {
      auto clone = availabilitySchedule().clone(model).cast<Schedule>();
      airLoopClone.setPointer(OS_AirLoopHVACFields::AvailabilitySchedule,clone.handle());
    }

    {
      AvailabilityManagerAssignmentList avmListClone = availabilityManagerAssignmentList().clone(model).cast<AvailabilityManagerAssignmentList>();
      avmListClone.setName(airLoopClone.name().get() + " AvailabilityManagerAssigmentList");
      airLoopClone.setPointer(OS_AirLoopHVACFields::AvailabilityManagerListName, avmListClone.handle());
    }

    airLoopClone.getImpl<detail::AirLoopHVAC_Impl>()->createTopology();

    auto supplyComps = supplyComponents();
    auto outletNodeClone = airLoopClone.supplyOutletNode();
    std::vector<Node> supplyNodes;

    for( const auto & comp : supplyComps ) {
      if( comp.iddObjectType() == Node::iddObjectType() ) {
        supplyNodes.push_back(comp.cast<Node>());
      } else {
        auto compClone = comp.clone(model).cast<HVACComponent>();
        compClone.addToNode(outletNodeClone);
        // If the original component was also on a PlantLoop
        if( boost::optional<HVACComponent> hvacComp = comp.optionalCast<HVACComponent>() ) {
          if( boost::optional<PlantLoop> pl = hvacComp->plantLoop() ) {
            // Connect the clone to the plantLoop too
            pl->addDemandBranchForComponent(compClone);
          }
        }
      }
    }

    auto supplyNodeClones = subsetCastVector<Node>(airLoopClone.supplyComponents());

    OS_ASSERT( supplyNodes.size() == supplyNodeClones.size() );
    for ( size_t i = 0; i < supplyNodes.size(); ++i ) {
      const auto node = supplyNodes[i];
      auto cloneNode = supplyNodeClones[i];

      auto spms = node.setpointManagers();
      for ( const auto & spm : spms ) {
        auto spmclone = spm.clone(model).cast<SetpointManager>();
        spmclone.addToNode(cloneNode);
      }
    }

    auto terms = terminals();
    std::vector<IddObjectType> termtypes;

    std::for_each(terms.begin(), terms.end(), [&termtypes](const HVACComponent & comp) {
      termtypes.push_back(comp.iddObjectType());
    });

    // std::unique only works on sorted vectors, need to sort
    std::sort(termtypes.begin(), termtypes.end());
    auto uniquetypes = std::vector<IddObjectType>(termtypes.begin(), std::unique(termtypes.begin(), termtypes.end()));
    std::vector<HVACComponent> uniqueterms;

    for ( const auto & type : uniquetypes ) {
      auto it = std::find_if(terms.begin(), terms.end(), [&type](const HVACComponent & comp) {
        return (type ==  comp.iddObjectType());
      });
      if ( it != terms.end() ) {
        uniqueterms.push_back(*it);
      }
    }

    for( const auto & term : uniqueterms ) {
      auto termclone = term.clone(model).cast<HVACComponent>();
      airLoopClone.addBranchForHVACComponent(termclone);
    }

    return airLoopClone;
  }

  std::vector<HVACComponent> AirLoopHVAC_Impl::terminals() const {
    auto demandComps = subsetCastVector<HVACComponent>(demandComponents());

    auto notterminal = [](const HVACComponent & comp) {
      bool result = false;
      const auto type = comp.iddObjectType();

      if( type == Node::iddObjectType() ) {
        result = true;
      } else if( type == ThermalZone::iddObjectType() ) {
        result = true;
      } else if( type == AirLoopHVACZoneSplitter::iddObjectType() ) {
        result = true;
      } else if( type == AirLoopHVACZoneMixer::iddObjectType() ) {
        result = true;
      } else if( type == AirLoopHVACSupplyPlenum::iddObjectType() ) {
        result = true;
      } else if( type == AirLoopHVACReturnPlenum::iddObjectType() ) {
        result = true;
      }

      return result;
    };

    auto end = std::remove_if(demandComps.begin(), demandComps.end(), notterminal);

    return std::vector<HVACComponent>(demandComps.begin(), end);
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

  boost::optional<Node> AirLoopHVAC_Impl::outdoorAirNode() const
  {
    if( airLoopHVACOutdoorAirSystem() )
    {
      return airLoopHVACOutdoorAirSystem()->outboardOANode();
    }
    else
    {
      return boost::optional<Node>();
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

  Splitter AirLoopHVAC_Impl::demandSplitter() const
  {
    return this->zoneSplitter();
  }

  Mixer AirLoopHVAC_Impl::demandMixer() const
  {
    return this->zoneMixer();
  }

  bool AirLoopHVAC_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
  {
    boost::optional<HVACComponent> comp;

    // If the add is successful we will remove existing loops
    auto loops = thermalZone.airLoopHVACs();

    auto thisloop = getObject<AirLoopHVAC>();
    auto splitter = thisloop.zoneSplitter();
    auto mixer = thisloop.zoneMixer();
    auto result = addBranchForZoneImpl(thermalZone, thisloop, splitter, mixer, true, comp);

    //if ( result ) {
    //  for ( auto & loop : loops ) {
    //    loop.removeBranchForZone(thermalZone);
    //  }
    //}

    return result;
  }

  bool AirLoopHVAC_Impl::addBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal)
  {
    boost::optional<HVACComponent> comp = airTerminal;

    // If the add is successful we will remove existing loops
    auto loops = thermalZone.airLoopHVACs();

    auto thisloop = getObject<AirLoopHVAC>();
    auto splitter = thisloop.zoneSplitter();
    auto mixer = thisloop.zoneMixer();
    auto result = addBranchForZoneImpl(thermalZone, thisloop, splitter, mixer, true, comp);

    //if ( result ) {
    //  for ( auto & loop : loops ) {
    //    loop.removeBranchForZone(thermalZone);
    //  }
    //}

    return result;
  }

  bool AirLoopHVAC_Impl::multiAddBranchForZone(ThermalZone & thermalZone)
  {
    boost::optional<HVACComponent> comp;

    auto thisloop = getObject<AirLoopHVAC>();
    auto splitter = thisloop.zoneSplitter();
    auto mixer = thisloop.zoneMixer();

    auto result = addBranchForZoneImpl(thermalZone, thisloop, splitter, mixer, false, comp);

    return result;
  }

  bool AirLoopHVAC_Impl::multiAddBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal)
  {
    boost::optional<HVACComponent> comp = airTerminal;

    auto thisloop = getObject<AirLoopHVAC>();
    auto splitter = thisloop.zoneSplitter();
    auto mixer = thisloop.zoneMixer();

    auto result = addBranchForZoneImpl(thermalZone, thisloop, splitter, mixer, false, comp);

    return result;
  }

  // TODO: REMOVE!
  //bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, OptionalStraightComponent & airTerminal)
  //{
  //  boost::optional<HVACComponent> comp;

  //  if( airTerminal ) {
  //    comp = airTerminal->cast<HVACComponent>();
  //  }

  //  // If the add is successful we will remove existing loops
  //  // This is the only "Impl" version ofr addBranchForZone that does remove
  //  // because the only place this method is called is from AirLoopHVAC::addBranchForZone(...)
  //  auto loops = thermalZone.airLoopHVACs();
  //  auto result = addBranchForZoneImpl(thermalZone, comp);

  //  if ( result ) {
  //    for ( auto & loop : loops ) {
  //      loop.removeBranchForZone(thermalZone);
  //    }
  //  }

  //  return result;
  //}

  //bool AirLoopHVAC_Impl::addBranchForZoneImpl(ThermalZone & thermalZone, OptionalHVACComponent & airTerminal)
  //{
  //  auto currentloops = thermalZone.airLoopHVACs();
  //  auto h = handle();
  //  for ( const auto & loop : currentloops ) {
  //    if( h == loop.handle() ) {
  //      return false;
  //    }
  //  }

  //  bool result = true;
  //  bool complete = false;

  //  Splitter splitter = zoneSplitter();
  //  Mixer mixer = zoneMixer();

  //  if( ! airTerminal )
  //  {
  //    std::vector<ModelObject> modelObjects = demandComponents(splitter,mixer);
  //    // Only clone last air terminal if there are no plenums
  //    if( subsetCastVector<AirLoopHVACSupplyPlenum>(modelObjects).empty() &&
  //        subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects).empty() )
  //    {
  //      auto lastAirTerminal = terminalForLastBranch(mixer);
  //      auto lastThermalZone = zoneForLastBranch(mixer);

  //      if(lastAirTerminal && lastThermalZone)
  //      {
  //        // clone the air terminal
  //        Model t_model = model();
  //        airTerminal = lastAirTerminal->clone(t_model).cast<HVACComponent>();

  //        // Reconnect the cloned terminal to the plant loop(s)

  //        // TODO: (Temporary?) Ugly hack for FourPipeInduction for now, which has both a cooling and heating plantLoop
  //        if (lastAirTerminal->iddObjectType() == IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction) {

  //          // Safe to directly cast
  //          AirTerminalSingleDuctConstantVolumeFourPipeInduction lastAtuFourPipe = lastAirTerminal->cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  //          AirTerminalSingleDuctConstantVolumeFourPipeInduction newAtuFourPipe = airTerminal->cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();

  //          // If the original ATU's heating coil has a plant loop, reconnect it here
  //          if (boost::optional<PlantLoop> _heatingPl = lastAtuFourPipe.heatingCoil().plantLoop() ){
  //            _heatingPl->addDemandBranchForComponent(newAtuFourPipe.heatingCoil());
  //          }

  //          // If the original ATU has a cooling coil, if it's a CoilCoolingWater, and the cooling coil has a plantLoop, reconnect it here
  //          if (lastAtuFourPipe.coolingCoil()) {
  //            if (boost::optional<CoilCoolingWater> _lastCC = lastAtuFourPipe.coolingCoil()->cast<CoilCoolingWater>() ) {
  //              if (boost::optional<PlantLoop> _coolingPl = _lastCC->plantLoop()) {
  //                _coolingPl->addDemandBranchForComponent(newAtuFourPipe.coolingCoil().get());
  //              }
  //            }
  //          }

  //          // TODO: Another ugly hack for CooledBeam, which isn't a HVAComponent but a StraightComponent
  //        } else if (lastAirTerminal->iddObjectType() == IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam) {

  //          // Safe to directly cast
  //          AirTerminalSingleDuctConstantVolumeCooledBeam lastAtuCooledBeam = lastAirTerminal->cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
  //          AirTerminalSingleDuctConstantVolumeCooledBeam newAtuCooledBeam = airTerminal->cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();

  //          // If the original ATU's coilCoolingCooledBeam has a plant loop, reconnect it here
  //          if (boost::optional<PlantLoop> _coolingPl = lastAtuCooledBeam.coilCoolingCooledBeam().plantLoop() ){
  //            _coolingPl->addDemandBranchForComponent(newAtuCooledBeam.coilCoolingCooledBeam());
  //          }

  //        } else {

  //          // Default (old) behavior should work here, only a possible heating coil
  //          boost::optional<PlantLoop> plantLoop = plantForAirTerminal(lastAirTerminal.get());

  //          if( plantLoop ) {

  //            setPlantForAirTerminal(airTerminal.get(),plantLoop.get());
  //          }
  //        }
  //      }
  //      else if(lastAirTerminal)
  //      {
  //        auto t_comps = demandComponents(lastAirTerminal.get(),demandOutletNode());
  //        OS_ASSERT(t_comps.size() > 1);
  //        auto t_comp = t_comps[1];
  //        boost::optional<Node> node = t_comp.optionalCast<Node>();
  //        OS_ASSERT(node);
  //        result = thermalZone.addToNode(node.get());
  //        complete = true;
  //      }
  //    }
  //  }

  //  if( ! complete )
  //  {
  //    AirLoopHVAC airLoopHVAC = getObject<AirLoopHVAC>();
  //    return addBranchForZoneImpl(thermalZone,airLoopHVAC,splitter,mixer,airTerminal);
  //  }
  //  else
  //  {
  //    return result;
  //  }
  //}

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

  bool AirLoopHVAC_Impl::setDesignSupplyAirFlowRate(boost::optional<double> designSupplyAirFlowRate) {
    bool result(false);
    if (designSupplyAirFlowRate) {
      result = setDouble(OS_AirLoopHVACFields::DesignSupplyAirFlowRate, designSupplyAirFlowRate.get());
    }
    else {
      resetDesignSupplyAirFlowRate();
      result = true;
    }
    OS_ASSERT(result);
    return result;
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

  bool AirLoopHVAC_Impl::setAvailabilitySchedule(Schedule & schedule)
  {
    // TODO: deal with this in regards to the new AvailabilityManagerAssignmentList
    // Actually, no, this is going to end up in the Fan Schedule
    auto result = setPointer(OS_AirLoopHVACFields::AvailabilitySchedule,schedule.handle());
    OS_ASSERT(result);

    auto seriesPIUs = subsetCastVector<AirTerminalSingleDuctSeriesPIUReheat>(demandComponents(AirTerminalSingleDuctSeriesPIUReheat::iddObjectType()));
    for( auto & piu : seriesPIUs ) {
      result &= piu.getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }

    auto parallelPIUs = subsetCastVector<AirTerminalSingleDuctParallelPIUReheat>(demandComponents(AirTerminalSingleDuctParallelPIUReheat::iddObjectType()));
    for( auto & piu : parallelPIUs ) {
      result &= piu.getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setFanAvailabilitySchedule(schedule);
    }

    return result;
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

    // Loop through the current AVMs, if you find an AVM NightCycle, set its controlType
    // TODO: @kbenne: Unless the controType is "StayOff" in which case perhaps remove? what do you think?
    for (auto & t_availabilityManager: availabilityManagers()) {
      if( auto nightCycle = t_availabilityManager.optionalCast<AvailabilityManagerNightCycle>() ) {
        /*
         *if ( openstudio::istringEqual(controlType, "StayOff") ) {
         *  nightCycle->remove()
         *  return true
         *} else {
         *  return nightCycle->setControlType(controlType);
         *}
         */
        return nightCycle->setControlType(controlType);

      }
    }

    // Else, add one
    if( auto nightCycle = createNightCycleManager() ) {
      auto result = addAvailabilityManager(nightCycle.get());
      OS_ASSERT(result);
      return true;
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
    // Loop through the current AVMs, if you find an AVM NightCycle, get its controlType
    for (auto & t_availabilityManager: availabilityManagers()) {
      if( auto nightCycle = t_availabilityManager.optionalCast<AvailabilityManagerNightCycle>() ) {
        return nightCycle->controlType();
      }
    }
    // Otherwise, return StayOff
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

  boost::optional<Node> AirLoopHVAC_Impl::returnAirNode() const
  {
    boost::optional<Node> result;

    if( boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVACOutdoorAirSystem() )
    {
      if( boost::optional<ModelObject> mo = oaSystem->returnAirModelObject() )
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



  AvailabilityManagerAssignmentList AirLoopHVAC_Impl::availabilityManagerAssignmentList() const
  {
    boost::optional<AvailabilityManagerAssignmentList> avmList = getObject<ModelObject>().getModelObjectTarget<AvailabilityManagerAssignmentList>(OS_AirLoopHVACFields::AvailabilityManagerListName);
    if (avmList) {
      return avmList.get();
    } else {
      LOG_AND_THROW(briefDescription() << "doesn't have an AvailabilityManagerAssignementList assigned, which shouldn't happen");
    }
  }


  bool AirLoopHVAC_Impl::addAvailabilityManager(const AvailabilityManager & availabilityManager)
  {

    // All types should be allowed here except HybridVentilation (special, stand-alone)
    // Except that OS diverged from E+ (where HybridVentilation is the one referencing the AirLoopHVAC)
    // In OS, you can add it to the AVM list (forward translation handles it)
    // TODO: perhaps should prevent the user from assigned twice an AVM:NightCycle? Or more generally twice the same
    return availabilityManagerAssignmentList().addAvailabilityManager(availabilityManager);
  }

  bool AirLoopHVAC_Impl::addAvailabilityManager(const AvailabilityManager & availabilityManager, unsigned priority)
  {
    return availabilityManagerAssignmentList().addAvailabilityManager(availabilityManager, priority);
  }

  std::vector<AvailabilityManager> AirLoopHVAC_Impl::availabilityManagers() const
  {
    // TODO: add the AVM Scheduled and NigthCycle?
    return availabilityManagerAssignmentList().availabilityManagers();
  }

  bool AirLoopHVAC_Impl::setAvailabilityManagers(const std::vector<AvailabilityManager> & avms)
  {
    // TODO: should this affect the AVM Scheduled and NightCycle?
    return availabilityManagerAssignmentList().setAvailabilityManagers(avms);
  }

    void AirLoopHVAC_Impl::resetAvailabilityManagers()
  {
    // TODO: should this affect the AVM Scheduled and NightCycle?
    availabilityManagerAssignmentList().resetAvailabilityManagers();
  }


  bool AirLoopHVAC_Impl::setAvailabilityManagerPriority(const AvailabilityManager & availabilityManager, unsigned priority)
  {
    return availabilityManagerAssignmentList().setAvailabilityManagerPriority(availabilityManager, priority);
  }

  unsigned AirLoopHVAC_Impl::availabilityManagerPriority(const AvailabilityManager & availabilityManager) const
  {
    return availabilityManagerAssignmentList().availabilityManagerPriority(availabilityManager);
  }

  bool AirLoopHVAC_Impl::removeAvailabilityManager(const AvailabilityManager& avm) {
    return availabilityManagerAssignmentList().removeAvailabilityManager(avm);
  }
  bool AirLoopHVAC_Impl::removeAvailabilityManager(unsigned priority) {
    return availabilityManagerAssignmentList().removeAvailabilityManager(priority);
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

  boost::optional<double> AirLoopHVAC_Impl::autosizedDesignSupplyAirFlowRate() const {
    return getAutosizedValue("Design Supply Air Flow Rate", "m3/s");
  }

  void AirLoopHVAC_Impl::autosize() {
    autosizeDesignSupplyAirFlowRate();
  }

  void AirLoopHVAC_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignSupplyAirFlowRate();
    if (val) {
      setDesignSupplyAirFlowRate(val.get());
    }

  }

  std::vector<EMSActuatorNames> AirLoopHVAC_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{{"AirLoopHVAC", "Availability Status"}}; //takes value 0,1,2,3
    return actuators;
  }

  std::vector<std::string> AirLoopHVAC_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types;
    return types;
  }

  void AirLoopHVAC_Impl::createTopology() {
    auto m = model();
    auto obj = getObject<model::AirLoopHVAC>();

    // supply side
    openstudio::model::Node supplyInletNode(m);
    openstudio::model::Node supplyOutletNode(m);

    m.connect( obj,openstudio::OS_AirLoopHVACFields::SupplySideInletNodeName,
               supplyInletNode,openstudio::OS_NodeFields::InletPort );
    m.connect( supplyInletNode,openstudio::OS_NodeFields::OutletPort,
               supplyOutletNode,openstudio::OS_NodeFields::InletPort );
    m.connect( supplyOutletNode,openstudio::OS_NodeFields::OutletPort,obj,
                   openstudio::OS_AirLoopHVACFields::SupplySideOutletNodeA );

    // demand side
    Node demandInletNode(m);
    Node demandOutletNode(m);
    Node branchNode(m);

    m.connect( obj,openstudio::OS_AirLoopHVACFields::DemandSideInletNodeA,
               demandInletNode,demandInletNode.inletPort() );
    m.connect( demandOutletNode,demandOutletNode.outletPort(),
                   obj,openstudio::OS_AirLoopHVACFields::DemandSideOutletNodeName );


    openstudio::model::AirLoopHVACZoneSplitter airLoopHVACZoneSplitter(m);
    setZoneSplitter(airLoopHVACZoneSplitter, 0);
    openstudio::model::AirLoopHVACZoneMixer airLoopHVACZoneMixer(m);
    setZoneMixer(airLoopHVACZoneMixer);

    m.connect( demandInletNode,demandInletNode.outletPort(),
               airLoopHVACZoneSplitter, airLoopHVACZoneSplitter.inletPort() );

    m.connect( airLoopHVACZoneSplitter, airLoopHVACZoneSplitter.nextOutletPort(),
               branchNode, branchNode.inletPort() );

    m.connect( branchNode, branchNode.outletPort(),
               airLoopHVACZoneMixer, airLoopHVACZoneMixer.nextInletPort() );

    m.connect( airLoopHVACZoneMixer,airLoopHVACZoneMixer.outletPort(),
                   demandOutletNode,demandOutletNode.inletPort() );
  }

} // detail

AirLoopHVAC::AirLoopHVAC(Model& model, bool dualDuct)
  : Loop(iddObjectType(),model)
{
  auto impl = getImpl<detail::AirLoopHVAC_Impl>();
  OS_ASSERT(impl);

  impl->createTopology();


  if( dualDuct ) {
    auto n = supplyOutletNode();
    ConnectorSplitter splitter(model);
    splitter.addToNode(n);
    impl->setSupplySplitter(splitter);
  }

  setString(openstudio::OS_AirLoopHVACFields::DesignSupplyAirFlowRate,"AutoSize");

  // Sizing:System
  SizingSystem sizingSystem(model,*this);

  // AvailabilityManagerScheduled
  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
  }

  // AvailabilityManagerAssignmentList
  AvailabilityManagerAssignmentList avmList(*this);
  setPointer(OS_AirLoopHVACFields::AvailabilityManagerListName, avmList.handle());
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
  if ( optAirTerminal ) {
    return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZone( thermalZone, optAirTerminal.get() );
  } else {
    return getImpl<detail::AirLoopHVAC_Impl>()->addBranchForZone( thermalZone );
  }
}

bool AirLoopHVAC::multiAddBranchForZone(ThermalZone & thermalZone)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->multiAddBranchForZone(thermalZone);
}

bool AirLoopHVAC::multiAddBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->multiAddBranchForZone(thermalZone, airTerminal);
}

ModelObject AirLoopHVAC::clone(Model model) const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->clone( model );
}

std::vector<ModelObject> AirLoopHVAC::oaComponents(openstudio::IddObjectType type)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->oaComponents( type );
}

boost::optional<Node> AirLoopHVAC::outdoorAirNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->outdoorAirNode();
}

boost::optional<Node> AirLoopHVAC::reliefAirNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->reliefAirNode();
}

boost::optional<Node> AirLoopHVAC::mixedAirNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->mixedAirNode();
}

boost::optional<Node> AirLoopHVAC::returnAirNode() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->returnAirNode();
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

bool AirLoopHVAC::setDesignSupplyAirFlowRate(double designSupplyAirFlowRate) {
  return getImpl<detail::AirLoopHVAC_Impl>()->setDesignSupplyAirFlowRate(designSupplyAirFlowRate);
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

bool AirLoopHVAC::setAvailabilitySchedule(Schedule & schedule)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setAvailabilitySchedule(schedule);
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


/* Prefered way to interact with the Availability Managers */
std::vector<AvailabilityManager> AirLoopHVAC::availabilityManagers() const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->availabilityManagers();
}

bool AirLoopHVAC::setAvailabilityManagers(const std::vector<AvailabilityManager> & avms)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setAvailabilityManagers(avms);
}

void AirLoopHVAC::resetAvailabilityManagers()
{
  return getImpl<detail::AirLoopHVAC_Impl>()->resetAvailabilityManagers();
}

bool AirLoopHVAC::addAvailabilityManager(const AvailabilityManager & availabilityManager)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addAvailabilityManager(availabilityManager);
}
// End prefered way


bool AirLoopHVAC::addAvailabilityManager(const AvailabilityManager & availabilityManager, unsigned priority)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->addAvailabilityManager(availabilityManager, priority);
}

boost::optional<double> AirLoopHVAC::autosizedDesignSupplyAirFlowRate() const {
  return getImpl<detail::AirLoopHVAC_Impl>()->autosizedDesignSupplyAirFlowRate();
}

unsigned AirLoopHVAC::availabilityManagerPriority(const AvailabilityManager & availabilityManager) const
{
  return getImpl<detail::AirLoopHVAC_Impl>()->availabilityManagerPriority(availabilityManager);
}

bool AirLoopHVAC::setAvailabilityManagerPriority(const AvailabilityManager & availabilityManager, unsigned priority)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->setAvailabilityManagerPriority(availabilityManager, priority);
}

bool AirLoopHVAC::removeAvailabilityManager(const AvailabilityManager & availabilityManager)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->removeAvailabilityManager(availabilityManager);
}

bool AirLoopHVAC::removeAvailabilityManager(unsigned priority)
{
  return getImpl<detail::AirLoopHVAC_Impl>()->removeAvailabilityManager(priority);
}

// TODO: START DEPRECATED SECTION

  boost::optional<AvailabilityManager> AirLoopHVAC::availabilityManager() const {
    boost::optional<AvailabilityManager> avm;
    std::vector<AvailabilityManager> avmVector = availabilityManagers();
    if (avmVector.size() > 0) {
      avm = avmVector[0];
    }
    return avm;
  }

  bool AirLoopHVAC::setAvailabilityManager(const AvailabilityManager& availabilityManager) {
    std::vector<AvailabilityManager> avmVector;
    avmVector.push_back(availabilityManager);
    return setAvailabilityManagers(avmVector);
  }
  void AirLoopHVAC::resetAvailabilityManager() {
    resetAvailabilityManagers();
  }


// END DEPRECATED

} // model

} // openstudio
