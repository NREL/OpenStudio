/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_WATERHEATERMIXED_HPP
#define MODEL_WATERHEATERMIXED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class CurveCubic;
class ThermalZone;

namespace detail {

  class WaterHeaterMixed_Impl;

} // detail

/** WaterHeaterMixed is a WaterToWaterComponent that wraps the OpenStudio IDD object 
 *  'OS:WaterHeater:Mixed'. */
class MODEL_API WaterHeaterMixed : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterMixed(const Model& model);

  virtual ~WaterHeaterMixed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heaterControlTypeValues();

  static std::vector<std::string> heaterFuelTypeValues();

  static std::vector<std::string> offCycleParasiticFuelTypeValues();

  static std::vector<std::string> onCycleParasiticFuelTypeValues();

  static std::vector<std::string> ambientTemperatureIndicatorValues();

  /** @name Getters */
  //@{

  boost::optional<double> tankVolume() const;

  OSOptionalQuantity getTankVolume(bool returnIP=false) const;

  bool isTankVolumeDefaulted() const;

  bool isTankVolumeAutosized() const;

  boost::optional<Schedule> setpointTemperatureSchedule() const;

  double deadbandTemperatureDifference() const;

  Quantity getDeadbandTemperatureDifference(bool returnIP=false) const;

  bool isDeadbandTemperatureDifferenceDefaulted() const;

  boost::optional<double> maximumTemperatureLimit() const;

  OSOptionalQuantity getMaximumTemperatureLimit(bool returnIP=false) const;

  std::string heaterControlType() const;

  bool isHeaterControlTypeDefaulted() const;

  boost::optional<double> heaterMaximumCapacity() const;

  OSOptionalQuantity getHeaterMaximumCapacity(bool returnIP=false) const;

  bool isHeaterMaximumCapacityAutosized() const;

  boost::optional<double> heaterMinimumCapacity() const;

  OSOptionalQuantity getHeaterMinimumCapacity(bool returnIP=false) const;

  double heaterIgnitionMinimumFlowRate() const;

  Quantity getHeaterIgnitionMinimumFlowRate(bool returnIP=false) const;

  bool isHeaterIgnitionMinimumFlowRateDefaulted() const;

  double heaterIgnitionDelay() const;

  Quantity getHeaterIgnitionDelay(bool returnIP=false) const;

  bool isHeaterIgnitionDelayDefaulted() const;

  std::string heaterFuelType() const;

  boost::optional<double> heaterThermalEfficiency() const;

  OSOptionalQuantity getHeaterThermalEfficiency(bool returnIP=false) const;

  boost::optional<CurveCubic> partLoadFactorCurve() const;

  double offCycleParasiticFuelConsumptionRate() const;

  Quantity getOffCycleParasiticFuelConsumptionRate(bool returnIP=false) const;

  bool isOffCycleParasiticFuelConsumptionRateDefaulted() const;

  boost::optional<std::string> offCycleParasiticFuelType() const;

  double offCycleParasiticHeatFractiontoTank() const;

  Quantity getOffCycleParasiticHeatFractiontoTank(bool returnIP=false) const;

  bool isOffCycleParasiticHeatFractiontoTankDefaulted() const;

  double onCycleParasiticFuelConsumptionRate() const;

  Quantity getOnCycleParasiticFuelConsumptionRate(bool returnIP=false) const;

  bool isOnCycleParasiticFuelConsumptionRateDefaulted() const;

  boost::optional<std::string> onCycleParasiticFuelType() const;

  double onCycleParasiticHeatFractiontoTank() const;

  Quantity getOnCycleParasiticHeatFractiontoTank(bool returnIP=false) const;

  bool isOnCycleParasiticHeatFractiontoTankDefaulted() const;

  std::string ambientTemperatureIndicator() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

  boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

  boost::optional<double> offCycleLossCoefficienttoAmbientTemperature() const;

  OSOptionalQuantity getOffCycleLossCoefficienttoAmbientTemperature(bool returnIP=false) const;

  double offCycleLossFractiontoThermalZone() const;

  Quantity getOffCycleLossFractiontoThermalZone(bool returnIP=false) const;

  bool isOffCycleLossFractiontoThermalZoneDefaulted() const;

  boost::optional<double> onCycleLossCoefficienttoAmbientTemperature() const;

  OSOptionalQuantity getOnCycleLossCoefficienttoAmbientTemperature(bool returnIP=false) const;

  double onCycleLossFractiontoThermalZone() const;

  Quantity getOnCycleLossFractiontoThermalZone(bool returnIP=false) const;

  bool isOnCycleLossFractiontoThermalZoneDefaulted() const;

  boost::optional<double> peakUseFlowRate() const;

  OSOptionalQuantity getPeakUseFlowRate(bool returnIP=false) const;

  boost::optional<Schedule> useFlowRateFractionSchedule() const;

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  double useSideEffectiveness() const;

  Quantity getUseSideEffectiveness(bool returnIP=false) const;

  bool isUseSideEffectivenessDefaulted() const;

  double sourceSideEffectiveness() const;

  Quantity getSourceSideEffectiveness(bool returnIP=false) const;

  bool isSourceSideEffectivenessDefaulted() const;

  boost::optional<double> useSideDesignFlowRate() const;

  OSOptionalQuantity getUseSideDesignFlowRate(bool returnIP=false) const;

  bool isUseSideDesignFlowRateDefaulted() const;

  bool isUseSideDesignFlowRateAutosized() const;

  boost::optional<double> sourceSideDesignFlowRate() const;

  OSOptionalQuantity getSourceSideDesignFlowRate(bool returnIP=false) const;

  bool isSourceSideDesignFlowRateDefaulted() const;

  bool isSourceSideDesignFlowRateAutosized() const;

  double indirectWaterHeatingRecoveryTime() const;

  Quantity getIndirectWaterHeatingRecoveryTime(bool returnIP=false) const;

  bool isIndirectWaterHeatingRecoveryTimeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setTankVolume(double tankVolume);

  bool setTankVolume(const Quantity& tankVolume);

  void resetTankVolume();

  void autosizeTankVolume();

  bool setSetpointTemperatureSchedule(Schedule& setpointTemperatureSchedule);

  void resetSetpointTemperatureSchedule();

  bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

  bool setDeadbandTemperatureDifference(const Quantity& deadbandTemperatureDifference);

  void resetDeadbandTemperatureDifference();

  void setMaximumTemperatureLimit(double maximumTemperatureLimit);

  bool setMaximumTemperatureLimit(const Quantity& maximumTemperatureLimit);

  void resetMaximumTemperatureLimit();

  bool setHeaterControlType(std::string heaterControlType);

  void resetHeaterControlType();

  bool setHeaterMaximumCapacity(double heaterMaximumCapacity);

  bool setHeaterMaximumCapacity(const Quantity& heaterMaximumCapacity);

  void resetHeaterMaximumCapacity();

  void autosizeHeaterMaximumCapacity();

  bool setHeaterMinimumCapacity(double heaterMinimumCapacity);

  bool setHeaterMinimumCapacity(const Quantity& heaterMinimumCapacity);

  void resetHeaterMinimumCapacity();

  bool setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate);

  bool setHeaterIgnitionMinimumFlowRate(const Quantity& heaterIgnitionMinimumFlowRate);

  void resetHeaterIgnitionMinimumFlowRate();

  bool setHeaterIgnitionDelay(double heaterIgnitionDelay);

  bool setHeaterIgnitionDelay(const Quantity& heaterIgnitionDelay);

  void resetHeaterIgnitionDelay();

  bool setHeaterFuelType(std::string heaterFuelType);

  bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

  bool setHeaterThermalEfficiency(const Quantity& heaterThermalEfficiency);

  void resetHeaterThermalEfficiency();

  bool setPartLoadFactorCurve(const CurveCubic& partLoadFactorCurve);

  void resetPartLoadFactorCurve();

  bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

  bool setOffCycleParasiticFuelConsumptionRate(const Quantity& offCycleParasiticFuelConsumptionRate);

  void resetOffCycleParasiticFuelConsumptionRate();

  bool setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType);

  void resetOffCycleParasiticFuelType();

  bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

  bool setOffCycleParasiticHeatFractiontoTank(const Quantity& offCycleParasiticHeatFractiontoTank);

  void resetOffCycleParasiticHeatFractiontoTank();

  bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

  bool setOnCycleParasiticFuelConsumptionRate(const Quantity& onCycleParasiticFuelConsumptionRate);

  void resetOnCycleParasiticFuelConsumptionRate();

  bool setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType);

  void resetOnCycleParasiticFuelType();

  bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

  bool setOnCycleParasiticHeatFractiontoTank(const Quantity& onCycleParasiticHeatFractiontoTank);

  void resetOnCycleParasiticHeatFractiontoTank();

  bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

  bool setAmbientTemperatureSchedule(Schedule& ambientTemperatureSchedule);

  void resetAmbientTemperatureSchedule();

  bool setAmbientTemperatureThermalZone(const ThermalZone& ambientTemperatureThermalZone);

  void resetAmbientTemperatureThermalZone();

  void setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName);

  void resetAmbientTemperatureOutdoorAirNodeName();

  bool setOffCycleLossCoefficienttoAmbientTemperature(double offCycleLossCoefficienttoAmbientTemperature);

  bool setOffCycleLossCoefficienttoAmbientTemperature(const Quantity& offCycleLossCoefficienttoAmbientTemperature);

  void resetOffCycleLossCoefficienttoAmbientTemperature();

  bool setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone);

  bool setOffCycleLossFractiontoThermalZone(const Quantity& offCycleLossFractiontoThermalZone);

  void resetOffCycleLossFractiontoThermalZone();

  bool setOnCycleLossCoefficienttoAmbientTemperature(double onCycleLossCoefficienttoAmbientTemperature);

  bool setOnCycleLossCoefficienttoAmbientTemperature(const Quantity& onCycleLossCoefficienttoAmbientTemperature);

  void resetOnCycleLossCoefficienttoAmbientTemperature();

  bool setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone);

  bool setOnCycleLossFractiontoThermalZone(const Quantity& onCycleLossFractiontoThermalZone);

  void resetOnCycleLossFractiontoThermalZone();

  bool setPeakUseFlowRate(double peakUseFlowRate);

  bool setPeakUseFlowRate(const Quantity& peakUseFlowRate);

  void resetPeakUseFlowRate();

  bool setUseFlowRateFractionSchedule(Schedule& useFlowRateFractionSchedule);

  void resetUseFlowRateFractionSchedule();

  bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

  void resetColdWaterSupplyTemperatureSchedule();

  bool setUseSideEffectiveness(double useSideEffectiveness);

  bool setUseSideEffectiveness(const Quantity& useSideEffectiveness);

  void resetUseSideEffectiveness();

  bool setSourceSideEffectiveness(double sourceSideEffectiveness);

  bool setSourceSideEffectiveness(const Quantity& sourceSideEffectiveness);

  void resetSourceSideEffectiveness();

  bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

  bool setUseSideDesignFlowRate(const Quantity& useSideDesignFlowRate);

  void resetUseSideDesignFlowRate();

  void autosizeUseSideDesignFlowRate();

  bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

  bool setSourceSideDesignFlowRate(const Quantity& sourceSideDesignFlowRate);

  void resetSourceSideDesignFlowRate();

  void autosizeSourceSideDesignFlowRate();

  bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

  bool setIndirectWaterHeatingRecoveryTime(const Quantity& indirectWaterHeatingRecoveryTime);

  void resetIndirectWaterHeatingRecoveryTime();

  //@}
 protected:

  /// @cond
  typedef detail::WaterHeaterMixed_Impl ImplType;

  explicit WaterHeaterMixed(std::shared_ptr<detail::WaterHeaterMixed_Impl> impl);

  friend class detail::WaterHeaterMixed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.WaterHeaterMixed");
};

/** \relates WaterHeaterMixed*/
typedef boost::optional<WaterHeaterMixed> OptionalWaterHeaterMixed;

/** \relates WaterHeaterMixed*/
typedef std::vector<WaterHeaterMixed> WaterHeaterMixedVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERMIXED_HPP

