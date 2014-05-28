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

#ifndef MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP
#define MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {
  class RunPeriodControlSpecialDays_Impl;
} // detail


/** RunPeriodControlSpecialDays derives from ModelObject and is an interface to the OpenStudio IDD object named "OS:RunPeriodControl:SpecialDays".
 */
class MODEL_API RunPeriodControlSpecialDays : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Constructor with string, will throw if incorrect format. */
  RunPeriodControlSpecialDays(const std::string& startDate, Model& model);

  /** Constructor with month of year and day, e.g. Jan, 1. */
  RunPeriodControlSpecialDays(const openstudio::MonthOfYear& monthOfYear, unsigned day, Model& model);

  /** Constructor from nth day in month, e.g. 1st Friday in January. */
  RunPeriodControlSpecialDays(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear, Model& model);

  virtual ~RunPeriodControlSpecialDays() {}
  
  //@}
  /** @name Getters */
  //@{

  openstudio::Date startDate() const;
  unsigned duration() const;
  std::string specialDayType() const;

  //@}
  /** @name Setters */
  //@{

  bool setStartDate(const std::string& startDate);
  bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
  bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);
  bool setDuration(unsigned duration);
  bool setSpecialDayType(const std::string& specialDayType);

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

 protected:

  /// @cond

  typedef detail::RunPeriodControlSpecialDays_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit RunPeriodControlSpecialDays(std::shared_ptr<detail::RunPeriodControlSpecialDays_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.RunPeriodControlSpecialDays");

  /// @endcond

};

/** \relates RunPeriodControlSpecialDays */
typedef boost::optional<RunPeriodControlSpecialDays> OptionalRunPeriodControlSpecialDays;

/** \relates RunPeriodControlSpecialDays */
typedef std::vector<RunPeriodControlSpecialDays> RunPeriodControlSpecialDaysVector;

} // model
} // openstudio

#endif // MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP
