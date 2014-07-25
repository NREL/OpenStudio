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

#ifndef MODEL_YEARDESCRIPTION_HPP
#define MODEL_YEARDESCRIPTION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  class YearDescription_Impl;

} // detail

/** YearDescription is a ParentObject that wraps the OpenStudio IDD object 'OS_YearDescription'. */
class MODEL_API YearDescription : public ParentObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~YearDescription() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validDayofWeekforStartDayValues();

  //@}
  /** @name Getters */
  //@{

  boost::optional<int> calendarYear() const;

  std::string dayofWeekforStartDay() const;

  bool isDayofWeekforStartDayDefaulted() const;

  bool isLeapYear() const;

  bool isIsLeapYearDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setCalendarYear(int calendarYear);

  void resetCalendarYear();

  bool setDayofWeekforStartDay(std::string dayofWeekforStartDay);

  void resetDayofWeekforStartDay();

  bool setIsLeapYear(bool isLeapYear);

  void resetIsLeapYear();

  //@}

  /// Returns assumed year for date calculations.
  int assumedYear() const;

  /// Returns a date which may be used in this model.
  /// Will throw if dayOfMonth is not appropriate for monthOfYear.
  openstudio::Date makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth);
  openstudio::Date makeDate(unsigned monthOfYear, unsigned dayOfMonth);
    
  /// nth weekday of month generators
  openstudio::Date makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear);
  
  /// day of year
  openstudio::Date makeDate(unsigned dayOfYear);

 protected:
  /// @cond
  typedef detail::YearDescription_Impl ImplType;

  friend class Model;
  friend class IdfObject;
  friend class detail::YearDescription_Impl;

  explicit YearDescription(std::shared_ptr<detail::YearDescription_Impl> impl);

  explicit YearDescription(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.YearDescription");
};

/** \relates YearDescription*/
typedef boost::optional<YearDescription> OptionalYearDescription;

/** \relates YearDescription*/
typedef std::vector<YearDescription> YearDescriptionVector;

} // model
} // openstudio

#endif // MODEL_YEARDESCRIPTION_HPP

