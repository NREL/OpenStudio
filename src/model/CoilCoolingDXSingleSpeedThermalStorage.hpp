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
  class BivariateFunctions;
  class UnivariateFunctions;
  class TrivariateFunctions;
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

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingOnlyModePartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

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

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    boost::optional<BivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

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

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const;

    // TODO: Check return type. From object lists, some candidates are: TrivariateFunctions.
    boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    boost::optional<BivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

    bool chargeOnlyModeAvailable() const;

    boost::optional<double> chargeOnlyModeRatedStorageChargingCapacity() const;

    bool isChargeOnlyModeRatedStorageChargingCapacityAutocalculated() const;

    double chargeOnlyModeCapacitySizingFactor() const;

    double chargeOnlyModeChargingRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const;

    bool dischargeOnlyModeAvailable() const;

    boost::optional<double> dischargeOnlyModeRatedStorageDischargingCapacity() const;

    bool isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated() const;

    double dischargeOnlyModeCapacitySizingFactor() const;

    boost::optional<double> dischargeOnlyModeRatedSensibleHeatRatio() const;

    double dischargeOnlyModeRatedCOP() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    boost::optional<BivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> dischargeOnlyModePartLoadFractionCorrelationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    boost::optional<BivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const;

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

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingOnlyModePartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingOnlyModePartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

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

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

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

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();

    // TODO: Check argument type. From object lists, some candidates are: TrivariateFunctions.
    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const TrivariateFunctions& trivariateFunctions);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();

    bool setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable);

    bool setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity);

    void resetChargeOnlyModeRatedStorageChargingCapacity();

    void autocalculateChargeOnlyModeRatedStorageChargingCapacity();

    bool setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor);

    bool setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();

    bool setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable);

    bool setDischargeOnlyModeRatedStorageDischargingCapacity(double dischargeOnlyModeRatedStorageDischargingCapacity);

    void resetDischargeOnlyModeRatedStorageDischargingCapacity();

    void autocalculateDischargeOnlyModeRatedStorageDischargingCapacity();

    bool setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor);

    bool setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio);

    void resetDischargeOnlyModeRatedSensibleHeatRatio();

    bool setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP);

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

    void resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setDischargeOnlyModePartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions);

    void resetDischargeOnlyModePartLoadFractionCorrelationCurve();

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions, TrivariateFunctions.
    bool setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions);

    void resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

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
