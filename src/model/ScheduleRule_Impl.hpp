/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~ScheduleRule_Impl() override = default;

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
