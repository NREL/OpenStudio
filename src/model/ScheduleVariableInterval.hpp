/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEVARIABLEINTERVAL_HPP
#define MODEL_SCHEDULEVARIABLEINTERVAL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ScheduleVariableInterval_Impl;

  }  // namespace detail

  /** ScheduleVariableInterval is a ScheduleInterval that wraps the OpenStudio IDD object
 *  'OS_Schedule_VariableInterval'. */
  class MODEL_API ScheduleVariableInterval : public ScheduleInterval
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleVariableInterval(const Model& model);

    virtual ~ScheduleVariableInterval() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleVariableInterval(const ScheduleVariableInterval& other) = default;
    ScheduleVariableInterval(ScheduleVariableInterval&& other) = default;
    ScheduleVariableInterval& operator=(const ScheduleVariableInterval&) = default;
    ScheduleVariableInterval& operator=(ScheduleVariableInterval&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    double outOfRangeValue() const;

    bool isOutOfRangeValueDefaulted() const;

    int startMonth() const;

    int startDay() const;

    //@}
    /** @name Setters */
    //@{

    bool setInterpolatetoTimestep(bool interpolatetoTimestep);

    void resetInterpolatetoTimestep();

    bool setOutOfRangeValue(double outOfRangeValue);

    void resetOutOfRangeValue();

    bool setStartMonth(int startMonth);

    bool setStartDay(int startDay);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleVariableInterval_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ScheduleVariableInterval_Impl;

    explicit ScheduleVariableInterval(std::shared_ptr<detail::ScheduleVariableInterval_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleVariableInterval");
  };

  /** \relates ScheduleVariableInterval*/
  using OptionalScheduleVariableInterval = boost::optional<ScheduleVariableInterval>;

  /** \relates ScheduleVariableInterval*/
  using ScheduleVariableIntervalVector = std::vector<ScheduleVariableInterval>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEVARIABLEINTERVAL_HPP
