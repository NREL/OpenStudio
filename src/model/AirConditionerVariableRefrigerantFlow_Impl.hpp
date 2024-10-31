/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class ModelObjectList;
  class ZoneHVACTerminalUnitVariableRefrigerantFlow;

  namespace detail {

    /** AirConditionerVariableRefrigerantFlow_Impl is a StraightComponent_Impl that is the implementation class for AirConditionerVariableRefrigerantFlow.*/
    class MODEL_API AirConditionerVariableRefrigerantFlow_Impl : public StraightComponent_Impl
    {

     public:
      AirConditionerVariableRefrigerantFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirConditionerVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirConditionerVariableRefrigerantFlow_Impl(const AirConditionerVariableRefrigerantFlow_Impl& other, Model_Impl* model, bool keepHandle);

      /** @name Virtual Methods */
      //@{

      virtual ~AirConditionerVariableRefrigerantFlow_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool addToNode(Node& node) override;

      virtual bool removeFromLoop() override;

      virtual std::vector<ModelObject> children() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}

      Schedule availabilitySchedule() const;

      boost::optional<double> grossRatedTotalCoolingCapacity() const;

      bool isGrossRatedTotalCoolingCapacityAutosized() const;

      double grossRatedCoolingCOP() const;

      double minimumOutdoorTemperatureinCoolingMode() const;

      double maximumOutdoorTemperatureinCoolingMode() const;

      boost::optional<Curve> coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

      boost::optional<Curve> coolingCapacityRatioBoundaryCurve() const;

      boost::optional<Curve> coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

      boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

      boost::optional<Curve> coolingEnergyInputRatioBoundaryCurve() const;

      boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

      boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

      boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

      boost::optional<Curve> coolingCombinationRatioCorrectionFactorCurve() const;

      boost::optional<Curve> coolingPartLoadFractionCorrelationCurve() const;

      boost::optional<double> grossRatedHeatingCapacity() const;

      bool isGrossRatedHeatingCapacityAutosized() const;

      double ratedHeatingCapacitySizingRatio() const;

      double ratedHeatingCOP() const;

      double minimumOutdoorTemperatureinHeatingMode() const;

      double maximumOutdoorTemperatureinHeatingMode() const;

      boost::optional<Curve> heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

      boost::optional<Curve> heatingCapacityRatioBoundaryCurve() const;

      boost::optional<Curve> heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

      boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

      boost::optional<Curve> heatingEnergyInputRatioBoundaryCurve() const;

      boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

      std::string heatingPerformanceCurveOutdoorTemperatureType() const;

      boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

      boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

      boost::optional<Curve> heatingCombinationRatioCorrectionFactorCurve() const;

      boost::optional<Curve> heatingPartLoadFractionCorrelationCurve() const;

      double minimumHeatPumpPartLoadRatio() const;

      boost::optional<ThermalZone> zoneforMasterThermostatLocation() const;

      std::string masterThermostatPriorityControlType() const;

      boost::optional<Schedule> thermostatPrioritySchedule() const;

      bool heatPumpWasteHeatRecovery() const;

      double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const;

      double verticalHeightusedforPipingCorrectionFactor() const;

      boost::optional<Curve> pipingCorrectionFactorforLengthinCoolingModeCurve() const;

      double pipingCorrectionFactorforHeightinCoolingModeCoefficient() const;

      double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const;

      boost::optional<Curve> pipingCorrectionFactorforLengthinHeatingModeCurve() const;

      double pipingCorrectionFactorforHeightinHeatingModeCoefficient() const;

      double crankcaseHeaterPowerperCompressor() const;

      int numberofCompressors() const;

      double ratioofCompressorSizetoTotalCompressorCapacity() const;

      double maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const;

      std::string defrostStrategy() const;

      std::string defrostControl() const;

      boost::optional<Curve> defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const;

      double defrostTimePeriodFraction() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      double maximumOutdoorDrybulbTemperatureforDefrostOperation() const;

      std::string condenserType() const;
      bool setCondenserType(const std::string& condenserType);
      bool isCondenserTypeDefaulted() const;
      void resetCondenserType();

      boost::optional<double> waterCondenserVolumeFlowRate() const;

      bool isWaterCondenserVolumeFlowRateAutosized() const;

      double evaporativeCondenserEffectiveness() const;

      boost::optional<double> evaporativeCondenserAirFlowRate() const;

      bool isEvaporativeCondenserAirFlowRateAutosized() const;

      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

      bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      std::string fuelType() const;

      double minimumOutdoorTemperatureinHeatRecoveryMode() const;

      double maximumOutdoorTemperatureinHeatRecoveryMode() const;

      boost::optional<Curve> heatRecoveryCoolingCapacityModifierCurve() const;

      double initialHeatRecoveryCoolingCapacityFraction() const;

      double heatRecoveryCoolingCapacityTimeConstant() const;

      boost::optional<Curve> heatRecoveryCoolingEnergyModifierCurve() const;

      double initialHeatRecoveryCoolingEnergyFraction() const;

      double heatRecoveryCoolingEnergyTimeConstant() const;

      boost::optional<Curve> heatRecoveryHeatingCapacityModifierCurve() const;

      double initialHeatRecoveryHeatingCapacityFraction() const;

      double heatRecoveryHeatingCapacityTimeConstant() const;

      boost::optional<Curve> heatRecoveryHeatingEnergyModifierCurve() const;

      double initialHeatRecoveryHeatingEnergyFraction() const;

      double heatRecoveryHeatingEnergyTimeConstant() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setGrossRatedTotalCoolingCapacity(boost::optional<double> grossRatedTotalCoolingCapacity);

      void autosizeGrossRatedTotalCoolingCapacity();

      bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

      bool setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode);

      bool setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode);

      bool setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve);

      void resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();

      bool setCoolingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve);

      void resetCoolingCapacityRatioBoundaryCurve();

      bool setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve);

      void resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();

      bool setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve);

      void resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();

      bool setCoolingEnergyInputRatioBoundaryCurve(const boost::optional<Curve>& curve);

      void resetCoolingEnergyInputRatioBoundaryCurve();

      bool setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve);

      void resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();

      bool setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<Curve>& curve);

      void resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();

      bool setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<Curve>& curve);

      void resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();

      bool setCoolingCombinationRatioCorrectionFactorCurve(const boost::optional<Curve>& curve);

      void resetCoolingCombinationRatioCorrectionFactorCurve();

      bool setCoolingPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve);

      void resetCoolingPartLoadFractionCorrelationCurve();

      bool setGrossRatedHeatingCapacity(boost::optional<double> grossRatedHeatingCapacity);

      void autosizeGrossRatedHeatingCapacity();

      bool setRatedHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio);

      bool setRatedHeatingCOP(double ratedHeatingCOP);

      bool setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode);

      bool setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode);

      bool setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve);

      void resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();

      bool setHeatingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve);

      void resetHeatingCapacityRatioBoundaryCurve();

      bool setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve);

      void resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();

      bool setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve);

      void resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();

      bool setHeatingEnergyInputRatioBoundaryCurve(const boost::optional<Curve>& curve);

      void resetHeatingEnergyInputRatioBoundaryCurve();

      bool setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve);

      void resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();

      bool setHeatingPerformanceCurveOutdoorTemperatureType(const std::string& heatingPerformanceCurveOutdoorTemperatureType);

      bool setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<Curve>& curve);

      void resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();

      bool setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<Curve>& curve);

      void resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();

      bool setHeatingCombinationRatioCorrectionFactorCurve(const boost::optional<Curve>& curve);

      void resetHeatingCombinationRatioCorrectionFactorCurve();

      bool setHeatingPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve);

      void resetHeatingPartLoadFractionCorrelationCurve();

      bool setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio);

      bool setZoneforMasterThermostatLocation(const boost::optional<ThermalZone>& zone);

      void resetZoneforMasterThermostatLocation();

      bool setMasterThermostatPriorityControlType(const std::string& masterThermostatPriorityControlType);

      bool setThermostatPrioritySchedule(Schedule& schedule);

      void resetThermostatPrioritySchedule();

      bool setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery);

      bool
        setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);

      bool setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor);

      bool setPipingCorrectionFactorforLengthinCoolingModeCurve(const boost::optional<Curve>& curve);

      void resetPipingCorrectionFactorforLengthinCoolingModeCurve();

      bool setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient);

      bool
        setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);

      bool setPipingCorrectionFactorforLengthinHeatingModeCurve(const boost::optional<Curve>& curve);

      void resetPipingCorrectionFactorforLengthinHeatingModeCurve();

      bool setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient);

      bool setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);

      bool setNumberofCompressors(int numberofCompressors);

      bool setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);

      bool setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater);

      bool setDefrostStrategy(const std::string& defrostStrategy);

      bool setDefrostControl(const std::string& defrostControl);

      bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

      void resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

      void autosizeResistiveDefrostHeaterCapacity();

      bool setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

      bool setWaterCondenserVolumeFlowRate(boost::optional<double> waterCondenserVolumeFlowRate);

      void autosizeWaterCondenserVolumeFlowRate();

      bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

      bool setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate);

      void autosizeEvaporativeCondenserAirFlowRate();

      bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption);

      void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setFuelType(const std::string& fuelType);

      bool setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);

      bool setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);

      bool setHeatRecoveryCoolingCapacityModifierCurve(const boost::optional<Curve>& curve);

      void resetHeatRecoveryCoolingCapacityModifierCurve();

      bool setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);

      bool setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);

      bool setHeatRecoveryCoolingEnergyModifierCurve(const boost::optional<Curve>& curve);

      void resetHeatRecoveryCoolingEnergyModifierCurve();

      bool setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);

      bool setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);

      bool setHeatRecoveryHeatingCapacityModifierCurve(const boost::optional<Curve>& curve);

      void resetHeatRecoveryHeatingCapacityModifierCurve();

      bool setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);

      bool setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);

      bool setHeatRecoveryHeatingEnergyModifierCurve(const boost::optional<Curve>& curve);

      void resetHeatRecoveryHeatingEnergyModifierCurve();

      bool setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);

      bool setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

      ModelObjectList vrfModelObjectList() const;

      bool setVRFModelObjectList(const ModelObjectList& modelObjectList);

      bool addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

      void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

      void removeAllTerminals();

      std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

      boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

      boost::optional<double> autosizedWaterCondenserVolumeFlowRate() const;

      boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

     private:
      REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlow");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_IMPL_HPP
