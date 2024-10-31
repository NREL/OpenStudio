/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API YearDescription_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      YearDescription_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      YearDescription_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      YearDescription_Impl(const YearDescription_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~YearDescription_Impl() override = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

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

      bool setCalendarYear(boost::optional<int> calendarYear);

      void resetCalendarYear();

      bool setDayofWeekforStartDay(const std::string& dayofWeekforStartDay);

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_YEARDESCRIPTION_IMPL_HPP
