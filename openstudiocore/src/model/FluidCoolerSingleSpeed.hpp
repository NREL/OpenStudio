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

#ifndef MODEL_FLUIDCOOLERSINGLESPEED_HPP
#define MODEL_FLUIDCOOLERSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class FluidCoolerSingleSpeed_Impl;

} // detail

/** FluidCoolerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:FluidCooler:SingleSpeed'. */
class MODEL_API FluidCoolerSingleSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FluidCoolerSingleSpeed(const Model& model);

  virtual ~FluidCoolerSingleSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> performanceInputMethodValues();

  /** @name Getters */
  //@{

  std::string performanceInputMethod() const;

  boost::optional<double> designAirFlowRateUfactorTimesAreaValue() const;

  bool isDesignAirFlowRateUfactorTimesAreaValueAutosized() const;

  double nominalCapacity() const;

  double designEnteringWaterTemperature() const;

  double designEnteringAirTemperature() const;

  double designEnteringAirWetbulbTemperature() const;

  boost::optional<double> designWaterFlowRate() const;

  bool isDesignWaterFlowRateAutosized() const;

  boost::optional<double> designAirFlowRate() const;

  bool isDesignAirFlowRateAutosized() const;

  boost::optional<double> designAirFlowRateFanPower() const;

  bool isDesignAirFlowRateFanPowerAutosized() const;

  boost::optional<Node> outdoorAirInletNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  bool setDesignAirFlowRateUfactorTimesAreaValue(double designAirFlowRateUfactorTimesAreaValue);

  void autosizeDesignAirFlowRateUfactorTimesAreaValue();

  bool setNominalCapacity(double nominalCapacity);

  bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

  bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

  bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void autosizeDesignWaterFlowRate();

  bool setDesignAirFlowRate(double designAirFlowRate);

  void autosizeDesignAirFlowRate();

  bool setDesignAirFlowRateFanPower(double designAirFlowRateFanPower);

  void autosizeDesignAirFlowRateFanPower();

  bool setOutdoorAirInletNode(const Node& node);

  void resetOutdoorAirInletNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FluidCoolerSingleSpeed_Impl ImplType;

  explicit FluidCoolerSingleSpeed(std::shared_ptr<detail::FluidCoolerSingleSpeed_Impl> impl);

  friend class detail::FluidCoolerSingleSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FluidCoolerSingleSpeed");
};

/** \relates FluidCoolerSingleSpeed*/
typedef boost::optional<FluidCoolerSingleSpeed> OptionalFluidCoolerSingleSpeed;

/** \relates FluidCoolerSingleSpeed*/
typedef std::vector<FluidCoolerSingleSpeed> FluidCoolerSingleSpeedVector;

} // model
} // openstudio

#endif // MODEL_FLUIDCOOLERSINGLESPEED_HPP

