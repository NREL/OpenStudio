/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/HeatPumpAirToWaterFuelFiredHeating.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/OutdoorAirNode.hpp"
#include "../../model/OutdoorAirNode_Impl.hpp"

#include "../../model/HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

#include "../../model/BivariateFunctions.hpp"
#include "../../model/BivariateFunctions_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include <utilities/idd/HeatPump_AirToWater_FuelFired_Heating_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatPumpAirToWaterFuelFiredHeating( model::HeatPumpAirToWaterFuelFiredHeating& modelObject )
{
  boost::optional<IdfObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an IdfObject of the class to store the values
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_AirToWater_FuelFired_Heating, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::HeatPump_AirToWater_FuelFired_Heating );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
  // But in some cases, you'll want to handle failure without pushing to the map
  // Name
  if (boost::optional<std::string> moName = modelObject.name()) {
    idfObject.setName(*moName);
  }

  // Water Inlet Node Name: Required Node
  Node waterInletNodeName = modelObject.waterInletNodeName();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(waterInletNodeName) )  {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName, _owo->nameString());
  }

  // Water Outlet Node Name: Required Node
  Node waterOutletNodeName = modelObject.waterOutletNodeName();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(waterOutletNodeName) )  {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName, _owo->nameString());
  }

  // Air Source Node Name: Optional Object
  if (boost::optional<OutdoorAirNode> _airSourceNode = modelObject.airSourceNode()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_airSourceNode.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName, _owo->nameString());
    }
  }

  // Companion Cooling Heat Pump Name: Optional Object
  if (boost::optional<HeatPumpAirToWaterFuelFiredCooling> _companionCoolingHeatPump = modelObject.companionCoolingHeatPump()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_companionCoolingHeatPump.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, _owo->nameString());
    }
  }

  // Fuel Type: Optional String
  std::string fuelType = modelObject.fuelType();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelType, fuelType);


  // End-Use Subcategory: Optional String
  std::string endUseSubcategory = modelObject.endUseSubcategory();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory, endUseSubcategory);


  if (modelObject.isNominalHeatingCapacityAutosized()) {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, "Autosize");
  } else {
    // Nominal Heating Capacity: boost::optional<double>
    if (boost::optional<double> _nominalHeatingCapacity = modelObject.nominalHeatingCapacity()) {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, _nominalHeatingCapacity.get());
    }
  }

  // Nominal COP: Optional Double
  double nominalCOP = modelObject.nominalCOP();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalCOP, nominalCOP);


  if (modelObject.isDesignFlowRateAutosized()) {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, "Autosize");
  } else {
    // Design Flow Rate: boost::optional<double>
    if (boost::optional<double> _designFlowRate = modelObject.designFlowRate()) {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, _designFlowRate.get());
    }
  }

  // Design Supply Temperature: Optional Double
  double designSupplyTemperature = modelObject.designSupplyTemperature();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignSupplyTemperature, designSupplyTemperature);


  if (modelObject.isDesignTemperatureLiftAutosized()) {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, "Autosize");
  } else {
    // Design Temperature Lift: boost::optional<double>
    if (boost::optional<double> _designTemperatureLift = modelObject.designTemperatureLift()) {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, _designTemperatureLift.get());
    }
  }

  // Sizing Factor: Optional Double
  double sizingFactor = modelObject.sizingFactor();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::SizingFactor, sizingFactor);


  // Flow Mode: Optional String
  std::string flowMode = modelObject.flowMode();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode, flowMode);


  // Outdoor Air Temperature Curve Input Variable: Optional String
  std::string outdoorAirTemperatureCurveInputVariable = modelObject.outdoorAirTemperatureCurveInputVariable();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable, outdoorAirTemperatureCurveInputVariable);


  // Water Temperature Curve Input Variable: Optional String
  std::string waterTemperatureCurveInputVariable = modelObject.waterTemperatureCurveInputVariable();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);


  // Normalized Capacity Function of Temperature Curve Name: Required Object
  BivariateFunctions normalizedCapacityFunctionofTemperatureCurve = modelObject.normalizedCapacityFunctionofTemperatureCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedCapacityFunctionofTemperatureCurve) )  {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName, _owo->nameString());
  }

  // Fuel Energy Input Ratio Function of Temperature Curve Name: Required Object
  BivariateFunctions fuelEnergyInputRatioFunctionofTemperatureCurve = modelObject.fuelEnergyInputRatioFunctionofTemperatureCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(fuelEnergyInputRatioFunctionofTemperatureCurve) )  {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName, _owo->nameString());
  }

  // Fuel Energy Input Ratio Function of PLR Curve Name: Required Object
  UnivariateFunctions fuelEnergyInputRatioFunctionofPLRCurve = modelObject.fuelEnergyInputRatioFunctionofPLRCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(fuelEnergyInputRatioFunctionofPLRCurve) )  {
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName, _owo->nameString());
  }

  // Minimum Part Load Ratio: Optional Double
  double minimumPartLoadRatio = modelObject.minimumPartLoadRatio();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MinimumPartLoadRatio, minimumPartLoadRatio);


  // Maximum Part Load Ratio: Optional Double
  double maximumPartLoadRatio = modelObject.maximumPartLoadRatio();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MaximumPartLoadRatio, maximumPartLoadRatio);


  // Defrost Control Type: Optional String
  std::string defrostControlType = modelObject.defrostControlType();
  idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType, defrostControlType);


  // Defrost Operation Time Fraction: Optional Double
  double defrostOperationTimeFraction = modelObject.defrostOperationTimeFraction();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DefrostOperationTimeFraction, defrostOperationTimeFraction);


  // Fuel Energy Input Ratio Defrost Adjustment Curve Name: Optional Object
  if (boost::optional<UnivariateFunctions> _fuelEnergyInputRatioDefrostAdjustmentCurve = modelObject.fuelEnergyInputRatioDefrostAdjustmentCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_fuelEnergyInputRatioDefrostAdjustmentCurve.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName, _owo->nameString());
    }
  }

  // Resistive Defrost Heater Capacity: Optional Double
  double resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity);


  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
  double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);


  // Cycling Ratio Factor Curve Name: Optional Object
  if (boost::optional<UnivariateFunctions> _cyclingRatioFactorCurve = modelObject.cyclingRatioFactorCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_cyclingRatioFactorCurve.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, _owo->nameString());
    }
  }

  // Nominal Auxiliary Electric Power: boost::optional<double>
  if (boost::optional<double> _nominalAuxiliaryElectricPower = modelObject.nominalAuxiliaryElectricPower()) {
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, _nominalAuxiliaryElectricPower.get());
  }

  // Auxiliary Electric Energy Input Ratio Function of Temperature Curve Name: Optional Object
  if (boost::optional<BivariateFunctions> _auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve = modelObject.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, _owo->nameString());
    }
  }

  // Auxiliary Electric Energy Input Ratio Function of PLR Curve Name: Optional Object
  if (boost::optional<UnivariateFunctions> _auxiliaryElectricEnergyInputRatioFunctionofPLRCurve = modelObject.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_auxiliaryElectricEnergyInputRatioFunctionofPLRCurve.get()) )  {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, _owo->nameString());
    }
  }

  // Standby Electric Power: Optional Double
  double standbyElectricPower = modelObject.standbyElectricPower();
  idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower, standbyElectricPower);


  return idfObject;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio