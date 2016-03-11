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

#ifndef MODEL_WATERHEATERSTRATIFIED_HPP
#define MODEL_WATERHEATERSTRATIFIED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

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

  boost::optional<double> tankVolume() const;

  bool isTankVolumeAutosized() const;

  boost::optional<double> tankHeight() const;

  bool isTankHeightAutosized() const;

  std::string tankShape() const;

  boost::optional<double> tankPerimeter() const;

  double maximumTemperatureLimit() const;

  std::string heaterPriorityControl() const;

  Schedule heater1SetpointTemperatureSchedule() const;

  double heater1DeadbandTemperatureDifference() const;

  boost::optional<double> heater1Capacity() const;

  bool isHeater1CapacityAutosized() const;

  double heater1Height() const;

  Schedule heater2SetpointTemperatureSchedule() const;

  double heater2DeadbandTemperatureDifference() const;

  double heater2Capacity() const;

  double heater2Height() const;

  std::string heaterFuelType() const;

  double heaterThermalEfficiency() const;

  double offCycleParasiticFuelConsumptionRate() const;

  std::string offCycleParasiticFuelType() const;

  double offCycleParasiticHeatFractiontoTank() const;

  double offCycleParasiticHeight() const;

  double onCycleParasiticFuelConsumptionRate() const;

  std::string onCycleParasiticFuelType() const;

  double onCycleParasiticHeatFractiontoTank() const;

  double onCycleParasiticHeight() const;

  std::string ambientTemperatureIndicator() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

  boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

  boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

  double skinLossFractiontoZone() const;

  boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature() const;

  double offCycleFlueLossFractiontoZone() const;

  boost::optional<double> peakUseFlowRate() const;

  boost::optional<Schedule> useFlowRateFractionSchedule() const;

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  double useSideEffectiveness() const;

  double useSideInletHeight() const;

  boost::optional<double> useSideOutletHeight() const;

  bool isUseSideOutletHeightAutocalculated() const;

  double sourceSideEffectiveness() const;

  boost::optional<double> sourceSideInletHeight() const;

  bool isSourceSideInletHeightAutocalculated() const;

  double sourceSideOutletHeight() const;

  std::string inletMode() const;

  boost::optional<double> useSideDesignFlowRate() const;

  bool isUseSideDesignFlowRateAutosized() const;

  boost::optional<double> sourceSideDesignFlowRate() const;

  bool isSourceSideDesignFlowRateAutosized() const;

  double indirectWaterHeatingRecoveryTime() const;

  int numberofNodes() const;

  double additionalDestratificationConductivity() const;

  double node1AdditionalLossCoefficient() const;

  double node2AdditionalLossCoefficient() const;

  double node3AdditionalLossCoefficient() const;

  double node4AdditionalLossCoefficient() const;

  double node5AdditionalLossCoefficient() const;

  double node6AdditionalLossCoefficient() const;

  double node7AdditionalLossCoefficient() const;

  double node8AdditionalLossCoefficient() const;

  double node9AdditionalLossCoefficient() const;

  double node10AdditionalLossCoefficient() const;

  std::string sourceSideFlowControlMode() const;

  boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  void setEndUseSubcategory(std::string endUseSubcategory);

  bool setTankVolume(double tankVolume);

  void autosizeTankVolume();

  bool setTankHeight(double tankHeight);

  void autosizeTankHeight();

  bool setTankShape(std::string tankShape);

  bool setTankPerimeter(double tankPerimeter);

  void resetTankPerimeter();

  void setMaximumTemperatureLimit(double maximumTemperatureLimit);

  bool setHeaterPriorityControl(std::string heaterPriorityControl);

  bool setHeater1SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference);

  bool setHeater1Capacity(double heater1Capacity);

  void autosizeHeater1Capacity();

  bool setHeater1Height(double heater1Height);

  bool setHeater2SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference);

  bool setHeater2Capacity(double heater2Capacity);

  bool setHeater2Height(double heater2Height);

  bool setHeaterFuelType(std::string heaterFuelType);

  bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

  bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

  bool setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType);

  bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

  bool setOffCycleParasiticHeight(double offCycleParasiticHeight);

  bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

  bool setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType);

  bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

  bool setOnCycleParasiticHeight(double onCycleParasiticHeight);

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

  bool setOffCycleFlueLossCoefficienttoAmbientTemperature(double offCycleFlueLossCoefficienttoAmbientTemperature);

  void resetOffCycleFlueLossCoefficienttoAmbientTemperature();

  bool setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone);

  bool setPeakUseFlowRate(double peakUseFlowRate);

  void resetPeakUseFlowRate();

  bool setUseFlowRateFractionSchedule(Schedule& schedule);

  void resetUseFlowRateFractionSchedule();

  bool setColdWaterSupplyTemperatureSchedule(Schedule& schedule);

  void resetColdWaterSupplyTemperatureSchedule();

  bool setUseSideEffectiveness(double useSideEffectiveness);

  bool setUseSideInletHeight(double useSideInletHeight);

  bool setUseSideOutletHeight(double useSideOutletHeight);

  void autocalculateUseSideOutletHeight();

  bool setSourceSideEffectiveness(double sourceSideEffectiveness);

  bool setSourceSideInletHeight(double sourceSideInletHeight);

  void autocalculateSourceSideInletHeight();

  bool setSourceSideOutletHeight(double sourceSideOutletHeight);

  bool setInletMode(std::string inletMode);

  bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

  void autosizeUseSideDesignFlowRate();

  bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

  void autosizeSourceSideDesignFlowRate();

  bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

  bool setNumberofNodes(int numberofNodes);

  bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

  void setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

  void setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

  void setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

  void setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

  void setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

  void setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

  void setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

  void setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

  void setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

  void setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

  bool setSourceSideFlowControlMode(std::string sourceSideFlowControlMode);

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

