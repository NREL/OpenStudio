/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULED_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULED_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class AvailabilityManagerScheduled_Impl;

  }  // namespace detail

  /** AvailabilityManagerScheduled is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:Scheduled'. */
  class MODEL_API AvailabilityManagerScheduled : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerScheduled(const Model& model);

    virtual ~AvailabilityManagerScheduled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerScheduled(const AvailabilityManagerScheduled& other) = default;
    AvailabilityManagerScheduled(AvailabilityManagerScheduled&& other) = default;
    AvailabilityManagerScheduled& operator=(const AvailabilityManagerScheduled&) = default;
    AvailabilityManagerScheduled& operator=(AvailabilityManagerScheduled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule schedule() const;

    //@}
    /** @name Setters */
    //@{

    /*
   * Get the Schedule, if not set, defaults to alwaysOnDiscreteSchedule
   */
    bool setSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerScheduled_Impl;

    explicit AvailabilityManagerScheduled(std::shared_ptr<detail::AvailabilityManagerScheduled_Impl> impl);

    friend class detail::AvailabilityManagerScheduled_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduled");
  };

  /** \relates AvailabilityManagerScheduled*/
  using OptionalAvailabilityManagerScheduled = boost::optional<AvailabilityManagerScheduled>;

  /** \relates AvailabilityManagerScheduled*/
  using AvailabilityManagerScheduledVector = std::vector<AvailabilityManagerScheduled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERSCHEDULED_HPP
