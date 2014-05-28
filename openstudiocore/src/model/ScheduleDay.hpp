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

#ifndef MODEL_SCHEDULEDAY_HPP
#define MODEL_SCHEDULEDAY_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase.hpp"

namespace openstudio {

class Time;
class OSQuantityVector;

namespace model {

class ScheduleTypeLimits;

namespace detail {

  class ScheduleDay_Impl;

} // detail

/** ScheduleDay is a ScheduleBase that wraps the OpenStudio IDD object 'OS_Schedule_Day'. */
class MODEL_API ScheduleDay : public ScheduleBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Initializes the schedule to a value of 0.0 until 24:00. */
  explicit ScheduleDay(const Model& model);

  virtual ~ScheduleDay() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Returns true if this object does not specify a ScheduleTypeLimits directly, but rather
   *  inherits one from a Schedule. */
  bool isScheduleTypeLimitsDefaulted() const;

  bool interpolatetoTimestep() const;

  bool isInterpolatetoTimestepDefaulted() const;

  /// Returns a vector of times marking the end value intervals.
  /// These times will be in order and have the same number of elements as values.
  /// All times will be less than or equal to 1 day.
  std::vector<openstudio::Time> times() const;

  /// Returns a vector of values in the same order and with the same number of elements as times.
  std::vector<double> values() const;

  /** Returns an OSQuantityVector in the same order and with the same number of elements as times.
   *  Otherwise (if units cannot be deduced), the return value will be .empty(). */
  OSQuantityVector getValues(bool returnIP=false) const;

  /// Returns the value in effect at the given time.  If time is less than 0 days or greater than 1 day, 0 is returned.
  double getValue(const openstudio::Time& time) const;

  /** Returns the value in effect at time, if possible. Otherwise, return value evaluates to
   *  false. */
  boost::optional<Quantity> getValueAsQuantity(const openstudio::Time& time, bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  void setInterpolatetoTimestep(bool interpolatetoTimestep);

  void resetInterpolatetoTimestep();

  /** Returns false if time is less than 0 days or greater than 1 day.  Replaces existing value
   *  for same time. */
  bool addValue(const openstudio::Time& untilTime, double value);

  /** Returns false if time is less than 0 days or greater than 1 day, or if there are no 
   *  scheduleTypeLimits() set, or if value cannot be converted to the 'SI' units implied by 
   *  scheduleTypeLimits(). Otherwise, converts value appropriately and then sets the value. */
  bool addValue(const openstudio::Time& untilTime, const Quantity& value);

  /** Remove a value added by addValue at the exact time.  Returns the removed
   *  value if there was one. */
  boost::optional<double> removeValue(const openstudio::Time& time);

  /// Clear all values from this schedule.
  void clearValues();

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleDay_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ScheduleDay_Impl;

  explicit ScheduleDay(std::shared_ptr<detail::ScheduleDay_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ScheduleDay");
};

/** \relates ScheduleDay*/
typedef boost::optional<ScheduleDay> OptionalScheduleDay;

/** \relates ScheduleDay*/
typedef std::vector<ScheduleDay> ScheduleDayVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEDAY_HPP

