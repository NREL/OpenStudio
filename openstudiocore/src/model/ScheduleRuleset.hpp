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

#ifndef MODEL_SCHEDULERULESET_HPP
#define MODEL_SCHEDULERULESET_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class Date;

namespace model {

class ScheduleDay;
class ScheduleRule;

namespace detail {

  class ScheduleRuleset_Impl;
  class ScheduleRule_Impl;

} // detail

/** ScheduleRuleset is a Schedule that wraps the OpenStudio IDD object 'OS_Schedule_Ruleset'. 
 *  ScheduleRuleset is the parent of its default day schedule, any non-default summer or winter 
 *  design day schedules, and its \link ScheduleRule ScheduleRules\endlink. */
class MODEL_API ScheduleRuleset : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new ScheduleRuleset, and also constructs a new ScheduleDay and assigns it to 
   *  be the defaultDaySchedule(). */
  explicit ScheduleRuleset(const Model& model);

  virtual ~ScheduleRuleset() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /// Returns the default day schedule.
  ScheduleDay defaultDaySchedule() const;

  /// Returns the summer design day schedule, which may be the same ScheduleDay as 
  /// defaultDaySchedule().
  ScheduleDay summerDesignDaySchedule() const;

  /// Returns true if the summer design day is defaulted; false if it is explicitly set.
  bool isSummerDesignDayScheduleDefaulted() const;

  /// Returns the winter design day schedule, which may be the same ScheduleDay as 
  /// defaultDaySchedule().
  ScheduleDay winterDesignDaySchedule() const;

  /// Returns true if the winter design day is defaulted; false if it is explicitly set.
  bool isWinterDesignDayScheduleDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets the summer design day schedule if ScheduleTypeLimits are compatible. Calls remove 
  /// on any existing non-default summer design day schedule. Clones schedule and parents the 
  /// clone, but does not call remove on the original schedule.
  bool setSummerDesignDaySchedule(const ScheduleDay& schedule);
  
  /// Resets the summer design day schedule. Calls remove on any existing non-default summer
  /// design day schedule. 
  void resetSummerDesignDaySchedule();

  /// Sets the winter design day schedule if ScheduleTypeLimits are compatible. Calls remove 
  /// on any existing non-default winter design day schedule. Clones schedule and parents the 
  /// clone, but does not call remove on the original schedule.
  bool setWinterDesignDaySchedule(const ScheduleDay& schedule);
  
  /// Resets the winter design day schedule. Calls remove on any existing non-default winter
  /// design day schedule. 
  void resetWinterDesignDaySchedule();

  //@}
  /** @name Other */
  //@{

  /// Returns a vector of the rules associated with this schedule, this may be empty.
  /// The rules returned are in order from highest priority to lowest priority.
  std::vector<ScheduleRule> scheduleRules() const;

  /// Sets the index of the given rule; 0 is the highest priority.
  /// Returns false if this rule is not a child of this rule set or if index > number of 
  /// rules - 1.
  bool setScheduleRuleIndex(ScheduleRule& scheduleRule, unsigned index);

  /// Returns a vector of indices into the result of scheduleRules() between start date 
  /// (inclusive) and end date (inclusive). If no rule is in place on a given day then -1 is 
  /// returned.
  std::vector<int> getActiveRuleIndices(const openstudio::Date& startDate, 
                                        const openstudio::Date& endDate) const;

  /// Returns a vector of day schedules between start date (inclusive) and end date (inclusive).
  std::vector<ScheduleDay> getDaySchedules(const openstudio::Date& startDate, 
                                           const openstudio::Date& endDate) const;

  //@}
 protected:

  friend class ScheduleRule;
  friend class detail::ScheduleRule_Impl;

  // Moves this rule to the last position. Called in ScheduleRule remove.
  bool moveToEnd(ScheduleRule& scheduleRule);

  /// @cond
  typedef detail::ScheduleRuleset_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ScheduleRuleset_Impl;

  explicit ScheduleRuleset(std::shared_ptr<detail::ScheduleRuleset_Impl> impl);
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleRuleset");
};

/** \relates ScheduleRuleset*/
typedef boost::optional<ScheduleRuleset> OptionalScheduleRuleset;

/** \relates ScheduleRuleset*/
typedef std::vector<ScheduleRuleset> ScheduleRulesetVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULERULESET_HPP

