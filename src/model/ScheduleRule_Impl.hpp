/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
    class MODEL_API ScheduleRule_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleRule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleRule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleRule_Impl(const ScheduleRule_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleRule_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual std::vector<ResourceObject> resources() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      // Note: there is no applyHoliday, instead that's implemented at the ScheduleRule's level

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

      bool setApplySunday(bool applySunday);

      bool setApplyMonday(bool applyMonday);

      bool setApplyTuesday(bool applyTuesday);

      bool setApplyWednesday(bool applyWednesday);

      bool setApplyThursday(bool applyThursday);

      bool setApplyFriday(bool applyFriday);

      bool setApplySaturday(bool applySaturday);

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

      // Convenience methods
      bool applyAllDays() const;
      bool applyWeekdays() const;
      bool applyWeekends() const;

      bool setApplyAllDays(bool applyAllDays);
      bool setApplyWeekdays(bool applyWeekdays);
      bool setApplyWeekends(bool applyWeekends);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleRule");

      boost::optional<ScheduleDay> optionalDaySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULERULE_IMPL_HPP
