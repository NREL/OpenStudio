/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class AvailabilityManagerScheduledOff_Impl;

  }  // namespace detail

  /** AvailabilityManagerScheduledOff is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:ScheduledOff'. */
  class MODEL_API AvailabilityManagerScheduledOff : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerScheduledOff(const Model& model);

    virtual ~AvailabilityManagerScheduledOff() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerScheduledOff(const AvailabilityManagerScheduledOff& other) = default;
    AvailabilityManagerScheduledOff(AvailabilityManagerScheduledOff&& other) = default;
    AvailabilityManagerScheduledOff& operator=(const AvailabilityManagerScheduledOff&) = default;
    AvailabilityManagerScheduledOff& operator=(AvailabilityManagerScheduledOff&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule schedule() const;

    //@}
    /** @name Setters */
    //@{

    /*
   * Get the Schedule, if not set, defaults to alwaysOffDiscreteSchedule
   */
    bool setSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerScheduledOff_Impl;

    explicit AvailabilityManagerScheduledOff(std::shared_ptr<detail::AvailabilityManagerScheduledOff_Impl> impl);

    friend class detail::AvailabilityManagerScheduledOff_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduledOff");
  };

  /** \relates AvailabilityManagerScheduledOff*/
  using OptionalAvailabilityManagerScheduledOff = boost::optional<AvailabilityManagerScheduledOff>;

  /** \relates AvailabilityManagerScheduledOff*/
  using AvailabilityManagerScheduledOffVector = std::vector<AvailabilityManagerScheduledOff>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_HPP
