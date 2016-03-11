/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../../model/SizingPlant.hpp"
#include "../../model/SizingPlant_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_Plant_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

enum class PlantSizingType {HOTWATER, CHILLEDWATER, CONDENSER, NONE};

PlantSizingType plantSizingType(const ModelObject & component)
{
  switch(component.iddObject().type().value())
  {
    case openstudio::IddObjectType::OS_Boiler_HotWater :
    {
      return PlantSizingType::HOTWATER;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Mixed :
    {
      return PlantSizingType::HOTWATER;
    }
    case openstudio::IddObjectType::OS_WaterHeater_Stratified :
    {
      return PlantSizingType::HOTWATER;
    }
    case openstudio::IddObjectType::OS_DistrictHeating :
    {
      return PlantSizingType::HOTWATER;
    }      
    case openstudio::IddObjectType::OS_Chiller_Electric_EIR :
    {
      return PlantSizingType::CHILLEDWATER;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect :
    {
      return PlantSizingType::CHILLEDWATER;
    }
    case openstudio::IddObjectType::OS_Chiller_Absorption :
    {
      return PlantSizingType::CHILLEDWATER;
    }
    case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed :
    {
      return PlantSizingType::CHILLEDWATER;
    }
    case openstudio::IddObjectType::OS_DistrictCooling :
    {
      return PlantSizingType::CHILLEDWATER;
    }      
    case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed :
    {
      return PlantSizingType::CONDENSER;
    }
    case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed :
    {
      return PlantSizingType::CONDENSER;
    }
    case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed:
    {
      return PlantSizingType::CONDENSER;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical :
    {
      return PlantSizingType::CONDENSER;
    }
    case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench :
    {
      return PlantSizingType::CONDENSER;
    }
    case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid :
    {
      return PlantSizingType::CONDENSER;
    }
    default:
    {
      return PlantSizingType::NONE;
    }
  }
}

boost::optional<IdfObject> ForwardTranslator::translateSizingPlant( SizingPlant & modelObject )
{
  // These will be used only if reasonable sizing values have not already been provided.
  auto condensorCheck = [](const ModelObject & comp) {
    return (plantSizingType(comp) == PlantSizingType::CONDENSER);
  };

  auto chilledWaterCheck = [](const ModelObject & comp) {
    return (plantSizingType(comp) == PlantSizingType::CHILLEDWATER);
  };

  if( (modelObject.designLoopExitTemperature() < 0.01) && (modelObject.loopDesignTemperatureDifference() < 0.01) )
  {
    const auto & components = modelObject.plantLoop().supplyComponents(); 
    if( std::find_if(components.begin(),components.end(),condensorCheck) != components.end() ) {
      modelObject.setLoopType("Condenser");
      modelObject.setDesignLoopExitTemperature(29.4);
      modelObject.setLoopDesignTemperatureDifference(5.6);
    } else if( std::find_if(components.begin(),components.end(),chilledWaterCheck) != components.end() ) {
      modelObject.setLoopType("Cooling");
      modelObject.setDesignLoopExitTemperature(7.22);
      modelObject.setLoopDesignTemperatureDifference(6.67);
    } else {
      modelObject.setLoopType("Heating");
      modelObject.setDesignLoopExitTemperature(82.0);
      modelObject.setLoopDesignTemperatureDifference(11.0);
    }
  }

  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Sizing_Plant);

  // PlantorCondenserLoopName

  s = modelObject.plantLoop().name();
  if( s )
  {
    idfObject.setString(Sizing_PlantFields::PlantorCondenserLoopName,s.get());
  }

  // LoopType

  s = modelObject.loopType();
  if( s )
  {
    idfObject.setString(Sizing_PlantFields::LoopType,s.get());
  }
  
  // DesignLoopExitTemperature

  value = modelObject.designLoopExitTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_PlantFields::DesignLoopExitTemperature,value.get());
  }

  // LoopDesignTemperatureDifference

  value = modelObject.loopDesignTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(Sizing_PlantFields::LoopDesignTemperatureDifference,value.get());
  }

  // SizingOption
  s = modelObject.sizingOption();
  if( s ) {
    idfObject.setString(Sizing_PlantFields::SizingOption,s.get());
  }

  // ZoneTimestepsinAveragingWindow
  value = modelObject.zoneTimestepsinAveragingWindow();
  if( value ) {
    idfObject.setDouble(Sizing_PlantFields::ZoneTimestepsinAveragingWindow,value.get());
  }

  // CoincidentSizingFactorMode
  s = modelObject.coincidentSizingFactorMode();
  if( s ) {
    idfObject.setString(Sizing_PlantFields::CoincidentSizingFactorMode,s.get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

