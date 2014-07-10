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

#ifndef MODEL_YEARDESCRIPTION_IMPL_HPP
#define MODEL_YEARDESCRIPTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  /** YearDescription_Impl is a ParentObject_Impl that is the implementation class for YearDescription.*/
  class MODEL_API YearDescription_Impl : public ParentObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<int> calendarYear READ calendarYear WRITE setCalendarYear RESET resetCalendarYear);
    Q_PROPERTY(std::string dayofWeekforStartDay READ dayofWeekforStartDay WRITE setDayofWeekforStartDay RESET resetDayofWeekforStartDay);
    Q_PROPERTY(bool isDayofWeekforStartDayDefaulted READ isDayofWeekforStartDayDefaulted);
    Q_PROPERTY(bool isLeapYear READ isLeapYear WRITE setIsLeapYear RESET resetIsLeapYear);
    Q_PROPERTY(bool isIsLeapYearDefaulted READ isIsLeapYearDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    YearDescription_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    YearDescription_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    YearDescription_Impl(const YearDescription_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    virtual ~YearDescription_Impl() {}

    //@}
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ModelObject> children() const;

    virtual std::vector<IddObjectType> allowableChildTypes() const;

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

    void setCalendarYear(boost::optional<int> calendarYear);

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
   private:

    void updateModelLeapYear(bool wasLeapYear, bool isLeapYear);

    REGISTER_LOGGER("openstudio.model.YearDescription");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_YEARDESCRIPTION_IMPL_HPP

