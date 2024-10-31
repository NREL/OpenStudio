/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP
#define MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    class SetpointManagerScheduledDualSetpoint_Impl;

  }  // namespace detail

  /** SetpointManagerScheduledDualSetpoint is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:Scheduled:DualSetpoint'. */
  class MODEL_API SetpointManagerScheduledDualSetpoint : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerScheduledDualSetpoint(const Model& model);

    virtual ~SetpointManagerScheduledDualSetpoint() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerScheduledDualSetpoint(const SetpointManagerScheduledDualSetpoint& other) = default;
    SetpointManagerScheduledDualSetpoint(SetpointManagerScheduledDualSetpoint&& other) = default;
    SetpointManagerScheduledDualSetpoint& operator=(const SetpointManagerScheduledDualSetpoint&) = default;
    SetpointManagerScheduledDualSetpoint& operator=(SetpointManagerScheduledDualSetpoint&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> highSetpointSchedule() const;

    boost::optional<Schedule> lowSetpointSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHighSetpointSchedule(Schedule& schedule);

    void resetHighSetpointSchedule();

    bool setLowSetpointSchedule(Schedule& schedule);

    void resetLowSetpointSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerScheduledDualSetpoint_Impl;

    explicit SetpointManagerScheduledDualSetpoint(std::shared_ptr<detail::SetpointManagerScheduledDualSetpoint_Impl> impl);

    friend class detail::SetpointManagerScheduledDualSetpoint_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerScheduledDualSetpoint");
  };

  /** \relates SetpointManagerScheduledDualSetpoint*/
  using OptionalSetpointManagerScheduledDualSetpoint = boost::optional<SetpointManagerScheduledDualSetpoint>;

  /** \relates SetpointManagerScheduledDualSetpoint*/
  using SetpointManagerScheduledDualSetpointVector = std::vector<SetpointManagerScheduledDualSetpoint>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP
