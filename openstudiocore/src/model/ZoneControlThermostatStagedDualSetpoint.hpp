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

#ifndef MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP
#define MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP

#include "ModelAPI.hpp"
#include "Thermostat.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class ZoneControlThermostatStagedDualSetpoint_Impl;

} // detail

/** ZoneControlThermostatStagedDualSetpoint is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:Thermostat:StagedDualSetpoint'. */
class MODEL_API ZoneControlThermostatStagedDualSetpoint : public Thermostat {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneControlThermostatStagedDualSetpoint(const Model& model);

  virtual ~ZoneControlThermostatStagedDualSetpoint() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  int numberofHeatingStages() const;

  boost::optional<Schedule> heatingTemperatureSetpointSchedule() const;

  double heatingThrottlingTemperatureRange() const;

  double stage1HeatingTemperatureOffset() const;

  double stage2HeatingTemperatureOffset() const;

  double stage3HeatingTemperatureOffset() const;

  double stage4HeatingTemperatureOffset() const;

  int numberofCoolingStages() const;

  boost::optional<Schedule> coolingTemperatureSetpointBaseSchedule() const;

  double coolingThrottlingTemperatureRange() const;

  double stage1CoolingTemperatureOffset() const;

  double stage2CoolingTemperatureOffset() const;

  double stage3CoolingTemperatureOffset() const;

  double stage4CoolingTemperatureOffset() const;

  //@}
  /** @name Setters */
  //@{

  bool setNumberofHeatingStages(int numberofHeatingStages);

  bool setHeatingTemperatureSetpointSchedule(Schedule& schedule);

  void resetHeatingTemperatureSetpointSchedule();

  bool setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange);

  bool setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset);

  bool setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset);

  bool setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset);

  bool setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset);

  bool setNumberofCoolingStages(int numberofCoolingStages);

  bool setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule);

  void resetCoolingTemperatureSetpointBaseSchedule();

  bool setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange);

  bool setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset);

  bool setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset);

  bool setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset);

  bool setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneControlThermostatStagedDualSetpoint_Impl ImplType;

  explicit ZoneControlThermostatStagedDualSetpoint(std::shared_ptr<detail::ZoneControlThermostatStagedDualSetpoint_Impl> impl);

  friend class detail::ZoneControlThermostatStagedDualSetpoint_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneControlThermostatStagedDualSetpoint");
};

/** \relates ZoneControlThermostatStagedDualSetpoint*/
typedef boost::optional<ZoneControlThermostatStagedDualSetpoint> OptionalZoneControlThermostatStagedDualSetpoint;

/** \relates ZoneControlThermostatStagedDualSetpoint*/
typedef std::vector<ZoneControlThermostatStagedDualSetpoint> ZoneControlThermostatStagedDualSetpointVector;

} // model
} // openstudio

#endif // MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP

