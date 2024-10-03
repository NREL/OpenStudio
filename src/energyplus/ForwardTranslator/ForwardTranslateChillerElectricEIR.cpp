/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/ChillerElectricEIR_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Chiller_Electric_EIR_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateChillerElectricEIR(ChillerElectricEIR& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Chiller_Electric_EIR);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // ReferenceCapacity

    if (modelObject.isReferenceCapacityAutosized()) {
      idfObject.setString(Chiller_Electric_EIRFields::ReferenceCapacity, "Autosize");
    } else if ((value = modelObject.referenceCapacity())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCapacity, value.get());
    }

    // ReferenceCOP

    if ((value = modelObject.referenceCOP())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCOP, value.get());
    }

    // ReferenceLeavingChilledWaterTemperature

    if ((value = modelObject.referenceLeavingChilledWaterTemperature())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, value.get());
    }

    // ReferenceEnteringCondenserFluidTemperature

    if ((value = modelObject.referenceEnteringCondenserFluidTemperature())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature, value.get());
    }

    // ReferenceChilledWaterFlowRate

    if (modelObject.isReferenceChilledWaterFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, "Autosize");
    } else if ((value = modelObject.referenceChilledWaterFlowRate())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, value.get());
    }

    // ReferenceCondenserFluidFlowRate

    if (modelObject.isReferenceCondenserFluidFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, "Autosize");
    } else if ((value = modelObject.referenceCondenserFluidFlowRate())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, value.get());
    }

    // MinimumPartLoadRatio

    if ((value = modelObject.minimumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::MinimumPartLoadRatio, value.get());
    }

    // MaximumPartLoadRatio

    if ((value = modelObject.maximumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::MaximumPartLoadRatio, value.get());
    }

    // OptimumPartLoadRatio

    if ((value = modelObject.optimumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::OptimumPartLoadRatio, value.get());
    }

    // MinimumUnloadingRatio

    if ((value = modelObject.minimumUnloadingRatio())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::MinimumUnloadingRatio, value.get());
    }

    // ChilledWaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.supplyInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_EIRFields::ChilledWaterInletNodeName, node->name().get());
      }
    }

    // ChilledWaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.supplyOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_EIRFields::ChilledWaterOutletNodeName, node->name().get());
      }
    }

    // CondenserInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.demandInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_EIRFields::CondenserInletNodeName, node->name().get());
      }
    }

    // CondenserOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.demandOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_EIRFields::CondenserOutletNodeName, node->name().get());
      }
    }

    // CondenserType
    // The "smart" logic is now handled in model itself
    // (eg: if you connect the chiller to a secondaryPlantLoop, it switches automatically to "WaterCooled")
    idfObject.setString(Chiller_Electric_EIRFields::CondenserType, modelObject.condenserType());

    if (!openstudio::istringEqual(modelObject.condenserType(), "WaterCooled")) {
      {
        // Create an OutdoorAir:NodeList for the condenser inlet conditions to be set directly from weather file
        IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
        auto name = modelObject.nameString() + " Inlet Node For Condenser";
        oaNodeListIdf.setString(0, name);
        m_idfObjects.push_back(oaNodeListIdf);
        idfObject.setString(Chiller_Electric_EIRFields::CondenserInletNodeName, name);
      }

      {
        // Name the condenser outlet node
        auto name = modelObject.nameString() + " Outlet Node For Condenser";
        idfObject.setString(Chiller_Electric_EIRFields::CondenserOutletNodeName, name);
      }
    }

    // CondenserFanPowerRatio

    if ((value = modelObject.condenserFanPowerRatio())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::CondenserFanPowerRatio, value.get());
    }

    // CompressorMotorEfficiency
    // Changed to Fraction of Compressor Electric Consumption Rejected by Condenser in E+ version 8.0

    if ((value = modelObject.fractionofCompressorElectricConsumptionRejectedbyCondenser())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, value.get());
    }

    // LeavingChilledWaterLowerTemperatureLimit

    if ((value = modelObject.leavingChilledWaterLowerTemperatureLimit())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, value.get());
    }

    // ChillerFlowMode

    if ((s = modelObject.chillerFlowMode())) {
      idfObject.setString(Chiller_Electric_EIRFields::ChillerFlowMode, s.get());
    }

    // CoolingCapacityFunctionofTemperatureCurveName

    Curve curve = modelObject.coolingCapacityFunctionOfTemperature();

    boost::optional<IdfObject> _curve = translateAndMapModelObject(curve);

    if (_curve) {
      idfObject.setString(Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName, _curve->name().get());
    }

    // ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName

    curve = modelObject.electricInputToCoolingOutputRatioFunctionOfTemperature();

    _curve = translateAndMapModelObject(curve);

    if (_curve) {
      idfObject.setString(Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, _curve->name().get());
    }

    // ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName

    Curve quadcurve = modelObject.electricInputToCoolingOutputRatioFunctionOfPLR();

    _curve = translateAndMapModelObject(quadcurve);

    if (_curve) {
      idfObject.setString(Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, _curve->name().get());
    }

    // DesignHeatRecoveryWaterFlowRate: If filled, then the Nodes are **required**...
    if (modelObject.heatRecoveryLoop()) {
      if (modelObject.isDesignHeatRecoveryWaterFlowRateAutosized()) {
        idfObject.setString(Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, "Autosize");
      } else if ((value = modelObject.designHeatRecoveryWaterFlowRate())) {
        idfObject.setDouble(Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, value.get());
      }

      // HeatRecoveryInletNodeName
      if (boost::optional<ModelObject> mo = modelObject.tertiaryInletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(Chiller_Electric_EIRFields::HeatRecoveryInletNodeName, node->name().get());
        }
      }

      // HeatRecoveryOutletNodeName
      if (boost::optional<ModelObject> mo = modelObject.tertiaryOutletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName, node->name().get());
        }
      }
    }

    // Sizing Factor
    if ((value = modelObject.sizingFactor())) {
      idfObject.setDouble(Chiller_Electric_EIRFields::SizingFactor, value.get());
    }

    // Basin Heater Capacity
    idfObject.setDouble(Chiller_Electric_EIRFields::BasinHeaterCapacity, modelObject.basinHeaterCapacity());

    // Basin Heater Setpoint Temperature
    idfObject.setDouble(Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature, modelObject.basinHeaterSetpointTemperature());

    // Basin Heater Operating Schedule Name
    if (auto _schedule = modelObject.basinHeaterSchedule()) {
      idfObject.setString(Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName, _schedule->name().get());
    }

    // Condenser Heat Recovery Relative Capacity Fraction
    idfObject.setDouble(Chiller_Electric_EIRFields::CondenserHeatRecoveryRelativeCapacityFraction,
                        modelObject.condenserHeatRecoveryRelativeCapacityFraction());

    // Heat Recovery Inlet High Temperature Limit Schedule Name
    if (auto _schedule = modelObject.heatRecoveryInletHighTemperatureLimitSchedule()) {
      idfObject.setString(Chiller_Electric_EIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, _schedule->name().get());
    }

    // Heat Recovery Leaving Temperature Setpoint Node Name
    if (auto _node = modelObject.heatRecoveryLeavingTemperatureSetpointNode()) {
      idfObject.setString(Chiller_Electric_EIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, _node->name().get());
    }

    // End Use Subcategory
    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(Chiller_Electric_EIRFields::EndUseSubcategory, s.get());
    }

    // Condenser Flow Control
    idfObject.setString(Chiller_Electric_EIRFields::CondenserFlowControl, modelObject.condenserFlowControl());

    // Condenser Loop Flow Rate Fraction Function of Loop Part Load Ratio Curve Name
    if (boost::optional<Curve> condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve_ =
          modelObject.condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve_.get())) {
        idfObject.setString(Chiller_Electric_EIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName, wo_->nameString());
      }
    }

    // Temperature Difference Across Condenser Schedule Name
    if (auto _schedule = modelObject.temperatureDifferenceAcrossCondenserSchedule()) {
      idfObject.setString(Chiller_Electric_EIRFields::TemperatureDifferenceAcrossCondenserScheduleName, _schedule->name().get());
    }

    // Condenser Minimum Flow Fraction
    idfObject.setDouble(Chiller_Electric_EIRFields::CondenserMinimumFlowFraction, modelObject.condenserMinimumFlowFraction());

    // Thermosiphon Capacity Fraction Curve Name
    if (boost::optional<Curve> thermosiphonCapacityFractionCurve_ = modelObject.thermosiphonCapacityFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(thermosiphonCapacityFractionCurve_.get())) {
        idfObject.setString(Chiller_Electric_EIRFields::ThermosiphonCapacityFractionCurveName, wo_->nameString());
      }
    }

    // Thermosiphon Minimum Temperature Difference
    idfObject.setDouble(Chiller_Electric_EIRFields::ThermosiphonMinimumTemperatureDifference, modelObject.thermosiphonMinimumTemperatureDifference());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
