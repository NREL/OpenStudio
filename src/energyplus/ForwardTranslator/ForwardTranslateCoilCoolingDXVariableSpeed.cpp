/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXVariableSpeedWithoutUnitary(CoilCoolingDXVariableSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    if (modelObject.speeds().empty()) {
      // TODO: if it has zero speed data, Log and don't translate
    }
    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX_VariableSpeed, modelObject);

    // IndoorAirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, node->name().get());
    }

    // IndoorAirOutletNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, node->name().get());
    }

    // NominalSpeedLevel
    if (auto speedLevel = modelObject.nominalSpeedLevel()) {
      idfObject.setInt(Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel, speedLevel);
    }

    // GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel
    if (modelObject.isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized()) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, "AutoSize");
    } else if ((value = modelObject.grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, value.get());
    }

    // RatedAirFlowRateAtSelectedNominalSpeedLevel
    if (modelObject.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized()) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "AutoSize");
    } else if ((value = modelObject.ratedAirFlowRateAtSelectedNominalSpeedLevel())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, value.get());
    }

    // NominalTimeforCondensatetoBeginLeavingtheCoil
    if ((value = modelObject.nominalTimeforCondensatetoBeginLeavingtheCoil())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil, value.get());
    }

    // InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity
    if ((value = modelObject.initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity, value.get());
    }

    // EnergyPartLoadFractionCurveName
    {
      auto curve = modelObject.energyPartLoadFractionCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName, _curve->name().get());
      }
    }

    // CondenserAirInletNodeName

    // CondenserType
    if ((s = modelObject.condenserType())) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::CondenserType, s.get());
    }

    // EvaporativeCondenserPumpRatedPowerConsumption
    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "AutoSize");
    } else if ((value = modelObject.evaporativeCondenserPumpRatedPowerConsumption())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, value.get());
    }

    // CrankcaseHeaterCapacity
    if ((value = modelObject.crankcaseHeaterCapacity())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value.get());
    }

    // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
    if ((value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, value.get());
    }

    // SupplyWaterStorageTankName

    // CondensateCollectionWaterStorageTankName

    // BasinHeaterCapacity
    if ((value = modelObject.basinHeaterCapacity())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity, value.get());
    }

    // BasinHeaterSetpointTemperature
    if ((value = modelObject.basinHeaterSetpointTemperature())) {
      idfObject.setDouble(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature, value.get());
    }

    // BasinHeaterOperatingScheduleName
    if (auto schedule = modelObject.basinHeaterOperatingSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName, _schedule->name().get());
      }
    }

    auto const speeds = modelObject.speeds();

    // NumberofSpeeds
    if (auto num = speeds.size()) {
      idfObject.setInt(Coil_Cooling_DX_VariableSpeedFields::NumberofSpeeds, num);
    }

    for (auto const& speed : speeds) {
      auto eg = idfObject.pushExtensibleGroup();

      // SpeedReferenceUnitGrossRatedTotalCoolingCapacity
      if ((value = speed.referenceUnitGrossRatedTotalCoolingCapacity())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedTotalCoolingCapacity, value.get());
      }

      // SpeedReferenceUnitGrossRatedSensibleHeatRatio
      if ((value = speed.referenceUnitGrossRatedSensibleHeatRatio())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedSensibleHeatRatio, value.get());
      }

      // SpeedReferenceUnitGrossRatedCoolingCOP
      if ((value = speed.referenceUnitGrossRatedCoolingCOP())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedCoolingCOP, value.get());
      }

      // SpeedReferenceUnitRatedAirFlowRate
      if ((value = speed.referenceUnitRatedAirFlowRate())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, value.get());
      }

      // SpeedReferenceUnitRatedCondenserAirFlowRate
      if ((value = speed.referenceUnitRatedCondenserAirFlowRate())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedCondenserAirFlowRate, value.get());
      }

      // SpeedReferenceUnitRatedPadEffectivenessofEvapPrecooling
      if ((value = speed.referenceUnitRatedPadEffectivenessofEvapPrecooling())) {
        eg.setDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedPadEffectivenessofEvapPrecooling, value.get());
      }

      // SpeedTotalCoolingCapacityFunctionofTemperatureCurveName
      {
        auto curve = speed.totalCoolingCapacityFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName
      {
        auto curve = speed.totalCoolingCapacityFunctionofAirFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName,
                       _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofTemperatureCurveName
      {
        auto curve = speed.energyInputRatioFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName
      {
        auto curve = speed.energyInputRatioFunctionofAirFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName, _curve->name().get());
        }
      }
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXVariableSpeed(CoilCoolingDXVariableSpeed& modelObject) {
    IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

    m_idfObjects.push_back(coilSystemCoolingDXIdf);

    boost::optional<IdfObject> idfObject = translateCoilCoolingDXVariableSpeedWithoutUnitary(modelObject);

    if (!idfObject) {
      return boost::none;
    }

    if (auto s = modelObject.name()) {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType, idfObject->iddObject().name());
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilName, s.get());
      coilSystemCoolingDXIdf.setName(s.get() + " CoilSystem");
    }

    if (auto node = modelObject.inletModelObject()) {
      translateAndMapModelObject(node.get());
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName, node->name().get());
    }

    if (auto node = modelObject.outletModelObject()) {
      translateAndMapModelObject(node.get());
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName, node->name().get());
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName, node->name().get());
    }

    return coilSystemCoolingDXIdf;
  }

}  // namespace energyplus

}  // namespace openstudio
