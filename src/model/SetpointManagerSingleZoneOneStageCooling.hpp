/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneOneStageCooling_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneOneStageCooling is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:OneStageCooling'. */
  class MODEL_API SetpointManagerSingleZoneOneStageCooling : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneOneStageCooling(const Model& model);

    virtual ~SetpointManagerSingleZoneOneStageCooling() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneOneStageCooling(const SetpointManagerSingleZoneOneStageCooling& other) = default;
    SetpointManagerSingleZoneOneStageCooling(SetpointManagerSingleZoneOneStageCooling&& other) = default;
    SetpointManagerSingleZoneOneStageCooling& operator=(const SetpointManagerSingleZoneOneStageCooling&) = default;
    SetpointManagerSingleZoneOneStageCooling& operator=(SetpointManagerSingleZoneOneStageCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    double coolingStageOnSupplyAirSetpointTemperature() const;

    double coolingStageOffSupplyAirSetpointTemperature() const;

    boost::optional<ThermalZone> controlZone() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature);

    bool setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneOneStageCooling_Impl;

    explicit SetpointManagerSingleZoneOneStageCooling(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageCooling_Impl> impl);

    friend class detail::SetpointManagerSingleZoneOneStageCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneOneStageCooling");
  };

  /** \relates SetpointManagerSingleZoneOneStageCooling*/
  using OptionalSetpointManagerSingleZoneOneStageCooling = boost::optional<SetpointManagerSingleZoneOneStageCooling>;

  /** \relates SetpointManagerSingleZoneOneStageCooling*/
  using SetpointManagerSingleZoneOneStageCoolingVector = std::vector<SetpointManagerSingleZoneOneStageCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP
