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

#ifndef MODEL_RUNPERIODCONTROLSPECIALDAYS_IMPL_HPP
#define MODEL_RUNPERIODCONTROLSPECIALDAYS_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  class MODEL_API RunPeriodControlSpecialDays_Impl : public ModelObject_Impl {
   public:

    // constructor
    RunPeriodControlSpecialDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    RunPeriodControlSpecialDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    // clone copy constructor
    RunPeriodControlSpecialDays_Impl(const RunPeriodControlSpecialDays_Impl& other,Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~RunPeriodControlSpecialDays_Impl(){}

    Date startDate() const;
    unsigned duration() const;
    std::string specialDayType() const;

    bool setStartDate(const std::string& startDate);
    bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
    bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);
    bool setDuration(unsigned duration);
    bool setSpecialDayType(const std::string& specialDayType);

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    // virtual methods
    virtual boost::optional<ParentObject> parent() const;

    virtual bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

   private:

    openstudio::Date getDate(const std::string& text) const;

    REGISTER_LOGGER("openstudio.model.RunPeriodControlSpecialDays");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_RUNPERIODCONTROLSPECIALDAYS_IMPL_HPP
