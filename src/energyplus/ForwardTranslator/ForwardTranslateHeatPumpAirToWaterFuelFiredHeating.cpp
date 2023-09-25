/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpAirToWaterFuelFiredHeating(model::HeatPumpAirToWaterFuelFiredHeating& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_AirToWater_FuelFired_Heating, modelObject);

    // Water Inlet Node Name: Required Node
    if (auto node_ = modelObject.inletModelObject()) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName, node_->nameString());
    }

    // Water Outlet Node Name: Required Node
    if (auto node_ = modelObject.outletModelObject()) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName, node_->nameString());
    }

    // Air Source Node Name
    const std::string oaNodeName = modelObject.nameString() + " OA Node";
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName, oaNodeName);

    IdfObject _oaNode(openstudio::IddObjectType::OutdoorAir_Node);
    _oaNode.setString(0, oaNodeName);
    m_idfObjects.push_back(_oaNode);

    // Companion Cooling Heat Pump Name: Optional Object
    if (boost::optional<HeatPumpAirToWaterFuelFiredCooling> companionCoolingHeatPump_ = modelObject.companionCoolingHeatPump()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(companionCoolingHeatPump_.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, wo_->nameString());
      }
    }

    // Fuel Type: Optional String
    const std::string fuelType = modelObject.fuelType();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelType, fuelType);

    // End-Use Subcategory: Optional String
    const std::string endUseSubcategory = modelObject.endUseSubcategory();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory, endUseSubcategory);

    // Nominal Heating Capacity
    if (modelObject.isNominalHeatingCapacityAutosized()) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, "Autosize");
    } else {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, modelObject.nominalHeatingCapacity().get());
    }

    // Nominal COP: Optional Double
    const double nominalCOP = modelObject.nominalCOP();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalCOP, nominalCOP);

    // Design Flow Rate
    if (modelObject.isDesignFlowRateAutosized()) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, "Autosize");
    } else {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, modelObject.designFlowRate().get());
    }

    // Design Supply Temperature: Optional Double
    const double designSupplyTemperature = modelObject.designSupplyTemperature();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignSupplyTemperature, designSupplyTemperature);

    if (modelObject.isDesignTemperatureLiftAutosized()) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, "Autosize");
    } else {
      idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, modelObject.designTemperatureLift().get());
    }

    // Sizing Factor: Optional Double
    const double sizingFactor = modelObject.sizingFactor();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::SizingFactor, sizingFactor);

    // Flow Mode: Optional String
    const std::string flowMode = modelObject.flowMode();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode, flowMode);

    // Outdoor Air Temperature Curve Input Variable: Optional String
    const std::string outdoorAirTemperatureCurveInputVariable = modelObject.outdoorAirTemperatureCurveInputVariable();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable,
                        outdoorAirTemperatureCurveInputVariable);

    // Water Temperature Curve Input Variable: Optional String
    const std::string waterTemperatureCurveInputVariable = modelObject.waterTemperatureCurveInputVariable();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);

    // Normalized Capacity Function of Temperature Curve Name: Required Object
    Curve normalizedCapacityFunctionofTemperatureCurve = modelObject.normalizedCapacityFunctionofTemperatureCurve();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(normalizedCapacityFunctionofTemperatureCurve)) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName, wo_->nameString());
    }

    // Fuel Energy Input Ratio Function of Temperature Curve Name: Required Object
    Curve fuelEnergyInputRatioFunctionofTemperatureCurve = modelObject.fuelEnergyInputRatioFunctionofTemperatureCurve();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(fuelEnergyInputRatioFunctionofTemperatureCurve)) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName, wo_->nameString());
    }

    // Fuel Energy Input Ratio Function of PLR Curve Name: Required Object
    Curve fuelEnergyInputRatioFunctionofPLRCurve = modelObject.fuelEnergyInputRatioFunctionofPLRCurve();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(fuelEnergyInputRatioFunctionofPLRCurve)) {
      idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName, wo_->nameString());
    }

    // Minimum Part Load Ratio: Optional Double
    const double minimumPartLoadRatio = modelObject.minimumPartLoadRatio();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MinimumPartLoadRatio, minimumPartLoadRatio);

    // Maximum Part Load Ratio: Optional Double
    const double maximumPartLoadRatio = modelObject.maximumPartLoadRatio();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MaximumPartLoadRatio, maximumPartLoadRatio);

    // Defrost Control Type: Optional String
    const std::string defrostControlType = modelObject.defrostControlType();
    idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType, defrostControlType);

    // Defrost Operation Time Fraction: Optional Double
    const double defrostOperationTimeFraction = modelObject.defrostOperationTimeFraction();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::DefrostOperationTimeFraction, defrostOperationTimeFraction);

    // Fuel Energy Input Ratio Defrost Adjustment Curve Name: Optional Object
    if (boost::optional<Curve> fuelEnergyInputRatioDefrostAdjustmentCurve_ = modelObject.fuelEnergyInputRatioDefrostAdjustmentCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(fuelEnergyInputRatioDefrostAdjustmentCurve_.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName, wo_->nameString());
      }
    }

    // Resistive Defrost Heater Capacity: Optional Double
    const double resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity);

    // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
    const double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                        maximumOutdoorDrybulbTemperatureforDefrostOperation);

    // Cycling Ratio Factor Curve Name: Optional Object
    if (boost::optional<Curve> cyclingRatioFactorCurve_ = modelObject.cyclingRatioFactorCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(cyclingRatioFactorCurve_.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, wo_->nameString());
      }
    }

    // Nominal Auxiliary Electric Power: Optional Double
    const double nominalAuxiliaryElectricPower = modelObject.nominalAuxiliaryElectricPower();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, nominalAuxiliaryElectricPower);

    // Auxiliary Electric Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Auxiliary Electric Energy Input Ratio Function of PLR Curve Name: Optional Object
    if (boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofPLRCurve_ =
          modelObject.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(auxiliaryElectricEnergyInputRatioFunctionofPLRCurve_.get())) {
        idfObject.setString(HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, wo_->nameString());
      }
    }

    // Standby Electric Power: Optional Double
    const double standbyElectricPower = modelObject.standbyElectricPower();
    idfObject.setDouble(HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower, standbyElectricPower);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
