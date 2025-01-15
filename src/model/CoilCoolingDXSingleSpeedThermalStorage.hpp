/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_HPP
#define MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class CoilCoolingDXSingleSpeedThermalStorage_Impl;

  }  // namespace detail

  /** CoilCoolingDXSingleSpeedThermalStorage is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:SingleSpeed:ThermalStorage'. */
  class MODEL_API CoilCoolingDXSingleSpeedThermalStorage : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXSingleSpeedThermalStorage(const Model& model);

    virtual ~CoilCoolingDXSingleSpeedThermalStorage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXSingleSpeedThermalStorage(const CoilCoolingDXSingleSpeedThermalStorage& other) = default;
    CoilCoolingDXSingleSpeedThermalStorage(CoilCoolingDXSingleSpeedThermalStorage&& other) = default;
    CoilCoolingDXSingleSpeedThermalStorage& operator=(const CoilCoolingDXSingleSpeedThermalStorage&) = default;
    CoilCoolingDXSingleSpeedThermalStorage& operator=(CoilCoolingDXSingleSpeedThermalStorage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> operatingModeControlMethodValues();

    static std::vector<std::string> storageTypeValues();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string operatingModeControlMethod() const;

    boost::optional<Schedule> operationModeControlSchedule() const;

    std::string storageType() const;

    int glycolConcentration() const;

    boost::optional<double> fluidStorageVolume() const;

    bool isFluidStorageVolumeAutosized() const;

    boost::optional<double> iceStorageCapacity() const;

    bool isIceStorageCapacityAutosized() const;

    boost::optional<double> storageCapacitySizingFactor() const;

    double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const;

    double fluidStorageTankRatingTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    bool coolingOnlyModeAvailable() const;

    boost::optional<double> coolingOnlyModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized() const;

    double coolingOnlyModeRatedSensibleHeatRatio() const;

    double coolingOnlyModeRatedCOP() const;

    boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingOnlyModePartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool coolingAndChargeModeAvailable() const;

    boost::optional<double> coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const;

    double coolingAndChargeModeCapacitySizingFactor() const;

    boost::optional<double> coolingAndChargeModeRatedStorageChargingCapacity() const;

    bool isCoolingAndChargeModeRatedStorageChargingCapacityAutosized() const;

    double coolingAndChargeModeStorageCapacitySizingFactor() const;

    double coolingAndChargeModeRatedSensibleHeatRatio() const;

    double coolingAndChargeModeCoolingRatedCOP() const;

    double coolingAndChargeModeChargingRatedCOP() const;

    boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool coolingAndDischargeModeAvailable() const;

    boost::optional<double> coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const;

    double coolingAndDischargeModeEvaporatorCapacitySizingFactor() const;

    boost::optional<double> coolingAndDischargeModeRatedStorageDischargingCapacity() const;

    bool isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized() const;

    double coolingAndDischargeModeStorageDischargeCapacitySizingFactor() const;

    double coolingAndDischargeModeRatedSensibleHeatRatio() const;

    double coolingAndDischargeModeCoolingRatedCOP() const;

    double coolingAndDischargeModeDischargingRatedCOP() const;

    boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool chargeOnlyModeAvailable() const;

    boost::optional<double> chargeOnlyModeRatedStorageChargingCapacity() const;

    bool isChargeOnlyModeRatedStorageChargingCapacityAutosized() const;

    double chargeOnlyModeCapacitySizingFactor() const;

    double chargeOnlyModeChargingRatedCOP() const;

    boost::optional<Curve> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    bool dischargeOnlyModeAvailable() const;

    boost::optional<double> dischargeOnlyModeRatedStorageDischargingCapacity() const;

    bool isDischargeOnlyModeRatedStorageDischargingCapacityAutosized() const;

    double dischargeOnlyModeCapacitySizingFactor() const;

    double dischargeOnlyModeRatedSensibleHeatRatio() const;

    double dischargeOnlyModeRatedCOP() const;

    boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    boost::optional<Curve> dischargeOnlyModePartLoadFractionCorrelationCurve() const;

    boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    boost::optional<double> ancillaryElectricPower() const;

    boost::optional<double> coldWeatherOperationMinimumOutdoorAirTemperature() const;

    boost::optional<double> coldWeatherOperationAncillaryPower() const;

    boost::optional<double> condenserDesignAirFlowRate() const;

    bool isCondenserDesignAirFlowRateAutosized() const;

    double condenserAirFlowSizingFactor() const;

    std::string condenserType() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterAvailabilitySchedule() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    // boost::optional<double> storageTankPlantConnectionDesignFlowRate() const;

    // double storageTankPlantConnectionHeatTransferEffectiveness() const;

    // boost::optional<double> storageTankMinimumOperatingLimitFluidTemperature() const;

    // boost::optional<double> storageTankMaximumOperatingLimitFluidTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setOperatingModeControlMethod(const std::string& operatingModeControlMethod);

    bool setOperationModeControlSchedule(Schedule& schedule);

    void resetOperationModeControlSchedule();

    bool setStorageType(const std::string& storageType);

    bool setGlycolConcentration(int glycolConcentration);

    bool setFluidStorageVolume(double fluidStorageVolume);

    void resetFluidStorageVolume();

    void autosizeFluidStorageVolume();

    bool setIceStorageCapacity(double iceStorageCapacity);

    void autosizeIceStorageCapacity();

    bool setStorageCapacitySizingFactor(double storageCapacitySizingFactor);

    bool setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);

    bool setFluidStorageTankRatingTemperature(double fluidStorageTankRatingTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autosizeRatedEvaporatorAirFlowRate();

    bool setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable);

    bool setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);

    void autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingOnlyModeRatedSensibleHeatRatio(double coolingOnlyModeRatedSensibleHeatRatio);

    bool setCoolingOnlyModeRatedCOP(double coolingOnlyModeRatedCOP);

    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    bool setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    bool setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    bool setCoolingOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingOnlyModePartLoadFractionCorrelationCurve();

    bool setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    bool setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setCoolingAndChargeModeAvailable(bool coolingAndChargeModeAvailable);

    bool setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(double coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity);

    void autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingAndChargeModeCapacitySizingFactor(double coolingAndChargeModeCapacitySizingFactor);

    bool setCoolingAndChargeModeRatedStorageChargingCapacity(double coolingAndChargeModeRatedStorageChargingCapacity);

    void autosizeCoolingAndChargeModeRatedStorageChargingCapacity();

    bool setCoolingAndChargeModeStorageCapacitySizingFactor(double coolingAndChargeModeStorageCapacitySizingFactor);

    bool setCoolingAndChargeModeRatedSensibleHeatRatio(double coolingAndChargeModeRatedSensibleHeatRatio);

    bool setCoolingAndChargeModeCoolingRatedCOP(double coolingAndChargeModeCoolingRatedCOP);

    bool setCoolingAndChargeModeChargingRatedCOP(double coolingAndChargeModeChargingRatedCOP);

    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    bool setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();

    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();

    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();

    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    bool setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeAvailable(bool coolingAndDischargeModeAvailable);

    bool setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(double coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity);

    void autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(double coolingAndDischargeModeEvaporatorCapacitySizingFactor);

    bool setCoolingAndDischargeModeRatedStorageDischargingCapacity(double coolingAndDischargeModeRatedStorageDischargingCapacity);

    void autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity();

    bool setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(double coolingAndDischargeModeStorageDischargeCapacitySizingFactor);

    bool setCoolingAndDischargeModeRatedSensibleHeatRatio(double coolingAndDischargeModeRatedSensibleHeatRatio);

    bool setCoolingAndDischargeModeCoolingRatedCOP(double coolingAndDischargeModeCoolingRatedCOP);

    bool setCoolingAndDischargeModeDischargingRatedCOP(double coolingAndDischargeModeDischargingRatedCOP);

    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();

    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();

    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();

    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable);

    bool setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity);

    void autosizeChargeOnlyModeRatedStorageChargingCapacity();

    bool setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor);

    bool setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP);

    bool setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();

    bool setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    bool setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable);

    bool setDischargeOnlyModeRatedStorageDischargingCapacity(double dischargeOnlyModeRatedStorageDischargingCapacity);

    void autosizeDischargeOnlyModeRatedStorageDischargingCapacity();

    bool setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor);

    bool setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio);

    bool setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP);

    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();

    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    bool setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    bool setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    bool setDischargeOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetDischargeOnlyModePartLoadFractionCorrelationCurve();

    bool setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    bool setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setAncillaryElectricPower(double ancillaryElectricPower);

    bool setColdWeatherOperationMinimumOutdoorAirTemperature(double coldWeatherOperationMinimumOutdoorAirTemperature);

    void resetColdWeatherOperationMinimumOutdoorAirTemperature();

    bool setColdWeatherOperationAncillaryPower(double coldWeatherOperationAncillaryPower);

    bool setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate);

    void autosizeCondenserDesignAirFlowRate();

    bool setCondenserAirFlowSizingFactor(double condenserAirFlowSizingFactor);

    bool setCondenserType(const std::string& condenserType);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterAvailabilitySchedule(Schedule& schedule);

    void resetBasinHeaterAvailabilitySchedule();

    // bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetCondensateCollectionWaterStorageTank();

    // bool setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate);

    // void resetStorageTankPlantConnectionDesignFlowRate();

    // bool setStorageTankPlantConnectionHeatTransferEffectiveness(double storageTankPlantConnectionHeatTransferEffectiveness);

    // bool setStorageTankMinimumOperatingLimitFluidTemperature(double storageTankMinimumOperatingLimitFluidTemperature);

    // void resetStorageTankMinimumOperatingLimitFluidTemperature();

    // bool setStorageTankMaximumOperatingLimitFluidTemperature(double storageTankMaximumOperatingLimitFluidTemperature);

    // void resetStorageTankMaximumOperatingLimitFluidTemperature();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedFluidStorageVolume();

    boost::optional<double> autosizedIceStorageCapacity();

    boost::optional<double> autosizedRatedEvaporatorAirFlowRate();

    boost::optional<double> autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();

    boost::optional<double> autosizedCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();

    boost::optional<double> autosizedCoolingAndChargeModeRatedStorageChargingCapacity();

    boost::optional<double> autosizedCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();

    boost::optional<double> autosizedCoolingAndDischargeModeRatedStorageDischargingCapacity();

    boost::optional<double> autosizedChargeOnlyModeRatedStorageChargingCapacity();

    boost::optional<double> autosizedDischargeOnlyModeRatedStorageDischargingCapacity();

    boost::optional<double> autosizedCondenserDesignAirFlowRate();

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXSingleSpeedThermalStorage_Impl;

    explicit CoilCoolingDXSingleSpeedThermalStorage(std::shared_ptr<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl> impl);

    friend class detail::CoilCoolingDXSingleSpeedThermalStorage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeedThermalStorage");
  };

  /** \relates CoilCoolingDXSingleSpeedThermalStorage*/
  using OptionalCoilCoolingDXSingleSpeedThermalStorage = boost::optional<CoilCoolingDXSingleSpeedThermalStorage>;

  /** \relates CoilCoolingDXSingleSpeedThermalStorage*/
  using CoilCoolingDXSingleSpeedThermalStorageVector = std::vector<CoilCoolingDXSingleSpeedThermalStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_HPP
