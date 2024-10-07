/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMIXEDAIR_HPP
#define MODEL_SETPOINTMANAGERMIXEDAIR_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"
#include <boost/optional.hpp>

namespace openstudio {

namespace model {

  namespace detail {

    class SetpointManagerMixedAir_Impl;

  }

  /** SetpointManagerMixedAir is an interface to the IDD object
 *  named "OS:SetpointManager:MixedAir"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:MixedAir object.
 */
  class MODEL_API SetpointManagerMixedAir : public SetpointManager
  {
   public:
    virtual ~SetpointManagerMixedAir() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMixedAir(const SetpointManagerMixedAir& other) = default;
    SetpointManagerMixedAir(SetpointManagerMixedAir&& other) = default;
    SetpointManagerMixedAir& operator=(const SetpointManagerMixedAir&) = default;
    SetpointManagerMixedAir& operator=(SetpointManagerMixedAir&&) = default;

    /** Constructs a new SetpointManagerMixedAir object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
    explicit SetpointManagerMixedAir(const Model& model);

    /** Returns the value of the ControlVariable field. **/
    std::string controlVariable() const;

    /** Sets the value of the ControlVariable field.
   *  Options are Temperature.
   */
    bool setControlVariable(const std::string& controlVariable);

    /** Returns the Node referred to by the referenceSetpointNodeName. **/
    boost::optional<Node> referenceSetpointNode();

    /** Sets the Node referred to by the referenceSetpointNodeName. **/
    bool setReferenceSetpointNode(Node& node);

    /** Returns the Node referred to by the fanInletNodeName. **/
    boost::optional<Node> fanInletNode();

    /** Sets the Node referred to by the fanInletNodeName. **/
    bool setFanInletNode(Node& node);

    /** Returns the Node referred to by the fanOutletNodeName. **/
    boost::optional<Node> fanOutletNode();

    /** Sets the Node referred to by the fanOutletNodeName. **/
    bool setFanOutletNode(Node& node);

    /** Returns the Node this setpoint manager is acting on. **/
    boost::optional<Node> setpointNode() const;

    static IddObjectType iddObjectType();

    static void updateFanInletOutletNodes(AirLoopHVAC& airLoopHVAC);

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    friend class detail::SetpointManagerMixedAir_Impl;

    /// @cond

    using ImplType = detail::SetpointManagerMixedAir_Impl;

    explicit SetpointManagerMixedAir(std::shared_ptr<detail::SetpointManagerMixedAir_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMixedAir");

    /// @endcond
  };

  using OptionalSetpointManagerMixedAir = boost::optional<SetpointManagerMixedAir>;

  using SetpointManagerMixedAirVector = std::vector<SetpointManagerMixedAir>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMIXEDAIR_HPP
