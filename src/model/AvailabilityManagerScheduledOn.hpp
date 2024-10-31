/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULEDON_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULEDON_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class AvailabilityManagerScheduledOn_Impl;

  }  // namespace detail

  /** AvailabilityManagerScheduledOn is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:ScheduledOn'. */
  class MODEL_API AvailabilityManagerScheduledOn : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerScheduledOn(const Model& model);

    virtual ~AvailabilityManagerScheduledOn() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerScheduledOn(const AvailabilityManagerScheduledOn& other) = default;
    AvailabilityManagerScheduledOn(AvailabilityManagerScheduledOn&& other) = default;
    AvailabilityManagerScheduledOn& operator=(const AvailabilityManagerScheduledOn&) = default;
    AvailabilityManagerScheduledOn& operator=(AvailabilityManagerScheduledOn&&) = default;

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
    using ImplType = detail::AvailabilityManagerScheduledOn_Impl;

    explicit AvailabilityManagerScheduledOn(std::shared_ptr<detail::AvailabilityManagerScheduledOn_Impl> impl);

    friend class detail::AvailabilityManagerScheduledOn_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduledOn");
  };

  /** \relates AvailabilityManagerScheduledOn*/
  using OptionalAvailabilityManagerScheduledOn = boost::optional<AvailabilityManagerScheduledOn>;

  /** \relates AvailabilityManagerScheduledOn*/
  using AvailabilityManagerScheduledOnVector = std::vector<AvailabilityManagerScheduledOn>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERSCHEDULEDON_HPP
