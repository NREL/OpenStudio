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
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpPlantLoopEIRHeating(HeatPumpPlantLoopEIRHeating& modelObject) {
    // createRegisterAndNameIdfObject will add it to m_map, m_idfObjects, and name it
    // The fact that it is added to m_map will avoid a recursion issue when it has a companion cooling coil (both FT methods call each other)
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_PlantLoop_EIR_Heating, modelObject);

    boost::optional<double> optvalue;

    if (auto value = modelObject.supplyOutletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.supplyInletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, value->name().get());
    }

    std::string condenserType = modelObject.condenserType();
    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, condenserType);

    if (openstudio::istringEqual(condenserType, "WaterSource")) {

      if (auto value = modelObject.demandOutletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, value->name().get());
      }

      if (auto value = modelObject.demandInletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, value->name().get());
      }

    } else {
      // AirSource
      {
        // Name the source side outlet node
        auto name = modelObject.nameString() + " Outlet Node For Source Side";
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, name);
      }

      {
        // Create an OutdoorAir:NodeList for the source side inlet conditions to be set directly from weather file
        IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
        auto name = modelObject.nameString() + " Inlet Node For Source Side";
        oaNodeListIdf.setString(0, name);
        m_idfObjects.push_back(oaNodeListIdf);
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, name);
      }
    }

    boost::optional<HeatPumpPlantLoopEIRCooling> companion = modelObject.companionCoolingHeatPump();
    if (companion) {
      boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
      if (_companion) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName, _companion->name().get());
      }
    }

    if (modelObject.isLoadSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.loadSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.isSourceSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.sourceSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.heatRecoveryLoop()) {
      if (modelObject.isHeatRecoveryReferenceFlowRateAutosized()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryReferenceFlowRate, "Autosize");
      } else if ((optvalue = modelObject.heatRecoveryReferenceFlowRate())) {
        idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryReferenceFlowRate, optvalue.get());
      }

      if (boost::optional<ModelObject> mo = modelObject.tertiaryInletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryInletNodeName, node->name().get());
        }
      }

      if (boost::optional<ModelObject> mo = modelObject.tertiaryOutletModelObject()) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryOutletNodeName, node->name().get());
        }
      }
    }

    if (modelObject.isReferenceCapacityAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, "Autosize");
    } else if ((optvalue = modelObject.referenceCapacity())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, optvalue.get());
    }

    {
      auto value = modelObject.referenceCoefficientofPerformance();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, value);
    }

    {
      auto curve = modelObject.capacityModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName, _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                            _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::HeatingToCoolingCapacitySizingRatio, modelObject.heatingToCoolingCapacitySizingRatio());

    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpSizingMethod, modelObject.heatPumpSizingMethod());

    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ControlType, modelObject.controlType());

    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::FlowMode, modelObject.flowMode());

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumPartLoadRatio, modelObject.minimumPartLoadRatio());

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumSourceInletTemperature, modelObject.minimumSourceInletTemperature());

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::MaximumSourceInletTemperature, modelObject.maximumSourceInletTemperature());

    if (boost::optional<model::Curve> curve = modelObject.minimumSupplyWaterTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::MinimumSupplyWaterTemperatureCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.maximumSupplyWaterTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::MaximumSupplyWaterTemperatureCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.dryOutdoorCorrectionFactorCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::DryOutdoorCorrectionFactorCurveName, _curve->name().get());
      }
    }

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::MaximumOutdoorDryBulbTemperatureForDefrostOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureForDefrostOperation());

    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostControl, modelObject.heatPumpDefrostControl());

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostTimePeriodFraction, modelObject.heatPumpDefrostTimePeriodFraction());

    if (boost::optional<model::Curve> curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.timedEmpiricalDefrostFrequencyCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostFrequencyCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.timedEmpiricalDefrostHeatLoadPenaltyCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatLoadPenaltyCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.timedEmpiricalDefrostHeatInputEnergyFractionCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatInputEnergyFractionCurveName, _curve->name().get());
      }
    }

    idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumHeatRecoveryOutletTemperature,
                        modelObject.minimumHeatRecoveryOutletTemperature());

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryCapacityModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                            _curve->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
