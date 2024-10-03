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
#include "../../model/ChillerElectricReformulatedEIR.hpp"
#include "../../model/ChillerElectricReformulatedEIR_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Chiller_Electric_ReformulatedEIR_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateChillerElectricReformulatedEIR(ChillerElectricReformulatedEIR& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Chiller_Electric_ReformulatedEIR);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // ReferenceCapacity
    if (modelObject.isReferenceCapacityAutosized()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, "Autosize");
    } else if ((value = modelObject.referenceCapacity())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, value.get());
    }

    // ReferenceCOP
    if ((value = modelObject.referenceCOP())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCOP, value.get());
    }

    // ReferenceLeavingChilledWaterTemperature
    if ((value = modelObject.referenceLeavingChilledWaterTemperature())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, value.get());
    }

    // ReferenceLeavingCondenserWaterTemperature
    if ((value = modelObject.referenceLeavingCondenserWaterTemperature())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature, value.get());
    }

    // ReferenceChilledWaterFlowRate
    if (modelObject.isReferenceChilledWaterFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, "Autosize");
    } else if ((value = modelObject.referenceChilledWaterFlowRate())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, value.get());
    }

    // ReferenceCondenserWaterFlowRate
    if (modelObject.isReferenceCondenserWaterFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, "Autosize");
    } else if ((value = modelObject.referenceCondenserWaterFlowRate())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, value.get());
    }

    // CoolingCapacityFunctionofTemperatureCurveName
    {
      auto curve = modelObject.coolingCapacityFunctionOfTemperature();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName
    {
      auto curve = modelObject.electricInputToCoolingOutputRatioFunctionOfTemperature();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType
    if ((s = modelObject.electricInputToCoolingOutputRatioFunctionOfPLRType())) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType, s.get());
    }

    // ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName
    {
      auto curve = modelObject.electricInputToCoolingOutputRatioFunctionOfPLR();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName,
                            _curve->name().get());
      }
    }

    // MinimumPartLoadRatio
    if ((value = modelObject.minimumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio, value.get());
    }

    // MaximumPartLoadRatio
    if ((value = modelObject.maximumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio, value.get());
    }

    // OptimumPartLoadRatio
    if ((value = modelObject.optimumPartLoadRatio())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio, value.get());
    }

    // MinimumUnloadingRatio
    if ((value = modelObject.minimumUnloadingRatio())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio, value.get());
    }

    // ChilledWaterInletNodeName
    if (boost::optional<ModelObject> mo = modelObject.supplyInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ChilledWaterInletNodeName, node->name().get());
      }
    }

    // ChilledWaterOutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.supplyOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ChilledWaterOutletNodeName, node->name().get());
      }
    }

    // CondenserInletNodeName
    if (boost::optional<ModelObject> mo = modelObject.demandInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::CondenserInletNodeName, node->name().get());
      }
    }

    // CondenserOutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.demandOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::CondenserOutletNodeName, node->name().get());
      }
    }

    // FractionofCompressorElectricConsumptionRejectedbyCondenser
    if ((value = modelObject.fractionofCompressorElectricConsumptionRejectedbyCondenser())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, value.get());
    }

    // LeavingChilledWaterLowerTemperatureLimit
    if ((value = modelObject.leavingChilledWaterLowerTemperatureLimit())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, value.get());
    }

    // ChillerFlowModeType
    if ((s = modelObject.chillerFlowMode())) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ChillerFlowModeType, s.get());
    }

    // DesignHeatRecoveryWaterFlowRate: If filled, then the Nodes are **required**...
    if (modelObject.heatRecoveryLoop()) {
      if (modelObject.isDesignHeatRecoveryWaterFlowRateAutosized()) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, "Autosize");
      } else if ((value = modelObject.designHeatRecoveryWaterFlowRate())) {
        idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, value.get());
      }

      // HeatRecoveryInletNodeName
      if (boost::optional<ModelObject> mo = modelObject.tertiaryInletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletNodeName, node->name().get());
        }
      }

      // HeatRecoveryOutletNodeName
      if (boost::optional<ModelObject> mo = modelObject.tertiaryOutletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryOutletNodeName, node->name().get());
        }
      }
    }

    // SizingFactor
    if ((value = modelObject.sizingFactor())) {
      idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::SizingFactor, value.get());
    }

    // CondenserHeatRecoveryRelativeCapacityFraction
    idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::CondenserHeatRecoveryRelativeCapacityFraction,
                        modelObject.condenserHeatRecoveryRelativeCapacityFraction());

    // HeatRecoveryInletHighTemperatureLimitSchedule Name
    if (auto _schedule = modelObject.heatRecoveryInletHighTemperatureLimitSchedule()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, _schedule->name().get());
    }

    // HeatRecoveryLeavingTemperatureSetpointNodeName
    if (auto _node = modelObject.heatRecoveryLeavingTemperatureSetpointNode()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, _node->name().get());
    }

    // EndUseSubcategory
    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::EndUseSubcategory, s.get());
    }

    // Condenser Flow Control
    idfObject.setString(Chiller_Electric_ReformulatedEIRFields::CondenserFlowControl, modelObject.condenserFlowControl());

    // Condenser Loop Flow Rate Fraction Function of Loop Part Load Ratio Curve Name
    if (boost::optional<Curve> condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve_ =
          modelObject.condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve_.get())) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName,
                            wo_->nameString());
      }
    }

    // Temperature Difference Across Condenser Schedule Name
    if (auto _schedule = modelObject.temperatureDifferenceAcrossCondenserSchedule()) {
      idfObject.setString(Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName, _schedule->name().get());
    }

    // Condenser Minimum Flow Fraction
    idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::CondenserMinimumFlowFraction, modelObject.condenserMinimumFlowFraction());

    // Thermosiphon Capacity Fraction Curve Name
    if (boost::optional<Curve> thermosiphonCapacityFractionCurve_ = modelObject.thermosiphonCapacityFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(thermosiphonCapacityFractionCurve_.get())) {
        idfObject.setString(Chiller_Electric_ReformulatedEIRFields::ThermosiphonCapacityFractionCurveName, wo_->nameString());
      }
    }

    // Thermosiphon Minimum Temperature Difference
    idfObject.setDouble(Chiller_Electric_ReformulatedEIRFields::ThermosiphonMinimumTemperatureDifference,
                        modelObject.thermosiphonMinimumTemperatureDifference());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
