/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;

  namespace detail {

    class SetpointManagerSingleZoneReheat_Impl;

  }

  /** SetpointManagerSingleZoneReheat is an interface to theIDD object
 *  named "OS:SetpointManager:SingleZone:Reheat"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:SingleZone:Reheat object.
 */
  class MODEL_API SetpointManagerSingleZoneReheat : public SetpointManager
  {

   public:
    virtual ~SetpointManagerSingleZoneReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneReheat(const SetpointManagerSingleZoneReheat& other) = default;
    SetpointManagerSingleZoneReheat(SetpointManagerSingleZoneReheat&& other) = default;
    SetpointManagerSingleZoneReheat& operator=(const SetpointManagerSingleZoneReheat&) = default;
    SetpointManagerSingleZoneReheat& operator=(SetpointManagerSingleZoneReheat&&) = default;

    /** Constructs a new SetpointManagerSingleZoneReheat object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
    explicit SetpointManagerSingleZoneReheat(const Model& model);

    /** Returns the value of the MinimumSupplyAirTemperature field. **/
    double minimumSupplyAirTemperature();

    /** Sets the value of the MinimumSupplyAirTemperature field. **/
    bool setMinimumSupplyAirTemperature(double value);

    /** Returns the value of the MaximumSupplyAirTemperature field. **/
    double maximumSupplyAirTemperature();

    /** Sets the value of the MaximumSupplyAirTemperature field. **/
    bool setMaximumSupplyAirTemperature(double value);

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    std::string controlVariable() const;

    bool setControlVariable(const std::string& controlVariable);

    boost::optional<ThermalZone> controlZone();

    bool setControlZone(ThermalZone& thermalZone);

    void resetControlZone();

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    friend class detail::SetpointManagerSingleZoneReheat_Impl;

    /// @cond

    using ImplType = detail::SetpointManagerSingleZoneReheat_Impl;

    explicit SetpointManagerSingleZoneReheat(std::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneReheat");

    /// @endcond
  };

  using OptionalSetpointManagerSingleZoneReheat = boost::optional<SetpointManagerSingleZoneReheat>;

  using SetpointManagerSingleZoneReheatVector = std::vector<SetpointManagerSingleZoneReheat>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP
