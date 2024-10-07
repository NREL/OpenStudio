/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGER_HPP
#define MODEL_SETPOINTMANAGER_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class SetpointManager_Impl;

  }

  /** SetpointManager is the base class for setpoint managers.
 *
 */
  class MODEL_API SetpointManager : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SetpointManager() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManager(const SetpointManager& other) = default;
    SetpointManager(SetpointManager&& other) = default;
    SetpointManager& operator=(const SetpointManager&) = default;
    SetpointManager& operator=(SetpointManager&&) = default;

    //@}

    /** @name Getters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Returns whether this SPM is allowed to be placed on a PlantLoop
   * (*all* SPMs are allowed on an AirLoopHVAC, *some* are allowed on a PlantLoop) **/
    bool isAllowedOnPlantLoop() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& controlVariable);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /** Constructs a new SetpointManager object and places it inside
   *  model. */
    SetpointManager(IddObjectType type, const Model& model);

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::SetpointManager_Impl;

    /// @cond
    using ImplType = detail::SetpointManager_Impl;

    explicit SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManager");
    /// @endcond
  };

  using OptionalSetpointManager = boost::optional<SetpointManager>;
  using SetpointManagerVector = std::vector<SetpointManager>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGER_HPP
