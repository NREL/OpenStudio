/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_WATERHEATERSTRATIFIED_HPP
#define MODEL_WATERHEATERSTRATIFIED_HPP

#include <model/ModelAPI.hpp>
#include <model/WaterToWaterComponent.hpp>

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class WaterHeaterStratified_Impl;

} // detail

/** WaterHeaterStratified is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:Stratified'. */
class MODEL_API WaterHeaterStratified : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterStratified(const Model& model);

  virtual ~WaterHeaterStratified() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> tankShapeValues();

  static std::vector<std::string> heaterPriorityControlValues();

  static std::vector<std::string> heaterFuelTypeValues();

  static std::vector<std::string> offCycleParasiticFuelTypeValues();

  static std::vector<std::string> onCycleParasiticFuelTypeValues();

  static std::vector<std::string> ambientTemperatureIndicatorValues();

  static std::vector<std::string> inletModeValues();

  static std::vector<std::string> sourceSideFlowControlModeValues();

  /** @name Getters */
  //@{

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  boost::optional<double> tankVolume() const;

  bool isTankVolumeAutosized() const;

  boost::optional<double> tankHeight() const;

  bool isTankHeightAutosized() const;

  std::string tankShape() const;

  bool isTankShapeDefaulted() const;

  boost::optional<double> tankPerimeter() const;

  boost::optional<double> maximumTemperatureLimit() const;

  std::string heaterPriorityControl() const;

  bool isHeaterPriorityControlDefaulted() const;

  Schedule heater1SetpointTemperatureSchedule() const;

  double heater1DeadbandTemperatureDifference() const;

  bool isHeater1DeadbandTemperatureDifferenceDefaulted() const;

  boost::optional<double> heater1Capacity() const;

  bool isHeater1CapacityAutosized() const;

  boost::optional<double> heater1Height() const;

  Schedule heater2SetpointTemperatureSchedule() const;

  double heater2DeadbandTemperatureDifference() const;

  bool isHeater2DeadbandTemperatureDifferenceDefaulted() const;

  boost::optional<double> heater2Capacity() const;

  boost::optional<double> heater2Height() const;

  std::string heaterFuelType() const;

  double heaterThermalEfficiency() const;

  double offCycleParasiticFuelConsumptionRate() const;

  bool isOffCycleParasiticFuelConsumptionRateDefaulted() const;

  boost::optional<std::string> offCycleParasiticFuelType() const;

  double offCycleParasiticHeatFractiontoTank() const;

  bool isOffCycleParasiticHeatFractiontoTankDefaulted() const;

  double offCycleParasiticHeight() const;

  bool isOffCycleParasiticHeightDefaulted() const;

  double onCycleParasiticFuelConsumptionRate() const;

  bool isOnCycleParasiticFuelConsumptionRateDefaulted() const;

  boost::optional<std::string> onCycleParasiticFuelType() const;

  double onCycleParasiticHeatFractiontoTank() const;

  bool isOnCycleParasiticHeatFractiontoTankDefaulted() const;

  double onCycleParasiticHeight() const;

  bool isOnCycleParasiticHeightDefaulted() const;

  std::string ambientTemperatureIndicator() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

  boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

  boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

  double skinLossFractiontoZone() const;

  bool isSkinLossFractiontoZoneDefaulted() const;

  boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature() const;

  double offCycleFlueLossFractiontoZone() const;

  bool isOffCycleFlueLossFractiontoZoneDefaulted() const;

  boost::optional<double> peakUseFlowRate() const;

  boost::optional<Schedule> useFlowRateFractionSchedule() const;

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  double useSideEffectiveness() const;

  bool isUseSideEffectivenessDefaulted() const;

  double useSideInletHeight() const;

  bool isUseSideInletHeightDefaulted() const;

  boost::optional<double> useSideOutletHeight() const;

  bool isUseSideOutletHeightDefaulted() const;

  bool isUseSideOutletHeightAutocalculated() const;

  double sourceSideEffectiveness() const;

  bool isSourceSideEffectivenessDefaulted() const;

  boost::optional<double> sourceSideInletHeight() const;

  bool isSourceSideInletHeightDefaulted() const;

  bool isSourceSideInletHeightAutocalculated() const;

  double sourceSideOutletHeight() const;

  bool isSourceSideOutletHeightDefaulted() const;

  std::string inletMode() const;

  bool isInletModeDefaulted() const;

  boost::optional<double> useSideDesignFlowRate() const;

  bool isUseSideDesignFlowRateDefaulted() const;

  bool isUseSideDesignFlowRateAutosized() const;

  boost::optional<double> sourceSideDesignFlowRate() const;

  bool isSourceSideDesignFlowRateDefaulted() const;

  bool isSourceSideDesignFlowRateAutosized() const;

  double indirectWaterHeatingRecoveryTime() const;

  bool isIndirectWaterHeatingRecoveryTimeDefaulted() const;

  int numberofNodes() const;

  bool isNumberofNodesDefaulted() const;

  double additionalDestratificationConductivity() const;

  bool isAdditionalDestratificationConductivityDefaulted() const;

  double node1AdditionalLossCoefficient() const;

  bool isNode1AdditionalLossCoefficientDefaulted() const;

  double node2AdditionalLossCoefficient() const;

  bool isNode2AdditionalLossCoefficientDefaulted() const;

  double node3AdditionalLossCoefficient() const;

  bool isNode3AdditionalLossCoefficientDefaulted() const;

  double node4AdditionalLossCoefficient() const;

  bool isNode4AdditionalLossCoefficientDefaulted() const;

  double node5AdditionalLossCoefficient() const;

  bool isNode5AdditionalLossCoefficientDefaulted() const;

  double node6AdditionalLossCoefficient() const;

  bool isNode6AdditionalLossCoefficientDefaulted() const;

  double node7AdditionalLossCoefficient() const;

  bool isNode7AdditionalLossCoefficientDefaulted() const;

  double node8AdditionalLossCoefficient() const;

  bool isNode8AdditionalLossCoefficientDefaulted() const;

  double node9AdditionalLossCoefficient() const;

  bool isNode9AdditionalLossCoefficientDefaulted() const;

  double node10AdditionalLossCoefficient() const;

  bool isNode10AdditionalLossCoefficientDefaulted() const;

  std::string sourceSideFlowControlMode() const;

  bool isSourceSideFlowControlModeDefaulted() const;

  boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  bool setTankVolume(double tankVolume);

  void autosizeTankVolume();

  bool setTankHeight(double tankHeight);

  void autosizeTankHeight();

  bool setTankShape(std::string tankShape);

  void resetTankShape();

  bool setTankPerimeter(double tankPerimeter);

  void resetTankPerimeter();

  void setMaximumTemperatureLimit(double maximumTemperatureLimit);

  void resetMaximumTemperatureLimit();

  bool setHeaterPriorityControl(std::string heaterPriorityControl);

  void resetHeaterPriorityControl();

  bool setHeater1SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference);

  void resetHeater1DeadbandTemperatureDifference();

  bool setHeater1Capacity(double heater1Capacity);

  void resetHeater1Capacity();

  void autosizeHeater1Capacity();

  bool setHeater1Height(double heater1Height);

  void resetHeater1Height();

  bool setHeater2SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference);

  void resetHeater2DeadbandTemperatureDifference();

  bool setHeater2Capacity(double heater2Capacity);

  void resetHeater2Capacity();

  bool setHeater2Height(double heater2Height);

  void resetHeater2Height();

  bool setHeaterFuelType(std::string heaterFuelType);

  bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

  bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

  void resetOffCycleParasiticFuelConsumptionRate();

  bool setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType);

  void resetOffCycleParasiticFuelType();

  bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

  void resetOffCycleParasiticHeatFractiontoTank();

  bool setOffCycleParasiticHeight(double offCycleParasiticHeight);

  void resetOffCycleParasiticHeight();

  bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

  void resetOnCycleParasiticFuelConsumptionRate();

  bool setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType);

  void resetOnCycleParasiticFuelType();

  bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

  void resetOnCycleParasiticHeatFractiontoTank();

  bool setOnCycleParasiticHeight(double onCycleParasiticHeight);

  void resetOnCycleParasiticHeight();

  bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

  bool setAmbientTemperatureSchedule(Schedule& schedule);

  void resetAmbientTemperatureSchedule();

  bool setAmbientTemperatureThermalZone(const ThermalZone& thermalZone);

  void resetAmbientTemperatureThermalZone();

  void setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName);

  void resetAmbientTemperatureOutdoorAirNodeName();

  bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

  void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

  bool setSkinLossFractiontoZone(double skinLossFractiontoZone);

  void resetSkinLossFractiontoZone();

  bool setOffCycleFlueLossCoefficienttoAmbientTemperature(double offCycleFlueLossCoefficienttoAmbientTemperature);

  void resetOffCycleFlueLossCoefficienttoAmbientTemperature();

  bool setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone);

  void resetOffCycleFlueLossFractiontoZone();

  bool setPeakUseFlowRate(double peakUseFlowRate);

  void resetPeakUseFlowRate();

  bool setUseFlowRateFractionSchedule(Schedule& schedule);

  void resetUseFlowRateFractionSchedule();

  bool setColdWaterSupplyTemperatureSchedule(Schedule& schedule);

  void resetColdWaterSupplyTemperatureSchedule();

  bool setUseSideEffectiveness(double useSideEffectiveness);

  void resetUseSideEffectiveness();

  bool setUseSideInletHeight(double useSideInletHeight);

  void resetUseSideInletHeight();

  bool setUseSideOutletHeight(double useSideOutletHeight);

  void resetUseSideOutletHeight();

  void autocalculateUseSideOutletHeight();

  bool setSourceSideEffectiveness(double sourceSideEffectiveness);

  void resetSourceSideEffectiveness();

  bool setSourceSideInletHeight(double sourceSideInletHeight);

  void resetSourceSideInletHeight();

  void autocalculateSourceSideInletHeight();

  bool setSourceSideOutletHeight(double sourceSideOutletHeight);

  void resetSourceSideOutletHeight();

  bool setInletMode(std::string inletMode);

  void resetInletMode();

  bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

  void resetUseSideDesignFlowRate();

  void autosizeUseSideDesignFlowRate();

  bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

  void resetSourceSideDesignFlowRate();

  void autosizeSourceSideDesignFlowRate();

  bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

  void resetIndirectWaterHeatingRecoveryTime();

  bool setNumberofNodes(int numberofNodes);

  void resetNumberofNodes();

  bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

  void resetAdditionalDestratificationConductivity();

  void setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

  void resetNode1AdditionalLossCoefficient();

  void setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

  void resetNode2AdditionalLossCoefficient();

  void setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

  void resetNode3AdditionalLossCoefficient();

  void setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

  void resetNode4AdditionalLossCoefficient();

  void setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

  void resetNode5AdditionalLossCoefficient();

  void setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

  void resetNode6AdditionalLossCoefficient();

  void setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

  void resetNode7AdditionalLossCoefficient();

  void setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

  void resetNode8AdditionalLossCoefficient();

  void setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

  void resetNode9AdditionalLossCoefficient();

  void setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

  void resetNode10AdditionalLossCoefficient();

  bool setSourceSideFlowControlMode(std::string sourceSideFlowControlMode);

  void resetSourceSideFlowControlMode();

  bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule);

  void resetIndirectAlternateSetpointTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::WaterHeaterStratified_Impl ImplType;

  explicit WaterHeaterStratified(std::shared_ptr<detail::WaterHeaterStratified_Impl> impl);

  friend class detail::WaterHeaterStratified_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.WaterHeaterStratified");
};

/** \relates WaterHeaterStratified*/
typedef boost::optional<WaterHeaterStratified> OptionalWaterHeaterStratified;

/** \relates WaterHeaterStratified*/
typedef std::vector<WaterHeaterStratified> WaterHeaterStratifiedVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERSTRATIFIED_HPP

