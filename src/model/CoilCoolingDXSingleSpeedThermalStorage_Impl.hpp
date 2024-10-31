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

#ifndef MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_IMPL_HPP
#define MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** CoilCoolingDXSingleSpeedThermalStorage_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXSingleSpeedThermalStorage.*/
    class MODEL_API CoilCoolingDXSingleSpeedThermalStorage_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXSingleSpeedThermalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXSingleSpeedThermalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXSingleSpeedThermalStorage_Impl(const CoilCoolingDXSingleSpeedThermalStorage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXSingleSpeedThermalStorage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
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

      virtual void autosize() override;

      virtual void applySizingValues() override;

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
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeedThermalStorage");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXSINGLESPEEDTHERMALSTORAGE_IMPL_HPP
