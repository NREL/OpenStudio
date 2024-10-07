/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSCHEDULED_HPP
#define MODEL_SETPOINTMANAGERSCHEDULED_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SetpointManagerScheduled_Impl;

  }

  /** SetpointManagerScheduled is an interface to theIDD object
 *  named "OS:SetpointManager:Scheduled"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:Scheduled object.
 */
  class MODEL_API SetpointManagerScheduled : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new SetpointManagerScheduled object and places it inside
   *  model. Sets controlVariable to 'Temperature'. Sets setpointSchedule's
   *  ScheduleTypeLimits if necessary and possible. */
    SetpointManagerScheduled(const Model& model, Schedule& setpointSchedule);

    /** Constructs a new SetpointManagerScheduled object and places it inside
   *  model. */
    SetpointManagerScheduled(const Model& model, const std::string& controlVariable, Schedule& setpointSchedule);

    virtual ~SetpointManagerScheduled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerScheduled(const SetpointManagerScheduled& other) = default;
    SetpointManagerScheduled(SetpointManagerScheduled&& other) = default;
    SetpointManagerScheduled& operator=(const SetpointManagerScheduled&) = default;
    SetpointManagerScheduled& operator=(SetpointManagerScheduled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Returns the Schedule **/
    Schedule schedule() const;

    /** Returns true if this object has a schedule, it is an error if this object does not have a schedule **/
    bool hasSchedule() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& controlVariable);

    /** Sets the Schedule **/
    bool setSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    /** Sets control variable and schedule simultaneously. Use in the case that the
   *  control variable change implies a change in schedule units. */
    bool setControlVariableAndSchedule(const std::string& controlVariable, Schedule& schedule);

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::SetpointManagerScheduled_Impl;

    /// @cond
    using ImplType = detail::SetpointManagerScheduled_Impl;

    explicit SetpointManagerScheduled(std::shared_ptr<detail::SetpointManagerScheduled_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerScheduled");
    /// @endcond
  };

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULED_HPP
