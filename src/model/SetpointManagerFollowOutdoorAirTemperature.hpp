/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SetpointManagerFollowOutdoorAirTemperature_Impl;

  }

  /** SetpointManagerFollowOutdoorAirTemperature is an SetpointManager that wraps the IDD object
 *  named "OS:SetpointManager:FollowOutdoorAirTemperature"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManagerFollowOutdoorAirTemperature object.
 */
  class MODEL_API SetpointManagerFollowOutdoorAirTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new SetpointManagerFollowOutdoorAirTemperature object and places it inside the
   *  model. */
    explicit SetpointManagerFollowOutdoorAirTemperature(const Model& model);

    virtual ~SetpointManagerFollowOutdoorAirTemperature() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerFollowOutdoorAirTemperature(const SetpointManagerFollowOutdoorAirTemperature& other) = default;
    SetpointManagerFollowOutdoorAirTemperature(SetpointManagerFollowOutdoorAirTemperature&& other) = default;
    SetpointManagerFollowOutdoorAirTemperature& operator=(const SetpointManagerFollowOutdoorAirTemperature&) = default;
    SetpointManagerFollowOutdoorAirTemperature& operator=(SetpointManagerFollowOutdoorAirTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters and Setters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& value);

    std::string referenceTemperatureType() const;

    bool setReferenceTemperatureType(const std::string& value);

    double offsetTemperatureDifference() const;

    bool setOffsetTemperatureDifference(double value);

    double maximumSetpointTemperature() const;

    bool setMaximumSetpointTemperature(double value);

    double minimumSetpointTemperature() const;

    bool setMinimumSetpointTemperature(double value);

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    friend class detail::SetpointManagerFollowOutdoorAirTemperature_Impl;

    /// @cond

    using ImplType = detail::SetpointManagerFollowOutdoorAirTemperature_Impl;

    explicit SetpointManagerFollowOutdoorAirTemperature(std::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerFollowOutdoorAirTemperature");

    /// @endcond
  };

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP
