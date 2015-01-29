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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ReturnPath_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/core/Containers.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

// Attempt to add all required setpoint managers upstream of the supply outlet node.
// This algorithm requires there to be a setpoint manager on the supply outlet node.
// This is a good method to consider adding to Model, as a method of AirLoopHVAC class.
void addRequiredSetpointManagers(const AirLoopHVAC & airLoopHVAC)
{
  std::vector<SetpointManager> _supplyOutletSetpointManagers = airLoopHVAC.supplyOutletNode().setpointManagers();
  if( ! _supplyOutletSetpointManagers.empty() )
  {
    auto t_model = airLoopHVAC.model();
    auto supplyComponents = airLoopHVAC.supplyComponents();

    auto fanOrUnitary = airLoopHVAC.supplyFan();

    if( ! fanOrUnitary ) {
      auto airLoopHVACUnitarySystems = subsetCastVector<AirLoopHVACUnitarySystem>(supplyComponents);
      if( ! airLoopHVACUnitarySystems.empty() ) {
        auto unitary = airLoopHVACUnitarySystems.back();
        if( unitary.supplyFan() ) {
          fanOrUnitary = unitary;
        }
      }
    }

    if( ! fanOrUnitary ) {
      auto airLoopHVACUnitaryHeatCoolVAVChangeoverBypass = subsetCastVector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>(supplyComponents);
      if( ! airLoopHVACUnitaryHeatCoolVAVChangeoverBypass.empty() ) {
        fanOrUnitary = airLoopHVACUnitaryHeatCoolVAVChangeoverBypass.back();
      }
    }

    if( ! fanOrUnitary ) {
      auto airLoopHVACUnitaryHeatPumpAirToAir = subsetCastVector<AirLoopHVACUnitaryHeatPumpAirToAir>(supplyComponents);
      if( ! airLoopHVACUnitaryHeatPumpAirToAir.empty() ) {
        fanOrUnitary = airLoopHVACUnitaryHeatPumpAirToAir.back();
      }
    }

    if( ! fanOrUnitary ) {
      auto airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed = subsetCastVector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>(supplyComponents);
      if( ! airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.empty() ) {
        fanOrUnitary = airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.back();
      }
    }

    std::vector<Node> upperNodes;
    std::vector<Node> lowerNodes;
    if( fanOrUnitary )
    {
      upperNodes = subsetCastVector<Node>(airLoopHVAC.supplyComponents(airLoopHVAC.supplyInletNode(),fanOrUnitary.get()));
      upperNodes.erase(upperNodes.begin());
      lowerNodes = subsetCastVector<Node>(airLoopHVAC.supplyComponents(fanOrUnitary.get(),airLoopHVAC.supplyOutletNode()));
      lowerNodes.erase(lowerNodes.end() - 1);
    }
    else
    {
      // Note if we don't have a fan this is going to be a problem for EnergyPlus,
      // but at this point it will be allowed in OS
      lowerNodes = subsetCastVector<Node>(supplyComponents);
      // We should at least have a supply inlet and outlet node
      OS_ASSERT(lowerNodes.size() >= 2);
      lowerNodes.erase(lowerNodes.begin());
      lowerNodes.erase(lowerNodes.end() - 1);
    }

    auto isTemperatureControl = [] ( SetpointManager & spm ) -> bool {
      return istringEqual("Temperature",spm.controlVariable());
    };

    for( auto & upperNode : upperNodes )
    {
      std::vector<SetpointManager> _setpointManagers = upperNode.setpointManagers();
      if( std::find_if(_setpointManagers.begin(),_setpointManagers.end(),isTemperatureControl) == _setpointManagers.end() ) {
        SetpointManagerMixedAir spm(t_model);
        spm.addToNode(upperNode);
        spm.setName(upperNode.name().get() + " OS Default SPM");
      }
    } 

    for( auto & lowerNode : lowerNodes )
    {
      std::vector<SetpointManager> _setpointManagers = lowerNode.setpointManagers();
      if( std::find_if(_setpointManagers.begin(),_setpointManagers.end(),isTemperatureControl) == _setpointManagers.end() ) {
        for( auto _setpointManager : _supplyOutletSetpointManagers )
        {
          SetpointManager spmClone = _setpointManager.clone(t_model).cast<SetpointManager>();
          spmClone.addToNode(lowerNode);
          spmClone.setName(lowerNode.name().get() + " OS Default SPM");
        }
      }
    }

    if( boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem() )
    {
      boost::optional<Node> outboardOANode = oaSystem->outboardOANode(); 
      std::vector<Node> oaNodes = subsetCastVector<Node>(oaSystem->oaComponents());
      if( outboardOANode )
      {
        for( auto & oaNode : oaNodes )
        {
          if( oaNode != outboardOANode.get() )
          {
            std::vector<SetpointManager> _setpointManagers = oaNode.setpointManagers();
            if( std::find_if(_setpointManagers.begin(),_setpointManagers.end(),isTemperatureControl) == _setpointManagers.end() ) {
              SetpointManagerMixedAir spm(t_model);
              spm.addToNode(oaNode);
              spm.setName(oaNode.name().get() + " OS Default SPM");
            }
          }
        }
      }
    }
  }
}

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVAC( AirLoopHVAC & airLoopHVAC )
{
  Model t_model = airLoopHVAC.model();

  addRequiredSetpointManagers(airLoopHVAC);
  
  // Create a new IddObjectType::AirLoopHVAC
  IdfObject idfObject(IddObjectType::AirLoopHVAC);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : airLoopHVAC.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<double> val;
  boost::optional<IdfObject> nightCycleIdf;
  std::string s;
  std::string airLoopHVACName = airLoopHVAC.name().get();

  // Always On Schedule
  Schedule alwaysOn = airLoopHVAC.model().alwaysOnDiscreteSchedule();
  IdfObject alwaysOnIdf = translateAndMapModelObject(alwaysOn).get();

  // Sizing System
  
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  translateAndMapModelObject(sizingSystem);

  // Name

  idfObject.setName(airLoopHVACName);

  std::vector<ModelObject> controllers;
  auto supplyComponents = airLoopHVAC.supplyComponents();

  for( const auto & supplyComponent : supplyComponents )
  {
    boost::optional<ControllerWaterCoil> controller;

    switch(supplyComponent.iddObject().type().value())
    {
      case openstudio::IddObjectType::OS_Coil_Cooling_Water :
      {
        controller = supplyComponent.cast<CoilCoolingWater>().controllerWaterCoil();
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Water :
      {
        controller = supplyComponent.cast<CoilHeatingWater>().controllerWaterCoil();
        break;
      }
      default:
      {
        break;
      }
    }

    if( controller )
    {
      controllers.push_back(controller.get());
    }
  } 

  if( controllers.size() > 0 )
  {
    IdfObject _controllerList(IddObjectType::AirLoopHVAC_ControllerList);
    _controllerList.clearExtensibleGroups();
    _controllerList.setName(airLoopHVAC.name().get() + " Controllers");
    m_idfObjects.push_back(_controllerList);

    idfObject.setString(AirLoopHVACFields::ControllerListName,_controllerList.name().get());

    int i = 1;

    for( auto & elem : controllers )
    {
      boost::optional<IdfObject> _controller = translateAndMapModelObject(elem);

      if( _controller )
      {
        _controllerList.setString(i,_controller->iddObject().name());
        _controllerList.setString(i + 1,_controller->name().get());
        
        i++;
        i++;
      }
    }
  }

  // Availability Manager List Name
  IdfObject availabilityManagerAssignmentListIdf(openstudio::IddObjectType::AvailabilityManagerAssignmentList);
  availabilityManagerAssignmentListIdf.setName(airLoopHVACName + "Availability Manager List");
  m_idfObjects.push_back(availabilityManagerAssignmentListIdf);

  idfObject.setString(openstudio::AirLoopHVACFields::AvailabilityManagerListName,
                      availabilityManagerAssignmentListIdf.name().get());

  Schedule availabilitySchedule = airLoopHVAC.availabilitySchedule();
  boost::optional<IdfObject> availabilityScheduleIdf = translateAndMapModelObject(availabilitySchedule);

  if( ! istringEqual(airLoopHVAC.nightCycleControlType(),"StayOff") )
  {
    nightCycleIdf = IdfObject(openstudio::IddObjectType::AvailabilityManager_NightCycle);
    nightCycleIdf->setName(airLoopHVACName + " NightCycle Manager");
    m_idfObjects.push_back(nightCycleIdf.get());

    nightCycleIdf->setString(openstudio::AvailabilityManager_NightCycleFields::ControlType,airLoopHVAC.nightCycleControlType());
    nightCycleIdf->setString(openstudio::AvailabilityManager_NightCycleFields::ApplicabilityScheduleName,alwaysOnIdf.name().get());

    nightCycleIdf->setDouble(openstudio::AvailabilityManager_NightCycleFields::ThermostatTolerance,1.0);
    nightCycleIdf->setUnsigned(openstudio::AvailabilityManager_NightCycleFields::CyclingRunTime,3600);

    // ForwardTranslateFanX will write set the Fan's availability schedule to AirLoopHVAC::availabilitySchedule() if the fan is connected to an AirLoopHVAC object.
    if( availabilityScheduleIdf )
    {
      nightCycleIdf->setString(openstudio::AvailabilityManager_NightCycleFields::FanScheduleName,availabilityScheduleIdf->name().get());
    }

    availabilityManagerAssignmentListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType,
                                                nightCycleIdf->iddObject().name());
    availabilityManagerAssignmentListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,
                                                nightCycleIdf->name().get());
  }
  else
  {
    IdfObject availabilityManagerScheduledIdf = IdfObject(openstudio::IddObjectType::AvailabilityManager_Scheduled);
    availabilityManagerScheduledIdf.setName(airLoopHVACName + " Availability Manager");
    m_idfObjects.push_back(availabilityManagerScheduledIdf);

    if( availabilityScheduleIdf )
    {
      availabilityManagerScheduledIdf.setString(openstudio::AvailabilityManager_ScheduledFields::ScheduleName,
                                                availabilityScheduleIdf->name().get());
    }

    availabilityManagerAssignmentListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType,
                                                availabilityManagerScheduledIdf.iddObject().name());
    availabilityManagerAssignmentListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,
                                                availabilityManagerScheduledIdf.name().get());
  }

  // Design Supply Air Flow Rate
  if( airLoopHVAC.isDesignSupplyAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::AirLoopHVACFields::DesignSupplyAirFlowRate,"AutoSize");
  }
  else if( (val = airLoopHVAC.designSupplyAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::AirLoopHVACFields::DesignSupplyAirFlowRate,val.get());
  }

  // Branch List Name
  IdfObject branchList(openstudio::IddObjectType::BranchList);
  branchList.setName(airLoopHVACName + " Supply Branches");
  m_idfObjects.push_back(branchList);

  IdfObject branch(openstudio::IddObjectType::Branch);
  branch.setName( airLoopHVACName + " Supply Branch");
  m_idfObjects.push_back(branch);

  branchList.setString(1,branch.name().get());

  branch.setString(openstudio::BranchFields::MaximumFlowRate,"AutoSize");

  // If there is a fan on the mixed air branch,
  // and it is not autosized, use it to size the branch.
  boost::optional<double> maximumFlowRate;

  boost::optional<model::Node> branchInletNode;
  
  if( boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem() )
  {
    branchInletNode = oaSystem->mixedAirModelObject()->cast<Node>();
  }
  else
  {
    branchInletNode = airLoopHVAC.supplyInletNode();
  }

  std::vector<model::FanConstantVolume> constantSpeedfans;
  constantSpeedfans = subsetCastVector<model::FanConstantVolume>(airLoopHVAC.supplyComponents(branchInletNode.get(),
                                                                 airLoopHVAC.supplyOutletNode(),
                                                                 model::FanConstantVolume::iddObjectType()));
  if( constantSpeedfans.size() > 0 )
  {
    maximumFlowRate = constantSpeedfans.back().maximumFlowRate();     
  }

  std::vector<model::FanVariableVolume> variableSpeedfans;
  variableSpeedfans = subsetCastVector<model::FanVariableVolume>(airLoopHVAC.supplyComponents(branchInletNode.get(),
                                                                 airLoopHVAC.supplyOutletNode(),
                                                                 model::FanVariableVolume::iddObjectType()));
  if( variableSpeedfans.size() > 0 )
  {
    maximumFlowRate = variableSpeedfans.back().maximumFlowRate();
  }

  if( maximumFlowRate )
  {
    branch.setDouble(openstudio::BranchFields::MaximumFlowRate,maximumFlowRate.get());
  }

  unsigned branchIndex = 3;

  ModelObjectVector branchComponents;
  branchComponents = airLoopHVAC.supplyComponents( airLoopHVAC.supplyInletNode(),
                                                   airLoopHVAC.supplyOutletNodes().front() );
  ModelObjectVector::iterator branchCompIt;
  for( branchCompIt = branchComponents.begin();
       branchCompIt != branchComponents.end();
       ++branchCompIt )
  {
    boost::optional<IdfObject> branchIdfObject = translateAndMapModelObject(*branchCompIt);
    if( branchIdfObject )
    {
      model::OptionalStraightComponent straightComp = branchCompIt->optionalCast<model::StraightComponent>();
      model::OptionalWaterToAirComponent waterToAirComp = branchCompIt->optionalCast<model::WaterToAirComponent>();
      model::OptionalAirLoopHVACOutdoorAirSystem oaSystemComp = branchCompIt->optionalCast<model::AirLoopHVACOutdoorAirSystem>();
      if( straightComp &&
          !(branchCompIt->optionalCast<Node>()) )
      {
        s = branchIdfObject->iddObject().name();
        branch.setString(branchIndex,s);

        branchIndex++;

        s = branchIdfObject->name().get();
        branch.setString(branchIndex,s);

        branchIndex++;

        temp = straightComp->inletModelObject();
        if(temp)
        {
          optS = temp->name();
          if(optS)
            branch.setString(branchIndex,*optS);
        }
        branchIndex++;


        temp = straightComp->outletModelObject();
        if(temp)
        {
          optS = temp->name();
          if(optS)
            branch.setString(branchIndex,*optS);
        }
        branchIndex++;

        branch.setString(branchIndex,"Passive");

        branchIndex++;
      }
      else if(waterToAirComp)
      {
        s = branchIdfObject->iddObject().name();
        branch.setString(branchIndex,s);

        branchIndex++;

        s = branchIdfObject->name().get();
        branch.setString(branchIndex,s);

        branchIndex++;

        temp = waterToAirComp->airInletModelObject();
        if(temp)
        {
          optS = temp->name();
          if(optS)
            branch.setString(branchIndex,*optS);
        }
        branchIndex++;


        temp = waterToAirComp->airOutletModelObject();
        if(temp)
        {
          optS = temp->name();
          if(optS)
            branch.setString(branchIndex,*optS);
        }
        branchIndex++;

        branch.setString(branchIndex,"Passive");

        branchIndex++;
      }
      else if(oaSystemComp)
      {
        s = branchIdfObject->iddObject().name();
        branch.setString(branchIndex,s);

        branchIndex++;

        s = branchIdfObject->name().get();
        branch.setString(branchIndex,s);

        branchIndex++;

        s = oaSystemComp->returnAirModelObject()->name().get();
        branch.setString(branchIndex,s);

        branchIndex++;

        s = oaSystemComp->mixedAirModelObject()->name().get();
        branch.setString(branchIndex,s);

        branchIndex++;

        branch.setString(branchIndex,"Passive");

        branchIndex++;
      }
    }
  }

  s = branchList.name().get();
  idfObject.setString(openstudio::AirLoopHVACFields::BranchListName,s);

  // Supply Side Inlet Node Name
  idfObject.setString(openstudio::AirLoopHVACFields::SupplySideInletNodeName,
                      airLoopHVAC.supplyInletNode().name().get());

  // Supply Side Outlet Node Names
  idfObject.setString(openstudio::AirLoopHVACFields::SupplySideOutletNodeNames,
                      airLoopHVAC.supplyOutletNodes().begin()->name().get());

  // Demand Side Inlet Node Names
  idfObject.setString(openstudio::AirLoopHVACFields::DemandSideInletNodeNames,
                      airLoopHVAC.demandInletNodes().begin()->name().get());

  // Demand Side Outlet Node Name
  idfObject.setString(openstudio::AirLoopHVACFields::DemandSideOutletNodeName,
                      airLoopHVAC.demandOutletNode().name().get());

  // Convert demand side components
  createAirLoopHVACSupplyPath(airLoopHVAC);
  createAirLoopHVACReturnPath(airLoopHVAC);

  std::vector<ModelObject> demandComponents = airLoopHVAC.demandComponents();
  for( auto & demandComponent : demandComponents )
  {
    translateAndMapModelObject(demandComponent);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

