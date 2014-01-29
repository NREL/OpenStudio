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

#ifndef MODEL_FANZONEEXHAUST_HPP
#define MODEL_FANZONEEXHAUST_HPP

#include <model/ModelAPI.hpp>
#include <model/ZoneHVACComponent.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Schedule;
class Schedule;
class Schedule;

namespace detail {

  class FanZoneExhaust_Impl;

} // detail

/** FanZoneExhaust is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:Fan:ZoneExhaust'. */
class MODEL_API FanZoneExhaust : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FanZoneExhaust(const Model& model);

  virtual ~FanZoneExhaust() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> systemAvailabilityManagerCouplingModeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  double fanEfficiency() const;

  double pressureRise() const;

  boost::optional<double> maximumFlowRate() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airOutletNode() const;

  std::string endUseSubcategory() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> flowFractionSchedule() const;

  std::string systemAvailabilityManagerCouplingMode() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> balancedExhaustFractionSchedule() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setFanEfficiency(double fanEfficiency);

  void setPressureRise(double pressureRise);

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirInletNode(const Connection& connection);

  void resetAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirOutletNode(const Connection& connection);

  void resetAirOutletNode();

  void setEndUseSubcategory(std::string endUseSubcategory);

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setFlowFractionSchedule(Schedule& schedule);

  void resetFlowFractionSchedule();

  bool setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode);

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

  void resetMinimumZoneTemperatureLimitSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setBalancedExhaustFractionSchedule(Schedule& schedule);

  void resetBalancedExhaustFractionSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FanZoneExhaust_Impl ImplType;

  explicit FanZoneExhaust(boost::shared_ptr<detail::FanZoneExhaust_Impl> impl);

  friend class detail::FanZoneExhaust_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
};

/** \relates FanZoneExhaust*/
typedef boost::optional<FanZoneExhaust> OptionalFanZoneExhaust;

/** \relates FanZoneExhaust*/
typedef std::vector<FanZoneExhaust> FanZoneExhaustVector;

} // model
} // openstudio

#endif // MODEL_FANZONEEXHAUST_HPP

