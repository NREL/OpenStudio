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

