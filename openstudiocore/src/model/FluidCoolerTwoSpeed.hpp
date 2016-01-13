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

#ifndef MODEL_FLUIDCOOLERTWOSPEED_HPP
#define MODEL_FLUIDCOOLERTWOSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class FluidCoolerTwoSpeed_Impl;

} // detail

/** FluidCoolerTwoSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:FluidCooler:TwoSpeed'. */
class MODEL_API FluidCoolerTwoSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FluidCoolerTwoSpeed(const Model& model);

  virtual ~FluidCoolerTwoSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> performanceInputMethodValues();

  /** @name Getters */
  //@{

  std::string performanceInputMethod() const;

  boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

  bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

  boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

  bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

  double lowFanSpeedUFactorTimesAreaSizingFactor() const;

  double highSpeedNominalCapacity() const;

  boost::optional<double> lowSpeedNominalCapacity() const;

  bool isLowSpeedNominalCapacityAutosized() const;

  double lowSpeedNominalCapacitySizingFactor() const;

  double designEnteringWaterTemperature() const;

  double designEnteringAirTemperature() const;

  double designEnteringAirWetbulbTemperature() const;

  boost::optional<double> designWaterFlowRate() const;

  bool isDesignWaterFlowRateAutosized() const;

  boost::optional<double> highFanSpeedAirFlowRate() const;

  bool isHighFanSpeedAirFlowRateAutosized() const;

  boost::optional<double> highFanSpeedFanPower() const;

  bool isHighFanSpeedFanPowerAutosized() const;

  boost::optional<double> lowFanSpeedAirFlowRate() const;

  bool isLowFanSpeedAirFlowRateAutosized() const;

  double lowFanSpeedAirFlowRateSizingFactor() const;

  boost::optional<double> lowFanSpeedFanPower() const;

  bool isLowFanSpeedFanPowerAutosized() const;

  double lowFanSpeedFanPowerSizingFactor() const;

  boost::optional<Node> outdoorAirInletNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  bool setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue);

  void autosizeHighFanSpeedUfactorTimesAreaValue();

  bool setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue);

  void autosizeLowFanSpeedUfactorTimesAreaValue();

  void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

  bool setHighSpeedNominalCapacity(double highSpeedNominalCapacity);

  bool setLowSpeedNominalCapacity(double lowSpeedNominalCapacity);

  void autosizeLowSpeedNominalCapacity();

  void setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

  bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

  bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

  bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void autosizeDesignWaterFlowRate();

  bool setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate);

  void autosizeHighFanSpeedAirFlowRate();

  bool setHighFanSpeedFanPower(double highFanSpeedFanPower);

  void autosizeHighFanSpeedFanPower();

  bool setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate);

  void autosizeLowFanSpeedAirFlowRate();

  void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

  bool setLowFanSpeedFanPower(double lowFanSpeedFanPower);

  void autosizeLowFanSpeedFanPower();

  void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

  bool setOutdoorAirInletNode(const Node& node);

  void resetOutdoorAirInletNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FluidCoolerTwoSpeed_Impl ImplType;

  explicit FluidCoolerTwoSpeed(std::shared_ptr<detail::FluidCoolerTwoSpeed_Impl> impl);

  friend class detail::FluidCoolerTwoSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FluidCoolerTwoSpeed");
};

/** \relates FluidCoolerTwoSpeed*/
typedef boost::optional<FluidCoolerTwoSpeed> OptionalFluidCoolerTwoSpeed;

/** \relates FluidCoolerTwoSpeed*/
typedef std::vector<FluidCoolerTwoSpeed> FluidCoolerTwoSpeedVector;

} // model
} // openstudio

#endif // MODEL_FLUIDCOOLERTWOSPEED_HPP
