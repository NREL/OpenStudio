/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
class MODEL_API AirConditionerVariableRefrigerantFlow_Impl : public StraightComponent_Impl {

 public:

  AirConditionerVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

  AirConditionerVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

  AirConditionerVariableRefrigerantFlow_Impl(const AirConditionerVariableRefrigerantFlow_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

  virtual ~AirConditionerVariableRefrigerantFlow_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

  Schedule availabilitySchedule() const;

  boost::optional<double> ratedTotalCoolingCapacity() const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  double ratedCoolingCOP() const;

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

  boost::optional<double> ratedTotalHeatingCapacity() const;

  bool isRatedTotalHeatingCapacityAutosized() const;

  double ratedTotalHeatingCapacitySizingRatio() const;

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

  bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();

  bool setRatedCoolingCOP(double ratedCoolingCOP);

  void setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode);

  void setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode);

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

  void setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

  void autosizeRatedTotalHeatingCapacity();

  bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

  void setRatedHeatingCOP(double ratedHeatingCOP);

  void setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode);

  void setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode);

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

  bool setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType);

  bool setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<Curve>& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();

  bool setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<Curve>& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();

  bool setHeatingCombinationRatioCorrectionFactorCurve(const boost::optional<Curve>& curve);

  void resetHeatingCombinationRatioCorrectionFactorCurve();

  bool setHeatingPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve);

  void resetHeatingPartLoadFractionCorrelationCurve();

  void setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio);

  bool setZoneforMasterThermostatLocation(const boost::optional<ThermalZone>& zone);

  void resetZoneforMasterThermostatLocation();

  bool setMasterThermostatPriorityControlType(std::string masterThermostatPriorityControlType);

  bool setThermostatPrioritySchedule(Schedule& schedule);

  void resetThermostatPrioritySchedule();

  void setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery);

  void setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);

  void setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor);

  bool setPipingCorrectionFactorforLengthinCoolingModeCurve(const boost::optional<Curve>& curve);

  void resetPipingCorrectionFactorforLengthinCoolingModeCurve();

  void setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient);

  void setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);

  bool setPipingCorrectionFactorforLengthinHeatingModeCurve(const boost::optional<Curve>& curve);

  void resetPipingCorrectionFactorforLengthinHeatingModeCurve();

  void setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient);

  void setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);

  void setNumberofCompressors(int numberofCompressors);

  void setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);

  void setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater);

  bool setDefrostStrategy(std::string defrostStrategy);

  bool setDefrostControl(std::string defrostControl);

  bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

  void resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

  bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

  void autosizeResistiveDefrostHeaterCapacity();

  void setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

  void setWaterCondenserVolumeFlowRate(boost::optional<double> waterCondenserVolumeFlowRate);

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

  bool setFuelType(std::string fuelType);

  void setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);

  void setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);

  bool setHeatRecoveryCoolingCapacityModifierCurve(const boost::optional<Curve>& curve);

  void resetHeatRecoveryCoolingCapacityModifierCurve();

  void setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);

  void setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);

  bool setHeatRecoveryCoolingEnergyModifierCurve(const boost::optional<Curve>& curve);

  void resetHeatRecoveryCoolingEnergyModifierCurve();

  void setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);

  void setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);

  bool setHeatRecoveryHeatingCapacityModifierCurve(const boost::optional<Curve>& curve);

  void resetHeatRecoveryHeatingCapacityModifierCurve();

  void setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);

  void setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);

  bool setHeatRecoveryHeatingEnergyModifierCurve(const boost::optional<Curve>& curve);

  void resetHeatRecoveryHeatingEnergyModifierCurve();

  void setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);

  void setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

  unsigned inletPort() override;

  unsigned outletPort() override;

  ModelObjectList vrfModelObjectList() const;

  bool setVRFModelObjectList(const ModelObjectList & modelObjectList);

  void addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf);

  void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf);

  void removeAllTerminals();

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

  ModelObject clone(Model model) const override;

  std::vector<openstudio::IdfObject> remove() override;

  bool addToNode(Node & node) override;

  std::vector<ModelObject> children() const override;

 private:

  REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlow");

  boost::optional<Schedule> optionalAvailabilitySchedule() const;
};

} // detail

} // model
} // openstudio

#endif // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_IMPL_HPP

