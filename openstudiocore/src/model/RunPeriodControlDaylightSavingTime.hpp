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

#ifndef MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP
#define MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {  

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  class RunPeriodControlDaylightSavingTime_Impl;

} // detail

/** RunPeriodControlDaylightSavingTime is a ModelObject that wraps the OpenStudio IDD object 'OS_RunPeriodControl_DaylightSavingTime'. */
class MODEL_API RunPeriodControlDaylightSavingTime : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~RunPeriodControlDaylightSavingTime() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  openstudio::Date startDate() const;

  boost::optional<openstudio::NthDayOfWeekInMonth> startNthDayOfWeekInMonth() const;

  openstudio::Date endDate() const;

  boost::optional<openstudio::NthDayOfWeekInMonth> endNthDayOfWeekInMonth() const;

  //@}
  /** @name Setters */
  //@{

  bool setStartDate(const std::string& startDate);
  bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
  bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);
  
  bool setEndDate(const std::string& endDate);
  bool setEndDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
  bool setEndDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);
  
  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

  //@}
 protected:
  /// @cond
  typedef detail::RunPeriodControlDaylightSavingTime_Impl ImplType;

  friend class detail::RunPeriodControlDaylightSavingTime_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit RunPeriodControlDaylightSavingTime(std::shared_ptr<detail::RunPeriodControlDaylightSavingTime_Impl> impl);

  explicit RunPeriodControlDaylightSavingTime(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.RunPeriodControlDaylightSavingTime");
};

/** \relates RunPeriodControlDaylightSavingTime*/
typedef boost::optional<RunPeriodControlDaylightSavingTime> OptionalRunPeriodControlDaylightSavingTime;

/** \relates RunPeriodControlDaylightSavingTime*/
typedef std::vector<RunPeriodControlDaylightSavingTime> RunPeriodControlDaylightSavingTimeVector;

} // model
} // openstudio

#endif // MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP

