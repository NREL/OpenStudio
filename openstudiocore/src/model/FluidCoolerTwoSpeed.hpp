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

#ifndef MODEL_FLUIDCOOLERTWOSPEED_HPP
#define MODEL_FLUIDCOOLERTWOSPEED_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
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

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> waterInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> waterOutletNode() const;

  std::string performanceInputMethod() const;

  boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

  bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

  boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

  bool isLowFanSpeedUfactorTimesAreaValueAutocalculated() const;

  double lowFanSpeedUFactorTimesAreaSizingFactor() const;

  double highSpeedNominalCapacity() const;

  boost::optional<double> lowSpeedNominalCapacity() const;

  bool isLowSpeedNominalCapacityAutocalculated() const;

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

  bool isLowFanSpeedAirFlowRateAutocalculated() const;

  double lowFanSpeedAirFlowRateSizingFactor() const;

  boost::optional<double> lowFanSpeedFanPower() const;

  bool isLowFanSpeedFanPowerAutocalculated() const;

  double lowFanSpeedFanPowerSizingFactor() const;

  // TODO: Check return type. From object lists, some candidates are: Node.
  boost::optional<Node> outdoorAirInletNode() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setWaterInletNode(const Connection& connection);

  void resetWaterInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setWaterOutletNode(const Connection& connection);

  void resetWaterOutletNode();

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  bool setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue);

  void autosizeHighFanSpeedUfactorTimesAreaValue();

  bool setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue);

  void autocalculateLowFanSpeedUfactorTimesAreaValue();

  void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

  bool setHighSpeedNominalCapacity(double highSpeedNominalCapacity);

  bool setLowSpeedNominalCapacity(double lowSpeedNominalCapacity);

  void autocalculateLowSpeedNominalCapacity();

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

  void autocalculateLowFanSpeedAirFlowRate();

  void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

  bool setLowFanSpeedFanPower(double lowFanSpeedFanPower);

  void autocalculateLowFanSpeedFanPower();

  void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

  // TODO: Check argument type. From object lists, some candidates are: Node.
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

