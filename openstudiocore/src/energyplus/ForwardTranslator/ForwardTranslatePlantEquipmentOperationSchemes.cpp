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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/BoilerHotWater_Impl.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/ChillerElectricEIR_Impl.hpp"
#include "../../model/ChillerAbsorptionIndirect.hpp"
#include "../../model/ChillerAbsorptionIndirect_Impl.hpp"
#include "../../model/ChillerAbsorption.hpp"
#include "../../model/ChillerAbsorption_Impl.hpp"
#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../../model/CoolingTowerVariableSpeed.hpp"
#include "../../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../../model/CoolingTowerTwoSpeed.hpp"
#include "../../model/CoolingTowerTwoSpeed_Impl.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../../model/PlantEquipmentOperationCoolingLoad.hpp"
#include "../../model/PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../../model/PlantEquipmentOperationScheme.hpp"
#include "../../model/PlantEquipmentOperationScheme_Impl.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/FluidCoolerSingleSpeed.hpp"
#include "../../model/FluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/FluidCoolerTwoSpeed.hpp"
#include "../../model/FluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage_Impl.hpp"
#include "../../model/SolarCollectorFlatPlateWater.hpp"
#include "../../model/SolarCollectorFlatPlateWater_Impl.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
#include "../../model/PlantComponentTemperatureSource.hpp"
#include "../../model/PlantComponentTemperatureSource_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_ComponentSetpoint_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>

// Special case
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

bool _isSetpointComponent(const PlantLoop & plantLoop,const ModelObject & comp)
{
  auto supplyOutletNode = plantLoop.supplyOutletNode();
  auto hvacComp = comp.optionalCast<HVACComponent>();
  OS_ASSERT(hvacComp);

  auto nodes = subsetCastVector<Node>(plantLoop.supplyComponents(hvacComp.get(),supplyOutletNode));
  OS_ASSERT(! nodes.empty());
  auto componentOutletNode = nodes.front();

  if( componentOutletNode != supplyOutletNode ) {
    auto _setpointManagers = componentOutletNode.setpointManagers();
    if( ! _setpointManagers.empty() ) {
      return true;
    }
  }

  return false;
}

boost::optional<Node> inletNode(const PlantLoop & plantLoop, const HVACComponent & component)
{
  auto comps = plantLoop.supplyComponents(plantLoop.supplyInletNode(),component);
  OS_ASSERT(comps.size() >= 2u);
  return (comps.end() - 2u)->optionalCast<Node>();
}

boost::optional<Node> outletNode(const PlantLoop & plantLoop, const HVACComponent & component)
{
  auto comps = plantLoop.supplyComponents(component,plantLoop.supplyOutletNode());
  OS_ASSERT(comps.size() >= 2u);
  return comps[1].optionalCast<Node>();
}

boost::optional<double> flowrate(const HVACComponent & component)
{
  boost::optional<double> result;
  switch(component.iddObject().type().value()) {
    case openstudio::IddObjectType::OS_Boiler_HotWater :
    {
      auto boiler = component.cast<BoilerHotWater>();
      result = boiler.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Mixed :
    {
      auto waterHeater = component.cast<WaterHeaterMixed>();
      result = waterHeater.useSideDesignFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Stratified :
    {
      auto waterHeater = component.cast<WaterHeaterStratified>();
      result = waterHeater.useSideDesignFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_DistrictHeating :
    {
      break;
    }      
    case openstudio::IddObjectType::OS_Chiller_Electric_EIR :
    {
      auto chiller = component.cast<ChillerElectricEIR>();
      result = chiller.referenceChilledWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect :
    {
      auto chiller = component.cast<ChillerAbsorptionIndirect>();
      result = chiller.designChilledWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption :
    {
      auto chiller = component.cast<ChillerAbsorption>();
      result = chiller.designChilledWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed :
    {
      break;
    }
    case openstudio::IddObjectType::OS_DistrictCooling :
    {
      break;
    }      
    case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed :
    {
      auto tower = component.cast<CoolingTowerSingleSpeed>();
      result = tower.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed :
    {
      auto tower = component.cast<CoolingTowerVariableSpeed>();
      result = tower.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed:
    {
      auto tower = component.cast<CoolingTowerTwoSpeed>();
      result = tower.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed:
    {
      auto mo = component.cast<EvaporativeFluidCoolerSingleSpeed>();
      result = mo.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed:
    {
      auto mo = component.cast<FluidCoolerSingleSpeed>();
      result = mo.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed:
    {
      auto mo = component.cast<FluidCoolerTwoSpeed>();
      result = mo.designWaterFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical :
    {
      auto hx = component.cast<GroundHeatExchangerVertical>();
      result = hx.maximumFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench :
    {
      auto hx = component.cast<GroundHeatExchangerHorizontalTrench>();
      result = hx.designFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid :
    {
      auto hx = component.cast<HeatExchangerFluidToFluid>();
      result = hx.loopSupplySideDesignFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal :
    {
      auto mo = component.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
      result = mo.designFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water :
    {
      auto mo = component.cast<SolarCollectorFlatPlateWater>();
      result = mo.maximumFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage :
    {
      auto mo = component.cast<SolarCollectorIntegralCollectorStorage>();
      result = mo.maximumFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource :
    {
      auto mo = component.cast<PlantComponentTemperatureSource>();
      result = mo.designVolumeFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling :
    {
      auto mo = component.cast<HeatPumpWaterToWaterEquationFitCooling>();
      result = mo.ratedLoadSideFlowRate();
      break;
    }
    case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating :
    {
      auto mo = component.cast<HeatPumpWaterToWaterEquationFitHeating>();
      result = mo.ratedLoadSideFlowRate();
      break;
    }
    default:
    {
      break;
    }
  }

  return result;
}

enum class ComponentType {HEATING, COOLING, BOTH, NONE};

ComponentType componentType(const HVACComponent & component)
{
  switch(component.iddObject().type().value())
  {
    case openstudio::IddObjectType::OS_Boiler_HotWater :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Mixed :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Stratified :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_DistrictHeating :
    {
      return ComponentType::HEATING;
    }      
    case openstudio::IddObjectType::OS_Chiller_Electric_EIR :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed :
    {
      return ComponentType::BOTH;
    }
    case openstudio::IddObjectType::OS_DistrictCooling :
    {
      return ComponentType::COOLING;
    }      
    case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed:
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed:
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed:
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed:
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical :
    {
      return ComponentType::BOTH;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench :
    {
      return ComponentType::BOTH;
    }
    case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid :
    {
      return ComponentType::BOTH;
    }
    case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource :
    {
      return ComponentType::BOTH;
    }
    case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating :
    {
      return ComponentType::HEATING;
    }
    case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling :
    {
      return ComponentType::COOLING;
    }
    case openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery :
    {
      // Maybe that should be both, in the case of an absorption chiller?
      // Also, should maybe check if it's in mode FollowThermal or FollowThermalLimitElectrical?
      // If not in these two modes, it doesn't care and just runs. Also, it's typically on the demand Side, and this method
      // is only called on the supply side
      return ComponentType::HEATING;
    }
    default:
    {
      return ComponentType::NONE;
    }
  }
}

// Some plant components air in a containingHVACComponent() and it is that
// container which needs to go on the plant operation scheme. Here is a filter to 
// figure that out. 
HVACComponent operationSchemeComponent(const HVACComponent & component) {
    boost::optional<HVACComponent> result;

    switch(component.iddObject().type().value())
    {
      case openstudio::IddObjectType::OS_WaterHeater_Mixed :
      {
        auto waterHeater = component.cast<WaterHeaterMixed>();
        if( auto hpwh = waterHeater.containingZoneHVACComponent() ) {
          result = hpwh;
        }
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified :
      {
        auto waterHeater = component.cast<WaterHeaterStratified>();
        if( auto hpwh = waterHeater.containingZoneHVACComponent() ) {
          result = hpwh;
        }
        break;
      }
      default:
      {
        break;
      }
    }

  if( result ) {
    return result.get();
  }

  return component;
}

std::vector<HVACComponent> setpointComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( ! comp.optionalCast<Node>() ) {
      if( _isSetpointComponent(plantLoop,comp) ) {
        result.push_back(operationSchemeComponent(comp));
      }
    }
  }

  return result;
}

std::vector<HVACComponent> coolingComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::COOLING ) {
      result.push_back(operationSchemeComponent(comp));
    }
  }

  return result;
}

std::vector<HVACComponent> heatingComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::HEATING ) {
      result.push_back(operationSchemeComponent(comp));
    }
  }

  return result;
}

std::vector<HVACComponent> uncontrolledComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::BOTH ) {
      result.push_back(operationSchemeComponent(comp));
    }
  }

  return result;
}

boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationSchemes( PlantLoop & plantLoop )
{
  IdfObject operationSchemes(IddObjectType::PlantEquipmentOperationSchemes);
  m_idfObjects.push_back(operationSchemes);

  operationSchemes.setName(plantLoop.name().get() + " Operation Schemes");

  auto alwaysOnSchedule = plantLoop.model().alwaysOnDiscreteSchedule();
  auto _alwaysOn = translateAndMapModelObject(alwaysOnSchedule);
  OS_ASSERT(_alwaysOn);

  // Lambda does what the name suggests, create setpoint operation schemes.
  // This is for any component that has a setpoint manager on its outlet node
  auto createSetpointOperationScheme = [&](PlantLoop & plantLoop) {
    const auto & t_setpointComponents = setpointComponents(plantLoop);
    if( ! t_setpointComponents.empty() ) {
      IdfObject setpointOperation(IddObjectType::PlantEquipmentOperation_ComponentSetpoint);
      setpointOperation.setName(plantLoop.name().get() + " Setpoint Operation Scheme");
      m_idfObjects.push_back(setpointOperation);
      setpointOperation.clearExtensibleGroups();

      IdfExtensibleGroup eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,setpointOperation.iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,setpointOperation.name().get());
      if( const auto & schedule = plantLoop.componentSetpointOperationSchemeSchedule() ) {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,schedule->nameString());
      } else {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());
      }

      for( auto setpointComponent : t_setpointComponents )
      {
        // TODO: Find the right way to deal with this
        // For now, "dirty" (?) fix for Generator:MicroTurbine
        // @kbenne, FYI

        boost::optional<IdfObject> _idfObject;

        if (boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = setpointComponent.optionalCast<GeneratorMicroTurbineHeatRecovery>())
        {
          GeneratorMicroTurbine mchp = mchpHR->generatorMicroTurbine();
          _idfObject = translateAndMapModelObject(mchp);
        }
        else
        {
          _idfObject = translateAndMapModelObject(setpointComponent);
        }

        OS_ASSERT(_idfObject);

        IdfExtensibleGroup eg = setpointOperation.pushExtensibleGroup();
        eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentObjectType,_idfObject->iddObject().name());
        eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentName,_idfObject->name().get());
        if( const auto & t_inletNode = inletNode(plantLoop,setpointComponent) ) {
          eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::DemandCalculationNodeName,t_inletNode->name().get());
        }
        if( const auto & t_outletNode = outletNode(plantLoop,setpointComponent) ) {
          eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::SetpointNodeName,t_outletNode->name().get());
        }
        if( auto value = flowrate(setpointComponent) ) {
          eg.setDouble(PlantEquipmentOperation_ComponentSetpointExtensibleFields::ComponentFlowRate,value.get());
        } else {
          eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::ComponentFlowRate,"Autosize");
        }
        auto t_componentType = componentType(setpointComponent);
        switch(t_componentType)
        {
          case ComponentType::HEATING :
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType,"Heating");
            break;
          case ComponentType::COOLING :
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType,"Cooling");
            break;
          default :
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType,"Dual");
            break;
        }
      }
    }
  };

  bool applyDefault = true;

  // If any operation schemes are defined in the model then don't apply default operation schemes

  if( auto coolingLoadScheme = plantLoop.plantEquipmentOperationCoolingLoad() ) {
    auto _scheme = translateAndMapModelObject(coolingLoadScheme.get());
    OS_ASSERT(_scheme);
    auto eg = operationSchemes.pushExtensibleGroup();
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,_scheme->iddObject().name());
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,_scheme->name().get());
    if( const auto & schedule = plantLoop.plantEquipmentOperationCoolingLoadSchedule() ) {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,schedule->nameString());
    } else {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());
    }

    applyDefault = false;
  } 

  if( auto heatingLoadScheme = plantLoop.plantEquipmentOperationHeatingLoad() ) {
    auto _scheme = translateAndMapModelObject(heatingLoadScheme.get());
    OS_ASSERT(_scheme);
    auto eg = operationSchemes.pushExtensibleGroup();
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,_scheme->iddObject().name());
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,_scheme->name().get());
    if( const auto & schedule = plantLoop.plantEquipmentOperationHeatingLoadSchedule() ) {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,schedule->nameString());
    } else {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());
    }

    applyDefault = false;
  }

  if( auto primaryScheme = plantLoop.primaryPlantEquipmentOperationScheme() ) {
    auto _scheme = translateAndMapModelObject(primaryScheme.get());
    OS_ASSERT(_scheme);
    auto eg = operationSchemes.pushExtensibleGroup();
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,_scheme->iddObject().name());
    eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,_scheme->name().get());
    if( const auto & schedule = plantLoop.primaryPlantEquipmentOperationSchemeSchedule() ) {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,schedule->nameString());
    } else {
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());
    }

    createSetpointOperationScheme(plantLoop);
    applyDefault = false;
  }

  if( applyDefault ) {
    // If we get here then there must not be any operation schemes defined in the model 
    // and we should go ahead and create default schemes.
    const auto & t_heatingComponents = heatingComponents( plantLoop );
    if( ! t_heatingComponents.empty() ) {
      IdfObject heatingOperation(IddObjectType::PlantEquipmentOperation_HeatingLoad);
      heatingOperation.setName(plantLoop.name().get() + " Heating Operation Scheme");
      m_idfObjects.push_back(heatingOperation);
      heatingOperation.clearExtensibleGroups();

      IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
      plantEquipmentList.setName(plantLoop.name().get() + " Heating Equipment List");
      plantEquipmentList.clearExtensibleGroups();
      m_idfObjects.push_back(plantEquipmentList);

      IdfExtensibleGroup eg = heatingOperation.pushExtensibleGroup();
      eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeLowerLimit,0.0);
      eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeUpperLimit,1E9);
      eg.setString(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName,plantEquipmentList.name().get());

      eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,heatingOperation.iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,heatingOperation.name().get());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());

      for( auto heatingComponent : t_heatingComponents ) {
        if( const auto & idfObject = translateAndMapModelObject(heatingComponent) ) {
          IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType,idfObject->iddObject().name());
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentName,idfObject->name().get());
        }
      }
    }

    const auto & t_coolingComponents = coolingComponents( plantLoop );
    if( ! t_coolingComponents.empty() ) {
      IdfObject coolingOperation(IddObjectType::PlantEquipmentOperation_CoolingLoad);
      coolingOperation.setName(plantLoop.name().get() + " Cooling Operation Scheme");
      m_idfObjects.push_back(coolingOperation);
      coolingOperation.clearExtensibleGroups();

      IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
      plantEquipmentList.setName(plantLoop.name().get() + " Cooling Equipment List");
      plantEquipmentList.clearExtensibleGroups();
      m_idfObjects.push_back(plantEquipmentList);

      IdfExtensibleGroup eg = coolingOperation.pushExtensibleGroup();
      eg.setDouble(PlantEquipmentOperation_CoolingLoadExtensibleFields::LoadRangeLowerLimit,0.0);
      eg.setDouble(PlantEquipmentOperation_CoolingLoadExtensibleFields::LoadRangeUpperLimit,1E9);
      eg.setString(PlantEquipmentOperation_CoolingLoadExtensibleFields::RangeEquipmentListName,plantEquipmentList.name().get());

      eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,coolingOperation.iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,coolingOperation.name().get());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());

      for( auto coolingComponent : t_coolingComponents ) {
        if( const auto & idfObject = translateAndMapModelObject(coolingComponent) ) {
          IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType,idfObject->iddObject().name());
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentName,idfObject->name().get());
        }
      }
    }

    const auto & t_uncontrolledComponents = uncontrolledComponents( plantLoop );
    if( ! t_uncontrolledComponents.empty() ) {

      IdfObject uncontrolledOperation(IddObjectType::PlantEquipmentOperation_Uncontrolled);
      uncontrolledOperation.setName(plantLoop.name().get() + " Uncontrolled Operation Scheme");
      m_idfObjects.push_back(uncontrolledOperation);
      uncontrolledOperation.clearExtensibleGroups();

      IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
      plantEquipmentList.setName(plantLoop.name().get() + " Uncontrolled Equipment List");
      plantEquipmentList.clearExtensibleGroups();
      m_idfObjects.push_back(plantEquipmentList);

      uncontrolledOperation.setString(PlantEquipmentOperation_UncontrolledFields::EquipmentListName,plantEquipmentList.name().get());

      auto eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType,uncontrolledOperation.iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName,uncontrolledOperation.name().get());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName,_alwaysOn->name().get());

      for( auto uncontrolledComponent : t_uncontrolledComponents ) {
        if( const auto & idfObject = translateAndMapModelObject(uncontrolledComponent) ) {
          IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType,idfObject->iddObject().name());
          eg.setString(PlantEquipmentListExtensibleFields::EquipmentName,idfObject->name().get());
        }
      }
    }

    createSetpointOperationScheme(plantLoop);
  }

  return operationSchemes;
}

} // energyplus

} // openstudio

