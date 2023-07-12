/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "RunPeriodControlDaylightSavingTime.hpp"
#include "RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "RunPeriodControlSpecialDays.hpp"
#include "RunPeriodControlSpecialDays_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "ScheduleBase.hpp"
#include "ScheduleBase_Impl.hpp"
#include "ScheduleRule.hpp"
#include "ScheduleRule_Impl.hpp"
#include "LightingDesignDay.hpp"
#include "LightingDesignDay_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_YearDescription_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Date.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    YearDescription_Impl::YearDescription_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == YearDescription::iddObjectType());
    }

    YearDescription_Impl::YearDescription_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == YearDescription::iddObjectType());
    }

    YearDescription_Impl::YearDescription_Impl(const YearDescription_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& YearDescription_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType YearDescription_Impl::iddObjectType() const {
      return YearDescription::iddObjectType();
    }

    std::vector<ModelObject> YearDescription_Impl::children() const {
      std::vector<ModelObject> result;
      Model model = this->model();

      boost::optional<RunPeriodControlDaylightSavingTime> dst = model.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>();
      if (dst) {
        result.push_back(*dst);
      }

      auto specialDays = model.getConcreteModelObjects<RunPeriodControlSpecialDays>();
      result.insert(result.end(), specialDays.begin(), specialDays.end());

      return result;
    }

    std::vector<IddObjectType> YearDescription_Impl::allowableChildTypes() const {
      return IddObjectTypeVector{
        RunPeriodControlDaylightSavingTime::iddObjectType(),
        RunPeriodControlSpecialDays::iddObjectType(),
      };
    }

    boost::optional<int> YearDescription_Impl::calendarYear() const {
      return getInt(OS_YearDescriptionFields::CalendarYear, true);
    }

    std::string YearDescription_Impl::dayofWeekforStartDay() const {

      boost::optional<int> calendarYear = this->calendarYear();
      if (calendarYear) {
        openstudio::Date jan1(MonthOfYear::Jan, 1, *calendarYear);
        std::string result = jan1.dayOfWeek().valueName();
        return result;
      }

      boost::optional<std::string> value = getString(OS_YearDescriptionFields::DayofWeekforStartDay, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool YearDescription_Impl::isDayofWeekforStartDayDefaulted() const {
      return isEmpty(OS_YearDescriptionFields::DayofWeekforStartDay);
    }

    bool YearDescription_Impl::isLeapYear() const {

      boost::optional<int> calendarYear = this->calendarYear();
      if (calendarYear) {
        openstudio::Date jan1(MonthOfYear::Jan, 1, *calendarYear);
        bool result = jan1.isLeapYear();
        return result;
      }

      boost::optional<std::string> value = getString(OS_YearDescriptionFields::IsLeapYear, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool YearDescription_Impl::isIsLeapYearDefaulted() const {
      return isEmpty(OS_YearDescriptionFields::IsLeapYear);
    }

    bool YearDescription_Impl::setCalendarYear(boost::optional<int> calendarYear) {
      bool wasLeapYear = this->isLeapYear();

      bool result = false;
      if (calendarYear) {
        result = setInt(OS_YearDescriptionFields::CalendarYear, calendarYear.get());
        this->resetDayofWeekforStartDay();
        this->resetIsLeapYear();
      } else {
        result = setString(OS_YearDescriptionFields::CalendarYear, "");
      }
      OS_ASSERT(result);

      bool isLeapYear = this->isLeapYear();
      // TODO: should really capture the output of updateModelLeapYear here...
      updateModelLeapYear(wasLeapYear, isLeapYear);
      return result;
    }

    void YearDescription_Impl::resetCalendarYear() {
      bool wasLeapYear = this->isLeapYear();

      bool result = setString(OS_YearDescriptionFields::CalendarYear, "");
      OS_ASSERT(result);

      bool isLeapYear = this->isLeapYear();
      updateModelLeapYear(wasLeapYear, isLeapYear);
    }

    bool YearDescription_Impl::setDayofWeekforStartDay(const std::string& dayofWeekforStartDay) {
      bool result = false;
      if (!this->calendarYear()) {
        result = setString(OS_YearDescriptionFields::DayofWeekforStartDay, dayofWeekforStartDay);
      }
      return result;
    }

    void YearDescription_Impl::resetDayofWeekforStartDay() {
      bool result = setString(OS_YearDescriptionFields::DayofWeekforStartDay, "");
      OS_ASSERT(result);
    }

    bool YearDescription_Impl::setIsLeapYear(bool isLeapYear) {
      bool result = false;
      bool wasLeapYear = this->isLeapYear();

      if (!this->calendarYear()) {
        if (isLeapYear) {
          result = setString(OS_YearDescriptionFields::IsLeapYear, "Yes");
        } else {
          result = setString(OS_YearDescriptionFields::IsLeapYear, "No");
        }
      }

      if (result) {
        updateModelLeapYear(wasLeapYear, isLeapYear);
      }

      return result;
    }

    void YearDescription_Impl::resetIsLeapYear() {
      bool wasLeapYear = this->isLeapYear();

      bool result = setString(OS_YearDescriptionFields::IsLeapYear, "");
      OS_ASSERT(result);

      bool isLeapYear = this->isLeapYear();
      updateModelLeapYear(wasLeapYear, isLeapYear);
    }

    int YearDescription_Impl::assumedYear() const {
      boost::optional<int> calendarYear = this->calendarYear();
      if (calendarYear) {
        return *calendarYear;
      }

      openstudio::YearDescription yd;

      yd.isLeapYear = this->isLeapYear();

      std::string dayofWeekforStartDay = this->dayofWeekforStartDay();
      if (!dayofWeekforStartDay.empty()) {
        if (istringEqual(dayofWeekforStartDay, "UseWeatherFile")) {
          if (auto weatherFile_ = this->model().weatherFile()) {
            if (auto epwStartYear = weatherFile_->startDateActualYear()) {
              LOG(Info, "YearDescription::assumedYear: using the WeatherFile startDateActualYear = " << epwStartYear.get());
              return epwStartYear.get();
            } else if (boost::optional<openstudio::DayOfWeek> dow_ = weatherFile_->startDayOfWeek()) {
              auto dow = dow_.get();
              LOG(Info, "assumedYear: using the WeatherFile startDayOfWeek = " << dow);
              yd.yearStartsOnDayOfWeek = dow;
            } else {
              LOG(Error, "'UseWeatherFile' is selected in YearDescription"
                           << " but the WeatherFile has neither an actual start year nor a Start Day of Week");
            }
          } else {
            LOG(Error, "'UseWeatherFile' is selected in YearDescription, but there are no weather file set for the model.");
          }
        } else {
          openstudio::DayOfWeek dow(dayofWeekforStartDay);
          yd.yearStartsOnDayOfWeek = dow;
        }
      }

      return yd.assumedYear();
    }

    openstudio::Date YearDescription_Impl::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth) {
      boost::optional<int> year = this->calendarYear();
      if (!year) {
        year = this->assumedYear();
      }

      return {monthOfYear, dayOfMonth, *year};
    }

    openstudio::Date YearDescription_Impl::makeDate(unsigned monthOfYear, unsigned dayOfMonth) {
      return makeDate(openstudio::MonthOfYear(monthOfYear), dayOfMonth);
    }

    openstudio::Date YearDescription_Impl::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek,
                                                    openstudio::MonthOfYear monthOfYear) {
      boost::optional<int> year = this->calendarYear();
      if (!year) {
        year = this->assumedYear();
      }

      return openstudio::Date::fromNthDayOfMonth(n, dayOfWeek, monthOfYear, *year);
    }

    openstudio::Date YearDescription_Impl::makeDate(unsigned dayOfYear) {
      boost::optional<int> year = this->calendarYear();
      if (!year) {
        year = this->assumedYear();
      }

      return openstudio::Date::fromDayOfYear(dayOfYear, *year);
    }

    void YearDescription_Impl::updateModelLeapYear(bool wasLeapYear, bool isLeapYear) {
      if (wasLeapYear == isLeapYear) {
        return;
      }

      if (!wasLeapYear && isLeapYear) {
        return;
      }

      model::Model model = this->model();
      if (wasLeapYear && !isLeapYear) {
        for (RunPeriod& runPeriod : model.getConcreteModelObjects<RunPeriod>()) {
          runPeriod.ensureNoLeapDays();
        }

        for (RunPeriodControlDaylightSavingTime& runPeriodControlDaylightSavingTime :
             model.getConcreteModelObjects<RunPeriodControlDaylightSavingTime>()) {
          runPeriodControlDaylightSavingTime.ensureNoLeapDays();
        }

        for (RunPeriodControlSpecialDays& runPeriodControlSpecialDays : model.getConcreteModelObjects<RunPeriodControlSpecialDays>()) {
          runPeriodControlSpecialDays.ensureNoLeapDays();
        }

        for (SizingPeriod& sizingPeriod : model.getModelObjects<SizingPeriod>()) {
          sizingPeriod.ensureNoLeapDays();
        }

        for (ScheduleBase& scheduleBase : model.getModelObjects<ScheduleBase>()) {
          scheduleBase.ensureNoLeapDays();
        }

        for (ScheduleRule& scheduleRule : model.getConcreteModelObjects<ScheduleRule>()) {
          scheduleRule.ensureNoLeapDays();
        }

        for (LightingDesignDay& lightingDesignDay : model.getConcreteModelObjects<LightingDesignDay>()) {
          lightingDesignDay.ensureNoLeapDays();
        }
      }
    }

  }  // namespace detail

  IddObjectType YearDescription::iddObjectType() {
    IddObjectType result(IddObjectType::OS_YearDescription);
    return result;
  }

  std::vector<std::string> YearDescription::validDayofWeekforStartDayValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_YearDescriptionFields::DayofWeekforStartDay);
  }

  boost::optional<int> YearDescription::calendarYear() const {
    return getImpl<detail::YearDescription_Impl>()->calendarYear();
  }

  std::string YearDescription::dayofWeekforStartDay() const {
    return getImpl<detail::YearDescription_Impl>()->dayofWeekforStartDay();
  }

  bool YearDescription::isDayofWeekforStartDayDefaulted() const {
    return getImpl<detail::YearDescription_Impl>()->isDayofWeekforStartDayDefaulted();
  }

  bool YearDescription::isLeapYear() const {
    return getImpl<detail::YearDescription_Impl>()->isLeapYear();
  }

  bool YearDescription::isIsLeapYearDefaulted() const {
    return getImpl<detail::YearDescription_Impl>()->isIsLeapYearDefaulted();
  }

  bool YearDescription::setCalendarYear(int calendarYear) {
    return getImpl<detail::YearDescription_Impl>()->setCalendarYear(calendarYear);
  }

  void YearDescription::resetCalendarYear() {
    getImpl<detail::YearDescription_Impl>()->resetCalendarYear();
  }

  bool YearDescription::setDayofWeekforStartDay(const std::string& dayofWeekforStartDay) {
    return getImpl<detail::YearDescription_Impl>()->setDayofWeekforStartDay(dayofWeekforStartDay);
  }

  void YearDescription::resetDayofWeekforStartDay() {
    getImpl<detail::YearDescription_Impl>()->resetDayofWeekforStartDay();
  }

  bool YearDescription::setIsLeapYear(bool isLeapYear) {
    return getImpl<detail::YearDescription_Impl>()->setIsLeapYear(isLeapYear);
  }

  void YearDescription::resetIsLeapYear() {
    getImpl<detail::YearDescription_Impl>()->resetIsLeapYear();
  }

  int YearDescription::assumedYear() const {
    return getImpl<detail::YearDescription_Impl>()->assumedYear();
  }

  openstudio::Date YearDescription::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth) {
    return getImpl<detail::YearDescription_Impl>()->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date YearDescription::makeDate(unsigned monthOfYear, unsigned dayOfMonth) {
    return getImpl<detail::YearDescription_Impl>()->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date YearDescription::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek,
                                             openstudio::MonthOfYear monthOfYear) {
    return getImpl<detail::YearDescription_Impl>()->makeDate(n, dayOfWeek, monthOfYear);
  }

  openstudio::Date YearDescription::makeDate(unsigned dayOfYear) {
    return getImpl<detail::YearDescription_Impl>()->makeDate(dayOfYear);
  }

  /// @cond
  YearDescription::YearDescription(std::shared_ptr<detail::YearDescription_Impl> impl) : ParentObject(std::move(impl)) {}
  YearDescription::YearDescription(Model& model) : ParentObject(YearDescription::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
