/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

    class MODEL_API RunPeriodControlSpecialDays_Impl : public ModelObject_Impl
    {
     public:
      // constructor
      RunPeriodControlSpecialDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      RunPeriodControlSpecialDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      RunPeriodControlSpecialDays_Impl(const RunPeriodControlSpecialDays_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~RunPeriodControlSpecialDays_Impl() override = default;

      Date startDate() const;
      unsigned duration() const;
      std::string specialDayType() const;

      bool setStartDate(const std::string& startDate);
      bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
      bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek,
                        const openstudio::MonthOfYear& monthOfYear);
      bool setDuration(unsigned duration);
      bool setSpecialDayType(const std::string& specialDayType);

      // ensure that this object does not contain the date 2/29
      void ensureNoLeapDays();

      // virtual methods
      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

     private:
      openstudio::Date getDate(const std::string& text) const;

      REGISTER_LOGGER("openstudio.model.RunPeriodControlSpecialDays");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIODCONTROLSPECIALDAYS_IMPL_HPP
