/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API RunPeriodControlDaylightSavingTime_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RunPeriodControlDaylightSavingTime_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RunPeriodControlDaylightSavingTime_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RunPeriodControlDaylightSavingTime_Impl(const RunPeriodControlDaylightSavingTime_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RunPeriodControlDaylightSavingTime_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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
      bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek,
                        const openstudio::MonthOfYear& monthOfYear);

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_IMPL_HPP
