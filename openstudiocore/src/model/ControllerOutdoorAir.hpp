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

#ifndef MODEL_CONTROLLEROUTDOORAIR_HPP
#define MODEL_CONTROLLEROUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class ControllerOutdoorAir_Impl;

} // detail

class CurveQuadratic;
class ScheduleCompact;
class ControllerMechanicalVentilation;
class AirLoopHVACOutdoorAirSystem;

class MODEL_API ControllerOutdoorAir : public ParentObject {
 public:

  explicit ControllerOutdoorAir(const Model& model);

  virtual ~ControllerOutdoorAir() {}

  static IddObjectType iddObjectType();

  boost::optional<Schedule> minimumOutdoorAirSchedule() const;
  bool setMinimumOutdoorAirSchedule(Schedule& schedule);
  void resetMinimumOutdoorAirSchedule();

  boost::optional<Schedule> minimumFractionofOutdoorAirSchedule() const;
  bool setMinimumFractionofOutdoorAirSchedule(Schedule& schedule);
  void resetMinimumFractionofOutdoorAirSchedule();
  
  boost::optional<Schedule> maximumFractionofOutdoorAirSchedule() const;
  bool setMaximumFractionofOutdoorAirSchedule(Schedule& schedule);
  void resetMaximumFractionofOutdoorAirSchedule();
  
  boost::optional<Schedule> timeofDayEconomizerControlSchedule() const;
  bool setTimeofDayEconomizerControlSchedule(Schedule& schedule);
  void resetTimeofDayEconomizerControlSchedule();

  boost::optional<double> minimumOutdoorAirFlowRate() const;
  OSOptionalQuantity getMinimumOutdoorAirFlowRate(bool returnIP=false) const;
  bool isMinimumOutdoorAirFlowRateAutosized() const;
  void setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate);
  bool setMinimumOutdoorAirFlowRate(const Quantity& minimumOutdoorAirFlowRate);
  void autosizeMinimumOutdoorAirFlowRate();

  boost::optional<double> maximumOutdoorAirFlowRate() const;
  OSOptionalQuantity getMaximumOutdoorAirFlowRate(bool returnIP=false) const;
  bool isMaximumOutdoorAirFlowRateAutosized() const;
  void setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);
  bool setMaximumOutdoorAirFlowRate(const Quantity& maximumOutdoorAirFlowRate);
  void autosizeMaximumOutdoorAirFlowRate();

  ControllerMechanicalVentilation controllerMechanicalVentilation() const;
  bool setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation);

  std::string getEconomizerControlType() const;
  void setEconomizerControlType( const std::string& value );

  std::string getEconomizerControlActionType() const;
  void setEconomizerControlActionType( const std::string& value );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitDryBulbTemperature() const;
  bool setEconomizerMaximumLimitDryBulbTemperature( double value );
  void resetEconomizerMaximumLimitDryBulbTemperature( );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitEnthalpy() const;
  bool setEconomizerMaximumLimitEnthalpy( double value );
  void resetEconomizerMaximumLimitEnthalpy( );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitDewpointTemperature() const;
  bool setEconomizerMaximumLimitDewpointTemperature( double value );
  void resetEconomizerMaximumLimitDewpointTemperature( );

  //QuadraticCurve getElectronicEnthalpyLimitCurve() const;
  //void setElectronicEnthalpyLimitCurve(QuadraticCurve c);

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
  bool setEconomizerMinimumLimitDryBulbTemperature( double value );
  void resetEconomizerMinimumLimitDryBulbTemperature( );

  std::string getLockoutType()const;
  void setLockoutType( const std::string& value );

  std::string getMinimumLimitType()const;
  void setMinimumLimitType( const std::string& value );

  boost::optional<bool> getHighHumidityControl() const;
  void setHighHumidityControl(bool val);

  //Zone getHumidistatControlZone() const;
  //void setHumidistatControlZone(Zone z)

  OptionalDouble getHighHumidityOutdoorAirFlowRatio() const;
  void setHighHumidityOutdoorAirFlowRatio(double v);

  boost::optional<bool> getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const;
  void setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v);

  OptionalString getHeatRecoveryBypassControlType() const;
  void setHeatRecoveryBypassControlType(const std::string& v);

  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  virtual std::vector<openstudio::IdfObject> remove();

 protected:

  typedef detail::ControllerOutdoorAir_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit ControllerOutdoorAir(std::shared_ptr<detail::ControllerOutdoorAir_Impl> impl);

 private:

  CurveQuadratic getElectronicEnthalpyLimitCurve() const;

  ScheduleCompact getMinimumOutdoorAirSchedule() const;

  ScheduleCompact getMinimumFractionOfOutdoorAirSchedule() const;

  ScheduleCompact getMaximumFractionOfOutdoorAirSchedule() const;

  //Controller:MechanicalVentilation getMechanicalVentilationController() const;

  ScheduleCompact getTimeOfDayEconomizerControlSchedule() const;

  ControllerOutdoorAir(const Handle& handle, const Model& model);

  REGISTER_LOGGER("openstudio.model.ControllerOutdoorAir");
};

typedef boost::optional<ControllerOutdoorAir> OptionalControllerOutdoorAir;

typedef std::vector<ControllerOutdoorAir> ControllerOutdoorAirVector;

} // model

} // openstudio

#endif // MODEL_CONTROLLEROUTDOORAIR_HPP

