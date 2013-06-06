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

// class Quantity;
// class OSOptionalQuantity;

namespace model {

// TODO: Check the following class names against object getters and setters.
//class Connection;
//class Connection;
//class Connection;
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

  // TODO: Check return type. From object lists, some candidates are: Connection.
  //Connection waterInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  //Connection waterOutletNode() const;

  boost::optional<double> designAirFlowRate() const;

  // OSOptionalQuantity getDesignAirFlowRate(bool returnIP=false) const;

  bool isDesignAirFlowRateAutosized() const;

  boost::optional<double> fanPoweratDesignAirFlowRate() const;

 // OSOptionalQuantity getFanPoweratDesignAirFlowRate(bool returnIP=false) const;

  bool isFanPoweratDesignAirFlowRateAutosized() const;

  double designSprayWaterFlowRate() const;

  // Quantity getDesignSprayWaterFlowRate(bool returnIP=false) const;

  boost::optional<std::string> performanceInputMethod() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  //boost::optional<Connection> outdoorAirInletNode() const;

  boost::optional<double> standardDesignCapacity() const;

 // OSOptionalQuantity getStandardDesignCapacity(bool returnIP=false) const;

  boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate() const;

 // OSOptionalQuantity getUfactorTimesAreaValueatDesignAirFlowRate(bool returnIP=false) const;

  bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const;

  boost::optional<double> designWaterFlowRate() const;

 // OSOptionalQuantity getDesignWaterFlowRate(bool returnIP=false) const;

  bool isDesignWaterFlowRateAutosized() const;

  boost::optional<double> userSpecifiedDesignCapacity() const;

 // OSOptionalQuantity getUserSpecifiedDesignCapacity(bool returnIP=false) const;

  boost::optional<double> designEnteringWaterTemperature() const;

  // OSOptionalQuantity getDesignEnteringWaterTemperature(bool returnIP=false) const;

  boost::optional<double> designEnteringAirTemperature() const;

 // OSOptionalQuantity getDesignEnteringAirTemperature(bool returnIP=false) const;

  boost::optional<double> designEnteringAirWetbulbTemperature() const;

 // OSOptionalQuantity getDesignEnteringAirWetbulbTemperature(bool returnIP=false) const;

  std::string capacityControl() const;

  bool isCapacityControlDefaulted() const;

  double sizingFactor() const;

  // Quantity getSizingFactor(bool returnIP=false) const;

  bool isSizingFactorDefaulted() const;

  std::string evaporationLossMode() const;

  bool isEvaporationLossModeDefaulted() const;

  boost::optional<double> evaporationLossFactor() const;

 // OSOptionalQuantity getEvaporationLossFactor(bool returnIP=false) const;

  double driftLossPercent() const;

 // Quantity getDriftLossPercent(bool returnIP=false) const;

  bool isDriftLossPercentDefaulted() const;

  std::string blowdownCalculationMode() const;

  bool isBlowdownCalculationModeDefaulted() const;

  double blowdownConcentrationRatio() const;

  // Quantity getBlowdownConcentrationRatio(bool returnIP=false) const;

  bool isBlowdownConcentrationRatioDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
  //boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setWaterInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setWaterOutletNode(const Connection& connection);

  bool setDesignAirFlowRate(double designAirFlowRate);

 // bool setDesignAirFlowRate(const Quantity& designAirFlowRate);

  void autosizeDesignAirFlowRate();

  bool setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate);

 // bool setFanPoweratDesignAirFlowRate(const Quantity& fanPoweratDesignAirFlowRate);

  void autosizeFanPoweratDesignAirFlowRate();

  bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

 // bool setDesignSprayWaterFlowRate(const Quantity& designSprayWaterFlowRate);

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  void resetPerformanceInputMethod();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setOutdoorAirInletNode(const Connection& connection);

  void resetOutdoorAirInletNode();

  bool setStandardDesignCapacity(double standardDesignCapacity);

 // bool setStandardDesignCapacity(const Quantity& standardDesignCapacity);

  void resetStandardDesignCapacity();

  bool setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate);

 // bool setUfactorTimesAreaValueatDesignAirFlowRate(const Quantity& ufactorTimesAreaValueatDesignAirFlowRate);

  void resetUfactorTimesAreaValueatDesignAirFlowRate();

  void autosizeUfactorTimesAreaValueatDesignAirFlowRate();

  bool setDesignWaterFlowRate(double designWaterFlowRate);

 // bool setDesignWaterFlowRate(const Quantity& designWaterFlowRate);

  void resetDesignWaterFlowRate();

  void autosizeDesignWaterFlowRate();

  bool setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity);

 // bool setUserSpecifiedDesignCapacity(const Quantity& userSpecifiedDesignCapacity);

  void resetUserSpecifiedDesignCapacity();

  bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

 // bool setDesignEnteringWaterTemperature(const Quantity& designEnteringWaterTemperature);

  void resetDesignEnteringWaterTemperature();

  bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

//  bool setDesignEnteringAirTemperature(const Quantity& designEnteringAirTemperature);

  void resetDesignEnteringAirTemperature();

  bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

 // bool setDesignEnteringAirWetbulbTemperature(const Quantity& designEnteringAirWetbulbTemperature);

  void resetDesignEnteringAirWetbulbTemperature();

  bool setCapacityControl(std::string capacityControl);

  void resetCapacityControl();

  bool setSizingFactor(double sizingFactor);

 // bool setSizingFactor(const Quantity& sizingFactor);

  void resetSizingFactor();

  bool setEvaporationLossMode(std::string evaporationLossMode);

  void resetEvaporationLossMode();

  void setEvaporationLossFactor(double evaporationLossFactor);

 // bool setEvaporationLossFactor(const Quantity& evaporationLossFactor);

  void resetEvaporationLossFactor();

  void setDriftLossPercent(double driftLossPercent);

  // bool setDriftLossPercent(const Quantity& driftLossPercent);

  void resetDriftLossPercent();

  bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

  void resetBlowdownCalculationMode();

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

//  bool setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio);

  void resetBlowdownConcentrationRatio();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

  void resetBlowdownMakeupWaterUsageSchedule();

  // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
  //bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

 // void resetSupplyWaterStorageTank();

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

