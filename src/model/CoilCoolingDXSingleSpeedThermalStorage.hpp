/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_HPP
#define MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;
  class FluidAndGlycol;
  class Connection;
  class Curve;
  class WaterStorageTank;

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

    explicit CoilCoolingDXSingleSpeedThermalStorage(const Model& model, bool coolingOnlyModeAvailable, bool coolingAndChargeModeAvailable,
                                                    bool coolingAndDischargeModeAvailable, bool chargeOnlyModeAvailable,
                                                    bool dischargeOnlyModeAvailable);

    virtual ~CoilCoolingDXSingleSpeedThermalStorage() = default;
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

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule availabilitySchedule() const;

    std::string operatingModeControlMethod() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> operationModeControlSchedule() const;

    std::string storageType() const;

    // TODO: Check return type. From object lists, some candidates are: FluidAndGlycol.
    boost::optional<FluidAndGlycol> userDefinedFluidType() const;

    boost::optional<double> fluidStorageVolume() const;

    bool isFluidStorageVolumeAutocalculated() const;

    boost::optional<double> iceStorageCapacity() const;

    bool isIceStorageCapacityAutocalculated() const;

    boost::optional<double> storageCapacitySizingFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection storageTankAmbientTemperatureNode() const;

    double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const;

    boost::optional<double> fluidStorageTankRatingTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    boost::optional<double> autosizedRatedEvaporatorAirFlowRate();

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection evaporatorAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection evaporatorAirOutletNode() const;

    bool coolingOnlyModeAvailable() const;

    boost::optional<double> coolingOnlyModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized() const;

    boost::optional<double> autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();

    double coolingOnlyModeRatedSensibleHeatRatio() const;

    double coolingOnlyModeRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModePartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool coolingAndChargeModeAvailable() const;

    boost::optional<double> coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const;

    double coolingAndChargeModeCapacitySizingFactor() const;

    boost::optional<double> coolingAndChargeModeRatedStorageChargingCapacity() const;

    bool isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated() const;

    double coolingAndChargeModeStorageCapacitySizingFactor() const;

    double coolingAndChargeModeRatedSensibleHeatRatio() const;

    double coolingAndChargeModeCoolingRatedCOP() const;

    double coolingAndChargeModeChargingRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve, Curve.
    boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool coolingAndDischargeModeAvailable() const;

    boost::optional<double> coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const;

    bool isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const;

    double coolingAndDischargeModeEvaporatorCapacitySizingFactor() const;

    boost::optional<double> coolingAndDischargeModeRatedStorageDischargingCapacity() const;

    bool isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated() const;

    double coolingAndDischargeModeStorageDischargeCapacitySizingFactor() const;

    double coolingAndDischargeModeRatedSensibleHeatRatio() const;

    double coolingAndDischargeModeCoolingRatedCOP() const;

    double coolingAndDischargeModeDischargingRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve, Curve.
    boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool chargeOnlyModeAvailable() const;

    boost::optional<double> chargeOnlyModeRatedStorageChargingCapacity() const;

    bool isChargeOnlyModeRatedStorageChargingCapacityAutocalculated() const;

    double chargeOnlyModeCapacitySizingFactor() const;

    double chargeOnlyModeChargingRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    bool dischargeOnlyModeAvailable() const;

    boost::optional<double> dischargeOnlyModeRatedStorageDischargingCapacity() const;

    bool isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated() const;

    double dischargeOnlyModeCapacitySizingFactor() const;

    double dischargeOnlyModeRatedSensibleHeatRatio() const;

    double dischargeOnlyModeRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModePartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve, Curve.
    boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Curve.
    boost::optional<Curve> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    boost::optional<double> ancillaryElectricPower() const;

    boost::optional<double> coldWeatherOperationMinimumOutdoorAirTemperature() const;

    boost::optional<double> coldWeatherOperationAncillaryPower() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection condenserAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection condenserAirOutletNode() const;

    boost::optional<double> condenserDesignAirFlowRate() const;

    bool isCondenserDesignAirFlowRateAutocalculated() const;

    double condenserAirFlowSizingFactor() const;

    std::string condenserType() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption();

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> basinHeaterAvailabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
    boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
    boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> storageTankPlantConnectionInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> storageTankPlantConnectionOutletNode() const;

    boost::optional<double> storageTankPlantConnectionDesignFlowRate() const;

    double storageTankPlantConnectionHeatTransferEffectiveness() const;

    boost::optional<double> storageTankMinimumOperatingLimitFluidTemperature() const;

    boost::optional<double> storageTankMaximumOperatingLimitFluidTemperature() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    bool setOperatingModeControlMethod(const std::string& operatingModeControlMethod);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setOperationModeControlSchedule(Schedule& schedule);

    void resetOperationModeControlSchedule();

    bool setStorageType(const std::string& storageType);

    // TODO: Check argument type. From object lists, some candidates are: FluidAndGlycol.
    bool setUserDefinedFluidType(const FluidAndGlycol& fluidAndGlycol);

    void resetUserDefinedFluidType();

    bool setFluidStorageVolume(double fluidStorageVolume);

    void resetFluidStorageVolume();

    void autocalculateFluidStorageVolume();

    bool setIceStorageCapacity(double iceStorageCapacity);

    void resetIceStorageCapacity();

    void autocalculateIceStorageCapacity();

    bool setStorageCapacitySizingFactor(double storageCapacitySizingFactor);

    void resetStorageCapacitySizingFactor();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setStorageTankAmbientTemperatureNode(const Connection& connection);

    bool setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);

    bool setFluidStorageTankRatingTemperature(double fluidStorageTankRatingTemperature);

    void resetFluidStorageTankRatingTemperature();

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autosizeRatedEvaporatorAirFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setEvaporatorAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setEvaporatorAirOutletNode(const Connection& connection);

    bool setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable);

    bool setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);

    void resetCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();

    void autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingOnlyModeRatedSensibleHeatRatio(double coolingOnlyModeRatedSensibleHeatRatio);

    bool setCoolingOnlyModeRatedCOP(double coolingOnlyModeRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingOnlyModePartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setCoolingAndChargeModeAvailable(bool coolingAndChargeModeAvailable);

    bool setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(double coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity);

    void resetCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();

    void autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingAndChargeModeCapacitySizingFactor(double coolingAndChargeModeCapacitySizingFactor);

    bool setCoolingAndChargeModeRatedStorageChargingCapacity(double coolingAndChargeModeRatedStorageChargingCapacity);

    void resetCoolingAndChargeModeRatedStorageChargingCapacity();

    void autocalculateCoolingAndChargeModeRatedStorageChargingCapacity();

    bool setCoolingAndChargeModeStorageCapacitySizingFactor(double coolingAndChargeModeStorageCapacitySizingFactor);

    bool setCoolingAndChargeModeRatedSensibleHeatRatio(double coolingAndChargeModeRatedSensibleHeatRatio);

    bool setCoolingAndChargeModeCoolingRatedCOP(double coolingAndChargeModeCoolingRatedCOP);

    bool setCoolingAndChargeModeChargingRatedCOP(double coolingAndChargeModeChargingRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve, Curve.
    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setCoolingAndDischargeModeAvailable(bool coolingAndDischargeModeAvailable);

    bool setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(double coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity);

    void resetCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();

    void autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();

    bool setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(double coolingAndDischargeModeEvaporatorCapacitySizingFactor);

    bool setCoolingAndDischargeModeRatedStorageDischargingCapacity(double coolingAndDischargeModeRatedStorageDischargingCapacity);

    void resetCoolingAndDischargeModeRatedStorageDischargingCapacity();

    void autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity();

    bool setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(double coolingAndDischargeModeStorageDischargeCapacitySizingFactor);

    bool setCoolingAndDischargeModeRatedSensibleHeatRatio(double coolingAndDischargeModeRatedSensibleHeatRatio);

    bool setCoolingAndDischargeModeCoolingRatedCOP(double coolingAndDischargeModeCoolingRatedCOP);

    bool setCoolingAndDischargeModeDischargingRatedCOP(double coolingAndDischargeModeDischargingRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve, Curve.
    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable);

    bool setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity);

    void resetChargeOnlyModeRatedStorageChargingCapacity();

    void autocalculateChargeOnlyModeRatedStorageChargingCapacity();

    bool setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor);

    bool setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    bool setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable);

    bool setDischargeOnlyModeRatedStorageDischargingCapacity(double dischargeOnlyModeRatedStorageDischargingCapacity);

    void resetDischargeOnlyModeRatedStorageDischargingCapacity();

    void autocalculateDischargeOnlyModeRatedStorageDischargingCapacity();

    bool setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor);

    bool setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio);

    void resetDischargeOnlyModeRatedSensibleHeatRatio();

    bool setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve);

    void resetDischargeOnlyModePartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve, Curve.
    bool setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve);

    void resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: Curve.
    bool setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve);

    void resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setAncillaryElectricPower(double ancillaryElectricPower);

    void resetAncillaryElectricPower();

    bool setColdWeatherOperationMinimumOutdoorAirTemperature(double coldWeatherOperationMinimumOutdoorAirTemperature);

    void resetColdWeatherOperationMinimumOutdoorAirTemperature();

    bool setColdWeatherOperationAncillaryPower(double coldWeatherOperationAncillaryPower);

    void resetColdWeatherOperationAncillaryPower();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCondenserAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCondenserAirOutletNode(const Connection& connection);

    bool setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate);

    void autocalculateCondenserDesignAirFlowRate();

    bool setCondenserAirFlowSizingFactor(double condenserAirFlowSizingFactor);

    bool setCondenserType(const std::string& condenserType);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setBasinHeaterAvailabilitySchedule(Schedule& schedule);

    void resetBasinHeaterAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

    void resetSupplyWaterStorageTank();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

    void resetCondensateCollectionWaterStorageTank();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setStorageTankPlantConnectionInletNode(const Connection& connection);

    void resetStorageTankPlantConnectionInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setStorageTankPlantConnectionOutletNode(const Connection& connection);

    void resetStorageTankPlantConnectionOutletNode();

    bool setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate);

    void resetStorageTankPlantConnectionDesignFlowRate();

    bool setStorageTankPlantConnectionHeatTransferEffectiveness(double storageTankPlantConnectionHeatTransferEffectiveness);

    bool setStorageTankMinimumOperatingLimitFluidTemperature(double storageTankMinimumOperatingLimitFluidTemperature);

    void resetStorageTankMinimumOperatingLimitFluidTemperature();

    bool setStorageTankMaximumOperatingLimitFluidTemperature(double storageTankMaximumOperatingLimitFluidTemperature);

    void resetStorageTankMaximumOperatingLimitFluidTemperature();

    //@}
    /** @name Other */
    //@{

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
