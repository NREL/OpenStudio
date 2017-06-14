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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/AvailabilityManager.hpp"
#include "../../model/AvailabilityManager_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/SizingPlant.hpp"
#include "../../model/SizingPlant_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Splitter.hpp"
#include "../../model/Splitter_Impl.hpp"
#include "../../model/Mixer.hpp"
#include "../../model/Mixer_Impl.hpp"
#include "../../model/PumpVariableSpeed.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/BoilerHotWater_Impl.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/ChillerElectricEIR_Impl.hpp"
#include "../../model/ChillerAbsorption.hpp"
#include "../../model/ChillerAbsorption_Impl.hpp"
#include "../../model/ChillerAbsorptionIndirect.hpp"
#include "../../model/ChillerAbsorptionIndirect_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/WaterHeaterHeatPump_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/CoolingTowerVariableSpeed.hpp"
#include "../../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../../model/CoolingTowerTwoSpeed.hpp"
#include "../../model/CoolingTowerTwoSpeed_Impl.hpp"
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include "../../model/WaterToWaterComponent.hpp"
#include "../../model/WaterToWaterComponent_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboard.hpp"
#include "../../model/CoilHeatingWaterBaseboard_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant_Impl.hpp"
#include "../../model/CoilCoolingCooledBeam.hpp"
#include "../../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../../model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "../../model/CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../../model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManagerScheduledDualSetpoint.hpp"
#include "../../model/SetpointManagerScheduledDualSetpoint_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/ConnectorList_FieldEnums.hxx>
#include <utilities/idd/Connector_Splitter_FieldEnums.hxx>
#include <utilities/idd/Connector_Mixer_FieldEnums.hxx>
#include <utilities/idd/Pipe_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_ComponentSetpoint_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/Sizing_Plant_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_CooledBeam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

IdfObject ForwardTranslator::populateBranch( IdfObject & branchIdfObject, 
                          std::vector<ModelObject> & modelObjects,
                          Loop & loop,
                          bool isSupplyBranch)
{
  if(modelObjects.size() > 0)
  {
    int i = 0;

    for( auto & modelObject : modelObjects )
    {
      boost::optional<Node> inletNode;
      boost::optional<Node> outletNode;
      std::string objectName;
      std::string iddType;
    
      //translate and map each model object
      //in most cases, the name and idd object type come directly from the resulting idfObject
      if( boost::optional<IdfObject> idfObject = this->translateAndMapModelObject(modelObject) ) {
        objectName = idfObject->name().get();
        iddType = idfObject->iddObject().name();
      }

      if( modelObject.optionalCast<Node>() ) {
        // Skip nodes we don't want them showing up on branches
        continue;
      }

      if( auto straightComponent = modelObject.optionalCast<StraightComponent>() )
      {
        inletNode = straightComponent->inletModelObject()->optionalCast<Node>();
        outletNode = straightComponent->outletModelObject()->optionalCast<Node>();
        //special case for ZoneHVAC:Baseboard:Convective:Water.  In E+, this object appears on both the 
        //zonehvac:equipmentlist and the branch.  In OpenStudio, this object was broken into 2 objects:
        //ZoneHVACBaseboardConvectiveWater and CoilHeatingWaterBaseboard.  The ZoneHVAC goes onto the zone and
        //has a child coil that goes onto the plantloop.  In order to get the correct translation to E+, we need
        //to put the name of the containing ZoneHVACBaseboardConvectiveWater onto the branch.
        if (boost::optional<CoilHeatingWaterBaseboard> coilBB = modelObject.optionalCast<CoilHeatingWaterBaseboard>() )
        {
          if (boost::optional<ZoneHVACComponent> contZnBB = coilBB->containingZoneHVACComponent())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfContZnBB = this->translateAndMapModelObject(*contZnBB) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfContZnBB->name().get();
              iddType = idfContZnBB->iddObject().name();
            }
          }
        }
        //special case for ZoneHVAC:Baseeboard:RadiantConvective:Water.  In E+, this object appears on both the 
        //zonehvac:equipmentlist and the branch.  In OpenStudio, this object was broken into 2 objects:
        //ZoneHVACBaseboardRadiantConvectiveWater and CoilHeatingWaterBaseboardRadiant.  The ZoneHVAC goes onto the zone and
        //has a child coil that goes onto the plantloop.  In order to get the correct translation to E+, we need
        //to put the name of the containing ZoneHVACBaseboardRadiantConvectiveWater onto the branch.
        if ( auto coilBBRad = modelObject.optionalCast<CoilHeatingWaterBaseboardRadiant>() )
        {
          if ( auto contZnBBRad = coilBBRad->containingZoneHVACComponent() )
          {
            //translate and map containingZoneHVACBBRadConvWater
            if ( auto idfContZnBBRad = this->translateAndMapModelObject(*contZnBBRad) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfContZnBBRad->name().get();
              iddType = idfContZnBBRad->iddObject().name();
            }
          }
        }
        //special case for AirTerminalSingleDuctConstantVolumeChilledBeam
        if (boost::optional<CoilCoolingCooledBeam> coilCB = modelObject.optionalCast<CoilCoolingCooledBeam>() )
        {
          if (boost::optional<StraightComponent> airTerm = coilCB->containingStraightComponent())
          {  
            boost::optional<IdfObject> idfAirDistUnit = this->translateAndMapModelObject(*airTerm);
            //translate and map containingStraightComponent
            if (idfAirDistUnit)
            {
              //Get the name and idd type of the air terminal inside the air distribution unit
              objectName = idfAirDistUnit->getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get();
              iddType = idfAirDistUnit->getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get();
            }
          }
        }
        //special case for ZoneHVAC:LowTemperatureRadiant:ConstantFlow and ZoneHVAC:LowTemperatureRadiant:VariableFlow.  In E+, this object appears on both the 
        //zonehvac:equipmentlist and the branch.  In OpenStudio, this object was broken into 2 objects:
        //ZoneHVACBaseboardConvectiveWater and CoilHeatingWaterBaseboard.  The ZoneHVAC goes onto the zone and
        //has a child coil that goes onto the plantloop.  In order to get the correct translation to E+, we need
        //to put the name of the containing ZoneHVACBaseboardConvectiveWater onto the branch.
        if (boost::optional<CoilHeatingLowTempRadiantConstFlow> coilHLRC = modelObject.optionalCast<CoilHeatingLowTempRadiantConstFlow>() )
        {
          if (boost::optional<ZoneHVACComponent> znLowTempRadConst = coilHLRC->containingZoneHVACComponent())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfZnLowTempRadConst = this->translateAndMapModelObject(*znLowTempRadConst) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfZnLowTempRadConst->name().get();
              iddType = idfZnLowTempRadConst->iddObject().name();
            }
          }
        }
        if (boost::optional<CoilCoolingLowTempRadiantConstFlow> coilCLRC = modelObject.optionalCast<CoilCoolingLowTempRadiantConstFlow>() )
        {
          if (boost::optional<ZoneHVACComponent> znLowTempRadConst = coilCLRC->containingZoneHVACComponent())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfZnLowTempRadConst = this->translateAndMapModelObject(*znLowTempRadConst) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfZnLowTempRadConst->name().get();
              iddType = idfZnLowTempRadConst->iddObject().name();
            }
          }
        }
        if (boost::optional<CoilHeatingLowTempRadiantVarFlow> coilHLRC = modelObject.optionalCast<CoilHeatingLowTempRadiantVarFlow>() )
        {
          if (boost::optional<ZoneHVACComponent> znLowTempRadVar = coilHLRC->containingZoneHVACComponent())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfZnLowTempRadVar = this->translateAndMapModelObject(*znLowTempRadVar) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfZnLowTempRadVar->name().get();
              iddType = idfZnLowTempRadVar->iddObject().name();
            }
          }
        }
        if (boost::optional<CoilCoolingLowTempRadiantVarFlow> coilCLRC = modelObject.optionalCast<CoilCoolingLowTempRadiantVarFlow>() )
        {
          if (boost::optional<ZoneHVACComponent> znLowTempRadVar = coilCLRC->containingZoneHVACComponent())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfZnLowTempRadVar = this->translateAndMapModelObject(*znLowTempRadVar) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfZnLowTempRadVar->name().get();
              iddType = idfZnLowTempRadVar->iddObject().name();
            }
          }
        }
        //special case for Generator:MicroTurbine. 
        //In OpenStudio, this object was broken into 2 objects:
        //GeneratorMicroTurbine and GeneratorMicroTurbineHeatRecovery. 
        //The GeneratorMicroTurbineHeatRecovery goes onto the plantloop.  In order to get the correct translation to E+, we need
        //to put the name of the linked GeneratorMicroTurbine onto the branch.
        if (boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = modelObject.optionalCast<GeneratorMicroTurbineHeatRecovery>() )
        {
          if (boost::optional<GeneratorMicroTurbine> mchp = mchpHR->generatorMicroTurbine())
          {
            //translate and map containingZoneHVACBBConvWater
            if ( boost::optional<IdfObject> idfmchp = this->translateAndMapModelObject(*mchp) )
            {
              //Get the name and the idd object from the idf object version of this
              objectName = idfmchp->name().get();
              iddType = idfmchp->iddObject().name();
            }
          }
        }
      }
      else if( auto waterToAirComponent = modelObject.optionalCast<WaterToAirComponent>() )
      {
        if( loop.optionalCast<PlantLoop>() ) {
          inletNode = waterToAirComponent->waterInletModelObject()->optionalCast<Node>();
          outletNode = waterToAirComponent->waterOutletModelObject()->optionalCast<Node>();
        } else {
          inletNode = waterToAirComponent->airInletModelObject()->optionalCast<Node>();
          outletNode = waterToAirComponent->airOutletModelObject()->optionalCast<Node>();
        }
      }
      else if( auto waterToWaterComponent = modelObject.optionalCast<WaterToWaterComponent>() )
      {
        if( isSupplyBranch )
        {
          inletNode = waterToWaterComponent->supplyInletModelObject()->optionalCast<Node>();
          outletNode = waterToWaterComponent->supplyOutletModelObject()->optionalCast<Node>();
        }
        else
        {
          if( auto tertiaryInletModelObject = waterToWaterComponent->tertiaryInletModelObject() ) {
            if( auto tertiaryInletNode = tertiaryInletModelObject->optionalCast<Node>() ) {
              if( loop.demandComponent(tertiaryInletNode->handle()) ) {
                inletNode = tertiaryInletNode;
                if( auto tertiaryOutletModelObject = waterToWaterComponent->tertiaryOutletModelObject() ) {
                  outletNode = tertiaryOutletModelObject->optionalCast<Node>();
                }
              }
            }
          }
          if( ! inletNode ) {
            inletNode = waterToWaterComponent->demandInletModelObject()->optionalCast<Node>();
            outletNode = waterToWaterComponent->demandOutletModelObject()->optionalCast<Node>();
          }
        }
        //special case for WaterHeater:HeatPump. In E+, this object appears on both the 
        //zonehvac:equipmentlist and the branch.  In OpenStudio the tank (WaterHeater:Mixed)
        //is attached to the plant and the WaterHeaterHeatPump is connected to the zone and zonehvac equipment list.
        //Here we resolve all of that since it is the WaterHeaterHeatPump that must show up on both
        if( auto tank = modelObject.optionalCast<WaterHeaterMixed>() ) {
          // containingZoneHVACComponent can be WaterHeaterHeatPump
          if( auto hpwh = tank->containingZoneHVACComponent() ) {
            //translate and map containingZoneHVAC
            if ( auto hpwhIDF = translateAndMapModelObject(hpwh.get()) ) {
              //Get the name and the idd object from the idf object version of this
              objectName = hpwhIDF->name().get();
              iddType = hpwhIDF->iddObject().name();
            }
          }
        }
        if( auto tank = modelObject.optionalCast<WaterHeaterStratified>() ) {
          // containingZoneHVACComponent can be WaterHeaterHeatPump
          if( auto hpwh = tank->containingZoneHVACComponent() ) {
            //translate and map containingZoneHVAC
            if ( auto hpwhIDF = translateAndMapModelObject(hpwh.get()) ) {
              //Get the name and the idd object from the idf object version of this
              objectName = hpwhIDF->name().get();
              iddType = hpwhIDF->iddObject().name();
            }
          }
        }
      }
      else if( auto oaSystem = modelObject.optionalCast<AirLoopHVACOutdoorAirSystem>() )
      {
        inletNode = oaSystem->returnAirModelObject()->optionalCast<Node>();
        outletNode = oaSystem->mixedAirModelObject()->optionalCast<Node>();
      }

      if( inletNode && outletNode ) {
        IdfExtensibleGroup eg = branchIdfObject.pushExtensibleGroup();
        eg.setString(BranchExtensibleFields::ComponentObjectType,iddType);
        eg.setString(BranchExtensibleFields::ComponentName,objectName);
        eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNode->name().get());
        eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNode->name().get());
      }

      i++;
    }
  }
  
  return branchIdfObject; 
}

boost::optional<IdfObject> ForwardTranslator::translatePlantLoop( PlantLoop & plantLoop )
{
  // Create a new IddObjectType::PlantLoop
  IdfObject idfObject(IddObjectType::PlantLoop);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : plantLoop.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  OptionalModelObject temp;
  boost::optional<std::string> s;
  boost::optional<double> value;

  // Name

  if( (s = plantLoop.name()) )
  {
    idfObject.setName(s.get());
  }

  // Fluid Type

  if( (s = plantLoop.fluidType()) )
  {
    if (istringEqual(s.get(),"PropyleneGlycol") || istringEqual(s.get(),"EthyleneGlycol")) {
      idfObject.setString(PlantLoopFields::FluidType,"UserDefinedFluidType");
      boost::optional<int> gc = plantLoop.glycolConcentration();
      if ( gc ) {
        boost::optional<IdfObject> fluidProperties = createFluidProperties(s.get(), gc.get());
        if (fluidProperties) {
          boost::optional<std::string> fluidPropertiesName = fluidProperties->getString(FluidProperties_NameFields::FluidName,true);
          if (fluidPropertiesName) {
            idfObject.setString(PlantLoopFields::UserDefinedFluidType,fluidPropertiesName.get());
          }
        }
      }

    } else {
      idfObject.setString(PlantLoopFields::FluidType,s.get());
    }
  }

  // Loop Temperature Setpoint Node Name

  if( boost::optional<Node> node = plantLoop.loopTemperatureSetpointNode() )
  {
    idfObject.setString(PlantLoopFields::LoopTemperatureSetpointNodeName,node->name().get());
  }

  // Maximum Loop Temperature

  if( (value = plantLoop.maximumLoopTemperature()) )
  {
    idfObject.setDouble(PlantLoopFields::MaximumLoopTemperature,value.get());
  } 

  // Minimum Loop Temperature

  if( (value = plantLoop.minimumLoopTemperature()) )
  {
    idfObject.setDouble(PlantLoopFields::MinimumLoopTemperature,value.get());
  } 
  
  // Maximum Loop Flow Rate

  if( plantLoop.isMaximumLoopFlowRateAutosized() )
  {
    idfObject.setString(PlantLoopFields::MaximumLoopFlowRate,"Autosize");
  }
  else if( (value = plantLoop.maximumLoopFlowRate()) )
  {
    idfObject.setDouble(PlantLoopFields::MaximumLoopFlowRate,value.get());
  }
  
  // Minimum Loop Flow Rate

  if( plantLoop.isMinimumLoopFlowRateAutosized() )
  {
    idfObject.setString(PlantLoopFields::MinimumLoopFlowRate,"Autosize");
  }
  else if( (value = plantLoop.minimumLoopFlowRate()) )
  {
    idfObject.setDouble(PlantLoopFields::MinimumLoopFlowRate,value.get());
  }

  // LoadDistributionScheme
  {
    auto scheme = plantLoop.loadDistributionScheme();
    idfObject.setString(PlantLoopFields::LoadDistributionScheme,scheme);
  }

  // AvailabilityManager
  if (OptionalAvailabilityManager availMgr = plantLoop.availabilityManager()) {

    // Availability Manager List
    IdfObject availabilityManagerAssignmentListIdf(openstudio::IddObjectType::AvailabilityManagerAssignmentList);
    availabilityManagerAssignmentListIdf.setName(plantLoop.name().get() + " Availability Manager List");
    m_idfObjects.push_back(availabilityManagerAssignmentListIdf);
    idfObject.setString(PlantLoopFields::AvailabilityManagerListName, availabilityManagerAssignmentListIdf.name().get());

    // Availability Manager
    OptionalIdfObject availMgrIdf = translateAndMapModelObject(availMgr.get());
    OS_ASSERT(availMgrIdf);
    IdfExtensibleGroup eg = availabilityManagerAssignmentListIdf.pushExtensibleGroup();
    eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType,availMgrIdf->iddObject().name());
    eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,availMgrIdf->name().get());
  }

  //  PlantLoopDemandCalculationScheme
  {
    auto spms = plantLoop.supplyOutletNode().setpointManagers();
    if( subsetCastVector<model::SetpointManagerScheduledDualSetpoint>(spms).empty() ) {
      idfObject.setString(PlantLoopFields::PlantLoopDemandCalculationScheme,"SingleSetpoint");
    } else {
      idfObject.setString(PlantLoopFields::PlantLoopDemandCalculationScheme,"DualSetpointDeadband");
    }
  }

  // Plant Loop Volume

  if( plantLoop.isPlantLoopVolumeAutocalculated() )
  {
    idfObject.setString(PlantLoopFields::PlantLoopVolume,"Autocalculate");
  }
  else if( (value = plantLoop.plantLoopVolume()) )
  {
    idfObject.setDouble(PlantLoopFields::PlantLoopVolume,value.get());
  }

  // Common Pipe Simulation

  if( (s = plantLoop.commonPipeSimulation()) )
  {
    idfObject.setString(PlantLoopFields::CommonPipeSimulation,s.get());
  } 

  // Inlet/Outlet Nodes
  idfObject.setString(PlantLoopFields::PlantSideInletNodeName,plantLoop.supplyInletNode().name().get());
  idfObject.setString(PlantLoopFields::PlantSideOutletNodeName,plantLoop.supplyOutletNode().name().get());
  idfObject.setString(PlantLoopFields::DemandSideInletNodeName,plantLoop.demandInletNode().name().get());
  idfObject.setString(PlantLoopFields::DemandSideOutletNodeName,plantLoop.demandOutletNode().name().get());

  auto supplyComponents = plantLoop.supplyComponents();

  SizingPlant sizingPlant = plantLoop.sizingPlant();
  translateAndMapModelObject(sizingPlant);

  // Mark where we want the plant operation schemes to go
  // Don't actually translate yet because we don't want the components to show up yet
  // auto operationSchemeLocation = std::distance(m_idfObjects.begin(),m_idfObjects.end());

  // Supply Side

  IdfObject _supplyBranchList(IddObjectType::BranchList);
  _supplyBranchList.setName(plantLoop.name().get() + " Supply Branches");
  _supplyBranchList.clearExtensibleGroups();
  m_idfObjects.push_back(_supplyBranchList);
  idfObject.setString(PlantLoopFields::PlantSideBranchListName,_supplyBranchList.name().get());

  IdfObject _supplyConnectorList(IddObjectType::ConnectorList);
  _supplyConnectorList.setName(plantLoop.name().get() + " Supply Connector List");
  m_idfObjects.push_back(_supplyConnectorList);
  idfObject.setString(PlantLoopFields::PlantSideConnectorListName,_supplyConnectorList.name().get());

  Splitter supplySplitter = plantLoop.supplySplitter();
  Mixer supplyMixer = plantLoop.supplyMixer();

  Node supplyInletNode = plantLoop.supplyInletNode();
  Node supplyOutletNode = plantLoop.supplyOutletNode();

  IdfObject _supplySplitter(IddObjectType::Connector_Splitter);
  _supplySplitter.clearExtensibleGroups();
  _supplySplitter.setName(plantLoop.name().get() + " Supply Splitter");
  m_idfObjects.push_back(_supplySplitter);
  ExtensibleIndex ei2(0,0);
  ei2.group = 0;
  ei2.field = ConnectorListExtensibleFields::ConnectorObjectType;
  _supplyConnectorList.setString(_supplyConnectorList.iddObject().index(ei2),
                                 _supplySplitter.iddObject().name());
  ei2.field = ConnectorListExtensibleFields::ConnectorName;
  _supplyConnectorList.setString(_supplyConnectorList.iddObject().index(ei2),
                                 _supplySplitter.name().get());

  IdfObject _supplyMixer(IddObjectType::Connector_Mixer);
  _supplyMixer.clearExtensibleGroups();
  _supplyMixer.setName(plantLoop.name().get() + " Supply Mixer");
  m_idfObjects.push_back(_supplyMixer);
  ei2.group = 1;
  ei2.field = ConnectorListExtensibleFields::ConnectorObjectType;
  _supplyConnectorList.setString(_supplyConnectorList.iddObject().index(ei2),
                                 _supplyMixer.iddObject().name());
  ei2.field = ConnectorListExtensibleFields::ConnectorName;
  _supplyConnectorList.setString(_supplyConnectorList.iddObject().index(ei2),
                                 _supplyMixer.name().get());

  // Supply inlet branch

  IdfObject _supplyInletBranch(IddObjectType::Branch);
  _supplyInletBranch.setName(plantLoop.name().get() + " Supply Inlet Branch");
  _supplyInletBranch.clearExtensibleGroups();
  IdfExtensibleGroup eg = _supplyBranchList.pushExtensibleGroup();
  eg.setString(BranchListExtensibleFields::BranchName,_supplyInletBranch.name().get());

  _supplySplitter.setString(Connector_SplitterFields::InletBranchName,_supplyInletBranch.name().get());

  m_idfObjects.push_back(_supplyInletBranch);

  std::vector<ModelObject> supplyInletModelObjects;
  supplyInletModelObjects = plantLoop.supplyComponents(supplyInletNode,supplySplitter);

  OS_ASSERT( supplyInletModelObjects.size() >= 2 );

  if( supplyInletModelObjects.size() > 2u )
  {
    populateBranch( _supplyInletBranch,
                    supplyInletModelObjects,
                    plantLoop,
                    true);
  }
  else
  {
    IdfObject pipe(IddObjectType::Pipe_Adiabatic);
    pipe.setName(plantLoop.name().get() + " Supply Inlet Pipe");
    m_idfObjects.push_back(pipe);

    std::string inletNodeName = plantLoop.supplyInletNode().name().get();
    std::string outletNodeName = plantLoop.name().get() + " Supply Inlet Pipe Node";

    pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
    pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

    IdfExtensibleGroup eg = _supplyInletBranch.pushExtensibleGroup();
    eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
    eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
    eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
    eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
  }

  // Populate supply equipment branches

  std::vector<model::ModelObject> splitterOutletObjects = supplySplitter.outletModelObjects();
  std::vector<model::ModelObject> mixerInletObjects = supplyMixer.inletModelObjects();

  auto it2 = mixerInletObjects.begin();
  unsigned i = 0;
  for( auto & splitterOutletObject : splitterOutletObjects )
  {
    i++;

    std::stringstream ss;
    ss << i;
    std::string istring = ss.str();

    model::HVACComponent comp1 = splitterOutletObject.optionalCast<model::HVACComponent>().get();
    model::HVACComponent comp2 = it2->optionalCast<model::HVACComponent>().get();

    std::vector<model::ModelObject> allComponents = plantLoop.supplyComponents(comp1,comp2);

    IdfObject _equipmentBranch(IddObjectType::Branch); 
    _equipmentBranch.clearExtensibleGroups();
    _equipmentBranch.setName( plantLoop.name().get() + " Supply Branch " + istring );
    m_idfObjects.push_back(_equipmentBranch);

    eg = _supplySplitter.pushExtensibleGroup();
    eg.setString(Connector_SplitterExtensibleFields::OutletBranchName,_equipmentBranch.name().get());

    eg = _supplyMixer.pushExtensibleGroup();
    eg.setString(Connector_MixerExtensibleFields::InletBranchName,_equipmentBranch.name().get());

    eg = _supplyBranchList.pushExtensibleGroup();
    eg.setString(BranchListExtensibleFields::BranchName,_equipmentBranch.name().get());

    if( allComponents.size() > 2u )
    {
      populateBranch( _equipmentBranch,
                      allComponents,
                      plantLoop,
                      true);
    }
    else
    {
      IdfObject pipe(IddObjectType::Pipe_Adiabatic);
      pipe.setName(plantLoop.name().get() + " Supply Branch " + istring + " Pipe");
      m_idfObjects.push_back(pipe);

      std::string inletNodeName = pipe.name().get() + " Inlet Node";
      std::string outletNodeName = pipe.name().get() + " Outlet Node";
      
      pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
      pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

      IdfExtensibleGroup eg = _equipmentBranch.pushExtensibleGroup();
      eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
      eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
      eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
      eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
    }

    ++it2;
  }

  // Populate supply outlet branch

  IdfObject _supplyOutletBranch(IddObjectType::Branch);
  _supplyOutletBranch.setName(plantLoop.name().get() + " Supply Outlet Branch");
  _supplyOutletBranch.clearExtensibleGroups();
  eg = _supplyBranchList.pushExtensibleGroup();
  eg.setString(BranchListExtensibleFields::BranchName,_supplyOutletBranch.name().get());

  _supplyMixer.setString(Connector_MixerFields::OutletBranchName,_supplyOutletBranch.name().get());

  m_idfObjects.push_back(_supplyOutletBranch);

  std::vector<ModelObject> supplyOutletModelObjects;
  supplyOutletModelObjects = plantLoop.supplyComponents(supplyMixer,supplyOutletNode);  

  OS_ASSERT( supplyOutletModelObjects.size() >= 2 );

  if( supplyOutletModelObjects.size() > 2u )
  {
    populateBranch( _supplyOutletBranch,
                    supplyOutletModelObjects,
                    plantLoop,
                    true);
  }
  else
  {
    IdfObject pipe(IddObjectType::Pipe_Adiabatic);
    pipe.setName(plantLoop.name().get() + " Supply Outlet Pipe");
    m_idfObjects.push_back(pipe);

    std::string inletNodeName = plantLoop.name().get() + " Supply Outlet Pipe Node";
    std::string outletNodeName = plantLoop.supplyOutletNode().name().get();
    
    pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
    pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

    IdfExtensibleGroup eg = _supplyOutletBranch.pushExtensibleGroup();
    eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
    eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
    eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
    eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
  }

  // Demand Side

  IdfObject _demandBranchList(IddObjectType::BranchList);
  _demandBranchList.setName(plantLoop.name().get() + " Demand Branches");
  _demandBranchList.clearExtensibleGroups();
  m_idfObjects.push_back(_demandBranchList);
  idfObject.setString(PlantLoopFields::DemandSideBranchListName,_demandBranchList.name().get());

  IdfObject _demandConnectorList(IddObjectType::ConnectorList);
  _demandConnectorList.setName(plantLoop.name().get() + " Demand Connector List");
  m_idfObjects.push_back(_demandConnectorList);
  idfObject.setString(PlantLoopFields::DemandSideConnectorListName,_demandConnectorList.name().get());

  Splitter demandSplitter = plantLoop.demandSplitter();
  Mixer demandMixer = plantLoop.demandMixer();

  Node demandInletNode = plantLoop.demandInletNode();
  Node demandOutletNode = plantLoop.demandOutletNode();

  IdfObject _demandSplitter(IddObjectType::Connector_Splitter);
  _demandSplitter.clearExtensibleGroups();
  _demandSplitter.setName(plantLoop.name().get() + " Demand Splitter");
  m_idfObjects.push_back(_demandSplitter);
  ExtensibleIndex ei(0,0);
  ei.group = 0;
  ei.field = ConnectorListExtensibleFields::ConnectorObjectType;
  _demandConnectorList.setString(_demandConnectorList.iddObject().index(ei),
                                 _demandSplitter.iddObject().name());
  ei.field = ConnectorListExtensibleFields::ConnectorName;
  _demandConnectorList.setString(_demandConnectorList.iddObject().index(ei),
                                 _demandSplitter.name().get());

  IdfObject _demandMixer(IddObjectType::Connector_Mixer);
  _demandMixer.clearExtensibleGroups();
  _demandMixer.setName(plantLoop.name().get() + " Demand Mixer");
  m_idfObjects.push_back(_demandMixer);
  ei.group = 1;
  ei.field = ConnectorListExtensibleFields::ConnectorObjectType;
  _demandConnectorList.setString(_demandConnectorList.iddObject().index(ei),
                                 _demandMixer.iddObject().name());
  ei.field = ConnectorListExtensibleFields::ConnectorName;
  _demandConnectorList.setString(_demandConnectorList.iddObject().index(ei),
                                 _demandMixer.name().get());

  // Demand inlet branch

  IdfObject _demandInletBranch(IddObjectType::Branch);
  _demandInletBranch.setName(plantLoop.name().get() + " Demand Inlet Branch");
  _demandInletBranch.clearExtensibleGroups();
  eg = _demandBranchList.pushExtensibleGroup();
  eg.setString(BranchListExtensibleFields::BranchName,_demandInletBranch.name().get());

  _demandSplitter.setString(Connector_SplitterFields::InletBranchName,_demandInletBranch.name().get());

  m_idfObjects.push_back(_demandInletBranch);

  std::vector<ModelObject> demandInletModelObjects;
  demandInletModelObjects = plantLoop.demandComponents(demandInletNode,demandSplitter);

  OS_ASSERT( demandInletModelObjects.size() >= 2u );  

  if( demandInletModelObjects.size() > 2u )
  {
    populateBranch( _demandInletBranch,
                    demandInletModelObjects,
                    plantLoop,
                    false);
  }
  else
  {
    IdfObject pipe(IddObjectType::Pipe_Adiabatic);
    pipe.setName(plantLoop.name().get() + " Demand Inlet Pipe");
    m_idfObjects.push_back(pipe);

    std::string inletNodeName = plantLoop.demandInletNode().name().get();
    std::string outletNodeName = plantLoop.name().get() + " Demand Inlet Pipe Node";
    
    pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
    pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

    IdfExtensibleGroup eg = _demandInletBranch.pushExtensibleGroup();
    eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
    eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
    eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
    eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
  }

  // Populate equipment branches

  splitterOutletObjects = demandSplitter.outletModelObjects();
  mixerInletObjects = demandMixer.inletModelObjects();

  it2 = mixerInletObjects.begin();
  i = 0;
  for( auto & splitterOutletObject : splitterOutletObjects )
  {
    i++;

    std::stringstream ss;
    ss << i;
    std::string istring = ss.str();

    model::HVACComponent comp1 = splitterOutletObject.optionalCast<model::HVACComponent>().get();
    model::HVACComponent comp2 = it2->optionalCast<model::HVACComponent>().get();

    std::vector<model::ModelObject> allComponents = plantLoop.demandComponents(comp1,comp2);

    IdfObject _equipmentBranch(IddObjectType::Branch); 
    _equipmentBranch.clearExtensibleGroups();
    _equipmentBranch.setName( plantLoop.name().get() + " Demand Branch " + istring );
    m_idfObjects.push_back(_equipmentBranch);

    eg = _demandSplitter.pushExtensibleGroup();
    eg.setString(Connector_SplitterExtensibleFields::OutletBranchName,_equipmentBranch.name().get());

    eg = _demandMixer.pushExtensibleGroup();
    eg.setString(Connector_MixerExtensibleFields::InletBranchName,_equipmentBranch.name().get());

    eg = _demandBranchList.pushExtensibleGroup();
    eg.setString(BranchListExtensibleFields::BranchName,_equipmentBranch.name().get());

    if( allComponents.size() > 2u )
    {
      populateBranch( _equipmentBranch,
                      allComponents,
                      plantLoop,
                      false);
    }
    else
    {
      IdfObject pipe(IddObjectType::Pipe_Adiabatic);
      pipe.setName(plantLoop.name().get() + " Demand Branch " + istring + " Pipe");
      m_idfObjects.push_back(pipe);

      std::string inletNodeName = pipe.name().get() + " Inlet Node";
      std::string outletNodeName = pipe.name().get() + " Outlet Node";
      
      pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
      pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

      IdfExtensibleGroup eg = _equipmentBranch.pushExtensibleGroup();
      eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
      eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
      eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
      eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
    }

    ++it2;
  }

  // Install a bypass branch with a pipe

  if( splitterOutletObjects.size() > 0u )
  {
    IdfObject _equipmentBranch(IddObjectType::Branch); 
    _equipmentBranch.clearExtensibleGroups();
    _equipmentBranch.setName( plantLoop.name().get() + " Demand Bypass Branch" );
    m_idfObjects.push_back(_equipmentBranch);

    eg = _demandSplitter.pushExtensibleGroup();
    eg.setString(Connector_SplitterExtensibleFields::OutletBranchName,_equipmentBranch.name().get());

    eg = _demandMixer.pushExtensibleGroup();
    eg.setString(Connector_MixerExtensibleFields::InletBranchName,_equipmentBranch.name().get());

    eg = _demandBranchList.pushExtensibleGroup();
    eg.setString(BranchListExtensibleFields::BranchName,_equipmentBranch.name().get());

    IdfObject pipe(IddObjectType::Pipe_Adiabatic);
    pipe.setName(plantLoop.name().get() + " Demand Bypass Pipe");
    m_idfObjects.push_back(pipe);

    std::string inletNodeName = pipe.name().get() + " Inlet Node";
    std::string outletNodeName = pipe.name().get() + " Outlet Node";
    
    pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
    pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

    IdfExtensibleGroup eg = _equipmentBranch.pushExtensibleGroup();
    eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
    eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
    eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
    eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
  }

  // Populate demand outlet branch

  IdfObject _demandOutletBranch(IddObjectType::Branch);
  _demandOutletBranch.setName(plantLoop.name().get() + " Demand Outlet Branch");
  _demandOutletBranch.clearExtensibleGroups();
  eg = _demandBranchList.pushExtensibleGroup();
  eg.setString(BranchListExtensibleFields::BranchName,_demandOutletBranch.name().get());

  _demandMixer.setString(Connector_MixerFields::OutletBranchName,_demandOutletBranch.name().get());

  m_idfObjects.push_back(_demandOutletBranch);

  std::vector<ModelObject> demandOutletModelObjects;
  demandOutletModelObjects = plantLoop.demandComponents(demandMixer,demandOutletNode);  

  OS_ASSERT( demandOutletModelObjects.size() >= 2u );

  if( demandOutletModelObjects.size() > 2u )
  {
    populateBranch( _demandOutletBranch,
                    demandOutletModelObjects,
                    plantLoop,
                    false);
  }
  else
  {
    IdfObject pipe(IddObjectType::Pipe_Adiabatic);
    pipe.setName(plantLoop.name().get() + " Demand Outlet Pipe");
    m_idfObjects.push_back(pipe);

    std::string inletNodeName = plantLoop.name().get() + " Demand Outlet Pipe Node";
    std::string outletNodeName = plantLoop.demandOutletNode().name().get();
    
    pipe.setString(Pipe_AdiabaticFields::InletNodeName,inletNodeName);
    pipe.setString(Pipe_AdiabaticFields::OutletNodeName,outletNodeName);

    IdfExtensibleGroup eg = _demandOutletBranch.pushExtensibleGroup();
    eg.setString(BranchExtensibleFields::ComponentObjectType,pipe.iddObject().name());
    eg.setString(BranchExtensibleFields::ComponentName,pipe.name().get());
    eg.setString(BranchExtensibleFields::ComponentInletNodeName,inletNodeName);
    eg.setString(BranchExtensibleFields::ComponentOutletNodeName,outletNodeName);
  }

  // Operation Scheme
  // auto opSchemeStart = m_idfObjects.end();
  const auto & operationSchemes = translatePlantEquipmentOperationSchemes(plantLoop);
  OS_ASSERT(operationSchemes);
  idfObject.setString(PlantLoopFields::PlantEquipmentOperationSchemeName,operationSchemes->name().get());
  // auto opSchemeEnd = m_idfObjects.end();

  //std::vector<IdfObject> opSchemeObjects(opSchemeStart,opSchemeEnd);
  //m_idfObjects.erase(opSchemeStart,opSchemeEnd);
  //m_idfObjects.insert(m_idfObjects.begin() + operationSchemeLocation,opSchemeObjects.begin(),opSchemeObjects.end());

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

