/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../ReverseTranslator.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../../model/AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirLoopHVAC( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirLoopHVAC )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirLoopHVAC");
     return boost::none;
  }

  OptionalModelObject result;
  boost::optional<double> val;
  boost::optional<std::string> optionalString;
  Workspace _workspace = workspaceObject.workspace();

  openstudio::model::AirLoopHVAC airLoopHVAC( m_model );

  boost::optional<std::string> supplyInletNodeName = workspaceObject.getString(AirLoopHVACFields::SupplySideInletNodeName);
  boost::optional<std::string> supplyOutletNodeName = workspaceObject.getString(AirLoopHVACFields::SupplySideOutletNodeNames);
  boost::optional<std::string> demandInletNodeName = workspaceObject.getString(AirLoopHVACFields::DemandSideInletNodeNames);
  boost::optional<std::string> demandOutletNodeName = workspaceObject.getString(AirLoopHVACFields::DemandSideOutletNodeName);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  Node demandInletNode = airLoopHVAC.demandInletNode();
  Node demandOutletNode = airLoopHVAC.demandOutletNode();

  if( supplyInletNodeName ) { supplyInletNode.setName(supplyInletNodeName.get()); }
  if( supplyOutletNodeName ) { supplyOutletNode.setName(supplyOutletNodeName.get()); }
  if( demandInletNodeName ) { demandInletNode.setName(demandInletNodeName.get()); }
  if( demandOutletNodeName ) { demandOutletNode.setName(demandOutletNodeName.get()); }

  optionalString = workspaceObject.getString(AirLoopHVACFields::Name);
  if( optionalString )
  {
    airLoopHVAC.setName(optionalString.get());
  }

  optionalString = workspaceObject.getString(AirLoopHVACFields::DesignSupplyAirFlowRate);
  if( optionalString && istringEqual(optionalString.get(),"AutoSize") )
  {
    airLoopHVAC.autosizeDesignSupplyAirFlowRate();
  }
  else if( (val = workspaceObject.getDouble(AirLoopHVACFields::DesignSupplyAirFlowRate)) )
  {
    airLoopHVAC.setDesignSupplyAirFlowRate(val.get());
  }

  // Go find the supply branch.
  // Currently only supporting one supply branch.
  // Dual ducts are not supported.
  OptionalWorkspaceObject _supplyBranchList;
  OptionalWorkspaceObject _supplyBranch;

  _supplyBranchList = workspaceObject.getTarget(AirLoopHVACFields::BranchListName);
  if( _supplyBranchList )
  {
    _supplyBranch = _supplyBranchList->getExtensibleGroup(0).cast<WorkspaceExtensibleGroup>().getTarget(BranchListExtensibleFields::BranchName);
    if( ! _supplyBranch )
    {
      LOG(Error, _supplyBranchList->briefDescription() << ": Missing supply branch");
    }
    else
    {
      // March through the equipment on the supply branch and convert them.
      for( unsigned i = 0; ! _supplyBranch->getExtensibleGroup(i).empty(); i++ )
      {
        WorkspaceExtensibleGroup eg = _supplyBranch->getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>();
        boost::optional<std::string> componentName = eg.getString(BranchExtensibleFields::ComponentName);
        boost::optional<std::string> componentType = eg.getString(BranchExtensibleFields::ComponentObjectType);
        boost::optional<std::string> componentInletNodeName = eg.getString(BranchExtensibleFields::ComponentInletNodeName);
        boost::optional<std::string> componentOutletNodeName = eg.getString(BranchExtensibleFields::ComponentOutletNodeName);
        boost::optional<WorkspaceObject> wo;
        OptionalNode node;
        OptionalModelObject targetModelObject;

        if( componentName && (componentName.get() != "") && componentType && (componentType.get() != "") )
        {
          IddObjectType iddType(componentType.get());
          wo = _workspace.getObjectByTypeAndName(iddType,componentName.get());
        }

        if( wo )
        {
          targetModelObject = translateAndMapWorkspaceObject( wo.get() );
          if( !targetModelObject)
          {
            LOG(Error, "Error importing object: " << wo->briefDescription() );
            continue;
          }

          if( OptionalHVACComponent hvacComponent = targetModelObject->optionalCast<HVACComponent>() )
          {
            Node node = airLoopHVAC.supplyOutletNode();
            if( hvacComponent->addToNode(node) )
            {
              if( boost::optional<StraightComponent> straightComponent = hvacComponent->optionalCast<StraightComponent>() )
              {
                Node outletNode = straightComponent->outletModelObject()->cast<Node>();
                Node inletNode = straightComponent->inletModelObject()->cast<Node>();
                if( componentOutletNodeName )
                {
                  outletNode.setName(componentOutletNodeName.get());
                }
                if( componentInletNodeName )
                {
                  inletNode.setName(componentInletNodeName.get());
                }
              }
              else if( boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = hvacComponent->optionalCast<AirLoopHVACOutdoorAirSystem>() )
              {
                Node outletNode = oaSystem->mixedAirModelObject()->cast<Node>();
                Node inletNode = oaSystem->returnAirModelObject()->cast<Node>();
                if( componentOutletNodeName )
                {
                  outletNode.setName(componentOutletNodeName.get());
                }
                if( componentInletNodeName )
                {
                  inletNode.setName(componentInletNodeName.get());
                }
              }
            }
          }
        }
        else
        {
          LOG(Error, _supplyBranch->briefDescription() << ": Missing object listed at ComponentName " << i);
        }
      }
    }
  }
  else
  {
    LOG( Error, workspaceObject.briefDescription() << ": Missing supply branch list, "
              << "Supply equipment will be incomplete");
  }

  // March through the zone on the demand side and add branches for them.
  if( demandOutletNodeName )
  {
    // Find the zone mixer for this air loop
    std::vector<WorkspaceObject> _airLoopHVACZoneMixers;
    _airLoopHVACZoneMixers = workspaceObject.workspace().getObjectsByType(IddObjectType::AirLoopHVAC_ZoneMixer);

    boost::optional<WorkspaceObject> _airLoopHVACZoneMixer;
    for( const auto & elem : _airLoopHVACZoneMixers )
    {
      boost::optional<std::string> mixerOutletNodeName;
      mixerOutletNodeName = elem.getString(AirLoopHVAC_ZoneMixerFields::OutletNodeName);

      if( mixerOutletNodeName && mixerOutletNodeName.get() == demandOutletNodeName.get() )
      {
        _airLoopHVACZoneMixer = elem;
        break;
      }
    }
    if( _airLoopHVACZoneMixer )
    {
      for( int i = 2;
           _airLoopHVACZoneMixer->getString(i);
           i++ )
      {

        std::vector<WorkspaceObject> _zoneHVACEquipmentConnections;

        std::string mixerInletNodeName = _airLoopHVACZoneMixer->getString(i).get();

        _zoneHVACEquipmentConnections = _workspace.getObjectsByType(IddObjectType::ZoneHVAC_EquipmentConnections);

        for( const auto & _zoneHVACEquipmentConnection : _zoneHVACEquipmentConnections )
        {

          OptionalString returnAirNodeName = _zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeorNodeListName);
          OptionalString inletAirNodeName = _zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneAirInletNodeorNodeListName);
          OptionalString zoneName = _zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneName);
          OptionalString zoneEquipListName = _zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName);

          OptionalWorkspaceObject _zone;
          OptionalWorkspaceObject _zoneEquipmentList;
          OptionalWorkspaceObject _zoneEquipment;
          OptionalWorkspaceObject _airTerminal;

          if( returnAirNodeName &&
              returnAirNodeName.get() == mixerInletNodeName &&
              zoneName &&
              zoneEquipListName )
          {
            _zone = _workspace.getObjectByTypeAndName(IddObjectType::Zone,*zoneName);

            _zoneEquipmentList = _workspace.getObjectByTypeAndName(IddObjectType::ZoneHVAC_EquipmentList,zoneEquipListName.get());

            if( ! _zone )
            {
              LOG( Error,
                  airLoopHVAC.briefDescription()
                  << " is connected to a zone that does not exist." );

              break;
            }

            if( ! _zoneEquipmentList )
            {
              LOG( Error,
                  _zone->briefDescription()
                  << " does not have a zone equipment list, but it is attached to a loop." );

              break;
            }

            for( int j = 1; (optionalString = _zoneEquipmentList->getString(j)); j = j + 4 )
            {
              boost::optional<std::string> zoneEquipmentName = _zoneEquipmentList->getString(j+1) ;
              // Possible Zone Equipment
              //
              // ZoneHVAC:AirDistributionUnit
              // AirTerminal:SingleDuct:Uncontrolled
              // ZoneHVAC:EnergyRecoveryVentilator
              // ZoneHVAC:FourPipeFanCoil
              // ZoneHVAC:OutdoorAirUnit
              // ZoneHVAC:PackagedTerminalAirConditioner
              // ZoneHVAC:PackagedTerminalHeatPump
              // ZoneHVAC:UnitHeater
              // ZoneHVAC:UnitVentilator
              // ZoneHVAC:VentilatedSlab
              // ZoneHVAC:WaterToAirHeatPump
              // ZoneHVAC:WindowAirConditioner
              // ZoneHVAC:Baseboard:RadiantConvective:Electric
              // ZoneHVAC:Baseboard:RadiantConvective:Water
              // ZoneHVAC:Baseboard:RadiantConvective:Steam
              // ZoneHVAC:Baseboard:Convective:Electric
              // ZoneHVAC:Baseboard:Convective:Water
              // ZoneHVAC:HighTemperatureRadiant
              // ZoneHVAC:LowTemperatureRadiant:VariableFlow
              // ZoneHVAC:LowTemperatureRadiant:ConstantFlow
              // ZoneHVAC:LowTemperatureRadiant:Electric
              // ZoneHVAC:Dehumidifier:DX
              // ZoneHVAC:IdealLoadsAirSystem
              // Fan:ZoneExhaust
              // WaterHeater:HeatPump
              //
              if( zoneEquipmentName )
              {
                if( istringEqual(optionalString.get(),"AirTerminal:SingleDuct:Uncontrolled") )
                {
                  _airTerminal = _workspace.getObjectByTypeAndName(IddObjectType::AirTerminal_SingleDuct_Uncontrolled,zoneEquipmentName.get());

                  break;
                }
                else if( istringEqual(optionalString.get(),"ZoneHVAC:AirDistributionUnit") )
                {
                  boost::optional<WorkspaceObject> _airDistributionUnit =
                    _workspace.getObjectByTypeAndName(IddObjectType::ZoneHVAC_AirDistributionUnit,zoneEquipmentName.get());

                  if( _airDistributionUnit )
                  {
                    boost::optional<std::string> airUnitName;
                    boost::optional<std::string> airUnitType;

                    airUnitType = _airDistributionUnit->getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType);
                    airUnitName = _airDistributionUnit->getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName);

                    if( airUnitName && airUnitType )
                    {
                      _airTerminal = _workspace.getObjectByTypeAndName(IddObjectType(airUnitType.get()),airUnitName.get());
                    }
                  }

                  break;
                }
              }
            }

            OptionalModelObject airTerminalModelObject;
            OptionalSpace space;
            OptionalStraightComponent straightComponent;
            OptionalThermalZone thermalZone;

            if( _airTerminal )
            {
              airTerminalModelObject = translateAndMapWorkspaceObject( _airTerminal.get() );
            }

            if( _zone )
            {
              if( OptionalModelObject mo = translateAndMapWorkspaceObject( _zone.get() ) )
              {
                space = mo->optionalCast<Space>();
              }
            }

            if( space )
            {
              thermalZone = space->thermalZone();
            }

            if( airTerminalModelObject )
            {
              straightComponent = airTerminalModelObject->optionalCast<StraightComponent>();
            }

            bool success = false;

            if( straightComponent && thermalZone )
            {
              success = airLoopHVAC.addBranchForZone(thermalZone.get(),straightComponent.get());
            }
            else if( thermalZone )
            {
              Model m;

              success = airLoopHVAC.addBranchForZone(thermalZone.get(),boost::none);
            }

            if( success )
            {
              if( inletAirNodeName ) { thermalZone->inletPortList().airLoopHVACModelObject()->cast<Node>().setName(inletAirNodeName.get()); }
              if( returnAirNodeName ) { thermalZone->returnAirModelObject()->cast<Node>().setName(returnAirNodeName.get()); }
            }
          }
        }
      }
    }
  }

  return airLoopHVAC;
}

} // energyplus

} // openstudio

