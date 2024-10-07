/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  }  // namespace detail

  /** YearDescription is a ParentObject that wraps the OpenStudio IDD object 'OS_YearDescription'. */
  class MODEL_API YearDescription : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~YearDescription() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    YearDescription(const YearDescription& other) = default;
    YearDescription(YearDescription&& other) = default;
    YearDescription& operator=(const YearDescription&) = default;
    YearDescription& operator=(YearDescription&&) = default;

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

    bool setCalendarYear(int calendarYear);

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
    /// @cond
    using ImplType = detail::YearDescription_Impl;

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
  using OptionalYearDescription = boost::optional<YearDescription>;

  /** \relates YearDescription*/
  using YearDescriptionVector = std::vector<YearDescription>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_YEARDESCRIPTION_HPP
