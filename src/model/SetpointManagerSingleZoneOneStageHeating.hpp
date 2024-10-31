/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEONESTAGEHEATING_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEONESTAGEHEATING_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneOneStageHeating_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneOneStageHeating is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:OneStageHeating'. */
  class MODEL_API SetpointManagerSingleZoneOneStageHeating : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneOneStageHeating(const Model& model);

    virtual ~SetpointManagerSingleZoneOneStageHeating() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneOneStageHeating(const SetpointManagerSingleZoneOneStageHeating& other) = default;
    SetpointManagerSingleZoneOneStageHeating(SetpointManagerSingleZoneOneStageHeating&& other) = default;
    SetpointManagerSingleZoneOneStageHeating& operator=(const SetpointManagerSingleZoneOneStageHeating&) = default;
    SetpointManagerSingleZoneOneStageHeating& operator=(SetpointManagerSingleZoneOneStageHeating&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    double heatingStageOnSupplyAirSetpointTemperature() const;

    double heatingStageOffSupplyAirSetpointTemperature() const;

    boost::optional<ThermalZone> controlZone() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setHeatingStageOnSupplyAirSetpointTemperature(double heatingStageOnSupplyAirSetpointTemperature);

    bool setHeatingStageOffSupplyAirSetpointTemperature(double heatingStageOffSupplyAirSetpointTemperature);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneOneStageHeating_Impl;

    explicit SetpointManagerSingleZoneOneStageHeating(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageHeating_Impl> impl);

    friend class detail::SetpointManagerSingleZoneOneStageHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneOneStageHeating");
  };

  /** \relates SetpointManagerSingleZoneOneStageHeating*/
  using OptionalSetpointManagerSingleZoneOneStageHeating = boost::optional<SetpointManagerSingleZoneOneStageHeating>;

  /** \relates SetpointManagerSingleZoneOneStageHeating*/
  using SetpointManagerSingleZoneOneStageHeatingVector = std::vector<SetpointManagerSingleZoneOneStageHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEONESTAGEHEATING_HPP
