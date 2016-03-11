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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;
class Schedule;

namespace detail {

  class ZoneHVACEnergyRecoveryVentilatorController_Impl;

} // detail

/** ZoneHVACEnergyRecoveryVentilatorController is a ParentObject that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EnergyRecoveryVentilator:Controller'. */
class MODEL_API ZoneHVACEnergyRecoveryVentilatorController : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACEnergyRecoveryVentilatorController(const Model& model);

  virtual ~ZoneHVACEnergyRecoveryVentilatorController() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> exhaustAirTemperatureLimitValues();

  static std::vector<std::string> exhaustAirEnthalpyLimitValues();

  /** @name Getters */
  //@{

  boost::optional<double> temperatureHighLimit() const;

  boost::optional<double> temperatureLowLimit() const;

  boost::optional<double> enthalpyHighLimit() const;

  boost::optional<double> dewpointTemperatureLimit() const;

  boost::optional<Curve> electronicEnthalpyLimitCurve() const;

  std::string exhaustAirTemperatureLimit() const;

  std::string exhaustAirEnthalpyLimit() const;

  boost::optional<Schedule> timeofDayEconomizerFlowControlSchedule() const;

  bool highHumidityControlFlag() const;

  double highHumidityOutdoorAirFlowRatio() const;

  bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio() const;

  //@}
  /** @name Setters */
  //@{

  void setTemperatureHighLimit(double temperatureHighLimit);

  void resetTemperatureHighLimit();

  void setTemperatureLowLimit(double temperatureLowLimit);

  void resetTemperatureLowLimit();

  void setEnthalpyHighLimit(double enthalpyHighLimit);

  void resetEnthalpyHighLimit();

  void setDewpointTemperatureLimit(double dewpointTemperatureLimit);

  void resetDewpointTemperatureLimit();

  bool setElectronicEnthalpyLimitCurve(const Curve& curve);

  void resetElectronicEnthalpyLimitCurve();

  bool setExhaustAirTemperatureLimit(std::string exhaustAirTemperatureLimit);

  bool setExhaustAirEnthalpyLimit(std::string exhaustAirEnthalpyLimit);

  bool setTimeofDayEconomizerFlowControlSchedule(Schedule& schedule);

  void resetTimeofDayEconomizerFlowControlSchedule();

  void setHighHumidityControlFlag(bool highHumidityControlFlag);

  bool setHighHumidityOutdoorAirFlowRatio(double highHumidityOutdoorAirFlowRatio);

  void setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACEnergyRecoveryVentilatorController_Impl ImplType;

  explicit ZoneHVACEnergyRecoveryVentilatorController(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl> impl);

  friend class detail::ZoneHVACEnergyRecoveryVentilatorController_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilatorController");
};

/** \relates ZoneHVACEnergyRecoveryVentilatorController*/
typedef boost::optional<ZoneHVACEnergyRecoveryVentilatorController> OptionalZoneHVACEnergyRecoveryVentilatorController;

/** \relates ZoneHVACEnergyRecoveryVentilatorController*/
typedef std::vector<ZoneHVACEnergyRecoveryVentilatorController> ZoneHVACEnergyRecoveryVentilatorControllerVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP

