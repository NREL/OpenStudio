/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULECOMPACT_HPP
#define MODEL_SCHEDULECOMPACT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ScheduleCompact_Impl;
  }  // namespace detail

  /** ScheduleCompact is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Compact'. */
  class MODEL_API ScheduleCompact : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Creates an empty (no values set for any days) ScheduleCompact. */
    explicit ScheduleCompact(const Model& model);

    /** Creates a ScheduleCompact with constantValue applied to the entire year. */
    ScheduleCompact(const Model& model, double constantValue);

    /** Creates a ScheduleCompact with constantValue applied to the entire year, if
   *  scheduleTypeLimits() and the units are compatible. Otherwise creates an empty
   *  ScheduleCompact. */
    virtual ~ScheduleCompact() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleCompact(const ScheduleCompact& other) = default;
    ScheduleCompact(ScheduleCompact&& other) = default;
    ScheduleCompact& operator=(const ScheduleCompact&) = default;
    ScheduleCompact& operator=(ScheduleCompact&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Setters */
    //@{

    /** Clears all existing data and sets the data so that value will be applied to the entire
   *  year. */
    bool setToConstantValue(double value);

    /** If value can be converted to scheduleTypeLimit()'s units, then all existing data is cleared
   *  and value is set (after unit conversion, if necessary). */

    //@}
    /** @name Queries */
    //@{

    /** Returns true if this ScheduleCompact has a single value applied to the entire year.
   *  (Roughly--the text fields are not checked.) */
    bool isConstantValue() const;

    /** Returns this ScheduleCompact's constant value, if this ScheduleCompact conforms to that
   *  pattern. Otherwise returns boost::none (evaluates to false). */
    boost::optional<double> constantValue() const;

    //@}
   protected:
    using ImplType = detail::ScheduleCompact_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit ScheduleCompact(std::shared_ptr<detail::ScheduleCompact_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleCompact");
  };

  /** \relates ScheduleCompact */
  using OptionalScheduleCompact = boost::optional<ScheduleCompact>;

  /** \relates ScheduleCompact */
  using ScheduleCompactVector = std::vector<ScheduleCompact>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECOMPACT_HPP
