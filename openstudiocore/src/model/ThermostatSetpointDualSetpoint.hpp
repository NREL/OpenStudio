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
#ifndef MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
#define MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP

#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail
{
  class ThermostatSetpointDualSetpoint_Impl;
}

class Schedule;

/** ThermostatSetpointDualSetpoint is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:ThermostatSetpoint:DualSetpoint'. */
class MODEL_API ThermostatSetpointDualSetpoint : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermostatSetpointDualSetpoint(const Model& model);

  virtual ~ThermostatSetpointDualSetpoint() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

  boost::optional<Schedule> coolingSetpointTemperatureSchedule() const;

  /** \deprecated */
  boost::optional<Schedule> getHeatingSchedule() const;

  /** \deprecated */
  boost::optional<Schedule> getCoolingSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

  void resetHeatingSetpointTemperatureSchedule();

  bool setCoolingSetpointTemperatureSchedule(Schedule& schedule);

  void resetCoolingSetpointTemperatureSchedule();

  /** \deprecated */
  bool setHeatingSchedule(Schedule& s );

  /** \deprecated */
  void resetHeatingSchedule();

  /** \deprecated */
  bool setCoolingSchedule(Schedule& s );

  /** \deprecated */
  void resetCoolingSchedule();

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  typedef detail::ThermostatSetpointDualSetpoint_Impl ImplType;

  explicit ThermostatSetpointDualSetpoint(std::shared_ptr<detail::ThermostatSetpointDualSetpoint_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ThermostatSetpointDualSetpoint");

};

typedef boost::optional<ThermostatSetpointDualSetpoint> OptionalThermostatSetpointDualSetpoint;

} // model
} // openstudio

#endif // MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
