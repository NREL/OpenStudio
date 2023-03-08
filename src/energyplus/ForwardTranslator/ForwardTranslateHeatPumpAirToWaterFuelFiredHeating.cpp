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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/HeatPump_AirToWater_FuelFired_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpAirToWaterFuelFiredHeating(model::HeatPumpAirToWaterFuelFiredHeating& modelObject) {
    boost::optional<IdfObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_AirToWater_FuelFired_Heating, modelObject);

    // Water Inlet Node Name
    _mo = modelObject.inletModelObject();
    if (_mo) {
      translateAndMapModelObject(*_mo);
      idfObject.setString(openstudio::HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName, _mo->nameString());
    }

    // Water Outlet Node Name
    _mo = modelObject.outletModelObject();
    if (_mo) {
      translateAndMapModelObject(*_mo);
      idfObject.setString(openstudio::HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName, _mo->nameString());
    }

    // Air Source Node Name
    std::string oaNodeName = modelObject.nameString() + " OA Node";
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName, oaNodeName);

    IdfObject _oaNode(openstudio::IddObjectType::OutdoorAir_Node);
    _oaNode.setString(0, oaNodeName);
    m_idfObjects.push_back(_oaNode);

    // Companion Cooling Heat Pump Name: Optional Object
    if (boost::optional<HeatPumpAirToWaterFuelFiredCooling> _companionCoolingHeatPump = modelObject.companionCoolingHeatPump()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_companionCoolingHeatPump.get())) {
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
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable,
                        outdoorAirTemperatureCurveInputVariable);

    // Water Temperature Curve Input Variable: Optional String
    std::string waterTemperatureCurveInputVariable = modelObject.waterTemperatureCurveInputVariable();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);

    // Normalized Capacity Function of Temperature Curve Name: Required Object
    Curve normalizedCapacityFunctionofTemperatureCurve = modelObject.normalizedCapacityFunctionofTemperatureCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedCapacityFunctionofTemperatureCurve)) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName, _owo->nameString());
    }

    // Fuel Energy Input Ratio Function of Temperature Curve Name: Required Object
    Curve fuelEnergyInputRatioFunctionofTemperatureCurve = modelObject.fuelEnergyInputRatioFunctionofTemperatureCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(fuelEnergyInputRatioFunctionofTemperatureCurve)) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName, _owo->nameString());
    }

    // Fuel Energy Input Ratio Function of PLR Curve Name: Required Object
    Curve fuelEnergyInputRatioFunctionofPLRCurve = modelObject.fuelEnergyInputRatioFunctionofPLRCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(fuelEnergyInputRatioFunctionofPLRCurve)) {
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
    if (boost::optional<Curve> _fuelEnergyInputRatioDefrostAdjustmentCurve = modelObject.fuelEnergyInputRatioDefrostAdjustmentCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_fuelEnergyInputRatioDefrostAdjustmentCurve.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName, _owo->nameString());
      }
    }

    // Resistive Defrost Heater Capacity: Optional Double
    double resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity);

    // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
    double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                        maximumOutdoorDrybulbTemperatureforDefrostOperation);

    // Cycling Ratio Factor Curve Name: Optional Object
    if (boost::optional<Curve> _cyclingRatioFactorCurve = modelObject.cyclingRatioFactorCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cyclingRatioFactorCurve.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, _owo->nameString());
      }
    }

    // Nominal Auxiliary Electric Power: Optional Double
    double nominalAuxiliaryElectricPower = modelObject.nominalAuxiliaryElectricPower();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, nominalAuxiliaryElectricPower);

    // Auxiliary Electric Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> _auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve =
          modelObject.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName,
                            _owo->nameString());
      }
    }

    // Auxiliary Electric Energy Input Ratio Function of PLR Curve Name: Optional Object
    if (boost::optional<Curve> _auxiliaryElectricEnergyInputRatioFunctionofPLRCurve =
          modelObject.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_auxiliaryElectricEnergyInputRatioFunctionofPLRCurve.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, _owo->nameString());
      }
    }

    // Standby Electric Power: Optional Double
    double standbyElectricPower = modelObject.standbyElectricPower();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower, standbyElectricPower);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio