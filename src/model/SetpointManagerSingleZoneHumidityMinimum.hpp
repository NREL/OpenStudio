/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {
namespace model {

  class Node;
  class ThermalZone;

  namespace detail {

    class SetpointManagerSingleZoneHumidityMinimum_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneHumidityMinimum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Humidity:Minimum'. */
  class MODEL_API SetpointManagerSingleZoneHumidityMinimum : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneHumidityMinimum(const Model& model);

    virtual ~SetpointManagerSingleZoneHumidityMinimum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneHumidityMinimum(const SetpointManagerSingleZoneHumidityMinimum& other) = default;
    SetpointManagerSingleZoneHumidityMinimum(SetpointManagerSingleZoneHumidityMinimum&& other) = default;
    SetpointManagerSingleZoneHumidityMinimum& operator=(const SetpointManagerSingleZoneHumidityMinimum&) = default;
    SetpointManagerSingleZoneHumidityMinimum& operator=(SetpointManagerSingleZoneHumidityMinimum&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    boost::optional<Node> setpointNode() const;

    boost::optional<ThermalZone> controlZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneHumidityMinimum_Impl;

    explicit SetpointManagerSingleZoneHumidityMinimum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl);

    friend class detail::SetpointManagerSingleZoneHumidityMinimum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMinimum");
  };

  /** \relates SetpointManagerSingleZoneHumidityMinimum*/
  using OptionalSetpointManagerSingleZoneHumidityMinimum = boost::optional<SetpointManagerSingleZoneHumidityMinimum>;

  /** \relates SetpointManagerSingleZoneHumidityMinimum*/
  using SetpointManagerSingleZoneHumidityMinimumVector = std::vector<SetpointManagerSingleZoneHumidityMinimum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP
