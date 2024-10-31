/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEINTERVAL_HPP
#define MODEL_SCHEDULEINTERVAL_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

  namespace detail {
    class ScheduleInterval_Impl;
  }  // namespace detail

  /** ScheduleInterval is a Schedule that serves as an abstract base class for ScheduleFixedInterval
 *  and ScheduleVariableInterval. ScheduleIntervals may be constructed from TimeSeries, and can return
 *  their values as TimeSeries. */
  class MODEL_API ScheduleInterval : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ScheduleInterval() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleInterval(const ScheduleInterval& other) = default;
    ScheduleInterval(ScheduleInterval&& other) = default;
    ScheduleInterval& operator=(const ScheduleInterval&) = default;
    ScheduleInterval& operator=(ScheduleInterval&&) = default;

    //@}

    static boost::optional<ScheduleInterval> fromTimeSeries(const openstudio::TimeSeries& timeSeries, Model& model);

    /** @name Getters */
    //@{

    openstudio::TimeSeries timeSeries() const;

    //@}
    /** @name Setters */
    //@{

    bool setTimeSeries(const openstudio::TimeSeries& timeSeries);

    //@}
   protected:
    using ImplType = detail::ScheduleInterval_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    ScheduleInterval(IddObjectType type, const Model& model);

    // constructor
    explicit ScheduleInterval(std::shared_ptr<detail::ScheduleInterval_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleInterval");
  };

  /** \relates ScheduleInterval */
  using OptionalScheduleInterval = boost::optional<ScheduleInterval>;

  /** \relates ScheduleInterval */
  using ScheduleIntervalVector = std::vector<ScheduleInterval>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEINTERVAL_HPP
