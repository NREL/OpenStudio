/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {
namespace model {

class Schedule;
//class WaterStorageTank;

namespace detail {

  class EvaporativeFluidCoolerSingleSpeed_Impl;

} // detail

/** EvaporativeFluidCoolerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeFluidCooler:SingleSpeed'. */
class MODEL_API EvaporativeFluidCoolerSingleSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EvaporativeFluidCoolerSingleSpeed(const Model& model);

  virtual ~EvaporativeFluidCoolerSingleSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> performanceInputMethodValues();

  static std::vector<std::string> capacityControlValues();

  static std::vector<std::string> evaporationLossModeValues();

  static std::vector<std::string> blowdownCalculationModeValues();

  /** @name Getters */
  //@{

  boost::optional<double> designAirFlowRate() const;

  bool isDesignAirFlowRateAutosized() const;

  boost::optional<double> fanPoweratDesignAirFlowRate() const;

  bool isFanPoweratDesignAirFlowRateAutosized() const;

  double designSprayWaterFlowRate() const;

  boost::optional<std::string> performanceInputMethod() const;

  boost::optional<double> standardDesignCapacity() const;

  boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate() const;

  bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const;

  boost::optional<double> designWaterFlowRate() const;

  bool isDesignWaterFlowRateAutosized() const;

  boost::optional<double> userSpecifiedDesignCapacity() const;

  boost::optional<double> designEnteringWaterTemperature() const;

  boost::optional<double> designEnteringAirTemperature() const;

  boost::optional<double> designEnteringAirWetbulbTemperature() const;

  std::string capacityControl() const;

  bool isCapacityControlDefaulted() const;

  double sizingFactor() const;

  bool isSizingFactorDefaulted() const;

  std::string evaporationLossMode() const;

  bool isEvaporationLossModeDefaulted() const;

  boost::optional<double> evaporationLossFactor() const;

  double driftLossPercent() const;

  bool isDriftLossPercentDefaulted() const;

  std::string blowdownCalculationMode() const;

  bool isBlowdownCalculationModeDefaulted() const;

  double blowdownConcentrationRatio() const;

  bool isBlowdownConcentrationRatioDefaulted() const;

  boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignAirFlowRate(double designAirFlowRate);

  void autosizeDesignAirFlowRate();

  bool setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate);

  void autosizeFanPoweratDesignAirFlowRate();

  bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  void resetPerformanceInputMethod();

  bool setStandardDesignCapacity(double standardDesignCapacity);

  void resetStandardDesignCapacity();

  bool setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate);

  void resetUfactorTimesAreaValueatDesignAirFlowRate();

  void autosizeUfactorTimesAreaValueatDesignAirFlowRate();

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void resetDesignWaterFlowRate();

  void autosizeDesignWaterFlowRate();

  bool setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity);

  void resetUserSpecifiedDesignCapacity();

  bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

  void resetDesignEnteringWaterTemperature();

  bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

  void resetDesignEnteringAirTemperature();

  bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

  void resetDesignEnteringAirWetbulbTemperature();

  bool setCapacityControl(std::string capacityControl);

  void resetCapacityControl();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  bool setEvaporationLossMode(std::string evaporationLossMode);

  void resetEvaporationLossMode();

  void setEvaporationLossFactor(double evaporationLossFactor);

  void resetEvaporationLossFactor();

  void setDriftLossPercent(double driftLossPercent);

  void resetDriftLossPercent();

  bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

  void resetBlowdownCalculationMode();

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

  void resetBlowdownConcentrationRatio();

  bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

  void resetBlowdownMakeupWaterUsageSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EvaporativeFluidCoolerSingleSpeed_Impl ImplType;

  explicit EvaporativeFluidCoolerSingleSpeed(boost::shared_ptr<detail::EvaporativeFluidCoolerSingleSpeed_Impl> impl);

  friend class detail::EvaporativeFluidCoolerSingleSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerSingleSpeed");
};

/** \relates EvaporativeFluidCoolerSingleSpeed*/
typedef boost::optional<EvaporativeFluidCoolerSingleSpeed> OptionalEvaporativeFluidCoolerSingleSpeed;

/** \relates EvaporativeFluidCoolerSingleSpeed*/
typedef std::vector<EvaporativeFluidCoolerSingleSpeed> EvaporativeFluidCoolerSingleSpeedVector;

} // model
} // openstudio

#endif // MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP

