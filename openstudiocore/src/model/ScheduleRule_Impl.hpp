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

#ifndef MODEL_SCHEDULERULE_IMPL_HPP
#define MODEL_SCHEDULERULE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {

class Date;

namespace model {

class ScheduleRuleset;
class ScheduleDay;

namespace detail {

  /** ScheduleRule_Impl is a ParentObject_Impl that is the implementation class for ScheduleRule.*/
  class MODEL_API ScheduleRule_Impl : public ParentObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(int ruleIndex READ ruleIndex WRITE setRuleIndex);
    Q_PROPERTY(bool applySunday READ applySunday WRITE setApplySunday);
    Q_PROPERTY(bool applyMonday READ applyMonday WRITE setApplyMonday);
    Q_PROPERTY(bool applyTuesday READ applyTuesday WRITE setApplyTuesday);
    Q_PROPERTY(bool applyWednesday READ applyWednesday WRITE setApplyWednesday);
    Q_PROPERTY(bool applyThursday READ applyThursday WRITE setApplyThursday);
    Q_PROPERTY(bool applyFriday READ applyFriday WRITE setApplyFriday);
    Q_PROPERTY(bool applySaturday READ applySaturday WRITE setApplySaturday);
    //Q_PROPERTY(bool applyHoliday READ applyHoliday WRITE setApplyHoliday);
    Q_PROPERTY(std::string dateSpecificationType READ dateSpecificationType);
   public:
    /** @name Constructors and Destructors */
    //@{

    ScheduleRule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleRule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    ScheduleRule_Impl(const ScheduleRule_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~ScheduleRule_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    virtual bool setParent(ParentObject& newParent);

    virtual std::vector<ResourceObject> resources() const;
 
    virtual std::vector<ModelObject> children() const;    

    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
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

    bool setRuleIndex(int index);

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

    /// Returns true if this rule covers the given date.
    bool containsDate(const openstudio::Date& date);

    /// Returns whether or not this rule covers each given date.
    std::vector<bool> containsDates(const std::vector<openstudio::Date>& dates);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleRule");

    boost::optional<ScheduleDay> optionalDaySchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULERULE_IMPL_HPP

