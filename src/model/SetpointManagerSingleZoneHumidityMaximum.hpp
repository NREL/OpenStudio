/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneHumidityMaximum_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneHumidityMaximum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Humidity:Maximum'. */
  class MODEL_API SetpointManagerSingleZoneHumidityMaximum : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneHumidityMaximum(const Model& model);

    virtual ~SetpointManagerSingleZoneHumidityMaximum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneHumidityMaximum(const SetpointManagerSingleZoneHumidityMaximum& other) = default;
    SetpointManagerSingleZoneHumidityMaximum(SetpointManagerSingleZoneHumidityMaximum&& other) = default;
    SetpointManagerSingleZoneHumidityMaximum& operator=(const SetpointManagerSingleZoneHumidityMaximum&) = default;
    SetpointManagerSingleZoneHumidityMaximum& operator=(SetpointManagerSingleZoneHumidityMaximum&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    boost::optional<ThermalZone> controlZone() const;

    boost::optional<Node> setpointNode() const;

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
    using ImplType = detail::SetpointManagerSingleZoneHumidityMaximum_Impl;

    explicit SetpointManagerSingleZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMaximum_Impl> impl);

    friend class detail::SetpointManagerSingleZoneHumidityMaximum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMaximum");
  };

  /** \relates SetpointManagerSingleZoneHumidityMaximum*/
  using OptionalSetpointManagerSingleZoneHumidityMaximum = boost::optional<SetpointManagerSingleZoneHumidityMaximum>;

  /** \relates SetpointManagerSingleZoneHumidityMaximum*/
  using SetpointManagerSingleZoneHumidityMaximumVector = std::vector<SetpointManagerSingleZoneHumidityMaximum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP
