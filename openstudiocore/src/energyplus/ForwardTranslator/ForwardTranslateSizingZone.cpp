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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"
#include "../../model/ControllerMechanicalVentilation_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSizingZone( SizingZone & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Sizing_Zone);

  m_idfObjects.push_back(idfObject);

  // ZoneorZoneListName

  model::ThermalZone thermalZone = modelObject.thermalZone();

  boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone);

  boost::optional<std::string> name;

  if( _thermalZone )
  {
    name = _thermalZone->name();
  }

  if( name )
  {
    idfObject.setString(Sizing_ZoneFields::ZoneorZoneListName,name.get());
  }

  // ZoneCoolingDesignSupplyAirTemperatureInputMethod
  {
    s = modelObject.zoneCoolingDesignSupplyAirTemperatureInputMethod();
    idfObject.setString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod,s.get());
  }

  // ZoneCoolingDesignSupplyAirTemperature
  value = modelObject.zoneCoolingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature,value.get());
  }

  // zoneCoolingDesignSupplyAirTemperatureDifference
  value = modelObject.zoneCoolingDesignSupplyAirTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference,value.get());
  }

  // ZoneHeatingDesignSupplyAirTemperatureInputMethod
  {
    s = modelObject.zoneHeatingDesignSupplyAirTemperatureInputMethod();
    idfObject.setString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod,s.get());
  }

  // ZoneHeatingDesignSupplyAirTemperature
  value = modelObject.zoneHeatingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature,value.get());
  }

  // ZoneHeatingDesignSupplyAirTemperatureDifference
  value = modelObject.zoneHeatingDesignSupplyAirTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference,value.get());
  }

  // ZoneCoolingDesignSupplyAirHumidityRatio
  
  value = modelObject.zoneCoolingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio,value.get());
  }

  // ZoneHeatingDesignSupplyAirHumidityRatio
  
  value = modelObject.zoneHeatingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio,value.get());
  }

  //  ((DesignSpecificationOutdoorAirObjectName)(Design Specification Outdoor Air Object Name))
  
  // ZoneHeatingSizingFactor 
  
  value = modelObject.zoneHeatingSizingFactor();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor,value.get());
  }
  
  // ZoneCoolingSizingFactor 
  
  value = modelObject.zoneCoolingSizingFactor();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingSizingFactor,value.get());
  }

  // CoolingDesignAirFlowMethod
  
  s = modelObject.coolingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_ZoneFields::CoolingDesignAirFlowMethod,s.get());
  }

  // CoolingDesignAirFlowRate
  
  value = modelObject.coolingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::CoolingDesignAirFlowRate,value.get());
  }

  // CoolingMinimumAirFlowperZoneFloorArea

  value = modelObject.coolingMinimumAirFlowperZoneFloorArea();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea,value.get());
  }

  // CoolingMinimumAirFlow

  value = modelObject.coolingMinimumAirFlow();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlow,value.get());
  }

  // CoolingMinimumAirFlowFraction

  value = modelObject.coolingMinimumAirFlowFraction();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction,value.get());
  }

  // HeatingDesignAirFlowMethod
  
  s = modelObject.heatingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_ZoneFields::HeatingDesignAirFlowMethod,s.get());
  }

  // HeatingDesignAirFlowRate
  
  value = modelObject.heatingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::HeatingDesignAirFlowRate,value.get());
  }

  // HeatingMaximumAirFlowperZoneFloorArea

  value = modelObject.heatingMaximumAirFlowperZoneFloorArea();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea,value.get());
  }

  // HeatingMaximumAirFlow

  value = modelObject.heatingMaximumAirFlow();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlow,value.get());
  }

  // HeatingMaximumAirFlowFraction

  value = modelObject.heatingMaximumAirFlowFraction();
  if( value )
  {
    idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction,value.get());
  }

  // DesignZoneAirDistributionEffectivenessinCoolingMode
  // DesignZoneAirDistributionEffectivenessinHeatingMode

  boost::optional<double> designZoneAirDistributionEffectivenessinCoolingMode = 
                            modelObject.designZoneAirDistributionEffectivenessinCoolingMode();
  boost::optional<double> designZoneAirDistributionEffectivenessinHeatingMode = 
                            modelObject.designZoneAirDistributionEffectivenessinHeatingMode();

  std::string designSpecificationZoneAirDistributionName;
  if( name )
  {
    designSpecificationZoneAirDistributionName = name.get() +  " Design Spec Zone Air Dist";
  }

  if( designZoneAirDistributionEffectivenessinCoolingMode ||
      designZoneAirDistributionEffectivenessinHeatingMode )
  {
    IdfObject _designSpecification_ZoneAirDistribution(IddObjectType::DesignSpecification_ZoneAirDistribution);

    if( name )
    {
      _designSpecification_ZoneAirDistribution.setName(designSpecificationZoneAirDistributionName);
    }

    m_idfObjects.push_back(_designSpecification_ZoneAirDistribution);

    if( designZoneAirDistributionEffectivenessinCoolingMode )
    {
      _designSpecification_ZoneAirDistribution.setDouble(
        DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode,
        designZoneAirDistributionEffectivenessinCoolingMode.get() );
    }

    if( designZoneAirDistributionEffectivenessinHeatingMode )
    {
      _designSpecification_ZoneAirDistribution.setDouble(
        DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode,
        designZoneAirDistributionEffectivenessinHeatingMode.get() );
    }

    idfObject.setString(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName,_designSpecification_ZoneAirDistribution.name().get());
  }


  // Add ThermalZone and associated design objects to ControllerMechanicalVentilation.
  // This would be done in forwardTranslateControllerMechanicalVentilation except doing it here maintains proper order of the idf file.
  
  boost::optional<model::ControllerMechanicalVentilation> controllerMechanicalVentilation;
  boost::optional<IdfObject> _controllerMechanicalVentilation;

  if( boost::optional<model::AirLoopHVAC> airLoopHVAC = thermalZone.airLoopHVAC() )
  {
    if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem() )
    {
      model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir(); 

      controllerMechanicalVentilation = controllerOutdoorAir.controllerMechanicalVentilation();
    }
  }
  
  if( controllerMechanicalVentilation )
  {
    _controllerMechanicalVentilation = translateAndMapModelObject(controllerMechanicalVentilation.get());
  }

  if( _controllerMechanicalVentilation && _thermalZone )
  {
    IdfExtensibleGroup eg = _controllerMechanicalVentilation->pushExtensibleGroup();

    // Thermal Zone Name
    eg.setString(Controller_MechanicalVentilationExtensibleFields::ZoneName,_thermalZone->name().get());

    // DesignSpecificationOutdoorAir
    std::vector<model::Space> spaces = thermalZone.spaces();

    if( spaces.size() > 0 )
    {
      if( boost::optional<model::DesignSpecificationOutdoorAir> designOASpec = spaces.front().designSpecificationOutdoorAir()  )
      {
        if( boost::optional<IdfObject> _designOASpec = translateAndMapModelObject(designOASpec.get()) )
        {
          eg.setString(Controller_MechanicalVentilationExtensibleFields::DesignSpecificationOutdoorAirObjectName,_designOASpec->name().get());
        }
      }
    }

    // DesignSpecificationZoneAirDistributionObjectName
    if( _thermalZone )
    {
      eg.setString(Controller_MechanicalVentilationExtensibleFields::DesignSpecificationZoneAirDistributionObjectName,
                   designSpecificationZoneAirDistributionName);
    }
  }

  if( modelObject.accountforDedicatedOutdoorAirSystem() ) {
    idfObject.setString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem,"Yes");

    {
      auto value = modelObject.dedicatedOutdoorAirSystemControlStrategy();
      idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy,value);
    }

    {
      if( modelObject.isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() ) {
        idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign,"Autosize");
      } else if ( auto value = modelObject.dedicatedOutdoorAirLowSetpointTemperatureforDesign() ) {
        idfObject.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign,value.get());
      }
    }

    {
      if( modelObject.isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() ) {
        idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign,"Autosize");
      } else if ( auto value = modelObject.dedicatedOutdoorAirHighSetpointTemperatureforDesign() ) {
        idfObject.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign,value.get());
      }
    }
  } else {
    idfObject.setString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem,"No");
  }

  return idfObject;
}

} // energyplus

} // openstudio

