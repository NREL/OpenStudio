/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilCoolingDXSingleSpeedThermalStorage.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
// #include "../../model/WaterStorageTank.hpp"
// #include "../../model/WaterStorageTank_Impl.hpp"

#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilCoolingDXSingleSpeedThermalStorageWithoutUnitary(model::CoilCoolingDXSingleSpeedThermalStorage& modelObject) {
    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage, modelObject);

    // Availability Schedule Name: Required Object
    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilityScheduleName, wo_->nameString());
    }

    // Operating Mode Control Method: Required String
    const std::string operatingModeControlMethod = modelObject.operatingModeControlMethod();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod, operatingModeControlMethod);

    // Operation Mode Control Schedule Name: Optional Object
    if (boost::optional<Schedule> operationModeControlSchedule_ = modelObject.operationModeControlSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(operationModeControlSchedule_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlScheduleName, wo_->nameString());
      }
    }

    // Storage Type: Required String
    const std::string storageType = modelObject.storageType();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, storageType);

    if (boost::optional<std::string> s_ = modelObject.storageType()) {
      if (istringEqual(s_.get(), "PropyleneGlycol") || istringEqual(s_.get(), "EthyleneGlycol")) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, "UserDefinedFluidType");
        boost::optional<int> gc = modelObject.glycolConcentration();
        if (gc) {
          boost::optional<IdfObject> fluidProperties = createFluidProperties(s_.get(), gc.get());
          if (fluidProperties) {
            boost::optional<std::string> fluidPropertiesName = fluidProperties->getString(FluidProperties_NameFields::FluidName, true);
            if (fluidPropertiesName) {
              idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType, fluidPropertiesName.get());
            }
          }
        }
      } else {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, s_.get());
      }
    }

    if (modelObject.isFluidStorageVolumeAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "Autosize");
    } else {
      // Fluid Storage Volume: boost::optional<double>
      if (boost::optional<double> fluidStorageVolume_ = modelObject.fluidStorageVolume()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, fluidStorageVolume_.get());
      }
    }

    if (modelObject.isIceStorageCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, "Autosize");
    } else {
      // Ice Storage Capacity: boost::optional<double>
      if (boost::optional<double> iceStorageCapacity_ = modelObject.iceStorageCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, iceStorageCapacity_.get());
      }
    }

    // Storage Capacity Sizing Factor: boost::optional<double>
    if (boost::optional<double> storageCapacitySizingFactor_ = modelObject.storageCapacitySizingFactor()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, storageCapacitySizingFactor_.get());
    }

    // Storage Tank Ambient Temperature Node Name: Required Node
    std::string storageTankAmbientTemperatureNodeName(modelObject.nameString() + " Storage Tank Ambient Temperature Node");
    // Create an OutdoorAir:NodeList for the storage tank conditions to be set directly from weather file
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0, storageTankAmbientTemperatureNodeName);
    m_idfObjects.push_back(oaNodeListIdf);
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNodeName,
                        storageTankAmbientTemperatureNodeName);

    // Storage Tank to Ambient U-value Times Area Heat Transfer Coefficient: Required Double
    const double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient =
      modelObject.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient,
                        storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);

    // Fluid Storage Tank Rating Temperature: Required Double
    const double fluidStorageTankRatingTemperature = modelObject.fluidStorageTankRatingTemperature();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature, fluidStorageTankRatingTemperature);

    if (modelObject.isRatedEvaporatorAirFlowRateAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, "Autosize");
    } else {
      // Rated Evaporator Air Flow Rate: boost::optional<double>
      if (boost::optional<double> ratedEvaporatorAirFlowRate_ = modelObject.ratedEvaporatorAirFlowRate()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate_.get());
      }
    }

    // Evaporator Air Inlet Node Name: Required Node
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName, node->name().get());
      }
    }

    // Evaporator Air Outlet Node Name: Required Node
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName, node->name().get());
      }
    }

    bool coolingOnlyModeAvailable = modelObject.coolingOnlyModeAvailable();
    bool coolingAndChargeModeAvailable = modelObject.coolingAndChargeModeAvailable();
    bool coolingAndDischargeModeAvailable = modelObject.coolingAndDischargeModeAvailable();
    bool chargeOnlyModeAvailable = modelObject.chargeOnlyModeAvailable();
    bool dischargeOnlyModeAvailable = modelObject.dischargeOnlyModeAvailable();

    // Cooling Only Mode Available: Required Boolean
    if (coolingOnlyModeAvailable) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable, "No");
    }

    // Cooling Only Mode Rated Total Evaporator Cooling Capacity
    if (modelObject.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, "Autosize");
    } else {
      if (boost::optional<double> coolingOnlyModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity,
                            coolingOnlyModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling Only Mode Rated Sensible Heat Ratio: Required Double
    const double coolingOnlyModeRatedSensibleHeatRatio = modelObject.coolingOnlyModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio,
                        coolingOnlyModeRatedSensibleHeatRatio);

    // Cooling Only Mode Rated COP: Required Double
    const double coolingOnlyModeRatedCOP = modelObject.coolingOnlyModeRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP, coolingOnlyModeRatedCOP);

    // Cooling Only Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling Only Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling Only Mode Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModePartLoadFractionCorrelationCurve_ = modelObject.coolingOnlyModePartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModePartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurveName, wo_->nameString());
      }
    }

    // Cooling Only Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Available: Required Boolean
    if (coolingAndChargeModeAvailable) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable, "No");
    }

    // Cooling And Charge Mode Rated Total Evaporator Cooling Capacity
    if (modelObject.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, "Autosize");
    } else {
      if (boost::optional<double> coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity,
                            coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling And Charge Mode Capacity Sizing Factor: Required Double
    const double coolingAndChargeModeCapacitySizingFactor = modelObject.coolingAndChargeModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor,
                        coolingAndChargeModeCapacitySizingFactor);

    // Cooling And Charge Mode Rated Storage Charging Capacity
    if (modelObject.isCoolingAndChargeModeRatedStorageChargingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, "Autosize");
    } else {
      if (boost::optional<double> coolingAndChargeModeRatedStorageChargingCapacity_ =
            modelObject.coolingAndChargeModeRatedStorageChargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity,
                            coolingAndChargeModeRatedStorageChargingCapacity_.get());
      }
    }

    // Cooling And Charge Mode Storage Capacity Sizing Factor: Required Double
    const double coolingAndChargeModeStorageCapacitySizingFactor = modelObject.coolingAndChargeModeStorageCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor,
                        coolingAndChargeModeStorageCapacitySizingFactor);

    // Cooling And Charge Mode Rated Sensible Heat Ratio: Required Double
    const double coolingAndChargeModeRatedSensibleHeatRatio = modelObject.coolingAndChargeModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio,
                        coolingAndChargeModeRatedSensibleHeatRatio);

    // Cooling And Charge Mode Cooling Rated COP: Required Double
    const double coolingAndChargeModeCoolingRatedCOP = modelObject.coolingAndChargeModeCoolingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP, coolingAndChargeModeCoolingRatedCOP);

    // Cooling And Charge Mode Charging Rated COP: Required Double
    const double coolingAndChargeModeChargingRatedCOP = modelObject.coolingAndChargeModeChargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP, coolingAndChargeModeChargingRatedCOP);

    // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Charge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Charge Capacity Function of Total Evaporator PLR Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve_ =
          modelObject.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Available: Required Boolean
    if (coolingAndDischargeModeAvailable) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable, "No");
    }

    // Cooling And Discharge Mode Rated Total Evaporator Cooling Capacity
    if (modelObject.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, "Autosize");
    } else {
      if (boost::optional<double> coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity,
                            coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling And Discharge Mode Evaporator Capacity Sizing Factor: Required Double
    const double coolingAndDischargeModeEvaporatorCapacitySizingFactor = modelObject.coolingAndDischargeModeEvaporatorCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor,
                        coolingAndDischargeModeEvaporatorCapacitySizingFactor);

    // Cooling And Discharge Mode Rated Storage Discharging Capacity
    if (modelObject.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, "Autosize");
    } else {
      if (boost::optional<double> coolingAndDischargeModeRatedStorageDischargingCapacity_ =
            modelObject.coolingAndDischargeModeRatedStorageDischargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity,
                            coolingAndDischargeModeRatedStorageDischargingCapacity_.get());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Sizing Factor: Required Double
    const double coolingAndDischargeModeStorageDischargeCapacitySizingFactor =
      modelObject.coolingAndDischargeModeStorageDischargeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor,
                        coolingAndDischargeModeStorageDischargeCapacitySizingFactor);

    // Cooling And Discharge Mode Rated Sensible Heat Ratio: Required Double
    const double coolingAndDischargeModeRatedSensibleHeatRatio = modelObject.coolingAndDischargeModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio,
                        coolingAndDischargeModeRatedSensibleHeatRatio);

    // Cooling And Discharge Mode Cooling Rated COP: Required Double
    const double coolingAndDischargeModeCoolingRatedCOP = modelObject.coolingAndDischargeModeCoolingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP,
                        coolingAndDischargeModeCoolingRatedCOP);

    // Cooling And Discharge Mode Discharging Rated COP: Required Double
    const double coolingAndDischargeModeDischargingRatedCOP = modelObject.coolingAndDischargeModeDischargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP,
                        coolingAndDischargeModeDischargingRatedCOP);

    // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Total Evaporator PLR Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Charge Only Mode Available: Required Boolean
    if (chargeOnlyModeAvailable) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable, "No");
    }

    // Charge Only Mode Rated Storage Charging Capacity
    if (modelObject.isChargeOnlyModeRatedStorageChargingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, "Autosize");
    } else {
      if (boost::optional<double> chargeOnlyModeRatedStorageChargingCapacity_ = modelObject.chargeOnlyModeRatedStorageChargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity,
                            chargeOnlyModeRatedStorageChargingCapacity_.get());
      }
    }

    // Charge Only Mode Capacity Sizing Factor: Required Double
    const double chargeOnlyModeCapacitySizingFactor = modelObject.chargeOnlyModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor, chargeOnlyModeCapacitySizingFactor);

    // Charge Only Mode Charging Rated COP: Required Double
    const double chargeOnlyModeChargingRatedCOP = modelObject.chargeOnlyModeChargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP, chargeOnlyModeChargingRatedCOP);

    // Charge Only Mode Storage Charge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve_ =
          modelObject.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Charge Only Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Available: Required Boolean
    if (dischargeOnlyModeAvailable) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable, "No");
    }

    // Discharge Only Mode Rated Storage Discharging Capacity
    if (modelObject.isDischargeOnlyModeRatedStorageDischargingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, "Autosize");
    } else {
      if (boost::optional<double> dischargeOnlyModeRatedStorageDischargingCapacity_ =
            modelObject.dischargeOnlyModeRatedStorageDischargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity,
                            dischargeOnlyModeRatedStorageDischargingCapacity_.get());
      }
    }

    // Discharge Only Mode Capacity Sizing Factor: Required Double
    const double dischargeOnlyModeCapacitySizingFactor = modelObject.dischargeOnlyModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor,
                        dischargeOnlyModeCapacitySizingFactor);

    // Discharge Only Mode Rated Sensible Heat Ratio: Required Double
    const double dischargeOnlyModeRatedSensibleHeatRatio = modelObject.dischargeOnlyModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio,
                        dischargeOnlyModeRatedSensibleHeatRatio);

    // Discharge Only Mode Rated COP: Required Double
    const double dischargeOnlyModeRatedCOP = modelObject.dischargeOnlyModeRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, dischargeOnlyModeRatedCOP);

    // Discharge Only Mode Storage Discharge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Storage Discharge Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Discharge Only Mode Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModePartLoadFractionCorrelationCurve_ = modelObject.dischargeOnlyModePartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModePartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Ancillary Electric Power: boost::optional<double>
    if (boost::optional<double> ancillaryElectricPower_ = modelObject.ancillaryElectricPower()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower, ancillaryElectricPower_.get());
    }

    // Cold Weather Operation Minimum Outdoor Air Temperature: boost::optional<double>
    if (boost::optional<double> coldWeatherOperationMinimumOutdoorAirTemperature_ = modelObject.coldWeatherOperationMinimumOutdoorAirTemperature()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature,
                          coldWeatherOperationMinimumOutdoorAirTemperature_.get());
    }

    // Cold Weather Operation Ancillary Power: boost::optional<double>
    if (boost::optional<double> coldWeatherOperationAncillaryPower_ = modelObject.coldWeatherOperationAncillaryPower()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower,
                          coldWeatherOperationAncillaryPower_.get());
    }

    // Condenser Air Inlet Node Name: Required Node
    std::string condenserInletNodeName(modelObject.nameString() + " Condenser Air Inlet Node");
    // Create an OutdoorAir:NodeList for the condenser inlet conditions to be set directly from weather file
    IdfObject oaNodeListIdf2(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf2.setString(0, condenserInletNodeName);
    m_idfObjects.push_back(oaNodeListIdf2);
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNodeName, condenserInletNodeName);

    // Condenser Air Outlet Node Name: Required Node
    std::string condenserOutletNodeName(modelObject.nameString() + " Condenser Air Outlet Node");
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNodeName, condenserOutletNodeName);

    // Condenser Design Air Flow Rate
    if (modelObject.isCondenserDesignAirFlowRateAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, "Autosize");
    } else {
      if (boost::optional<double> condenserDesignAirFlowRate_ = modelObject.condenserDesignAirFlowRate()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, condenserDesignAirFlowRate_.get());
      }
    }

    // Condenser Air Flow Sizing Factor: Required Double
    const double condenserAirFlowSizingFactor = modelObject.condenserAirFlowSizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor, condenserAirFlowSizingFactor);

    // Condenser Type: Required String
    const std::string condenserType = modelObject.condenserType();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType, condenserType);

    // Evaporative Condenser Effectiveness: Required Double
    const double evaporativeCondenserEffectiveness = modelObject.evaporativeCondenserEffectiveness();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);

    // Evaporative Condenser Pump Rated Power Consumption
    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    } else {
      if (boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption_ = modelObject.evaporativeCondenserPumpRatedPowerConsumption()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption,
                            evaporativeCondenserPumpRatedPowerConsumption_.get());
      }
    }

    // Basin Heater Capacity: Required Double
    const double basinHeaterCapacity = modelObject.basinHeaterCapacity();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity, basinHeaterCapacity);

    // Basin Heater Setpoint Temperature: Required Double
    const double basinHeaterSetpointTemperature = modelObject.basinHeaterSetpointTemperature();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);

    // Basin Heater Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> basinHeaterAvailabilitySchedule_ = modelObject.basinHeaterAvailabilitySchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(basinHeaterAvailabilitySchedule_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilityScheduleName, wo_->nameString());
      }
    }

    // Supply Water Storage Tank Name: Optional Object
    // if (boost::optional<WaterStorageTank> supplyWaterStorageTank_ = modelObject.supplyWaterStorageTank()) {
    // if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(supplyWaterStorageTank_.get())) {
    // idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTankName, wo_->nameString());
    // }
    // }

    // Condensate Collection Water Storage Tank Name: Optional Object
    // if (boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank_ = modelObject.condensateCollectionWaterStorageTank()) {
    // if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condensateCollectionWaterStorageTank_.get())) {
    // idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTankName, wo_->nameString());
    // }
    // }

    // Storage Tank Plant Connection Design Flow Rate: boost::optional<double>
    // if (boost::optional<double> storageTankPlantConnectionDesignFlowRate_ = modelObject.storageTankPlantConnectionDesignFlowRate()) {
    // idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate,
    // storageTankPlantConnectionDesignFlowRate_.get());
    // }

    // Storage Tank Plant Connection Heat Transfer Effectiveness: Required Double
    // const double storageTankPlantConnectionHeatTransferEffectiveness = modelObject.storageTankPlantConnectionHeatTransferEffectiveness();
    // idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness,
    // storageTankPlantConnectionHeatTransferEffectiveness);

    // Storage Tank Minimum Operating Limit Fluid Temperature: boost::optional<double>
    // if (boost::optional<double> storageTankMinimumOperatingLimitFluidTemperature_ = modelObject.storageTankMinimumOperatingLimitFluidTemperature()) {
    // idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature,
    // storageTankMinimumOperatingLimitFluidTemperature_.get());
    // }

    // Storage Tank Maximum Operating Limit Fluid Temperature: boost::optional<double>
    // if (boost::optional<double> storageTankMaximumOperatingLimitFluidTemperature_ = modelObject.storageTankMaximumOperatingLimitFluidTemperature()) {
    // idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature,
    // storageTankMaximumOperatingLimitFluidTemperature_.get());
    // }

    return idfObject;
  }

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilCoolingDXSingleSpeedThermalStorage(model::CoilCoolingDXSingleSpeedThermalStorage& modelObject) {

    IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

    m_idfObjects.push_back(coilSystemCoolingDXIdf);

    boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXSingleSpeedThermalStorageWithoutUnitary(modelObject);

    if (!oIdfObject) {
      return boost::none;
    }

    IdfObject idfObject = oIdfObject.get();

    OptionalString s;

    s = modelObject.name();
    if (s) {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType, idfObject.iddObject().name());

      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilName, *s);

      coilSystemCoolingDXIdf.setName(*s + " CoilSystem");
    }

    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);

    coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName, sched.name().get());

    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName, *s);
      }
    }

    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName, *s);

        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName, *s);
      }
    }

    return coilSystemCoolingDXIdf;
  }

}  // end namespace energyplus
}  // end namespace openstudio
