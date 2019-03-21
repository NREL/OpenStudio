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
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSizingSystem( SizingSystem & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Sizing_System);

  m_idfObjects.push_back(idfObject);

  // AirLoopName

  s = modelObject.airLoopHVAC().name();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::AirLoopName,s.get());
  }

  // TypeofLoadtoSizeOn

  s = modelObject.typeofLoadtoSizeOn();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::TypeofLoadtoSizeOn,s.get());
  }

  // DesignOutdoorAirFlowRate

  if( modelObject.isDesignOutdoorAirFlowRateAutosized() )
  {
    idfObject.setString(Sizing_SystemFields::DesignOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.designOutdoorAirFlowRate()) )
  {
    idfObject.setDouble(Sizing_SystemFields::DesignOutdoorAirFlowRate,value.get());
  }

  // CentralHeatingMaximumSystemAirFlowRatio
  if( modelObject.isCentralHeatingMaximumSystemAirFlowRatioAutosized() ) {
    idfObject.setString(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, "Autosize");
  } else if( (value = modelObject.centralHeatingMaximumSystemAirFlowRatio()) ) {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, value.get());

  }

  // PreheatDesignTemperature

  value = modelObject.preheatDesignTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PreheatDesignTemperature,value.get());
  }

  // PreheatDesignHumidityRatio

  value = modelObject.preheatDesignHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PreheatDesignHumidityRatio,value.get());
  }

  // PrecoolDesignTemperature

  value = modelObject.precoolDesignTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PrecoolDesignTemperature,value.get());
  }

  // PrecoolDesignHumidityRatio

  value = modelObject.precoolDesignHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio,value.get());
  }

  // CentralCoolingDesignSupplyAirTemperature

  value = modelObject.centralCoolingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature,value.get());
  }

  // CentralHeatingDesignSupplyAirTemperature

  value = modelObject.centralHeatingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature,value.get());
  }

  // SizingOption

  s = modelObject.sizingOption();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::TypeofZoneSumtoUse,s.get());
  }

  // AllOutdoorAirinCooling

  bool b = modelObject.allOutdoorAirinCooling();
  if( b )
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinCooling,"Yes");
  }
  else
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinCooling,"No");
  }

  // AllOutdoorAirinHeating

  b = modelObject.allOutdoorAirinHeating();
  if( b )
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinHeating,"Yes");
  }
  else
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinHeating,"No");
  }

  // CentralCoolingDesignSupplyAirHumidityRatio

  value = modelObject.centralCoolingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio,value.get());
  }

  // CentralHeatingDesignSupplyAirHumidityRatio

  value = modelObject.centralHeatingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio,value.get());
  }

  // CoolingDesignAirFlowMethod

  s = modelObject.coolingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::CoolingSupplyAirFlowRateMethod,s.get());
  }

  // CoolingDesignAirFlowRate

  value = modelObject.coolingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRate,value.get());
  }

  // HeatingDesignAirFlowMethod

  s = modelObject.heatingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::HeatingSupplyAirFlowRateMethod,s.get());
  }

  // HeatingDesignAirFlowRate

  value = modelObject.heatingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRate,value.get());
  }

  // SystemOutdoorAirMethod

  s = modelObject.systemOutdoorAirMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::SystemOutdoorAirMethod,s.get());
  }

  // ZoneMaximumOutdoorAirFraction
  value = modelObject.zoneMaximumOutdoorAirFraction();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::ZoneMaximumOutdoorAirFraction,value.get());
  }

  // CoolingSupplyAirFlowRatePerFloorArea
  value = modelObject.coolingSupplyAirFlowRatePerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea,value.get());
  }

  // CoolingFractionofAutosizedCoolingSupplyAirFlowRate
  value = modelObject.coolingFractionofAutosizedCoolingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate,value.get());
  }

  // CoolingSupplyAirFlowRatePerUnitCoolingCapacity
  value = modelObject.coolingSupplyAirFlowRatePerUnitCoolingCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity,value.get());
  }

  // HeatingSupplyAirFlowRatePerFloorArea
  value = modelObject.heatingSupplyAirFlowRatePerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea,value.get());
  }

  // HeatingFractionofAutosizedHeatingSupplyAirFlowRate
  value = modelObject.heatingFractionofAutosizedHeatingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate,value.get());
  }

  // HeatingFractionofAutosizedCoolingSupplyAirFlowRate
  value = modelObject.heatingFractionofAutosizedCoolingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate,value.get());
  }

  // HeatingSupplyAirFlowRatePerUnitHeatingCapacity
  value = modelObject.heatingSupplyAirFlowRatePerUnitHeatingCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity,value.get());
  }

  // CoolingDesignCapacityMethod
  s = modelObject.coolingDesignCapacityMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::CoolingDesignCapacityMethod,s.get());
  }

  // CoolingDesignCapacity
  value = modelObject.coolingDesignCapacity();
  if( modelObject.isCoolingDesignCapacityAutosized() ) {
    idfObject.setString(Sizing_SystemFields::CoolingDesignCapacity,"Autosize");
  } else if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingDesignCapacity,value.get());
  }


  // CoolingDesignCapacityPerFloorArea
  value = modelObject.coolingDesignCapacityPerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingDesignCapacityPerFloorArea,value.get());
  }

  // FractionofAutosizedCoolingDesignCapacity
  value = modelObject.fractionofAutosizedCoolingDesignCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity,value.get());
  }

  // HeatingDesignCapacityMethod
  s = modelObject.heatingDesignCapacityMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::HeatingDesignCapacityMethod,s.get());
  }

  // HeatingDesignCapacity
  value = modelObject.heatingDesignCapacity();
  if( modelObject.isHeatingDesignCapacityAutosized() ) {
    idfObject.setString(Sizing_SystemFields::HeatingDesignCapacity,"Autosize");
  } else if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingDesignCapacity,value.get());
  }

  // HeatingDesignCapacityPerFloorArea
  value = modelObject.heatingDesignCapacityPerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingDesignCapacityPerFloorArea,value.get());
  }

  // FractionofAutosizedHeatingDesignCapacity
  value = modelObject.fractionofAutosizedHeatingDesignCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity,value.get());
  }

  // CentralCoolingCapacityControlMethod
  s = modelObject.centralCoolingCapacityControlMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::CentralCoolingCapacityControlMethod,s.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

