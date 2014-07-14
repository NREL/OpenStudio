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

#ifndef MODEL_SCHEDULERULE_HPP
#define MODEL_SCHEDULERULE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Date;

namespace model {

class ScheduleDay;
class ScheduleRuleset;

namespace detail {

  class ScheduleRule_Impl;
  class ScheduleRuleset_Impl;

} // detail

/** ScheduleRule is a ParentObject that wraps the OpenStudio IDD object 'OS:Schedule:Rule'. */
class MODEL_API ScheduleRule : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /// Constructor associates this rule with the given ruleset.  This rule is made to be the highest priority rule.
  /// A new ScheduleDay is created which is owned by (a child of) this rule.
  explicit ScheduleRule(ScheduleRuleset& scheduleRuleset);

  /// Constructor associates this rule with the given ruleset. This rule is made to be the highest priority rule.
  /// The given daySchedule is cloned and the new object is owned by (a child of) this rule.
  explicit ScheduleRule(ScheduleRuleset& scheduleRuleset, const ScheduleDay& daySchedule);

  virtual ~ScheduleRule() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validDateSpecificationTypeValues();

  /** @name Getters */
  //@{
  
  ScheduleRuleset scheduleRuleset() const;

  int ruleIndex() const;

  ScheduleDay daySchedule() const;

  bool applySunday() const;

  bool applyMonday() const;

  bool applyTuesday() const;

  bool applyWednesday() const;

  bool applyThursday() const;

  bool applyFriday() const;

  bool applySaturday() const;

  //bool applyHoliday() const;

  std::string dateSpecificationType() const;

  /// Returns the start date of the date range if set.
  /// A start date > end date indicates that the range wraps around the year.
  boost::optional<openstudio::Date> startDate() const;

  /// Returns the end date of the date range if set.
  /// A start date > end date indicates that the range wraps around the year.
  boost::optional<openstudio::Date> endDate() const;

  /// Returns the specific dates specified if any.
  std::vector<openstudio::Date> specificDates() const;

  //@}
  /** @name Setters */
  //@{

  void setApplySunday(bool applySunday);

  void setApplyMonday(bool applyMonday);

  void setApplyTuesday(bool applyTuesday);

  void setApplyWednesday(bool applyWednesday);

  void setApplyThursday(bool applyThursday);

  void setApplyFriday(bool applyFriday);

  void setApplySaturday(bool applySaturday);

  //void setApplyHoliday(bool applyHoliday);

  /// Sets the start date of the date range, will delete any specific dates specified.
  /// A start date > end date indicates that the range wraps around the year.
  bool setStartDate(const openstudio::Date& date);

  /// Sets the end date of the date range, will delete any specific dates specified.
  /// A start date > end date indicates that the range wraps around the year.
  bool setEndDate(const openstudio::Date& date);

  /// Adds a specific date, will delete start and end dates.
  bool addSpecificDate(const openstudio::Date& date);

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

  //@}
  /** @name Other */
  //@{

  /// Returns true if this rule covers the given date.
  bool containsDate(const openstudio::Date& date);

  /// Returns whether or not this rule covers each given date.
  std::vector<bool> containsDates(const std::vector<openstudio::Date>& dates);
  
  //@}
 protected:

  friend class detail::ScheduleRuleset_Impl;
  bool setRuleIndex(int index);

  /// @cond
  typedef detail::ScheduleRule_Impl ImplType;

  explicit ScheduleRule(std::shared_ptr<detail::ScheduleRule_Impl> impl);

  friend class detail::ScheduleRule_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ScheduleRule");
};

/** \relates ScheduleRule */
typedef boost::optional<ScheduleRule> OptionalScheduleRule;

/** \relates ScheduleRule */
typedef std::vector<ScheduleRule> ScheduleRuleVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULERULE_HPP

