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

#ifndef MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP
#define MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class ZoneVentilationDesignFlowRate_Impl;

} // detail

/** ZoneVentilationDesignFlowRate is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneVentilation:DesignFlowRate'. */
class MODEL_API ZoneVentilationDesignFlowRate : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneVentilationDesignFlowRate(const Model& model);

  virtual ~ZoneVentilationDesignFlowRate() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> designFlowRateCalculationMethodValues();

  static std::vector<std::string> ventilationTypeValues();

  /** @name Getters */
  //@{

  Schedule schedule() const;

  std::string designFlowRateCalculationMethod() const;

  double designFlowRate() const;

  double flowRateperZoneFloorArea() const;

  double flowRateperPerson() const;

  double airChangesperHour() const;

  std::string ventilationType() const;

  double fanPressureRise() const;

  double fanTotalEfficiency() const;

  double constantTermCoefficient() const;

  double temperatureTermCoefficient() const;

  double velocityTermCoefficient() const;

  double velocitySquaredTermCoefficient() const;

  double minimumIndoorTemperature() const;

  boost::optional<Schedule> minimumIndoorTemperatureSchedule() const;

  double maximumIndoorTemperature() const;

  boost::optional<Schedule> maximumIndoorTemperatureSchedule() const;

  double deltaTemperature() const;

  boost::optional<Schedule> deltaTemperatureSchedule() const;

  double minimumOutdoorTemperature() const;

  boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

  double maximumOutdoorTemperature() const;

  boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

  double maximumWindSpeed() const;

  //@}
  /** @name Setters */
  //@{

  bool setSchedule(Schedule& schedule);

  bool setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod);

  bool setDesignFlowRate(double designFlowRate);

  bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

  bool setFlowRateperPerson(double flowRateperPerson);

  bool setAirChangesperHour(double airChangesperHour);

  bool setVentilationType(std::string ventilationType);

  bool setFanPressureRise(double fanPressureRise);

  bool setFanTotalEfficiency(double fanTotalEfficiency);

  void setConstantTermCoefficient(double constantTermCoefficient);

  void setTemperatureTermCoefficient(double temperatureTermCoefficient);

  void setVelocityTermCoefficient(double velocityTermCoefficient);

  void setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

  bool setMinimumIndoorTemperature(double minimumIndoorTemperature);

  bool setMinimumIndoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumIndoorTemperatureSchedule();

  bool setMaximumIndoorTemperature(double maximumIndoorTemperature);

  bool setMaximumIndoorTemperatureSchedule(Schedule& schedule);

  void resetMaximumIndoorTemperatureSchedule();

  bool setDeltaTemperature(double deltaTemperature);

  bool setDeltaTemperatureSchedule(Schedule& schedule);

  void resetDeltaTemperatureSchedule();

  bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

  bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumOutdoorTemperatureSchedule();

  bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

  bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMaximumOutdoorTemperatureSchedule();

  bool setMaximumWindSpeed(double maximumWindSpeed);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneVentilationDesignFlowRate_Impl ImplType;

  explicit ZoneVentilationDesignFlowRate(std::shared_ptr<detail::ZoneVentilationDesignFlowRate_Impl> impl);

  friend class detail::ZoneVentilationDesignFlowRate_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneVentilationDesignFlowRate");
};

/** \relates ZoneVentilationDesignFlowRate*/
typedef boost::optional<ZoneVentilationDesignFlowRate> OptionalZoneVentilationDesignFlowRate;

/** \relates ZoneVentilationDesignFlowRate*/
typedef std::vector<ZoneVentilationDesignFlowRate> ZoneVentilationDesignFlowRateVector;

} // model
} // openstudio

#endif // MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP

