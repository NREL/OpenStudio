/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULE_HPP
#define MODEL_SCHEDULE_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Schedule_Impl;
  }

  /** Schedule is a ScheduleBase abstract class that represents the reference list 'ScheduleNames'.
 *  The 'ScheduleNames' reference list includes objects of type:
 *     \li OS:Schedule:Year
 *     \li OS:Schedule:Compact
 *     \li OS:Schedule:Constant
 *     \li OS:Schedule:Ruleset
 *     \li OS:Schedule:FixedInterval
 *     \li OS:Schedule:VariableInterval
 *
 *  For detailed information on ScheduleTypeLimits, please see ScheduleBase and
 *  \link ScheduleTypeRegistrySingleton ScheduleTypeRegistry \endlink. In general, ModelObject
 *  setters that take a Schedule will fail if the ScheduleTypeLimits are set and are incompatible
 *  with the proposed use. This ensures that the Schedule is written using units and bounds
 *  expected by the user object (e.g. fractional for lighting schedules, W for people activity
 *  level schedules, C or F for thermostat schedules). */
  class MODEL_API Schedule : public ScheduleBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Schedule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Schedule(const Schedule& other) = default;
    Schedule(Schedule&& other) = default;
    Schedule& operator=(const Schedule&) = default;
    Schedule& operator=(Schedule&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::Schedule_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    // constructor
    explicit Schedule(std::shared_ptr<detail::Schedule_Impl> impl);

    Schedule(IddObjectType type, const Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Schedule");
  };

  // optional Schedule
  using OptionalSchedule = boost::optional<Schedule>;

  // vector of Schedule
  using ScheduleVector = std::vector<Schedule>;

}  // namespace model
}  // namespace openstudio

#endif
