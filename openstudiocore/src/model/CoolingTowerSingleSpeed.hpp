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

#ifndef MODEL_COOLINGTOWERSINGLESPEED_HPP
#define MODEL_COOLINGTOWERSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;

namespace detail {

class CoolingTowerSingleSpeed_Impl;

} // detail

/** CoolingTowerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 
 *  'OS:CoolingTower:SingleSpeed'. */
class MODEL_API CoolingTowerSingleSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoolingTowerSingleSpeed(const Model& model);

  virtual ~CoolingTowerSingleSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> performanceInputMethodValues();

  static std::vector<std::string> evaporationLossModeValues();

  static std::vector<std::string> blowdownCalculationModeValues();

  static std::vector<std::string> capacityControlValues();

  static std::vector<std::string> cellControlValues();

  /** @name Getters */
  //@{

  boost::optional<double> designWaterFlowRate() const;

  OSOptionalQuantity getDesignWaterFlowRate(bool returnIP=false) const;

  bool isDesignWaterFlowRateAutosized() const;

  boost::optional<double> designAirFlowRate() const;

  OSOptionalQuantity getDesignAirFlowRate(bool returnIP=false) const;

  bool isDesignAirFlowRateAutosized() const;

  boost::optional<double> fanPoweratDesignAirFlowRate() const;

  OSOptionalQuantity getFanPoweratDesignAirFlowRate(bool returnIP=false) const;

  bool isFanPoweratDesignAirFlowRateAutosized() const;

  boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate() const;

  OSOptionalQuantity getUFactorTimesAreaValueatDesignAirFlowRate(bool returnIP=false) const;

  bool isUFactorTimesAreaValueatDesignAirFlowRateAutosized() const;

  boost::optional<double> airFlowRateinFreeConvectionRegime() const;

  OSOptionalQuantity getAirFlowRateinFreeConvectionRegime(bool returnIP=false) const;

  bool isAirFlowRateinFreeConvectionRegimeDefaulted() const;

  bool isAirFlowRateinFreeConvectionRegimeAutosized() const;

  boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate() const;

  OSOptionalQuantity getUFactorTimesAreaValueatFreeConvectionAirFlowRate(bool returnIP=false) const;

  bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted() const;

  bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() const;

  std::string performanceInputMethod() const;

  bool isPerformanceInputMethodDefaulted() const;

  boost::optional<double> nominalCapacity() const;

  OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

  boost::optional<double> freeConvectionCapacity() const;

  OSOptionalQuantity getFreeConvectionCapacity(bool returnIP=false) const;

  double basinHeaterCapacity() const;

  Quantity getBasinHeaterCapacity(bool returnIP=false) const;

  bool isBasinHeaterCapacityDefaulted() const;

  double basinHeaterSetpointTemperature() const;

  Quantity getBasinHeaterSetpointTemperature(bool returnIP=false) const;

  bool isBasinHeaterSetpointTemperatureDefaulted() const;

  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  boost::optional<std::string> evaporationLossMode() const;

  double evaporationLossFactor() const;

  Quantity getEvaporationLossFactor(bool returnIP=false) const;

  bool isEvaporationLossFactorDefaulted() const;

  double driftLossPercent() const;

  Quantity getDriftLossPercent(bool returnIP=false) const;

  bool isDriftLossPercentDefaulted() const;

  boost::optional<std::string> blowdownCalculationMode() const;

  double blowdownConcentrationRatio() const;

  Quantity getBlowdownConcentrationRatio(bool returnIP=false) const;

  bool isBlowdownConcentrationRatioDefaulted() const;

  boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

  std::string capacityControl() const;

  bool isCapacityControlDefaulted() const;

  int numberofCells() const;

  bool isNumberofCellsDefaulted() const;

  std::string cellControl() const;

  bool isCellControlDefaulted() const;

  double cellMinimumWaterFlowRateFraction() const;

  Quantity getCellMinimumWaterFlowRateFraction(bool returnIP=false) const;

  bool isCellMinimumWaterFlowRateFractionDefaulted() const;

  double cellMaximumWaterFlowRateFraction() const;

  Quantity getCellMaximumWaterFlowRateFraction(bool returnIP=false) const;

  bool isCellMaximumWaterFlowRateFractionDefaulted() const;

  double sizingFactor() const;

  Quantity getSizingFactor(bool returnIP=false) const;

  bool isSizingFactorDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  bool setDesignWaterFlowRate(const Quantity& designWaterFlowRate);

  void resetDesignWaterFlowRate();

  void autosizeDesignWaterFlowRate();

  bool setDesignAirFlowRate(double designAirFlowRate);

  bool setDesignAirFlowRate(const Quantity& designAirFlowRate);

  void autosizeDesignAirFlowRate();

  bool setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate);

  bool setFanPoweratDesignAirFlowRate(const Quantity& fanPoweratDesignAirFlowRate);

  void autosizeFanPoweratDesignAirFlowRate();

  bool setUFactorTimesAreaValueatDesignAirFlowRate(double uFactorTimesAreaValueatDesignAirFlowRate);

  bool setUFactorTimesAreaValueatDesignAirFlowRate(const Quantity& uFactorTimesAreaValueatDesignAirFlowRate);

  void resetUFactorTimesAreaValueatDesignAirFlowRate();

  void autosizeUFactorTimesAreaValueatDesignAirFlowRate();

  bool setAirFlowRateinFreeConvectionRegime(double airFlowRateinFreeConvectionRegime);

  bool setAirFlowRateinFreeConvectionRegime(const Quantity& airFlowRateinFreeConvectionRegime);

  void resetAirFlowRateinFreeConvectionRegime();

  void autosizeAirFlowRateinFreeConvectionRegime();

  bool setUFactorTimesAreaValueatFreeConvectionAirFlowRate(double uFactorTimesAreaValueatFreeConvectionAirFlowRate);

  bool setUFactorTimesAreaValueatFreeConvectionAirFlowRate(const Quantity& uFactorTimesAreaValueatFreeConvectionAirFlowRate);

  void resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();

  void autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  void resetPerformanceInputMethod();

  bool setNominalCapacity(double nominalCapacity);

  bool setNominalCapacity(const Quantity& nominalCapacity);

  void resetNominalCapacity();

  bool setFreeConvectionCapacity(double freeConvectionCapacity);

  bool setFreeConvectionCapacity(const Quantity& freeConvectionCapacity);

  void resetFreeConvectionCapacity();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  bool setBasinHeaterCapacity(const Quantity& basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  bool setBasinHeaterSetpointTemperature(const Quantity& basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  bool setBasinHeaterOperatingSchedule(Schedule& basinHeaterOperatingSchedule);

  void resetBasinHeaterOperatingSchedule();

  bool setEvaporationLossMode(std::string evaporationLossMode);

  void resetEvaporationLossMode();

  void setEvaporationLossFactor(double evaporationLossFactor);

  bool setEvaporationLossFactor(const Quantity& evaporationLossFactor);

  void resetEvaporationLossFactor();

  void setDriftLossPercent(double driftLossPercent);

  bool setDriftLossPercent(const Quantity& driftLossPercent);

  void resetDriftLossPercent();

  bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

  void resetBlowdownCalculationMode();

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

  bool setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio);

  void resetBlowdownConcentrationRatio();

  bool setBlowdownMakeupWaterUsageSchedule(Schedule& blowdownMakeupWaterUsageSchedule);

  void resetBlowdownMakeupWaterUsageSchedule();

  bool setCapacityControl(std::string capacityControl);

  void resetCapacityControl();

  bool setNumberofCells(int numberofCells);

  void resetNumberofCells();

  bool setCellControl(std::string cellControl);

  void resetCellControl();

  bool setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction);

  bool setCellMinimumWaterFlowRateFraction(const Quantity& cellMinimumWaterFlowRateFraction);

  void resetCellMinimumWaterFlowRateFraction();

  bool setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction);

  bool setCellMaximumWaterFlowRateFraction(const Quantity& cellMaximumWaterFlowRateFraction);

  void resetCellMaximumWaterFlowRateFraction();

  bool setSizingFactor(double sizingFactor);

  bool setSizingFactor(const Quantity& sizingFactor);

  void resetSizingFactor();

  //@}
 protected:

  /// @cond
  typedef detail::CoolingTowerSingleSpeed_Impl ImplType;

  explicit CoolingTowerSingleSpeed(std::shared_ptr<detail::CoolingTowerSingleSpeed_Impl> impl);

  friend class detail::CoolingTowerSingleSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.CoolingTowerSingleSpeed");
};

/** \relates CoolingTowerSingleSpeed*/
typedef boost::optional<CoolingTowerSingleSpeed> OptionalCoolingTowerSingleSpeed;

/** \relates CoolingTowerSingleSpeed*/
typedef std::vector<CoolingTowerSingleSpeed> CoolingTowerSingleSpeedVector;

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERSINGLESPEED_HPP

