/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHEATING_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHEATING_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneHeating_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneHeating is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Heating'. */
  class MODEL_API SetpointManagerSingleZoneHeating : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneHeating(const Model& model);

    virtual ~SetpointManagerSingleZoneHeating() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneHeating(const SetpointManagerSingleZoneHeating& other) = default;
    SetpointManagerSingleZoneHeating(SetpointManagerSingleZoneHeating&& other) = default;
    SetpointManagerSingleZoneHeating& operator=(const SetpointManagerSingleZoneHeating&) = default;
    SetpointManagerSingleZoneHeating& operator=(SetpointManagerSingleZoneHeating&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double minimumSupplyAirTemperature() const;

    double maximumSupplyAirTemperature() const;

    boost::optional<ThermalZone> controlZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature);

    bool setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneHeating_Impl;

    explicit SetpointManagerSingleZoneHeating(std::shared_ptr<detail::SetpointManagerSingleZoneHeating_Impl> impl);

    friend class detail::SetpointManagerSingleZoneHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHeating");
  };

  /** \relates SetpointManagerSingleZoneHeating*/
  using OptionalSetpointManagerSingleZoneHeating = boost::optional<SetpointManagerSingleZoneHeating>;

  /** \relates SetpointManagerSingleZoneHeating*/
  using SetpointManagerSingleZoneHeatingVector = std::vector<SetpointManagerSingleZoneHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHEATING_HPP
