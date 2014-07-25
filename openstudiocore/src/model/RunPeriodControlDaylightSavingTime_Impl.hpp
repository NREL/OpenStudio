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

#ifndef MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_IMPL_HPP
#define MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
  
class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  /** RunPeriodControlDaylightSavingTime_Impl is a ModelObject_Impl that is the implementation class for RunPeriodControlDaylightSavingTime.*/
  class MODEL_API RunPeriodControlDaylightSavingTime_Impl : public ModelObject_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    RunPeriodControlDaylightSavingTime_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    RunPeriodControlDaylightSavingTime_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    RunPeriodControlDaylightSavingTime_Impl(const RunPeriodControlDaylightSavingTime_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~RunPeriodControlDaylightSavingTime_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const;

    virtual bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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
    private:

    openstudio::Date getDate(const std::string& text) const;

    boost::optional<openstudio::NthDayOfWeekInMonth> getNthDayOfWeekInMonth(const std::string& text) const;

    REGISTER_LOGGER("openstudio.model.RunPeriodControlDaylightSavingTime");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_IMPL_HPP

