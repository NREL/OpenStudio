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

#ifndef MODEL_ZONEMIXING_HPP
#define MODEL_ZONEMIXING_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class Schedule;

namespace detail {

  class ZoneMixing_Impl;

} // detail

/** ZoneMixing is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneMixing'. */
class MODEL_API ZoneMixing : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneMixing(const ThermalZone& thermalZone);

  virtual ~ZoneMixing() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /// Returns the ThermalZone which receives air from this mixing object.
  ThermalZone zone() const;

  /// Returns fractional schedule which moderates amount of air flow from the design level.
  Schedule schedule() const;

  std::string designFlowRateCalculationMethod() const;

  boost::optional<double> designFlowRate() const;

  boost::optional<double> flowRateperZoneFloorArea() const;

  boost::optional<double> flowRateperPerson() const;

  boost::optional<double> airChangesperHour() const;
  
  /// Returns the ThermalZone which supplies air to this mixing object.
  boost::optional<ThermalZone> sourceZone() const;

  /// Returns the constant temperature differential between source and receiving zones below which mixing is shutoff.
  // DLM: is this a signed difference or absolute?
  boost::optional<double> deltaTemperature() const;

  /// Returns the temperature schedule containing the differential between source and receiving zones versus time below which mixing is shutoff.
  boost::optional<Schedule> deltaTemperatureSchedule() const;

  /// Returns the temperature schedule containing the receiving zone dry-bulb temperature below which mixing is shutoff.
  boost::optional<Schedule> minimumZoneTemperatureSchedule() const;

  /// Returns the temperature schedule containing the receiving zone dry-bulb temperature above which mixing is shutoff.
  boost::optional<Schedule> maximumZoneTemperatureSchedule() const;

  /// Returns the temperature schedule containing the source zone dry-bulb temperature below which mixing is shutoff.
  boost::optional<Schedule> minimumSourceZoneTemperatureSchedule() const;

  /// Returns the temperature schedule containing the source zone dry-bulb temperature above which mixing is shutoff.
  boost::optional<Schedule> maximumSourceZoneTemperatureSchedule() const;

  /// Returns the temperature schedule containing the outdoor temperature below which mixing is shutoff.
  boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

  /// Returns the temperature schedule containing the outdoor temperature above which mixing is shutoff.
  boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets the fractional schedule which moderates amount of air flow from the design level.
  bool setSchedule(Schedule& schedule);

  bool setDesignFlowRate(double designFlowRate);

  bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

  bool setFlowRateperPerson(double flowRateperPerson);

  bool setAirChangesperHour(double airChangesperHour);

  /// Sets the ThermalZone which supplies air to this mixing object.
  bool setSourceZone(const ThermalZone& zone);

  void resetSourceZone();

  /// Sets the constant temperature differential between source and receiving zones below which mixing is shutoff.
  void setDeltaTemperature(double deltaTemperature);

  void resetDeltaTemperature();

  /// Sets the temperature schedule containing the differential between source and receiving zones versus time below which mixing is shutoff.
  bool setDeltaTemperatureSchedule(Schedule& schedule);

  void resetDeltaTemperatureSchedule();

  /// Sets the temperature schedule containing the receiving zone dry-bulb temperature below which mixing is shutoff.
  bool setMinimumZoneTemperatureSchedule(Schedule& schedule);

  void resetMinimumZoneTemperatureSchedule();

  /// Sets the temperature schedule containing the receiving zone dry-bulb temperature above which mixing is shutoff.
  bool setMaximumZoneTemperatureSchedule(Schedule& schedule);

  void resetMaximumZoneTemperatureSchedule();

  /// Sets the temperature schedule containing the source zone dry-bulb temperature below which mixing is shutoff.
  bool setMinimumSourceZoneTemperatureSchedule(Schedule& schedule);

  void resetMinimumSourceZoneTemperatureSchedule();

  /// Sets the temperature schedule containing the source zone dry-bulb temperature above which mixing is shutoff.
  bool setMaximumSourceZoneTemperatureSchedule(Schedule& schedule);

  void resetMaximumSourceZoneTemperatureSchedule();

  /// Returns the temperature schedule containing the outdoor temperature below which mixing is shutoff.
  bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumOutdoorTemperatureSchedule();

  /// Returns the temperature schedule containing the outdoor temperature above which mixing is shutoff.
  bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMaximumOutdoorTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}

 protected:
  /// @cond
  typedef detail::ZoneMixing_Impl ImplType;

  explicit ZoneMixing(std::shared_ptr<detail::ZoneMixing_Impl> impl);

  friend class detail::ZoneMixing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneMixing");
};

/** \relates ZoneMixing*/
typedef boost::optional<ZoneMixing> OptionalZoneMixing;

/** \relates ZoneMixing*/
typedef std::vector<ZoneMixing> ZoneMixingVector;

} // model
} // openstudio

#endif // MODEL_ZONEMIXING_HPP

