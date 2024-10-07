/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEWEEK_HPP
#define MODEL_SCHEDULEWEEK_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ScheduleDay;

  namespace detail {

    class ScheduleWeek_Impl;

  }  // namespace detail

  /** ScheduleWeek is a ResourceObject that wraps the OpenStudio IDD object
 *  'OS_Schedule_Week'. */
  class MODEL_API ScheduleWeek : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleWeek(const Model& model);

    virtual ~ScheduleWeek() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleWeek(const ScheduleWeek& other) = default;
    ScheduleWeek(ScheduleWeek&& other) = default;
    ScheduleWeek& operator=(const ScheduleWeek&) = default;
    ScheduleWeek& operator=(ScheduleWeek&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ScheduleDay> sundaySchedule() const;

    boost::optional<ScheduleDay> mondaySchedule() const;

    boost::optional<ScheduleDay> tuesdaySchedule() const;

    boost::optional<ScheduleDay> wednesdaySchedule() const;

    boost::optional<ScheduleDay> thursdaySchedule() const;

    boost::optional<ScheduleDay> fridaySchedule() const;

    boost::optional<ScheduleDay> saturdaySchedule() const;

    boost::optional<ScheduleDay> holidaySchedule() const;

    boost::optional<ScheduleDay> summerDesignDaySchedule() const;

    boost::optional<ScheduleDay> winterDesignDaySchedule() const;

    boost::optional<ScheduleDay> customDay1Schedule() const;

    boost::optional<ScheduleDay> customDay2Schedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSundaySchedule(const ScheduleDay& schedule);

    bool setMondaySchedule(const ScheduleDay& schedule);

    bool setTuesdaySchedule(const ScheduleDay& schedule);

    bool setWednesdaySchedule(const ScheduleDay& schedule);

    bool setThursdaySchedule(const ScheduleDay& schedule);

    bool setFridaySchedule(const ScheduleDay& schedule);

    bool setSaturdaySchedule(const ScheduleDay& schedule);

    bool setHolidaySchedule(const ScheduleDay& schedule);

    bool setSummerDesignDaySchedule(const ScheduleDay& schedule);

    bool setWinterDesignDaySchedule(const ScheduleDay& schedule);

    bool setCustomDay1Schedule(const ScheduleDay& schedule);

    bool setCustomDay2Schedule(const ScheduleDay& schedule);

    //@}
    /** @name Other */
    //@{

    /// Set schedules for all days.
    bool setAllSchedules(const ScheduleDay& schedule);

    /// Set schedules for all weekdays.
    bool setWeekdaySchedule(const ScheduleDay& schedule);

    /// Set schedules for all weekends.
    bool setWeekendSchedule(const ScheduleDay& schedule);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleWeek_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ScheduleWeek_Impl;

    explicit ScheduleWeek(std::shared_ptr<detail::ScheduleWeek_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleWeek");
  };

  /** \relates ScheduleWeek*/
  using OptionalScheduleWeek = boost::optional<ScheduleWeek>;

  /** \relates ScheduleWeek*/
  using ScheduleWeekVector = std::vector<ScheduleWeek>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEWEEK_HPP
