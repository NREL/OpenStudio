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

#ifndef MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP
#define MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveQuadratic;
class ScheduleCompact;
class AirLoopHVACOutdoorAirSystem;
class ControllerMechanicalVentilation;

namespace detail {

class MODEL_API ControllerOutdoorAir_Impl : public ParentObject_Impl {
 public:

  // constructor
  ControllerOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  ControllerOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

  // copy constructor
  ControllerOutdoorAir_Impl(const ControllerOutdoorAir_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

  // virtual destructor
  virtual ~ControllerOutdoorAir_Impl();

  // Get all output variable names that could be associated with this object.
  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  // return the parent object in the hierarchy
  virtual boost::optional<ParentObject> parent() const;

  // return any children objects in the hierarchy
  virtual std::vector<ModelObject> children() const;

  virtual std::vector<openstudio::IdfObject> remove();

  ModelObject clone(Model model) const;

  std::string getEconomizerControlType() const;
  void setEconomizerControlType( const std::string& value );

  std::string getEconomizerControlActionType() const;
  void setEconomizerControlActionType( const std::string& value );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitDryBulbTemperature() const;
  bool setEconomizerMaximumLimitDryBulbTemperature(boost::optional<double> value );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitEnthalpy() const;
  bool setEconomizerMaximumLimitEnthalpy( boost::optional<double> value );

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMaximumLimitDewpointTemperature() const;
  bool setEconomizerMaximumLimitDewpointTemperature(boost::optional<double> value);

  //QuadraticCurve getElectronicEnthalpyLimitCurve() const;
  //void setElectronicEnthalpyLimitCurve(QuadraticCurve c);

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
  bool setEconomizerMinimumLimitDryBulbTemperature(boost::optional<double> value);

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

  CurveQuadratic getElectronicEnthalpyLimitCurve() const;

  ScheduleCompact getMinimumOutdoorAirSchedule() const;

  ScheduleCompact getMinimumFractionOfOutdoorAirSchedule() const;

  ScheduleCompact getMaximumFractionOfOutdoorAirSchedule() const;

  ScheduleCompact getTimeOfDayEconomizerControlSchedule() const;

  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  boost::optional<double> minimumOutdoorAirFlowRate() const;

  OSOptionalQuantity getMinimumOutdoorAirFlowRate(bool returnIP=false) const;

  bool isMinimumOutdoorAirFlowRateAutosized() const;

  void setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate);

  bool setMinimumOutdoorAirFlowRate(const OSOptionalQuantity& minimumOutdoorAirFlowRate);

  void autosizeMinimumOutdoorAirFlowRate();

  boost::optional<double> maximumOutdoorAirFlowRate() const;

  OSOptionalQuantity getMaximumOutdoorAirFlowRate(bool returnIP=false) const;

  bool isMaximumOutdoorAirFlowRateAutosized() const;

  void setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

  bool setMaximumOutdoorAirFlowRate(const OSOptionalQuantity& maximumOutdoorAirFlowRate);

  void autosizeMaximumOutdoorAirFlowRate();

  ControllerMechanicalVentilation controllerMechanicalVentilation() const;

  bool setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation);

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

  std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

 private:

  boost::optional<ControllerMechanicalVentilation> optionalControllerMechanicalVentilation() const;
  boost::optional<ModelObject> controllerMechanicalVentilationAsModelObject() const;
  bool setControllerMechanicalVentilationAsModelObject(const boost::optional<ModelObject>& modelObject);

  REGISTER_LOGGER("openstudio.model.ControllerOutdoorAir");
};

} // detail

} // model

} // openstudio

#endif // MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP

