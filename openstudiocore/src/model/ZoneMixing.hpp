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

#ifndef MODEL_ZONEMIXING_HPP
#define MODEL_ZONEMIXING_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Zone;
class Schedule;
class Zone;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;

namespace detail {

  class ZoneMixing_Impl;

} // detail

/** ZoneMixing is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneMixing'. */
class MODEL_API ZoneMixing : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneMixing(const Model& model);

  virtual ~ZoneMixing() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> designFlowRateCalculationMethodValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Zone.
  Zone zone() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule schedule() const;

  std::string designFlowRateCalculationMethod() const;

  bool isDesignFlowRateCalculationMethodDefaulted() const;

  boost::optional<double> designFlowRate() const;

  boost::optional<double> flowRateperZoneFloorArea() const;

  boost::optional<double> flowRateperPerson() const;

  boost::optional<double> airChangesperHour() const;

  // TODO: Check return type. From object lists, some candidates are: Zone.
  boost::optional<Zone> sourceZone() const;

  double deltaTemperature() const;

  bool isDeltaTemperatureDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> deltaTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> minimumZoneTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> maximumZoneTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> minimumSourceZoneTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> maximumSourceZoneTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Zone.
  bool setZone(const Zone& zone);

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setSchedule(Schedule& schedule);

  bool setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod);

  void resetDesignFlowRateCalculationMethod();

  bool setDesignFlowRate(double designFlowRate);

  void resetDesignFlowRate();

  bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

  void resetFlowRateperZoneFloorArea();

  bool setFlowRateperPerson(double flowRateperPerson);

  void resetFlowRateperPerson();

  bool setAirChangesperHour(double airChangesperHour);

  void resetAirChangesperHour();

  // TODO: Check argument type. From object lists, some candidates are: Zone.
  bool setSourceZone(const Zone& zone);

  void resetSourceZone();

  void setDeltaTemperature(double deltaTemperature);

  void resetDeltaTemperature();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setDeltaTemperatureSchedule(Schedule& schedule);

  void resetDeltaTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMinimumZoneTemperatureSchedule(Schedule& schedule);

  void resetMinimumZoneTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMaximumZoneTemperatureSchedule(Schedule& schedule);

  void resetMaximumZoneTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMinimumSourceZoneTemperatureSchedule(Schedule& schedule);

  void resetMinimumSourceZoneTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMaximumSourceZoneTemperatureSchedule(Schedule& schedule);

  void resetMaximumSourceZoneTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumOutdoorTemperatureSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
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

