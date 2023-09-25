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
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/WaterHeaterSizing.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/core/Optional.hpp"

#include <utilities/idd/WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWaterHeaterMixed(WaterHeaterMixed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Schedule> schedule;

    if (!(modelObject.plantLoop() || (modelObject.peakUseFlowRate() && modelObject.useFlowRateFractionSchedule()))) {
      LOG(Warn,
          modelObject.briefDescription() << " will not be translated as it is not on a PlantLoop, and it does not have both a Peak Use Flow Rate "
                                            "and a Use Flow Rate Fraction Schedule which is a required condition for stand-alone operation");
      return boost::none;
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::WaterHeater_Mixed, modelObject);

    // Trigger translation of the WaterHeater:Sizing object, if any
    bool hasWaterHeaterSizing = true;
    try {
      auto siz = modelObject.waterHeaterSizing();
      translateAndMapModelObject(siz);
    } catch (...) {
      hasWaterHeaterSizing = false;
    }

    // TankVolume

    if (modelObject.isTankVolumeAutosized()) {
      idfObject.setString(WaterHeater_MixedFields::TankVolume, "Autosize");
      if (!hasWaterHeaterSizing) {
        LOG(Error, modelObject.briefDescription() << " has its Tank Volume autosized but it does not have a WaterHeaterSizing object attached");
      }
    } else {
      value = modelObject.tankVolume();

      if (value) {
        idfObject.setDouble(WaterHeater_MixedFields::TankVolume, value.get());
      }
    }

    // SetpointTemperatureScheduleName

    schedule = modelObject.setpointTemperatureSchedule();
    if (schedule) {
      translateAndMapModelObject(schedule.get());

      idfObject.setString(WaterHeater_MixedFields::SetpointTemperatureScheduleName, schedule->name().get());
    }

    // DeadbandTemperatureDifference

    value = modelObject.deadbandTemperatureDifference();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::DeadbandTemperatureDifference, value.get());
    }

    // MaximumTemperatureLimit

    value = modelObject.maximumTemperatureLimit();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::MaximumTemperatureLimit, value.get());
    }

    // HeaterControlType

    s = modelObject.heaterControlType();
    if (s) {
      idfObject.setString(WaterHeater_MixedFields::HeaterControlType, s.get());
    }

    // HeaterMaximumCapacity

    if (modelObject.isHeaterMaximumCapacityAutosized()) {
      idfObject.setString(WaterHeater_MixedFields::HeaterMaximumCapacity, "Autosize");
      if (!hasWaterHeaterSizing) {
        LOG(Error,
            modelObject.briefDescription() << " has its Heater Maximum Capacity autosized but it does not have a WaterHeaterSizing object attached");
      }
    } else {
      value = modelObject.heaterMaximumCapacity();

      if (value) {
        idfObject.setDouble(WaterHeater_MixedFields::HeaterMaximumCapacity, value.get());
      }
    }

    // HeaterMinimumCapacity

    value = modelObject.heaterMinimumCapacity();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::HeaterMinimumCapacity, value.get());
    }

    // HeaterIgnitionMinimumFlowRate

    value = modelObject.heaterIgnitionMinimumFlowRate();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::HeaterIgnitionMinimumFlowRate, value.get());
    }

    // HeaterIgnitionDelay

    value = modelObject.heaterIgnitionDelay();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::HeaterIgnitionDelay, value.get());
    }

    // HeaterFuelType

    s = modelObject.heaterFuelType();
    if (s) {
      idfObject.setString(WaterHeater_MixedFields::HeaterFuelType, s.get());
    }

    // HeaterThermalEfficiency

    value = modelObject.heaterThermalEfficiency();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::HeaterThermalEfficiency, value.get());
    }

    // PartLoadFactorCurveName

    if (boost::optional<CurveCubic> c = modelObject.partLoadFactorCurve()) {
      boost::optional<IdfObject> _c = translateAndMapModelObject(c.get());

      if (_c) {
        idfObject.setString(WaterHeater_MixedFields::PartLoadFactorCurveName, _c->name().get());
      }
    }

    // OffCycleParasiticFuelConsumptionRate

    value = modelObject.offCycleParasiticFuelConsumptionRate();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OffCycleParasiticFuelConsumptionRate, value.get());
    }

    // OffCycleParasiticFuelType

    s = modelObject.offCycleParasiticFuelType();
    if (s) {
      idfObject.setString(WaterHeater_MixedFields::OffCycleParasiticFuelType, s.get());
    }

    // OffCycleParasiticHeatFractiontoTank

    value = modelObject.offCycleParasiticHeatFractiontoTank();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OffCycleParasiticHeatFractiontoTank, value.get());
    }

    // OnCycleParasiticFuelConsumptionRate

    value = modelObject.onCycleParasiticFuelConsumptionRate();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OnCycleParasiticFuelConsumptionRate, value.get());
    }

    // OnCycleParasiticFuelType

    s = modelObject.onCycleParasiticFuelType();
    if (s) {
      idfObject.setString(WaterHeater_MixedFields::OnCycleParasiticFuelType, s.get());
    }

    // OnCycleParasiticHeatFractiontoTank

    value = modelObject.onCycleParasiticHeatFractiontoTank();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OnCycleParasiticHeatFractiontoTank, value.get());
    }

    // AmbientTemperatureIndicator

    s = modelObject.ambientTemperatureIndicator();
    if (s) {
      if (istringEqual(s.get(), "ThermalZone")) {
        idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureIndicator, "Zone");
      } else {
        idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureIndicator, s.get());
      }

      if (istringEqual(s.get(), "Schedule")) {
        // AmbientTemperatureScheduleName
        schedule = modelObject.ambientTemperatureSchedule();
        if (schedule) {
          translateAndMapModelObject(schedule.get());

          idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureScheduleName, schedule->name().get());
        }
      }

      if (istringEqual(s.get(), "Outdoors")) {
        if ((!modelObject.ambientTemperatureOutdoorAirNodeName()) || modelObject.ambientTemperatureOutdoorAirNodeName()->empty()) {
          IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
          auto name = modelObject.nameString() + " Outdoor Air Node";
          oaNodeListIdf.setString(0, name);
          m_idfObjects.push_back(oaNodeListIdf);
          idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, name);
        }
      }
    }

    // AmbientTemperatureZoneName

    if (boost::optional<ThermalZone> zone = modelObject.ambientTemperatureThermalZone()) {
      if (zone) {
        idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureZoneName, zone->name().get());
      }
    }

    // AmbientTemperatureOutdoorAirNodeName
    s = modelObject.ambientTemperatureOutdoorAirNodeName();
    if (s && !s->empty()) {
      IdfObject oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
      oaNodeList.setString(0, s.get());
      m_idfObjects.push_back(oaNodeList);
      idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, s.get());
    } else {
      // Even if there is no node defined, if user chooses the outdoor air indicator we need to make a node
      auto indicator = modelObject.ambientTemperatureIndicator();
      if (istringEqual(indicator, "Outdoors")) {
        std::string oaNodeName = modelObject.nameString() + " Ambient Temp OA Node";
        IdfObject oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
        oaNodeList.setString(0, oaNodeName);
        m_idfObjects.push_back(oaNodeList);
        idfObject.setString(WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, oaNodeName);
      }
    }

    // OffCycleLossCoefficienttoAmbientTemperature

    value = modelObject.offCycleLossCoefficienttoAmbientTemperature();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OffCycleLossCoefficienttoAmbientTemperature, value.get());
    }

    // OffCycleLossFractiontoThermalZone

    value = modelObject.offCycleLossFractiontoThermalZone();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OffCycleLossFractiontoZone, value.get());
    }

    // OnCycleLossCoefficienttoAmbientTemperature

    value = modelObject.onCycleLossCoefficienttoAmbientTemperature();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OnCycleLossCoefficienttoAmbientTemperature, value.get());
    }

    // OnCycleLossFractiontoThermalZone

    value = modelObject.onCycleLossFractiontoThermalZone();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::OnCycleLossFractiontoZone, value.get());
    }

    // PeakUseFlowRate

    value = modelObject.peakUseFlowRate();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::PeakUseFlowRate, value.get());
    }

    // UseFlowRateFractionScheduleName

    schedule = modelObject.useFlowRateFractionSchedule();
    if (schedule) {
      translateAndMapModelObject(schedule.get());

      idfObject.setString(WaterHeater_MixedFields::UseFlowRateFractionScheduleName, schedule->name().get());
    }

    // ColdWaterSupplyTemperatureScheduleName

    schedule = modelObject.coldWaterSupplyTemperatureSchedule();
    if (schedule) {
      translateAndMapModelObject(schedule.get());

      idfObject.setString(WaterHeater_MixedFields::ColdWaterSupplyTemperatureScheduleName, schedule->name().get());
    }

    // UseSideInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.supplyInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(WaterHeater_MixedFields::UseSideInletNodeName, node->name().get());
      }
    }

    // UseSideOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.supplyOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(WaterHeater_MixedFields::UseSideOutletNodeName, node->name().get());
      }
    }

    // UseSideEffectiveness

    value = modelObject.useSideEffectiveness();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::UseSideEffectiveness, value.get());
    }

    // SourceSideInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.demandInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(WaterHeater_MixedFields::SourceSideInletNodeName, node->name().get());
      }
    }

    // SourceSideOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.demandOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(WaterHeater_MixedFields::SourceSideOutletNodeName, node->name().get());
      }
    }

    // SourceSideEffectiveness

    value = modelObject.sourceSideEffectiveness();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::SourceSideEffectiveness, value.get());
    }

    // UseSideDesignFlowRate

    if (modelObject.isUseSideDesignFlowRateAutosized()) {
      idfObject.setString(WaterHeater_MixedFields::UseSideDesignFlowRate, "Autosize");
    } else {
      value = modelObject.useSideDesignFlowRate();

      if (value) {
        idfObject.setDouble(WaterHeater_MixedFields::UseSideDesignFlowRate, value.get());
      }
    }

    // SourceSideDesignFlowRate

    if (modelObject.isSourceSideDesignFlowRateAutosized()) {
      idfObject.setString(WaterHeater_MixedFields::SourceSideDesignFlowRate, "Autosize");
    } else {
      value = modelObject.sourceSideDesignFlowRate();

      if (value) {
        idfObject.setDouble(WaterHeater_MixedFields::SourceSideDesignFlowRate, value.get());
      }
    }

    // IndirectWaterHeatingRecoveryTime

    value = modelObject.indirectWaterHeatingRecoveryTime();
    if (value) {
      idfObject.setDouble(WaterHeater_MixedFields::IndirectWaterHeatingRecoveryTime, value.get());
    }

    // SourceSideFlowControlMode
    s = modelObject.sourceSideFlowControlMode();
    if (s) {
      idfObject.setString(WaterHeater_MixedFields::SourceSideFlowControlMode, s.get());

      if (openstudio::istringEqual("IndirectHeatAlternateSetpoint", s.get())) {
        // IndirectAlternateSetpointTemperatureScheduleName
        schedule = modelObject.indirectAlternateSetpointTemperatureSchedule();
        if (schedule) {
          translateAndMapModelObject(schedule.get());
          idfObject.setString(WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName, schedule->name().get());
        } else {
          LOG(Error, "Something really wrong happened, the Source Side Control Mode is set to 'IndirectHeatAlternateSetpoint' yet "
                     "there is no IndirectAlternateSetpointTemperatureScheduleName which is impossible per model logic for "
                       << modelObject.briefDescription());
        }
      }
    }

    // EndUseSubcategory
    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(WaterHeater_MixedFields::EndUseSubcategory, s.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
