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
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/BoilerHotWater_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
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
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

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
    default:
    {
      break;
    }
  }

  return result;
}

enum class ComponentType {HEATING, COOLING, BOTH};

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
    default:
    {
      return ComponentType::BOTH;
    }
  }
}

std::vector<HVACComponent> setpointComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( _isSetpointComponent(plantLoop,comp) ) {
      result.push_back(comp);
    }
  }

  return result;
}

std::vector<HVACComponent> coolingComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::COOLING ) {
      result.push_back(comp);
    }
  }

  return result;
}

std::vector<HVACComponent> heatingComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::HEATING ) {
      result.push_back(comp);
    }
  }

  return result;
}

std::vector<HVACComponent> uncontrolledComponents(const PlantLoop & plantLoop)
{
  std::vector<HVACComponent> result;

  for( const auto & comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents()) ) {
    if( componentType(comp) == ComponentType::BOTH ) {
      result.push_back(comp);
    }
  }

  return result;
}

boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationSchemes( PlantLoop & plantLoop )
{
  IdfObject _operationSchemes(IddObjectType::PlantEquipmentOperationSchemes);
  m_idfObjects.push_back(_operationSchemes);

  _operationSchemes.setName(plantLoop.name().get() + " Operation Schemes");
  return _operationSchemes;
}

} // energyplus

} // openstudio

